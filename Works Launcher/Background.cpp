// include
#include "Background.h"

// バックグラウンド処理

// 初期化
void Background::init()
{
	TextureAsset::Register(U"lightIcon", Icon(0xf0eb, 38));
	AudioAsset::Register(U"cursorAudio", U"data//cursorAudio.mp3");
	AudioAsset::Register(U"dog", U"data//dog.mp3");
	FontAsset::Register(U"timerFont", 16, U"data//fontR.ttc");
	FontAsset::Register(U"messageFont", 72, U"data//fontR.ttc", FontStyle::Bold);
	m_effectStopwatch.start();
	lightIconPos = Vec2(Scene::Width() - TextureAsset(U"lightIcon").width() - 10, 10);
	const INIData configINI(U"data//config.ini");
	demoFlag = configINI.get<bool>(U"Demo", U"flag");
	if (demoFlag) demoTimer = Timer(configINI.get<double>(U"Demo", U"limit_time"), true);
}

// 更新
bool Background::update(AppData& data)
{
	// 描画モード切替
	if (TextureAsset(U"lightIcon").region(lightIconPos).leftClicked())
	{
		AudioAsset(U"cursorAudio").stop();
		AudioAsset(U"cursorAudio").play();
		setDrawMode(data);
	}

	// 背景エフェクト
	if (m_effectStopwatch.elapsed() > 50ms)
	{
		const Vec2 pos = RandomVec2(Scene::Width(), Scene::Height());
		const Vec2 direction = Circular(Scene::Width() + Scene::Height(), Random(360_deg));
		m_effect << std::pair<Line, Stopwatch>(Line(pos - direction, pos + direction), Stopwatch(true));
		m_effectStopwatch.restart();
	}

	// タイマー
	if (demoFlag)
	{
		if (KeyShift.pressed() && KeyAlt.pressed() && KeyControl.pressed() && KeyR.pressed())
		{
			Window::SetFullscreen(false);
			Window::Restore();
			demoTimer.restart();
			return true;
		}
		if (demoTimer.reachedZero())
		{
			Window::SetFullscreen(true);
			FontAsset(U"messageFont")(U"時間制限に達しました．\n大変申し訳ありませんが，交代をお願いします．").drawAt(Scene::Center(), data.stringColor);
			return false;
		}
	}


	// 隠し要素
	if (KeyShift.pressed() && KeyD.pressed() && KeyO.pressed() && KeyG.pressed()) AudioAsset(U"dog").play();

	return true;
}

// 描画
void Background::draw(AppData& data)
{
	// 描画モード切替
	if (!(demoFlag && demoTimer.reachedZero())) TextureAsset(U"lightIcon").draw(lightIconPos, (TextureAsset(U"lightIcon").region(lightIconPos).mouseOver() ? data.schemeColor5 : data.schemeColor4));

	// 背景エフェクト
	for (auto line : m_effect)
	{
		if (line.second.sF() < 2.) line.first.draw(2, AlphaF((1.0 - Abs(line.second.sF() - 1.0)) * 0.3));
	}

	// タイマー
	if (demoFlag) FontAsset(U"timerFont")(U"残り時間：" + demoTimer.format(U"mm:ss.xx")).draw(5, 5, data.stringColor);
}

// ダークモードとライトモードを切り替える
void Background::setDrawMode(AppData& data)
{
	(++data.drawMode) %= 2;
	if (data.drawMode == 0)
	{
		data.schemeColor1 = Color(22, 22, 22);
		data.schemeColor2 = Color(66, 66, 66);
		data.schemeColor3 = Color(110, 110, 110);
		data.schemeColor4 = Color(154, 154, 154);
		data.schemeColor5 = Color(198, 198, 198);
		data.stringColor = Palette::White;
	}
	else
	{
		data.schemeColor1 = Color(64, 138, 241);
		data.schemeColor2 = Color(90, 162, 246);
		data.schemeColor3 = Color(120, 182, 250);
		data.schemeColor4 = Color(136, 203, 250);
		data.schemeColor5 = Color(168, 215, 250);
		data.stringColor = Palette::Black;
	}
	Scene::SetBackground(data.schemeColor1);
	manager.setFadeColor(data.schemeColor1);
}
