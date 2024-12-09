#include "util.h"
#include <string>
#include "iserver.h"
#include <baseclient.h>

GarrysMod::Lua::ILuaInterface* g_Lua;

IServer* Util::server;
std::vector<CBaseClient*> Util::GetClients()
{
	std::vector<CBaseClient*> pClients;

	for (int i = 0; i < Util::server->GetClientCount(); i++)
	{
		IClient* pClient = Util::server->GetClient(i);
		pClients.push_back((CBaseClient*)pClient);
	}

	return pClients;
}