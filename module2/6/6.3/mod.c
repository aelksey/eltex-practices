#include "mod.h"

double libmod(double a, double b){
    int result = (int)a % (int)b;
    return (double)result;
}