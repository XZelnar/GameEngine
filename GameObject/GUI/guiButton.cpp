#include "guiButton.h"

#include "../../Graphics/graphicsResourceManager.h"
#include "../../Input/inputManager.h"
#include "../../Graphics/texture.h"
#include "../Component/componentGUIRenderer.h"

unsigned int GuiButton::CurID = 0;

GuiButton::GuiButton()
{
	onButtonPressed = null;
	id = CurID++;
	wasPressed = false;

	renderer = new ComponentGUIRenderer();
	renderer->SetSize(D3DXVECTOR2(80, 28));
	SetRenderer(renderer);
}

void GuiButton::SetButtonPressedEvent(IGuiButtonEvents* _eventsObject, void (IGuiButtonEvents::* _onButtonPressed)(int))
{
	eventsObject = _eventsObject;
	onButtonPressed = _onButtonPressed;
}

void GuiButton::SetTexture(Texture* _textureNormal, Texture* _texturePressed, float _border, float _borderPix)
{
	textureNormal = _textureNormal;
	texturePressed = _texturePressed;
	renderer->SetTexture(textureNormal);
	renderer->SetBorder(_border, _borderPix);
}

void GuiButton::SetPosition(const float& x, const float& y)
{
	renderer->SetPosition(D3DXVECTOR2(x, y));
}

void GuiButton::SetSize(const float& w, const float& h)
{
	renderer->SetSize(D3DXVECTOR2(w, h));
}

void GuiButton::PostInput()
{
	long x, y;
	InputManager::mGetPosition(x, y);
	if (InputManager::mIsButtonPressed(IM_BUTTON::Left) &&
		x > renderer->GetPosition().x && y > renderer->GetPosition().y && 
		x < renderer->GetPosition().x + renderer->GetSize().x && y < renderer->GetPosition().y + renderer->GetSize().y)//pressed
	{
		if (!wasPressed)
		{
			renderer->SetTexture(texturePressed);
			wasPressed = true;
		}
	}
	else
	{
		if (wasPressed)
		{
			renderer->SetTexture(textureNormal);
			wasPressed = false;
			if (!InputManager::mIsButtonPressed(IM_BUTTON::Left) && onButtonPressed != null)//invoke event
				(eventsObject->*onButtonPressed)(id);
		}
	}

}