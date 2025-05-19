#pragma once
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace GameNamespace
{
	enum class SettingsType
	{
		ScreenResolution
	};

	struct SettingValue
	{
		SettingValue(const char* lable) : lable(lable) {}

		std::string GetLable() const
		{
			return lable;
		}

	private:
		std::string lable{};
	};

	struct ScreenResolution : public SettingValue
	{
		ScreenResolution(const char* lable, int width, int height) : 
			SettingValue(lable), 
			width(width), 
			height(height) {}

		int GetWidth() const
		{
			return width;
		}

		int GetHeight() const
		{
			return height;
		}

	private:
		int width{};
		int height{};
	};

	class Settings
	{
	public:
		Settings(const char* settingsFileName);
		~Settings();

		std::string GetSetting(SettingsType setting);
		void SetSetting(SettingsType setting, const char* value);

		void Save();

	private:
		const char* settingsFileName{};
		std::map<std::string, std::string> settings{};

		void ReadSettings();
		const char* ConvertSettingsType(SettingsType setting);
	};
}
