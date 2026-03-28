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
}
