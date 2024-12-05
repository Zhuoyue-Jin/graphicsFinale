#include "cylinder.h"

void Cylinder::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}


void Cylinder::makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 normal) {

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal);


    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal);


    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal);

    // Second triangle
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal);

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal);
}

void Cylinder::makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 normal1, glm::vec3 normal2) {


    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, normal1);

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal1);


    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal2);

    // Second triangle
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, normal2);

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal1);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal2);
}


void Cylinder::construct(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 normal1, glm::vec3 normal2) {
    glm::vec3 leftStep = (bottomLeft - topLeft) / float(m_param1);
    glm::vec3 rightStep = (bottomRight - topRight) / float(m_param1);

    for (int i = 0; i < m_param1; i++) {
        glm::vec3 tl = leftStep * float(i) + topLeft;
        glm::vec3 tr = rightStep * float(i) + topRight;
        glm::vec3 bl = leftStep * float(i + 1) + topLeft;
        glm::vec3 br = rightStep * float(i + 1) + topRight;

        makeTile(tl, tr, bl, br, normal1, normal2);
    }
}

void Cylinder::construct(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 normal) {
    glm::vec3 leftStep = (bottomLeft - topLeft) / float(m_param1);
    glm::vec3 rightStep = (bottomRight - topRight) / float(m_param1);

    for (int i = 0; i < m_param1; i++) {
        glm::vec3 tl = leftStep * float(i) + topLeft;
        glm::vec3 tr = rightStep * float(i) + topRight;
        glm::vec3 bl = leftStep * float(i + 1) + topLeft;
        glm::vec3 br = rightStep * float(i + 1) + topRight;

        makeTile(tl, tr, bl, br, normal);
    }
}

void Cylinder::setVertexData() {
    int divs = std::max(3, m_param2);
    float angleStep = glm::radians(360.f / divs);
    float radius = 0.5f;
    float angleOffset = glm::radians(90.0f);

    glm::vec3 top(0.f, 0.5f, 0.f);
    glm::vec3 bottom(0.f, -0.5f, 0.f);

    for (int i = 0; i < divs; ++i) {
        float theta = i * angleStep + angleOffset;
        float nextTheta = (i + 1) * angleStep + angleOffset;

        // Calculate positions for the vertices on the curved surface
        glm::vec3 topLeft(radius * sin(theta), 0.5f, radius * cos(theta));
        glm::vec3 topRight(radius * sin(nextTheta), 0.5f, radius * cos(nextTheta));
        glm::vec3 bottomLeft(radius * sin(theta), -0.5f, radius * cos(theta));
        glm::vec3 bottomRight(radius * sin(nextTheta), -0.5f, radius * cos(nextTheta));

        // Calculate the normals for the left and right points on the curved surface
        glm::vec3 surfaceNormal_left = glm::normalize(glm::vec3(sin(theta), 0.0f, cos(theta)));
        glm::vec3 surfaceNormal_right = glm::normalize(glm::vec3(sin(nextTheta), 0.0f, cos(nextTheta)));

        // Construct the curved surface tile with separate normals for left and right
        construct(topLeft, topRight, bottomLeft, bottomRight, surfaceNormal_left, surfaceNormal_right);

        // Top face tile with normal pointing up
        construct(top, top, topLeft, topRight, glm::vec3(0.f, 1.f, 0.f));

        // Bottom face tile with normal pointing down
        construct(bottom, bottom, bottomRight, bottomLeft, glm::vec3(0.f, -1.f, 0.f));
    }
}

// Inserts a glm::vec3 into a vector of floats
void Cylinder::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
