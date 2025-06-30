#ifndef BEZIER_PATH_H
#define BEZIER_PATH_H

#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

struct BezierSegment {
    glm::vec3 p0, p1, p2, p3;
};

class BezierPath {
private:
    std::vector<BezierSegment> segments;

public:
    BezierPath() {}

    void addSegment(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);

    glm::vec3 evaluate(float t);

    // Gera pontos para debug visual (ex: para desenhar linhas)
    std::vector<glm::vec3> getDebugPoints(int samplesPerSegment = 100);

    int getSegmentCount() const { return segments.size(); }
};

#endif
