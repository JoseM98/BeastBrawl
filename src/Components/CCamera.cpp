#include "CCamera.h"

CCamera::CCamera(float _rotExtraY)
: rotExtraY(_rotExtraY), rotExtraYPrev(_rotExtraY), rotExtraYNext(_rotExtraY) 
{
    m_compType = CompType::CameraComp;
}