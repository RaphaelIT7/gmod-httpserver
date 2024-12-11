#include "threadtools.h"
#include "util.h"
#include <netadr.h>
#include "baseclient.h"
#include "inetchannel.h"

void CallFunc(int func, HttpRequest* request, HttpResponse* response)
{
	g_Lua->ReferencePush(func);

	Push_HttpRequest(request);
	Push_HttpResponse(response);

	g_Lua->PCall(2, 0, 0);

	Delete_HttpRequest(request);
	Delete_HttpResponse(response); // Destroys the Lua reference after we used it
}

void HttpServer::Start(const char* address, unsigned port)
{
	if (m_iStatus != HTTPSERVER_OFFLINE)
		Stop();

	m_strAddress = address;
	m_iPort = port;
	CreateSimpleThread((ThreadFunc_t)HttpServer::Server, this);
	m_iStatus = HTTPSERVER_ONLINE;
}

void HttpServer::Stop()
{
	if (m_iStatus == HTTPSERVER_OFFLINE)
		return;

	m_pServer.stop();
	m_iStatus = HTTPSERVER_OFFLINE;
}

void HttpServer::Think()
{
	if (m_iStatus == HTTPSERVER_OFFLINE || !m_bUpdate)
		return;

	m_bInUpdate = true;
	for (auto it = m_pRequests.begin(); it != m_pRequests.end(); ++it) {
		auto pEntry = (*it);
		if (pEntry->bHandled) { continue; }
		if (pEntry->bDelete) {
			it = m_pRequests.erase(it);
			delete pEntry;
			continue;
		}

		CallFunc(pEntry->iFunction, pEntry, &pEntry->pResponseData);
		pEntry->bHandled = true;
	}

	m_bUpdate = false;
	m_bInUpdate = false;
}

httplib::Server::Handler HttpServer::CreateHandler(const char* path, int func, bool ipwhitelist)
{
	return [=](const httplib::Request& req, httplib::Response& res) {
		if (ipwhitelist) {
			bool found = false;
			for (auto& pClient : Util::GetClients())
			{
				if (!pClient->IsActive())
					continue;

				const netadr_s& addr = pClient->GetNetChannel()->GetRemoteAddress();
				std::string address = addr.ToString();
				size_t port_pos = address.find(":");
				if (address.substr(0, port_pos) == req.remote_addr || (req.remote_addr == "127.0.0.1" && address.substr(0, port_pos) == "loopback")) {
					found = true;
					break;
				}
			}

			if (!found) { return; }
		}

		HttpRequest* request = new HttpRequest;
		request->strPath = path;
		request->pRequest = req;
		request->iFunction = func;
		request->pResponse = res;
		m_pRequests.push_back(request); // We should add a check here since we could write to it from multiple threads?
		m_bUpdate = true;
		while (!request->bHandled) {
			ThreadSleep(1);
		}
		HttpResponse* rdata = &request->pResponseData;
		if (rdata->bSetContent) {
			res.set_content(rdata->strContent, rdata->strContentType);
		}

		if (rdata->bSetRedirect) {
			res.set_redirect(rdata->strRedirect, rdata->iRedirectCode);
		}

		if (rdata->bSetHeader) {
			for (auto& [key, value] : rdata->pHeaders) {
				res.set_header(key, value);
			}
		}

		request->bDelete = true;
	};
}