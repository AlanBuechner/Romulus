#include "pch.h"
#include "Time.h"
#include <chrono>

float Engine::Time::s_DeltaTime = 0.0f;
float Engine::Time::s_LastFrameTime = Engine::Time::GetTime();

namespace Engine
{
    using namespace std::chrono;

    double Time::GetTime()
    {
        return duration<double>(system_clock::now().time_since_epoch()).count();
    }

    void Time::UpdateDeltaTime()
    {
        float time = GetTime();
        s_DeltaTime = time - s_LastFrameTime;
        s_LastFrameTime = time;

    }

}