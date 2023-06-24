#include <GarrysMod/FactoryLoader.hpp>
#include <GarrysMod/Lua/Interface.h>
#include "util.h"

static SourceSDK::FactoryLoader luashared_loader("lua_shared");

LUA_FUNCTION(Think)
{
	HTTPServer->Think();

	return 0;
}

LUA_FUNCTION(Get)
{
	const char* path = LUA->CheckString(1);
	LUA->CheckType(2, Type::Function);
	GarrysMod::Lua::CFunc func = LUA->GetCFunction(2);

	HTTPServer->Get(path, func);

	return 0;
}

LUA_FUNCTION(Start)
{
	const char* address = LUA->CheckString(1);
	unsigned port = LUA->CheckNumber(2);

	HTTPServer->Start(address, port);

	return 0;
}

LUA_FUNCTION(Stop)
{
	HTTPServer->Stop();

	return 0;
}

void LUA_InitServer(GarrysMod::Lua::ILuaBase* LUA)
{
	Start_Table(LUA);
		Add_Func(LUA, Think, "Think");
		Add_Func(LUA, Start, "Start");
		Add_Func(LUA, Stop, "Stop");
		Add_Func(LUA, Get, "Get");
	Finish_Table(LUA, "httpserver");

	LUA->PushSpecial(SPECIAL_GLOB);
		LUA->GetField(-1, "RunString");
		LUA->PushString("\
hook.Add('Think', 'HTTPServer', function()\
	httpserver.Think()\
end)");
		LUA->Call(1, 0);
	LUA->Pop();


	LuaPrint("[HTTPServer] Successfully Loaded.");
}

GarrysMod::Lua::ILuaBase* GetRealm(int realm)
{
	CLuaShared* LuaShared = (CLuaShared*)luashared_loader.GetFactory()("LUASHARED003", nullptr);
	if (LuaShared == nullptr)
		Msg("I've failed\nYou\n");

	return LuaShared->GetLuaInterface(realm);
}