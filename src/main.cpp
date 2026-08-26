#include "main.hpp"


#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>


int main() {
    Application& app = Application::getInstance();

    // Create an empty image with 3 channels (BGR) and 8-bit depth
    const Camera& camera = app.getCamera();
    cv::Mat image(camera.getHeight(), camera.getWidth(), CV_8UC3, cv::Scalar(0, 0, 0));

    const auto start = std::chrono::high_resolution_clock::now();

    app.render(image);

    const auto end = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

    cv::Mat processed = image.clone();
    for (int y = 1; y < image.rows - 1; y++) {
        for (int x = 1; x < image.cols - 1; x++) {
            for (int c = 0; c < 3; c++) { // For each color channel
                // Calculate mean of center pixel and four adjacent pixels
                const uchar mean = static_cast<uchar>((
                    image.at<cv::Vec3b>(y, x)[c] +      // center
                    image.at<cv::Vec3b>(y - 1, x)[c] +  // top
                    image.at<cv::Vec3b>(y + 1, x)[c] +  // bottom
                    image.at<cv::Vec3b>(y, x - 1)[c] +  // left
                    image.at<cv::Vec3b>(y, x + 1)[c]    // right
                ) / 5);

                processed.at<cv::Vec3b>(y, x)[c] = mean;
            }
        }
    }

    // Get current time
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << "images/output_" << std::put_time(std::localtime(&time), "%y%m%d_%H%M") << ".png";

    // Save the image with timestamp
    cv::imwrite(ss.str(), processed);

    return 0;
}
