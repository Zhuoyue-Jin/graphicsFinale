#ifndef REFLECTION_H
#define REFLECTION_H

#include "sceneparser.h"
#include "scenedata.h"
#include <glm/gtx/transform.hpp>
#include <vector>
#include "rgba.h"

RGBA phong(glm::vec4 position,
           glm::vec4 normal,
           glm::vec4 directionToCamera,
           SceneMaterial &material,
           std::vector<SceneLightData> &lights,
           SceneGlobalData ObjectConstant,
           glm::mat4 ctm,
           std::vector<RenderShapeData> Shapes,
           int rec_depth,
           glm::vec4 textureColor,
           std::vector<std::pair<glm::mat4,Image*>> ctmTextureList);

bool checkShadow(SceneLightData light, glm::vec4 position, std::vector<RenderShapeData> Shapes,std::vector<std::pair<glm::mat4,Image*>> ctmTextureList);


#endif // REFLECTION_H
