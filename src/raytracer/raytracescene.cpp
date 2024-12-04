
#include "raytracescene.h"
#include "utils/sceneparser.h"



RayTraceScene::RayTraceScene(int width, int height, const RenderData &metaData)
    : m_width(width),
    m_height(height),
    m_globalData(metaData.globalData),
    m_SceneLight(metaData.lights),
    m_Shapes(metaData.shapes),
    m_camera(Camera(width, height,metaData))
{}

const int& RayTraceScene::width() const {
    // Optional TODO: implement the getter or make your own design
    // throw std::runtime_error("not implemented");
    return m_width;
}

const int& RayTraceScene::height() const {
    // Optional TODO: implement the getter or make your own design
    // throw std::runtime_error("not implemented");
    return m_height;
}

const SceneGlobalData& RayTraceScene::getGlobalData() const {
    // Optional TODO: implement the getter or make your own design
    // throw std::runtime_error("not implemented");
    return m_globalData;
}


const Camera& RayTraceScene::getCamera() const {
    // Optional TODO: implement the getter or make your own design
    // throw std::runtime_error("not implemented");
    return m_camera;
}
