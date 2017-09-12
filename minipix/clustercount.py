import argparse
import pdb
import subprocess
import sys
import traceback

from itertools import islice
from math import sqrt
from pprint import pprint

import numpy as np

# Width and height of minipix detector
DATA_FRAME_WIDTH = 256
DATA_FRAME_HEIGHT = 256


# Quick way of determining line count of a file
def file_len(fname):
    p = subprocess.Popen(['wc', '-l', fname], stdout=subprocess.PIPE,
                         stderr=subprocess.PIPE)
    result, err = p.communicate()
    if p.returncode != 0:
        raise IOError(err)
    return int(result.strip().split()[0])


# Describes an individual pixel from a minipix detector
# This should be expanded upon later
class Pixel:
    def __init__(self, value, dim, indices):
        self.filled = False
        self.value = value
        self.dim_x, self.dim_y = dim
        self.x, self.y = indices

    # Return all surrounding pixels that are in bounds
    def surrounding_pixels(self):
        pixels = []

        if self._inbounds(self.x, self.y - 1):
            pixels.append((self.x, self.y - 1))
        if self._inbounds(self.x, self.y + 1):
            pixels.append((self.x, self.y + 1))
        if self._inbounds(self.x - 1, self.y):
            pixels.append((self.x - 1, self.y))
        if self._inbounds(self.x + 1, self.y):
            pixels.append((self.x + 1, self.y))
        if self._inbounds(self.x - 1, self.y + 1):
            pixels.append((self.x - 1, self.y + 1))
        if self._inbounds(self.x + 1, self.y + 1):
            pixels.append((self.x + 1, self.y + 1))
        if self._inbounds(self.x - 1, self.y - 1):
            pixels.append((self.x - 1, self.y - 1))
        if self._inbounds(self.x + 1, self.y - 1):
            pixels.append((self.x + 1, self.y - 1))

        return pixels

    def hit(self):
        return self.value > 0

    def _inbounds(self, x, y):
        if x > self.dim_x - 1 or x < 0:
            return False
        if y > self.dim_y - 1 or y < 0:
            return False

        return True


# Describes minipix acquisition file
class PmfFile:
    def __init__(self, filename):
        num_lines = file_len(filename)
        self.filename = filename
        self.num_frames = int(num_lines / DATA_FRAME_HEIGHT)

        self.a = None
        self.b = None
        self.c = None
        self.t = None

    def get_frame_raw(self, frame):

        pmf_file = open(self.filename, "r")

        if frame > self.num_frames or frame < 0:
            raise IndexError("Frame index out of range")

        start = frame * DATA_FRAME_HEIGHT
        end = (frame * DATA_FRAME_HEIGHT) + DATA_FRAME_HEIGHT

        lines = list(islice(pmf_file, start, end))

        return lines

    def get_frame(self, frame):

        lines = self.get_frame_raw(frame)
        pmf_data = []

        for y, line in enumerate(lines):
            row_vals = []

            for x, row_val in enumerate(line.split()):
                row_vals.append(Pixel(int(row_val), (DATA_FRAME_WIDTH, DATA_FRAME_HEIGHT), (x, y % 256)))

            pmf_data.append(row_vals)

        return pmf_data

    @staticmethod
    def frame2nparray(frame):
        array = np.ones((DATA_FRAME_HEIGHT, DATA_FRAME_WIDTH), dtype=float)
        for i, line in enumerate(frame):
            for j, value in enumerate(line.split()):
                array[i][j] = float(value)
        return array

    def get_total_energy(self, pixels):
        total_energy = 0

        for pixel in pixels:
            energy = 0
            x, y, tot = pixel

            a = self.a[y][x]
            t = self.t[y][x]
            b = self.b[y][x] - a * t - self.c[y][x]
            c = t * tot - self.b[y][x] * t - self.c[y][x]

            if a != 0 and (b * b - 4.0 * a * c) >= 0:
                energy = (b * -1 + sqrt(b * b - 4.0 * a * c)) / 2.0 / a
                if energy < 0:
                    energy = 0
            total_energy += energy
        return energy

    def get_frame_e(self, frame):
        calib_data = [self.a, self.b, self.c, self.t]

        if not all(calib_data):
            raise Exception("Not all of the calibration files have been loaded, cannot generate e")
        else:
            ToT = frame2nparray(self.get_frame_raw(frame))
            a, b, c, t = calib_data

            return self._get_energy(ToT, a, b, c, t)

    # Generator for frames
    def frames(self):
        for i in range(self.num_frames):
            yield self.get_frame(i)

    def _get_energy(self, ToT, a, b, c, d):
        # Need to fully understand what equation to use to calculate this
        raise NotImplemented

    def load_calib_a(self, filename):
        with open(filename, 'r') as a:
            file_a = a.readlines()
            self.a = self.frame2nparray(file_a)

    def load_calib_b(self, filename):
        with open(filename, 'r') as b:
            file_b = b.readlines()
            self.b = self.frame2nparray(file_b)

    def load_calib_c(self, filename):
        with open(filename, 'r') as c:
            file_c = c.readlines()
            self.c = self.frame2nparray(file_c)

    def load_calib_t(self, filename):
        with open(filename, 'r') as t:
            file_t = t.readlines()
            self.t = self.frame2nparray(file_t)


