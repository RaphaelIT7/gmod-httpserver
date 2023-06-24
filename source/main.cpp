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
		res.set_content("Hello World", "text/plain");
		LuaPrint("Will this crash the server");
	});

	svr.listen("0.0.0.0", 32039); // The port for my testserver.
	Msg("Continued");

	return 0;
}

GMOD_MODULE_CLOSE()
{
	LuaPrint("HTTPServer shutdown");
	svr.stop();

	return 0;
}