#pragma once


#include <vector>
#include <opencv2/opencv.hpp>

#include "utils.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "Sphere.hpp"
#include "TriangleSet.hpp"


class Object;


class Scene {

    private :

        float3 backgroundColor;
        std::vector<Object*> objects;
        
    public :

        Scene(int sceneIndex);
        std::vector<Object*> getObjects();
        float3 getBackgroundColor();
        void render(Camera *camera, cv::Mat &image);
        ~Scene();

};
