#pragma once
#include "Core.h"

namespace Engine
{
	class Window;
}

namespace Engine
{
	class Cursor
	{
	public:
		static void ConstrainToWindow(Ref<Window> window);
		static void Visibility(bool v);
	};
}