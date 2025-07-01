#include "Scene.hpp"


Scene::Scene(int sceneIndex) {
    if (sceneIndex == 1) {
        backgroundColor = {0.2, 0.2, 0.2};

        Object *light = new Object(new Sphere({-2, 4, 0}, 1), {4.0, 4.0, 4.0});
        light->setLight();
        objects.push_back(light);

        objects.push_back(new Object(new Sphere({1, 4, 0}, 1), {0.0, 0.0, 1.0}));
        objects.push_back(new Object(new Sphere({0, 4, -4}, 3), {1.0, 1.0, 1.0}));
    } else if (sceneIndex == 2) {
        backgroundColor = {0.3, 0.3, 0.3};

        Object *light1 = new Object(new Sphere({-2.5, 6, 0}, 1.5), {10.0, 10.0, 10.0});
        light1->setLight();
        objects.push_back(light1);

        Object *sphere = new Object(new Sphere({0, 14, 0}, 5), {1.0, 1.0, 1.0});
        sphere->setMirror();
        objects.push_back(sphere);

        Object* bishop = new Object(new TriangleSet("assets/bishop.obj"), {0.5, 1.0, 0.5});
        bishop->scale({0.8, 0.8, 0.8});
        bishop->translate({2.5, 7, -3});
        objects.push_back(bishop);
    } else if (sceneIndex == 3) {
        backgroundColor = {0.3, 0.3, 0.3};

        Object *light = new Object(new Sphere({-2.5, 7, 0}, 2.0), {10.0, 10.0, 10.0});
        light->setLight();
        objects.push_back(light);

        Object* bishop1 = new Object(new TriangleSet("assets/bishop.obj"), {0.5, 1.0, 0.5});
        bishop1->scale({0.8, 0.8, 0.8});
        bishop1->translate({2.5, 5.5, -3});
        objects.push_back(bishop1);

        Object* bishop2 = new Object(new TriangleSet("assets/bishop.obj"), {1.0, 0.5, 0.5});
        bishop2->scale({0.8, 0.8, 0.8});
        bishop2->translate({2.5, 8, -3});
        objects.push_back(bishop2);
    } else if (sceneIndex == 4) {
        backgroundColor = {0.3, 0.3, 0.3};

        Object *light = new Object(new Sphere({0.0, 0.0, 2.0}, 1.0), {10.0, 10.0, 10.0});
        light->setLight();
        objects.push_back(light);

        Object* lion = new Object(new TriangleSet("assets/lion.obj"), {0.7, 0.7, 0.7});
        // lion->scale({0.8, 0.8, 0.8});
        lion->rotate({1.0, 0.0, 0.0}, 90.0f);
        lion->translate({0.0, 1.0, -0.2});
        objects.push_back(lion);
    }
}


std::vector<Object*> Scene::getObjects() {
    return objects;
}


float3 Scene::getBackgroundColor() {
    return backgroundColor;
}


void Scene::render(Camera *camera, cv::Mat &image) {
    int maxDepth = 3;
    for (int i = 0; i < camera->get_height(); ++i) {
        for (int j = 0; j < camera->get_width(); ++j) {
            if (j == 0 and i % 10 == 0) {
                std::cout << "\rRendering row " << i << " of " << camera->get_height() << std::flush;
            }
            Ray *ray = camera->get_ray(i, j);
            float rayLength = std::numeric_limits<float>::max();
            float3 color = backgroundColor;

            HitInfo hit;
            Object *closestObject = nullptr;
            float3 position;
            float3 normal;
            for (Object *object : objects) {
                object->intersect(ray, hit, 0, maxDepth);
                if (hit.distance < rayLength) {
                    rayLength = hit.distance;
                    position = hit.position;
                    normal = hit.normal;
                    closestObject = object;
                }
            }

            if (closestObject == nullptr) {
                // No intersection, use background color
                color = backgroundColor;
            } else {
                // Otherwise, calculate the color based on the object's material and lighting
                color = closestObject->getRayColor(position, normal, ray->getDirection(), 0, maxDepth);
            }
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(color.x*255, color.y*255, color.z*255);
        }
    }
    std::cout << std::endl;
}


Scene::~Scene() {
    for (auto object : objects) {
        delete object;
    }
}
