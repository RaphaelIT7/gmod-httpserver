#include "GarrysMod/Lua/Interface.h"
#include "unordered_map"
#include "httplib.h"

struct ResponseData_t {
	bool set_content = false;
	const char* content = "";
	const char* content_type = "text/plain";

	bool set_redirect = false;
	const char* redirect = "";
	int redirect_code = 302;

	bool set_header = false;
	std::unordered_map<const char*, const char*> headers;
};

struct RequestData_t {
	int func;
	ResponseData_t* response_data;
	httplib::Response response;
	httplib::Request request;
	bool handled = false;
	const char* path;
	bool should_delete = false;
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
	unsigned status = HTTPSERVER_OFFLINE;

	HttpServer();

	void Think();
	void Start(const char*, unsigned);
	void Stop();
	void Get(const char*, int, bool);
	void Post(const char*, int, bool);
	void Put(const char*, int, bool);
	void Patch(const char*, int, bool);
	void Delete(const char*, int, bool);
	void Options(const char*, int, bool);
	httplib::Server::Handler CreateHandler(const char*, int, bool);
private:
	ThreadData_t* data;
};