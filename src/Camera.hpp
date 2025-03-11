#pragma once


#include <vector>

#include "utils.hpp"
#include "Ray.hpp"


class Camera {

    private :

        int width, height; // in pixels
        mat4 view;
        float fov;
        std::vector<std::vector<Ray *>> rays;

    public :

        Camera();
        int get_width();
        int get_height();
        Ray *get_ray(int i, int j);
        mat4 viewMatrix();
        void translate(float3 t);
        ~Camera();

};
