#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <type_traits>
#include <string>
#include <sstream>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <fstream>

#include "Logger.h"

#include "debug/instrumentor.h"

#ifdef MONSI_PLATFORM_WINDOWS
	#include <Windows.h>
#endif