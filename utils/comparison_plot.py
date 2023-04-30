import matplotlib.pyplot as plt
import argparse
from sys import argv

# python3 comparison_plot.py -deo=outputs/de_test.out -hso=outputs/hs_test.out -s=comparison_g1 -i=1

def get_args():
    parser = argparse.ArgumentParser(description="Plot arguments")
    parser.add_argument(
        '-deo', '--de_out_file',
        help='differential evolution output file',
        dest='de_out_file',
        action='store',
        type=str,
        required=True,
    )
    parser.add_argument(
        '-hso', '--hs_out_file',
        help='harmony search output file',
        dest='hs_out_file',
        action='store',
        type=str,
        required=True,
    )
    parser.add_argument(
        '-s', '--save_path',
        help='path to save the plot',
        dest='save_path',
        action='store',
        type=str,
        required=True,
    )
    parser.add_argument(
        '-i', '--instance',
        help='instance number',
        dest='instance',
        action='store',
        type=int,
        required=True,
    )
    args = parser.parse_args()
    return args


def get_output_splited(line):
    line = line.split(';')
    x, y = [], []
    for numbers in line:
        if '\n' in numbers: continue
        n = numbers.split('|')
        x += [float(n[0])]
        y += [float(n[1])]
    return x, y

args = get_args()

sizes = [100, 90, 80, 70, 60, 50, 40, 30, 20, 10]
colors = [
    "#99F2D1", "#8BDEC0", "#7DCAAE", "#6FB69D", "#61A28C",
    "#548E7A", "#467A69", "#386658", "#2A5246", "#1C3E35"
]


max_sensors = -1

de_file = open(args.de_out_file, 'r')
hs_file = open(args.hs_out_file, 'r')
x_de, y_de = get_output_splited(de_file.readlines()[0])
x_hs, y_hs_avg = get_output_splited(hs_file.readlines()[10])
max_sensors = max(max(x_de), max(x_hs))

## color
# plt.scatter(x_de, y_de, s=50, color=colors[9], marker='o', label='Differential Evolution')
# plt.scatter(x_hs, y_hs_avg, s=50, color=colors[2], marker='o', label='Harmony Search')
## bw
plt.scatter(x_de, y_de, s=40, facecolors='none', edgecolors='black', marker='o', label='Differential Evolution')
plt.scatter(x_hs, y_hs_avg, s=15, color='black', marker='.', label='Harmony Search')

plt.title(f"Comparação DE e HS: Instância {args.instance}")
plt.legend()    
plt.xlim(0, max_sensors + 1)
plt.ylim(0, 1.1)
plt.xlabel('Número de sensores')
plt.ylabel('% de cobertura')
plt.hlines(y=1, linestyles='--', xmin=0, xmax=max_sensors+1, colors='#000', alpha=0.15)
plt.tight_layout()
plt.savefig(args.save_path, dpi=300)
