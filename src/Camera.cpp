#include "Camera.hpp"


Camera::Camera() {
    width = 1920;
    height = 1080;
    view = id();
    fov = M_PI / 2;
    rays = std::vector<std::vector<float3>>(height, std::vector<float3>(width));
    float gap = 2*std::tan(fov/2)/height;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float x = (j - width/2) * gap;
            float y = 1.0;
            float z = -(i - height/2) * gap;
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


mat4 Camera::viewMatrix() {
    return view;
}


void Camera::translate(float3 t) {
    view = mTranslate(-t)*view;
}
