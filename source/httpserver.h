#include "GarrysMod/Lua/Interface.h"
#include "unordered_map"
#include "httplib.h"

struct RequestData_t {
	GarrysMod::Lua::CFunc func;
	httplib::Request request;
	bool handled = false;
	const char* path;
};

struct ThreadData_t {
	int port;
	const char* address;
	bool update = false;
	unsigned request_count = 0;
	std::unordered_map<unsigned, RequestData_t*> requests;
};

class HttpServer
{
public:
	httplib::Server server;

	HttpServer();

	void Think();
	void Start(const char*, unsigned);
	void Stop();
	void Get(const char*, GarrysMod::Lua::CFunc);
private:
	ThreadData_t* data;
	unsigned status = HTTPSERVER_OFFLINE;
};

enum
{
	HTTPSERVER_ONLINE,
	HTTPSERVER_OFFLINE
};