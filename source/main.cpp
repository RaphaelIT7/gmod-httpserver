#include <GarrysMod/Lua/Interface.h>
#include <GarrysMod/InterfacePointers.hpp>
#include <GarrysMod/FactoryLoader.hpp>
#include "util.h"
#include "lua.h"
#include "cpp-httplib/httplib.h"
#include "iserver.h"
#include "threadtools.h"
#include "unordered_map"

struct ThreadData_t {
	int port;
	bool update = false;
	std::unordered_map<unsigned, httplib::Request> requests;
};
ThreadData_t* data;

httplib::Server svr;
unsigned HTTPServer(void* params)
{
	ThreadData_t* vars = (ThreadData_t*)params;

	svr.set_read_timeout(10000);
	svr.set_keep_alive_timeout(10000);
	svr.set_write_timeout(10000);

	svr.listen("0.0.0.0", vars->port); // The port for my testserver.

	return 0;
}

LUA_FUNCTION(Think)
{
	if (data->update) {
		LUA->PushString("Update");
	}
	else {
		LUA->PushString("Nothing");
	}

	return 1;
}

CThreadFastMutex* Mutex = new CThreadFastMutex();
GMOD_MODULE_OPEN()
{
	GlobalLUA = LUA;

	LUA_InitServer(LUA);

	data = new ThreadData_t;
	data->port = 32039;

	svr.Get("/", [=](const httplib::Request& req, httplib::Response& res) {
		Mutex->Lock();
		data->requests[data->requests.size() + 1] = req;
		data->update = true;
		Mutex->Unlock();
		ThreadSleep(1000);
		res.set_content("Hello World", "text/plain");
	});

	CreateSimpleThread(HTTPServer, data);

	Start_Table(LUA);
		Add_Func(LUA, Think, "Think");
	Finish_Table(LUA, "httpserver");

	LUA->PushSpecial(SPECIAL_GLOB);
	LUA->GetField(-1, "RunString");
	LUA->PushString(" \
		hook.Add('Think', 'HTTPServer', function() \
				httpserver.Think() \
		end)");
	LUA->Call(1, 0);
	LUA->Pop();


	return 0;
}

GMOD_MODULE_CLOSE()
{
	LuaPrint("HTTPServer shutdown");
	svr.stop();

	return 0;
}