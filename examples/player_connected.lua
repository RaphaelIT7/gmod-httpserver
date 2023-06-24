require("httpserver")

httpserver.Get("/", function(request, response)
	for k, v in ipairs(player.GetAll()) do
		if string.Split(v:IPAddress(), ":")[1] == request.remote_addr then
			response.SetContent("You are connected to the Server.", "text/plain")
			return
		end
	end

	response.SetContent("You are NOT connected to the Server.", "text/plain")
end)

httpserver.Start("0.0.0.0", 32039) -- Change the Port to one you have open & unused.