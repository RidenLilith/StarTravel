#include "bezier.h"

glm::vec3 BezierCubic(const glm::vec3& p0, const glm::vec3& p1, 
                      const glm::vec3& p2, const glm::vec3& p3, float t)
{
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    float c0 = uuu;
    float c1 = 3.0f * uu * t;
    float c2 = 3.0f * u * tt;
    float c3 = ttt;

    glm::vec3 term0 = p0 * c0;
    glm::vec3 term1 = p1 * c1;
    glm::vec3 term2 = p2 * c2;
    glm::vec3 term3 = p3 * c3;

    glm::vec3 point = term0 + term1 + term2 + term3;

    return point;
}

