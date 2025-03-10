#include "main.hpp"


int main() {

    // Create an empty image with 3 channels (BGR) and 8-bit depth
    cv::Mat image(1080, 1920, CV_8UC3, cv::Scalar(0, 0, 0));
    
    // Set pixels to specific colors
    for (int y = 0; y < image.rows; ++y) {
        for (int x = 0; x < image.cols; ++x) {
            // Set pixel at (x, y) to blue
            image.at<cv::Vec3b>(y, x) = cv::Vec3b(x * 255 / image.cols, y * 255 / image.rows, 0); // BGR gradient
        }
    }

    // Save the image
    cv::imwrite("build/output.png", image);

    return 0;
}