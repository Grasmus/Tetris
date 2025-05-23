#pragma once

#include <vector>
#include <string>
#include <SDL_ttf.h>
#include <memory>
#include "UIElement.h"
#include "Button.h"
#include "TextView.h"
#include "Settings.h"

namespace GameNamespace
{
	class SettingsElement : public UIElement
	{
	public:
		SettingsElement(
			SDL_Point position,
			int width,
			SDL_Renderer* renderer,
			SettingsType settingType,
			const char* settingName,
			std::vector<std::string> values,
			int currentValueIndex,
			TTF_Font* font,
			SDL_Color textColor);
		~SettingsElement();

		void Render(SDL_Renderer* renderer);
		bool HandleMouseLeftClick(SDL_Point pressPoint);
		void HandleTextInput(const char* textInput) {};
		void HandleKeyDown(SDL_Keycode keyCode) {};

		SettingsType GetSettingType() const;
		int GetValueIndex() const;

	private:
		const char* settingName{};
		SettingsType settingType{};
		SDL_Point position{};
		int width{};
		std::vector<std::string> values{};
		SDL_Color textColor{};
		TTF_Font* textFont{};
		int currentValueIndex{};
		SDL_Renderer* renderer{};

		std::unique_ptr<Button> leftButton{};
		std::unique_ptr<Button> rightButton{};

		std::unique_ptr<TextView> settingsTextView{};
		std::unique_ptr<TextView> valueTextView{};

		SDL_Rect leftButtonRect{};
		SDL_Rect rightButtonRect{};
		SDL_Rect settingsTextViewRect{};
		SDL_Rect valueTextViewRect{};
		SDL_Rect currentValueRect{};

		SDL_Rect CalcTextDimensions(TTF_Font* font, const char* text);

		void CreateLeftButton();
		void CreateRightButton();
		void CreateSettingsTextView();
		void CreateCurrentValueButton();
	};
}
