#include "sceneparser.h"
#include <glm/gtx/transform.hpp>
#include <cmath>
#include "imagereader.h"
#include <iostream>

bool sphere(glm::vec4 raydir, glm::vec4 eye, RenderShapeData i, float &t_out, glm::vec4 &normal_out,glm::vec4 &textureColor,std::vector<std::pair<glm::mat4,Image*>> ctmTextureList)
{
    glm::vec4 objRayDir = i.ctmInverse * raydir;
    glm::vec4 objEye = i.ctmInverse * eye;

    glm::vec3 d = glm::vec3(objRayDir);
    glm::vec3 o = glm::vec3(objEye);

    float r = 0.5f;
    float A = glm::dot(d, d);
    float B = 2.0f * glm::dot(o, d);
    float C = glm::dot(o, o) - r * r;

    float discriminant = B * B - 4 * A * C;

    if (discriminant < 0) {
        return false;
    }

    float sqrtDiscriminant = glm::sqrt(discriminant);
    float t1 = (-B - sqrtDiscriminant) / (2.0f * A);
    float t2 = (-B + sqrtDiscriminant) / (2.0f * A);

    float t;
    if (t1 >= 0 && t2 >= 0) {
        t = glm::min(t1, t2);
    } else if (t1 >= 0) {
        t = t1;
    } else if (t2 >= 0) {
        t = t2;
    } else {
        return false;
    }

    glm::vec3 intersectionPoint = o + t * d;
    glm::vec4 normalObjectSpace = glm::vec4(2*intersectionPoint.x, 2*intersectionPoint.y, 2*intersectionPoint.z, 0);

    if (i.primitive.material.textureMap.isUsed) 
    {
        Image* image = nullptr;
        for (const auto& pair : ctmTextureList) 
        {
            if (pair.first == i.ctm) {
                image = pair.second;
                break;
            }
        }

        if (image) 
        {
            int w = image->width;
            int h = image->height;

            float u, v;

            // circle dimensions
            float y_min = -0.5f; // circle bottom
            float y_max = 0.5f;  // circle top
            float radius = 0.5f; // circle radius

            // Use a threshold to determine if the intersection is on the cap
            float epsilon = 1e-4f;


            float theta = atan2(intersectionPoint.z, intersectionPoint.x);
            if(theta<0)
                u = -theta/(2.0f * M_PI);
            else if(theta>=0)
                u = 1-theta/(2.0f * M_PI);
            
            float phi = glm::asin(intersectionPoint.y/radius);
            v = phi/M_PI + 0.5f;

            if(v==0 || v==1)
                u = 0.5;

            int m = i.primitive.material.textureMap.repeatU;
            int n = i.primitive.material.textureMap.repeatV;
            int c = static_cast<int>(u*w*m)%w;
            int r = static_cast<int>((1-v)*h*n)%h;

            c = (c==m*w)?w-1:c;
            r = (r==h*n)?h-1:r;

            RGBA color = image->data[r * w + c];
            textureColor = glm::vec4(
            float(color.r) / 255.0f,
            float(color.g) / 255.0f,
            float(color.b) / 255.0f,
            1.0f);
        }
    }

    t_out = t;
    normal_out = normalObjectSpace;
    return true;
}

