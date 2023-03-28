import matplotlib.pyplot as plt
from matplotlib import container
import numpy as np
from sys import argv

def get_output_splited(file):
    text = file.readline()
    line = text.split(';')
    x, y_min, y_avg, y_max = [], [], [], []
    for i in range(0, len(line)):
        if '\n' in line[i]: continue
        l = line[i].split('|')
        x += [float(l[0])]
        y_min += [float(l[1])]
        y_avg += [float(l[2])]
        y_max += [float(l[3])]
    return x, y_min, y_avg, y_max

def plot_scatter(x, y_avg, yerr, max_sensors, label, marker, color):
    def remove_errorbars(ax):
        handles, labels = ax.get_legend_handles_labels()
        handles = [h[0] if isinstance(h, container.ErrorbarContainer) else h for h in handles]
        ax.legend(handles, labels)
        
    plt.errorbar(
        x, y_avg, yerr=yerr, label=label,
        fmt=marker, ecolor = "#000", capsize=2, color=color
    )
    remove_errorbars(plt.gca())
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
colors = [
    "#99F2D1", "#8BDEC0", "#7DCAAE", "#6FB69D", "#61A28C", 
    "#548E7A", "#467A69", "#386658", "#2A5246", "#1C3E35"
]
x, y_min, y_avg, y_max = get_output_splited(f)
max_sensors = max(max(x), max_sensors)
y_min = np.array(y_avg) - np.array(y_min)
y_max = np.array(y_max) - np.array(y_avg)
assymetric_error = np.array(list(zip(y_min, y_max))).T
plot_scatter(x, y_avg, assymetric_error, max_sensors, "Average", 'o', colors[3])

plt.title("Harmony Search: Instância 1 - Max, média e min de 10 execuções")
plt.tight_layout()
plt.hlines(y=1, linestyles='--', xmin=0, xmax=max_sensors+1, colors='#000', alpha=0.15)
plt.savefig(path_save, dpi=300)
