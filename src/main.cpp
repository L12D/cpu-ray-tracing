#include "main.hpp"


int main() {

    Camera *camera = new Camera();

    Scene *scene = Scene::getInstance();

    // Create an empty image with 3 channels (BGR) and 8-bit depth
    cv::Mat image(camera->get_height(), camera->get_width(), CV_8UC3, cv::Scalar(0, 0, 0));
    scene->render(camera, image);

    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << "images/output_" << std::put_time(std::localtime(&time), "%y%m%d_%H%M") << ".png";

    // Save the image with timestamp
    cv::imwrite(ss.str(), image);

    delete scene;
    delete camera;

    return 0;
}