bool cube(glm::vec4 raydir, glm::vec4 eye, RenderShapeData i, float &t_out, glm::vec4 &normal_out,glm::vec4 &textureColor,
          std::vector<std::pair<glm::mat4,Image*>> ctmTextureList)
{
    const float epsilon = 1e-3f;

    glm::vec4 objRayDir = i.ctmInverse * raydir;
    glm::vec4 objEye = i.ctmInverse * eye;
    glm::vec3 d = glm::vec3(objRayDir);
    glm::vec3 o = glm::vec3(objEye);

    float tmin = -FLT_MAX;
    float tmax = FLT_MAX;

    // Check intersection with x planes
    if (std::abs(d.x) >= epsilon) {
        float tx1 = (-0.5f - o.x) / d.x;
        float tx2 = (0.5f - o.x) / d.x;
        tmin = glm::max(tmin, glm::min(tx1, tx2));
        tmax = glm::min(tmax, glm::max(tx1, tx2));
    } else if (o.x > 0.5f || o.x < -0.5f) {
        return false; // Ray is parallel to x plane and outside cube
    }

    // Check intersection with y planes
    if (std::abs(d.y) >= epsilon) {
        float ty1 = (-0.5f - o.y) / d.y;
        float ty2 = (0.5f - o.y) / d.y;
        tmin = glm::max(tmin, glm::min(ty1, ty2));
        tmax = glm::min(tmax, glm::max(ty1, ty2));
    } else if (o.y > 0.5f || o.y < -0.5f) {
        return false; // Ray is parallel to y plane and outside cube
    }

    // Check intersection with z planes
    if (std::abs(d.z) >= epsilon) {
        float tz1 = (-0.5f - o.z) / d.z;
        float tz2 = (0.5f - o.z) / d.z;
        tmin = glm::max(tmin, glm::min(tz1, tz2));
        tmax = glm::min(tmax, glm::max(tz1, tz2));
    } else if (o.z > 0.5f || o.z < -0.5f) {
        return false; // Ray is parallel to z plane and outside cube
    }

    if (tmin > tmax + epsilon || tmax < -epsilon) {
        return false;
    }

    float t = (tmin >= -epsilon) ? tmin : tmax;
    glm::vec3 intersectionPoint = o + t * d;

    // Initialize normal to avoid undefined behavior
    glm::vec4 normalObjectSpace = glm::vec4(0, 0, 0, 0);

    // Find the face that was hit by checking which component is at ±0.5
    if (std::abs(intersectionPoint.x - 0.5f) < epsilon) {
        normalObjectSpace = glm::vec4(1, 0, 0, 0);
    } else if (std::abs(intersectionPoint.x + 0.5f) < epsilon) {
        normalObjectSpace = glm::vec4(-1, 0, 0, 0);
    } else if (std::abs(intersectionPoint.y - 0.5f) < epsilon) {
        normalObjectSpace = glm::vec4(0, 1, 0, 0);
    } else if (std::abs(intersectionPoint.y + 0.5f) < epsilon) {
        normalObjectSpace = glm::vec4(0, -1, 0, 0);
    } else if (std::abs(intersectionPoint.z - 0.5f) < epsilon) {
        normalObjectSpace = glm::vec4(0, 0, 1, 0);
    } else if (std::abs(intersectionPoint.z + 0.5f) < epsilon) {
        normalObjectSpace = glm::vec4(0, 0, -1, 0);
    }

    if (normalObjectSpace == glm::vec4(0, 0, 0, 0)) {
        return false;
    }

    if (i.primitive.material.textureMap.isUsed) 
    {
        Image* image = nullptr;
        for (const auto& pair : ctmTextureList) 
        {
            if (pair.first == i.ctm) {
                image = pair.second;
                break;
            }
        }

        if (image) 
        {
            int w = image->width;
            int h = image->height;

            float u, v;
            float epsilon = 1e-4f;
            float x = intersectionPoint.x;
            float y = intersectionPoint.y;
            float z = intersectionPoint.z;

            // Cube dimensions (assuming it's centered at the origin and has sides of length 1)
            float minCoord = -0.5f;
            float maxCoord = 0.5f;

            // Determine which face the intersection point is on
            if (fabs(x - maxCoord) < epsilon) 
            {
                // Right face (positive X)
                u = 0.5f - z; // Flip z to get the correct orientation
                v = 0.5f + y;
            } else if (fabs(x - minCoord) < epsilon) {
                // Left face (negative X)
                u = 0.5f + z;
                v = 0.5f + y;
            } else if (fabs(y - maxCoord) < epsilon) {
                // Top face (positive Y)
                u = 0.5f + x;
                v = 0.5f - z; // Flip z
            } else if (fabs(y - minCoord) < epsilon) {
                // Bottom face (negative Y)
                u = 0.5f + x;
                v = 0.5f + z;
            } else if (fabs(z - maxCoord) < epsilon) 
            {
                // Front face (positive Z)
                u = 0.5f + x;
                v = 0.5f + y;
            } 
            else if (fabs(z - minCoord) < epsilon) 
            {
                // Back face (negative Z)
                u = 0.5f - x; // Flip x
                v = 0.5f + y;
            } else 
            {
                // Intersection point is not on any face (should not happen)
                u = v = 0.0f;
            }

            // Apply tiling if needed
                int m = i.primitive.material.textureMap.repeatU;
                int n = i.primitive.material.textureMap.repeatV;
                int c = static_cast<int>(u*w*m)%w;
                int r = static_cast<int>((1-v)*h*n)%h;

                c = (c==m*w)?w-1:c;
                r = (r==h*n)?h-1:r;

                RGBA color = image->data[r * w + c];
                textureColor = glm::vec4(
                float(color.r) / 255.0f,
                float(color.g) / 255.0f,
                float(color.b) / 255.0f,
                1.0f);
        }
    }
    
    t_out = t;
    normal_out = normalObjectSpace; //glm::normalize(glm::inverse(glm::transpose(i.ctm)) * normalObjectSpace);
    return true;
}

