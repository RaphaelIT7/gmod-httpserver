#include "util.h"

static int HttpResponse_TypeID = -1;
PushReferenced_LuaClass(HttpResponse, HttpResponse_TypeID)
Get_LuaClass(HttpResponse, HttpResponse_TypeID, "HttpResponse")

LUA_FUNCTION_STATIC(HttpResponse__tostring)
{
	HttpResponse* pData = Get_HttpResponse(1, false);
	if (!pData)
		LUA->PushString("HttpResponse [NULL]");
	else
		LUA->PushString("HttpResponse");
	return 1;
}

LUA_FUNCTION_STATIC(HttpResponse__index)
{
	if (LUA->FindOnObjectsMetaTable(1, 2))
		return 1;

	LUA->Pop(1);
	LUA->ReferencePush(g_pPushedHttpResponse[Get_HttpResponse(1, true)]->iTableReference);
	if (!LUA->FindObjectOnTable(-1, 2))
		LUA->PushNil();

	LUA->Remove(-2);

	return 1;
}

LUA_FUNCTION_STATIC(HttpResponse__newindex)
{
	LUA->ReferencePush(g_pPushedHttpResponse[Get_HttpResponse(1, true)]->iTableReference);
	LUA->Push(2);
	LUA->Push(3);
	LUA->RawSet(-3);
	LUA->Pop(1);

	return 0;
}

LUA_FUNCTION_STATIC(HttpResponse_GetTable)
{
	LUA->ReferencePush(g_pPushedHttpResponse[Get_HttpResponse(1, true)]->iTableReference); // This should never crash so no safety checks.
	return 1;
}

LUA_FUNCTION_STATIC(HttpResponse_SetContent)
{
	HttpResponse* pData = Get_HttpResponse(1, true);
	pData->bSetContent = true;
	pData->strContent = LUA->CheckString(2);
	pData->strContentType = LUA->CheckString(3);

	return 0;
}

LUA_FUNCTION_STATIC(HttpResponse_SetRedirect)
{
	HttpResponse* pData = Get_HttpResponse(1, true);
	pData->bSetRedirect = true;
	pData->strRedirect = LUA->CheckString(2);
	pData->iRedirectCode = LUA->CheckNumber(3);

	return 0;
}

LUA_FUNCTION_STATIC(HttpResponse_SetHeader)
{
	HttpResponse* pData = Get_HttpResponse(1, true);
	pData->bSetHeader = true;
	pData->pHeaders[LUA->CheckString(2)] = LUA->CheckString(3);

	return 0;
}

static int HttpRequest_TypeID = -1;
PushReferenced_LuaClass(HttpRequest, HttpRequest_TypeID)
Get_LuaClass(HttpRequest, HttpRequest_TypeID, "HttpRequest")

LUA_FUNCTION_STATIC(HttpRequest__tostring)
{
	HttpRequest* pData = Get_HttpRequest(1, false);
	if (!pData)
		LUA->PushString("HttpRequest [NULL]");
	else
		LUA->PushString("HttpRequest");
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest__index)
{
	if (LUA->FindOnObjectsMetaTable(1, 2))
		return 1;

	LUA->Pop(1);
	LUA->ReferencePush(g_pPushedHttpRequest[Get_HttpRequest(1, true)]->iTableReference);
	if (!LUA->FindObjectOnTable(-1, 2))
		LUA->PushNil();

	LUA->Remove(-2);

	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest__newindex)
{
	LUA->ReferencePush(g_pPushedHttpRequest[Get_HttpRequest(1, true)]->iTableReference);
	LUA->Push(2);
	LUA->Push(3);
	LUA->RawSet(-3);
	LUA->Pop(1);

	return 0;
}

