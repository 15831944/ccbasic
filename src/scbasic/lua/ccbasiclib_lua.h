﻿#ifndef _CCBASICLIB_LUA_H_
#define _CCBASICLIB_LUA_H_

extern "C" {
#include "lua.h"
}
#include "kaguya/kaguya.hpp"
#include <basic.h>

void ExportBasiclibClassToLua(lua_State* L);
template<class T>
T* GetBasicLibClass(lua_State* L, int nIndex) {
	return kaguya::get_pointer(L, nIndex, kaguya::types::typetag<T>());
}

#endif