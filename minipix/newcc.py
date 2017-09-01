import numpy as np

from clustercount import PmfFile


def frame2np(frame):
    array = np.ones((256,256), dtype=int)
    for i, line in enumerate(frame):
        for j, value in enumerate(line.split()):
            array[i][j] = int(value)
    return array
            


if __name__ == "__main__":
    data = PmfFile("data.pmf")
    threshold = 1
    a = frame2np(data.get_frame_raw(9))
    bool_filter = a > threshold
    print(bool_filter)
    pass
