#ifndef HELPER_H
#define HELPER_H

#include "sceneparser.h"
#include "scenedata.h"
#include <glm/gtx/transform.hpp>
#include <vector>
#include "rgba.h"
#include "imagereader.h"

RGBA normalRGBA(glm::vec4 illumination);
RGBA toRGBA(const glm::vec4 &illumination);

void dfs(SceneNode* node, std::vector<SceneLightData>& lights, std::vector<RenderShapeData>& shapes, glm::mat4 parentCTM);
void checkIntersection(glm::vec4 raydir,glm::vec4 eye, std::vector<RenderShapeData> Shapes, std::vector<SceneLightData> Lights,RGBA &imageData,SceneGlobalData ObjectConstant, int rec_depth,
                       std::vector<std::pair<glm::mat4,Image*>> ctmTextureList, int max_rec=4);






#endif // HELPER_H
