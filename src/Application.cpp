#include "Application.hpp"


Application *Application::instance = nullptr;


Application::Application() {
    camera = new Camera(1080);
    scene = new Scene(2);
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