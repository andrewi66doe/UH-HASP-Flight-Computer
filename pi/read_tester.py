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
    if len(argv) < 2:
        print("USAGE: {} <data filename>")
        exit(0)

    with open(argv[1], 'r') as input_stream:
        for i in input_stream:
            i = i.replace('[', "")
            i = i.replace(']', "")
            splitlist = i.split()
            # print(splitlist)
            sendto(splitlist)

    write_token_data()