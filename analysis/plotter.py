# Author: Kevin Portillo 
# This script will create plots of all .csv in current directory

import sys, datetime, pylab, os, optparse
import pandas as pd
from matplotlib import pyplot as plt

if __name__ == "__main__":
	# Command line parsing 
	parser = optparse.OptionParser("usage: %prog [options]")
	parser.add_option("-i", "--input", dest="input_file", type="string", help="specify input filename")
	(opts, args) = parser.parse_args()
	if opts.input_file is None:
		parser.print_help()
		parser.error("No input file specified")
	else: 
		input_file = opts.input_file

	# Save only the filenames into 'filenames' list and remove the .csv part
	# Also popping 'plotter.py' and '-i'
	args = "\n".join(sys.argv).split()
	args.pop(0) 
	args.pop(0)
	filenames = [os.path.splitext(each)[0] for each in args]

	# For each file, plot time vs data
	# Title = filename
	# x axis = time 
	# y axis = data

	for file in filenames:
		with open(file+'.csv', 'r', encoding='utf-8') as inFile:
			df = pd.read_csv(inFile, parse_dates=True, usecols=['Time','Data'])
			time = pd.to_datetime(df['Time'],format='%H:%M:%S').values.tolist()
			data = df['Data'].values.tolist()

		plt.plot(time,data,'r')
		plt.xlabel('Time')
		plt.ylabel('Data')
		plt.title(file)
		plt.show()
		pylab.savefig(file +'.png')
	

