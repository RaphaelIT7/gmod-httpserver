#include "util.h"

static int HttpServer_TypeID = -1;
PushReferenced_LuaClass(HttpServer, HttpServer_TypeID)
Get_LuaClass(HttpServer, HttpServer_TypeID, "HttpServer")

LUA_FUNCTION_STATIC(HttpServer_Think)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	pServer->Think();

	return 0;
}

inline int CheckFunction(GarrysMod::Lua::ILuaInterface* LUA, int iStackPos)
{
	LUA->CheckType(iStackPos, GarrysMod::Lua::Type::Function);
	LUA->Push(iStackPos);
	return LUA->ReferenceCreate();
}

inline bool CheckBool(GarrysMod::Lua::ILuaInterface* LUA, int iStackPos)
{
	LUA->CheckType(iStackPos, GarrysMod::Lua::Type::Bool);
	return LUA->GetBool(iStackPos);
}

LUA_FUNCTION_STATIC(HttpServer__tostring)
{
	HttpServer* pServer = Get_HttpServer(1, false);
	if (!pServer)
	{
		LUA->PushString("HttpServer [NULL]");
		return 1;
	}

	char szBuf[64] = {};
	V_snprintf(szBuf, sizeof(szBuf),"HttpServer [%s]", (pServer->GetAddress() + std::to_string(pServer->GetPort())).c_str()); 
	LUA->PushString(szBuf);
	return 1;
}

LUA_FUNCTION_STATIC(HttpServer__index)
{
	if (LUA->FindOnObjectsMetaTable(1, 2))
		return 1;

	LUA->Pop(1);
	LUA->ReferencePush(g_pPushedHttpServer[Get_HttpServer(1, true)]->iTableReference);
	if (!LUA->FindObjectOnTable(-1, 2))
		LUA->PushNil();

	LUA->Remove(-2);

	return 1;
}

LUA_FUNCTION_STATIC(HttpServer__newindex)
{
	LUA->ReferencePush(g_pPushedHttpServer[Get_HttpServer(1, true)]->iTableReference);
	LUA->Push(2);
	LUA->Push(3);
	LUA->RawSet(-3);
	LUA->Pop(1);

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_GetTable)
{
	LUA->ReferencePush(g_pPushedHttpServer[Get_HttpServer(1, true)]->iTableReference);

	return 1;
}

LUA_FUNCTION_STATIC(HttpServer_Get)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	const char* path = LUA->CheckString(2);
	int func = CheckFunction(LUA, 3);
	bool ipWhitelist = LUA->GetBool(4);

	pServer->Get(path, func, ipWhitelist);

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_Post)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	const char* path = LUA->CheckString(2);
	int func = CheckFunction(LUA, 3);
	bool ipWhitelist = LUA->GetBool(4);

	pServer->Post(path, func, ipWhitelist);

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_Put)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	const char* path = LUA->CheckString(2);
	int func = CheckFunction(LUA, 3);
	bool ipWhitelist = LUA->GetBool(4);

	pServer->Put(path, func, ipWhitelist);

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_Patch)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	const char* path = LUA->CheckString(2);
	int func = CheckFunction(LUA, 3);
	bool ipWhitelist = LUA->GetBool(4);

	pServer->Patch(path, func, ipWhitelist);

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_Delete)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	const char* path = LUA->CheckString(2);
	int func = CheckFunction(LUA, 3);
	bool ipWhitelist = LUA->GetBool(4);

	pServer->Delete(path, func, ipWhitelist);

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_Options)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	const char* path = LUA->CheckString(2);
	int func = CheckFunction(LUA, 3);
	bool ipWhitelist = LUA->GetBool(4);

	pServer->Options(path, func, ipWhitelist);

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_IsRunning)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	LUA->PushBool(pServer->GetStatus() == HTTPSERVER_ONLINE);

	return 1;
}

