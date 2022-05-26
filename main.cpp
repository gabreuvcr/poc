#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

typedef struct sensor {
    double x, y;
    sensor(double _x, double _y) : x(_x), y(_y) {}
    sensor(pair<double, double> point) : x(point.first), y(point.second) {}
    sensor() : x(-1), y(-1) {}
} sensor;

random_device rd;
mt19937 rng(rd());
uniform_real_distribution<> dist_0_1(0, 1);

vector<vector<sensor>> HM;

pair<double, double> random_point(double x_l, double x_u, double y_l, double y_u) {
    double x = x_l + (x_u - x_l) * dist_0_1(rng);
    double y = y_l + (y_u - y_l) * dist_0_1(rng);
    pair<double, double> point(x, y);
    return point;
}

int main() {
    cout.precision(1);
    cout << fixed;

    double sensor_radius = 2;
    int W = 10;
    int H = 10;

    double x_lower = sensor_radius;
    double y_lower = sensor_radius;

    double x_upper = W - sensor_radius;
    double y_upper = H - sensor_radius;

    int min_sensors = 1;
    int max_sensors = (W / (2 * sensor_radius)) * (H / (2 * sensor_radius));

    int HM_size = 10;

    cout << "max_sensors: " << max_sensors << endl;

    HM.resize(HM_size, vector<sensor>(max_sensors, sensor()));

    for (int vec = 0; vec < HM_size; vec++) {
        int HM_vector_size = round(min_sensors + (max_sensors - min_sensors) * dist_0_1(rng));
        
        for (int s = 0; s < HM_vector_size; s++) {
            HM[vec][s] = sensor(random_point(x_lower, x_upper, y_lower, y_upper));
        }
        shuffle(begin(HM[vec]), end(HM[vec]), rng);
    }

    for (int vec = 0; vec < HM_size; vec++) {
        for (int s = 0; s < max_sensors; s++) {
            if (HM[vec][s].x == -1) {
                cout << "#" << " "; continue;
            }
            cout << "(" << HM[vec][s].x << "," << HM[vec][s].y << ") ";
        }
        cout << endl;
    }

    return 0;
}
