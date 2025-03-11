#include "Scene.hpp"


Scene::Scene() {
    background_color = {50, 50, 50};
    objects.push_back(new Object(new Circle({0, 2, 0}, 1)));
}


void Scene::render(Camera *camera, cv::Mat &image) {
    for (int i = 0; i < camera->get_height(); ++i) {
        for (int j = 0; j < camera->get_width(); ++j) {
            Ray *ray = camera->get_ray(i, j);
            float rayLength = 10000.0;
            cv::Vec3b color = background_color;
            for (Object *object : objects) {
                object->intersect(ray);
                if (ray->getLength() < rayLength) {
                    rayLength = ray->getLength();
                    color = ray->getColor();
                }
            }
            image.at<cv::Vec3b>(i, j) = color;
        }
    }    
}


Scene::~Scene() {
    for (auto object : objects) {
        delete object;
    }
}
