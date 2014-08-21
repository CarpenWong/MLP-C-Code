#!/usr/bin/env python2

import sys, argparse

import numpy
from matplotlib import pyplot as plot



colors = ('#FF4500', '#3CB371', '#4682B4', '#DB7093', '#FFD700')

def main():
	parser = argparse.ArgumentParser(description='Shows data points colored by clusters')
	parser.add_argument('input', metavar='PATH', type=str, nargs=1, help='input file')
	args = parser.parse_args()

	try:
		data = numpy.loadtxt(args.input[0])
	except IOError as (errno, strerror):
		print "Failed to load '%s':%s" % (args.input[0], strerror)
		sys.exit(0)

	# Compute a color for each cluster id
	clusters_ids = list(set(int(value) for value in data[:,-1]))
	cluster_colors = [colors[clusters_ids.index(int(value)) % len(colors)] for value in data[:,-1]]

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
	x, y = 0, 0
	for i in range(n):
		for j in range(i + 1, n):
			plot.subplot2grid((w, h), (x, y), rowspan = 1, colspan = 1)
			plot.scatter(data[:,i], data[:,j], c = cluster_colors)		
			plot.title('dim %d / %d' % (i, j))

			if x >= w - 1:
				x, y = 0, y + 1
			else:
				x, y = x + 1, y

	plot.tight_layout()
	plot.show()



if __name__ == "__main__":
	main()
