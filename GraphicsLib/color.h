#ifndef COLOR_H
#define COLOR_H

namespace Graphics
{
	class Color
	{
		typedef unsigned char uchar;
	public:
		uchar red, green, blue, alpha;

	public:
		struct Palette
		{
			static const Color
				Maroon, DarkRed, Brown, Firebrick,
				Crimson, Red, Tomato, Coral,
				IndianRed, LightCoral, DarkSalmon, Salmon,
				LightSalmon, OrangeRed, DarkOrange, Orange,
				Gold, DarkGoldenRod, GoldenRod, PaleGoldenRod,
				DarkKhaki, Khaki, Olive, Yellow,
				YellowGreen, DarkOliveGreen, OliveDrab, LawnGreen,
				ChartReuse, GreenYellow, DarkGreen, Green,
				ForestGreen, Lime, LimeGreen, LightGreen,
				PaleGreen, DarkSeaGreen, MediumSpringGreen, SpringGreen,
				SeaGreen, MediumAquaMarine, MediumSeaGreen, LightSeaGreen,
				DarkSlateGray, Teal, DarkCyan, Aqua,
				Cyan, LightCyan, DarkTurquoise, Turquoise,
				MediumTurquoise, PaleTurquoise, AquaMarine, PowderBlue,
				CadetBlue, SteelBlue, CornFlowerBlue, DeepSkyBlue,
				DodgerBlue, LightBlue, SkyBlue, LightSkyBlue,
				MidnightBlue, Navy, DarkBlue, MediumBlue,
				Blue, RoyalBlue, BlueViolet, Indigo,
				DarkSlateBlue, SlateBlue, MediumSlateBlue, MediumPurple,
				DarkMagenta, DarkViolet, DarkOrchid, MediumOrchid,
				Purple, Thistle, Plum, Violet,
				Magenta, Orchid, MediumVioletRed, PaleVioletRed,
				DeepPink, HotPink, LightPink, Pink,
				AntiqueWhite, Beige, Bisque, BlanchedAlmond,
				Wheat, CornSilk, LemonChiffon, LightGoldenRodYellow,
				LightYellow, SaddleBrown, Sienna, Chocolate,
				Peru, SandyBrown, BurlyWood, Tan,
				RosyBrown, Moccasin, NavajoWhite, PeachPuff,
				MistyRose, LavenderBlush, Linen, OldLace,
				PapayaWhip, SeaShell, MintCream, SlateGray,
				LightSlateGray, LightSteelBlue, Lavender, FloralWhite,
				AliceBlue, GhostWhite, Honeydew, Ivory,
				Azure, Snow, Black, DimGrey,
				Grey, DarkGrey, Silver, LightGrey,
				Gainsboro, WhiteSmoke, White;
		};


	public:
		Color(const uchar& value = uchar(0xFFu));
		Color(const uchar& red, const uchar& green, const uchar& blue, const uchar& alpha = uchar(0xFFu));
		Color(const Color &color);

	public:
		Color& operator=(const Color& color);
		Color& operator=(Color&& color) noexcept;
		Color& operator=(const uchar& value);
	};
}

#endif // !COLOR_H