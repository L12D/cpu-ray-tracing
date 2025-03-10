#pragma once


#include <vector>

#include "utils.hpp"


class Camera {

    private :

        int width, height; // in pixels
        mat4 view;
        float fov;
        std::vector<std::vector<float3>> rays;

    public :

        Camera();
        int get_width();
        int get_height();
        float3 get_ray(int i, int j);
        ~Camera() = default;

};
