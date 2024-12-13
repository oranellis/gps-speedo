#include "interp.h"

double Interp(double &start_val, double end_val, int &interp_count, int interp_num) {
    if (interp_count >= interp_num) {
        start_val = ((start_val * (1 - ((interp_num - 1) / interp_num))) + (end_val * ((interp_num - 1) / interp_num)));
        interp_count = 0;
    }

    double interp_mult = interp_count / interp_num;
    interp_count ++;
    return ((start_val * (1 - interp_mult)) + (end_val * interp_mult));
};
