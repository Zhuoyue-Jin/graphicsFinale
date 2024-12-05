#include "helper.h"
#include "sceneparser.h"
#include "scenedata.h"
#include <glm/gtx/transform.hpp>
#include <vector>
#include <QSettings>
#include "shapes/cube.h"
#include "shapes/cone.h"
#include "shapes/cylinder.h"
#include "shapes/sphere.h"
#include "realtime.h"
#include "settings.h"
#include <GL/glew.h>
#include <glm/glm.hpp>




glm::mat4 figureCalc(glm::mat4 curr, SceneNode* node)
{
    for (SceneTransformation* node_t: node->transformations)
        switch (node_t->type)
        {
        case TransformationType::TRANSFORMATION_TRANSLATE:
            curr = glm::translate(curr,node_t->translate);
            break;
        case TransformationType::TRANSFORMATION_SCALE :
            curr = glm::scale(curr,node_t->scale);
            break;
        case TransformationType::TRANSFORMATION_ROTATE :
            curr = glm::rotate(curr,node_t->angle, node_t->rotate);
            break;
        case TransformationType::TRANSFORMATION_MATRIX :
            curr *= node_t->matrix;
        }

    return curr;
}

void dfs(SceneNode* node, std::vector<SceneLightData>& lights, std::vector<RenderShapeData>& shapes, glm::mat4 parentCTM)
{
    // Calculate the cumulative transformation matrix for this node
    glm::mat4 CTM = figureCalc(parentCTM, node);

    // Recursively process child nodes
    for (SceneNode* child : node->children) {
        dfs(child, lights, shapes, CTM); // Pass the current CTM to child nodes
    }

    // Process primitives and lights at the current node (only if it's a leaf node)
    if (node->children.empty())
    {
        // Process the primitives at this node
        for (ScenePrimitive* s : node->primitives)
        {
            RenderShapeData shapeData;
            shapeData.primitive = *s;  // Copy the full primitive
            shapeData.ctm = CTM;
            shapeData.normalMatrix =  glm::transpose(glm::inverse(glm::mat3(CTM)));
            shapes.push_back(shapeData);
        }


        for (SceneLight* l : node->lights)
        {
            SceneLightData lightData;
            lightData.id = l->id;
            lightData.type = l->type;
            lightData.color = l->color;
            lightData.function = l->function;
            lightData.penumbra = l->penumbra;
            lightData.angle = l->angle;
            lightData.width = l->width;
            lightData.height = l->height;

            if (l->type == LightType::LIGHT_POINT || l->type == LightType::LIGHT_SPOT)
                lightData.pos = CTM * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

            if (l->type == LightType::LIGHT_DIRECTIONAL || l->type == LightType::LIGHT_SPOT)
                lightData.dir = glm::normalize(CTM * l->dir);

            lights.push_back(lightData);
        }
    }
}


