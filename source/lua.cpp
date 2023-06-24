#include <GarrysMod/FactoryLoader.hpp>
#include <GarrysMod/Lua/Interface.h>
#include "util.h"

static SourceSDK::FactoryLoader luashared_loader("lua_shared");

LUA_FUNCTION(Think)
{
	HTTPServer->Think();
}

void LUA_InitServer(GarrysMod::Lua::ILuaBase* LUA)
{
	Start_Table(LUA);
		Add_Func(LUA, Think, "Think");
	Finish_Table(LUA, "httpserver");

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->GetField(-1, "RunString");
		LUA->PushString(" \
			hook.Add('Think', 'HTTPServer', function() \
					httpserver.Think() \
			end)");
		LUA->Call(1, 0);
	LUA->Pop();


	LuaPrint("[HTTPNet] Successfully Loaded.");
}

GarrysMod::Lua::ILuaBase* GetRealm(int realm)
{
	CLuaShared* LuaShared = (CLuaShared*)luashared_loader.GetFactory()("LUASHARED003", nullptr);
	if (LuaShared == nullptr)
		Msg("I've failed\nYou\n");

	return LuaShared->GetLuaInterface(realm);
}