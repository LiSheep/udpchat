#include "client.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <merror.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define T Client

#define lua_errquit() printf("%s\n", lua_tostring(L, -1)); exit(-1)

void client_tojson(T client, char* str, size_t len){
	assert(client);
	//id,name,sockadd,socklen
	snprintf(str, len, "{\"id\":%d, \"addr\":\"%s\", \"port\":%d, \"name\":\"%s\"}", 
			/*client->id*/ 0, client->addr, client->port, client->name);
}

void client_toobj(char *str, T cli){
	assert(str);
	assert(cli);
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);
	if(luaL_loadfile(L, "jsontrans.lua") || lua_pcall(L, 0, 0, 0)){
		lua_errquit();
		exit(-1);
	}
	lua_getglobal(L, "decode");
	if(!lua_isfunction(L, -1)){
		err_quit("lua: decode not function");
	}
	lua_pushstring(L, str);

	if(lua_pcall(L, 1, 4, 0) != 0){
		lua_errquit();
	}
	if(!lua_isnumber(L, -4))
		err_quit("json trans error");
	cli->id = (int)lua_tonumber(L, -4);

	if(!lua_isstring(L, -3))
		err_quit("json trans error");
	size_t len = NAME_LEN;
	strcpy(cli->name, lua_tolstring(L, -3, &len));

	if(!lua_isstring(L, -2))
		err_quit("json trans error");
	len = ADDR_LEN;
	strcpy(cli->addr, lua_tolstring(L, -2, &len));

	if(!lua_isnumber(L, -1))
		err_quit("json trans error");
	cli->port = (int)lua_tonumber(L, -1);
}

void client_getClient(char *key, T cli){
	assert(cli);
	assert(key);
}

void client_getKey(T cli, char *key){

}

#undef T
