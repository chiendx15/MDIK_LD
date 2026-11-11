#pragma once

// RGB Color Name 및 비율
// 출처 :  https://htmlcolorcodes.com/color-names/
namespace RgbColor
{
	// Red Color Names
	static constexpr COLORREF IndianRed			= RGB(205, 92, 92);
	static constexpr COLORREF LightCoral		= RGB(240, 128, 128);
	static constexpr COLORREF Salmon			= RGB(250, 128, 114);
	static constexpr COLORREF DarkSalmon		= RGB(233, 150, 122);
	static constexpr COLORREF Crimson			= RGB(220, 20, 60);
	static constexpr COLORREF Red				= RGB(255, 0, 0);
	static constexpr COLORREF FireBrick			= RGB(178, 34, 34);
	static constexpr COLORREF DarkRed			= RGB(139, 0, 0);
	
	// Pink Color Names
	static constexpr COLORREF Pink				= RGB(255, 192, 203);
	static constexpr COLORREF LightPink			= RGB(255, 182, 193);
	static constexpr COLORREF HotPink			= RGB(255, 105, 180);
	static constexpr COLORREF DeepPink			= RGB(255, 20, 147);
	static constexpr COLORREF MediumVioletRed	= RGB(199, 21, 133);
	static constexpr COLORREF PaleVioletRed		= RGB(219, 112, 147);
	
	// Orange Color Names
	static constexpr COLORREF LightSalmon		= RGB(255, 160, 122);
	static constexpr COLORREF Coral				= RGB(255, 127, 80);
	static constexpr COLORREF Tomato			= RGB(255, 99, 71);
	static constexpr COLORREF OrangeRed			= RGB(255, 69, 0);
	static constexpr COLORREF DarkOrange		= RGB(255, 140, 0);
	static constexpr COLORREF Orange			= RGB(255, 165, 0);
	
	// Yellow Color Names
	static constexpr COLORREF Gold				= RGB(255, 215, 0);
	static constexpr COLORREF Yellow			= RGB(255, 255, 0);
	static constexpr COLORREF LightYellow		= RGB(255, 255, 224);
	static constexpr COLORREF LemonChiffon		= RGB(255, 250, 205);
	static constexpr COLORREF PapayaWhip		= RGB(255, 239, 213);
	static constexpr COLORREF Moccasin			= RGB(255, 228, 181);
	static constexpr COLORREF PeachPuff			= RGB(255, 218, 185);
	static constexpr COLORREF PaleGoldenrod		= RGB(238, 232, 170);
	static constexpr COLORREF Khaki				= RGB(240, 230, 140);
	static constexpr COLORREF DarkKhaki			= RGB(189, 183, 107);
	
	// Purple Color Names
	static constexpr COLORREF Lavender			= RGB(230, 230, 250);
	static constexpr COLORREF Thistle			= RGB(216, 191, 216);
	static constexpr COLORREF Plum				= RGB(221, 160, 221);
	static constexpr COLORREF Violet			= RGB(238, 130, 238);
	static constexpr COLORREF Orchid			= RGB(218, 112, 214);
	static constexpr COLORREF Fuchsia			= RGB(255, 0, 255);
	static constexpr COLORREF Magenta			= RGB(255, 0, 255);
	static constexpr COLORREF MediumOrchid		= RGB(186, 85, 211);
	static constexpr COLORREF MediumPurple		= RGB(147, 112, 219);
	static constexpr COLORREF RebeccaPurple		= RGB(102, 51, 153);
	static constexpr COLORREF BlueViolet		= RGB(138, 43, 226);
	static constexpr COLORREF DarkViolet		= RGB(148, 0, 211);
	static constexpr COLORREF DarkOrchid		= RGB(153, 50, 204);
	static constexpr COLORREF DarkMagenta		= RGB(139, 0, 139);
	static constexpr COLORREF Purple			= RGB(128, 0, 128);
	static constexpr COLORREF Indigo			= RGB(75, 0, 130);
	static constexpr COLORREF SlateBlue			= RGB(106, 90, 205);
	static constexpr COLORREF DarkSlateBlue		= RGB(72, 61, 139);
	
