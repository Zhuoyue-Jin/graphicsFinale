#ifndef SHAPES_H
#define SHAPES_H


#include "sceneparser.h"
#include <glm/gtx/transform.hpp>


bool sphere(glm::vec4 raydir, glm::vec4 eye, RenderShapeData i, float &t_out, glm::vec4 &normal_out, glm::vec4 &textureColor,
            std::vector<std::pair<glm::mat4,Image*>> ctmTextureList);
bool cube(glm::vec4 raydir, glm::vec4 eye, RenderShapeData i, float &t_out, glm::vec4 &normal_out,glm::vec4 &textureColor,
          std::vector<std::pair<glm::mat4,Image*>> ctmTextureList);
bool cylinder(glm::vec4 raydir, glm::vec4 eye, RenderShapeData i, float &t_out, glm::vec4 &normal_out,glm::vec4 &textureColor,
              std::vector<std::pair<glm::mat4,Image*>> ctmTextureList);
bool cone(glm::vec4 raydir, glm::vec4 eye, RenderShapeData i, float &t_out, glm::vec4 &normal_out,glm::vec4 &textureColor,
          std::vector<std::pair<glm::mat4,Image*>> ctmTextureList);

#endif // SHAPES_H
