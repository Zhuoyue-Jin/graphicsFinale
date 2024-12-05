#include "sceneparser.h"
#include "scenefilereader.h"
#include <glm/gtx/transform.hpp>
#include "helper.h"

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readJSON();
    if (!success) {
        return false;
    }


    // TODO: Use your Lab 5 code here

    renderData.globalData = fileReader.getGlobalData();
    renderData.cameraData = fileReader.getCameraData();
    SceneNode* root_node = fileReader.getRootNode();
    renderData.shapes.clear();
    renderData.lights.clear();

    glm::mat4 identityMatrix(1.0f);
    dfs(root_node,renderData.lights,renderData.shapes,identityMatrix);

    return true;
}
