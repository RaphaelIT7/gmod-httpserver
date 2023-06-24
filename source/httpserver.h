#include "GarrysMod/Lua/Interface.h"
#include "unordered_map"
#include "httplib.h"

struct RequestData_t {
	int func;
	httplib::Response response;
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

enum
{
	HTTPSERVER_ONLINE,
	HTTPSERVER_OFFLINE
};

class HttpServer
{
public:
	httplib::Server server;

	HttpServer();

	void Think();
	void Start(const char*, unsigned);
	void Stop();
	void Get(const char*, int);
private:
	ThreadData_t* data;
	unsigned status = HTTPSERVER_OFFLINE;
};