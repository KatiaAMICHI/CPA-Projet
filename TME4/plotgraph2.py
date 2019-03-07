import argparse
import matplotlib.pyplot as plt
import networkx as nx

parser = argparse.ArgumentParser(description='Create a graph')
parser.add_argument('efile', metavar="efile", nargs=1, help='edge file')
parser.add_argument('cfile', metavar="cfile", nargs=1, help='community file')

args = parser.parse_args()

graph = nx.Graph()
graph.add_edges_from(nx.read_edgelist(args.efile[0]).edges())

colorMap = ['r','b','g','y','p','c']
colors = ['r' for _ in range(0, nx.number_of_nodes(graph))]
with open(args.cfile[0], 'r') as cfile:
    for line in cfile:
        line = line.rstrip().split(' ')
        node = int(line[0]); com = int(line[1])
        colors[node] = com

colorSet = list(set(colors))
print(colorSet)
colors = [colorMap[colorSet.index(c)] for c in colors]

nx.draw(graph, node_size=20, node_color=colors)
plt.show();
