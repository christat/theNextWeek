//==================================================================================================
// Written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is distributed
// without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication along
// with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==================================================================================================

#ifndef HITABLEH
#define HITABLEH 

#include "aabb.h"
#include "ray.h"

class material;

struct hit_record
{
    float t;
    float u;
    float v;
    vec3 p;
    vec3 normal; 
    material *mat_ptr;
};

class hitable  {
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
        virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;
};

class flip_normals : public hitable {
public:
    flip_normals() {}
    flip_normals(hitable* child): ht(child) {}
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
        if (ht->hit(r, t_min, t_max, rec)) {
            rec.normal = -rec.normal;
            return true;
        }
        return false;
    }
    virtual bool bounding_box(float t0, float t1, aabb& box) const {
        return ht->bounding_box(t0, t1, box);
    }
private:
    hitable *ht;
};

#endif




