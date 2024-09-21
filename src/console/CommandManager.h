#pragma once

#include <string>
#include <map>
#include <functional>
#include <memory>

#include "modules/Module.h"
#include "modules/Log.h"

class CommandManager : public Module
{
private:
	using THandler = std::function<void(const std::string&)>;

	std::unordered_map<std::string, THandler> m_commands;
	std::shared_ptr<Log> m_log;

public:
	CommandManager();

	// Registers a command with a handler
	void Register(const std::string& name, THandler handler);

	// Executes a command string
	void Execute(const std::string& str);
};