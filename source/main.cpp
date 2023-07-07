#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/InterfacePointers.hpp>
#include <GarrysMod/FactoryLoader.hpp>
#include "util.h"

GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;

	HTTPServer = new HttpServer();

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
	HTTPServer->Stop();

	return 0;
}