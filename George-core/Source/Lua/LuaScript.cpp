#include "LuaScript.h"
#include <lua.hpp>
#include<luaconf.h>


namespace george
{
	void LuaScript::PrintError(const std::string& variableName, const std::string& reason)
	{
		std::cout << "Error: can't get [" << variableName << "]. " << reason << std::endl;
	}
	
	std::vector<std::string> LuaScript::getTableKeys(const std::string& name) {
		const char* code =
			"function getKeys(name) "
			"s = \"\""
			"for k, v in pairs(_G[name]) do "
			"    s = s..k..\",\" "
			"    end "
			"return s "
			"end"; // function for getting table keys
		luaL_loadstring(L, code); // execute code
		lua_pcall(L, 0, 0, 0);
		lua_getglobal(L, "getKeys"); // get function
		lua_pushstring(L, name.c_str());
		lua_pcall(L, 1, 1, 0); // execute function
		std::string test = lua_tostring(L, -1);
		std::vector<std::string> strings;
		std::string temp = "";
		std::cout << "TEMP:" << test << std::endl;
		for (unsigned int i = 0; i < test.size(); i++) {
			if (test.at(i) != ',') {
				temp += test.at(i);
			}
			else {
				strings.push_back(temp);
				temp = "";
			}
		}
		clean();
		return strings;
	}

	bool LuaScript::lua_gettopstack(const std::string& variableName)
	{
		level = 0;
		std::string var = "";
		for (unsigned int i = 0; i < variableName.size(); i++) {
			if (variableName.at(i) == '.') {
				if (level == 0) {
					lua_getglobal(L, var.c_str());
				}
				else {
					lua_getfield(L, -1, var.c_str());
				}

				if (lua_isnil(L, -1)) {
					PrintError(variableName, var + " is not defined");
					return false;
				}
				else {
					var = "";
					level++;
				}
			}
			else {
				var += variableName.at(i);
			}
		}
		if (level == 0) {
			lua_getglobal(L, var.c_str());
		}
		else {
			lua_getfield(L, -1, var.c_str());
		}
		if (lua_isnil(L, -1)) {
			PrintError(variableName, var + " is not defined");
			return false;
		}

		return true;
	}


}
