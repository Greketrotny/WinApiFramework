#include "keyboard.h"

namespace WinapiFramework
{
	Keyboard::Keyboard()
		:Keys(keys),
		Autorepeat(autorepeat),
		KeyEvents(keyEvents),
		CharEvents(charEvents)
	{

	}
	Keyboard::~Keyboard()
	{

	}

	void Keyboard::KeyPress(Keyboard::Key key)
	{
		keys[key] = true;
		keyEvents.PushEvent(KeyEvent(Keyboard::KeyEvent::Type::Press, key));
	}
	void Keyboard::KeyRelase(Keyboard::Key key)
	{
		keys[key] = false;
		keyEvents.PushEvent(KeyEvent(Keyboard::KeyEvent::Type::Relase, key));
	}
	void Keyboard::CharInput(const wchar_t &newChar)
	{
		charEvents.PushEvent(CharEvent(Keyboard::CharEvent::Type::CharInput, newChar));
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
}