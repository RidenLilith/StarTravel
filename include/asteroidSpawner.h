#ifndef ASTEROID_SPAWNER_H
#define ASTEROID_SPAWNER_H

#include <vector>
#include "asteroid.h"
#include "BezierPath.h"
#include "Texture.h"
#include "ShaderClass.h"

std::vector<Asteroid> generateAsteroidsAlongBezier(
    BezierPath& path,
    int asteroidsPerSegment,
    Texture* texture,
    Shader* shader,
    GLuint vaoID
);

#endif
