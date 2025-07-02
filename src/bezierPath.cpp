#include "BezierPath.h"
#include <cmath>

void BezierPath::addSegment(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
    segments.push_back({p0, p1, p2, p3});
}

glm::vec3 BezierPath::evaluate(int segmentIndex, float t) {
    if (segments.empty() || segmentIndex < 0 || segmentIndex >= segments.size())
        return glm::vec3(0.0f);

    const BezierSegment& seg = segments[segmentIndex];
    t = glm::clamp(t, 0.0f, 1.0f); // Adiciona clamping de segurança

    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    return seg.p0 * uuu +
           seg.p1 * (3.0f * uu * t) +
           seg.p2 * (3.0f * u * tt) +
           seg.p3 * ttt;
}




std::vector<glm::vec3> BezierPath::getDebugPoints(int samplesPerSegment) {
    std::vector<glm::vec3> points;

    for (int segmentIndex = 0; segmentIndex < getSegmentCount(); ++segmentIndex) {
        for (int i = 0; i <= samplesPerSegment; ++i) {
            float t = static_cast<float>(i) / samplesPerSegment;
            points.push_back(evaluate(segmentIndex, t));
        }
    }

    return points;
}
