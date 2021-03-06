//main.cc
#include "Tracing.h"

#include "sphere.h"
#include "camera.h"
#include <iostream>
#include "bvh.h"
vec3 ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return vec3(0, 0, 0);

    if (world.hit(r, 0.0000001, infinity, rec)) {
        ray scattered;
        vec3 attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return vec3(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}
hittable_list random_scene() {
    hittable_list world;

    world.add(make_shared<sphere>(
        vec3(0, -1000, 0), 1000, make_shared<lambertian>(vec3(0.5, 0.5, 0.5))));

    int i = 1;
    for (int a = -10; a < 10; a++) {
        for (int b = -10; b < 10; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());
            if ((center - vec3(4, .2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = vec3::random() * vec3::random();
                    world.add(make_shared<moving_sphere>(
                        center, center + vec3(0, random_double(0, .5), 0), 0.0, 1.0, 0.2,
                        make_shared<lambertian>(albedo)));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = vec3::random(.5, 1);
                    auto fuzz = random_double(0, .5);
                    world.add(
                        make_shared<sphere>(center, 0.2, make_shared<metal>(albedo, fuzz)));
                }
                else {
                    // glass
                    world.add(make_shared<sphere>(center, 0.2, make_shared<dielectric>(1.5)));
                }
            }
        }
    }

    world.add(make_shared<sphere>(vec3(0, 1, 0), 1.0, make_shared<dielectric>(1.5)));
    world.add(make_shared<sphere>(
        vec3(-4, 1, 0), 1.0, make_shared<lambertian>(vec3(0.4, 0.2, 0.1))));
    world.add(make_shared<sphere>(
        vec3(4, 1, 0), 1.0, make_shared<metal>(vec3(0.7, 0.6, 0.5), 0.0)));

    return static_cast<hittable_list>(make_shared<bvh_node>(world, 0, 1));
}

double tu[1300][800][3];
int main() {
    const int image_width = 1200;
    const int image_height = 600;
    const int samples_per_pixel = 500;
    const int max_depth = 50;
    const auto aspect_ratio = double(image_width) / image_height;

    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    //auto dist_to_focus = (lookfrom - lookat).length();
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;    
    camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus,0.0,1.0);
    auto world = random_scene();
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            vec3 color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / image_width;
                auto v = (j + random_double()) / image_height;
                ray r = cam.get_ray(u, v);
                color += ray_color(r, world, max_depth);
            }
            auto scale = 1.0 / samples_per_pixel;
            auto r = sqrt(scale * color.x());
            auto g = sqrt(scale * color.y());
            auto b = sqrt(scale * color.z());
            tu[i][j][0] = r; tu[i][j][1] = g; tu[i][j][2] = b;

            //zcolor.write_color(std::cout, samples_per_pixel);
        }
    }

    
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            std::cout << static_cast<int>(256 * clamp(tu[i][j][0], 0.0, 0.999)) << ' '
                << static_cast<int>(256 * clamp(tu[i][j][1], 0.0, 0.999)) << ' '
                << static_cast<int>(256 * clamp(tu[i][j][2], 0.0, 0.999)) << '\n';
        }
    }
    std::cerr << "\nDone.\n";
}