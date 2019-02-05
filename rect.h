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

#endif