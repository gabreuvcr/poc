import matplotlib.pyplot as plt
from sys import argv

# Input Format:
# max_number_of_sensors
# num_sensores_1,cobertura_1;num_sensores_2,cobertura_2;...;num_sensores_n,cobertura_n
# num_sensores_1,cobertura_1;num_sensores_2,cobertura_2;...;num_sensores_n,cobertura_n

def get_output_splited(file):
    text = file.readline()
    line = text.split(';')
    x, y = [], []
    for i in range(0, len(line)):
        l = line[i].split(',')
        x += [float(l[0])]
        y += [float(l[1])]
    return x, y

def plot_output(x, y, max_sensors, color, name, size=25, marker='o', fc=None):
    plt.scatter(x, y, s=size,color=color, facecolors=fc, label=name, marker=marker)
    plt.legend()
    plt.xlim(0, max_sensors + 1)
    plt.ylim(0, 1.1)
    plt.xlabel('Número de sensores')
    plt.ylabel('% de cobertura')


output_file = argv[1]
path_save = argv[2]

f = open(output_file, 'r')
max_sensors = int(f.readline())

x_de, y_de = get_output_splited(f)
x_hs, y_hs = get_output_splited(f)

plot_output(x_de, y_de, max_sensors, 'black', 'Differential Evolution', 40, 'o', 'none')
plot_output(x_hs, y_hs, max_sensors, 'black', 'Harmony Search', 15, '.')

plt.title('Comparação DE e HS: instância 3')
plt.tight_layout()
plt.hlines(y=1, linestyles='--', xmin=0, xmax=max_sensors+1, colors='#000', alpha=0.15)
plt.savefig(path_save)
print(f"plotted chart pop_evolution")
