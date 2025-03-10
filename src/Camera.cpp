#include "Camera.hpp"


Camera::Camera() {
    width = 1920;
    height = 1080;
    view = id();
    fov = 90;
    rays = std::vector<std::vector<float3>>(width, std::vector<float3>(height));
    float gap = 2*std::sin(fov/2)/height;
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            float x = (j - width/2) * gap;
            float y = 1.0;
            float z = (i - height/2) * gap;
            float3 ray = normalize({x, y, z});
            rays[i][j] = ray;
        }
    }
}


int Camera::get_width() {
    return width;
}


int Camera::get_height() {
    return height;
}


float3 Camera::get_ray(int i, int j) {
    return rays[i][j];
}
