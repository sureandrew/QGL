#pragma once
#ifndef _CALL_LUA_FUNC_H_
#define _CALL_LUA_FUNC_H_

extern "C"
{
#include "lua.h"
}

struct LuaVar
{
	std::string name;
	LuaVar(const char *s) : name(s) {}
};

INLINE void LuaPush(lua_State *L)
{
	lua_pushnil(L);
}

INLINE void LuaPush(lua_State *L, char* param)
{
	lua_pushstring(L, param);
}

INLINE void LuaPush(lua_State *L, const char* param)
{
	lua_pushstring(L, param);
}

INLINE void LuaPush(lua_State *L, const std::string &param)
{
	lua_pushlstring(L, param.c_str(), param.size());
}

INLINE void LuaPush(lua_State *L, unsigned int param)
{
	lua_pushnumber(L, (int)(param));
}

INLINE void LuaPush(lua_State *L, int param)
{
	lua_pushnumber(L, param);
}

INLINE void LuaPush(lua_State *L, DWORD param)
{
	lua_pushnumber(L, param);
}

INLINE void LuaPush(lua_State *L, long param)
{
	lua_pushnumber(L, param);
}

INLINE void LuaPush(lua_State *L, float param)
{
	lua_pushnumber(L, param);
}

INLINE void LuaPush(lua_State *L, double param)
{
	lua_pushnumber(L, param);
}

INLINE void LuaPush(lua_State *L, bool param)
{
	lua_pushboolean(L, param);
}

INLINE void LuaPush(lua_State *L, lua_CFunction  param)
{
	lua_pushcfunction(L, param);
}

INLINE void LuaPush(lua_State *L, LuaVar& param)
{
	lua_getglobal(L, param.name.c_str());
}

INLINE int GetLuaBoolean(lua_State *L)
{
	return lua_toboolean(L, 1);
};

INLINE lua_Integer GetLuaInteger(lua_State *L)
{
	return lua_tointeger(L, 1);
};

INLINE lua_Number GetLuaNumber(lua_State *L)
{
	return lua_tonumber(L, 1);
};

INLINE const char* GetLuaString(lua_State *L)
{
	return lua_tostring(L, 1);
};

INLINE int LuaOK(lua_State *L)
{
	return lua_status(L);
}

#define PROTO_LuaPush(_type_)								\
	void LuaPush(lua_State *L, _type_ *ptr);

INLINE int CallLuaFunc_HEAD(lua_State *luaState, PCSTRINGA func, String& result)
{
	if (luaState == NULL)
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("func %s, state %p, status %d, top %d"), 
			String(func).c_str(), luaState, lua_status(luaState), lua_gettop(luaState));
		result = _T("invalid lua state");
		return -1;
	}

#ifdef LUA_STACK_TRACE_SERVER
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("function %s, state %p, status %d, top %d"), 
		String(func).c_str(), luaState, lua_status(luaState), lua_gettop(luaState));
#endif	// LUA_STACK_TRACE_SERVER
	lua_getglobal(luaState, func);

	return 0;
}

INLINE int HelpPCall(lua_State *L, int nargs, int nresults, int errfunc)
{
	__try
	{
		return lua_pcall(L, nargs, nresults, errfunc);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return -5;
	}
}

INLINE int CallLuaFunc_TAIL(lua_State *luaState, PCSTRINGA func, String& result, int argc)
{
#ifdef LUA_STACK_TRACE_SERVER
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("function %s, state %p, status %d, top %d"), 
		String(func).c_str(), luaState, lua_status(luaState), lua_gettop(luaState));
#endif	// LUA_STACK_TRACE_SERVER
	int err = HelpPCall(luaState, argc, 0, 0);
	if (err == -5)
	{
		err = 0;
		TRACE_ERRORDTL_4(GLOBAL_LOGGER, _FE("lua_pcall exception, func %s, state %p, status %d, top %d"), 
			String(func).c_str(), luaState, lua_status(luaState), lua_gettop(luaState));
	}
