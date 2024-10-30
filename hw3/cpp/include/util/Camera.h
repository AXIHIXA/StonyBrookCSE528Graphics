/// STOP. You should not modify this file unless you KNOW what you are doing.

#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext.hpp>


class Camera
{
public:
    // Defines several possible options for camera movement.
    // Used as abstraction to stay away from pWindow-system specific input methods.
    enum Movement: int
    {
        kUp,
        kDown,
        kLeft,
        kRight,
        kForward,
        kBackWard
    };

    // Default camera values
    static constexpr float kDefaultYaw = -90.0f;
    static constexpr float kDefaultPitch = 0.0f;
    static constexpr float kDefaultSpeed = 2.5f;
    static constexpr float kDefaultSensitivity = 0.1f;
    static constexpr float kDefaultZoom = 45.0f;

public:
    Camera() = delete;

    explicit Camera(glm::vec3 position = {0.0f, 0.0f, 0.0f},
                    glm::vec3 up = {0.0f, 1.0f, 0.0f},
                    float yaw = kDefaultYaw,
                    float pitch = kDefaultPitch)
            : position(position),
              front({}),
              up(up),
              right({}),
              worldUp(up),
              yaw(yaw),
              pitch(pitch),
              movementSpeed(kDefaultSpeed),
              mouseSensitivity(kDefaultSensitivity),
              zoom(kDefaultZoom)
    {
        updateCameraVectors();

#ifdef DEBUG_CAMERA
        std::cout << "position = " << this->position << '\n'
                  << "   front = " << this->front << '\n'
                  << "   right = " << this->right << '\n'
                  << "      up = " << this->up << '\n'
                  << " worldUp = " << this->worldUp << '\n';
#endif  // DEBUG_CAMERA
    }

    Camera(const Camera &) = default;
    Camera(Camera &&) = default;
    Camera & operator=(const Camera &) = default;
    Camera & operator=(Camera &&) = default;

    ~Camera() = default;

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix.
    [[nodiscard]] glm::mat4 getViewMatrix() const
    {
        return glm::lookAt(position, position + front, up);
    }

    // Processes input received from any keyboard-like input system.
    // Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems).
    void processKeyboard(Movement direction, double deltaTime)
    {
        float displacement = movementSpeed * static_cast<float>(deltaTime);

        switch (direction)
        {
        case kUp:
        {
            position += up * displacement;
            break;
        }
        case kDown:
        {
            position -= up * displacement;
            break;
        }
        case kLeft:
        {
            position -= right * displacement;
            break;
        }
        case kRight:
        {
            position += right * displacement;
            break;
        }
        case kForward:
        {
            position += front * displacement;
            break;
        }
        case kBackWard:
        {
            position -= front * displacement;
            break;
        }
        default:
        {
            throw std::invalid_argument("unknown CameraMovement enumerator " +
                                        std::to_string(static_cast<int>(direction)));
        }
        }
    }

    // Processes input received from a mouse input system.
    // Expects the offset value in both the x and y direction.
    void processMouseMovement(double xoffset, double yoffset, bool constrainPitch = true)
    {
        yaw += static_cast<float>(xoffset) * mouseSensitivity;
        pitch += static_cast<float>(yoffset) * mouseSensitivity;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped.
        if (constrainPitch)
        {
            if (89.0f < pitch)
            {
                pitch = 89.0f;
            }

            if (pitch < -89.0f)
            {
                pitch = -89.0f;
            }
        }

        // Update Front, Right and Up Vectors using the updated Euler angles.
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event.
    // Only requires inputs on the vertical wheel-axis.
    void processMouseScroll(double yoffset)
    {
        zoom -= static_cast<float>(yoffset);

        if (zoom < 1.0f)
        {
            zoom = 1.0f;
        }

        if (zoom > 45.0f)
        {
            zoom = 45.0f;
        }
    }

public:
    // Camera attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    // Euler angles
    float yaw;
    float pitch;

    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

private:
    // Calculates the front vector from the Camera's (updated) Euler angles
    void updateCameraVectors()
    {
        front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        front.y = std::sin(glm::radians(pitch));
        front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
        front = glm::normalize(front);

        // Normalize the vectors,
        // because their length gets closer to 0 the more you look up or down which results in slower movement.
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};


#endif  // CAMERA_H
