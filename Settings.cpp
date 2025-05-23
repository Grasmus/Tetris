#include "Settings.h"
#include "GameExceptions.h"

namespace GameNamespace
{
	Settings::Settings(const char* settingsFileName): 
		settingsFileName(settingsFileName)
	{
		ReadSettings();
	}

	Settings::~Settings() { }

	std::string Settings::GetSetting(SettingsType setting)
	{
		const char* settingName = ConvertSettingsType(setting);

		if (settings.find(settingName) == settings.end())
		{
			throw SettingNotFoundException();
		}
		else
		{
			return settings[settingName];
		}
	}

	void Settings::SetSetting(SettingsType setting, const char* value)
	{
		const char* settingName = ConvertSettingsType(setting);

		if (settings.find(settingName) == settings.end())
		{
			throw SettingNotFoundException();
		}
		else
		{
			settings[settingName] = value;
		}
	}

	void Settings::Save()
	{
		std::ofstream settingsFile(settingsFileName, std::ofstream::trunc);

		if (!settingsFile)
		{
			throw SettingsFileNameException();
		}

		for (auto &setting : settings)
		{
			settingsFile << setting.first << " = " << setting.second << std::endl;
		}

		settingsFile.close();
	}

	void Settings::ReadSettings()
	{
		std::ifstream settingsFile(settingsFileName);

		if (!settingsFile)
		{
			throw SettingsFileNameException();
		}

		std::string readLine{};

		while (std::getline(settingsFile, readLine))
		{
			std::istringstream stringStream(readLine);

			std::string settingName{}, equalSign{}, settingValue{};

			// convert read line to values and check if it's successful
			if ((stringStream >> settingName >> equalSign >> settingValue))
			{
				if (equalSign.compare("=") == 0)
				{
					settings[settingName] = settingValue;
				}
				else
				{
					settingsFile.close();

					throw SettingsInvalidSyntaxException();
				}
			}
		}

		settingsFile.close();
	}

	const char* Settings::ConvertSettingsType(SettingsType setting)
	{
		switch (setting)
		{
		case SettingsType::ScreenResolution:
			return "ScreenResolution";

		default:
			throw SettingsInvalidTypeException();
		}
	}
}