#ifdef LUA_STACK_TRACE_SERVER
	TRACE_INFODTL_4(GLOBAL_LOGGER, _F("function after pcall %s, state %p, status %d, top %d"), 
		String(func).c_str(), luaState, lua_status(luaState), lua_gettop(luaState));
	if (err ==0 && lua_gettop(luaState) > 0)
	{
		TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("function after pcall not zero %s, state %p, status %d, top %d"), 
			String(func).c_str(), luaState, lua_status(luaState), lua_gettop(luaState));
	}
#endif //LUA_STACK_TRACE_SERVER
	if (err != 0 && err != LUA_YIELD)
	{
		result = lua_tostring(luaState, -1);
		TRACE_WARNDTL_6(GLOBAL_LOGGER, _FW("func %s, err %d, result %s, state %p, status %d, top %d"), 
			String(func).c_str(), err, result.c_str(), luaState, lua_status(luaState), lua_gettop(luaState));									
		lua_pop(luaState, 1);
		if (lua_gettop(luaState) != 0)
		{
			TRACE_WARNDTL_4(GLOBAL_LOGGER, _FW("top not zero, function %s, state %p, status %d, top %d"), 
				String(func).c_str(), luaState, lua_status(luaState), lua_gettop(luaState));
		}
	}
	return err;
}

INLINE int CallLuaFunc(lua_State *luaState, PCSTRINGA func, String& result)
{
	int err = CallLuaFunc_HEAD(luaState, func, result);
	if (err == 0)
		err = CallLuaFunc_TAIL(luaState, func, result, 0);
	return err;
}

template <typename T1>
int CallLuaFunc(lua_State *luaState, PCSTRINGA func, String& result, T1 P1)
{
	int err = CallLuaFunc_HEAD(luaState, func, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		err = CallLuaFunc_TAIL(luaState, func, result, 1);
	}
	return err;
}

template <typename T1, typename T2>
int CallLuaFunc(lua_State *luaState, PCSTRINGA func, String& result, T1 P1, T2 P2)
{
	int err = CallLuaFunc_HEAD(luaState, func, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		err = CallLuaFunc_TAIL(luaState, func, result, 2);
	}
	return err;
}


template <typename T1, typename T2, typename T3>
int CallLuaFunc(lua_State *luaState, PCSTRINGA func, String& result, T1 P1, T2 P2, T3 P3)
{
	int err = CallLuaFunc_HEAD(luaState, func, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		err = CallLuaFunc_TAIL(luaState, func, result, 3);
	}
	return err;
}

