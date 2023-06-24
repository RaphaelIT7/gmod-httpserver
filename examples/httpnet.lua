--[[
	This Example shows an alternative way to network huge amounts of data, and it provides a small library to manage the data.
	This adds:
		httpnet.SetData(ply, key, data) -- Allow the client to download the given data by requesting the key.
		httpnet.GetData(ply, key) -- returns the data set for the given key.

		(Clientside)
		httpnet.Download(key, callback) -- Make a GET request to download the given data.
]]

if CLIENT then -- Put this code into a seperate file only the client executes.
	httpnet = {}
	httpnet.Port = 32039
	httpnet.Url = "http://" .. string.Split(game.GetIPAddress(), ":")[1] .. ":" .. httpnet.Port .. "/httpnet2"
	function httpnet.Download(key, callback)
		if !key or !callback then return end
		HTTP({
			success = function(code, body)
		    	callback(body)
		    end, 
		    failure = function(reason)
		    	print("HTTPNet Failed!", reason)
		    end,
		    method = "GET",
		    url = httpnet.Url,
		    parameters = {
		    	["httpnet"] = key
		    }
		})
	end

	net.Receive("HTTPNet_Example", function()
		httpnet.Download(net.ReadString(), function(body)
			PrintTable(util.JSONToTable(body))
		end)
	end)
end

require("httpserver")

local ply_data = {}
httpserver.Get("/httpnet2", function(request, response)
	if request.HasParam("httpnet") then
		local tbl = ply_data[request.remote_addr]
		if tbl then
			response.SetContent(tbl[request.GetParam("httpnet")] or "", "text/plain")
			return
		end
	end

	response.SetContent("", "text/plain")
end)

httpserver.Start("0.0.0.0", 32039) -- Change the Port to one you have open & unused.

httpnet = {}
function httpnet.SetData(ply, key, data)
	if !ply or ply == NULL then return end
	if istable(data) then
		data = util.TableToJSON(data)
	end

	local ip = string.Split(ply:IPAddress(), ":")[1]
	if !ply_data[ip] then
		ply_data[ip] = {}
	end

	ply_data[ip][key] = data
end

function httpnet.GetData(ply, key)
	if !ply or ply == NULL or !key then return end
	return ply_data[string.Split(ply:IPAddress(), ":")[1]][key]
end

util.AddNetworkString("HTTPNet_Example")
concommand.Add("httpnet_example", function(ply)
	if !ply then return end -- Console. No.
	httpnet.SetData(ply, "Example", {
		["Hello"] = "World",
		["This"] = "is a Example"
	})
	net.Start("HTTPNet_Example")
		net.WriteString("Example")
	net.Send(ply)
end)