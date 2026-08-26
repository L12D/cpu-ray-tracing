#pragma once


#include <vector>

#include "utils.hpp"


class Camera {

    private:

        int m_width;  // in pixels
        int m_height; // in pixels
        mat4 m_view;
        std::vector<std::vector<Ray>> m_rays;

    public:

        explicit Camera(int resolution);
        [[nodiscard]] int getWidth() const noexcept;
        [[nodiscard]] int getHeight() const noexcept;
        [[nodiscard]] const Ray& getRay(int i, int j) const noexcept;
        [[nodiscard]] mat4 viewMatrix() const noexcept;
        void translate(float3 t);

};
