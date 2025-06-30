#include "BezierPath.h"
#include <cmath>

void BezierPath::addSegment(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3) {
    segments.push_back({p0, p1, p2, p3});
}

glm::vec3 BezierPath::evaluate(float t) {
    if (segments.empty()) return glm::vec3(0.0f);

    int numSegments = segments.size();
    float totalT = t * numSegments;
    int segmentIndex = static_cast<int>(floor(totalT));
    if (segmentIndex >= numSegments) segmentIndex = numSegments - 1;
    float localT = totalT - segmentIndex;

    const BezierSegment& seg = segments[segmentIndex];

    float u = 1.0f - localT;
    float tt = localT * localT;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * localT;

    float c0 = uuu;
    float c1 = 3.0f * uu * localT;
    float c2 = 3.0f * u * tt;
    float c3 = ttt;

    glm::vec3 point = 
        seg.p0 * c0 +
        seg.p1 * c1 +
        seg.p2 * c2 +
        seg.p3 * c3;

    return point;
}


std::vector<glm::vec3> BezierPath::getDebugPoints(int samplesPerSegment) {
    std::vector<glm::vec3> points;
    int totalSamples = samplesPerSegment * getSegmentCount();
    for (int i = 0; i <= totalSamples; ++i) {
        float t = static_cast<float>(i) / totalSamples;
        points.push_back(evaluate(t));
    }
    return points;
}
