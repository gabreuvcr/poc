import matplotlib.pyplot as plt
import argparse

#python3 distribution_plot.py -o=outputs/de_g1_distribution.out -s=distribution_de_g1 -i=1 -n=1 -hm="Differential Evolution"
#python3 distribution_plot.py -o=outputs/hs_test.out -s=distribution_hs_g1 -i=1 -n=10 -hm="Harmony Search" -std

def get_args():
    parser = argparse.ArgumentParser(description="Plot arguments")
    parser.add_argument(
        '-o', '--out_file',
        help='output file',
        dest='out_file',
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
    parser.add_argument(
        '-n', '--number-of-runs',
        help='number of runs',
        dest='n_runs',
        action='store',
        type=int,
        required=True,
    )
    parser.add_argument(
        '-hm', '--heuristic-name',
        help='heuristic name',
        dest='heuristic_name',
        action='store',
        type=str,
        required=True,
    )
    parser.add_argument(
        '-std', '--standard-deviation',
        help='standard deviation',
        default=False,
        dest='std',
        action='store_true',
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

f = open(args.out_file, 'r')
lines = f.readlines()
for i in range(0, args.n_runs):
    x_de, y_de = get_output_splited(lines[i])
    max_sensors = max(max(x_de), max_sensors)
    
    plt.scatter(
        x_de, y_de, s=sizes[i], marker='o',
        label=f'{i + 1}', color=colors[i], edgecolor='None'
    )

plt.title(f"{args.heuristic_name}: Instância {args.instance}")
plt.legend()
plt.xlim(0, max_sensors + 1)
plt.ylim(0, 1.1)
plt.xlabel('Número de sensores')
plt.ylabel('% de cobertura')
plt.hlines(y=1, linestyles='--', xmin=0, xmax=max_sensors+1, colors='#000', alpha=0.15)
plt.tight_layout()
plt.savefig(args.save_path, dpi=300)
# plt.close()

if args.std:
    x, y = get_output_splited(lines[args.n_runs + 1])
    plt.plot(x, y, color='black', label='Desvio padrão')
    plt.title(f"{args.heuristic_name}: Instância {args.instance}")
    plt.legend()
    plt.xlabel('Número de sensores')
    plt.ylabel('% de cobertura')
    plt.tight_layout()
    plt.savefig(args.save_path + '_std', dpi=300)
    plt.close()
