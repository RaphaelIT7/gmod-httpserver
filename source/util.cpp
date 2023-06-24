#include "GarrysMod/Lua/Interface.h"
#include "util.h"
#include <string>

GarrysMod::Lua::ILuaBase* GlobalLUA;

// should never be used outside of main thread!!! what happends: memory access violation
void LuaPrint(const char* Text) {
	GlobalLUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		GlobalLUA->GetField(-1, "print");
		GlobalLUA->PushString(Text);
		GlobalLUA->Call(1, 0);
	GlobalLUA->Pop();
}

// should never be used outside of main thread!!! what happends: memory access violation
void LuaPrint(std::string Text) {
	GlobalLUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		GlobalLUA->GetField(-1, "print");
		GlobalLUA->PushString(Text.c_str());
		GlobalLUA->Call(1, 0);
	GlobalLUA->Pop();
}

// should never be used outside of main thread!!! what happends: memory access violation
void LuaPrint(const char* Text, GarrysMod::Lua::ILuaBase* LUA) {
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "print");
		LUA->PushString(Text);
		LUA->Call(1, 0);
	LUA->Pop();
}

// should never be used outside of main thread!!! what happends: memory access violation
void LuaPrint(std::string Text, GarrysMod::Lua::ILuaBase* LUA) {
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "print");
		LUA->PushString(Text.c_str());
		LUA->Call(1, 0);
	LUA->Pop();
}

void Start_Table(GarrysMod::Lua::ILuaBase* LUA) {
	LUA->PushSpecial(SPECIAL_GLOB);
	LUA->CreateTable();
}

void Add_Func(GarrysMod::Lua::ILuaBase* LUA, CFunc Func, const char* Name) {
	LUA->PushCFunction(Func);
	LUA->SetField(-2, Name);
}

void Add_Func(GarrysMod::Lua::ILuaBase* LUA, CFunc Func, std::string Name) {
	LUA->PushCFunction(Func);
	LUA->SetField(-2, Name.c_str());
}

void Finish_Table(GarrysMod::Lua::ILuaBase* LUA, const char* Name) {
	LUA->SetField(-2, Name);
	LUA->Pop();
}

void Finish_Table(GarrysMod::Lua::ILuaBase* LUA, std::string Name) {
	LUA->SetField(-2, Name.c_str());
	LUA->Pop();
}

void ThrowError(const char* error) {
	GlobalLUA->ThrowError(error);
}