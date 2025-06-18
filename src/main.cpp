#include "main.hpp"


int main() {

    Application *app = Application::getInstance();

    // Create an empty image with 3 channels (BGR) and 8-bit depth
    Camera *camera = app->getCamera();
    cv::Mat image(camera->get_height(), camera->get_width(), CV_8UC3, cv::Scalar(0, 0, 0));
    app->render(image);

    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << "images/output_" << std::put_time(std::localtime(&time), "%y%m%d_%H%M") << ".png";

    // Save the image with timestamp
    cv::imwrite(ss.str(), image);

    delete app;
    return 0;
}