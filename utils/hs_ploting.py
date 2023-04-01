import matplotlib.pyplot as plt
from matplotlib import container
import numpy as np
from sys import argv

def get_output_splited(line):
    line = line.split(';')
    x, y_min, y_avg, y_max = [], [], [], []
    for numbers in line:
        if '\n' in numbers: continue
        n = numbers.split('|')
        x += [float(n[0])]
        y_min += [float(n[1])]
        y_avg += [float(n[2])]
        y_max += [float(n[3])]
    return x, y_min, y_avg, y_max


def remove_errorbars(ax):
        handles, labels = ax.get_legend_handles_labels()
        handles = [h[0] if isinstance(h, container.ErrorbarContainer) else h for h in handles]
        ax.legend(handles, labels)


sizes = [100, 90, 80, 70, 60, 50, 40, 30, 20, 10]
colors = [
    "#99F2D1", "#8BDEC0", "#7DCAAE", "#6FB69D", "#61A28C", 
    "#548E7A", "#467A69", "#386658", "#2A5246", "#1C3E35"
]

output_file = argv[1]
path_save = argv[2]
max_sensors = -1;

f = open(output_file, 'r')
lines = f.readlines()
x, y_min, y_avg, y_max = get_output_splited(lines[0])
max_sensors = max(max(x), max_sensors)
y_min = np.array(y_avg) - np.array(y_min)
y_max = np.array(y_max) - np.array(y_avg)
assymetric_error = np.array(list(zip(y_min, y_max))).T
remove_errorbars(plt.gca())
plt.errorbar(
    x, y_avg, yerr=assymetric_error, label='Average',
    fmt='o', ecolor = "#000", capsize=2, color=colors[3]
)

plt.title("Harmony Search: Instância 1")
plt.legend()
plt.xlim(0, max_sensors + 1)
plt.ylim(0, 1.1)
plt.xlabel('Número de sensores')
plt.ylabel('% de cobertura')
plt.hlines(y=1, linestyles='--', xmin=0, xmax=max_sensors+1, colors='#000', alpha=0.15)
plt.tight_layout()
plt.savefig(path_save, dpi=300)
