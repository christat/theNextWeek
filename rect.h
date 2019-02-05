#ifndef RECTH
#define RECTH

#include "aabb.h"
#include "hitable.h"
#include "vec3.h"

class xy_rect : public hitable {
public:
    xy_rect() {}
    xy_rect(float x0, float x1, float y0, float y1, float z_pos, material* matptr): x_min(x0), x_max(x1), y_min(y0), y_max(y1), z(z_pos), mat(matptr) {}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        float t = (z - r.origin().z()) / r.direction().z();
        if (t < t_min || t > t_max) return false;
        float x = r.origin().x() + t * r.direction().x();
        float y = r.origin().y() + t * r.direction().y();
        if (x < x_min || x > x_max || y < y_min || y > y_max) return false;
        rec.u = (x - x_min) / (x_max - x_min);
        rec.v = (y - y_min) / (y_max - y_min);
        rec.t = t;
        rec.mat_ptr = mat;
        rec.p = r.point_at_parameter(t);
        rec.normal = vec3(0,0,1);
        return true;
    }
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box = aabb(vec3(x_min, y_min, z - 0.001), vec3(x_max, y_max, z + 0.001));
        return true;
    }

private:
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    float z;
    material* mat;
};

class xz_rect : public hitable {
public:
    xz_rect() {}
    xz_rect(float x0, float x1, float z0, float z1, float y_pos, material* matptr): x_min(x0), x_max(x1), z_min(z0), z_max(z1), y(y_pos), mat(matptr) {}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        float t = (y - r.origin().y()) / r.direction().y();
        if (t < t_min || t > t_max) return false;
        float x = r.origin().x() + t * r.direction().x();
        float z = r.origin().z() + t * r.direction().z();
        if (x < x_min || x > x_max || z < z_min || z > z_max) return false;
        rec.u = (x - x_min) / (x_max - x_min);
        rec.v = (z - z_min) / (z_max - z_min);
        rec.t = t;
        rec.mat_ptr = mat;
        rec.p = r.point_at_parameter(t);
        rec.normal = vec3(0,1,0);
        return true;
    }
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box = aabb(vec3(x_min, y - 0.001, z_min), vec3(x_max, y + 0.001, z_max));
        return true;
    }

private:
    float x_min;
    float x_max;
    float z_min;
    float z_max;
    float y;
    material* mat;
};

class yz_rect : public hitable {
public:
    yz_rect() {}
    yz_rect(float y0, float y1, float z0, float z1, float x_pos, material* matptr): y_min(y0), y_max(y1), z_min(z0), z_max(z1), x(x_pos), mat(matptr) {}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        float t = (x - r.origin().x()) / r.direction().x();
        if (t < t_min || t > t_max) return false;
        float y = r.origin().y() + t * r.direction().y();
        float z = r.origin().z() + t * r.direction().z();
        if (y < y_min || y > y_max || z < z_min || z > z_max) return false;
        rec.u = (y - y_min) / (y_max - y_min);
        rec.v = (z - z_min) / (z_max - z_min);
        rec.t = t;
        rec.mat_ptr = mat;
        rec.p = r.point_at_parameter(t);
        rec.normal = vec3(1,0,0);
        return true;
    }
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        box = aabb(vec3(x - 0.001, y_min, z_min), vec3(x + 0.001, y_max, z_max));
        return true;
    }

private:
    float y_min;
    float y_max;
    float z_min;
    float z_max;
    float x;
    material* mat;
};

#endif