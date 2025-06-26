#include "Scene.hpp"


Scene::Scene(int sceneIndex) {
    if (sceneIndex == 1) {
        backgroundColor = {0.2, 0.2, 0.2};
        brightness = 1.5;
        backgroundColor = mul(brightness, backgroundColor);
        // objects.push_back(new Object(new Circle({0, 2, 1}, 1), {1.0, 1.0, 1.0}, false));
        // objects.push_back(new Object(new Sphere({-3, 4, 0}, 1), {1.0, 0.0, 0.0}, false));
        objects.push_back(new Object(new Sphere({-1, 4, 0}, 1), {1.0, 1.0, 1.0}));
        objects.push_back(new Object(new Sphere({1, 4, 0}, 1), {0.0, 0.0, 1.0}));
        objects.push_back(new Object(new Sphere({0, 4, -4}, 3), {1.0, 1.0, 1.0}));
        // for (int i = 0; i < 9; ++i) {
        //     for (int j = 0; j < 5; ++j) {
        //        objects.push_back(new Object(new Sphere((float3){(float)4*(i-4), 10, (float)4*(j-2)}, 1), {1.0, 1.0, 1.0}, (i+j)%2==0));
        //     }
        // }
    } else if (sceneIndex == 2) {
        backgroundColor = {0.1, 0.1, 0.1};
        brightness = 1.2;
        backgroundColor = mul(brightness, backgroundColor);

        Object *light1 = new Object(new Sphere({-3, 8, 0}, 2), {4.0, 4.0, 4.0});
        light1->setLight();
        objects.push_back(light1);

        Object *sphere = new Object(new Sphere({1, 12, 0}, 3), {0.8, 0.8, 0.8});
        sphere->setMirror();
        objects.push_back(sphere);

        Object* bishop = new Object(new TriangleSet("assets/bishop.obj"), {0.5, 1.0, 0.5});
        bishop->scale({0.8, 0.8, 0.8});
        bishop->translate({3, 8, -3});
        objects.push_back(bishop);
    }
}


std::vector<Object*> Scene::getObjects() {
    return objects;
}


float3 Scene::getBackgroundColor() {
    return backgroundColor;
}


float Scene::getBrightness() {
    return brightness;
}


void Scene::render(Camera *camera, cv::Mat &image) {
    for (int i = 0; i < camera->get_height(); ++i) {
        for (int j = 0; j < camera->get_width(); ++j) {
            if (j == 0 and i % 10 == 0) {
                std::cout << "Rendering row " << i << " of " << camera->get_height() << std::endl;
            }
            Ray *ray = camera->get_ray(i, j);
            float rayLength = 10000.0;
            float3 color = backgroundColor;
            for (Object *object : objects) {
                object->intersect(ray, 0, 4);
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
