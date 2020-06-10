/*Header only class for camera object*/

#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum class Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

// Default camera values
const float YAW           = -90.0f;
const float PITCH         = 0.0f;
const float SPEED         = 5.0f;
const float SENSITIVITY   = 0.05f;
const float ZOOM          = 45.0f;

// Abstract camera class for OpenGL
class Camera {
private:
  // Camera attributes
  glm::vec3 m_position;
  glm::vec3 m_front;
  glm::vec3 m_up;
  glm::vec3 m_right;
  glm::vec3 m_worldUp;
  // Euler angles
  float m_yaw;
  float m_pitch;
  // camera options
  float m_movementSpeed;
  float m_mouseSensitivity;
  float m_zoom;

public:

  // Getters
  glm::vec3 getFront() {return m_front;}
  glm::vec3 getPosition() {return m_position;}

  // Constructor with vector
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
    float yaw = YAW,
    float pitch = PITCH) :
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_movementSpeed(SPEED),
    m_mouseSensitivity(SENSITIVITY),
    m_zoom(ZOOM) {

      m_position = position;
      m_worldUp = up;
      m_yaw = yaw;
      m_pitch = pitch;
      updateCameraVectors();
  }

  // Constructor with scalar values
  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
    m_movementSpeed(SPEED),
    m_mouseSensitivity(SENSITIVITY),
    m_zoom(ZOOM) {

    m_position = glm::vec3(posX, posY, posZ);
    m_worldUp = glm::vec3(upX, upY, upZ);
    m_yaw = yaw;
    m_pitch = pitch;
    updateCameraVectors();
  }

  // Returns the view matrix calculated with euler angles and LookAt matrix
  glm::mat4 GetViewMatrix() {
    //return glm::lookAt(m_position, m_position + m_front, m_up);
    return look(m_position, m_position + m_front, m_up);
  }

  // processes input from keyboard-like system. Abstraction from windowing systems through enum
  void processKeyboard(Camera_Movement direction, float deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    if (direction == Camera_Movement::FORWARD) {
      m_position += glm::normalize(glm::vec3(m_front.x, m_front.y, m_front.z)) * velocity;
    }
    if (direction == Camera_Movement::BACKWARD) {
      m_position -= glm::normalize(glm::vec3(m_front.x, m_front.y, m_front.z)) * velocity;
    }
    if (direction == Camera_Movement::LEFT) {
      m_position -= glm::normalize(glm::vec3(m_right.x, m_right.y, m_right.z)) * velocity;
    }
    if (direction == Camera_Movement::RIGHT) {
      m_position += glm::normalize(glm::vec3(m_right.x, m_right.y, m_right.z)) * velocity;
    }
  }

  // Processes input received from a mouse input system. Expects offset in x and y direction
  void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrainPitch) {
      if (m_pitch > 89.0f) {
        m_pitch = 89.0f;
      }
      if (m_pitch < -89.0f) {
        m_pitch = -89.0f;
      }
    }

    // Update m_front, m_right and m_up vectors using the updated angles
    updateCameraVectors();
  }

  // Process input from mouse scroll event, only requires input on the vertical wheel
  void processMouseScroll(float yoffset) {
    m_zoom -= yoffset;
    if (m_zoom < 1.0f) {
      m_zoom = 1.0f;
    }
    if (m_zoom > 45.0f) {
      m_zoom = 45.0f;
    }
  }

  // Getters
  float getZoom() {
    return m_zoom;
  }

  glm::mat4 look(glm::vec3 position, glm::vec3 target, glm::vec3 up) {
    // Vectors
    glm::vec3 cameraDirection = glm::normalize(position - target);
    glm::vec3 cameraRight = glm::normalize(glm::cross(glm::normalize(up), cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight); // Wrong??

    glm::mat4 rotation;
    // First column
    rotation[0].x = cameraRight.x;
    rotation[0].y = cameraUp.x;
    rotation[0].z = cameraDirection.x;
    rotation[0].w = 0;

    // Second column
    rotation[1].x = cameraRight.y;
    rotation[1].y = cameraUp.y;
    rotation[1].z = cameraDirection.y;
    rotation[1].w = 0;

    // Third column
    rotation[2].x = cameraRight.z;
    rotation[2].y = cameraUp.z;
    rotation[2].z = cameraDirection.z;
    rotation[2].w = 0;

    // Fourth column
    rotation[3].x = 0;
    rotation[3].y = 0;
    rotation[3].z = 0;
    rotation[3].w = 1;

    glm::mat4 translation = glm::mat4(1.0f);
    translation[3].x = -position.x;
    translation[3].y = -position.y;
    translation[3].z = -position.z;

    return rotation * translation;
  }

private:
  // Calculate front, right and up vector for camera
  void updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);

    // Need to be normalized since length of vectors change at different angles
    // affecting movement speed
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
  }
};

#endif // CAMERA_H
