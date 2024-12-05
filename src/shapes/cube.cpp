#include "cube.h"

void Cube::updateParams(int param1) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    setVertexData();
}

void Cube::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 2: create a tile (i.e. 2 triangles) based on 4 given points.
    // Triangle 1
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(glm::cross(bottomLeft - topLeft, bottomRight - topLeft)));
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, glm::normalize(glm::cross(bottomRight - bottomLeft, topLeft - bottomLeft)));
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(glm::cross(topLeft - bottomRight, bottomLeft - bottomRight)));

    // Triangle 2
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, glm::normalize(glm::cross(topRight - bottomRight, topLeft - bottomRight)));
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, glm::normalize(glm::cross(topLeft - topRight, bottomRight - topRight)));
    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, glm::normalize(glm::cross(bottomRight - topLeft, topRight - topLeft)));
}

glm::vec3 scale(glm::vec3 vec, int i)
{
    return glm::vec3(vec[0] * i, vec[1] * i, vec[2] * i);
}

void Cube::makeFace(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight) {
    // Task 3: create a single side of the cube out of the 4
    //         given points and makeTile()
    // Note: think about how param 1 affects the number of triangles on
    //       the face of the cube

    // Calculate the incremental shifts for each tile based on tessellation factor
    glm::vec3 rightShift = (topRight - topLeft) / static_cast<float>(m_param1);
    glm::vec3 downShift = (bottomLeft - topLeft) / static_cast<float>(m_param1);

    for (int i = 0; i < m_param1; i++) {
        glm::vec3 startPoint_i = topLeft + scale(downShift, i);
        for (int j = 0; j < m_param1; j++) {
            glm::vec3 startPoint_j = startPoint_i + scale(rightShift, j);
            makeTile(
                startPoint_j,
                startPoint_j + rightShift,
                startPoint_j + downShift,
                startPoint_j + rightShift + downShift
                );
        }
    }
}

void Cube::setVertexData() {
    // Uncomment these lines for Task 2, then comment them out for Task 3:

    // makeTile(glm::vec3(-0.5f,  0.5f, 0.5f),
    //          glm::vec3( 0.5f,  0.5f, 0.5f),
    //          glm::vec3(-0.5f, -0.5f, 0.5f),
    //          glm::vec3( 0.5f, -0.5f, 0.5f));

    // Uncomment these lines for Task 3:

    //front face
    makeFace(glm::vec3(-0.5f,  0.5f, 0.5f),
             glm::vec3( 0.5f,  0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, 0.5f),
             glm::vec3( 0.5f, -0.5f, 0.5f));

    //left face
    makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3( -0.5f,  0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3( -0.5f, -0.5f, 0.5f));

    //right face
    makeFace(glm::vec3(0.5f,  0.5f, 0.5f),
             glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3(0.5f, -0.5f, 0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f));

    //top face
    makeFace(glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3(0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f, 0.5f, 0.5f),
             glm::vec3(0.5f, 0.5f, 0.5f));

    //bottom face
    makeFace(glm::vec3(-0.5f,  -0.5f, 0.5f),
             glm::vec3(0.5f,  -0.5f, 0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f),
             glm::vec3(0.5f, -0.5f, -0.5f));


    // Back face with normals facing backward
    makeFace(glm::vec3( 0.5f,  0.5f, -0.5f),
             glm::vec3(-0.5f,  0.5f, -0.5f),
             glm::vec3( 0.5f, -0.5f, -0.5f),
             glm::vec3(-0.5f, -0.5f, -0.5f));


    // Task 4: Use the makeFace() function to make all 6 sides of the cube
}

// Inserts a glm::vec3 into a vector of floats.
// This will come in handy if you want to take advantage of vectors to build your shape!
void Cube::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}
