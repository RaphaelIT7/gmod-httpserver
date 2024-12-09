#include "util.h"

static int ResponseData_TypeID = -1;
PushReferenced_LuaClass(ResponseData, ResponseData_TypeID)
Get_LuaClass(ResponseData, ResponseData_TypeID, "ResponseData")

LUA_FUNCTION_STATIC(ResponseData__tostring)
{
	ResponseData* pData = Get_ResponseData(1, false);
	if (!pData)
		LUA->PushString("ResponseData [NULL]");
	else
		LUA->PushString("ResponseData");
	return 1;
}

LUA_FUNCTION_STATIC(ResponseData__index)
{
	if (LUA->FindOnObjectsMetaTable(1, 2))
		return 1;

	LUA->Pop(1);
	LUA->ReferencePush(g_pPushedResponseData[Get_ResponseData(1, true)]->iTableReference);
	if (!LUA->FindObjectOnTable(-1, 2))
		LUA->PushNil();

	LUA->Remove(-2);

	return 1;
}

LUA_FUNCTION_STATIC(ResponseData__newindex)
{
	LUA->ReferencePush(g_pPushedResponseData[Get_ResponseData(1, true)]->iTableReference);
	LUA->Push(2);
	LUA->Push(3);
	LUA->RawSet(-3);
	LUA->Pop(1);

	return 0;
}

LUA_FUNCTION_STATIC(ResponseData_GetTable)
{
	LUA->ReferencePush(g_pPushedResponseData[Get_ResponseData(1, true)]->iTableReference); // This should never crash so no safety checks.
	return 1;
}

LUA_FUNCTION_STATIC(ResponseData_SetContent)
{
	ResponseData* pData = Get_ResponseData(1, true);
	pData->bSetContent = true;
	pData->strContent = LUA->CheckString(2);
	pData->strContentType = LUA->CheckString(3);

	return 0;
}

LUA_FUNCTION_STATIC(ResponseData_SetRedirect)
{
	ResponseData* pData = Get_ResponseData(1, true);
	pData->bSetRedirect = true;
	pData->strRedirect = LUA->CheckString(2);
	pData->iRedirectCode = LUA->CheckNumber(3);

	return 0;
}

LUA_FUNCTION_STATIC(ResponseData_SetHeader)
{
	ResponseData* pData = Get_ResponseData(1, true);
	pData->bSetHeader = true;
	pData->pHeaders[LUA->CheckString(2)] = LUA->CheckString(3);

	return 0;
}

static int RequestData_TypeID = -1;
PushReferenced_LuaClass(RequestData, RequestData_TypeID)
Get_LuaClass(RequestData, RequestData_TypeID, "RequestData")

LUA_FUNCTION_STATIC(RequestData__tostring)
{
	RequestData* pData = Get_RequestData(1, false);
	if (!pData)
		LUA->PushString("RequestData [NULL]");
	else
		LUA->PushString("RequestData");
	return 1;
}

LUA_FUNCTION_STATIC(RequestData__index)
{
	if (LUA->FindOnObjectsMetaTable(1, 2))
		return 1;

	LUA->Pop(1);
	LUA->ReferencePush(g_pPushedRequestData[Get_RequestData(1, true)]->iTableReference);
	if (!LUA->FindObjectOnTable(-1, 2))
		LUA->PushNil();

	LUA->Remove(-2);

	return 1;
}

LUA_FUNCTION_STATIC(RequestData__newindex)
{
	LUA->ReferencePush(g_pPushedRequestData[Get_RequestData(1, true)]->iTableReference);
	LUA->Push(2);
	LUA->Push(3);
	LUA->RawSet(-3);
	LUA->Pop(1);

	return 0;
}

LUA_FUNCTION_STATIC(RequestData_GetTable)
{
	LUA->ReferencePush(g_pPushedRequestData[Get_RequestData(1, true)]->iTableReference); // This should never crash so no safety checks.
	return 1;
}

LUA_FUNCTION_STATIC(RequestData_HasHeader)
{
	RequestData* pData = Get_RequestData(1, false);

	LUA->PushBool(pData->pRequest.has_header(LUA->CheckString(2)));
	return 1;
}

LUA_FUNCTION_STATIC(RequestData_HasParam)
{
	RequestData* pData = Get_RequestData(1, false);

	LUA->PushBool(pData->pRequest.has_param(LUA->CheckString(2)));
	return 1;
}

