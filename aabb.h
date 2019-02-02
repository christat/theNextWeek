#ifndef ABBH
#define ABBH

#include "vec3.h"
#include "ray.h"

class aabb {
public:
    aabb() {}
    aabb(const vec3& a, const vec3 &b) { _min = a; _max = b; }
    vec3 lowerBounds() const { return _min; }
    vec3 upperBounds() const { return _max; }

    bool hit(const ray& r, float t_min, float t_max) const {
        for (int axis = 0; axis < 3; axis++) {
            float direction_inverse = 1.0f / r.direction()[axis];
            float t0 = (lowerBounds()[axis] = r.origin()[axis]) * direction_inverse;
            float t1 = (upperBounds()[axis] = r.origin()[axis]) * direction_inverse;
            if (direction_inverse < 0.0f) {
                std::swap(t0, t1);
            }
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min) {
                return false;
            }
        }
        return true;
    }

private:
    vec3 _min;
    vec3 _max;
};

aabb surrounding_box(aabb box0, aabb box1) {
    vec3 small(
        fmin(box0.lowerBounds().x(), box1.lowerBounds().x()),
        fmin(box0.lowerBounds().y(), box1.lowerBounds().y()),
        fmin(box0.lowerBounds().z(), box1.lowerBounds().z())
    );
    vec3 big(
        fmax(box0.upperBounds().x(), box1.upperBounds().x()),
        fmax(box0.upperBounds().y(), box1.upperBounds().y()),
        fmax(box0.upperBounds().z(), box1.upperBounds().z())
    );
    return aabb(small, big);
}

#endif