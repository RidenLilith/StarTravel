#include "asteroidSpawner.h"
#include <random>

std::vector<Asteroid> generateAsteroidsAlongBezier(
    BezierPath& path,
    int asteroidsPerSegment,
    Texture* texture,
    Shader* shader,
    GLuint vaoID
) {
    std::vector<Asteroid> asteroids;

    std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<float> offsetDist(-5.0f, 5.0f);
    std::uniform_real_distribution<float> scaleDist(0.1f, 0.5f);

    int segmentCount = path.getSegmentCount();

    for (int i = 0; i < segmentCount; ++i) {
        for (int j = 0; j < asteroidsPerSegment; ++j) {
            float t = (i + static_cast<float>(j) / asteroidsPerSegment) / segmentCount;
            glm::vec3 basePos = path.evaluate(t);

            basePos.x += offsetDist(rng);
            basePos.y += offsetDist(rng);
            basePos.z += offsetDist(rng);

            // Corrigido: agora passando a posição como primeiro argumento
            Asteroid asteroid(basePos, 10, 10, texture, shader, vaoID);
            asteroid.Scale(glm::vec3(scaleDist(rng)));

            asteroids.push_back(asteroid);
        }
    }

    return asteroids;
}
