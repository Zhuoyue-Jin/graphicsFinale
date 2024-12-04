#include "helper.h"
#include "sceneparser.h"
#include "scenedata.h"
#include <glm/gtx/transform.hpp>
#include <vector>
#include "rgba.h"
#include "shapes.h"
#include "reflection.h"
#include <QSettings>
#include "imagereader.h"
/*
 *  Populating primitives
 */

RGBA normalRGBA(glm::vec4 illumination)
{
    std::uint8_t r = ((illumination[0]+1)/2)*255;
    std::uint8_t g = ((illumination[1]+1)/2)*255;
    std::uint8_t b = ((illumination[2]+1)/2)*255;

    r = (r>255)?255:r;
    g = (g>255)?255:g;
    b = (b>255)?255:b;

    return RGBA{r, g, b};
}


RGBA toRGBA(const glm::vec4 &illumination) {


    std::uint8_t r = (illumination[0]>1)?255:int(illumination[0]*255);
    std::uint8_t g = (illumination[1]>1)?255:int(illumination[1]*255);
    std::uint8_t b = (illumination[2]>1)?255:int(illumination[2]*255);

    return RGBA{r, g, b,255};
}


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
            shapeData.ctmInverse = glm::inverse(CTM);           // Apply the cumulative transformation matrix
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


void checkIntersection(glm::vec4 raydir, glm::vec4 eye, std::vector<RenderShapeData> Shapes, std::vector<SceneLightData> Lights, RGBA &imageData,SceneGlobalData ObjectConstant, int rec_depth,
                       std::vector<std::pair<glm::mat4,Image*>> ctmTextureList, int max_rec)
{
    if(rec_depth==max_rec)
        return ;
    else
        rec_depth++;

    float min_t = FLT_MAX;

    glm::vec4 final_normal; // supposed normal in object space
    glm::vec4 final_texture;

    bool hit_found = false;

    ScenePrimitive final;
    glm::mat4 xtm(1.0f);


    for(RenderShapeData& shape : Shapes)
    {
        float current_t;
        glm::vec4 current_normal;
        bool intersection_found = false;
        glm::vec4 textureColor;


        if(shape.primitive.type == PrimitiveType::PRIMITIVE_SPHERE)
            intersection_found = sphere(raydir, eye, shape, current_t, current_normal,textureColor, ctmTextureList);
        else if(shape.primitive.type == PrimitiveType::PRIMITIVE_CYLINDER)
            intersection_found = cylinder(raydir, eye, shape, current_t, current_normal,textureColor,ctmTextureList);
        else if(shape.primitive.type == PrimitiveType::PRIMITIVE_CONE)
            intersection_found = cone(raydir, eye, shape, current_t, current_normal,textureColor, ctmTextureList);
        else if(shape.primitive.type == PrimitiveType::PRIMITIVE_CUBE)
            intersection_found = cube(raydir, eye, shape, current_t, current_normal,textureColor,ctmTextureList);

        if(intersection_found && current_t < min_t && current_t >= 0)
        {
            min_t = current_t;
            final_normal = current_normal;
            hit_found = true;
            final = shape.primitive;
            xtm = shape.ctm;
            final_texture = textureColor;
        }
    }

    if(hit_found)
    {
        glm::vec4 intersection_point = eye + glm::vec4(raydir.x* min_t, raydir.y * min_t, raydir.z *min_t,0);
        glm::vec4 normal = final_normal;
        glm::vec4 normal_out = glm::vec4(glm::normalize(glm::inverse(glm::transpose(glm::mat3(xtm))) * glm::vec3(final_normal)),0);
        glm::vec4 dirCam = -raydir;
        SceneMaterial mat = final.material;
        glm::mat4 final_ctm = xtm;

        //imageData = normalRGBA(normal_out);
        imageData = phong(intersection_point,normal_out,dirCam,mat,Lights,ObjectConstant,xtm,Shapes,rec_depth,final_texture,ctmTextureList);
    }
}



