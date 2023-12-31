#include "threadtools.h"
#include "util.h"
#include <netadr.h>
#include "iclient.h"
#include "inetchannel.h"

CThreadFastMutex* Mutex = new CThreadFastMutex();

unsigned Server(void * params)
{
	ThreadData_t* data = (ThreadData_t*)params;

	HTTPServer->server.listen(data->address, data->port);

	return 0;
}

HttpServer::HttpServer() {}

ResponseData_t* current_response;
LUA_FUNCTION(SetContent)
{
	Mutex->Lock();
	current_response->set_content = true;
	current_response->content = LUA->CheckString(1);
	current_response->content_type = LUA->CheckString(2);
	Mutex->Unlock();

	return 0;
}

LUA_FUNCTION(SetRedirect)
{
	Mutex->Lock();
	current_response->set_redirect = true;
	current_response->redirect = LUA->CheckString(1);
	current_response->redirect_code = LUA->CheckNumber(2);
	Mutex->Unlock();

	return 0;
}

LUA_FUNCTION(SetHeader)
{
	Mutex->Lock();
	current_response->set_header = true;
	current_response->headers[LUA->CheckString(1)] = LUA->CheckString(2);
	Mutex->Unlock();

	return 0;
}

httplib::Request current_request;
LUA_FUNCTION(HasHeader)
{
	LUA->PushBool(current_request.has_header(LUA->CheckString(1)));
	return 1;
}

LUA_FUNCTION(HasParam)
{
	LUA->PushBool(current_request.has_param(LUA->CheckString(1)));
	return 1;
}

LUA_FUNCTION(GetHeader)
{
	const char* header = LUA->CheckString(1);
	LUA->PushString(current_request.get_header_value(header).c_str());
	return 1;
}

LUA_FUNCTION(GetParam)
{
	const char* param = LUA->CheckString(1);
	LUA->PushString(current_request.get_param_value(param).c_str());
	return 1;
}

void CallFunc(int func, httplib::Request request, ResponseData_t* response)
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
	GlobalLUA->SetField(-2, "authorization_count");

	GlobalLUA->PushNumber(request.content_length_);
	GlobalLUA->SetField(-2, "content_length");

	GlobalLUA->PushCFunction(HasHeader);
	GlobalLUA->SetField(-2, "HasHeader");

	GlobalLUA->PushCFunction(HasParam);
	GlobalLUA->SetField(-2, "HasParam");

	GlobalLUA->PushCFunction(GetHeader);
	GlobalLUA->SetField(-2, "GetHeader");

	GlobalLUA->PushCFunction(GetParam);
	GlobalLUA->SetField(-2, "GetParam");

	GlobalLUA->CreateTable();

	GlobalLUA->PushCFunction(SetContent);
	GlobalLUA->SetField(-2, "SetContent");

	GlobalLUA->PushCFunction(SetRedirect);
	GlobalLUA->SetField(-2, "SetRedirect");

	GlobalLUA->PushCFunction(SetHeader);
	GlobalLUA->SetField(-2, "SetHeader");

	current_response = response;
	current_request = request;
	GlobalLUA->PCall(2, 0, 0);
}


void HttpServer::Think()
{
	if (status == HTTPSERVER_OFFLINE || !data->update) { return; }

	for (auto& [id, entry] : data->requests) {
		if (entry->handled) { return; }
		if (entry->should_delete) {
			data->requests.erase(id);
			delete entry;
			return;
		}

		CallFunc(entry->func, entry->request, entry->response_data);
		entry->handled = true;
	}

	Mutex->Lock();
	data->update = false;
	Mutex->Unlock();
}

httplib::Server::Handler HttpServer::CreateHandler(const char* path, int func, bool ipwhitelist)
{
	return [=](const httplib::Request& req, httplib::Response& res) {
		if (ipwhitelist) {
			bool found = false;
			for (int i = 1; i <= Gmod_Server->GetMaxClients(); ++i)
			{
				INetChannelInfo* channel = Engine->GetPlayerNetInfo(i);
				if (channel == nullptr) { continue; }; // We skip bots and empty slots with this.

				IClient* client = Gmod_Server->GetClient(i - 1);
				netadr_s addr = client->GetNetChannel()->GetRemoteAddress();
				std::string address = addr.ToString();
				size_t port_pos = address.find(":");
				if (address.substr(0, port_pos) == req.remote_addr || (req.remote_addr == "127.0.0.1" && address.substr(0, port_pos) == "loopback")) {
					found = true;
					break;
				}
			}

			if (!found) { return; }
		}

		RequestData_t* request = new RequestData_t;
		request->path = path;
		request->request = req;
		request->func = func;
		request->response = res;
		request->response_data = new ResponseData_t;
		Mutex->Lock();
		data->request_count = data->request_count + 1;
		data->requests[data->request_count] = request;
		data->update = true;
		Mutex->Unlock();
		while (!request->handled) {
			ThreadSleep(1);
		}
		Mutex->Lock();
		request->should_delete = true;
		Mutex->Unlock();
		ResponseData_t* rdata = request->response_data;
		if (rdata->set_content) {
			res.set_content(rdata->content, rdata->content_type);
		}

		if (rdata->set_redirect) {
			res.set_redirect(rdata->redirect, rdata->redirect_code);
		}

		if (rdata->set_header) {
			for (auto& [key, value] : rdata->headers) {
				res.set_header(key, value);
			}
		}
	};
}

void HttpServer::Get(const char* path, int func, bool ipwhitelist)
{
	server.Get(path, CreateHandler(path, func, ipwhitelist));
}

void HttpServer::Post(const char* path, int func, bool ipwhitelist)
{
	server.Post(path, CreateHandler(path, func, ipwhitelist));
}

void HttpServer::Put(const char* path, int func, bool ipwhitelist)
{
	server.Put(path, CreateHandler(path, func, ipwhitelist));
}

void HttpServer::Patch(const char* path, int func, bool ipwhitelist)
{
	server.Patch(path, CreateHandler(path, func, ipwhitelist));
}

void HttpServer::Delete(const char* path, int func, bool ipwhitelist)
{
	server.Delete(path, CreateHandler(path, func, ipwhitelist));
}

void HttpServer::Options(const char* path, int func, bool ipwhitelist)
{
	server.Options(path, CreateHandler(path, func, ipwhitelist));
}

void HttpServer::Start(const char* address, unsigned port)
{
	if (status != HTTPSERVER_OFFLINE) {
		Stop();
	}

	data = new ThreadData_t;
	data->address = address;
	data->port = port;
	CreateSimpleThread((ThreadFunc_t)Server, data);
	status = HTTPSERVER_ONLINE;
}

void HttpServer::Stop()
{
	if (status == HTTPSERVER_OFFLINE) { return; }

	Mutex->Lock();
	server.stop();
	delete data;
	Mutex->Unlock();

	status = HTTPSERVER_OFFLINE;
}