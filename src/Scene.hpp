#pragma once


#include <vector>
#include <opencv2/opencv.hpp>

#include "utils.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "Sphere.hpp"


class Object;


class Scene {

    private :

        static Scene *instance;
        float3 background_color;
        std::vector<Object*> objects;
        Scene();

    public :

        static Scene *getInstance();
        std::vector<Object*> getObjects();
        void render(Camera *camera, cv::Mat &image);
        ~Scene();

};
