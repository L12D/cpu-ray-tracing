#include "Application.hpp"

#include "Scene.hpp"

#include <utility>


Application::Application()
    : m_camera(std::make_unique<Camera>(RESOLUTION)),
      m_scene(std::make_unique<Scene>(SCENE)) {
    if constexpr (PRODUCTION) {
        m_directions = generateDirections();
    }
}


Application& Application::getInstance() {
    static Application instance;
    return instance;
}


Application::~Application() = default;


Camera& Application::getCamera() const noexcept {
    return *m_camera;
}


Scene& Application::getScene() const noexcept {
    return *m_scene;
}


std::vector<float3> Application::generateDirections() const {
    std::vector<float3> result;
    result.reserve(N_RAYS);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (int i = 0; i < N_RAYS; ++i) {
        // Generate random angles
        const float theta = 2.0f * static_cast<float>(PI) * dis(gen); // Azimuthal angle [0, 2π]
        const float phi = acos(2.0f * dis(gen) - 1.0f);               // Polar angle [0, π]

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


std::vector<float3> Application::getDirections() const {
    return m_directions;
}


void Application::render(cv::Mat& image) const {
    m_scene->render(*m_camera, image);
}
