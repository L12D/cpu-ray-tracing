#pragma once


#include <vector>
#include <opencv2/opencv.hpp>

#include "utils.hpp"
#include "Camera.hpp"
#include "Object.hpp"
#include "shapes/Sphere.hpp"
#include "shapes/Mesh.hpp"
#include "shapes/Cube.hpp"
#include "shapes/Quad.hpp"


class Object;


class Scene {

    private :

        std::vector<Object*> objects;
        
    public :

        Scene(int sceneIndex);
        std::vector<Object*> getObjects();
        void render(Camera *camera, cv::Mat &image);
        ~Scene();

};
