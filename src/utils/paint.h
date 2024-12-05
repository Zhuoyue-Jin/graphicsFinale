#ifndef PAINT_H
#define PAINT_H

#include "realtime.h"

#include <QCoreApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include "settings.h"

#include "utils/sceneparser.h"
#include "utils/helper.h"
#include <glm/gtc/type_ptr.hpp>
#include "debug.h"

void paintTexture(GLuint texture_shader,GLuint m_fbo_texture, GLuint m_fullscreen_vao,bool processing,bool blur, int width, int height)
{
    glUseProgram(texture_shader);

    glUniform2f(glGetUniformLocation(texture_shader, "texSize"), width, height);

    if(processing)
        glUniform1i(glGetUniformLocation(texture_shader, "processing"),1);
    else
        glUniform1i(glGetUniformLocation(texture_shader, "processing"),0);

    if(blur)
        glUniform1i(glGetUniformLocation(texture_shader, "blur"), 1);
    else
        glUniform1i(glGetUniformLocation(texture_shader, "blur"), 0);

    glBindVertexArray(m_fullscreen_vao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_fbo_texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void paint(
    Realtime* realtimeInstance,       // Pointer to the Realtime instance for makeCurrent
    RenderData& data,                 // Scene data, including lights and shapes
    GLuint m_shader,
    const glm::mat4& m_model,         // Model matrix
    const glm::mat4& m_view,          // View matrix
    const glm::mat4& m_proj,          // Projection matrix
    vertexCollection& vertices,       // Vertex collection for VAOs and VBOs
    const Settings& settings,
    GLuint texture_shader,
    GLuint m_fbo,
    GLuint m_fbo_texture,
    GLuint m_fullscreen_vao,
    int width,
    int height,
    GLuint m_defaultFBO
    )
{
    // Ensure OpenGL context is current
    realtimeInstance->makeCurrent();

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    //glViewport(0, 0, m_fbo_width, m_fbo_height); // Match the FBO size
    // Clear the color and depth buffers
    glClearColor(0.f, 0.f, 0.f, 0.f); // Background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightDirections;
    std::vector<glm::vec3> lightFunctions;
    std::vector<glm::vec3> lightColors;
    std::vector<float> lightPenumbra;
    std::vector<float> lightAngle;
    std::vector<int> directional_or_not;

    // Process lights
    for (SceneLightData& light : data.lights)
    {
        lightColors.push_back(glm::vec3(light.color));
        lightFunctions.push_back(glm::vec3(light.function));
        lightPenumbra.push_back(light.penumbra);
        lightAngle.push_back(light.angle);
        lightDirections.push_back(light.dir);

        if (light.type == LightType::LIGHT_SPOT)
        {
            lightPositions.push_back(glm::vec3(light.pos));
            directional_or_not.push_back(0);
        }
        else if(light.type == LightType::LIGHT_POINT)
        {
            lightPositions.push_back(glm::vec3(light.pos));
            directional_or_not.push_back(-1);
        }
        else // DIRECTIONAL
        {
            lightPositions.push_back(glm::vec3(light.dir));
            directional_or_not.push_back(1);
        }
    }

    int numLights = lightPositions.size();

    // Process shapes
    for (RenderShapeData& i : data.shapes)
    {
        glUseProgram(m_shader);

        glm::mat4 currentModel = i.ctm;

        if (settings.extraCredit2)
        {
            int param1 = settings.shapeParameter1;
            int param2 = settings.shapeParameter2;
            adjustPrimitiveParameters(realtimeInstance, i, vertices, currentModel, glm::vec3(data.cameraData.pos), param1, param2);
        }

        // Pass matrices to the shader
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "modelMatrix"), 1, GL_FALSE, &currentModel[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "viewMatrix"), 1, GL_FALSE, &m_view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(m_shader, "projMatrix"), 1, GL_FALSE, &m_proj[0][0]);

        // Pass normal matrix
        glm::mat3 normalMatrix = i.normalMatrix;
        glUniformMatrix3fv(glGetUniformLocation(m_shader, "normalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

        // Pass material properties
        glUniform1f(glGetUniformLocation(m_shader, "k_a"), data.globalData.ka);
        glUniform3fv(glGetUniformLocation(m_shader, "O_a"), 1, &i.primitive.material.cAmbient[0]);
        glUniform1f(glGetUniformLocation(m_shader, "k_d"), data.globalData.kd);
        glUniform4fv(glGetUniformLocation(m_shader, "O_d"), 1, &i.primitive.material.cDiffuse[0]);

        // Pass light data
        glUniform3fv(glGetUniformLocation(m_shader, "lightPositions"), numLights, glm::value_ptr(lightPositions[0]));
        glUniform3fv(glGetUniformLocation(m_shader, "lightFunctions"), numLights, glm::value_ptr(lightFunctions[0]));
        glUniform3fv(glGetUniformLocation(m_shader, "lightColors"), numLights, glm::value_ptr(lightColors[0]));
        glUniform3fv(glGetUniformLocation(m_shader, "lightDirections"), numLights, glm::value_ptr(lightDirections[0]));
        glUniform1iv(glGetUniformLocation(m_shader, "dirOrNot"), numLights, directional_or_not.data());
        glUniform1fv(glGetUniformLocation(m_shader, "lightAngle"), numLights, lightAngle.data());
        glUniform1fv(glGetUniformLocation(m_shader, "lightPenumbra"), numLights, lightPenumbra.data());
        glUniform1i(glGetUniformLocation(m_shader, "numLights"), numLights);

        // Pass specular properties
        glUniform1f(glGetUniformLocation(m_shader, "k_s"), data.globalData.ks);
        glUniform3fv(glGetUniformLocation(m_shader, "O_s"), 1, &i.primitive.material.cSpecular[0]);
        glUniform3fv(glGetUniformLocation(m_shader, "cameraPos"), 1, &data.cameraData.pos[0]);
        glUniform1i(glGetUniformLocation(m_shader, "N"), i.primitive.material.shininess);

        // Draw the appropriate shape
        if (i.primitive.type == PrimitiveType::PRIMITIVE_CUBE)
        {
            glBindVertexArray(vertices.cu_vao);
            glDrawArrays(GL_TRIANGLES, 0, vertices.cube.size() / 6);
        }
        else if (i.primitive.type == PrimitiveType::PRIMITIVE_SPHERE)
        {
            glBindVertexArray(vertices.s_vao);
            glDrawArrays(GL_TRIANGLES, 0, vertices.sphere.size() / 6);
        }
        else if (i.primitive.type == PrimitiveType::PRIMITIVE_CYLINDER)
        {
            glBindVertexArray(vertices.cy_vao);
            glDrawArrays(GL_TRIANGLES, 0, vertices.cylinder.size() / 6);
        }
        else if (i.primitive.type == PrimitiveType::PRIMITIVE_CONE)
        {
            glBindVertexArray(vertices.co_vao);
            glDrawArrays(GL_TRIANGLES, 0, vertices.cone.size() / 6);
        }

        glBindVertexArray(0);
        glUseProgram(0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_defaultFBO);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    paintTexture(texture_shader,m_fbo_texture,m_fullscreen_vao,settings.perPixelFilter,settings.kernelBasedFilter,width,height);

}

#endif // PAINT_H
