#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/InterfacePointers.hpp>
#include <GarrysMod/FactoryLoader.hpp>
#include "util.h"
#include "lua.h"
#include "cpp-httplib/httplib.h"
#include "iserver.h"
#include "threadtools.h"

struct ThreadData_t {
	int port;
};

httplib::Server svr;
unsigned HTTPServer(void* params)
{
	ThreadData_t* vars = (ThreadData_t*)params;

	svr.listen("0.0.0.0", vars->port); // The port for my testserver.

	return 0;
}

GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;

	LUA_InitServer(LUA);

	svr.Get("/", [=](const httplib::Request& req, httplib::Response& res) {
		Msg("Hello World from HTTP");
		printf("Works");
		printf(req.remote_addr.c_str());
		res.set_content("Hello World", "text/plain");
	});

	ThreadData_t* data = new ThreadData_t;
	data->port = 32039;
	CreateSimpleThread(HTTPServer, data);

	return 0;
}

GMOD_MODULE_CLOSE()
{
	LuaPrint("HTTPServer shutdown");
	svr.stop();

	return 0;
}