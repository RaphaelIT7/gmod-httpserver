#pragma once

#include "httpserver.h"
#include "LuaInterface.h"
#include <string>
#include "lua.h"
#include <vector>

extern GarrysMod::Lua::ILuaInterface* g_Lua;

class IServer;
class CBaseClient;

namespace Util
{
	extern IServer* server;

	inline void StartTable() {
		g_Lua->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		g_Lua->CreateTable();
	}

	inline void AddFunc(GarrysMod::Lua::CFunc Func, const char* Name) {
		g_Lua->PushString(Name);
		g_Lua->PushCFunction(Func);
		g_Lua->RawSet(-3);
	}

	inline void AddValue(double value, const char* Name) {
		g_Lua->PushString(Name);
		g_Lua->PushNumber(value);
		g_Lua->RawSet(-3);
	}

	inline void FinishTable(const char* Name) {
		g_Lua->SetField(-2, Name);
		g_Lua->Pop();
	}

	inline void NukeTable(const char* pName)
	{
		g_Lua->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		g_Lua->PushNil();
		g_Lua->SetField(-2, pName);
		g_Lua->Pop(1);
	}

	inline bool PushTable(const char* pName)
	{
		g_Lua->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		g_Lua->GetField(-1, pName);
		g_Lua->Remove(-2);
		if (g_Lua->IsType(-1, GarrysMod::Lua::Type::Table))
			return true;

		g_Lua->Pop(1);
		return false;
	}

	inline void PopTable()
	{
		g_Lua->Pop(1);
	}

	inline void RemoveField(const char* pName)
	{
		g_Lua->PushNil();
		g_Lua->SetField(-2, pName);
	}

	inline bool HasField(const char* pName, int iType)
	{
		g_Lua->GetField(-1, pName);
		return g_Lua->IsType(-1, iType);
	}

	extern std::vector<CBaseClient*> GetClients();
}

#define MakeString( str1, str2, str3 ) ((std::string)str1).append(str2).append(str3)
#define Get_LuaClass( className, luaType, strName ) \
static std::string invalidType_##className = MakeString("Tried to use something that wasn't a ", strName, "!"); \
static std::string triedNull_##className = MakeString("Tried to use a NULL ", strName, "!"); \
className* Get_##className(int iStackPos, bool bError) \
{ \
	if (!g_Lua->IsType(iStackPos, luaType)) \
	{ \
		if (bError) \
			g_Lua->ThrowError(invalidType_##className.c_str()); \
\
		return NULL; \
	} \
\
	className* pVar = g_Lua->GetUserType<className>(iStackPos, luaType); \
	if (!pVar && bError) \
		g_Lua->ThrowError(triedNull_##className.c_str()); \
\
	return pVar; \
}

struct LuaUserData { // ToDo: Maybe implement this also for other things?
	~LuaUserData() {
		if (iReference != -1)
		{
			g_Lua->ReferencePush(iReference);
			g_Lua->SetUserType(-1, NULL);
			g_Lua->Pop(1);
			g_Lua->ReferenceFree(iReference);
			iReference = -1;
		}

		if (iTableReference != -1)
		{
			g_Lua->ReferenceFree(iTableReference);
			iTableReference = -1;
		}

		pAdditionalData = NULL;
	}

	int iReference = -1;
	int iTableReference = -1;
	int pAdditionalData = NULL; // Used by HLTVClient.
};

// This one is special
#define PushReferenced_LuaClass( className, luaType ) \
static std::unordered_map<className*, LuaUserData*> g_pPushed##className; \
void Push_##className(className* var) \
{ \
	if (!var) \
	{ \
		g_Lua->PushNil(); \
		return; \
	} \
\
	auto it = g_pPushed##className.find(var); \
	if (it != g_pPushed##className.end()) \
	{ \
		g_Lua->ReferencePush(it->second->iReference); \
	} else { \
		g_Lua->PushUserType(var, luaType); \
		g_Lua->Push(-1); \
		LuaUserData* userData = new LuaUserData; \
		userData->iReference = g_Lua->ReferenceCreate(); \
		g_Lua->CreateTable(); \
		userData->iTableReference = g_Lua->ReferenceCreate(); \
		g_pPushed##className[var] = userData; \
	} \
} \
\
void Delete_##className(className* var) \
{ \
	auto it = g_pPushed##className.find(var); \
	if (it != g_pPushed##className.end()) \
	{ \
		delete it->second; \
		g_pPushed##className.erase(it); \
	} \
}

#define Vector_RemoveElement(vec, element) \
{ \
    auto _it = std::find((vec).begin(), (vec).end(), (element)); \
    if (_it != (vec).end()) \
        (vec).erase(_it); \
}

extern void HttpRequest_LuaInit();

extern void Push_HttpRequest(HttpRequest* pData);
extern void Delete_HttpRequest(HttpRequest* pData);

extern void Push_HttpResponse(HttpResponse* pData);
extern void Delete_HttpResponse(HttpResponse* pData);