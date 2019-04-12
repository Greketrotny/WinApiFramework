#include "Keyboard.h"

using namespace WinApiFramework;

// [CLASS] Keyboard ----------------------------|
// -- constructor -- //
Keyboard::Keyboard()
	:Keys(keys),
	Autorepeat(autorepeat)
{

}
Keyboard::~Keyboard()
{

}

// -- methods -- //
void Keyboard::KeyPress(Keyboard::Key key)
{
	keys[key] = true;
	KeyEvent kev(Keyboard::KeyEvent::Type::Press, key);
	keyEvents.push(kev);
	TrimKeyBuffer();

	if (keyEventHandler) keyEventHandler->HandleEvent(kev);
}
void Keyboard::KeyRelase(Keyboard::Key key)
{
	keys[key] = false;
	KeyEvent kev(Keyboard::KeyEvent::Type::Relase, key);
	keyEvents.push(kev);
	TrimKeyBuffer();

	if (keyEventHandler) keyEventHandler->HandleEvent(kev);
}
void Keyboard::CharInput(const wchar_t &newChar)
{
	CharEvent cev(Keyboard::CharEvent::Type::CharInput, newChar);
	charEvents.push(cev);
	TrimCharBuffer();

	if (charEventHandler) charEventHandler->HandleEvent(cev);
}
void Keyboard::TrimKeyBuffer()
{
	while (keyEvents.size() > buffLength)
	{
		keyEvents.pop();
	}
}
void Keyboard::TrimCharBuffer()
{
	while (charEvents.size() > buffLength)
	{
		charEvents.pop();
	}
}

bool Keyboard::KeyPressed(Keyboard::Key key) const
{
	return keys[key];
}
bool Keyboard::KeyPressed(unsigned char key) const
{
	return keys[key];
}
bool Keyboard::KeyRelased(Keyboard::Key key) const
{
	return !keys[key];
}
bool Keyboard::KeyRelased(unsigned char key) const
{
	return keys[key];
}
void Keyboard::ClearBuffers()
{
	ClearCharBuffer();
	ClearKeyBuffer();
}

Keyboard::CharEvent Keyboard::GetCharEvent()
{
	if (charEvents.size() > 0u)
	{
		Keyboard::CharEvent e = charEvents.front();
		charEvents.pop();
		return e;
	}
	else
	{
		return Keyboard::CharEvent();
	}
}
bool Keyboard::IsCharEmpty()
{
	return charEvents.empty();
}
void Keyboard::ClearCharBuffer()
{
	charEvents = std::queue<CharEvent>();
}

Keyboard::KeyEvent Keyboard::GetKeyEvent()
{
	if (keyEvents.size() > 0u)
	{
		Keyboard::KeyEvent e = keyEvents.front();
		keyEvents.pop();
		return e;
	}
	else
	{
		return Keyboard::KeyEvent();
	}
}
bool Keyboard::IsKeyEmpty()
{
	return keyEvents.empty();
}
void Keyboard::ClearKeyBuffer()
{
	keyEvents = std::queue<KeyEvent>();
}

void Keyboard::SetKeyEventHandler(Keyboard::KeyEventHandler* keh)
{
	this->keyEventHandler = keh;
}
void Keyboard::SetCharEventHandler(Keyboard::CharEventHandler* ceh)
{
	this->charEventHandler = ceh;
}


// Keyboard::Keys struct definition
Keyboard::Keys::Keys()
{
	keys_number = 256;
	keys = new bool[keys_number];
	for (unsigned int i = 0; i < keys_number; i++) keys[i] = false;
}
Keyboard::Keys::~Keys()
{
	delete keys;
}
bool& Keyboard::Keys::operator[](const unsigned char &key)
{
	return keys[key];
}
const bool& Keyboard::Keys::operator[](const unsigned char &key) const
{
	if (key >= 0 && (key < keys_number))
		return keys[key];
	return keys[0];
}
// [CLASS] Keyboard ----------------------------|