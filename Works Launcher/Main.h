// include
#pragma once
#include <Siv3D.hpp>

// 基本情報
namespace AppInfo
{
	const String windowTitle = U"Works Launcher v1.5";
	const int32 windowWidth = 1280;
	const int32 windowHeight = 720;
};

// 共有データ
struct AppData
{
	int drawMode = 0;
	Color schemeColor1 = Color(22, 22, 22);
	Color schemeColor2 = Color(66, 66, 66);
	Color schemeColor3 = Color(110, 110, 110);
	Color schemeColor4 = Color(154, 154, 154);
	Color schemeColor5 = Color(198, 198, 198);
	Color stringColor = Color(Palette::White);

};

using MyApp = SceneManager<String, AppData>;
static MyApp manager;
