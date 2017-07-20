# Author: Kevin Portillo 
# This script will create plots of all .csv in current directory

import sys, datetime, os, optparse
import pandas as pd
from matplotlib import pyplot as plt

if __name__ == "__main__":
	# Command line parsing 
	parser = optparse.OptionParser("usage: %prog [options]")
	parser.add_option("-i", "--input", dest="input_file", type="string", help="specify input filename")
	(opts, args) = parser.parse_args()
	if opts.input_file is None:
		parser.print_help()
		parser.error("No input file(s) specified")
	else: 
		input_file = opts.input_file

	# Place each token from the command line into list but without plotter.py and -i
	args = "\n".join(sys.argv).split()[2:]
	filenames = [os.path.splitext(each)[0] for each in args]

	# For each file, plot time vs data
	# Title = filename, x axis = time, y axis = data

	for file in filenames:
		with open(file +'.csv', 'r', encoding='utf-8') as inFile:
			df = pd.read_csv(inFile, parse_dates = True, index_col = 0)
		df.plot(lw=2, colormap='jet', marker='.', markersize=10, title=file).get_figure().savefig(file + '.png')

