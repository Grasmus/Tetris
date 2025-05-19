#include "SettingsElement.h"
#include "Strings.h"

namespace GameNamespace
{
	SettingsElement::SettingsElement(
		SDL_Point position,
		int width,
		SDL_Renderer* renderer,
		SettingsType settingType,
		const char* settingName,
		std::vector<std::string> values,
		int currentValueIndex,
		TTF_Font* font,
		SDL_Color textColor
	) :
		position(position),
		width(width),
		renderer(renderer),
		settingName(settingName),
		settingType(settingType),
		values(values),
		textColor(textColor),
		textFont(font),
		currentValueIndex(currentValueIndex)
	{
		CreateSettingsTextView();
		CreateRightButton();
		CreateCurrentValueButton();
		CreateLeftButton();
	}

	SettingsElement::~SettingsElement() {}

	void SettingsElement::Render(SDL_Renderer* renderer)
	{
		settingsTextView->Render(renderer);
		leftButton->Render(renderer);
		rightButton->Render(renderer);
		valueTextView->Render(renderer);
	}

	bool SettingsElement::HandleMouseLeftClick(SDL_Point pressPoint)
	{
		if (leftButton->HandleMouseLeftClick(pressPoint) && currentValueIndex)
		{
			currentValueIndex--;
		}
		else if (rightButton->HandleMouseLeftClick(pressPoint) 
				&& 
				currentValueIndex + 1 < values.size())
		{
			currentValueIndex++;
		}
		else
		{
			return false;
		}

		CreateCurrentValueButton();
		CreateLeftButton();

		return true;
	}

	SettingsType SettingsElement::GetSettingType() const
	{
		return settingType;
	}

	int SettingsElement::GetValueIndex() const
	{
		return currentValueIndex;
	}

	SDL_Rect SettingsElement::CalcTextDimensions(TTF_Font* font, const char* text)
	{
		int textWidth{}, textHeight{};

		if (TTF_SizeText(font, text, &textWidth, &textHeight))
		{
			throw TTFSizeTextException();
		}

		return { 0, 0, textWidth, textHeight };
	}

	void SettingsElement::CreateLeftButton()
	{
		leftButtonRect = CalcTextDimensions(textFont, BUTTON_LEFT_TEXT);

		leftButtonRect.x = currentValueRect.x - leftButtonRect.w;

		if (settingsTextViewRect.w > width / 2)
		{
			leftButtonRect.y = position.y + settingsTextViewRect.h;
		}
		else
		{
			leftButtonRect.y = position.y;
		}

		leftButton = std::unique_ptr<Button>(
			new Button(
				leftButtonRect,
				renderer,
				BUTTON_LEFT_TEXT,
				textFont,
				textColor,
				nullptr
			)
		);
	}

	void SettingsElement::CreateRightButton()
	{
		rightButtonRect = CalcTextDimensions(textFont, BUTTON_RIGHT_TEXT);

		rightButtonRect.x = position.x + width - rightButtonRect.w;

		if (settingsTextViewRect.w > width / 2)
		{
			rightButtonRect.y = position.y + settingsTextViewRect.h;
		}
		else
		{
			rightButtonRect.y = position.y;
		}

		rightButton = std::unique_ptr<Button>(
			new Button(
				rightButtonRect,
				renderer,
				BUTTON_RIGHT_TEXT,
				textFont,
				textColor,
				nullptr
			)
		);
	}

	void SettingsElement::CreateSettingsTextView()
	{
		settingsTextViewRect = CalcTextDimensions(textFont, settingName);

		settingsTextViewRect.x = position.x;
		settingsTextViewRect.y = position.y;

		settingsTextView = std::unique_ptr<TextView>(
			new TextView(
				settingsTextViewRect,
				textFont,
				renderer,
				textColor,
				settingName
			)
		);
	}

	void SettingsElement::CreateCurrentValueButton()
	{
		currentValueRect = CalcTextDimensions(textFont, values[currentValueIndex].c_str());

		currentValueRect.x = rightButtonRect.x - currentValueRect.w;

		if (settingsTextViewRect.w > width / 2)
		{
			currentValueRect.y = position.y + settingsTextViewRect.h;
		}
		else
		{
			currentValueRect.y = position.y;
		}

		valueTextView = std::unique_ptr<TextView>(
			new TextView(
				currentValueRect,
				textFont,
				renderer,
				textColor,
				values[currentValueIndex].c_str()
			)
		);
	}
}
