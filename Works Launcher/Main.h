// include
#pragma once
#include <Siv3D.hpp>

// 基本情報
namespace AppInfo
{
	const String windowTitle = U"Works Launcher v1.0";
	const int32 windowWidth = 800;
	const int32 windowHeight = 600;
	const ColorF backgroundColor(0.8, 0.9, 1.0);
	const Color fadeColor(200, 200, 200);
};

// 共有データ
struct AppData
{

};

using MyApp = SceneManager<String, AppData>;
