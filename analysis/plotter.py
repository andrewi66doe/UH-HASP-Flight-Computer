# Author: Kevin Portillo 
# This script will create plots of any input .csv assuming two columns of data 

import sys
import pandas as pd
from matplotlib import pyplot as plt
import optparse

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

	print("\n".join(sys.argv))
	# Read Columns 
	# fields = ['time','presidents']
	# with open(input_file, 'r', encoding='utf-8') as inFile:
	# 	df = pd.read_csv(inFile, usecols=fields)
	# 	time = df['time'].values.tolist()
	# 	presidents = df['presidents'].values.tolist()

	# plt.plot(time,presidents,'r')
	# plt.xlabel('Time by Queaters Since 1945')
	# plt.ylabel('% Approval Ratings')
	# plt.title('Quarterly Approval Ratings of US Presidents Since 1945')
	# plt.show()
	

