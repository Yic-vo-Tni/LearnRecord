//
// Created by lenovo on 9/28/2023.
//

#ifndef LEARN_RECORD_CAMERA_H
#define LEARN_RECORD_CAMERA_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace yic {
    inline struct cameraPosition{
        glm::vec3 Position{};
        glm::vec3 Front{};
        glm::vec3 Up{};
        glm::vec3 Right{};
        glm::vec3 WorldUp{};
    } cameraPosition;

    enum CameraMovement {
        Forward,
        Backward,
        Left,
        Right_
    };

    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;

    class Camera {
    public:
//        glm::vec3 Position;
        glm::vec3 Front;
//        glm::vec3 Up;
//        glm::vec3 Right;
//        glm::vec3 WorldUp;

        // euler Angles
        float Yaw;
        float Pitch;
        // camera options
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = YAW, float pitch = PITCH);
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        glm::mat4 GetViewMatrix() { return glm::lookAt(cameraPosition.Position, cameraPosition.Position + cameraPosition.Front, cameraPosition.Up); }

        void ProcessKeyboard(CameraMovement direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
        void ProcessMouseScroll(float yoffset);


    private:
        void updateCameraVectors();
    };

}

#endif //LEARN_RECORD_CAMERA_H
