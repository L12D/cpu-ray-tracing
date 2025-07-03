#include "Camera.hpp"


Camera::Camera(int resolution) {
    height = resolution;
    width = resolution * 16 / 9;
    view = id();
    fov = M_PI / 3;
    rays = new std::vector<std::vector<ray>>(height, std::vector<ray>(width));
    float gap = 2*std::tan(fov/2)/height;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float x = (j - width/2) * gap;
            float y = 1.0;
            float z = -(i - height/2) * gap;
            float3 dir = normalize({x, y, z});
            (*rays)[i][j] = ray({0, 0, 0}, dir);
        }
    }
}


int Camera::get_width() {
    return width;
}


int Camera::get_height() {
    return height;
}


ray Camera::get_ray(int i, int j) {
    return (*rays)[i][j];
}


mat4 Camera::viewMatrix() {
    return view;
}


void Camera::translate(float3 t) {
    view = mTranslate(-t)*view;
}


Camera::~Camera() {
    delete rays;
}
