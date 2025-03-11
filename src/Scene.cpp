#include "Scene.hpp"


Scene::Scene() {
    background_color = {200, 200, 200};
    objects.push_back(new Object(new Circle({0, 2, 0}, 1)));
}


void Scene::render(Camera *camera, cv::Mat &image) {
    Circle *circle = (Circle *) objects[0]->getShape();
    for (int i = 0; i < camera->get_height(); ++i) {
        for (int j = 0; j < camera->get_width(); ++j) {
            float3 ray = camera->get_ray(i, j);
            if (circle->intersect(ray)) {
                image.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
            } else {
                image.at<cv::Vec3b>(i, j) = cv::Vec3b(50, 50, 50);
            }
            
        }
    }    
}


Scene::~Scene() {
    for (auto object : objects) {
        delete object;
    }
}
