﻿#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/InterfacePointers.hpp>
#include <GarrysMod/FactoryLoader.hpp>
#include "util.h"
#include "lua.h"
#include "iserver.h"

GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;

	HTTPServer = new HttpServer();

	LUA_InitServer(LUA);

	return 0;
}

GMOD_MODULE_CLOSE()
{
	HTTPServer->Stop();

	return 0;
}