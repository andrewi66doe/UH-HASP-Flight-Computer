import pickle
import csv
import pytz
from clustercount import PmfFile
from pprint import pprint
from dateutil import parser as dateparser

cluster_types = ["SMALL_BLOB", "HEAVY_TRACK", "HEAVY_BLOB", "MEDIUM_BLOB", "STRAIGHT_TRACK", "LIGHT_TRACK"]

utc = pytz.UTC


def get_cluster_count(file, data):
    for frame in range(file.num_frames):
        frame_data = data.get(frame, None)

        if frame_data:
            acq_time = data[frame]["acq_time"].replace(tzinfo=utc)
            clusters = data[frame]["clusters"]

            # Cluster types
            types = [cluster[5] for cluster in clusters]
            type_freq = {c_type: types.count(c_type) for c_type in cluster_types}

            print(acq_time, end=",")
            print(",".join(map(str, type_freq.values())))
        else:
            acq_time = file.timestamps[frame]
            print(acq_time, end=",")
            print("0,0,0,0,0,0")


def get_frame_energy(file, data):
    for frame in range(file.num_frames):
        frame_data = data.get(frame, None)

        if frame_data:
            acq_time = data[frame]["acq_time"].replace(tzinfo=utc)
            clusters = data[frame]["clusters"]
            # Total energy
            cluster_energy = [cluster[2] for cluster in clusters]
            total_energy = sum(cluster_energy)

            print(acq_time, end=",")
            print(total_energy)
        else:
            acq_time = file.timestamps[frame]
            print(acq_time, end=",")
            print(0)


def get_cluster_energy(file, data):
    for frame in range(file.num_frames):
        frame_data = data.get(frame, None)

        if frame_data:
            acq_time = data[frame]["acq_time"].replace(tzinfo=utc)
            clusters = data[frame]["clusters"]

            for cluster in clusters:
                print(acq_time, end=",")
                print(cluster[5], end=",")
                print(cluster[2])


if __name__ == "__main__":
    serialized = open('clusters.pkl', 'rb')

    data = pickle.load(serialized)

    start = dateparser.parse("09/04/17 08:56:23 GMT")

    file = PmfFile("HASPDATA/thurs_test36.pmf")
    file.load_dsc()

    get_cluster_energy(file, data)
    # get_cluster_count(file, data)
    # get_energy(file, data)
