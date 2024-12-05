#include "cone.h"

void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

// Overloaded makeTile for flat faces 
void Cone::makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 normal) {
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

// Overloaded makeTile for curved surface 
void Cone::makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 normal1, glm::vec3 normal2) {
    insertVec3(m_vertexData, topLeft);
    if(topLeft == apex)
        insertVec3(m_vertexData, apexn);
    else
        insertVec3(m_vertexData, normal1);

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal1);

    insertVec3(m_vertexData, topRight);
    if(topRight == apex)
        insertVec3(m_vertexData, apexn);
    else
        insertVec3(m_vertexData, normal2);


    // Second triangle
    insertVec3(m_vertexData, topRight);
    if(topRight == apex)
        insertVec3(m_vertexData, apexn);
    else
        insertVec3(m_vertexData, normal2);

    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, normal1);

    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, normal2);
}

// Modified construct for the curved surface, using two normals
void Cone::construct(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 normal1, glm::vec3 normal2) {
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

// Overloaded construct for the flat base, using a single normal
void Cone::construct(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 normal) {
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

void Cone::setVertexData() {

    float angleOffset = glm::radians(90.0f);

    int divs = std::max(3, m_param2);
    float angleStep = glm::radians(360.f / divs);
    float radius = 0.5f;
    float h = 1.0f; // Height from base (-0.5) to apex (0.5)

    apex = glm::vec3(0.f, 0.5f, 0.f); // Apex at y = 0.5
    glm::vec3 bottomCenter(0.f, -0.5f, 0.f); // Base center at y = -0.5

    // Precompute scaling factor for normals

    for (int i = 0; i < divs; ++i) {
        float theta = i * angleStep + angleOffset;
        float nextTheta = (i + 1) * angleStep + angleOffset;

        // Base vertices positions
        // glm::vec3 bottomLeft(radius * sin(theta), -0.5f, radius * cos(theta));
        // glm::vec3 bottomRight(radius * sin(nextTheta), -0.5f, radius * cos(nextTheta));

        // glm::vec3 bottomLeft(radius * sin(theta), -0.5f, radius *  cos(theta));
        // glm::vec3 bottomRight(radius * sin(nextTheta), -0.5f, radius * cos(nextTheta));

        // // Normals at the base vertices
        // glm::vec3 normalLeft = glm::normalize(glm::vec3(sin(theta), 0.5f,  cos(theta)));
        // glm::vec3 normalRight = glm::normalize(glm::vec3(sin(nextTheta), 0.5f, cos(nextTheta)));

        // Positions at the base
        glm::vec3 bottomLeft(radius * sin(theta), -0.5f, radius * cos(theta));
        glm::vec3 bottomRight(radius * sin(nextTheta), -0.5f, radius * cos(nextTheta));

        // Normals at the base vertices, aligned with the cone surface
        float slope = radius / h;  // Height-to-radius ratio
        glm::vec3 normalLeft = glm::normalize(glm::vec3(sin(theta), slope, cos(theta)));
        glm::vec3 normalRight = glm::normalize(glm::vec3(sin(nextTheta), slope, cos(nextTheta)));

        apexn = glm::normalize((normalLeft + normalRight)/2.f);
        apexn.y = 0;
        apexn = glm::normalize(apexn);
        apexn.y += 0.5f;
        apexn = glm::normalize(apexn);
        // Construct the curved surface of the cone
        construct(apex, apex, bottomLeft, bottomRight, normalLeft, normalRight);

        // Base of the cone with normal pointing down
        construct(bottomCenter, bottomCenter, bottomRight, bottomLeft, glm::vec3(0.f, -1.f, 0.f));
    }
}
// Inserts a glm::vec3 into a vector of floats
void Cone::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
