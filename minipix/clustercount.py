import sys
import pdb
import traceback
import argparse
import subprocess

from threading import Thread
from pprint import pprint
from random import randint, seed
from itertools import islice

# Width and height of minipix detector
DATA_FRAME_WIDTH = 256
DATA_FRAME_HEIGHT = 256

# Maximum number of threads for an i7 this will be different on other systems
MAX_THREAD_COUNT = 96


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

    def get_frame_raw(self, frame):

        pmf_file = open(self.filename, "r")

        if frame > self.num_frames or frame < 0:
            raise IndexError("Frame index out of range")

        start = frame * DATA_FRAME_HEIGHT
        end = (frame * DATA_FRAME_HEIGHT) + DATA_FRAME_HEIGHT

        lines = list(islice(pmf_file, start, end))

        return lines

    def get_frame(self, frame):
        pmf_file = open(self.filename, "r")

        if frame > self.num_frames or frame < 0:
            raise IndexError("Frame index out of range")

        start = frame * DATA_FRAME_HEIGHT
        end = (frame * DATA_FRAME_HEIGHT) + DATA_FRAME_HEIGHT

        lines = islice(pmf_file, start, end)
        pmf_data = []

        for y, line in enumerate(lines):
            row_vals = []

            for x, row_val in enumerate(line.split()):
                row_vals.append(Pixel(int(row_val), (DATA_FRAME_WIDTH, DATA_FRAME_HEIGHT), (x, y % 256)))

            pmf_data.append(row_vals)

        pmf_file.close()

        return pmf_data


# Iterative floodfill (Python doesn't optimize tail recursion)
# Returns average ToT for a cluster
def floodfill(x, y, arr):
    to_fill = set()
    to_fill.add((x, y))

    sum = 0
    count = 0

    while not len(to_fill) == 0:
        x, y = to_fill.pop()

        pixel = arr[y][x]
        pixel.filled = True
        count += 1
        sum += pixel.value

        for x, y in pixel.surrounding_pixels():
            if arr[y][x].value > threshold and not arr[y][x].filled:
                to_fill.add((x, y))
    return sum / count


# Determines the number of clusters given a single frame of acquisition data
def cluster_count(frame):
    clusters = 0

    for row in frame:
        for pixel in row:
            if pixel.value > threshold and not pixel.filled:
                avg_tot = floodfill(pixel.x, pixel.y, frame)
                clusters += 1
                print("Cluster {0}: {1:.2f}".format(clusters, avg_tot))

    return clusters


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Determine the cluster count for each frame in a pmf acquisition file")
    parser.add_argument('filename', help='Pmf file to process')
    parser.add_argument('-t',
                        action='store',
                        dest='threshold',
                        default=1,
                        help='Threshold')
    args = parser.parse_args()
    data = PmfFile(args.filename)
    threshold = int(args.threshold)

    try:
        print("Cluster Counts:")
        threads = []
        i = 0

        while i < data.num_frames:
            print("Frame (" + str(i) + ")")
            t = Thread(target=cluster_count, args=(data.get_frame(i),))
            t.start()
            threads.append(t)

            i += 1

            # If we're at max thread count then wait for thread list to empty 
            # This can be improved later by adding a queue
            if len(threads) == MAX_THREAD_COUNT:
                for thread in threads:
                    thread.join()
                    threads.remove(thread)

    # Drop into shell on failure for postmortem debugging
    except:
        _, _, tb = sys.exc_info()
        traceback.print_exc()
        pdb.post_mortem(tb)
