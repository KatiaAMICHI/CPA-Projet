import argparse
import matplotlib.pyplot as plt

parser = argparse.ArgumentParser(description='Create a graph')
parser.add_argument('files', metavar="files", nargs='+', help='name of the files to parse and show')

args = parser.parse_args()

for filename in args.files:
    x = []
    y = []
    with open(filename, 'r') as f:
        for line in f:
            tab = line[:-1].split(" ")
            x+=[int(tab[0])]; y+=[float(tab[1])]

plt.bar(x, y, label=filename.split("/")[1].split(".")[0])
plt.xlabel("Degré")
plt.ylabel("Nombres de noeuds")
plt.legend()
plt.show()
