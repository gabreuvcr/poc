import matplotlib.pyplot as plt
from sys import argv

def get_output_splited(line):
    line = line.split(';')
    x, y = [], []
    for numbers in line:
        if '\n' in numbers: continue
        n = numbers.split('|')
        x += [float(n[0])]
        y += [float(n[1])]
    return x, y


sizes = [100, 90, 80, 70, 60, 50, 40, 30, 20, 10]
sizes2 = [190, 170, 150, 130, 110, 90, 70, 50, 30, 10]
colors = [
    "#99F2D1", "#8BDEC0", "#7DCAAE", "#6FB69D", "#61A28C", 
    "#548E7A", "#467A69", "#386658", "#2A5246", "#1C3E35"
]

output_file = argv[1]
path_save = argv[2]
instance_n = argv[3]
max_sensors = -1;

f = open(output_file, 'r')
lines = f.readlines()
for i in range(0, 10):
    x_de, y_de = get_output_splited(lines[i])
    max_sensors = max(max(x_de), max_sensors)
    
    # ## bw
    # plt.scatter(
    #     x_de, y_de, s=sizes2[i], facecolors='none', 
    #     edgecolors='black', marker='o', label=f'{i + 1}', linewidths=0.5
    # )
    ## color
    plt.scatter(
        x_de, y_de, s=sizes[i], marker='o',
        label=f'{i + 1}', color=colors[i], edgecolor='None'
    )
    # ## line
    # plt.plot(x_de, y_de, label=f'{i + 1}', linestyle=':')

plt.title(f"Differential Evolution: Instância {instance_n}")
plt.legend()
plt.xlim(0, max_sensors + 1)
plt.ylim(0, 1.1)
plt.xlabel('Número de sensores')
plt.ylabel('% de cobertura')
plt.hlines(y=1, linestyles='--', xmin=0, xmax=max_sensors+1, colors='#000', alpha=0.15)
plt.tight_layout()
plt.savefig(path_save, dpi=300)