	// Green Color Names
	static constexpr COLORREF GreenYellow		= RGB(173, 255, 47);
	static constexpr COLORREF Chartreuse		= RGB(127, 255, 0);
	static constexpr COLORREF LawnGreen			= RGB(124, 252, 0);
	static constexpr COLORREF Lime				= RGB(0, 255, 0);
	static constexpr COLORREF LimeGreen			= RGB(50, 205, 50);
	static constexpr COLORREF PaleGreen			= RGB(152, 251, 152);
	static constexpr COLORREF LightGreen		= RGB(144, 238, 144);
	static constexpr COLORREF SpringGreen		= RGB(0, 255, 127);
	static constexpr COLORREF MediumSeaGreen	= RGB(60, 179, 113);
	static constexpr COLORREF SeaGreen			= RGB(46, 139, 87);
	static constexpr COLORREF ForestGreen		= RGB(34, 139, 34);
	static constexpr COLORREF Green				= RGB(0, 128, 0);
	static constexpr COLORREF DarkGreen			= RGB(0, 100, 0);
	static constexpr COLORREF YellowGreen		= RGB(154, 205, 50);
	static constexpr COLORREF OliveDrab			= RGB(107, 142, 35);
	static constexpr COLORREF Olive				= RGB(128, 128, 0);
	static constexpr COLORREF DarkOliveGreen	= RGB(85, 107, 47);
	static constexpr COLORREF MediumAquamarine	= RGB(102, 205, 170);
	static constexpr COLORREF DarkSeaGreen		= RGB(143, 188, 139);
	static constexpr COLORREF LightSeaGreen		= RGB(32, 178, 170);
	static constexpr COLORREF DarkCyan			= RGB(0, 139, 139);
	static constexpr COLORREF Teal				= RGB(0, 128, 128);
	
	// Blue Color Names
	static constexpr COLORREF Aqua				= RGB(0, 255, 255);
	static constexpr COLORREF Cyan				= RGB(0, 255, 255);
	static constexpr COLORREF LightCyan			= RGB(224, 255, 255);
	static constexpr COLORREF PaleTurquoise		= RGB(175, 238, 238);
	static constexpr COLORREF Aquamarine		= RGB(127, 255, 212);
	static constexpr COLORREF Turquoise			= RGB(64, 224, 208);
	static constexpr COLORREF MediumTurquoise	= RGB(72, 209, 204);
	static constexpr COLORREF DarkTurquoise		= RGB(0, 206, 209);
	static constexpr COLORREF CadetBlue			= RGB(95, 158, 160);
	static constexpr COLORREF SteelBlue			= RGB(70, 130, 180);
	static constexpr COLORREF LightSteelBlue	= RGB(176, 196, 222);
	static constexpr COLORREF PowderBlue		= RGB(176, 224, 230);
	static constexpr COLORREF LightBlue			= RGB(173, 216, 230);
	static constexpr COLORREF SkyBlue			= RGB(135, 206, 235);
	static constexpr COLORREF LightSkyBlue		= RGB(135, 206, 250);
	static constexpr COLORREF DeepSkyBlue		= RGB(0, 191, 255);
	static constexpr COLORREF DodgerBlue		= RGB(30, 144, 255);
	static constexpr COLORREF CornflowerBlue	= RGB(100, 149, 237);
	static constexpr COLORREF MediumSlateBlue	= RGB(123, 104, 238);
	static constexpr COLORREF RoyalBlue			= RGB(65, 105, 225);
	static constexpr COLORREF Blue				= RGB(0, 0, 255);
	static constexpr COLORREF MediumBlue		= RGB(0, 0, 205);
	static constexpr COLORREF DarkBlue			= RGB(0, 0, 139);
	static constexpr COLORREF Navy				= RGB(0, 0, 128);
	static constexpr COLORREF MidnightBlue		= RGB(25, 25, 112);
	
