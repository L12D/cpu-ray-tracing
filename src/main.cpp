#include "main.hpp"


int main() {

    Camera *camera = new Camera();

    Scene *scene = new Scene();

    // Create an empty image with 3 channels (BGR) and 8-bit depth
    cv::Mat image(camera->get_height(), camera->get_width(), CV_8UC3, cv::Scalar(0, 0, 0));
    scene->render(camera, image);

    // Save the image
    cv::imwrite("build/output.png", image);

    delete scene;
    delete camera;

    return 0;
}