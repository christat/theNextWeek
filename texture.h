#ifndef TEXTUREH
#define TEXTUREH

#include "vec3.h"

class texture {
public:
    virtual vec3 value(float u, float v, const vec3& p) const = 0;
};

class constant_texture: public texture {
public:
    constant_texture() {}
    constant_texture(vec3 c) : color(c) {}
    vec3 value(float u, float v, const vec3& p) const {
        return color;
    }

private:
    vec3 color;
};

class checker_texture: public texture {
public:
    checker_texture() {}
    checker_texture(texture *t0, texture *t1): even(t0), odd(t1) {}
    vec3 value(float u, float v, const vec3& p) const {
        float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0) {
            return odd->value(u, v, p);
        }
        return even->value(u, v, p);
    }

private:
    texture* even;
    texture* odd;
};

#endif