#include <stdexcept>
#include "camera.h"
#include <iostream>

// changes
#include <glm/glm.hpp>
#include "utils/scenedata.h"
#include "utils/scenefilereader.h"
#include <glm/gtc/matrix_transform.hpp>
// changes end

Camera::Camera(int imageWidth, int imageHeight, const RenderData &metaData)
    : imageWidth(imageWidth), imageHeight(imageHeight), metaData(metaData)
{

}


// glm::mat4 Camera::getViewMatrix() const {
//     // Optional TODO: implement the getter or make your own design
//     // throw std::runtime_error("not implemented");

//     SceneCameraData camera = metaData.cameraData;
//     glm::vec4 pos = camera.pos;
//     glm::vec4 look = camera.look;
//     glm::vec4 up = camera.up;


//     // glm::mat4 M_translate(glm::vec4(1.0f,0.f,0.f,0.f),
//     //                       glm::vec4(0.0f,1.f,0.f,0.f),
//     //                       glm::vec4(0.0f,0.f,1.f,0.f),
//     //                       glm::vec4(-pos[0],-pos[1],-pos[2],1.f));

//     // glm::mat4 M_translate(glm::vec4(1,0,0,0),
//     //                       glm::vec4(0,1,0,0),
//     //                       glm::vec4(0,0,1,0),
//     //                       glm::vec4(-pos[0],-pos[1],-pos[2],1.f));
//     glm::mat4 M_translate(1,0,0,0,0,1,0,0,0,0,1,
//                           0,-pos[0],-pos[1],-pos[2],1.f);

//     glm::vec4 w(-look/glm::length(look));

//     glm::vec4 v((up - (glm::dot(up,w)*w))
//                 /glm::length(up - (glm::dot(up,w)*w)));

//     glm::vec4 u = glm::vec4(v) * glm::vec4(w);


//     // glm::mat4 M_rotate(glm::vec4(u[0],v[0],w[0],0.f),
//     //                    glm::vec4(u[1],v[1],w[1],0.f),
//     //                    glm::vec4(u[2],v[2],w[2],0.f),
//     //                    glm::vec4(0,0,0,1));

//     glm::mat4 M_rotate(u[0],v[0],w[0],0.f,u[1],v[1],w[1],
//                        0.f,u[2],v[2],w[2],0.f,0,0,0,1);


//     return M_rotate * M_translate;

//     // return glm::lookAt(pos, look, up);

// }

// glm::mat4 Camera::getViewMatrixInverse() const {
//     auto vm = getViewMatrix();
//     return glm::mat4(glm::inverse(vm));
// }

glm::mat4 Camera::getViewMatrix() const {
    SceneCameraData camera = metaData.cameraData;
    glm::vec4 pos = camera.pos;
    glm::vec4 look = camera.look;
    glm::vec4 up = camera.up;

    // Translation matrix
    glm::mat4 M_translate(1.0f, 0.0f, 0.0f, 0.0f,
                          0.0f, 1.0f, 0.0f, 0.0f,
                          0.0f, 0.0f, 1.0f, 0.0f,
                          -pos[0], -pos[1], -pos[2], 1.0f);

    // Calculate w (forward) vector
    float lookLength = glm::length(look);
    if (lookLength < 1e-6f) return glm::mat4(1.0f); // Guard against zero length
    glm::vec4 w = -look / lookLength;

    // Calculate v (up) vector
    glm::vec4 upProjW = glm::dot(up, w) * w;
    glm::vec4 upPerp = up - upProjW;
    float upPerpLength = glm::length(upPerp);
    if (upPerpLength < 1e-6f) return glm::mat4(1.0f); // Guard against zero length
    glm::vec4 v = upPerp / upPerpLength;

    // Calculate u (right) vector using cross product
    glm::vec3 v3(v), w3(w);
    glm::vec3 u3 = glm::cross(v3, w3);
    glm::vec4 u = glm::vec4(u3, 0.0f);

    // Rotation matrix
    glm::mat4 M_rotate(u[0], v[0], w[0], 0.0f,
                       u[1], v[1], w[1], 0.0f,
                       u[2], v[2], w[2], 0.0f,
                       0.0f, 0.0f, 0.0f, 1.0f);

    return M_rotate * M_translate;
}

glm::mat4 Camera::getViewMatrixInverse() const {
    return glm::inverse(getViewMatrix());
}

float Camera::getAspectRatio() const {
    // Optional TODO: implement the getter or make your own design
    // throw std::runtime_error("not implemented");

    float k{getFocalLength()};

    float horizontal_angle{getHeightAngle()};
    float V = 2 * k * glm::tan(horizontal_angle/2);
    float U = (imageWidth * V)/imageHeight;

    return U/V;

}

float Camera::getHeightAngle() const {
    // Optional TODO: implement the getter or make your own design
    // throw std::runtime_error("not implemented");
    SceneCameraData camera = metaData.cameraData;
    return camera.heightAngle;

}
float Camera::getFocalLength() const {
    // Optional TODO: implement the getter or make your own design
    // throw std::runtime_error("not implemented");
    SceneCameraData camera = metaData.cameraData;
    return camera.focalLength;

}

float Camera::getAperture() const {
    // Optional TODO: implement the getter or make your own design
    // throw std::runtime_error("not implemented");
    SceneCameraData camera = metaData.cameraData;
    return camera.aperture;

}
