#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/InterfacePointers.hpp>
#include <GarrysMod/FactoryLoader.hpp>
#include "util.h"
#include "lua.h"
#include "cpp-httplib/httplib.h"
#include "iserver.h"

httplib::Server svr;
GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;

	LUA_InitServer(LUA);

	IServer* server = InterfacePointers::Server();

	svr.Get("/", [=](const httplib::Request&, httplib::Response& res) {
		res.set_content("Hello World! \n\
			Servername: " + std::string(server->GetName()) + "\
			Mapname: " + std::string(server->GetMapName()), "text/plain");
	});

	svr.listen("0.0.0.0", 32039); // The port for my testserver.

	return 0;
}

GMOD_MODULE_CLOSE()
{
	svr.stop();

	return 0;
}