#pragma warning (disable: 4005)
#pragma warning (disable: 26812)
#pragma warning (disable: 4244)

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <deque>
#include <array>
#include <vector>
#include <time.h>

#include "globals.h"
#include "vars/vars.h"


#include "minhook/minhook.h"
#pragma comment (lib, "minhook/minhook.lib")

#include <d3d9.h>
#pragma comment(lib, "d3d9")


#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"


#include "hook/hook.h"
#include "hack/hack.h"
#include "menu/menu.h"
#include "menu/fonts.h"
#include "memory/memory.h"
#include "sdk/sdk.hpp"
#include "math/math.hpp"

#include "math/qangle/qangle.h"
#include "math/vector/vector.h"
#include "math/martix4x4/matrix4x4.h"
#include "draw-list/draw-list.h"