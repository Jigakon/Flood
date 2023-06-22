#ifndef TAGCOMPONENT__H
#define TAGCOMPONENT__H

namespace Flood
{
	struct TagComponent
	{
		std::string Name;

		TagComponent() = default;
		TagComponent(const char* name) { Name = name; };
	};
}

#endif // !TAGCOMPONENT__H