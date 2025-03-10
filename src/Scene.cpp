#include "Scene.hpp"


Scene::Scene() {
    background_color = {200, 200, 200};
}


void Scene::render(Camera *camera, cv::Mat &image) {
    float3 v0 = {0, -1, 0};
    float3 v1 = {1, -1, 0};
    float3 v2 = {0, -1, 1};
    for (int i = 0; i < camera->get_width(); ++i) {
        for (int j = 0; j < camera->get_height(); ++j) {
            float3 ray = camera->get_ray(i, j);
            float3 color;
            if (intersect_triangle(ray, v0, v1, v2)) {
                image.at<cv::Vec3b>(j, i) = cv::Vec3b(235, 235, 235);
            } else {
                image.at<cv::Vec3b>(j, i) = cv::Vec3b(0, 0, 0);
            }
            
        }
    }    
}