#include "Scene.hpp"


Scene *Scene::instance = nullptr;


Scene::Scene() {
    background_color = {0.2, 0.2, 0.2};
    // objects.push_back(new Object(new Circle({0, 2, 1}, 1), {1.0, 1.0, 1.0}, false));
    objects.push_back(new Object(new Circle({0, 2, 0}, 1), {1.0, 1.0, 1.0}, true));
    objects.push_back(new Object(new Circle({-1, -2, 0}, 1), {1.0, 1.0, 1.0}, true));
    objects.push_back(new Object(new Circle({1, -2, 0}, 1), {1.0, 1.0, 1.0}, true));
    // for (int i = 0; i < 9; ++i) {
    //     for (int j = 0; j < 7; ++j) {
    //        objects.push_back(new Object(new Circle((float3){(float)4*(i-4), 10, (float)4*(j-3)}, 1), {1.0, 1.0, 1.0}, (i+j)%2==0));
    //     }
    // }
}


Scene *Scene::getInstance() {
    if (instance == nullptr) {
        instance = new Scene();
    }
    return instance;
}


std::vector<Object*> Scene::getObjects() {
    return objects;
}


void Scene::render(Camera *camera, cv::Mat &image) {
    for (int i = 0; i < camera->get_height(); ++i) {
        for (int j = 0; j < camera->get_width(); ++j) {
            Ray *ray = camera->get_ray(i, j);
            float rayLength = 10000.0;
            float3 color = background_color;
            for (Object *object : objects) {
                object->intersect(ray, 2);
                if (ray->getLength() < rayLength) {
                    rayLength = ray->getLength();
                    color = ray->getColor();
                }
            }
            // std::cout << "Pixel (" << i << ", " << j << "): Color = (" << color.x << ", " << color.y << ", " << color.z << ")" << std::endl;
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(color.x*255, color.y*255, color.z*255);
        }
    }    
}


Scene::~Scene() {
    for (auto object : objects) {
        delete object;
    }
}
