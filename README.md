This Project aims to add a HTTPServer to Gmod.

This project uses the [cpp-httplib](https://github.com/yhirose/cpp-httplib) as the HTTPServer.

TODO:
- Move IP Whitelist chec to a Pre-routing handler
- Add a Error handler and function
- Add a Exception handler and function
- Add a Pre-routing handler and function
- Add a Post-routing handler and function

# Functions
## Basic Functions
#### httpserver.Start(String IP, Number Port)
This will start or restart the HTTP Server, and it will listen on the given address + port.  
NOTE: If a Method function was called like httpserver.Get after httpserver.Start was called, you need to call httpserver.Start again!
#### httpserver.Stop()
This stops the HTTP Server.
#### (internal function!) httpserver.Think()
This is internally used to manage all requests and to call all functions needed. (Never call this yourself.)

## Method Functions
All Method functions add a listener for the given path and the given method, like this:
```lua
httpserver.Get("/public", function(_, response) 
  print("Public GET request") 
  response.SetContent("You sent a GET request to a public site.", "text/plain", false)
end)

httpserver.Get("/private", function(_, response) 
  print("Public GET request") 
  response.SetContent("You sent a GET request to a private site.", "text/plain", true)
end)
```

If you enable the IP Whitelist, only requests sent by connected players are processed.

#### httpserver.Get(String path, function (Request, Response), bool ipwhitelist)
#### httpserver.Put(String path, function (Request, Response), bool ipwhitelist)
#### httpserver.Post(String path, function (Request, Response), bool ipwhitelist)
#### httpserver.Patch(String path, function (Request, Response), bool ipwhitelist)
#### httpserver.Delete(String path, function (Request, Response), bool ipwhitelist)
#### httpserver.Options(String path, function (Request, Response), bool ipwhitelist)

## Additional Functions
#### httpserver.IsRunning()
Returns true if the HTTPServer is running.
#### httpserver.SetTCPnodelay(bool nodelay)
Sets whether a delay should be added to tcp or not.
#### httpserver.SetReadTimeout(int sec, int usec)
Sets the maximum amount of time before a read times out.
#### httpserver.SetWriteTimeout(int sec, int usec)
Sets the maximum amount of time before a write times out.
#### httpserver.SetPayloadMaxLength(int maxlength)
Sets the maximum payload length.
#### httpserver.SetKeepAliveTimeout(int sec)
Sets the maximum time a connection is kept alive.
#### httpserver.SetKeepAliveMaxCount(int amount)
Sets the maximum amount of connections that can be kept alive at the same time.
#### httpserver.SetMountPoint(string path, string folder)
This mounts the given folder to the given path.
(You can call this multiple times for the same path to mount multiple folders to it.)
#### httpserver.RemoveMountPoint(string path)
This removes all mounts for the given path.

## Request
#### Request.body
The body of the HTTP Request.
#### Request.remote_addr
the IP Address of the Person who sent the HTTP Request
#### Request.remote_port
The Port the HTTP request was received from.
#### Request.local_addr
#### Request.local_port
#### Request.method
The HTTP Method that was used like GET or PUT.
#### Request.authorization_count
#### Request.content_length
The length of the HTTP Request content.
#### Request.HasHeader(key)
returns true if the client has the given key in the header.
#### Request.HasParam(key)
returns true if the client has the given key in the parameters.
#### Request.GetHeader(key)
returns the value of the given key from the header.
#### Request.GetParam(key)
returns the value of the given key from the parameters.

## Response
#### Response.SetContent(content, content-type)
Sets the content like this:
```lua
Response.SetContent("Hello World", "text/plain")
```
#### Response.SetRedirect(url, code)
Redirects one to the given URL and returns the given code.
#### Response.SetHeader(key, value)
Sets the given value for the given key in the header.