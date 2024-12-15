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

	if (g_Lua->CallFunctionProtected(2, 1, true))
	{
		request->bHandled = !g_Lua->GetBool(-1);
		g_Lua->Pop(1);
	}

	Delete_HttpRequest(request);
	Delete_HttpResponse(response); // Destroys the Lua reference after we used it
}

void HttpServer::Start(const char* address, unsigned short port)
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
	for (auto it = m_pRequests.begin(); it != m_pRequests.end(); ++it)
	{
		auto pEntry = (*it);
		if (pEntry->bDelete)
		{
			it = m_pRequests.erase(it);
			delete pEntry;
			continue;
		}

		if (!pEntry->bHandled)
			CallFunc(pEntry->iFunction, pEntry, &pEntry->pResponseData);
	}

	m_bUpdate = false;
	m_bInUpdate = false;
}

static std::string localAddr = "127.0.0.1";
static std::string loopBack = "loopback";
httplib::Server::Handler HttpServer::CreateHandler(const char* path, int func, bool ipWhitelist)
{
	return [=](const httplib::Request& req, httplib::Response& res)
	{
		if (ipWhitelist)
		{
			bool found = false;
			for (auto& pClient : Util::GetClients())
			{
				if (!pClient->IsActive())
					continue;

				const netadr_s& addr = pClient->GetNetChannel()->GetRemoteAddress();
				std::string address = addr.ToString();
				size_t port_pos = address.find(":");
				if (address.substr(0, port_pos) == req.remote_addr || (req.remote_addr == localAddr && address.substr(0, port_pos) == loopBack))
				{
					found = true;
					break;
				}
			}

			if (!found)
				return;
		}

		HttpRequest* request = new HttpRequest;
		request->strPath = path;
		request->pRequest = req;
		request->iFunction = func;
		request->pResponse = res;
		m_pRequests.push_back(request); // We should add a check here since we could write to it from multiple threads?
		m_bUpdate = true;
		while (!request->bHandled)
			ThreadSleep(m_iThreadSleep);

		HttpResponse* rdata = &request->pResponseData;
		if (rdata->bSetContent)
			res.set_content(rdata->strContent, rdata->strContentType);

		if (rdata->bSetRedirect)
			res.set_redirect(rdata->strRedirect, rdata->iRedirectCode);

		if (rdata->bSetHeader)
			for (auto& [key, value] : rdata->pHeaders)
				res.set_header(key, value);

		request->bDelete = true;
	};
}