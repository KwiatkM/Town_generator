#include <raymath.h>


double dist_eucl(int x_src, int y_src, int x_dst, int y_dst) {
    return sqrt(pow(x_src - x_dst, 2) + pow(y_src - y_dst, 2)); // w pryzpadku tego programmu pierwiastek jest nie potrzebny
}

double dist_manh(int x_src, int y_src, int x_dst, int y_dst) {
    return abs(x_src - x_dst) + abs(y_src - y_dst);
}

double dist_max(int x_src, int y_src, int x_dst, int y_dst) {
    return abs(x_src - x_dst) > abs(y_src - y_dst) ? abs(x_src - x_dst) : abs(y_src - y_dst);
}