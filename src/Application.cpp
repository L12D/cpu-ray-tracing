#include "Application.hpp"


Application *Application::instance = nullptr;


Application::Application() {
    camera = new Camera(720);
    scene = new Scene(2);
    directions = generateDirections(100);
}


Application *Application::getInstance() {
    if (instance == nullptr) {
        instance = new Application();
    }
    return instance;
}


Camera *Application::getCamera() {
    return camera;
}


Scene *Application::getScene() {
    return scene;
}


std::vector<float3> Application::generateDirections(int n) {
    std::vector<float3> result;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int i = 0; i < n; ++i) {
        // Generate random angles
        float theta = 2.0f * M_PI * dis(gen);  // Azimuthal angle [0, 2π]
        float phi = acos(2.0f * dis(gen) - 1.0f);  // Polar angle [0, π]
        
        // Convert spherical to Cartesian coordinates
        float3 randomDirection = {
            sin(phi) * cos(theta),
            sin(phi) * sin(theta),
            cos(phi)
        };

        result.push_back(randomDirection);
    }
    return result;
}


std::vector<float3> Application::getDirections() {
    return directions;
}


void Application::render(cv::Mat &image) {
    scene->render(camera, image);
}


Application::~Application() {
    delete camera;
    delete scene;
}