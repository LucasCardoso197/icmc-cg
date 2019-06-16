#include <cg_utils.hpp>
#include <cmath>

void normalizeVector(float *vec){
    float len = 0;
    for(int i=0; i<3; i++)
        len += vec[i]*vec[i];
    len = sqrt(len);

    for(int i=0; i<3; i++)
        vec[i] /= len;
}

void crossProduct(float *a, float *b, float *res){
    res[0] = a[1]*b[2] - a[2]*b[1];
    res[1] = a[2]*b[0] - a[0]*b[2];
    res[2] = a[0]*b[1] - a[1]*b[0];
}

float dotProduct(float *a, float *b){
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}