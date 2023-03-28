import matplotlib.pyplot as plt
from sys import argv

def get_output_splited(file):
    text = file.readline()
    line = text.split(';')
    x, y = [], []
    for i in range(0, len(line)):
        if '\n' in line[i]: continue
        l = line[i].split('|')
        x += [float(l[0])]
        y += [float(l[1])]
    return x, y

def plot_scatter(x, y, max_sensors, label, marker, size, color):
    plt.scatter(x, y, s=size, label=label, marker=marker, color=color, edgecolor='None')
    plt.legend()
    plt.xlim(0, max_sensors + 1)
    plt.ylim(0, 1.1)
    plt.xlabel('Número de sensores')
    plt.ylabel('% de cobertura')

def plot_line(x, y, max_sensors, label):
    plt.plot(x, y, label=label, linestyle=':')
    plt.legend()
    plt.xlim(0, max_sensors + 1)
    plt.ylim(0, 1.1)
    plt.xlabel('Número de sensores')
    plt.ylabel('% de cobertura')

max_sensors = -1;
output_file = argv[1]
path_save = argv[2]

f = open(output_file, 'r')
markers = ['o', 'v', 's', 'p', '*', 'h', 'H', 'D', 'd', 'P', 'X']
sizes = [100, 90, 80, 70, 60, 50, 40, 30, 20, 10]
colors = ["#99F2D1", "#8BDEC0", "#7DCAAE", "#6FB69D", "#61A28C", "#548E7A", "#467A69", "#386658", "#2A5246", "#1C3E35"]
bw = ["#000", "#161616", "#2C2C2C", "#424242", "#585858", "#6E6E6E", "#848484", "#9A9A9A", "#B0B0B0", "#C6C6C6"]
# bw = ["#C6C6C6", "#B0B0B0", "#9A9A9A", "#848484", "#6E6E6E", "#585858", "#424242", "#2C2C2C", "#161616", "#000"]

for i in range(0, 10):
    x_de, y_de = get_output_splited(f)
    print(x_de)
    print(y_de)
    max_sensors = max(max(x_de), max_sensors)
    # plot_scatter(x_de, y_de, max_sensors, f'{i + 1}', marker=markers[0], size=sizes[i], color=bw[i])
    plot_scatter(x_de, y_de, max_sensors, f'{i + 1}', marker=markers[0], size=sizes[i], color=colors[i])

plt.title("")
plt.tight_layout()
plt.hlines(y=1, linestyles='--', xmin=0, xmax=max_sensors+1, colors='#000', alpha=0.15)
plt.savefig(path_save, dpi=300)
