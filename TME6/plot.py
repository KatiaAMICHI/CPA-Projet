import argparse
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description='Create a graph')
parser.add_argument('n', metavar='n', nargs=1, help="number of nodes in the graph")
parser.add_argument('xfile', metavar="xfile", nargs=1, help='name of the x axis file to parse and show')
parser.add_argument('yfile', metavar="yfile", nargs=1, help='name of the y axis file to parse and show')

args = parser.parse_args()

x = [0 for _ in range(int(args.n[0]))]
y = [0 for _ in range(int(args.n[0]))]

with open(args.xfile[0], 'r') as f:
    for line in f:
        tab = line[:-1].split(" ")
        x[int(tab[0])] = float(tab[1])

xmin = min(x); xmax = max(x)

with open(args.yfile[0], 'r') as f:
    for line in f:
        tab = line[:-1].split(" ")
        y[int(tab[0])] = float(tab[1])

ymin = min(y); ymax = max(y)

plt.yscale("log")
plt.xscale("log")
plt.xlim(xmin/10, xmax*10)
plt.ylim(ymin/10, ymax*10)
plt.plot([xmin/10,xmax*10], [xmin/10,xmax*10], ls="-", color="k")
plt.scatter(x, y, label="x = Degree, y = Coreness")
plt.xlabel("Degree")
plt.ylabel("Coreness")
plt.legend()
plt.show()
