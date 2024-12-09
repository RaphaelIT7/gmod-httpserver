#include "GarrysMod/Lua/Interface.h"
#include "unordered_map"
#include "httplib.h"

struct ResponseData {
	bool bSetContent = false;
	bool bSetRedirect = false;
	bool bSetHeader = false;
	int iRedirectCode = 302;
	std::string strContent = "";
	std::string strContentType = "text/plain";
	std::string strRedirect = "";
	std::unordered_map<std::string, std::string> pHeaders;
};

struct RequestData {
	bool bHandled = false;
	bool bDelete = false;
	int iFunction;
	std::string strPath;
	ResponseData* pResponseData;
	httplib::Response pResponse;
	httplib::Request pRequest;
};

enum
{
	HTTPSERVER_ONLINE,
	HTTPSERVER_OFFLINE
};

class HttpServer
{
public:
	void Start(const char* address, unsigned port);
	void Stop();
	void Think();

#if ARCHITECTURE_IS_X86_64
	static long long unsigned Server(void* params)
#else
	static unsigned Server(void* params)
#endif
	{
		HttpServer* pServer = (HttpServer*)params;
		pServer->GetServer().listen(pServer->GetAddress(), pServer->GetPort());

		return 0;
	}

	void Get(const char* path, int func, bool ipwhitelist)
	{
		m_pServer.Get(path, CreateHandler(path, func, ipwhitelist));
	}

	void Post(const char* path, int func, bool ipwhitelist)
	{
		m_pServer.Post(path, CreateHandler(path, func, ipwhitelist));
	}

	void Put(const char* path, int func, bool ipwhitelist)
	{
		m_pServer.Put(path, CreateHandler(path, func, ipwhitelist));
	}

	void Patch(const char* path, int func, bool ipwhitelist)
	{
		m_pServer.Patch(path, CreateHandler(path, func, ipwhitelist));
	}

	void Delete(const char* path, int func, bool ipwhitelist)
	{
		m_pServer.Delete(path, CreateHandler(path, func, ipwhitelist));
	}

	void Options(const char* path, int func, bool ipwhitelist)
	{
		m_pServer.Options(path, CreateHandler(path, func, ipwhitelist));
	}

	httplib::Server::Handler CreateHandler(const char*, int, bool);

public:
	httplib::Server& GetServer() { return m_pServer; };
	unsigned char GetStatus() { return m_iStatus; };
	std::string& GetAddress() { return m_strAddress; };
	unsigned short GetPort() { return m_iPort; };

private:
	unsigned char m_iStatus = HTTPSERVER_OFFLINE;
	unsigned short m_iPort;
	bool m_bUpdate = false;
	bool m_bInUpdate = false;
	std::string m_strAddress;
	std::vector<RequestData*> m_pRequests;
	httplib::Server m_pServer;
};