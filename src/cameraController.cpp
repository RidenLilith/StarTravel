#include "CameraController.h"

void CameraController::addSegmentDuration(float duration) {
    segmentDurations.push_back(duration);
}

float CameraController::getTotalTime() const {
    float total = 0.0f;
    for (float t : segmentDurations) {
        total += t;
    }
    return total;
}

float CameraController::getSegmentTime(int segmentIndex) const {
    if (segmentIndex < 0 || segmentIndex >= segmentDurations.size()) return 0.0f;
    return segmentDurations[segmentIndex];
}

int CameraController::getCurrentSegment(float elapsedTime) const {
    float time = 0.0f;
    for (int i = 0; i < segmentDurations.size(); ++i) {
        if (elapsedTime < time + segmentDurations[i])
            return i;
        time += segmentDurations[i];
    }
    return segmentDurations.size() - 1;  // Último
}

float CameraController::getLocalT(float elapsedTime) const {
    float time = 0.0f;
    for (int i = 0; i < segmentDurations.size(); ++i) {
        if (elapsedTime < time + segmentDurations[i]) {
            float localElapsed = elapsedTime - time;
            return localElapsed / segmentDurations[i];
        }
        time += segmentDurations[i];
    }
    return 1.0f;
}
