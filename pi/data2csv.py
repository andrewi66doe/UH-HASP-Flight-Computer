# take in data file
# go through string and check for token
# append to appropriate csv file
# time stamp that is in the first column should be from the title
import datetime
import csv

from sys import argv

tokens = {}


def sendto(splitlist):
    token = splitlist[3]

    if token not in tokens.keys():
        tokens[token] = []
        tokens[token].append(splitlist)
    else:
        tokens[token].append(splitlist)


def write_token_data():
    for token in tokens:
        filename = token.strip().strip(':') + '.csv'

        with open(filename, 'w') as output_stream:
            writer = csv.writer(output_stream, dialect=csv.excel,
                                 quoting=csv.QUOTE_NONNUMERIC)
            writer.writerow(["Date", "Time", "---", "Sensor Name", "Data"])
            writer.writerows(tokens[token])


if __name__ == "__main__":
    if len(argv) == 2:
        filename = argv[1]
    elif len(argv) < 2:
        filename = "readings.data"
    else:
        print("USAGE: {} <data filename>".format(arg[0]))
        exit(0)
    try:
        with open(filename, 'r') as input_stream:
            for i in input_stream:
                i = i.replace('[', "")
                i = i.replace(']', "")
                splitlist = i.split()
                # print(splitlist)
                sendto(splitlist)
    except FileNotFoundError:
        print("Failed to open file \"{}\"".format(filename))
        exit(0);
    write_token_data()
