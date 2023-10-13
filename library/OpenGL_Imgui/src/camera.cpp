//
// Created by lenovo on 9/28/2023.
//

#include "camera.h"

namespace yic{
//    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch): Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
//                                                                              MouseSensitivity(SENSITIVITY), Zoom(ZOOM){
//        cameraPosition.Position = position;
//        cameraPosition.WorldUp = up;
//        Yaw = yaw;
//        Pitch = pitch;
//        updateCameraVectors();
//    }

    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch): MovementSpeed(SPEED),
                                                                              MouseSensitivity(SENSITIVITY), Zoom(ZOOM){
        cameraPosition.Position = position;
        cameraPosition.WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(
            glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {
        cameraPosition.Position = glm::vec3(posX, posY, posZ);
        cameraPosition.WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    void Camera::ProcessKeyboard(yic::CameraMovement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == Forward)
            cameraPosition.Position += cameraPosition.Front * velocity;
        if (direction == Backward)
            cameraPosition.Position -= cameraPosition.Front * velocity;
        if (direction == Left)
            cameraPosition.Position -= cameraPosition.Right * velocity;
        if (direction == Right_)
            cameraPosition.Position += cameraPosition.Right * velocity;
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void Camera::ProcessMouseScroll(float yoffset) {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    void Camera::updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        cameraPosition.Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        cameraPosition.Right = glm::normalize(glm::cross(cameraPosition.Front, cameraPosition.WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        cameraPosition.Up    = glm::normalize(glm::cross(cameraPosition.Right, cameraPosition.Front));
    }

}