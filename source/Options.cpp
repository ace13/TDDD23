#include "Options.hpp"
#include "Application.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>

Options::Options(Application& app) : mApp(app)
{
	registerProperty<bool>([this](const bool&) {
		auto getName = [](const std::type_index& id) -> std::string {
			if (id == typeid(int))
				return "int";
			if (id == typeid(float))
				return "float";
			if (id == typeid(bool))
				return "bool";
			if (id == typeid(std::string))
				return "string";
			return "error";
		};

		std::cout << "Usage: " << mApp.getApplicationName() << " [OPTION]..." << std::endl
			<< "Options:" << std::endl;
		for (auto& i : mStored)
		{
			std::cout << "  --" << std::setw(4) << std::left << i.first << "  \t" << i.second.Description << " (" << getName(i.second.Type) << ")" << std::endl;
		}

		exit(0);
	}, []() { return false; }, "help", "Show this text");
}

Options::~Options()
{
	for (auto& i : mStored)
	{
		CVAR& cv = i.second;
		
		if (cv.Type == typeid(bool))
		{
			delete (std::function<void(const bool&)>*)cv.Set;
			delete (std::function<bool()>*)cv.Get;

			if (cv.Opaque)
				delete (bool*)cv.Opaque;
		}
		else if (cv.Type == typeid(int))
		{
			delete (std::function<void(const int&)>*)cv.Set;
			delete (std::function<int()>*)cv.Get;

			if (cv.Opaque)
				delete (int*)cv.Opaque;
		}
		else if (cv.Type == typeid(float))
		{
			delete (std::function<void(const float&)>*)cv.Set;
			delete (std::function<float()>*)cv.Get;

			if (cv.Opaque)
				delete (float*)cv.Opaque;
		}
		else if (cv.Type == typeid(std::string))
		{
			delete (std::function<void(const std::string&)>*)cv.Set;
			delete (std::function<std::string()>*)cv.Get;

			if (cv.Opaque)
				delete (std::string*)cv.Opaque;
		}

		cv.Get = nullptr;
		cv.Set = nullptr;
		cv.Opaque = nullptr;
	}
}


void Options::parseARGV(int argc, char** argv)
{
	int i = 1;

	auto handle = [&](CVAR& cv)
	{
		typedef std::pair<std::type_index, std::function<bool(const char*)> > kv;
		std::unordered_map<std::type_index, std::function<bool(const char*)> > isType {
			kv { typeid(int), [](const char* str) -> bool { int i; return sscanf(str, "%8i", &i) == 1; } },
			kv { typeid(bool), [](const char* str) -> bool {
				int i;
				if (sscanf(str, "%1i", &i) == 1) return true;
				else if (strcmp(str, "false") == 0) return true;
				else if (strcmp(str, "true") == 0) return true;
				return false;
			} },
			kv { typeid(float), [](const char* str) -> bool { float f; return sscanf(str, "%8f", &f) == 1; } },
			kv { typeid(std::string), [](const char*) -> bool { return true; } }
		};

		if (argv[i + 1] == nullptr)
		{
			if (cv.Type != typeid(bool))
				mApp.getLogger().log("Option '%s' requires an argument, skipping.", Logger::Warning, cv.Name.c_str());
			else
			{
				auto set = *(std::function<void(const bool&)>*)cv.Set;
				set(true);
			}
		}
		else if (isType[cv.Type](argv[++i]))
		{
			if (cv.Type == typeid(int))
			{
				auto set = *(std::function<void(const int&)>*)cv.Set;
				set(atoi(argv[i]));
			}
			else if (cv.Type == typeid(bool))
			{
				auto set = *(std::function<void(const bool&)>*)cv.Set;
				int i = 0;
				if (sscanf(argv[i], "%1i", &i) == 1)
					set(i > 0);
				else if (strcmp(argv[i], "true"))
					set(true);
				else
					set(false);
			}
			else if (cv.Type == typeid(float))
			{
				auto set = *(std::function<void(const float&)>*)cv.Set;
				set(atof(argv[i]));
			}
			else if (cv.Type == typeid(std::string))
			{
				auto set = *(std::function<void(const std::string&)>*)cv.Set;
				set(argv[i]);
			}
		}
	};

	for (; i < argc; ++i)
	{
		std::string str = argv[i];

		if (str[0] == '-')
		{
			if (str[1] == '-') // Long name style
			{
				str = str.substr(2, str.length() - 2);
				if (str.find('=') != std::string::npos)
				{
					mApp.getLogger().log("'%s', doesn't support = assignment for the moment, skipping.", Logger::Warning, str.c_str());
					continue;
				}

				for (auto& j : mStored)
				{
					if (j.first == str)
					{
						handle(j.second);
						break;
					}
				}
			}
			else // Short name style
			{
				for (unsigned int j = 1; j < str.length(); ++j)
				{
					char c = str[j];

					for (auto& k : mStored)
					{
						if (k.second.Name[0] == c)
						{
							handle(k.second);
							break;
						}
					}
				}
			}
		}
	}
}

void Options::loadConfig(const std::string& file)
{

}


void Options::registerCVAR(const CVAR& t)
{
	assert(t.Type == typeid(bool) || t.Type == typeid(int) || t.Type == typeid(float) || t.Type == typeid(std::string));

    mStored[t.Name] = t;
}

const CVAR& Options::getCVAR(const std::string& name) const
{
	return mStored.at(name);
}