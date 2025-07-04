#pragma once


#include "Camera.hpp"
#include "Scene.hpp"


class Scene;


class Application {

    private :

        static Application *instance;
        Camera* camera;
        Scene *scene;
        std::vector<float3> directions;
        Application();

    public :

        static Application *getInstance();
        Camera *getCamera();
        Scene *getScene();
        std::vector<float3> generateDirections(int n);
        std::vector<float3> getDirections();
        void render(cv::Mat &image);
        ~Application();

};
