#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include "Hooking.hpp"

namespace mh
{
	NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config,
		remove_legal_screen,
		remove_quit_message,
		hide_menu_on_start);
};

mh::Config& Hooking::GetConfig()
{
	return m_config;
}

void Hooking::LoadConfig()
{
	nlohmann::json j;

	try
	{
		std::ifstream ifs("config.json");

		//ifs >> j;
		j = nlohmann::json::parse(ifs, nullptr, true, true);

		ifs.close();
	}
	catch (std::exception& e)
	{
		return;
	}

	try
	{
		m_config = j.get<mh::Config>();
	}
	catch (std::exception& e)
	{
		std::ostringstream text;
		text << e.what() << "\nThe default configuration will be used, Press 'OK' to continue.";

		MessageBoxA(nullptr, text.str().c_str(), "Could not load configuration", MB_OK | MB_ICONWARNING);
	}
}