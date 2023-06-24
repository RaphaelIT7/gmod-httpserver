#include "threadtools.h"
#include "util.h"

CThreadFastMutex* Mutex = new CThreadFastMutex();

unsigned Server(void * params)
{
	ThreadData_t* data = (ThreadData_t*)params;

	HTTPServer->server.listen(data->address, data->port);

	return 0;
}

HttpServer::HttpServer() {}

httplib::Response current_response;
LUA_FUNCTION(Set_Content)
{
	current_response.set_content(LUA->CheckString(1), LUA->CheckString(2));

	return 0;
}

void CallFunc(int func, httplib::Request request, httplib::Response response)
{
	GlobalLUA->ReferencePush(func);

	GlobalLUA->CreateTable();

	GlobalLUA->PushString(request.body.c_str());
	GlobalLUA->SetField(-2, "body");

	GlobalLUA->PushString(request.remote_addr.c_str());
	GlobalLUA->SetField(-2, "remote_addr");

	GlobalLUA->PushNumber(request.remote_port);
	GlobalLUA->SetField(-2, "remote_port");

	GlobalLUA->PushString(request.local_addr.c_str());
	GlobalLUA->SetField(-2, "local_addr");

	GlobalLUA->PushNumber(request.local_port);
	GlobalLUA->SetField(-2, "local_port");

	GlobalLUA->PushString(request.method.c_str());
	GlobalLUA->SetField(-2, "method");

	GlobalLUA->PushNumber(request.authorization_count_);
	GlobalLUA->SetField(-2, "authorization_count_");

	GlobalLUA->PushNumber(request.content_length_);
	GlobalLUA->SetField(-2, "content_length_");
	GlobalLUA->SetField(-2, "");

	GlobalLUA->CreateTable();

	GlobalLUA->PushCFunction(Set_Content);
	GlobalLUA->SetField(-2, "Set_Content");

	current_response = response;
	GlobalLUA->Call(2, 0);
}


void HttpServer::Think()
{
	if (status == HTTPSERVER_OFFLINE || !data->update) { return; }

	for (auto& [cache, entry] : data->requests) {
		CallFunc(entry->func, entry->request, entry->response);
		entry->handled = true;
	}

	Mutex->Lock();
	data->request_count = 0;
	data->update = false;
	Mutex->Unlock();
}

void HttpServer::Get(const char* path, int func)
{
	server.Get(path, [=](const httplib::Request& req, httplib::Response& res) {
		RequestData_t* request = new RequestData_t;
		request->path = path;
		request->request = req;
		request->func = func;
		request->response = res;
		Mutex->Lock();
		data->request_count = data->request_count + 1;
		data->requests[data->request_count] = request;
		data->update = true;
		Mutex->Unlock();
		while (!request->handled) {
			ThreadSleep(20);
		}
		delete request;
	});
}

void HttpServer::Start(const char* address, unsigned port)
{
	if (status != HTTPSERVER_OFFLINE) {
		Stop();
	}

	data = new ThreadData_t;
	data->address = address;
	data->port = port;
	CreateSimpleThread(Server, data);
	status = HTTPSERVER_ONLINE;
}

void HttpServer::Stop()
{
	if (status == HTTPSERVER_OFFLINE) { return; }

	server.stop();
	delete data;
	status = HTTPSERVER_OFFLINE;
}