This Project aims to add a HTTPServer to Gmod.

This project uses the [cpp-httplib](https://github.com/yhirose/cpp-httplib) as the HTTPServer.

TODO:
- Move IP Whitelist check to a Pre-routing handler
- Add a Error handler and function
- Add a Exception handler and function
- Add a Pre-routing handler and function
- Add a Post-routing handler and function

# Functions

## Main Functions

#### HttpServer HttpServer:Create()
Creates a new HTTPServer.  

#### HttpServer:Destroy(HttpServer server)
Destroys the given http server.  

## Basic Functions

#### HttpServer:Start(String IP, Number Port)
This will start or restart the HTTP Server, and it will listen on the given address + port.  
NOTE: If a Method function was called like HttpServer:Get after HttpServer:Start was called, you need to call HttpServer:Start again!
#### HttpServer:Stop()
This stops the HTTP Server.
#### (internal function) HttpServer:Think()
This is internally used to manage all requests and to call all functions needed.

## Method Functions
All Method functions add a listener for the given path and the given method, like this:
```lua
HttpServer:Get("/public", function(_, response)
  print("Public GET request")
  response.SetContent("You sent a GET request to a public site.", "text/plain")
end, false)

HttpServer:Get("/private", function(_, response)
  print("Private GET request")
  response.SetContent("You sent a GET request to a private site.", "text/plain")
end, true)
```

If you enable the IP Whitelist, only requests sent by connected players are processed.

#### HttpServer:Get(String path, function (Request, Response), bool ipwhitelist)
#### HttpServer:Put(String path, function (Request, Response), bool ipwhitelist)
#### HttpServer:Post(String path, function (Request, Response), bool ipwhitelist)
#### HttpServer:Patch(String path, function (Request, Response), bool ipwhitelist)
#### HttpServer:Delete(String path, function (Request, Response), bool ipwhitelist)
#### HttpServer:Options(String path, function (Request, Response), bool ipwhitelist)

## Additional Functions

#### bool HttpServer:IsRunning()
Returns true if the HTTPServer is running.

#### HttpServer:SetTCPnodelay(bool nodelay)
Sets whether a delay should be added to tcp or not.

#### HttpServer:SetReadTimeout(int sec, int usec)
Sets the maximum amount of time before a read times out.

#### HttpServer:SetWriteTimeout(int sec, int usec)
Sets the maximum amount of time before a write times out.

#### HttpServer:SetPayloadMaxLength(int maxlength)
Sets the maximum payload length.

#### HttpServer:SetKeepAliveTimeout(int sec)
Sets the maximum time a connection is kept alive.

#### HttpServer:SetKeepAliveMaxCount(int amount)
Sets the maximum amount of connections that can be kept alive at the same time.

#### HttpServer:SetMountPoint(string path, string folder)
This mounts the given folder to the given path.
(You can call this multiple times for the same path to mount multiple folders to it.)

#### HttpServer:RemoveMountPoint(string path)
This removes all mounts for the given path.

## Request

#### bool Request.HasHeader(key)
returns true if the client has the given key in the header.

#### bool Request.HasParam(key)
returns true if the client has the given key in the parameters.

#### string Request.GetHeader(key)
returns the value of the given key from the header.

#### string Request:GetParam(key)
returns the value of the given key from the parameters.

#### string Request:GetBody()
The body of the HTTP Request.

#### string Request:GetRemoteAddr()
the IP Address of the Person who sent the HTTP Request

#### number Request:GetRemotePort()
The Port the HTTP request was received from.

#### string Request:GetLocalAddr()

#### number Request:GetLocalPort()

#### string Request:GetMethod()
The HTTP Method that was used like GET or PUT.

#### number Request:GetAuthorizationCount()

#### number Request:GetContentLength()
The length of the HTTP Request content.

## Response

#### Response:SetContent(content, content-type)
Sets the content like this:
```lua
Response.SetContent("Hello World", "text/plain")
```
#### Response:SetRedirect(url, code)
Redirects one to the given URL and returns the given code.

#### Response:SetHeader(key, value)
Sets the given value for the given key in the header.

#### table Response:GetTable()
Returns the lua table.