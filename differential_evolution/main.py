import matplotlib.pyplot as plt

# Plotando os pontos de uma população para visualizar
# a fronteira de Pareto

text = open('input.txt', 'r').read()
l1 = text.split(';')
x1, y1 = [], []
x2, y2 = [], []

for i in range(0, len(l1)):
    l2 = l1[i].split(',')
    if (int(l2[2]) == 0):
        x1 += [float(l2[0])]
        y1 += [float(l2[1])]
    else:
        x2 += [float(l2[0])]
        y2 += [float(l2[1])]

plt.scatter(x1, y1, color='blue')
plt.scatter(x2, y2, color='red')
plt.savefig("mygraph.png")
