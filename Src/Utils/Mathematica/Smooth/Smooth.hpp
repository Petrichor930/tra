#pragma once

#include <cmath>

#define T_ACC_CNT 100

inline float sCurve(float v_max, float cnt)
{
    float cntcnt;
    if (cnt < 0.0f) {
        cnt = -cnt;
        v_max = -v_max;
    }
    cntcnt = cnt / ((float)T_ACC_CNT);
    if (cnt < T_ACC_CNT / 2.0f) {
        return 2.0f * v_max * (cntcnt * cntcnt);
    } else if (cnt < T_ACC_CNT) {
        cntcnt = cntcnt - 1.0f;
        return v_max * (1.0f - 2.0f * (cntcnt * cntcnt));
    } else
        return v_max;
}

class LinearInterpolator {
public:
    LinearInterpolator(float _dt, float _initPos, float _maxAccel = 0.05f)
            : dt(_dt)
            , currentPos(_initPos)
            , targetPos(_initPos)
            , maxAccel(_maxAccel)
    {
    }

    void setTarget(float newTarget) { targetPos = newTarget; }

    inline float update()
    {
        float remainingDist = targetPos - currentPos;
        float maxStep = maxAccel * dt;

        if (fabs(remainingDist) <= maxStep) {
            currentPos = targetPos;
        } else {
            currentPos += (remainingDist > 0 ? maxStep : -maxStep);
        }
        return currentPos;
    }

    void syncPosition(float externalPos)
    {
        targetPos = externalPos;
        currentPos = externalPos;
    }

private:
    float dt;
    float currentPos;
    float targetPos;
    float maxAccel;
};