#ifndef CAMERA_H
#define CAMERA_H


void calMatrices( int width,
                  int height,
                  glm::mat4& m_view,
                  glm::mat4& m_proj,
                  glm::vec4& pos,
                  glm::vec4& look,
                  glm::vec4& up)
{

    m_view = glm::lookAt(glm::vec3(pos),
                         glm::vec3(pos) + glm::vec3(look),
                         glm::vec3(up));


    float aspectRatio = static_cast<float>(width) / height;
    float theta_h = 45.f;
    float near = 0.1f;
    float far = 100.f;

    float tanHalfFovy = tan(theta_h / 2.0f);
    float top = near * tanHalfFovy;
    float rightPlane = top * aspectRatio;

    m_proj = glm::mat4(0.0f);
    m_proj[0][0] = near / rightPlane;
    m_proj[1][1] = near / top;
    m_proj[2][2] = -(far + near) / (far - near);
    m_proj[2][3] = -1.0f;
    m_proj[3][2] = -(2.0f * far * near) / (far - near);
}

void calcMatrices(int width,
                  int height,
                  glm::mat4& m_view,
                  glm::mat4& m_proj,
                  std::unordered_map<Qt::Key, bool> m_keyMap,
                  float deltaTime,
                  glm::vec4& pos ,
                  glm::vec4& look ,
                  glm::vec4& up)
{

    glm::vec3 movement(0.0f);

    glm::vec3 lookVec = glm::normalize(glm::vec3(look));
    glm::vec3 upVec = glm::normalize(glm::vec3(up));
    glm::vec3 rightVec = glm::normalize(glm::cross(lookVec, upVec));


    const float movementSpeed = 5.0f; // Units per second


    if (m_keyMap.at(Qt::Key_W))
        movement += lookVec * movementSpeed * deltaTime;

    if (m_keyMap.at(Qt::Key_S))
        movement -= lookVec * movementSpeed * deltaTime;

    if (m_keyMap.at(Qt::Key_A))
        movement -= rightVec * movementSpeed * deltaTime;

    if (m_keyMap.at(Qt::Key_D))
        movement += rightVec * movementSpeed * deltaTime;

    if (m_keyMap.at(Qt::Key_Space))
        movement += glm::vec3(0.0f, 1.0f, 0.0f) * movementSpeed * deltaTime;

    if (m_keyMap.at(Qt::Key_Control))
        movement += glm::vec3(0.0f, -1.0f, 0.0f) * movementSpeed * deltaTime;


    pos += glm::vec4(movement, 0.0f);


    m_view = glm::lookAt(glm::vec3(pos),
                         glm::vec3(pos) + glm::vec3(look),
                         glm::vec3(up));


    float aspectRatio = static_cast<float>(width) / height;
    float theta_h = 30.f;
    float near = 0.1f;
    float far = 100.f;

    float tanHalfFovy = tan(theta_h / 2.0f);
    float top = near * tanHalfFovy;
    float rightPlane = top * aspectRatio;

    m_proj = glm::mat4(0.0f);
    m_proj[0][0] = near / rightPlane;
    m_proj[1][1] = near / top;
    m_proj[2][2] = -(far + near) / (far - near);
    m_proj[2][3] = -1.0f;
    m_proj[3][2] = -(2.0f * far * near) / (far - near);
}

glm::mat4 rotationM(glm::vec3 axisX, float angleX)
{
    float cosX = cos(angleX);
    float sinX = sin(angleX);
    glm::mat4 rotationX = glm::mat4(1.0f);

    rotationX[0][0] = cosX + axisX.x * axisX.x * (1 - cosX);
    rotationX[0][1] = axisX.x * axisX.y * (1 - cosX) - axisX.z * sinX;
    rotationX[0][2] = axisX.x * axisX.z * (1 - cosX) + axisX.y * sinX;
    rotationX[1][0] = axisX.y * axisX.x * (1 - cosX) + axisX.z * sinX;
    rotationX[1][1] = cosX + axisX.y * axisX.y * (1 - cosX);
    rotationX[1][2] = axisX.y * axisX.z * (1 - cosX) - axisX.x * sinX;
    rotationX[2][0] = axisX.z * axisX.x * (1 - cosX) - axisX.y * sinX;
    rotationX[2][1] = axisX.z * axisX.y * (1 - cosX) + axisX.x * sinX;
    rotationX[2][2] = cosX + axisX.z * axisX.z * (1 - cosX);

    return rotationX;

}
void calcMatrices(int width,
                  int height,
                  glm::mat4& m_view,
                  glm::mat4& m_proj,
                  float deltaX,
                  float deltaY,
                  glm::vec4& pos,
                  glm::vec4& look,
                  glm::vec4& up)
{
    float angleX = glm::radians(1.0f) * deltaX;
    float angleY = glm::radians(1.0f) * deltaY;


    glm::vec3 axisY = glm::vec3(0.f, 1.f, 0.f);
    glm::mat4 rotationY = rotationM(axisY, angleX);


    look = rotationY * look;
    up = rotationY * up;


    glm::vec3 lookVec = glm::normalize(glm::vec3(look));
    glm::vec3 upVec = glm::normalize(glm::vec3(up));
    glm::vec3 rightVec = glm::normalize(glm::cross(lookVec, upVec));

    glm::mat4 rotationX = rotationM(rightVec, angleY);


    look = rotationX * look;
    up = rotationX * up;


    look = glm::normalize(look);
    up = glm::normalize(up);


    m_view = glm::lookAt(glm::vec3(pos),
                         glm::vec3(pos) + glm::vec3(look),
                         glm::vec3(up));


    float aspectRatio = static_cast<float>(width) / height;
    float theta_h = 30.f;
    float near = 0.1f;
    float far = 100.f;

    float tanHalfFovy = tan(theta_h / 2.0f);
    float top = near * tanHalfFovy;
    float rightPlane = top * aspectRatio;

    m_proj = glm::mat4(0.0f);
    m_proj[0][0] = near / rightPlane;
    m_proj[1][1] = near / top;
    m_proj[2][2] = -(far + near) / (far - near);
    m_proj[2][3] = -1.0f;
    m_proj[3][2] = -(2.0f * far * near) / (far - near);
}

#endif // CAMERA_H
