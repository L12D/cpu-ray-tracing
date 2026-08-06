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

        std::vector<Object*> objects;
        
    public :

        Scene(int sceneIndex);
        const std::vector<Object*>& getObjects() const;
        void render(Camera *camera, cv::Mat &image);
        ~Scene();

};
