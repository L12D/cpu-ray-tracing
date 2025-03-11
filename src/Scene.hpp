#pragma once


#include <vector>
#include <opencv2/opencv.hpp>

#include "utils.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "Circle.hpp"


class Scene {

    private :

        float3 background_color;
        std::vector<Object*> objects;

    public :

        Scene();
        void render(Camera *camera, cv::Mat &image);
        ~Scene();

};
