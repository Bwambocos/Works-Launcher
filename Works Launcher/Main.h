// include
#pragma once
#include <Siv3D.hpp>

// 基本情報
namespace AppInfo
{
	const String windowTitle = U"Works Launcher v1.0";
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

// ダークモードとライトモードを切り替える
void setDrawMode(AppData& data);

// 背景アニメーション
struct TitleBackGroundEffect : IEffect
{
	Line m_line;

	TitleBackGroundEffect()
	{
		const Vec2 pos = RandomVec2(Scene::Width(), Scene::Height());
		const Vec2 direction = Circular(Scene::Width() + Scene::Height(), Random(360_deg));
		m_line.set(pos - direction, pos + direction);
	}

	bool update(double timeSec)
	{
		m_line.draw(2, AlphaF((1.0 - Abs(timeSec - 1.0)) * 0.3));
		return timeSec < 2.0;
	}
};
