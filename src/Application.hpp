#pragma once


#include "Camera.hpp"
#include "Scene.hpp"
#include "RandomGenerator.hpp"


class Scene;


class Application {

    private :

        static Application *instance;
        Camera* camera;
        Scene *scene;
        RandomGenerator *randomGenerator;
        Application();

    public :

        static Application *getInstance();
        Camera *getCamera();
        Scene *getScene();
        RandomGenerator *getRandomGenerator();
        void render(cv::Mat &image);
        ~Application();

};
