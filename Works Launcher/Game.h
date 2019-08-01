// include
#pragma once
#include "Main.h"

class Games : public MyApp::Scene
{
private:

public:
	// 初期化
	Games(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;
};
