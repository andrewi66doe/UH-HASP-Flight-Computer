
import numpy as np
from math import sqrt
from clustercount import PmfFile

def frame2nparray(frame):
    array = np.ones((256,256), dtype=float)
    for i, line in enumerate(frame):
        for j, value in enumerate(line.split()):
            array[i][j] = float(value)
    return array


def arrayscale(a, b):
    result = np.ones((256,256), dtype=float)
    for i in range(256):
        for j in range(256):
            result[i][j] = a[i][j] * b[i][j]
    return result


def pixel_energy(frame):            

    with open("a.txt", 'r') as a:
        file_a = a.readlines()
    with open("b.txt", 'r') as b:
        file_b = b.readlines()
    with open("c.txt", 'r') as c:
        file_c = c.readlines()
    with open("t.txt", 'r') as t:
        file_t = t.readlines()

    a = frame2nparray(file_a)
    b = frame2nparray(file_b)
    c = frame2nparray(file_c)
    t = frame2nparray(file_t)
    ToT = frame2nparray(frame)


    E = (np.sqrt(arrayscale(a, a) + 2*arrayscale(b,t) - 2*arrayscale(a,ToT) + arrayscale(arrayscale(b, b),arrayscale(t,t)) - 4*arrayscale(b,c) - 2*arrayscale(b,arrayscale(t,ToT)) + arrayscale(ToT,ToT)) - a +arrayscale(b,t) + ToT) / (2*b)
    print(type(E))
    return E



if __name__ == "__main__":
    data = PmfFile("carbon4_frame100.pmf")
    print(pixel_energy(data.get_frame_raw(0)))
