#include "Application.hpp"


Application *Application::instance = nullptr;


Application::Application() {
    camera = new Camera(720);
    scene = new Scene(4);
}


Application *Application::getInstance() {
    if (instance == nullptr) {
        instance = new Application();
    }
    return instance;
}


Camera *Application::getCamera() {
    return camera;
}


Scene *Application::getScene() {
    return scene;
}


void Application::render(cv::Mat &image) {
    scene->render(camera, image);
}


Application::~Application() {
    delete camera;
    delete scene;
}