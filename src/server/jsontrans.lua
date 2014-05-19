local cjson = require "cjson"

function decode(str)
	local tab = cjson.decode(str);
	return tab.id, tab.name, tab.addr, tab.port;
end

--print(decode("{\"port\":100,\"name\":\"ltc\",\"addr\":\"172.18.99.12\", \"id\":123}"))

