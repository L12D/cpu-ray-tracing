#pragma once


#include "Camera.hpp"
#include "Scene.hpp"


class Scene;


class Application {

    private :

        static Application *instance;
        Camera* camera;
        Scene *scene;
        Application();

    public :

        static Application *getInstance();
        Camera *getCamera();
        Scene *getScene();
        void render(cv::Mat &image);
        ~Application();

};
