#pragma once

#include <vector>
#include <glm/glm.hpp>

class Cylinder
{
public:
    void updateParams(int param1, int param2);
    std::vector<float> generateShape() { return m_vertexData; }

private:
    void insertVec3(std::vector<float> &data, glm::vec3 v);
    void setVertexData();
    void construct(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 normal);
    void construct(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 normal, glm::vec3 normal2);
    void makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 normal);
    void makeTile(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 normal1, glm::vec3 normal2);

    std::vector<float> m_vertexData;
    int m_param1;
    int m_param2;
    float m_radius = 0.5;
};
