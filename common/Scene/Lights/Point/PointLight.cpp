#include "common/Scene/Lights/Point/PointLight.h"

void PointLight::ComputeSampleRays(std::vector<Ray>& output, glm::vec3 origin, glm::vec3 normal) const
{
    origin += normal * LARGE_EPSILON;
    const glm::vec3 lightPosition = glm::vec3(GetPosition());
    const glm::vec3 rayDirection = glm::normalize(lightPosition - origin);
    const float distanceToOrigin = glm::distance(origin, lightPosition);
    output.emplace_back(origin, rayDirection, distanceToOrigin);
}

float PointLight::ComputeLightAttenuation(glm::vec3 origin) const
{
    return 1.f;
}

void PointLight::GenerateRandomPhotonRay(Ray& ray) const
{
    // Assignment 8 TODO: Fill in the random point light samples here.
    glm::vec3 position = glm::vec3(GetPosition());
    ray.SetRayPosition(position);
    glm::vec3 dir;
    do{
        dir.x = ((float) rand()) / (float) RAND_MAX * 2 - 1;
        dir.y = ((float) rand()) / (float) RAND_MAX * 2 - 1;
        dir.z = ((float) rand()) / (float) RAND_MAX * 2 - 1;
    } while(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z > 1.0);
    ray.SetRayDirection(glm::normalize(dir));
    return;
}
