#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "WindowInclude.h"
#include "ExternIncludes.h"

namespace WinApiFramework
{
	class Keyboard
	{
		// -- fields -- //
	private:
		struct Keys
		{
		private:
			unsigned int keys_number;
			bool *keys;

		public:
			Keys();
			~Keys();

		private:
			bool& operator[](const unsigned char &key);
		public:
			const bool& operator[](const unsigned char &key) const;

			friend class Keyboard;
		};
		Keys keys;

	public:
		struct CharEvent
		{
			enum Type
			{
				CharInput,
				Invalid
			};
			Type type;
			wchar_t character;

			CharEvent()
			{
				type = Type::Invalid;
				character = 0u;
			}
			CharEvent(Type type, wchar_t character)
			{
				this->type = type;
				this->character = character;
			}
		};

		enum Key
		{
			Backspace = 0x08,
			Tab = 0x09,
			Enter = 0x0D,
			Shift = 0x10,
			Control = 0x11,
			Alt = 0x12,
			CapsLock = 0x14,
			Esc = 0x1B,
			Space = 0x20,
			PageUp = 0x21,
			PageDown = 0x22,
			End = 0x23,
			Home = 0x24,
			LeftArrow = 0x25,
			UpArrow = 0x26,
			RightArrow = 0x27,
			DownArrow = 0x28,
			Select = 0x29,
			Print = 0x2A,
			Execute = 0x2B,
			PrintScreen = 0x2C,
			Insert = 0x2D,
			Delete = 0x2E,
			Help = 0x2F,
			Digit0 = 0x30,
			Digit1 = 0x31,
			Digit2 = 0x32,
			Digit3 = 0x33,
			Digit4 = 0x34,
			Digit5 = 0x35,
			Digit6 = 0x36,
			Digit7 = 0x37,
			Digit8 = 0x38,
			Digit9 = 0x39,
			A = 0x41,
			B = 0x42,
			C = 0x43,
			D = 0x44,
			E = 0x45,
			F = 0x46,
			G = 0x47,
			H = 0x48,
			I = 0x49,
			J = 0x4A,
			K = 0x4B,
			L = 0x4C,
			M = 0x4D,
			N = 0x4E,
			O = 0x4F,
			P = 0x50,
			Q = 0x51,
			R = 0x52,
			S = 0x53,
			T = 0x54,
			U = 0x55,
			V = 0x56,
			W = 0x57,
			X = 0x58,
			Y = 0x59,
			Z = 0x5A,
			LeftWindows = 0x5B,
			RightWindows = 0x5C,
			Aplications = 0x5D,
			Sleep = 0x5F,
			Num0 = 0x60,
			Num1 = 0x61,
			Num2 = 0x62,
			Num3 = 0x63,
			Num4 = 0x64,
			Num5 = 0x65,
			Num6 = 0x66,
			Num7 = 0x67,
			Num8 = 0x68,
			Num9 = 0x69,
			Multiply = 0x6A,
			Add = 0x6B,
			Separator = 0x6C,
			Subtract = 0x6D,
			Decimal = 0x6E,
			Divide = 0x6F,
			Colon = 0xBA,
			Quote = 0xDE,
			SquareBrackets = 0xE2,
			ForwardSlash = 0xBF,
			BackSlash = 0xDC,
			Tilda = 0xC0,
			OpeningSquareBracket = 0xDB,
			ClosingSquareBracket = 0xDD,
			Comma = 0xBC,
			Point = 0xBE,
			Dot = 0xBE,
			Plus = 0xBB,
			Equals = 0xBB,
			Minus = 0xBC,
			Underscore = 0xBC,
			F1 = 0x70,
			F2 = 0x71,
			F3 = 0x72,
			F4 = 0x73,
			F5 = 0x74,
			F6 = 0x75,
			F7 = 0x76,
			F8 = 0x77,
			F9 = 0x78,
			F10 = 0x79,
			F11 = 0x7A,
			F12 = 0x7B,
			NumLock = 0x90,
			ScrollLock = 0x91,
			LeftShift = 0xA0,
			RightShift = 0xA1,
			LeftControl = 0xA2,
			RightControl = 0xA3,
			LeftMenu = 0xA4,
			RightMenu = 0xA5,
			Invalid
		};
		struct KeyEvent
		{
			enum Type
			{
				Press,
				Relase,
				Invalid
			};
			Type type;
			Key key;

			KeyEvent()
			{
				type = Type::Invalid;
				key = Key::Invalid;
			}
			KeyEvent(Type type, Keyboard::Key key)
			{
				this->type = type;
				this->key = key;
			}
		};
	private:
		template <class T> struct EventsManager
		{
		private:
			std::queue<T> events;
			const unsigned short buffSize = 32u;
			std::vector<std::function<void(T)>> eventHandlers;
			bool eventHandlersEnabled = true;

		public:
			// -- constructor -- //
			EventsManager() {}
			~EventsManager() {}

		public:
			// -- methods -- //
			void PushEvent(T newEvent)
			{
				// push event to buffer
				events.push(newEvent);
				if (events.size() > buffSize)
					events.pop();

				// call handler function
				if (eventHandlersEnabled)
				{
					for (unsigned int i = 0u; i < eventHandlers.size(); ++i)
					{
						eventHandlers[i](newEvent);
					}
				}
			}
			T GetEvent()
			{
				if (events.size() > 0u)
				{
					T e = events.front();
					events.pop();
					return e;
				}
				else
				{
					return T();
				}
			}
			void ClearBuffer()
			{
				events = std::queue<T>();
			}
			template <class EventReceiver> void AddEventHandler(EventReceiver* receiverObject, void(EventReceiver::*eventFunction)(T))
			{
				using std::placeholders::_1;
				std::function<void(T)> f;
				f = std::bind(eventFunction, receiverObject, _1);
				eventHandlers.push_back(f);
			}
			void RemoveAllEventHandlers()
			{
				eventHandlers.clear();
			}
			void EnableEventHandlers()
			{
				eventHandlersEnabled = true;
			}
			void DisableEventHandlers()
			{
				eventHandlersEnabled = false;
			}
		};
		EventsManager<KeyEvent> keyEvents;
		EventsManager<CharEvent> charEvents;
		bool autorepeat = true;


		// -- constructors -- //
	public:
		Keyboard();
		Keyboard(const Keyboard &keyboard) = delete;
		Keyboard(const Keyboard&& keyboard) = delete;
		~Keyboard();


		// -- operators -- //
	public:
		Keyboard& operator=(const Keyboard &keyboard) = delete;
		Keyboard& operator=(const Keyboard &&keyboard) = delete;


		// -- methods -- //
	private:
		void KeyPress(Keyboard::Key key);
		void KeyRelase(Keyboard::Key key);
		void CharInput(const wchar_t &newChar);

	public:
		bool KeyPressed(Keyboard::Key key) const;
		bool KeyPressed(unsigned char key) const;
		bool KeyRelased(Keyboard::Key key) const;
		bool KeyRelased(unsigned char key) const;


		// -- property fields -- //
	public:
		const Keys& Keys;
		bool& Autorepeat;
		EventsManager<KeyEvent>& KeyEvents;
		EventsManager<CharEvent>& CharEvents;

		// -- friend statements -- //
		friend class Window;
		friend class Framework;
	};
}

#endif // !KEYBOARD_H