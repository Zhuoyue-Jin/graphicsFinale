#include "helper.h"
#include "sceneparser.h"
#include "scenedata.h"
#include <glm/gtx/transform.hpp>
#include <vector>
#include <cmath>
#include "rgba.h"
#include "shapes.h"
#include "reflection.h"


RGBA phong(glm::vec4 position, // position of intersection
           glm::vec4 normal,    // normal in world space
           glm::vec4 directionToCamera, // direction to camera
           SceneMaterial &material, // material properties
           std::vector<SceneLightData> &lights, //
           SceneGlobalData ObjectConstant,
           glm::mat4 ctm,
           std::vector<RenderShapeData> Shapes,
           int rec_depth,
           //textures
           glm::vec4 textureColor,
           std::vector<std::pair<glm::mat4,Image*>> ctmTextureList) {

    // Normalizing directions
    normal = glm::normalize(normal);
    directionToCamera = glm::normalize(directionToCamera);

    // Output illumination
    glm::vec4 illumination(0, 0, 0, 1);

    // Ambient term

    illumination += ObjectConstant.ka * material.cAmbient;
    float n = material.shininess;
    float attenuation;

    for (const SceneLightData &light : lights)
    {
        if(checkShadow(light,position,Shapes,ctmTextureList))
            continue;

        glm::vec4 incident = glm::normalize(-light.dir); // assuming it's directional

        if(light.type==LightType::LIGHT_POINT || light.type==LightType::LIGHT_SPOT)
            incident = glm::normalize(light.pos-position); // Because emits light in all direction

        float dot_prod = glm::dot(normal,incident);
        dot_prod *= dot_prod>0;

        glm::vec4 diffuse;
        if (material.textureMap.isUsed) {
            
            // Compute the base diffuse color (using material.cDiffuse)
            glm::vec4 baseDiffuse = ObjectConstant.kd * material.cDiffuse * dot_prod;

            // Compute the texture diffuse color
            glm::vec4 textureDiffuse = textureColor * dot_prod;

            // Perform linear interpolation based on the blend factor
            diffuse = (1.0f - material.blend) * baseDiffuse + material.blend * textureDiffuse;
        } else {
            // If no texture, use only the base diffuse color
            diffuse = ObjectConstant.kd * material.cDiffuse * dot_prod;
        }

        glm::vec4 reflectionDir =  (2.0f * glm::dot(normal,incident) * normal) - incident;
        reflectionDir = glm::normalize(reflectionDir);

        // specular
        dot_prod = glm::dot(reflectionDir,directionToCamera);
        dot_prod *= dot_prod>0;
        glm::vec4 specular = ObjectConstant.ks * material.cSpecular * pow(dot_prod, n);


        // attenuation

        float distance = glm::distance(position,light.pos);
        attenuation = pow(light.function.x + distance * light.function.y + pow(distance,2) * light.function.z,-1);
        attenuation = glm::clamp(attenuation, 0.0f, 1.0f);

        if(light.type==LightType::LIGHT_DIRECTIONAL)
            attenuation = 1.0f;

        // SPOT LIGHT FALLOUT
        if(light.type==LightType::LIGHT_SPOT)
        {
            // Assuming outer_angle = inner_angle * 2
            SceneColor spotLightColor;
            float inner_angle = light.angle - light.penumbra;

            float dotProduct = glm::dot(-incident, light.dir);
            float magnitudeA = glm::length(-incident);
            float magnitudeB = glm::length(light.dir);
            float x = glm::acos(dotProduct / (magnitudeA * magnitudeB));


            if(x<=inner_angle)
                spotLightColor = light.color;
            else if(x>inner_angle && x<=light.angle)
            {
                float val = (x-inner_angle)/(light.angle - inner_angle);
                float falloff = 1 - (-2 * pow(val,3) + 3 * pow(val,2));
                spotLightColor = light.color * falloff;
            }
            else
            {
                spotLightColor = glm::vec4(0);
            }
            illumination += attenuation * spotLightColor * (diffuse + specular);
        }
        else
            illumination += attenuation * light.color * (diffuse + specular);
    }

    if(glm::length(material.cReflective)!= 0)
    {
        // REFLECTION
        glm::vec4 reflectionDir =  (2.0f * glm::dot(normal,directionToCamera) * normal) - directionToCamera;
        reflectionDir = glm::normalize(reflectionDir); 
        RGBA color{0,0,0};


        auto p = position + reflectionDir * 1e-3f;
            
        checkIntersection(reflectionDir,p,Shapes,lights,color,ObjectConstant,rec_depth,ctmTextureList);
        glm::vec4 clr(
        float(color.r) / 255.0f,
        float(color.g) / 255.0f,
        float(color.b) / 255.0f,
        1.0f
        );


        clr = glm::clamp(clr, 0.0f, 1.0f);
        // REFLECTION OVER

        illumination += ObjectConstant.ks * material.cReflective * clr;
    }

    // Ensure that illumination values are clamped between 0.0f and 1.0f
    //illumination = glm::clamp(illumination, 0.0f, 1.0f);
    return toRGBA(illumination);
}

bool checkShadow(SceneLightData light, glm::vec4 position, std::vector<RenderShapeData> Shapes,
                 std::vector<std::pair<glm::mat4,Image*>> ctmTextureList)
{
    glm::vec4 raydir;
    glm::vec4 eye;
    float maxDistance = FLT_MAX;

    // Determine ray direction and maxDistance based on light type
    if (light.type == LightType::LIGHT_POINT || light.type == LightType::LIGHT_SPOT)
    {
        // For point lights and spotlights
        raydir = glm::normalize(light.pos - position);
        maxDistance = glm::distance(position, light.pos);
        eye = position + raydir * 1e-3f;  // Offset to prevent self-shadowing
    }
    else if (light.type == LightType::LIGHT_DIRECTIONAL)
    {
        // For directional lights
        raydir = glm::normalize(-light.dir);
        maxDistance = FLT_MAX;
        eye = position + raydir * 1e-3f;  // Offset to prevent self-shadowing
    }


    for (RenderShapeData &shape : Shapes)
    {
        float current_t;
        glm::vec4 current_normal;
        bool intersection_found = false;
        glm::vec4 textureColor;

        if (shape.primitive.type == PrimitiveType::PRIMITIVE_SPHERE)
            intersection_found = sphere(raydir, eye, shape, current_t, current_normal,textureColor,ctmTextureList);
        else if (shape.primitive.type == PrimitiveType::PRIMITIVE_CYLINDER)
            intersection_found = cylinder(raydir, eye, shape, current_t, current_normal,textureColor,ctmTextureList);
        else if (shape.primitive.type == PrimitiveType::PRIMITIVE_CONE)
            intersection_found = cone(raydir, eye, shape, current_t, current_normal,textureColor,ctmTextureList);
        else if (shape.primitive.type == PrimitiveType::PRIMITIVE_CUBE)
            intersection_found = cube(raydir, eye, shape, current_t, current_normal,textureColor,ctmTextureList);


        if (intersection_found && current_t >= 0)
        {
            glm::vec4 intersectionPoint = eye + raydir * current_t;
            float intersectionDistance = glm::length(intersectionPoint - position);

            // If the intersection is within the range to the light, there's a shadow
            if (intersectionDistance < maxDistance)
                return true;
        }
    }

    // No obstruction found; no shadow
    return false;
}

