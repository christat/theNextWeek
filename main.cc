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

#include <iostream>
#include "sphere.h"
#include "hitable.h"
#include "hitable_list.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "moving_sphere.h"
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "rect.h"

vec3 color(const ray& r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec)) { 
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
             return emitted + attenuation*color(scattered, world, depth+1);
        } else {
            return emitted;
        }
    }
    else {
        //vec3 unit_direction = unit_vector(r.direction());
        //float t = 0.5*(unit_direction.y() + 1.0);
        //return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
        return vec3(0,0,0);
    }
}


hitable *random_scene() {
    int n = 500;
    hitable **list = new hitable*[n+1];
    texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
    list[0] =  new sphere(vec3(0,-1000,0), 1000, new lambertian(checker));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = drand48();
            vec3 center(a+0.9*drand48(),0.2,b+0.9*drand48()); 
            if ((center-vec3(4,0.2,0)).length() > 0.9) { 
                if (choose_mat < 0.8) {  // diffuse
                    list[i++] = new moving_sphere(center, center + vec3(0, 0.5 * drand48(), 0), 0.0, 1.0, 0.2, new lambertian(new constant_texture(vec3(drand48()*drand48(), drand48()*drand48(), drand48()*drand48()))));
                }
                else if (choose_mat < 0.95) { // metal
                    list[i++] = new sphere(center, 0.2,
                            new metal(vec3(0.5*(1 + drand48()), 0.5*(1 + drand48()), 0.5*(1 + drand48())),  0.5*drand48()));
                }
                else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(list,i);
}

hitable *simple_light() {
    int tx, ty, tn;
    // image courtesy of Tom Patterson, www.shadedrelief.com
    unsigned char *tex_data = stbi_load("img/earth_texture.jpg", &tx, &ty, &tn, 0);
    texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.2, 0.2)), new constant_texture(vec3(0.9, 0.9, 0.9)));
    hitable** list = new hitable*[4];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(new image_texture(tex_data, tx, ty)));
    list[2] = new sphere(vec3(0,7,0), 2, new diffuse_light(new constant_texture(vec3(4,4,3))));
    list[3] = new xy_rect(3,5,1,3,-2, new diffuse_light(new constant_texture(vec3(4,4,4))));

    return new hitable_list(list, 4);
}

hitable *cornell_box() {
    hitable **list = new hitable*[6];
    int i = 0;
    material *red = new lambertian(new constant_texture(vec3(0.65,0.05,0.05)));
    material *white = new lambertian(new constant_texture(vec3(0.73,0.73,0.73)));
    material *green = new lambertian(new constant_texture(vec3(0.12,0.45,0.15)));
    material *light = new diffuse_light(new constant_texture(vec3(15,15,15)));
    list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
    list[i++] = new xz_rect(213, 343, 227, 332, 554, light);
    list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
    list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
    return new hitable_list(list, i);
}

int main() {
    int nx = 1200;
    int ny = 800;
    int ns = 1000;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    //hitable *list[5];
    //float R = cos(M_PI/4);
    //int tx, ty, tn;
    // image courtesy of Tom Patterson, www.shadedrelief.com
    //unsigned char *tex_data = stbi_load("img/earth_texture.jpg", &tx, &ty, &tn, 0);
    //material *earth_mat = new lambertian(new image_texture(tex_data, tx, ty));
    //list[0] = new sphere(vec3(0,0,-1), 0.5, earth_mat);
    //texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.2, 0.2)), new constant_texture(vec3(0.9, 0.9, 0.9)));
    //list[1] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(checker));
    //list[2] = new sphere(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
    //list[3] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    //list[4] = new sphere(vec3(-1,0,-1), -0.45, new dielectric(1.5));
    //hitable *world = new hitable_list(list,5);
    //world = random_scene();
    //hitable *world = simple_light();
    hitable *world = cornell_box();

    vec3 lookfrom(278,278,-800);
    vec3 lookat(278,278,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    int vfov = 40;

    camera cam(lookfrom, lookat, vec3(0,1,0), vfov, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);

    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s=0; s < ns; s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                //vec3 p = r.point_at_parameter(2.0);
                col += color(r, world,0);
            }
            col /= float(ns);
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            int ir = int(255.99*col[0]); 
            int ig = int(255.99*col[1]); 
            int ib = int(255.99*col[2]); 
            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}



