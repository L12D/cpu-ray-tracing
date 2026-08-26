#include "Scene.hpp"

#include "Object.hpp"

#include <utility>


Scene::Scene(int sceneIndex) {
    if (sceneIndex == 1) {
        auto light = std::make_unique<Object>(std::make_unique<Sphere>(float3{-2, 4, 0}, 1), float3{4.0, 4.0, 4.0});
        light->setLight(true);
        m_objects.push_back(std::move(light));

        m_objects.push_back(std::make_unique<Object>(std::make_unique<Sphere>(float3{1, 4, 0}, 1), float3{0.0, 0.0, 1.0}));
        m_objects.push_back(std::make_unique<Object>(std::make_unique<Sphere>(float3{0, 4, -4}, 3), float3{1.0, 1.0, 1.0}));
    } else if (sceneIndex == 2) {
        auto light1 = std::make_unique<Object>(std::make_unique<Sphere>(float3{-2.5, 6, 0}, 1.5), float3{8.0, 8.0, 8.0});
        light1->setLight(true);
        m_objects.push_back(std::move(light1));

        auto sphere = std::make_unique<Object>(std::make_unique<Sphere>(float3{0, 14, 0}, 5), float3{1.0, 1.0, 1.0});
        sphere->setMirror(true);
        m_objects.push_back(std::move(sphere));

        auto bishop = std::make_unique<Object>(std::make_unique<Mesh>("assets/bishop.obj"), float3{0.5, 1.0, 0.5});
        bishop->scale({0.8, 0.8, 0.8});
        bishop->translate({2.5, 7, -3});
        m_objects.push_back(std::move(bishop));
    } else if (sceneIndex == 3) {
        auto light = std::make_unique<Object>(std::make_unique<Sphere>(float3{-2.5, 7, 0}, 2.0), float3{10.0, 10.0, 10.0});
        light->setLight(true);
        m_objects.push_back(std::move(light));

        auto bishop1 = std::make_unique<Object>(std::make_unique<Mesh>("assets/bishop.obj"), float3{0.5, 1.0, 0.5});
        bishop1->scale({0.8, 0.8, 0.8});
        bishop1->translate({2.5, 5.5, -3});
        m_objects.push_back(std::move(bishop1));

        auto bishop2 = std::make_unique<Object>(std::make_unique<Mesh>("assets/bishop.obj"), float3{1.0, 0.5, 0.5});
        bishop2->scale({0.8, 0.8, 0.8});
        bishop2->translate({2.5, 8, -3});
        m_objects.push_back(std::move(bishop2));
    } else if (sceneIndex == 4) {
        auto light1 = std::make_unique<Object>(std::make_unique<Sphere>(float3{-2.3, 0.8, 0.0}, 1.0), float3{7.0, 7.0, 7.0});
        light1->setLight(true);
        m_objects.push_back(std::move(light1));

        auto light2 = std::make_unique<Object>(std::make_unique<Sphere>(float3{2.3, 0.8, 0.0}, 1.0), float3{2.0, 2.0, 10.0});
        light2->setLight(true);
        m_objects.push_back(std::move(light2));

        auto light3 = std::make_unique<Object>(std::make_unique<Sphere>(float3{0.0, 0.0, 2.0}, 1.0), float3{2.0, 2.0, 2.0});
        light3->setLight(true);
        m_objects.push_back(std::move(light3));

        auto lion = std::make_unique<Object>(std::make_unique<Mesh>("assets/lion05.obj"), float3{0.7, 0.7, 0.7});
        // lion->scale({1.5, 1.5, 1.5});
        lion->rotate({1.0, 0.0, 0.0}, 90.0f);
        lion->translate({0.0, 1.0, -0.2});
        // lion->translate({0.0, 1.0, -0.3});
        m_objects.push_back(std::move(lion));
    } else if (sceneIndex == 5) {
        auto wall1 = std::make_unique<Object>(std::make_unique<Quad>(float3{-0.5, 1.5, -0.5}, float3{1.0, 0.0, 1.0}, float3{1.0, 0.0, 0.0}, float3{0.0, 0.0, 1.0}), float3{1.0, 1.0, 1.0});
        m_objects.push_back(std::move(wall1));

        auto wall2 = std::make_unique<Object>(std::make_unique<Quad>(float3{-0.5, 0.5, -0.5}, float3{0.0, 1.0, 1.0}, float3{0.0, 1.0, 0.0}, float3{0.0, 0.0, 1.0}), float3{0.1, 1.0, 0.1});
        m_objects.push_back(std::move(wall2));

        auto wall3 = std::make_unique<Object>(std::make_unique<Quad>(float3{0.5, 0.5, -0.5}, float3{0.0, 1.0, 1.0}, float3{0.0, 1.0, 0.0}, float3{0.0, 0.0, 1.0}), float3{1.0, 0.1, 0.1});
        m_objects.push_back(std::move(wall3));

        auto wall4 = std::make_unique<Object>(std::make_unique<Quad>(float3{-0.5, 0.5, -0.5}, float3{1.0, 0.0, 1.0}, float3{1.0, 0.0, 0.0}, float3{0.0, 0.0, 1.0}), float3{1.0, 1.0, 1.0});
        wall4->setInvisible(true);
        m_objects.push_back(std::move(wall4));

        auto floor = std::make_unique<Object>(std::make_unique<Quad>(float3{-0.5, 0.5, -0.5}, float3{1.0, 1.0, 0.0}, float3{1.0, 0.0, 0.0}, float3{0.0, 1.0, 0.0}), float3{1.0, 1.0, 1.0});
        m_objects.push_back(std::move(floor));

        auto cieling = std::make_unique<Object>(std::make_unique<Quad>(float3{-0.5, 0.5, 0.5}, float3{1.0, 1.0, 0.0}, float3{1.0, 0.0, 0.0}, float3{0.0, 1.0, 0.0}), float3{1.0, 1.0, 1.0});
        m_objects.push_back(std::move(cieling));

        auto light = std::make_unique<Object>(std::make_unique<Cube>(), float3{7.0, 7.0, 7.0});
        light->setLight(true);
        light->scale({0.4, 0.4, 0.1});
        light->translate({0.0, 1.0, 0.52});
        m_objects.push_back(std::move(light));

        auto lion = std::make_unique<Object>(std::make_unique<Mesh>("assets/lion05.obj"), float3{0.8, 0.8, 0.8});
        lion->scale({0.8, 0.8, 0.8});
        lion->rotate({1.0, 0.0, 0.0}, 90.0f);
        lion->translate({0.0, 1.2, -0.25});
        m_objects.push_back(std::move(lion));
    } else if (sceneIndex == 6) {
        auto wall1 = std::make_unique<Object>(std::make_unique<Quad>(float3{-0.5, 1.5, -0.5}, float3{1.0, 0.0, 1.0}, float3{1.0, 0.0, 0.0}, float3{0.0, 0.0, 1.0}), float3{1.0, 1.0, 1.0});
        wall1->setMirror(true);
        m_objects.push_back(std::move(wall1));

        auto wall2 = std::make_unique<Object>(std::make_unique<Quad>(float3{-0.5, 0.5, -0.5}, float3{0.0, 1.0, 1.0}, float3{0.0, 1.0, 0.0}, float3{0.0, 0.0, 1.0}), float3{0.1, 1.0, 0.1});
        m_objects.push_back(std::move(wall2));

        auto wall3 = std::make_unique<Object>(std::make_unique<Quad>(float3{0.5, 0.5, -0.5}, float3{0.0, 1.0, 1.0}, float3{0.0, 1.0, 0.0}, float3{0.0, 0.0, 1.0}), float3{1.0, 0.1, 0.1});
        m_objects.push_back(std::move(wall3));

        auto wall4 = std::make_unique<Object>(std::make_unique<Quad>(float3{-0.5, 0.5, -0.5}, float3{1.0, 0.0, 1.0}, float3{1.0, 0.0, 0.0}, float3{0.0, 0.0, 1.0}), float3{1.0, 1.0, 1.0});
        wall4->setInvisible(true);
        wall4->setMirror(true);
        m_objects.push_back(std::move(wall4));

        auto floor = std::make_unique<Object>(std::make_unique<Quad>(float3{-0.5, 0.5, -0.5}, float3{1.0, 1.0, 0.0}, float3{1.0, 0.0, 0.0}, float3{0.0, 1.0, 0.0}), float3{1.0, 1.0, 1.0});
        m_objects.push_back(std::move(floor));

        auto cieling = std::make_unique<Object>(std::make_unique<Quad>(float3{-0.5, 0.5, 0.5}, float3{1.0, 1.0, 0.0}, float3{1.0, 0.0, 0.0}, float3{0.0, 1.0, 0.0}), float3{1.0, 1.0, 1.0});
        m_objects.push_back(std::move(cieling));

        auto light = std::make_unique<Object>(std::make_unique<Cube>(), float3{7.0, 7.0, 7.0});
        light->setLight(true);
        light->scale({0.4, 0.4, 0.1});
        light->translate({0.0, 1.0, 0.52});
        m_objects.push_back(std::move(light));

        auto lion1 = std::make_unique<Object>(std::make_unique<Mesh>("assets/lion05.obj"), float3{0.8, 0.8, 0.8});
        lion1->scale({0.8, 0.8, 0.8});
        lion1->rotate({1.0, 0.0, 0.0}, 90.0f);
        lion1->rotate({0.0, 0.0, 1.0}, 90.0f);
        lion1->translate({-0.3, 1.0, -0.25});
        m_objects.push_back(std::move(lion1));

        auto lion2 = std::make_unique<Object>(std::make_unique<Mesh>("assets/lion05.obj"), float3{0.8, 0.8, 0.8});
        lion2->scale({0.8, 0.8, 0.8});
        lion2->rotate({1.0, 0.0, 0.0}, 90.0f);
        lion2->rotate({0.0, 0.0, -1.0}, 90.0f);
        lion2->translate({0.3, 1.0, -0.25});
        m_objects.push_back(std::move(lion2));
    }
}


const std::vector<std::unique_ptr<Object>>& Scene::getObjects() const noexcept {
    return m_objects;
}


Scene::~Scene() = default;


void Scene::render(const Camera& camera, cv::Mat& image) const {
    for (int i = 0; i < camera.getHeight(); ++i) {
        for (int j = 0; j < camera.getWidth(); ++j) {
            if (j == 0 && i % 10 == 0) {
                std::cout << "\rRendering row " << i << " of " << camera.getHeight() << std::flush;
            }
            const Ray ray = camera.getRay(i, j);
            float rayLength = FLOAT_MAX;
            float3 color = BC_COLOR_2;

            HitInfo hit;
            Object* closestObject = nullptr;
            float3 position;
            float3 normal;
            for (const auto& object : m_objects) {
                if (object->isInvisible()) continue;

                object->intersect(ray, hit);
                if (hit.distance < rayLength) {
                    rayLength = hit.distance;
                    position = hit.position;
                    normal = hit.normal;
                    closestObject = object.get();
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
            image.at<cv::Vec3b>(i, j) = cv::Vec3b(static_cast<uchar>(color.x * 255), static_cast<uchar>(color.y * 255), static_cast<uchar>(color.z * 255));
        }
    }
    std::cout << std::endl;
}
