#ifndef BEZIER_H
#define BEZIER_H

#include <glm/glm.hpp>

// Função que retorna um ponto na curva de Bézier cúbica
glm::vec3 BezierCubic(const glm::vec3& p0, const glm::vec3& p1, 
                      const glm::vec3& p2, const glm::vec3& p3, float t);

#endif
