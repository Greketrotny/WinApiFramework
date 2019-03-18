#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <queue>


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
	private:
		std::queue<CharEvent> charEvents;

	public:
		struct KeyEvent
		{
			enum Type
			{
				Press,
				Relase,
				Invalid
			};
			Type type;
			unsigned char code;

			KeyEvent()
			{
				type = Type::Invalid;
				code = 0u;
			}
			KeyEvent(Type type, unsigned char code)
			{
				this->type = type;
				this->code = code;
			}
		};
	private:
		std::queue<KeyEvent> keyEvents;

		bool autorepeat = false;
		const unsigned short buffLength = 32u;


		// -- constructors -- //
	public:
		Keyboard();
		Keyboard(const Keyboard &keyboard) = delete;
		~Keyboard();


		// -- operators -- //
	public:
		Keyboard& operator=(const Keyboard &keyboard) = delete;


		// -- methods -- //
	private:
		void KeyPress(const unsigned char &Key);
		void KeyRelase(const unsigned char &Key);
		void CharInput(const wchar_t &newChar);
		void TrimKeyBuffer();
		void TrimCharBuffer();

	public:
		bool KeyPressed(const unsigned char &key) const;
		bool KeyRelased(const unsigned char &key) const;
		void ClearBuffers();

		CharEvent GetCharEvent();
		bool IsCharEmpty();
		void ClearCharBuffer();

		KeyEvent GetKeyEvent();
		bool IsKeyEmpty();
		void ClearKeyBuffer();



		// -- property fields -- //
	public:
		const Keys& Keys;
		bool& Autorepeat;


		// -- friend statements -- //
		friend class Window;
	};
}

#endif // !KEYBOARD_H
