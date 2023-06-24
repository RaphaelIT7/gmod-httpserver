#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/FactoryLoader.hpp>
#include "util.h"
#include "lua.h"
#include "cpp-httplib/httplib.h"

GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;

	LUA_InitServer(LUA);

	httplib::Server svr;

	svr.Get("/hi", [](const httplib::Request&, httplib::Response& res) {
		res.set_content("Hello World!", "text/plain");
		});

	svr.listen("0.0.0.0", 32039); // The port for my testserver.

	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}