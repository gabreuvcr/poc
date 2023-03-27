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
    plt.plot(x, y, label=label, linestyle='--')
    plt.legend()
    plt.xlim(0, max_sensors + 1)
    plt.ylim(0, 1.1)
    plt.xlabel('Número de sensores')
    plt.ylabel('% de cobertura')

max_sensors = -1;
output_file = argv[1]
path_save = argv[2]
type_plot = argv[3] # line or scatter

f = open(output_file, 'r')
markers = ['o', 'v', 's', 'p', '*', 'h', 'H', 'D', 'd', 'P', 'X']
sizes = [100, 90, 80, 70, 60, 50, 40, 30, 20, 10]
# colors = ["#ffdab9", "#6495ed", "#ff00ff", "#00ffff", "#00ff00", "#ffd700", "#ff4500", "#b03060", "#00008b", "#006400"]
colors = ["#99F2D1", "#8BDEC0", "#7DCAAE", "#6FB69D", "#61A28C", "#548E7A", "#467A69", "#386658", "#2A5246", "#1C3E35"]
for i in range(0, 10):
    x_de, y_de = get_output_splited(f)
    print(x_de)
    print(y_de)
    max_sensors = max(max(x_de), max_sensors)
    if type_plot == 'l':
        plot_line(x_de, y_de, max_sensors, f'{i + 1}')
    elif type_plot == 's':
        plot_scatter(x_de, y_de, max_sensors, f'{i + 1}', marker=markers[0], size=sizes[i], color=colors[i])
    else:
        raise Exception('Wrong type of plot')

plt.title("")
plt.tight_layout()

plt.hlines(y=1, linestyles='--', xmin=0, xmax=max_sensors+1, colors='#000', alpha=0.15)
plt.savefig(path_save, dpi=300)