bool cylinder(glm::vec4 raydir, glm::vec4 eye, RenderShapeData i, float &t_out, glm::vec4 &normal_out,glm::vec4 &textureColor,
              std::vector<std::pair<glm::mat4,Image*>> ctmTextureList)
{
    glm::vec4 objRayDir = i.ctmInverse * raydir;
    glm::vec4 objEye = i.ctmInverse * eye;

    glm::vec3 d = glm::vec3(objRayDir);
    glm::vec3 o = glm::vec3(objEye);

    float A = d.x * d.x + d.z * d.z;
    float B = 2.0f * (o.x * d.x + o.z * d.z);
    float C = o.x * o.x + o.z * o.z - 0.25f;

    float discriminant = B * B - 4 * A * C;

    float t1 = FLT_MAX, t2 = FLT_MAX;
    if (discriminant >= 0) {
        float sqrtDiscriminant = glm::sqrt(discriminant);
        t1 = (-B - sqrtDiscriminant) / (2.0f * A);
        t2 = (-B + sqrtDiscriminant) / (2.0f * A);
    }

    bool valid_t1 = false, valid_t2 = false;
    glm::vec3 P1, P2;

    if (t1 >= 0) {
        P1 = o + t1 * d;
        if (P1.y >= -0.5f && P1.y <= 0.5f) {
            valid_t1 = true;
        }
    }

    if (t2 >= 0) {
        P2 = o + t2 * d;
        if (P2.y >= -0.5f && P2.y <= 0.5f) {
            valid_t2 = true;
        }
    }

    float t3 = (0.5f - o.y) / d.y;
    glm::vec3 P3 = o + t3 * d;
    bool valid_t3 = (t3 >= 0 && (P3.x * P3.x + P3.z * P3.z <= 0.25f));

    float t4 = (-0.5f - o.y) / d.y;
    glm::vec3 P4 = o + t4 * d;
    bool valid_t4 = (t4 >= 0 && (P4.x * P4.x + P4.z * P4.z <= 0.25f));

    float t = FLT_MAX;
    if (valid_t1) t = glm::min(t, t1);
    if (valid_t2) t = glm::min(t, t2);
    if (valid_t3) t = glm::min(t, t3);
    if (valid_t4) t = glm::min(t, t4);

    if (t == FLT_MAX) {
        return false;
    }

    glm::vec3 intersectionPoint = o + t * d;
    glm::vec4 normalObjectSpace;

    if (t == t1 || t == t2) {
        normalObjectSpace = glm::vec4(2.0f * intersectionPoint.x, 0, 2.0f * intersectionPoint.z, 0);
    } else {
        normalObjectSpace = glm::vec4(0, (t == t3) ? 1.0f : -1.0f, 0, 0);
    }

    if (i.primitive.material.textureMap.isUsed) 
    {
        Image* image = nullptr;
        for (const auto& pair : ctmTextureList) 
        {
            if (pair.first == i.ctm) {
                image = pair.second;
                break;
            }
        }

        if (image) 
        {
            int w = image->width;
            int h = image->height;

            float u, v;

            // Cylinder dimensions
            float y_min = -0.5f; // Cylinder bottom
            float y_max = 0.5f;  // Cylinder top
            float radius = 0.5f; // Cylinder radius

            // Use a threshold to determine if the intersection is on the cap
            float epsilon = 1e-4f;

            if (t==t1 || t==t2) 
            {
                // Intersection is on the curved surface
                float theta = atan2(intersectionPoint.z, intersectionPoint.x);
                if(theta<0)
                    u = -theta/(2.0f * M_PI);
                else if(theta>=0)
                    u = 1-theta/(2.0f * M_PI);
                
                v = (intersectionPoint.y - y_min) / (y_max - y_min); // Normalize y to [0, 1]
            }
            else if (fabs(intersectionPoint.y - y_max) < epsilon) 
            {
                    // Intersection is on the caps
                    // Top cap
                    u = 0.5f + intersectionPoint.x;
                    v = 0.5f - intersectionPoint.z;
            } 
            else
            {
                    // Bottom cap
                    u = 0.5f + intersectionPoint.x;
                    v = 0.5f + intersectionPoint.z; // Flip the v-coordinate
            } 
            int m = i.primitive.material.textureMap.repeatU;
            int n = i.primitive.material.textureMap.repeatV;
            int c = static_cast<int>(u*w*m)%w;
            int r = static_cast<int>((1-v)*h*n)%h;

            c = (c==m*w)?w-1:c;
            r = (r==h*n)?h-1:r;

            RGBA color = image->data[r * w + c];
            textureColor = glm::vec4(
            float(color.r) / 255.0f,
            float(color.g) / 255.0f,
            float(color.b) / 255.0f,
            1.0f);
        }
    }

    t_out = t;
    //normal_out = glm::normalize(glm::inverse(glm::transpose(i.ctm)) * normalObjectSpace);
    normal_out = normalObjectSpace;
    return true;
}

