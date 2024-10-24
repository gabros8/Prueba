#include "vector.h"

vec4_t vec4_from_vec3(vec3_t v) {
    vec4_t vec4 = {v.x, v.y, v.z, 1};
    return vec4;
}

vec3_t vec3_from_vec4(vec4_t v) {
    vec3_t vec3 = {v.x, v.y, v.z};
    return vec3;
}