LUA_FUNCTION_STATIC(HttpServer_SetTCPnodelay)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	pServer->GetServer().set_tcp_nodelay(CheckBool(LUA, 2));

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_SetReadTimeout)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	pServer->GetServer().set_read_timeout((time_t)LUA->CheckNumber(2), (time_t)LUA->CheckNumber(3));
	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_SetWriteTimeout)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	pServer->GetServer().set_write_timeout((time_t)LUA->CheckNumber(1), (time_t)LUA->CheckNumber(2));
	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_SetPayloadMaxLength)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	pServer->GetServer().set_payload_max_length((size_t)LUA->CheckNumber(2));
	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_SetKeepAliveTimeout)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	pServer->GetServer().set_keep_alive_timeout((time_t)LUA->CheckNumber(2));

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_SetKeepAliveMaxCount)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	pServer->GetServer().set_keep_alive_max_count((size_t)LUA->CheckNumber(2));

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_SetMountPoint)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	pServer->GetServer().set_mount_point(LUA->CheckString(2), LUA->CheckString(3));

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_RemoveMountPoint)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	pServer->GetServer().remove_mount_point(LUA->CheckString(2));

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_Start)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	const char* address = LUA->CheckString(2);
	unsigned short port = (unsigned short)LUA->CheckNumber(3);

	pServer->Start(address, port);

	return 0;
}

LUA_FUNCTION_STATIC(HttpServer_Stop)
{
	HttpServer* pServer = Get_HttpServer(1, true);
	pServer->Stop();

	return 0;
}

LUA_FUNCTION_STATIC(httpserver_Create)
{
	Push_HttpServer(new HttpServer);
	return 1;
}

LUA_FUNCTION_STATIC(httpserver_Destroy)
{
	HttpServer* pServer = Get_HttpServer(1, true);

	if (pServer->GetStatus() == HTTPSERVER_ONLINE)
		pServer->Stop();

	Delete_HttpServer(pServer);
	delete pServer;

	return 0;
}

void LUA_InitServer(GarrysMod::Lua::ILuaBase* LUA)
{
	HttpRequest_LuaInit();

	HttpServer_TypeID = g_Lua->CreateMetaTable("HttpServer");
		Util::AddFunc(HttpServer__tostring, "__tostring");
		Util::AddFunc(HttpServer__index, "__index");
		Util::AddFunc(HttpServer__newindex, "__newindex");
		Util::AddFunc(HttpServer_GetTable, "GetTable");

		Util::AddFunc(HttpServer_Think, "Think");
		Util::AddFunc(HttpServer_Start, "Start");
		Util::AddFunc(HttpServer_Stop, "Stop");

		Util::AddFunc(HttpServer_IsRunning, "IsRunning");
		Util::AddFunc(HttpServer_SetTCPnodelay, "SetTCPnodelay");
		Util::AddFunc(HttpServer_SetReadTimeout, "SetReadTimeout");
		Util::AddFunc(HttpServer_SetWriteTimeout, "SetWriteTimeout");
		Util::AddFunc(HttpServer_SetPayloadMaxLength, "SetPayloadMaxLength");
		Util::AddFunc(HttpServer_SetKeepAliveTimeout, "SetKeepAliveTimeout");
		Util::AddFunc(HttpServer_SetKeepAliveMaxCount, "SetKeepAliveMaxCount");

		Util::AddFunc(HttpServer_SetMountPoint, "SetMountPoint");
		Util::AddFunc(HttpServer_RemoveMountPoint, "RemoveMountPoint");

		Util::AddFunc(HttpServer_Get, "Get");
		Util::AddFunc(HttpServer_Post, "Post");
		Util::AddFunc(HttpServer_Put, "Put");
		Util::AddFunc(HttpServer_Patch, "Patch");
		Util::AddFunc(HttpServer_Delete, "Delete");
		Util::AddFunc(HttpServer_Options, "Options");
	g_Lua->Pop(1);

	Util::StartTable();
		Util::AddFunc(httpserver_Create, "Create");
		Util::AddFunc(httpserver_Destroy, "Destroy");
	Util::FinishTable("httpserver");

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "RunString");
		LUA->PushString("\
hook.Add('Think', 'HTTPServer', function()\
	httpserver.Think()\
end)");
		LUA->Call(1, 0);
	LUA->Pop();


	Msg("[HTTPServer] Successfully Loaded.\n");
}