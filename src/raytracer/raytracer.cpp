#include "raytracer.h"
#include "raytracescene.h"
#include "utils/scenedata.h"
#include "utils/helper.h"
#include <glm/glm.hpp>
#include "utils/helper.h"
#include "utils/imagereader.h"

RayTracer::RayTracer(Config config) :
    m_config(config)
{}

void RayTracer::render(RGBA *imageData, const RayTraceScene &scene) {
    // Note that we're passing `data` as a pointer (to its first element)
    // Recall from Lab 1 that you can access its elements like this: `data[i]`

    // TODO: Implement the ray tracing algorithm. Good luck!
    int width = scene.m_width;
    int height = scene.m_height;

    SceneGlobalData ObjectConstant = scene.getGlobalData();
    Camera camera = scene.getCamera();
    std::vector<SceneLightData> Lights = scene.m_SceneLight;
    std::vector<RenderShapeData> Shapes = scene.m_Shapes;

    float k = .1f;
    float horizontal_angle = camera.getHeightAngle();
    float V = 2 * k * glm::tan(horizontal_angle/2);
    float U = (width * V)/height;

    std::vector<std::pair<glm::mat4,Image*>> ctmTextureList;

    // images ctm stored
    for(RenderShapeData shape : Shapes)
    {
        if(!shape.primitive.material.textureMap.isUsed)
            continue;
        std::string filename = shape.primitive.material.textureMap.filename;
        Image* image= loadImageFromFile(filename);
        ctmTextureList.emplace_back(shape.ctm,image);
    }


    // pixel iterations
    for (int j = 0; j<height ; j++)
        for (int i = 0 ; i<width ; i++)
        {
            float x = ((i + 0.5)/width)-0.5;
            float y = ((height - 1 - j + 0.5)/height)-0.5;

            glm::vec4 uvk(U*x,V*y,-k,1.f);

            glm::vec4 eye(0.f,0.f,0.f,1.f); // the eye will always be at 0,0,0,1 because you are in camera co-ordinate space.
            glm::vec4 raydir = glm::normalize((uvk-eye));
            glm::vec4 worldEye = camera.getViewMatrixInverse() * eye;
            glm::vec4 worldRayDir = glm::normalize(camera.getViewMatrixInverse() * raydir); // to world space
            checkIntersection(worldRayDir,worldEye,Shapes,Lights,imageData[j * width + i],ObjectConstant,0,ctmTextureList,m_config.maxRecursiveDepth);
        }



}
