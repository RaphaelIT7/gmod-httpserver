This Project aims to add a HTTPServer to Gmod.

This project uses the [cpp-httplib](https://github.com/yhirose/cpp-httplib) as the HTTPServer.
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
httpserver.Get("/", function(_, response) 
  print("GET request") 
  response.SetContent("You sent a GET request.", "text/plain")
end)
```
#### httpserver.Get(String path, function (Request, Response))
#### httpserver.Put(String path, function (Request, Response))
#### httpserver.Post(String path, function (Request, Response))
#### httpserver.Patch(String path, function (Request, Response))
#### httpserver.Delete(String path, function (Request, Response))
#### httpserver.Options(String path, function (Request, Response))

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