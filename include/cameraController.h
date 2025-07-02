#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H

#include <vector>

class CameraController {
public:
    void addSegmentDuration(float duration);
    float getTotalTime() const;
    float getSegmentTime(int segmentIndex) const;
    int getCurrentSegment(float elapsedTime) const;
    float getLocalT(float elapsedTime) const;

private:
    std::vector<float> segmentDurations;
};

#endif