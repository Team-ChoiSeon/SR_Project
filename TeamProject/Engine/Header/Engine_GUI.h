#pragma once

// 기존 new 매크로로 인해 충돌 방지
#pragma push_macro("new")
#undef new
#define NOMINMAX  
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

#pragma pop_macro("new")
