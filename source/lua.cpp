#include <GarrysMod/FactoryLoader.hpp>
#include <GarrysMod/Lua/Interface.h>
#include "util.h"

static SourceSDK::FactoryLoader luashared_loader("lua_shared");

GarrysMod::Lua::ILuaBase* GetRealm(int realm)
{
	CLuaShared* LuaShared = (CLuaShared*)luashared_loader.GetFactory()("LUASHARED003", nullptr);
	if (LuaShared == nullptr)
		Msg("I've failed\nYou\n");

	return LuaShared->GetLuaInterface(realm);
}

void LUA_InitServer(GarrysMod::Lua::ILuaBase* LUA)
{

	LuaPrint("[HTTPNet] Successfully Loaded.");
}