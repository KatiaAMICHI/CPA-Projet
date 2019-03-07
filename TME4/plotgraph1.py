import argparse
import matplotlib.pyplot as plt
import networkx as nx

parser = argparse.ArgumentParser(description='Create a graph')
parser.add_argument('gfile', metavar="gfile", nargs=1, help='name of the files to parse and show')

args = parser.parse_args()

graph = nx.Graph()
graph.add_edges_from(nx.read_edgelist(args.gfile[0]).edges())

nx.draw(graph, node_size=10)
plt.show();