void setupBuffers(GLuint& vao, GLuint& vbo, const std::vector<float>& vertices,vertexCollection& ver)
{
    ver.instance->makeCurrent();
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

    // Color/Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    // Cleanup
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void scan_primitives(int param1, int param2, vertexCollection& vertices)
{
    int vertex_count = 0;

    if (vertices.status[1]) {
        vertex_count += handleCube(param1, vertices);
    }

    if (vertices.status[3]) {
        vertex_count += handleCone(param1, param2, vertices);
    }

    if (vertices.status[2]) {
        vertex_count += handleCylinder(param1, param2, vertices);
    }

    if (vertices.status[0]) {
        vertex_count += handleSphere(param1, param2, vertices);
    }


}

int handleCube(int param1, vertexCollection& vertices)
{
    Cube cube;
    cube.updateParams(param1);
    vertices.cube = cube.generateShape();
    setupBuffers(vertices.cu_vao, vertices.cu_vbo, vertices.cube,vertices);
    return vertices.cube.size(); // Return the vertex count
}

int handleCone(int param1, int param2, vertexCollection& vertices)
{
    Cone cone;
    cone.updateParams(param1, param2);
    vertices.cone = cone.generateShape();
    setupBuffers(vertices.co_vao, vertices.co_vbo, vertices.cone,vertices);
    return vertices.cone.size(); // Return the vertex count
}

int handleSphere(int param1, int param2, vertexCollection& vertices)
{
    Sphere sphere;
    sphere.updateParams(param1, param2);
    vertices.sphere = sphere.generateShape();
    setupBuffers(vertices.s_vao, vertices.s_vbo, vertices.sphere,vertices);
    return vertices.sphere.size(); // Return the vertex count
}

int handleCylinder(int param1, int param2, vertexCollection& vertices)
{
    Cylinder cylinder;
    cylinder.updateParams(param1, param2);
    vertices.cylinder = cylinder.generateShape();
    setupBuffers(vertices.cy_vao, vertices.cy_vbo, vertices.cylinder,vertices);
    return vertices.cylinder.size(); // Return the vertex count
}


void calcMatrices(RenderData& data, glm::mat4& m_view, glm::mat4& m_proj, int width, int height)
{

    m_view = glm::lookAt(glm::vec3(data.cameraData.pos),
                         glm::vec3(data.cameraData.pos) + glm::vec3(data.cameraData.look),
                         glm::vec3(data.cameraData.up));


    // Aspect ratio
    float aspectRatio = static_cast<float>(width) / height;

    // Vertical field of view angle in radians
    float theta_h = data.cameraData.heightAngle;

    // Near and Far planes
    float near = settings.nearPlane;
    float far = settings.farPlane;


    float tanHalfFovy = tan(theta_h / 2.0f);
    float top = near * tanHalfFovy;
    float right = top * aspectRatio;


    m_proj = glm::mat4(0.0f);

    m_proj[0][0] = near / right;
    m_proj[1][1] = near / top;
    m_proj[2][2] = -(far + near) / (far - near);
    m_proj[2][3] = -1.0f;
    m_proj[3][2] = -(2.0f * far * near) / (far - near);
    m_proj[3][3] = 0.0f;
}


void calcMatrices(RenderData& data, glm::mat4& m_view, glm::mat4& m_proj,
                  int width, int height, std::unordered_map<Qt::Key, bool> m_keyMap, float deltaTime)
{

    glm::vec3 movement(0.0f);

    glm::vec3 lookVec = glm::normalize(glm::vec3(data.cameraData.look));
    glm::vec3 upVec = glm::normalize(glm::vec3(data.cameraData.up));
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


    data.cameraData.pos += glm::vec4(movement, 0.0f);


    m_view = glm::lookAt(glm::vec3(data.cameraData.pos),
                         glm::vec3(data.cameraData.pos) + glm::vec3(data.cameraData.look),
                         glm::vec3(data.cameraData.up));


    float aspectRatio = static_cast<float>(width) / height;
    float theta_h = data.cameraData.heightAngle;
    float near = settings.nearPlane;
    float far = settings.farPlane;

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
void calcMatrices(RenderData& data, glm::mat4& m_view, glm::mat4& m_proj,
                  int width, int height, float deltaX, float deltaY)
{
    float angleX = glm::radians(1.0f) * deltaX;
    float angleY = glm::radians(1.0f) * deltaY;


    glm::vec3 axisY = glm::vec3(0.f, 1.f, 0.f);
    glm::mat4 rotationY = rotationM(axisY, angleX);


    data.cameraData.look = rotationY * data.cameraData.look;
    data.cameraData.up = rotationY * data.cameraData.up;


    glm::vec3 lookVec = glm::normalize(glm::vec3(data.cameraData.look));
    glm::vec3 upVec = glm::normalize(glm::vec3(data.cameraData.up));
    glm::vec3 rightVec = glm::normalize(glm::cross(lookVec, upVec));

    glm::mat4 rotationX = rotationM(rightVec, angleY);


    data.cameraData.look = rotationX * data.cameraData.look;
    data.cameraData.up = rotationX * data.cameraData.up;


    data.cameraData.look = glm::normalize(data.cameraData.look);
    data.cameraData.up = glm::normalize(data.cameraData.up);


    m_view = glm::lookAt(glm::vec3(data.cameraData.pos),
                         glm::vec3(data.cameraData.pos) + glm::vec3(data.cameraData.look),
                         glm::vec3(data.cameraData.up));


    float aspectRatio = static_cast<float>(width) / height;
    float theta_h = data.cameraData.heightAngle;
    float near = settings.nearPlane;
    float far = settings.farPlane;

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
void figureVertices(RenderData data, vertexCollection& vertices)
{
    for(RenderShapeData& i : data.shapes)
    {
        if(i.primitive.type == PrimitiveType::PRIMITIVE_CUBE)
            vertices.status[1] = 1;
        else if(i.primitive.type == PrimitiveType::PRIMITIVE_SPHERE)
            vertices.status[0] = 1;
        else if(i.primitive.type == PrimitiveType::PRIMITIVE_CYLINDER)
            vertices.status[2] = 1;
        else if(i.primitive.type == PrimitiveType::PRIMITIVE_CONE)
            vertices.status[3] = 1;

    }
}

void extraCredits1(int& baseParam1, int& baseParam2, int param_count)
{

    if (baseParam1 > 10 || baseParam2 > 10) 
    {

        int maxTessellation = 25;

        int startReduction = 15;  // Start scaling after 15 objects
        int maxReductionCount = 30; // Maximum reduction by 30 objects

        float scaleFactor = 1.0f;
        if (param_count > startReduction) {
            scaleFactor = std::max(0.1f, (float)(maxReductionCount - std::min(param_count, maxReductionCount)) / (maxReductionCount - startReduction));
        }


    baseParam1 = std::max(15, static_cast<int>(maxTessellation * scaleFactor)); // Minimum 10, scaled down
    baseParam2 = std::max(15, static_cast<int>(maxTessellation * scaleFactor)); // Minimum 10, scaled down
    }
        
}

void adjustPrimitiveParameters(Realtime* realtimeInstance,RenderShapeData& shape, vertexCollection& vertices, const glm::mat4& modelMatrix, const glm::vec3& cameraPos, int& param1, int& param2) {

    realtimeInstance->makeCurrent();
    float distanceToCamera = glm::length(glm::vec3(modelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)) - cameraPos);

    int adjustedParam1 = param1;
    int adjustedParam2 = param2;

    float distanceThreshold = 10.0f;
    if (distanceToCamera >= distanceThreshold)
    {
        adjustedParam1 = glm::clamp(param1 - static_cast<int>(distanceToCamera / 10.0f), 1, 15);
        adjustedParam2 = glm::clamp(param2 - static_cast<int>(distanceToCamera / 10.0f), 1, 15);
    }


    switch (shape.primitive.type) {
    case PrimitiveType::PRIMITIVE_CUBE:
        handleCube(adjustedParam1, vertices);
        break;
    case PrimitiveType::PRIMITIVE_SPHERE:
        handleSphere(adjustedParam1, adjustedParam2, vertices);
        break;
    case PrimitiveType::PRIMITIVE_CYLINDER:
        handleCylinder(adjustedParam1, adjustedParam2, vertices);
        break;
    case PrimitiveType::PRIMITIVE_CONE:
        handleCone(adjustedParam1, adjustedParam2, vertices);
        break;
    default:
        break; // Handle other primitive types if needed
    }
}

