#pragma once


#include <vector>

#include "utils.hpp"


class Camera {

    private :

        int width, height; // in pixels
        mat4 view;
        float fov;
        std::vector<std::vector<ray>> *rays;

    public :

        Camera(int resolution);
        int get_width();
        int get_height();
        ray get_ray(int i, int j);
        mat4 viewMatrix();
        void translate(float3 t);
        ~Camera();

};
