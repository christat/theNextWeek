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

class image_texture: public texture {
public:
    image_texture() {}
    image_texture(unsigned char* pixels, int A, int B): data(pixels), nx(A), ny(B) {}
    vec3 value(float u, float v, const vec3& p) const {
        int i = u * nx;
        int j = (1-v) * ny - 0.001;
        if (i < 0) i = 0;
        if (j < 0) j = 0;
        if (i > nx-1) i = nx-1;
        if (j > ny-1) j = ny-1;
        float r = int(data[3*i + 3*nx*j]) / 255.0;
        float g = int(data[3*i + 3*nx*j + 1]) / 255.0;
        float b = int(data[3*i + 3*nx*j + 2]) / 255.0;
        return vec3(r, g, b);
    }

private:
    unsigned char *data;
    int nx;
    int ny;
};

#endif