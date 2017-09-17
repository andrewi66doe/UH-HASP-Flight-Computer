import argparse
import pdb
import subprocess
import sys
import traceback
import math

from itertools import islice
from math import sqrt
from pprint import pprint

import numpy as np

# Width and height of minipix detector
DATA_FRAME_WIDTH = 256
DATA_FRAME_HEIGHT = 256

SMALL_BLOB = "SMALL_BLOB"
HEAVY_TRACK = "HEAVY_TRACK"
HEAVY_BLOB = "HEAVY_BLOB"
MEDIUM_BLOB = "MEDIUM_BLOB"
STRAIGHT_TRACK = "STRAIGHT_TRACK"
LIGHT_TRACK = "LIGHT_TRACK"


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
        energy = 0

        for pixel in pixels:
            energy = 0
            x, y, tot = pixel

            A = self.a[y][x]
            T = self.t[y][x]
            B = self.b[y][x] - A * T - self.c[y][x]
            C = T * tot - self.b[y][x] * T - self.c[y][x]

            if A != 0 and (B * B - 4.0 * A * C) >= 0:
                energy = ((B * -1) + sqrt(B * B - 4.0 * A * C)) / 2.0 / A
                if energy < 0:
                    energy = 0
            total_energy += energy

        return energy

    def calib_loaded(self):
        calib_data = [self.a, self.b, self.c, self.t]

        if not calib_data.all():
            raise Exception("Not all of the calibration files have been loaded, cannot generate e")

    def get_frame_e(self, frame):
        self.calib_loaded()

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


# Begin code stolen from the internet
# https://github.com/dbworth/minimum-area-bounding-rectangle

# Copyright (c) 2013, David Butterworth, University of Queensland
# All rights reserved.


link = lambda a, b: np.concatenate((a, b[1:]))
edge = lambda a, b: np.concatenate(([a], [b]))


def qhull2d(sample):
    def dome(sample, base):
        h, t = base
        dists = np.dot(sample - h, np.dot(((0, -1), (1, 0)), (t - h)))
        outer = np.repeat(sample, dists > 0, 0)
        if len(outer):
            pivot = sample[np.argmax(dists)]
            return link(dome(outer, edge(h, pivot)),
                        dome(outer, edge(pivot, t)))
        else:
            return base

    if len(sample) > 2:
        axis = sample[:, 0]
        base = np.take(sample, [np.argmin(axis), np.argmax(axis)], 0)
        return link(dome(sample, base), dome(sample, base[::-1]))
    else:
        return sample


def min_bounding_rect(hull_points_2d):
    # Compute edges (x2-x1,y2-y1)
    edges = np.zeros((len(hull_points_2d) - 1, 2))  # empty 2 column array
    for i in range(len(edges)):
        edge_x = hull_points_2d[i + 1, 0] - hull_points_2d[i, 0]
        edge_y = hull_points_2d[i + 1, 1] - hull_points_2d[i, 1]
        edges[i] = [edge_x, edge_y]
    # print "Edges: \n", edges

    # Calculate edge angles   atan2(y/x)
    edge_angles = np.zeros((len(edges)))  # empty 1 column array
    for i in range(len(edge_angles)):
        edge_angles[i] = math.atan2(edges[i, 1], edges[i, 0])
    # print "Edge angles: \n", edge_angles

    # Check for angles in 1st quadrant
    for i in range(len(edge_angles)):
        edge_angles[i] = abs(edge_angles[i] % (math.pi / 2))  # want strictly positive answers
    # print "Edge angles in 1st Quadrant: \n", edge_angles

    # Remove duplicate angles
    edge_angles = np.unique(edge_angles)
    # print "Unique edge angles: \n", edge_angles

    # Test each angle to find bounding box with smallest area
    min_bbox = (0, sys.maxsize, 0, 0, 0, 0, 0, 0)  # rot_angle, area, width, height, min_x, max_x, min_y, max_y
    # print"Testing", len(edge_angles), "possible rotations for bounding box... \n"
    for i in range(len(edge_angles)):

        # Create rotation matrix to shift points to baseline
        # R = [ cos(theta)      , cos(theta-PI/2)
        #       cos(theta+PI/2) , cos(theta)     ]
        R = np.array([[math.cos(edge_angles[i]), math.cos(edge_angles[i] - (math.pi / 2))],
                      [math.cos(edge_angles[i] + (math.pi / 2)), math.cos(edge_angles[i])]])
        # print "Rotation matrix for ", edge_angles[i], " is \n", R

        # Apply this rotation to convex hull points
        rot_points = np.dot(R, np.transpose(hull_points_2d))  # 2x2 * 2xn
        # print "Rotated hull points are \n", rot_points

        # Find min/max x,y points
        min_x = np.nanmin(rot_points[0], axis=0)
        max_x = np.nanmax(rot_points[0], axis=0)
        min_y = np.nanmin(rot_points[1], axis=0)
        max_y = np.nanmax(rot_points[1], axis=0)
        # print "Min x:", min_x, " Max x: ", max_x, "   Min y:", min_y, " Max y: ", max_y

        # Calculate height/width/area of this bounding rectangle
        width = max_x - min_x
        height = max_y - min_y
        area = width * height
        # print "Potential bounding box ", i, ":  width: ", width, " height: ", height, "  area: ", area

        # Store the smallest rect found first (a simple convex hull might have 2 answers with same area)
        if (area < min_bbox[1]):
            min_bbox = (edge_angles[i], area, width, height, min_x, max_x, min_y, max_y)
            # Bypass, return the last found rect
            # min_bbox = ( edge_angles[i], area, width, height, min_x, max_x, min_y, max_y )

    # Re-create rotation matrix for smallest rect
    angle = min_bbox[0]
    R = np.array(
        [[math.cos(angle), math.cos(angle - (math.pi / 2))], [math.cos(angle + (math.pi / 2)), math.cos(angle)]])
    # print "Projection matrix: \n", R

    proj_points = np.dot(R, np.transpose(hull_points_2d))  # 2x2 * 2xn

    # min/max x,y points are against baseline
    min_x = min_bbox[4]
    max_x = min_bbox[5]
    min_y = min_bbox[6]
    max_y = min_bbox[7]
    # print "Min x:", min_x, " Max x: ", max_x, "   Min y:", min_y, " Max y: ", max_y

    # Calculate center point and project onto rotated frame
    center_x = (min_x + max_x) / 2
    center_y = (min_y + max_y) / 2
    center_point = np.dot([center_x, center_y], R)
    # print "Bounding box center point: \n", center_point

    # Calculate corner points and project onto rotated frame
    corner_points = np.zeros((4, 2))  # empty 2 column array
    corner_points[0] = np.dot([max_x, min_y], R)
    corner_points[1] = np.dot([min_x, min_y], R)
    corner_points[2] = np.dot([min_x, max_y], R)
    corner_points[3] = np.dot([max_x, max_y], R)
    # print "Bounding box corner points: \n", corner_points

    # print "Angle of rotation: ", angle, "rad  ", angle * (180/math.pi), "deg"

    return angle, min_bbox[1], min_bbox[2], min_bbox[3], center_point, corner_points


