#include "httplib.h"
#include "unordered_map"

struct HttpResponse {
	bool bSetContent = false;
	bool bSetRedirect = false;
	bool bSetHeader = false;
	int iRedirectCode = 302;
	std::string strContent = "";
	std::string strContentType = "text/plain";
	std::string strRedirect = "";
	std::unordered_map<std::string, std::string> pHeaders;
};

struct HttpRequest {
	~HttpRequest();

	bool bHandled = false;
	bool bDelete = false; // We only delete from the main thread.
	int iFunction;
	std::string strPath;
	HttpResponse pResponseData;
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
	void Start(const char* address, unsigned short port);
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

	void Get(const char* path, int func, bool ipWhitelist)
	{
		m_pServer.Get(path, CreateHandler(path, func, ipWhitelist));
	}

	void Post(const char* path, int func, bool ipWhitelist)
	{
		m_pServer.Post(path, CreateHandler(path, func, ipWhitelist));
	}

	void Put(const char* path, int func, bool ipWhitelist)
	{
		m_pServer.Put(path, CreateHandler(path, func, ipWhitelist));
	}

	void Patch(const char* path, int func, bool ipWhitelist)
	{
		m_pServer.Patch(path, CreateHandler(path, func, ipWhitelist));
	}

	void Delete(const char* path, int func, bool ipWhitelist)
	{
		m_pServer.Delete(path, CreateHandler(path, func, ipWhitelist));
	}

	void Options(const char* path, int func, bool ipWhitelist)
	{
		m_pServer.Options(path, CreateHandler(path, func, ipWhitelist));
	}

	httplib::Server::Handler CreateHandler(const char* path, int func, bool ipWhitelist);

public:
	httplib::Server& GetServer() { return m_pServer; };
	unsigned char GetStatus() { return m_iStatus; };
	std::string& GetAddress() { return m_strAddress; };
	unsigned short GetPort() { return m_iPort; };
	void SetThreadSleep(unsigned int threadSleep) { m_iThreadSleep = threadSleep; };

private:
	unsigned char m_iStatus = HTTPSERVER_OFFLINE;
	unsigned short m_iPort;
	unsigned int m_iThreadSleep = 5; // How long the threads sleep / wait for a request to be handled
	bool m_bUpdate = false;
	bool m_bInUpdate = false;
	std::string m_strAddress;
	std::vector<HttpRequest*> m_pRequests;
	httplib::Server m_pServer;
};