template <typename T1, typename T2, typename T3, typename T4>
int CallLuaFunc(lua_State *luaState, PCSTRINGA func, String& result, T1 P1, T2 P2, T3 P3, T4 P4)
{
	int err = CallLuaFunc_HEAD(luaState, func, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		LuaPush(luaState, P4);
		err = CallLuaFunc_TAIL(luaState, func, result, 4);
	}
	return err;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
int CallLuaFunc(lua_State *luaState, PCSTRINGA func, String& result, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5)
{
	int err = CallLuaFunc_HEAD(luaState, func, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		LuaPush(luaState, P4);
		LuaPush(luaState, P5);
		err = CallLuaFunc_TAIL(luaState, func, result, 5);
	}
	return err;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
int CallLuaFunc(lua_State *luaState, PCSTRINGA func, String& result, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6)
{
	int err = CallLuaFunc_HEAD(luaState, func, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		LuaPush(luaState, P4);
		LuaPush(luaState, P5);
		LuaPush(luaState, P6);
		err = CallLuaFunc_TAIL(luaState, func, result, 6);
	}
	return err;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
int CallLuaFunc(lua_State *luaState, PCSTRINGA func, String& result, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7)
{
	int err = CallLuaFunc_HEAD(luaState, func, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		LuaPush(luaState, P4);
		LuaPush(luaState, P5);
		LuaPush(luaState, P6);
		LuaPush(luaState, P7);
		err = CallLuaFunc_TAIL(luaState, func, result, 7);
	}
	return err;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
int CallLuaFunc(lua_State *luaState, PCSTRINGA func, String& result, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8)
{
	int err = CallLuaFunc_HEAD(luaState, func, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		LuaPush(luaState, P4);
		LuaPush(luaState, P5);
		LuaPush(luaState, P6);
		LuaPush(luaState, P7);
		LuaPush(luaState, P8);
		err = CallLuaFunc_TAIL(luaState, func, result, 8)
	}
	return err;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
int CallLuaFunc(lua_State *luaState, PCSTRINGA func, String& result, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9)
{
	int err = CallLuaFunc_HEAD(luaState, func, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		LuaPush(luaState, P4);
		LuaPush(luaState, P5);
		LuaPush(luaState, P6);
		LuaPush(luaState, P7);
		LuaPush(luaState, P8);
		LuaPush(luaState, P9);
		err = CallLuaFunc_TAIL(luaState, func, result, 9);
	}
	return err;
}

INLINE bool HasLuaFunc(lua_State* state, const char* func)
{
	if (state == NULL)
		return false;

	lua_getglobal(state, func);
	bool bFunc = lua_isfunction(state, -1);
	lua_pop(state, 1); 

	return bFunc;

}

INLINE int CallLuaMethod_HEAD(lua_State *luaState, PCSTRINGA object, PCSTRINGA method, String & result)											
{
	if (luaState == NULL)
	{
		TRACE_WARNDTL_5(GLOBAL_LOGGER, _FW("object %s, method %s, state %p, status %d, top %d"), 
			String(object).c_str(), String(method).c_str(), luaState, lua_status(luaState), lua_gettop(luaState));
		result = _T("invalid lua state");
		return -1;
	}

#ifdef LUA_STACK_TRACE_SERVER
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("object %s, method %s, state %p, status %d, top %d"), 
		String(object).c_str(), String(method).c_str(), luaState, lua_status(luaState), lua_gettop(luaState));
#endif // LUA_STACK_TRACE_SERVER															

	lua_getglobal(luaState, object);
	lua_getfield(luaState, -1, method);
	lua_insert(luaState, -2);

	return 0;
}

INLINE int  CallLuaMethod_TAIL(lua_State *luaState, PCSTRINGA object, PCSTRINGA method, String & result, int argc)										
{
#ifdef LUA_STACK_TRACE_SERVER
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("object %s, method %s, state %p, status %d, top %d"), 
		String(object).c_str(), String(method).c_str(), luaState, lua_status(luaState), lua_gettop(luaState));									
#endif //LUA_STACK_TRACE_SERVER
	int err = lua_resume(luaState, argc+1);									
#ifdef LUA_STACK_TRACE_SERVER
	TRACE_INFODTL_5(GLOBAL_LOGGER, _F("object %s, method %s, state %p, status %d, top %d"), 
		String(object).c_str(), String(method).c_str(), luaState, lua_status(luaState), lua_gettop(luaState));									
	if (err == 0 && lua_gettop(luaState) > 0)
	{
		TRACE_WARNDTL_5(GLOBAL_LOGGER, _FW("after resume not zero, object %s, method %s, state %p, status %d, top %d"), 
			String(object).c_str(), String(method).c_str(), luaState, lua_status(luaState), lua_gettop(luaState));									
	}
#endif //LUA_STACK_TRACE_SERVER
	if (err != 0 && err != LUA_YIELD)
	{
		result = lua_tostring(luaState, -1);
		TRACE_WARNDTL_7(GLOBAL_LOGGER, _FW("object %s, method %s, err %d, result %s, state %p, status %d, top %d"), 
			String(object).c_str(), String(method).c_str(), err, result.c_str(), luaState, lua_status(luaState), lua_gettop(luaState));		 
		lua_pop(luaState, 1);
	}
	return err;
}

INLINE int CallLuaMethod(lua_State *luaState, PCSTRINGA object, PCSTRINGA method, String & result)
{
	int err = CallLuaMethod_HEAD(luaState, object, method, result);
	if (err == 0)
		err = CallLuaMethod_TAIL(luaState, object, method, result, 0);
	return err;
}

template <typename T1>
int CallLuaMethod(lua_State *luaState, PCSTRINGA object, PCSTRINGA method, String & result, T1 P1)
{
	int err = CallLuaMethod_HEAD(luaState, object, method, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		err = CallLuaMethod_TAIL(luaState, object, method, result, 1);
	}
	return err;
}

template <typename T1, typename T2>
int CallLuaMethod(lua_State *luaState, PCSTRINGA object, PCSTRINGA method, String & result, T1 P1, T2 P2)
{
	int err = CallLuaMethod_HEAD(luaState, object, method, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		err = CallLuaMethod_TAIL(luaState, object, method, result, 2);
	}
	return err;
}


template <typename T1, typename T2, typename T3>
int CallLuaMethod(lua_State *luaState, PCSTRINGA object, PCSTRINGA method, String & result, T1 P1, T2 P2, T3 P3)
{
	int err = CallLuaMethod_HEAD(luaState, object, method, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		err = CallLuaMethod_TAIL(luaState, object, method, result, 3);
	}
	return err;
}

template <typename T1, typename T2, typename T3, typename T4>
int CallLuaMethod(lua_State *luaState, PCSTRINGA object, PCSTRINGA method, String & result, T1 P1, T2 P2, T3 P3, T4 P4)
{
	int err = CallLuaMethod_HEAD(luaState, object, method, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		LuaPush(luaState, P4);
		err = CallLuaMethod_TAIL(luaState, object, method, result, 4);
	}
	return err;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
int CallLuaMethod(lua_State *luaState, PCSTRINGA object, PCSTRINGA method, String & result, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5)
{
	int err = CallLuaMethod_HEAD(luaState, object, method, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		LuaPush(luaState, P4);
		LuaPush(luaState, P5);
		err = CallLuaMethod_TAIL(luaState, object, method, result, 5);
	}
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
int CallLuaMethod(lua_State *luaState, PCSTRINGA object, PCSTRINGA method, String & result, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6)
{
	int err = CallLuaMethod_HEAD(luaState, object, method, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		LuaPush(luaState, P4);
		LuaPush(luaState, P5);
		LuaPush(luaState, P6);
		err = CallLuaMethod_TAIL(luaState, object, method, result, 6);
	}
	return err;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
int CallLuaMethod(lua_State *luaState, PCSTRINGA object, PCSTRINGA method, String & result, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7)
{
	int err = CallLuaMethod_HEAD(luaState, object, method, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		LuaPush(luaState, P4);
		LuaPush(luaState, P5);
		LuaPush(luaState, P6);
		LuaPush(luaState, P7);
		err = CallLuaMethod_TAIL(luaState, object, method, result, 7);
	}
	return err;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
int CallLuaMethod(lua_State *luaState, PCSTRINGA object, PCSTRINGA method, String & result, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8)
{
	int err = 
		(luaState, object, method, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		LuaPush(luaState, P4);
		LuaPush(luaState, P5);
		LuaPush(luaState, P6);
		LuaPush(luaState, P7);
		LuaPush(luaState, P8);
		err = CallLuaMethod_TAIL(luaState, object, method, 8);
	}
	return err;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
int CallLuaMethod(lua_State *luaState, PCSTRINGA object, PCSTRINGA method, String & result, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9)
{
	int err = CallLuaMethod_HEAD(luaState, object, method, result);
	if (err == 0)
	{
		LuaPush(luaState, P1);
		LuaPush(luaState, P2);
		LuaPush(luaState, P3);
		LuaPush(luaState, P4);
		LuaPush(luaState, P5);
		LuaPush(luaState, P6);
		LuaPush(luaState, P7);
		LuaPush(luaState, P8);
		LuaPush(luaState, P9);
		err = CallLuaMethod_TAIL(luaState, object, method, 9);
	}
	return err;
}

INLINE bool HasLuaMethod(lua_State* state, PCSTRINGA object, PCSTRINGA method)
{
	if (state == NULL)
		return false;

	bool bMethod;
	lua_getglobal(state, object);
	bMethod = lua_istable(state, -1);
	if (bMethod) {
		lua_getfield(state, -1, method);
		bMethod = lua_isfunction(state, -1);
		lua_pop(state, 2); 
	} else {
		lua_pop(state, 1);
	}

	return bMethod;

}

#endif // _CALL_LUA_FUNC_H_