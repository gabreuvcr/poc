#include "HarmonySearch.h"

int main() {
    int W = 50;
    int H = 50;
    int HM_size = 5;
    double sensor_radius = 10;
    double uncertainty_sensor = 5;

    HarmonySearch hs(W, H, HM_size, sensor_radius, uncertainty_sensor);
    
    hs.init_harmony_memory();
    hs.cout_harmony_memory();
    
    return 0;
}
