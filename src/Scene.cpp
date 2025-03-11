#include "Scene.hpp"


Scene::Scene() {
    background_color = {200, 200, 200};
}


void Scene::render(Camera *camera, cv::Mat &image) {
    float3 v0 = {0, 1, 0};
    float3 v1 = {1, 1, 0};
    float3 v2 = {0, 1, 1};
    v0 = mul(camera->viewMatrix(), v0);
    v1 = mul(camera->viewMatrix(), v1);
    v2 = mul(camera->viewMatrix(), v2);
    for (int i = 0; i < camera->get_height(); ++i) {
        for (int j = 0; j < camera->get_width(); ++j) {
            float3 ray = camera->get_ray(i, j);
            if (intersect_triangle(ray, v0, v1, v2)) {
                image.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
            } else {
                image.at<cv::Vec3b>(i, j) = cv::Vec3b(50, 50, 50);
            }
            
        }
    }    
}