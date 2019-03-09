import argparse
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description='Create a graph')
parser.add_argument('xfile', metavar="xfile", nargs=1, help='name of the x axis file to parse and show')
parser.add_argument('yfile', metavar="yfile", nargs=1, help='name of the y axis file to parse and show')

args = parser.parse_args()

x = []
y = []

with open(args.xfile[0], 'r') as f:
    for line in f:
        tab = line[:-1].split(" ")
        x += [float(tab[1])]

xmin = min(x); xmax = max(x)

with open(args.yfile[0], 'r') as f:
    for line in f:
        tab = line[:-1].split(" ")
        y += [float(tab[1])]

ymin = min(y); ymax = max(y)

plt.yscale("log")
plt.xscale("log")
plt.xlim(xmin, xmax)
plt.ylim(ymin, ymax)
plt.scatter(x, y, label="x = PageRank (alpha = 0.15), y = PageRank (alpha = 0.9)")
plt.xlabel("alpha = 0.15")
plt.ylabel("alpha = 0.9")
plt.legend()
plt.show()
