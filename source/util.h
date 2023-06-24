#pragma once

#include "GarrysMod/Lua/Interface.h"
#include "GameEventListener.h"
#include <string>
#include "lua.h"

using namespace GarrysMod::Lua;

extern GarrysMod::Lua::ILuaBase* GlobalLUA;

extern void LuaPrint(const char*);
extern void LuaPrint(std::string);
extern void LuaPrint(const char*, GarrysMod::Lua::ILuaBase*);
extern void LuaPrint(std::string, GarrysMod::Lua::ILuaBase*);

extern void Start_Table(GarrysMod::Lua::ILuaBase*);

extern void Add_Func(GarrysMod::Lua::ILuaBase*, CFunc, const char*);
extern void Add_Func(GarrysMod::Lua::ILuaBase*, CFunc, std::string);

extern void Finish_Table(GarrysMod::Lua::ILuaBase*, const char*);
extern void Finish_Table(GarrysMod::Lua::ILuaBase*, std::string);

extern void ThrowError(const char*);

enum
{
	Realm_Client,
	Realm_Server,
	Realm_Menu
};

class CLuaShared
{
public:
	virtual ~CLuaShared() = 0;
	virtual int Init(const char*, const char*, void*, void*) = 0;
	virtual int Shutdown(void) = 0;
	virtual int DumpStats(void) = 0;
	virtual GarrysMod::Lua::ILuaBase* CreateLuaInterface(int, bool) = 0;
	virtual int CloseLuaInterface(GarrysMod::Lua::ILuaBase*) = 0;
	virtual GarrysMod::Lua::ILuaBase* GetLuaInterface(int) = 0;
	virtual int LoadFile(std::string, std::string, std::string, bool) = 0;
	virtual int GetCache(std::string) = 0;
	virtual int MountLua(const char*) = 0;
	virtual int MountLuaAdd(const char*, const char*) = 0;
	virtual int UnMountLua(const char*) = 0;
	virtual void SetFileContents() = 0;
	virtual int SetLuaFindHook(void*) = 0;
	virtual int FindScripts() = 0;
	virtual void* GetStackTraces() = 0;
};