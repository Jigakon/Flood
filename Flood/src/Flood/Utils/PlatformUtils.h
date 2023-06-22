#ifndef PLATFORMUTILS__H
#define PLATFORMUTILS__H

#include <string>

namespace Flood
{
	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filters);
		static std::string SaveFile(const char* filters);
	};
}

#endif // !PLATFORMUTILS__H
