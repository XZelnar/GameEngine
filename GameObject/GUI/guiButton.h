#pragma once

#include "../../stdafx.h"
#include "../gameObject.h"
#include "../../Scene/sceneInterfaces.h"

class Texture;
class ComponentGUIRenderer;

class GuiButton : public GameObject
{
public:
	GuiButton();
	void SetButtonPressedEvent(IGuiButtonEvents* _eventsObject, void (IGuiButtonEvents::* _onButtonPressed)(int));
	void SetTexture(Texture* _textureNormal, Texture* _texturePressed, float _border, float _borderPix);
	void SetPosition(const float& x, const float& y);
	void SetSize(const float& w, const float& h);
	virtual void PostInput();
	unsigned int GetID() { return id; }

private:
	bool wasPressed;
	unsigned int id;
	Texture* textureNormal;
	Texture* texturePressed;
	IGuiButtonEvents* eventsObject;
	void (IGuiButtonEvents::* onButtonPressed)(int);
	ComponentGUIRenderer* renderer;

private:
	static unsigned int CurID;
};