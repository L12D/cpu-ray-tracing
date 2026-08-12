#include "Scene.hpp"


Scene::Scene(int sceneIndex) {
    if (sceneIndex == 1) {
        Object *light = new Object(new Sphere({-2, 4, 0}, 1), {4.0, 4.0, 4.0});
        light->setLight();
        objects.push_back(light);

        objects.push_back(new Object(new Sphere({1, 4, 0}, 1), {0.0, 0.0, 1.0}));
        objects.push_back(new Object(new Sphere({0, 4, -4}, 3), {1.0, 1.0, 1.0}));
    } else if (sceneIndex == 2) {
        Object *light1 = new Object(new Sphere({-2.5, 6, 0}, 1.5), {8.0, 8.0, 8.0});
        light1->setLight();
        objects.push_back(light1);

        Object *sphere = new Object(new Sphere({0, 14, 0}, 5), {1.0, 1.0, 1.0});
        sphere->setMirror();
        objects.push_back(sphere);

        Object* bishop = new Object(new Mesh("assets/bishop.obj"), {0.5, 1.0, 0.5});
        bishop->scale({0.8, 0.8, 0.8});
        bishop->translate({2.5, 7, -3});
        objects.push_back(bishop);
    } else if (sceneIndex == 3) {
        Object *light = new Object(new Sphere({-2.5, 7, 0}, 2.0), {10.0, 10.0, 10.0});
        light->setLight();
        objects.push_back(light);

        Object* bishop1 = new Object(new Mesh("assets/bishop.obj"), {0.5, 1.0, 0.5});
        bishop1->scale({0.8, 0.8, 0.8});
        bishop1->translate({2.5, 5.5, -3});
        objects.push_back(bishop1);

        Object* bishop2 = new Object(new Mesh("assets/bishop.obj"), {1.0, 0.5, 0.5});
        bishop2->scale({0.8, 0.8, 0.8});
        bishop2->translate({2.5, 8, -3});
        objects.push_back(bishop2);
    } else if (sceneIndex == 4) {
        Object *light1 = new Object(new Sphere({-2.3, 0.8, 0.0}, 1.0), {7.0, 7.0, 7.0});
        light1->setLight();
        objects.push_back(light1);

        Object *light2 = new Object(new Sphere({2.3, 0.8, 0.0}, 1.0), {2.0, 2.0, 10.0});
        light2->setLight();
        objects.push_back(light2);

        Object *light3 = new Object(new Sphere({0.0, 0.0, 2.0}, 1.0), {2.0, 2.0, 2.0});
        light3->setLight();
        objects.push_back(light3);

        Object* lion = new Object(new Mesh("assets/lion05.obj"), {0.7, 0.7, 0.7});
        // lion->scale({1.5, 1.5, 1.5});
        lion->rotate({1.0, 0.0, 0.0}, 90.0f);
        lion->translate({0.0, 1.0, -0.2});
        // lion->translate({0.0, 1.0, -0.3});
        objects.push_back(lion);
    } else if (sceneIndex == 5) {
        Object *wall1 = new Object(new Cube(), {1.0, 1.0, 1.0});
        wall1->scale({1.0, 0.1, 1.0});
        wall1->translate({0.0, 1.5, 0.0});
        objects.push_back(wall1);

        Object *wall2 = new Object(new Cube(), {0.1, 1.0, 0.1});
        wall2->scale({0.1, 1.0, 1.0});
        wall2->translate({-0.5, 1.0, 0.0});
        objects.push_back(wall2);

        Object *wall3 = new Object(new Cube(), {1.0, 0.1, 0.1});
        wall3->scale({0.1, 1.0, 1.0});
        wall3->translate({0.5, 1.0, 0.0});
        objects.push_back(wall3);

        Object *floor = new Object(new Cube(), {0.1, 0.1, 1.0});
        floor->scale({1.0, 1.0, 0.1});
        floor->translate({0.0, 1.0, -0.5});
        objects.push_back(floor);

        Object *cieling = new Object(new Cube(), {1.0, 1.0, 1.0});
        cieling->scale({1.0, 1.0, 0.1});
        cieling->translate({0.0, 1.0, 0.5});
        objects.push_back(cieling);

        Object *light = new Object(new Cube(), {5.0, 5.0, 5.0});
        light->setLight();
        light->scale({0.5, 0.5, 0.1});
        light->translate({0.0, 1.0, 0.48});
        objects.push_back(light);

        Object* lion = new Object(new Mesh("assets/lion05.obj"), {1.0, 1.0, 1.0});
        lion->setMirror();
        lion->scale({1.3, 1.3, 1.3});
        lion->rotate({1.0, 0.0, 0.0}, 90.0f);
        lion->translate({0.0, 1.3, -0.3});
        objects.push_back(lion);
    }
}


std::vector<Object*> Scene::getObjects() {
    return objects;
}


void Scene::render(Camera *camera, cv::Mat &image) {
    int maxDepth = 3;
    for (int i = 0; i < camera->get_height(); ++i) {
        for (int j = 0; j < camera->get_width(); ++j) {
            if (j == 0 and i % 10 == 0) {
                std::cout << "\rRendering row " << i << " of " << camera->get_height() << std::flush;
            }
            ray ray = camera->get_ray(i, j);
            float rayLength = FLOAT_MAX;
            float3 color = BC_COLOR_2;

            HitInfo hit;
            hit.renderDepth = 0;
            hit.actualDepth = 0;
            Object *closestObject = nullptr;
            float3 position;
            float3 normal;
            for (Object *object : objects) {
                object->intersect(ray, hit);
                if (hit.distance < rayLength) {
                    rayLength = hit.distance;
                    position = hit.position;
                    normal = hit.normal;
                    closestObject = object;
                }
            }

            if (closestObject == nullptr) {
                // No intersection, use background color
                color = BC_COLOR_1;
            } else {
                // Otherwise, calculate the color based on the object's material and lighting
                color = closestObject->getRayColor(position, normal, ray.direction, hit.renderDepth, hit.actualDepth);
                // color = {0.0, 0.0, 1.0};
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