# End stolen code from the internet


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
# returns list of pixel indices and their corresponding values
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


def analyze_cluster(data, frame, pixels):
    points = [(pixel[0], pixel[1]) for pixel in pixels]

    # Calculate convex hull for cluster
    hull = qhull2d(np.array(points))
    hull = hull[::-1]

    # Calculate minimum spanning rectangle
    rot_angle, area, width, height, center, corners = min_bounding_rect(hull)

    # Centroid of the cluster
    cluster_centroid = centroid(pixels)
    # Total deposited energy for a given cluster
    total_energy = data.get_total_energy(pixels)
    # Number of inner pixels for a given cluster
    inner_pixels = len(list(filter(lambda x: is_inner_pixel(x, frame), pixels)))
    # Pixel with the maximum ToT
    max_pixel = max(pixels, key=lambda x: x[2])

    # Define length  as maximum of the two sides of the rectangle
    length = max([width, height])
    width = min([width, height])

    pixelcount = len(pixels)

    # Calculating convex hull for only one pixel leads to some strange behavior,
    # so special case for when n=1
    if pixelcount > 1:
        density = pixelcount / area
        lwratio = length / width
    else:
        lwratio = 1
        density = 1

    if inner_pixels == 0 and pixelcount <= 4:
        cluster_type = SMALL_BLOB
    elif inner_pixels > 4 and lwratio > 1.25 and density > 0.3:
        cluster_type = HEAVY_TRACK
    elif inner_pixels > 4 and lwratio <= 1.25 and density > 0.5:
        cluster_type = HEAVY_BLOB
    elif inner_pixels > 1 and lwratio <= 1.25 and density > 0.5:
        cluster_type = MEDIUM_BLOB
    elif inner_pixels == 0 and lwratio > 8.0:
        cluster_type = STRAIGHT_TRACK
    else:
        cluster_type = LIGHT_TRACK

    return max_pixel, density, total_energy, rot_angle, cluster_centroid, cluster_type, corners


# Determines the number of clusters given a single frame of acquisition data
def cluster_count(data, frame, threshold=0):
    clusters = 0
    cluster_info = []

    for row in frame:
        for pixel in row:
            if pixel.value > threshold and not pixel.filled:
                pixels = floodfill(pixel.x, pixel.y, frame)
                cluster_info.append(analyze_cluster(data, frame, pixels))
                clusters += 1

    return cluster_info


def main(args):
    data = PmfFile(args.filename)
    threshold = int(args.threshold)

    data.load_calib_a("a.txt")
    data.load_calib_b("b.txt")
    data.load_calib_c("c.txt")
    data.load_calib_t("t.txt")

    print("Processing {} frames...".format(data.num_frames))

    for i, frame in enumerate(data.frames()):
        print("Frame {}".format(i))
        energy = 0
        for cluster in cluster_count(data, frame, threshold=threshold):
            _, _, total_energy, _, _, _ = cluster
            energy += total_energy
            print(cluster)
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
