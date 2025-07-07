#include "main.hpp"


int main() {

    Application *app = Application::getInstance();

    // Create an empty image with 3 channels (BGR) and 8-bit depth
    Camera *camera = app->getCamera();
    cv::Mat image(camera->get_height(), camera->get_width(), CV_8UC3, cv::Scalar(0, 0, 0));

    auto start = std::chrono::high_resolution_clock::now();
    
    app->render(image);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

    cv::Mat processed = image.clone();
    for(int y = 1; y < image.rows - 1; y++) {
        for(int x = 1; x < image.cols - 1; x++) {
            for(int c = 0; c < 3; c++) {  // For each color channel
                // Calculate mean of center pixel and four adjacent pixels
                uchar mean = (
                    image.at<cv::Vec3b>(y, x)[c] +      // center
                    image.at<cv::Vec3b>(y - 1, x)[c] +  // top
                    image.at<cv::Vec3b>(y + 1, x)[c] +  // bottom
                    image.at<cv::Vec3b>(y, x - 1)[c] +  // left
                    image.at<cv::Vec3b>(y, x + 1)[c]    // right
                ) / 5;
                
                processed.at<cv::Vec3b>(y, x)[c] = mean;
            }
        }
    }

    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << "images/output_" << std::put_time(std::localtime(&time), "%y%m%d_%H%M") << ".png";

    // Save the image with timestamp
    cv::imwrite(ss.str(), processed);

    delete app;
    return 0;
}