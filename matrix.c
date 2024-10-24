#include "matrix.h"
#include <math.h>

mat4_t mat4_identity(void) {
    mat4_t identidad = {{{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}}};
    return identidad;
}

mat4_t mat4_make_translation(float tx, float ty, float tz) {
    mat4_t traslacion = {{{1,0,0,tx}, {0,1,0,ty}, {0,0,1,tz}, {0,0,0,1}}};
    return traslacion;
}

mat4_t mat4_make_scale(float sx, float sy, float sz) {
    mat4_t escala = {{{sx,0,0,0}, {0,sy,0,0}, {0,0,sz,0}, {0,0,0,1}}};
    return escala;
}

mat4_t mat4_make_rotation_x(float angle) {
    mat4_t rotacionX = {{{1,0,0,0}, {0,cos(angle),-sin(angle),0}, {0,sin(angle),cos(angle),0}, {0,0,0,1}}};
    return rotacionX;
}

mat4_t mat4_make_rotation_y(float angle) {
    mat4_t rotacionY = {{{cos(angle),0,sin(angle),0}, {0,1,0,0}, {-sin(angle),0,cos(angle),0}, {0,0,0,1}}};
    return rotacionY;
}

mat4_t mat4_make_rotation_z(float angle) {
    mat4_t rotacionZ = {{{cos(angle),-sin(angle),0,0}, {sin(angle),cos(angle),0,0}, {0,0,1,0}, {0,0,0,1}}};
    return rotacionZ;
}

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
    float x,y,z,w;

    x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
    y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
    z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
    w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;
    vec4_t vectorRes = {x,y,z,w};
    return vectorRes;
}

mat4_t mat4_mul_mat4(mat4_t a, mat4_t b) {
    int i,j,k;
    float suma;
    mat4_t matrizRes;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            suma = 0;
            for (k = 0; k < 4; k++) {
                suma += a.m[i][k] * b.m[k][j];
            }
            matrizRes.m[i][j] = suma;
        }
    }
    return matrizRes;
}
