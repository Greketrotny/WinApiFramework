#ifndef COLOR_H
#define COLOR_H

namespace Graphics
{
	class Color
	{
		// -- fields -- //
	private:
		unsigned int color;

		// static colors
		static const Color White, Red, Green, Blue, Yellow, Purple, Aqua, Black;

		// -- constructor -- //
	public:
		Color();
		Color(const Color &color);
		Color(unsigned int color);
		Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 0xFF);

		// -- operators -- //
	public:
		Color& operator=(const Color& color);
		Color& operator=(Color&& color);


		// -- methods -- //
	public:
		unsigned char GetA() const;
		unsigned char GetR() const;
		unsigned char GetG() const;
		unsigned char GetB() const;
		unsigned int GetColor() const;
		void SetA(unsigned char alpha);
		void SetR(unsigned char red);
		void SetG(unsigned char green);
		void SetB(unsigned char blue);
		void SetColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 0xFF);
	};
}

#endif // !COLOR_H