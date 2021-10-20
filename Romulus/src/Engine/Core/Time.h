#pragma once
#include "Engine/Core/Application.h"

namespace Engine
{
	class Time
	{
		friend Application;

	public:
		Time() {}

		static inline float GetDeltaTime() { return s_Instance->s_DeltaTime; }
		static inline float GetDeltaSeconds() { return s_Instance->s_DeltaTime; }
		static inline float GetDeltaMilliseconds() { return s_Instance->s_DeltaTime * 1000.0f; }

		static inline float GetFPS() { return 1.0f / s_DeltaTime; }

		static inline double GetTime();

	private:
		static void UpdateDeltaTime();

	protected:
		static float s_DeltaTime;
		static float s_LastFrameTime;

	private:
		static Time* s_Instance;
	};
}

typedef Engine::Time Time;