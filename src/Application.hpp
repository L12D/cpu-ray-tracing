#pragma once


#include <memory>
#include <vector>
#include <opencv2/opencv.hpp>

#include "Camera.hpp"


class Scene;


class Application {

    private:

        std::unique_ptr<Camera> m_camera;
        std::unique_ptr<Scene> m_scene;
        std::vector<float3> m_directions;

        Application();

    public:

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        ~Application();

        [[nodiscard]] static Application& getInstance();
        [[nodiscard]] Camera& getCamera() const noexcept;
        [[nodiscard]] Scene& getScene() const noexcept;
        [[nodiscard]] std::vector<float3> generateDirections() const;
        [[nodiscard]] std::vector<float3> getDirections() const;
        void render(cv::Mat& image) const;

};
