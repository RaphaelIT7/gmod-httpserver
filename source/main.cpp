﻿#include <GarrysMod/InterfacePointers.hpp>
#include <GarrysMod/FactoryLoader.hpp>
#include "util.h"

GMOD_MODULE_OPEN()
{
	g_Lua = LUA;

	Util::server = InterfacePointers::Server();
	if (Util::server == nullptr)
		LUA->ThrowError("Failed to get IServer");

	Gmod_Server = InterfacePointers::Server();
	if (Gmod_Server == nullptr)
		ThrowError("Failed to get IServer");

	Engine = InterfacePointers::VEngineServer();
	if (Engine == nullptr)
		ThrowError("Failed to get IVEngineServer");

	LUA_InitServer(LUA);

	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}