#include "Engine/Renderer/OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Engine {

    OrthographicCamera::OrthographicCamera()
        : m_ProjectionMatrix(glm::ortho(-1.6f, 1.6f, -0.9f, 0.9f, -1.0f, 1.0f)), m_ViewMatrix(1.0f) {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f) {
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top) {
        m_OriginalLeft = left;
        m_OriginalRight = right;
        m_OriginalBottom = bottom;
        m_OriginalTop = top;
        
        // Apply current zoom level
        float zoomedLeft = left / m_ZoomLevel;
        float zoomedRight = right / m_ZoomLevel;
        float zoomedBottom = bottom / m_ZoomLevel;
        float zoomedTop = top / m_ZoomLevel;
        
        m_ProjectionMatrix = glm::ortho(zoomedLeft, zoomedRight, zoomedBottom, zoomedTop, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
    
    void OrthographicCamera::SetZoomLevel(float zoom) {
        // Clamp zoom to reasonable values (0.1x to 10x)
        m_ZoomLevel = glm::clamp(zoom, 0.1f, 10.0f);
        
        // Recalculate projection with new zoom
        float zoomedLeft = m_OriginalLeft / m_ZoomLevel;
        float zoomedRight = m_OriginalRight / m_ZoomLevel;
        float zoomedBottom = m_OriginalBottom / m_ZoomLevel;
        float zoomedTop = m_OriginalTop / m_ZoomLevel;
        
        m_ProjectionMatrix = glm::ortho(zoomedLeft, zoomedRight, zoomedBottom, zoomedTop, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix() {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
        
        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

}