LUA_FUNCTION_STATIC(RequestData_GetHeader)
{
	RequestData* pData = Get_RequestData(1, false);
	const char* header = LUA->CheckString(2);

	LUA->PushString(pData->pRequest.get_header_value(header).c_str());
	return 1;
}

LUA_FUNCTION_STATIC(RequestData_GetParam)
{
	RequestData* pData = Get_RequestData(1, false);
	const char* param = LUA->CheckString(2);

	LUA->PushString(pData->pRequest.get_param_value(param).c_str());
	return 1;
}

LUA_FUNCTION_STATIC(RequestData_GetBody)
{
	RequestData* pData = Get_RequestData(1, false);

	LUA->PushString(pData->pRequest.body.c_str());
	return 1;
}

LUA_FUNCTION_STATIC(RequestData_GetRemoteAddr)
{
	RequestData* pData = Get_RequestData(1, false);

	LUA->PushString(pData->pRequest.remote_addr.c_str());
	return 1;
}

LUA_FUNCTION_STATIC(RequestData_GetRemotePort)
{
	RequestData* pData = Get_RequestData(1, false);

	LUA->PushNumber(pData->pRequest.remote_port);
	return 1;
}

LUA_FUNCTION_STATIC(RequestData_GetLocalAddr)
{
	RequestData* pData = Get_RequestData(1, false);

	LUA->PushString(pData->pRequest.local_addr.c_str());
	return 1;
}

LUA_FUNCTION_STATIC(RequestData_GetLocalPort)
{
	RequestData* pData = Get_RequestData(1, false);

	LUA->PushNumber(pData->pRequest.local_port);
	return 1;
}

LUA_FUNCTION_STATIC(RequestData_GetMethod)
{
	RequestData* pData = Get_RequestData(1, false);

	LUA->PushString(pData->pRequest.method.c_str());
	return 1;
}

LUA_FUNCTION_STATIC(RequestData_GetAuthorizationCount)
{
	RequestData* pData = Get_RequestData(1, false);

	LUA->PushNumber(pData->pRequest.authorization_count_);
	return 1;
}

LUA_FUNCTION_STATIC(RequestData_GetContentLength)
{
	RequestData* pData = Get_RequestData(1, false);

	LUA->PushNumber(pData->pRequest.content_length_);
	return 1;
}

void RequestData_LuaInit()
{
	ResponseData_TypeID = g_Lua->CreateMetaTable("ResponseData");
		Util::AddFunc(ResponseData__tostring, "__tostring");
		Util::AddFunc(ResponseData__index, "__index");
		Util::AddFunc(ResponseData__newindex, "__newindex");
		Util::AddFunc(ResponseData_GetTable, "GetTable");

		Util::AddFunc(ResponseData_SetContent, "SetContent");
		Util::AddFunc(ResponseData_SetHeader, "SetHeader");
		Util::AddFunc(ResponseData_SetRedirect, "SetRedirect");
	g_Lua->Pop(1);

	RequestData_TypeID = g_Lua->CreateMetaTable("RequestData");
		Util::AddFunc(RequestData__tostring, "__tostring");
		Util::AddFunc(RequestData__index, "__index");
		Util::AddFunc(RequestData__newindex, "__newindex");
		Util::AddFunc(RequestData_GetTable, "GetTable");

		Util::AddFunc(RequestData_HasHeader, "HasHeader");
		Util::AddFunc(RequestData_HasParam, "HasParam");
		Util::AddFunc(RequestData_GetHeader, "GetHeader");
		Util::AddFunc(RequestData_GetParam, "GetParam");
		Util::AddFunc(RequestData_GetBody, "GetBody");
		Util::AddFunc(RequestData_GetRemoteAddr, "GetRemoteAddr");
		Util::AddFunc(RequestData_GetRemotePort, "GetRemotePort");
		Util::AddFunc(RequestData_GetLocalAddr, "GetLocalAddr");
		Util::AddFunc(RequestData_GetLocalPort, "GetLocalPort");
		Util::AddFunc(RequestData_GetMethod, "GetMethod");
		Util::AddFunc(RequestData_GetAuthorizationCount, "GetAuthorizationCount");
		Util::AddFunc(RequestData_GetContentLength, "GetContentLength");
	g_Lua->Pop(1);
}