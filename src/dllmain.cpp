#include <process.h>
#include "client.h"
#include "discord.h"
#include "query.h"
#include "http.h"
#include "logger.h"

static void process(void *)
{
	SAMP::ServerData data;
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	Discord::initialize();
	if (SAMP::readServerData(GetCommandLine(), data))
	{
		std::string logo = "logo";
		{
			std::stringstream httpResponseStream;
			if (
				 HTTP::WebRequest(
					  [&httpResponseStream](auto data, auto len)
					  {
						  httpResponseStream.write(data, len);
						  return true;
					  },
					//   "Mozilla/5.0", "raw.githubusercontent.com", INTERNET_DEFAULT_HTTPS_PORT)
					//   .get("Hual/samp-discord-plugin/custom-logos/custom-logos.txt"))
					  "Mozilla/5.0", "gist.githubusercontent.com", INTERNET_DEFAULT_HTTPS_PORT)
					  .get("kunaldangi/437bd36855da1d70fff8944bd8146dd9/raw/8466cb8059a241cdf7bda6bda4936cdd30a26741/custom-logos.txt"))
			{
				logo = data.logoFromStream(httpResponseStream, logo);
			}
		}
		
		auto start = std::time(0);
		if (data.connect == SAMP::SAMP_CONNECT_SERVER)
		{
			SAMP::Query query(data.address, std::stoi(data.port));
			while (true)
			{
				SAMP::Query::Information information;
				if (query.info(information))
				{
					auto fullAddress = data.address + ':' + data.port;
					Logger::log(LogLevel::Debug, "Connected to %s", fullAddress.c_str());
					auto players = std::to_string(information.basic.players) + "/" + std::to_string(information.basic.maxPlayers) + " players online";
					auto info = "Playing " + information.gamemode + " as " + data.username + " in " + information.language;
					auto image = logo;
					Logger::log(LogLevel::Debug, "Logo: %s, Image: %s", logo.c_str(), image.c_str());
					
					if (image == "logo")
					{
						if (information.basic.password)
						{
							image = "lock";
						}
						else if (information.basic.players < 10)
						{
							image = "tumbleweed";
						}
					}

					auto logoURL = data.getServerLogoURL(image); // hosted logo urls

					Discord::update(start, fullAddress, information.hostname, logoURL, info, players);
					Sleep(15000 - QUERY_DEFAULT_TIMEOUT * 2);
				}
			}
		}
		else if (data.connect == SAMP::SAMP_CONNECT_DEBUG)
		{
			while (true)
			{
				Discord::update(start, "localhost", "Debug server", "tumbleweed", "Playing debug mode in English", "Most likely 1 player online as it's debug mode");
				Sleep(15000);
			}
		}
	}
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	{
		DisableThreadLibraryCalls(module);

		Logger::init("samp-discord-rpc.log");
		Logger::log(LogLevel::Info, "====== SAMP Discord RPC Plugin Loaded ======");

		_beginthread(&process, 0, nullptr);
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		WSACleanup();
		break;
	}
	}
	return TRUE;
}
