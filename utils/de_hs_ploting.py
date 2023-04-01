import matplotlib.pyplot as plt
from sys import argv

def de_get_output_splited(line):
    line = line.split(';')
    x, y = [], []
    for numbers in line:
        if '\n' in numbers: continue
        n = numbers.split('|')
        x += [float(n[0])]
        y += [float(n[1])]
    return x, y


def hs_get_output_splited(line):
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


sizes = [100, 90, 80, 70, 60, 50, 40, 30, 20, 10]
colors = [
    "#99F2D1", "#8BDEC0", "#7DCAAE", "#6FB69D", "#61A28C",
    "#548E7A", "#467A69", "#386658", "#2A5246", "#1C3E35"
]

de_output_file = argv[1]
hs_output_file = argv[2]
path_save = argv[3]
instance_n = argv[4]
max_sensors = -1;


de_file = open(de_output_file, 'r')
hs_file = open(hs_output_file, 'r')
x_de, y_de = de_get_output_splited(de_file.readlines()[10])
x_hs, _, y_hs_avg, _ = hs_get_output_splited(hs_file.readlines()[0])
max_sensors = max(max(x_de), max(x_hs))

## color
# plt.scatter(x_de, y_de, s=50, color=colors[9], marker='o', label='Differential Evolution')
# plt.scatter(x_hs, y_hs_avg, s=50, color=colors[2], marker='o', label='Harmony Search')
## bw
plt.scatter(x_de, y_de, s=40, facecolors='none', edgecolors='black', marker='o', label='Differential Evolution')
plt.scatter(x_hs, y_hs_avg, s=15, color='black', marker='.', label='Harmony Search')

plt.title(f"Comparação DE e HS: Instância {instance_n}")
plt.legend()    
plt.xlim(0, max_sensors + 1)
plt.ylim(0, 1.1)
plt.xlabel('Número de sensores')
plt.ylabel('% de cobertura')
plt.hlines(y=1, linestyles='--', xmin=0, xmax=max_sensors+1, colors='#000', alpha=0.15)
plt.tight_layout()
plt.savefig(path_save, dpi=300)
