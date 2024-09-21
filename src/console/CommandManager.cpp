#include "CommandManager.h"

#include <stdexcept>

#include "Hook.h"

CommandManager::CommandManager() : m_commands()
{
	m_log = Hook::GetInstance().GetModule<Log>();
}

void CommandManager::Register(const std::string& name, THandler handler)
{
	if (m_commands.find(name) != m_commands.end())
	{
		throw std::invalid_argument("A command with the same name has already been registered");
	}

	m_commands.insert({ name, handler });
}

void CommandManager::Execute(const std::string& str)
{
	// Get the command name from the string
	auto pos = str.find(" ");
	auto name = pos == std::string::npos ? str : str.substr(0, pos);

	if (m_commands.find(name) == m_commands.end())
	{
		m_log->WriteLine("No such command");
		return;
	}

	auto command = m_commands.find(name);

	// Execute the command
	command->second(pos == std::string::npos ? "" : str.substr(pos + 1));
}