LUA_FUNCTION_STATIC(HttpRequest_GetTable)
{
	LUA->ReferencePush(g_pPushedHttpRequest[Get_HttpRequest(1, true)]->iTableReference); // This should never crash so no safety checks.
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest_HasHeader)
{
	HttpRequest* pData = Get_HttpRequest(1, false);

	LUA->PushBool(pData->pRequest.has_header(LUA->CheckString(2)));
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest_HasParam)
{
	HttpRequest* pData = Get_HttpRequest(1, false);

	LUA->PushBool(pData->pRequest.has_param(LUA->CheckString(2)));
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest_GetHeader)
{
	HttpRequest* pData = Get_HttpRequest(1, false);
	const char* header = LUA->CheckString(2);

	LUA->PushString(pData->pRequest.get_header_value(header).c_str());
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest_GetParam)
{
	HttpRequest* pData = Get_HttpRequest(1, false);
	const char* param = LUA->CheckString(2);

	LUA->PushString(pData->pRequest.get_param_value(param).c_str());
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest_GetBody)
{
	HttpRequest* pData = Get_HttpRequest(1, false);

	LUA->PushString(pData->pRequest.body.c_str());
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest_GetRemoteAddr)
{
	HttpRequest* pData = Get_HttpRequest(1, false);

	LUA->PushString(pData->pRequest.remote_addr.c_str());
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest_GetRemotePort)
{
	HttpRequest* pData = Get_HttpRequest(1, false);

	LUA->PushNumber(pData->pRequest.remote_port);
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest_GetLocalAddr)
{
	HttpRequest* pData = Get_HttpRequest(1, false);

	LUA->PushString(pData->pRequest.local_addr.c_str());
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest_GetLocalPort)
{
	HttpRequest* pData = Get_HttpRequest(1, false);

	LUA->PushNumber(pData->pRequest.local_port);
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest_GetMethod)
{
	HttpRequest* pData = Get_HttpRequest(1, false);

	LUA->PushString(pData->pRequest.method.c_str());
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest_GetAuthorizationCount)
{
	HttpRequest* pData = Get_HttpRequest(1, false);

	LUA->PushNumber(pData->pRequest.authorization_count_);
	return 1;
}

LUA_FUNCTION_STATIC(HttpRequest_GetContentLength)
{
	HttpRequest* pData = Get_HttpRequest(1, false);

	LUA->PushNumber(pData->pRequest.content_length_);
	return 1;
}

void HttpRequest_LuaInit()
{
	HttpResponse_TypeID = g_Lua->CreateMetaTable("HttpResponse");
		Util::AddFunc(HttpResponse__tostring, "__tostring");
		Util::AddFunc(HttpResponse__index, "__index");
		Util::AddFunc(HttpResponse__newindex, "__newindex");
		Util::AddFunc(HttpResponse_GetTable, "GetTable");

		Util::AddFunc(HttpResponse_SetContent, "SetContent");
		Util::AddFunc(HttpResponse_SetHeader, "SetHeader");
		Util::AddFunc(HttpResponse_SetRedirect, "SetRedirect");
	g_Lua->Pop(1);

	HttpRequest_TypeID = g_Lua->CreateMetaTable("HttpRequest");
		Util::AddFunc(HttpRequest__tostring, "__tostring");
		Util::AddFunc(HttpRequest__index, "__index");
		Util::AddFunc(HttpRequest__newindex, "__newindex");
		Util::AddFunc(HttpRequest_GetTable, "GetTable");

		Util::AddFunc(HttpRequest_HasHeader, "HasHeader");
		Util::AddFunc(HttpRequest_HasParam, "HasParam");
		Util::AddFunc(HttpRequest_GetHeader, "GetHeader");
		Util::AddFunc(HttpRequest_GetParam, "GetParam");
		Util::AddFunc(HttpRequest_GetBody, "GetBody");
		Util::AddFunc(HttpRequest_GetRemoteAddr, "GetRemoteAddr");
		Util::AddFunc(HttpRequest_GetRemotePort, "GetRemotePort");
		Util::AddFunc(HttpRequest_GetLocalAddr, "GetLocalAddr");
		Util::AddFunc(HttpRequest_GetLocalPort, "GetLocalPort");
		Util::AddFunc(HttpRequest_GetMethod, "GetMethod");
		Util::AddFunc(HttpRequest_GetAuthorizationCount, "GetAuthorizationCount");
		Util::AddFunc(HttpRequest_GetContentLength, "GetContentLength");
	g_Lua->Pop(1);
}