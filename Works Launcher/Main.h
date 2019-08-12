// include
#pragma once
#include <Siv3D.hpp>

// 基本情報
namespace AppInfo
{
	const String windowTitle = U"Works Launcher v1.0";
	const int32 windowWidth = 1280;
	const int32 windowHeight = 720;
	const Color schemeColor1(22, 22, 22);
	const Color schemeColor2(66, 66, 66);
	const Color schemeColor3(83, 83, 83);
	const Color schemeColor4(112, 112, 112);
	const Color schemeColor5(159, 159, 159);
	const Color backgroundColor = schemeColor1;
	const Color fadeColor = schemeColor1;
};

// 共有データ
struct AppData
{

};

using MyApp = SceneManager<String, AppData>;
