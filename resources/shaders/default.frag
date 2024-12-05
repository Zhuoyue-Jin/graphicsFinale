#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;

out vec4 fragColor;

uniform float k_a;                 // Ambient coefficient
uniform vec3 O_a;                  // Ambient color
uniform float k_d;                 // Diffuse coefficient
uniform vec4 O_d;                  // Diffuse color
uniform float k_s;                 // Specular coefficient
uniform vec3 O_s;                  // Specular color
uniform int N;                     // Shininess for specular highlight
uniform vec3 cameraPos;            // Camera position for specular calculation

uniform vec3 lightPositions[100];  // Array for light positions
uniform vec3 lightDirections[100];  // Array for light positions
uniform vec3 lightColors[100];     // Array for light colors
uniform vec3 lightFunctions[100];     // Array for light functions
uniform float lightAngle[100]; // array for light angles
uniform float lightPenumbra[100]; //array for light penumbra
uniform int dirOrNot[100];         // Array indicating if each light is directional (1), point (-1) , spot (0)
uniform int numLights;             // Number of actual lights passed

void main()
{


    vec3 norm = normalize(worldNormal);
    vec3 dirToCam = normalize(cameraPos - worldPosition);

    // Initialize illumination with ambient component
    vec3 illumination = k_a * O_a;

    for (int i = 0; i < numLights; ++i)
    {
        vec3 lightDir;
        float dist;
        float attenuation = 1.0f;
        vec3 specular = vec3(0.f,0.f,0.f);

        // Determine if the light is directional or point light
        if (dirOrNot[i] == 1)
        {
            // Directional light
            lightDir = normalize(-lightPositions[i]);
        }
        else
        {
            // Point light or Spot Lights
            lightDir = normalize(lightPositions[i] - worldPosition);
        }

        // Diffuse component
        float dotprod = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = k_d * dotprod * vec3(O_d.rgb) * lightColors[i];
        // illumination += diffuse;

        // Specular component (only if shininess N > 0)
        if (N > 0) {
            vec3 reflectedRay = reflect(-lightDir, norm);
            float dotProd = max(dot(reflectedRay, dirToCam), 0.0);
            float specIntensity = (dotProd > 0.0) ? pow(dotProd, N) : 0.0;
            specular = k_s * specIntensity * O_s * lightColors[i];
            // illumination += specular;
        }

        //attenuation


        dist = distance(lightPositions[i],worldPosition);
        if(dirOrNot[i]!=1)
            attenuation = 1.f/(lightFunctions[i][0] + (lightFunctions[i][1]*dist) + (lightFunctions[i][2]*dist*dist));

        // spot light angular fall off
        if(dirOrNot[i] == 0)
        {
            vec3 spotLightColor;

            float inner_angle = lightAngle[i] - lightPenumbra[i];
            float dotProduct = dot(-lightDir, lightDirections[i]); // lightdir is light to incident whereas lightdirections is the angle at which spot light is pointed
            float magnitudeA = length(-lightDir);
            float magnitudeB = length(lightDirections[i]);
            float x = acos(dotProduct / (magnitudeA * magnitudeB));


            if(x<=inner_angle)
                spotLightColor = lightColors[i];
            else if(x>inner_angle && x<=lightAngle[i])
            {
                float val = (x-inner_angle)/(lightAngle[i] - inner_angle);
                float falloff = 1 - (-2 * pow(val,3) + 3 * pow(val,2));
                spotLightColor = lightColors[i] * falloff;
            }
            else
            {
                spotLightColor = vec3(0);
            }
            illumination += attenuation * spotLightColor * (diffuse + specular);
        }
        else
        {
            illumination += attenuation * lightColors[i] * (diffuse + specular);
        }

    }

    // Clamp the final color to the range [0, 1]
    vec3 color = clamp(illumination, 0.0, 1.0);
    fragColor = vec4(color, 1.0);
}