	// Brown Color Names
	static constexpr COLORREF Cornsilk			= RGB(255, 248, 220);
	static constexpr COLORREF BlanchedAlmond	= RGB(255, 235, 205);
	static constexpr COLORREF Bisque			= RGB(255, 228, 196);
	static constexpr COLORREF NavajoWhite		= RGB(255, 222, 173);
	static constexpr COLORREF Wheat				= RGB(245, 222, 179);
	static constexpr COLORREF BurlyWood			= RGB(222, 184, 135);
	static constexpr COLORREF Tan				= RGB(210, 180, 140);
	static constexpr COLORREF RosyBrown			= RGB(188, 143, 143);
	static constexpr COLORREF SandyBrown		= RGB(244, 164, 96);
	static constexpr COLORREF Goldenrod			= RGB(218, 165, 32);
	static constexpr COLORREF DarkGoldenrod		= RGB(184, 134, 11);
	static constexpr COLORREF Peru				= RGB(205, 133, 63);
	static constexpr COLORREF Chocolate			= RGB(210, 105, 30);
	static constexpr COLORREF SaddleBrown		= RGB(139, 69, 19);
	static constexpr COLORREF Sienna			= RGB(160, 82, 45);
	static constexpr COLORREF Brown				= RGB(165, 42, 42);
	static constexpr COLORREF Maroon			= RGB(128, 0, 0);
	
	// White Color Names
	static constexpr COLORREF White				= RGB(255, 255, 255);
	static constexpr COLORREF Snow				= RGB(255, 250, 250);
	static constexpr COLORREF HoneyDew			= RGB(240, 255, 240);
	static constexpr COLORREF MintCream			= RGB(245, 255, 250);
	static constexpr COLORREF Azure				= RGB(240, 255, 255);
	static constexpr COLORREF AliceBlue			= RGB(240, 248, 255);
	static constexpr COLORREF GhostWhite		= RGB(248, 248, 255);
	static constexpr COLORREF WhiteSmoke		= RGB(245, 245, 245);
	static constexpr COLORREF SeaShell			= RGB(255, 245, 238);
	static constexpr COLORREF Beige				= RGB(245, 245, 220);
	static constexpr COLORREF OldLace			= RGB(253, 245, 230);
	static constexpr COLORREF FloralWhite		= RGB(255, 250, 240);
	static constexpr COLORREF Ivory				= RGB(255, 255, 240);
	static constexpr COLORREF AntiqueWhite		= RGB(250, 235, 215);
	static constexpr COLORREF Linen				= RGB(250, 240, 230);
	static constexpr COLORREF LavenderBlush		= RGB(255, 240, 245);
	static constexpr COLORREF MistyRose			= RGB(255, 228, 225);
	
	// Gray Color Names
	static constexpr COLORREF Gainsboro			= RGB(220, 220, 220);
	static constexpr COLORREF LightGray			= RGB(211, 211, 211);
	static constexpr COLORREF Silver			= RGB(192, 192, 192);
	static constexpr COLORREF DarkGray			= RGB(169, 169, 169);
	static constexpr COLORREF Gray				= RGB(128, 128, 128);
	static constexpr COLORREF DimGray			= RGB(105, 105, 105);
	static constexpr COLORREF LightSlateGray	= RGB(119, 136, 153);
	static constexpr COLORREF SlateGray			= RGB(112, 128, 144);
	static constexpr COLORREF DarkSlateGray		= RGB(47, 79, 79);
	static constexpr COLORREF Black				= RGB(0, 0, 0);


	// MDI
	static constexpr COLORREF MDIBackgroundColor = WhiteSmoke;

	static constexpr COLORREF MDIDefaultColor	= RGB(200, 200, 200);	// 버튼 기본 상태
	static constexpr COLORREF MDISelectedColor	= DarkGray;				// 버튼 눌린 상태
	static constexpr COLORREF MDIMouseOverColor	= Gainsboro;			// 버튼 눌린 상태

	static constexpr COLORREF MDIOnColor		= RGB(255, 170, 60);	// 항목 선택된 상태
}

