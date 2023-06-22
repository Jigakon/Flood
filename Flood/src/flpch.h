#ifndef FLPCH__H
#define FLPCH__H

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "Flood/Core/Log.h"

#include "Flood/Debug/Instrumentor.h"

#ifdef FLOOD_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

#endif //!FLPCH__H