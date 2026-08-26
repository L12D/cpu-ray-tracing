#include "Camera.hpp"


Camera::Camera(int resolution)
    : m_width(resolution * 16 / 9),
      m_height(resolution),
      m_view(identity()),
      m_rays(m_height, std::vector<Ray>(m_width)) {
    const float gap = 2 * std::tan(FOV / 2) / m_height;
    for (int i = 0; i < m_height; ++i) {
        for (int j = 0; j < m_width; ++j) {
            const float x = (j - m_width / 2) * gap;
            const float y = 1.0;
            const float z = -(i - m_height / 2) * gap;
            const float3 dir = normalize({x, y, z});
            m_rays[i][j] = Ray({0, 0, 0}, dir);
        }
    }
}


int Camera::getWidth() const noexcept {
    return m_width;
}


int Camera::getHeight() const noexcept {
    return m_height;
}


const Ray& Camera::getRay(int i, int j) const noexcept {
    return m_rays[i][j];
}


mat4 Camera::viewMatrix() const noexcept {
    return m_view;
}


void Camera::translate(float3 t) {
    m_view = mTranslate(-t) * m_view;
}