bool cone(glm::vec4 raydir, glm::vec4 eye, RenderShapeData i, float &t_out, glm::vec4 &normal_out,glm::vec4 &textureColor,
          std::vector<std::pair<glm::mat4,Image*>> ctmTextureList)
{
    glm::vec4 objRayDir = i.ctmInverse * raydir;
    glm::vec4 objEye = i.ctmInverse * eye;

    glm::vec3 d = glm::vec3(objRayDir);
    glm::vec3 o = glm::vec3(objEye);

    // Conical part (Top part of the cone)
    float A = d.x * d.x + d.z * d.z - (0.25f * d.y * d.y);
    float B = 2.0f * (o.x * d.x + o.z * d.z) - 0.5f * o.y * d.y + 0.25f * d.y;
    float C = (o.x * o.x) + ( o.z * o.z) - (0.25f * o.y * o.y )+ (0.25f * o.y) -( 1.0f / 16.0f);

    float discriminant = B * B - 4.0f * A * C;

    float t1 = FLT_MAX, t2 = FLT_MAX;
    if (discriminant >= 0) {
        float sqrtDiscriminant = glm::sqrt(discriminant);
        t1 = (-B - sqrtDiscriminant) / (2.0f * A);
        t2 = (-B + sqrtDiscriminant) / (2.0f * A);
    }

    // Validating t1 and t2 within the height bounds (-0.5 <= y <= 0.5)
    bool valid_t1 = false, valid_t2 = false;
    glm::vec3 P1, P2;

    if (t1 >= 0) {
        P1 = o + t1 * d;
        if (P1.y >= -0.5f && P1.y <= 0.5f) {
            valid_t1 = true;
        }
    }

    if (t2 >= 0) {
        P2 = o + t2 * d;
        if (P2.y >= -0.5f && P2.y <= 0.5f) {
            valid_t2 = true;
        }
    }

    // Flat base (bottom part of the cone)
    float t3 = (-0.5f - o.y) / d.y; // Solving for intersection with base (y = -0.5)
    glm::vec3 P3 = o + t3 * d;
    bool valid_t3 = (t3 >= 0 && (P3.x * P3.x + P3.z * P3.z <= 0.25f));  // Check if within radius of base

    // Find the smallest valid t
    float t = FLT_MAX;
    if (valid_t1) t = glm::min(t, t1);
    if (valid_t2) t = glm::min(t, t2);
    if (valid_t3) t = glm::min(t, t3);

    // If no valid intersection is found, return false
    if (t == FLT_MAX) {
        return false; // No valid intersection
    }

    // Calculate the intersection point in object space
    glm::vec3 intersectionPoint = o + t * d;
    glm::vec4 normalObjectSpace;

    // Determine if the intersection is on the conical part or the flat base
    if (t == t1 || t == t2) {
        // Intersection with the conical sides
        normalObjectSpace = glm::vec4(8.0f * intersectionPoint.x, -2.f * intersectionPoint.y + 1, 8.f * intersectionPoint.z, 0);
    } else {
        // Intersection with the flat base
        normalObjectSpace = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);
        //normalObjectSpace = glm::vec4(2.0f * intersectionPoint.x, 0 , 2.f * intersectionPoint.z, 0);  // Normal for the flat base is pointing upwards in the negative y-direction
    }
    // Output the intersection t and the transformed normal
    if (i.primitive.material.textureMap.isUsed) 
    {
        Image* image = nullptr;
        for (const auto& pair : ctmTextureList) 
        {
            if (pair.first == i.ctm) {
                image = pair.second;
                break;
            }
        }

        if (image) 
        {
            int w = image->width;
            int h = image->height;

            float u, v;

            // Cylinder dimensions
            float y_min = -0.5f; // Cylinder bottom
            float y_max = 0.5f;  // Cylinder top
            float radius = 0.5f; // Cylinder radius

            // Use a threshold to determine if the intersection is on the cap
            float epsilon = 1e-4f;

            if (t==t1 || t==t2) 
            {
                // Intersection is on the curved surface
                float theta = atan2(intersectionPoint.z, intersectionPoint.x);
                if(theta<0)
                    u = -theta/(2.0f * M_PI);
                else if(theta>=0)
                    u = 1-theta/(2.0f * M_PI);
                
                v = (intersectionPoint.y - y_min) / (y_max - y_min); // Normalize y to [0, 1]
            }
            else if (fabs(intersectionPoint.y - y_max) < epsilon) 
            {
                    // Intersection is on the caps
                    // Top cap
                    u = 0.5f + intersectionPoint.x;
                    v = 0.5f - intersectionPoint.z;
            } 
            else
            {
                    // Bottom cap
                    u = 0.5f + intersectionPoint.x;
                    v = 0.5f + intersectionPoint.z; // Flip the v-coordinate
            } 
            int m = i.primitive.material.textureMap.repeatU;
            int n = i.primitive.material.textureMap.repeatV;
            int c = static_cast<int>(u*w*m)%w;
            int r = static_cast<int>((1-v)*h*n)%h;

            c = (c==m*w)?w-1:c;
            r = (r==h*n)?h-1:r;

            RGBA color = image->data[r * w + c];
            textureColor = glm::vec4(
            float(color.r) / 255.0f,
            float(color.g) / 255.0f,
            float(color.b) / 255.0f,
            1.0f);
        }
    }

    t_out = t;
    normal_out = normalObjectSpace;
    return true;
}
