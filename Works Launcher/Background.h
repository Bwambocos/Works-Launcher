// include
#pragma once
#include "Main.h"

// バックグラウンド処理

class Background
{
private:

	Array<std::pair<Line, Stopwatch>>m_effect;
	Stopwatch m_effectStopwatch;
	Vec2 lightIconPos;

public:

	// 初期化
	void init();

	// 更新
	bool update(AppData& data);

	// 描画
	void draw(AppData& data);

	// ダークモードとライトモードを切り替える
	void setDrawMode(AppData& data);
};
