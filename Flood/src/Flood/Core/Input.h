#ifndef INPUT__H
#define INPUT__H

#include "Flood/Core/Core.h"

#include "Flood/Core/KeyCodes.h"
#include "Flood/Core/MouseCodes.h"

namespace Flood
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}

#endif // !INPUT__H 
