#ifndef HELPER_H
#define HELPER_H

#include "sceneparser.h"
#include "scenedata.h"
#include <glm/gtx/transform.hpp>
#include <vector>
#include "realtime.h"

void dfs(SceneNode* node, std::vector<SceneLightData>& lights, std::vector<RenderShapeData>& shapes, glm::mat4 parentCTM);
void scan_primitives(int param1,int param2, vertexCollection& vertices);
void calcMatrices(RenderData& data,glm::mat4& m_view,glm::mat4& m_proj,int width, int height);
void calcMatrices(RenderData& data, glm::mat4& m_view, glm::mat4& m_proj, int width, int height, std::unordered_map<Qt::Key, bool> m_keyMap, float deltaTime);
void calcMatrices(RenderData& data, glm::mat4& m_view, glm::mat4& m_proj, int width, int height, float deltaX, float deltaY);
void figureVertices(RenderData data, vertexCollection& vertices);
void extraCredits1(int& baseParam1, int& baseParam2, int param_count);

int handleCube(int param1, vertexCollection& vertices);
int handleCone(int param1, int param2, vertexCollection& vertices);
int handleSphere(int param1, int param2, vertexCollection& vertices);
int handleCylinder(int param1, int param2, vertexCollection& vertices);
void adjustPrimitiveParameters(Realtime* realtimeInstance, RenderShapeData& shape, vertexCollection& vertices, const glm::mat4& modelMatrix, const glm::vec3& cameraPos, int& param1, int& param2);

#endif // HELPER_H
