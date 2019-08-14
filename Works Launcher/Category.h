// include
#pragma once
#include "Main.h"

// カテゴリー選択画面
class Category : public MyApp::Scene
{
private:

	const size_t choicesNum = 3;
	Rect titleRect, exitRect;
	Array<Rect>choicesRects;
	Array<String>choicesStrs;
	Vec2 lightIconPos;
	bool exitFlag;

public:
	// 初期化
	Category(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;
};
