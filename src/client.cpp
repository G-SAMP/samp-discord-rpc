#include <memory>
#include <Ws2tcpip.h>
#include <cstring>

#include "client.h"
#include "logger.h"
#include "http.h"

namespace SAMP
{
	bool readServerData(const char *cmdline, ServerData &data)
	{
		auto input = std::unique_ptr<char>(_strdup(cmdline));
		char *next = nullptr;
		char *token = strtok_s(input.get(), " ", &next);
		while (token != nullptr)
		{
			if (!std::strcmp(token, "-c"))
			{
				if (data.connect != SAMP_CONNECT_UNKNOWN)
				{
					return false;
				}
				data.connect = SAMP_CONNECT_SERVER;
			}
			else if (!std::strcmp(token, "-d"))
			{
				if (data.connect != SAMP_CONNECT_UNKNOWN)
				{
					return false;
				}
				data.connect = SAMP_CONNECT_DEBUG;
			}
			else if (!std::strcmp(token, "-h"))
			{
				if (!next)
				{
					return false;
				}
				data.address = strtok_s(nullptr, " ", &next);
			}
			else if (!std::strcmp(token, "-p"))
			{
				if (!next)
				{
					return false;
				}
				data.port = strtok_s(nullptr, " ", &next);
			}
			else if (!std::strcmp(token, "-n"))
			{
				if (!next)
				{
					return false;
				}
				data.username = strtok_s(nullptr, " ", &next);
			}
			token = strtok_s(nullptr, " ", &next);
		}

		return true;
	}

	std::string ServerData::logoFromStream(std::stringstream &stream, std::string defaultLogo)
	{
		for (std::string line; std::getline(stream, line);)
		{
			auto keyValueDelimiterIndex = line.find('=');
			if (keyValueDelimiterIndex != std::string::npos)
			{
				std::string key = line.substr(0, keyValueDelimiterIndex);
				{
					std::string addressTmp = key;
					std::string portTmp = "7777";
					auto portDelimiterIndex = line.find(':');
					if (portDelimiterIndex != std::string::npos)
					{
						addressTmp = key.substr(0, portDelimiterIndex);
						portTmp = key.substr(portDelimiterIndex + 1);
					}

					addrinfo *result = nullptr;
					addrinfo hints = {0};
					hints.ai_family = AF_INET;

					if (!getaddrinfo(addressTmp.c_str(), nullptr, &hints, &result) && result)
					{
						std::string resolvedAddress = inet_ntoa(reinterpret_cast<sockaddr_in *>(result->ai_addr)->sin_addr);

						if (resolvedAddress == address && portTmp == port)
						{
							defaultLogo = line.substr(keyValueDelimiterIndex + 1);
							break;
						}

						freeaddrinfo(result);
					}
				}
			}
		}

		return defaultLogo;
	}

	std::string ServerData::getServerLogoURL(std::string serverShortName)
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
				  .get("kunaldangi/6e43b0a941b37d04022b0919c5b7c189/raw/2b6d7afe6adeb2c05abb0a4e3f6dbdce642ffa62/samp-server-logo.txt"))
		{
			for (std::string line; std::getline(httpResponseStream, line);)
			{
				Logger::log(LogLevel::Debug, "line: %s", line.c_str());
	
				auto keyValueDelimiterIndex = line.find('=');
				if (keyValueDelimiterIndex != std::string::npos)
				{
					std::string keyTmp = line.substr(0, keyValueDelimiterIndex);
					std::string url = line.substr(keyValueDelimiterIndex + 1);
					if (keyTmp == serverShortName)
					{
						Logger::log(LogLevel::Debug, "keyTmp: %s, url: %s", keyTmp.c_str(), url.c_str());
						return url;
					}
				}
			}
		}

		return "logo";
	}
}
