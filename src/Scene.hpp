#pragma once


#include <memory>
#include <vector>
#include <opencv2/opencv.hpp>

#include "utils.hpp"
#include "Camera.hpp"
#include "shapes/Sphere.hpp"
#include "shapes/Mesh.hpp"
#include "shapes/Cube.hpp"
#include "shapes/Quad.hpp"


class Object;


class Scene {

    private:

        std::vector<std::unique_ptr<Object>> m_objects;

    public:

        explicit Scene(int sceneIndex);
        ~Scene();
        [[nodiscard]] const std::vector<std::unique_ptr<Object>>& getObjects() const noexcept;
        void render(const Camera& camera, cv::Mat& image) const;

};