def is_inner_pixel(index, arr):
    px, py, _ = index

    neighbors = arr[py][px].surrounding_pixels()
    count = 0

    for pixel in neighbors:
        x, y = pixel
        if arr[y][x].hit():
            count += 1
    return count > 4


# Iterative floodfill (Python doesn't optimize tail recursion)
# list of pixel indices and their corresponding values
def floodfill(x, y, arr, threshold=0):
    to_fill = set()
    to_fill.add((x, y))

    cluster_pixels = []

    while not len(to_fill) == 0:
        x, y = to_fill.pop()

        pixel = arr[y][x]
        pixel.filled = True

        cluster_pixels.append((x, y, pixel.value))

        for x, y in pixel.surrounding_pixels():
            if arr[y][x].value > threshold and not arr[y][x].filled:
                to_fill.add((x, y))

    return cluster_pixels


def distance(a, b):
    x = a[0] - b[0]
    y = a[1] - b[1]
    z = a[2] - b[2]

    return sqrt(x ** 2 + y ** 2 + z ** 2)


def medioid(pixels):
    # Brute force solution, could probably use memoization here
    pixel_dict = {}

    for pixel in pixels:
        sum = 0
        for other_pixel in pixels:
            sum += distance(pixel, other_pixel)
        pixel_dict[pixel] = sum

    minimum = min(pixel_dict, key=pixel_dict.get)

    x, y, w = minimum
    return x + 1, y + 1, w


def centroid(pixels):
    cx, cy, tw = (0, 0, 0)

    for i, pixel in enumerate(pixels, 1):
        x, y, w = pixel
        cx += x * w
        cy += y * w
        tw += w

    cx /= tw
    cy /= tw
    avg_cluster_value = tw / i

    return int(cx + 1), int(cy + 1), avg_cluster_value


# Determines the number of clusters given a single frame of acquisition data
def cluster_count(data, frame, threshold=0):
    clusters = 0
    cluster_info = []

    for row in frame:
        for pixel in row:
            if pixel.value > threshold and not pixel.filled:
                pixels = floodfill(pixel.x, pixel.y, frame)
                total_energy = data.get_total_energy(pixels)
                inner_pixels = list(filter(lambda x: is_inner_pixel(x, frame), pixels))
                max_p = max(pixels, key=lambda x: x[2])

                cluster_info.append((clusters,
                                     len(inner_pixels),
                                     total_energy,
                                     max_p,
                                     medioid(pixels),
                                     centroid(pixels)))
                clusters += 1

    return cluster_info


def main(args):
    data = PmfFile(args.filename)
    threshold = int(args.threshold)

    data.load_calib_a("a.txt")
    data.load_calib_b("b.txt")
    data.load_calib_c("c.txt")
    data.load_calib_t("t.txt")

    #print("Processing {} frames...".format(data.num_frames))

    for i, frame in enumerate(data.frames()):
        energy = 0
        for cluster in cluster_count(data, frame, threshold=threshold):
            _, _, total_energy, _, _, _ = cluster
            energy += total_energy
        print(energy)





if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Determine the cluster count for each frame in a pmf acquisition file")
    parser.add_argument('filename', help='Pmf file to process')
    parser.add_argument('-t',
                        action='store',
                        dest='threshold',
                        default=1,
                        help='Threshold')
    args = parser.parse_args()

    try:
        main(args)
    # Drop into shell on failure for postmortem debugging
    except Exception:
        _, _, tb = sys.exc_info()
        traceback.print_exc()
        pdb.post_mortem(tb)
