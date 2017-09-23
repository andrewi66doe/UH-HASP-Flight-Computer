import pickle
from pprint import pprint

cluster_types = ["SMALL_BLOB", "HEAVY_TRACK", "HEAVY_BLOB", "MEDIUM_BLOB", "STRAIGHT_TRACK", "LIGHT_TRACK"]

if __name__ == "__main__":
    serialized = open('clusters.pkl', 'rb')

    data = pickle.load(serialized)

    for frame in data:
        clusters = data[frame]

        # Cluster types
        types = [cluster[5] for cluster in clusters]
        type_freq = {c_type: types.count(c_type) for c_type in cluster_types}

        # Total energy
        cluster_energy = [cluster[2] for cluster in clusters]
        total_energy = sum(cluster_energy)

        print(type_freq)
    serialized.close()
