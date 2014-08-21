#!/usr/bin/env python2

import sys, argparse

import numpy
from matplotlib import pyplot as plot



def main():
	parser = argparse.ArgumentParser(description = 'Shows data points')
	parser.add_argument('input', metavar='PATH', type=str, nargs=1, help='input file')
	args = parser.parse_args()

	try:
		data = numpy.loadtxt(args.input[0])
	except IOError as (errno, strerror):
		print "Failed to load '%s':%s" % (args.input[0], strerror)
		sys.exit(0)

	# Compute the grid layout
	n = data.shape[1] - 1
	graph_count = (n * (n - 1)) / 2

	w, h = 1, 1
	while w * h < graph_count:
		if w < h:
			w += 1
		else:
			h += 1

	# Plot the graphics
	colors = [('#000000', '#ffffff')[int(i) % 2] for i in data[:,-1]]
	x, y = 0, 0
	for i in range(n):
		for j in range(i + 1, n):
			plot.subplot2grid((w, h), (x, y), rowspan=1, colspan=1)
			plot.scatter(data[:,i], data[:,j], c = colors)		
			plot.title('dim %d / %d' % (i, j))

			if x >= w - 1:
				x, y = 0, y + 1
			else:
				x, y = x + 1, y

	plot.tight_layout()
	plot.show()



if __name__ == "__main__":
	main()
