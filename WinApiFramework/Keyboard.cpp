#include "keyboard.h"

namespace WinapiFramework
{
	// ~~~~~~~~ [STRUCT] Keyboard::Keys ~~~~~~~~
	Keyboard::Keys::Keys()
		: keys_number(256)
	{
		keys = new bool[keys_number];
		for (unsigned int i = 0; i < keys_number; i++) keys[i] = false;
	}
	Keyboard::Keys::~Keys()
	{
		delete keys;
	}
	bool& Keyboard::Keys::operator[](const unsigned char& key)
	{
		return keys[key];
	}
	const bool& Keyboard::Keys::operator[](const unsigned char& key) const
	{
		if (key >= 0 && (key < keys_number))
			return keys[key];
		return keys[0];
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



	// ~~~~~~~~ [CLASS] Keyboard ~~~~~~~~
	Keyboard::Keyboard()
		: Keys(keys)
		, Autorepeat(autorepeat)
	{
	}
	Keyboard::~Keyboard()
	{

	}

	LRESULT Keyboard::HandleMessage(WPARAM wParam, LPARAM lParam)
	{
		MSG *msg = (MSG*)lParam;
		switch (msg->message)
		{
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
			{
				if (!(msg->lParam & 0x40000000) || autorepeat)
				{
					Keyboard::Key key = (Keyboard::Key)msg->wParam;
					keys[static_cast<unsigned char>(key)] = true;
					RaiseEventByHandler<Events::EventKeyPress>(key);
				}
				break;
			}

			case WM_KEYUP:
			case WM_SYSKEYUP:
			{
				Keyboard::Key key = (Keyboard::Key)msg->wParam;
				keys[static_cast<unsigned char>(key)] = false;
				RaiseEventByHandler<Events::EventKeyRelease>(key);
			}

			case WM_CHAR:
			case WM_SYSCHAR:
			{
				if (!(msg->lParam & 0x40000000) || autorepeat)
				{
					wchar_t c = (wchar_t)msg->wParam;
					RaiseEventByHandler<Events::EventCharInput>(c);
				}
			}


			default: return 1;
		}

		return 0;
	}

	bool Keyboard::KeyPressed(Keyboard::Key key) const
	{
		return keys[static_cast<unsigned char>(key)];
	}
	bool Keyboard::KeyPressed(unsigned char key) const
	{
		return keys[key];
	}
	bool Keyboard::KeyRelased(Keyboard::Key key) const
	{
		return !keys[static_cast<unsigned char>(key)];
	}
	bool Keyboard::KeyRelased(unsigned char key) const
	{
		return keys[key];
	}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
}