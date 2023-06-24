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
	LUA->Push(2);
	int func = LUA->ReferenceCreate();

	HTTPServer->Get(path, func);

	return 0;
}

LUA_FUNCTION(Post)
{
	const char* path = LUA->CheckString(1);
	LUA->CheckType(2, Type::Function);
	LUA->Push(2);
	int func = LUA->ReferenceCreate();

	HTTPServer->Post(path, func);

	return 0;
}

LUA_FUNCTION(Put)
{
	const char* path = LUA->CheckString(1);
	LUA->CheckType(2, Type::Function);
	LUA->Push(2);
	int func = LUA->ReferenceCreate();

	HTTPServer->Put(path, func);

	return 0;
}

LUA_FUNCTION(Patch)
{
	const char* path = LUA->CheckString(1);
	LUA->CheckType(2, Type::Function);
	LUA->Push(2);
	int func = LUA->ReferenceCreate();

	HTTPServer->Patch(path, func);

	return 0;
}

LUA_FUNCTION(Delete)
{
	const char* path = LUA->CheckString(1);
	LUA->CheckType(2, Type::Function);
	LUA->Push(2);
	int func = LUA->ReferenceCreate();

	HTTPServer->Delete(path, func);

	return 0;
}

LUA_FUNCTION(Options)
{
	const char* path = LUA->CheckString(1);
	LUA->CheckType(2, Type::Function);
	LUA->Push(2);
	int func = LUA->ReferenceCreate();

	HTTPServer->Options(path, func);

	return 0;
}

LUA_FUNCTION(IsRunning)
{
	LUA->PushBool(HTTPServer->status == HTTPSERVER_ONLINE);

	return 1;
}

LUA_FUNCTION(SetTCPnodelay)
{
	LUA->CheckType(1, Type::BOOL);
	HTTPServer->server.set_tcp_nodelay(LUA->GetBool(1));
	return 0;
}

LUA_FUNCTION(SetReadTimeout)
{
	HTTPServer->server.set_read_timeout(LUA->CheckNumber(1), LUA->CheckNumber(2));
	return 0;
}

LUA_FUNCTION(SetWriteTimeout)
{
	HTTPServer->server.set_write_timeout(LUA->CheckNumber(1), LUA->CheckNumber(2));
	return 0;
}

LUA_FUNCTION(SetPayloadMaxLength)
{
	HTTPServer->server.set_payload_max_length(LUA->CheckNumber(1));
	return 0;
}

LUA_FUNCTION(SetKeepAliveTimeout)
{
	HTTPServer->server.set_keep_alive_timeout(LUA->CheckNumber(1));
	return 0;
}

LUA_FUNCTION(SetKeepAliveMaxCount)
{
	HTTPServer->server.set_keep_alive_max_count(LUA->CheckNumber(1));
	return 0;
}

LUA_FUNCTION(SetMountPoint)
{
	HTTPServer->server.set_mount_point(LUA->CheckString(1), LUA->CheckString(2));
	return 0;
}

LUA_FUNCTION(RemoveMountPoint)
{
	HTTPServer->server.remove_mount_point(LUA->CheckString(1));
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

		Add_Func(LUA, IsRunning, "IsRunning");
		Add_Func(LUA, SetTCPnodelay, "SetTCPnodelay");
		Add_Func(LUA, SetReadTimeout, "SetReadTimeout");
		Add_Func(LUA, SetWriteTimeout, "SetWriteTimeout");
		Add_Func(LUA, SetPayloadMaxLength, "SetPayloadMaxLength");
		Add_Func(LUA, SetKeepAliveTimeout, "SetKeepAliveTimeout");
		Add_Func(LUA, SetKeepAliveMaxCount, "SetKeepAliveMaxCount");

		Add_Func(LUA, SetMountPoint, "SetMountPoint");
		Add_Func(LUA, RemoveMountPoint, "RemoveMountPoint");

		Add_Func(LUA, Get, "Get");
		Add_Func(LUA, Post, "Post");
		Add_Func(LUA, Put, "Put");
		Add_Func(LUA, Patch, "Patch");
		Add_Func(LUA, Delete, "Delete");
		Add_Func(LUA, Options, "Options");
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