#ifndef PLAYERCONTROLLER__H
#define PLAYERCONTROLLER__H

#include "Flood/Core/Input.h"

namespace Flood
{
	struct PlayerControllerComponent
	{
		float speed = 10.f;

		KeyCode up = Key::W;
		KeyCode down = Key::S;
		KeyCode left = Key::A;
		KeyCode right = Key::D;

		ECS::Entity FollowingCamera = ECS::Null;
		bool isCameraFollowing = false;
	};
}

#endif //!PLAYERCONTROLLER__H
