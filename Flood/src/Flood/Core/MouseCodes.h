#ifndef MOUSEBUTTONCODES__H
#define MOUSEBUTTONCODES__H

namespace Flood
{
	using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			// from GLFW
			Button0        = 0,
			Button1        = 1,
			Button2        = 2,
			Button3        = 3,
			Button4        = 4,
			Button5        = 5,
			Button6        = 6,
			Button7        = 7,
			ButtonLast     = Button7,
			ButtonLeft     = Button0,
			ButtonRight    = Button1,
			ButtonMiddle   = Button2
		};
	}
}

#endif // !MOUSEBUTTONCODES__H