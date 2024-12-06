#version 330 core

in vec3 rayDirection;
out vec4 FragColor;

// Uniforms for the camera position and sphere
uniform vec3 cameraPos;   // Camera position in world space
uniform vec3 spherePos;   // Sphere position in world space
uniform float sphereRadius; // Sphere radius

// Function to compute ray-sphere intersection
float intersectSphere(vec3 rayOrigin, vec3 rayDir, vec3 sphereCenter, float radius)
{
    vec3 oc = rayOrigin - sphereCenter;
    float a = dot(rayDir, rayDir);
    float b = 2.0 * dot(oc, rayDir);
    float c = dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0.0) return -1.0; // No intersection
    else return (-b - sqrt(discriminant)) / (2.0 * a); // Nearest intersection
}

void main()
{
    // Ray origin and direction
    vec3 rayOrigin = cameraPos;
    vec3 rayDir = normalize(rayDirection);

    // Compute intersection with the sphere
    float t = intersectSphere(rayOrigin, rayDir, spherePos, sphereRadius);

    if (t > 0.0) {
        // Compute intersection point and normal
        vec3 intersection = rayOrigin + t * rayDir;
        vec3 normal = normalize(intersection - spherePos);

        // Simple lighting (Lambertian reflection)
        vec3 lightDir = normalize(vec3(1.0, 1.0, -1.0));
        float diff = max(dot(normal, lightDir), 0.0);

        FragColor = vec4(diff, diff, diff, 1.0); // Grayscale based on lighting
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0); // Background color
    }

}
