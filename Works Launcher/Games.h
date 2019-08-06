// include
#pragma once
#include "Main.h"
#include "CreateProcess.h"

const double tileSize = 100;

// ゲーム選択画面
class Games : public MyApp::Scene
{
private:

	// ゲーム情報
	struct Game
	{
		String title;
		FilePath path;
		Texture icon;
		Texture image;
		String desc;
		FilePath readme;
		String credit;
		String tools;
	};

	Array<Game>games;
	Optional<s3dx::ProcessInfo>process;
	size_t selectedGameIndex = 0;
	Vec2 baseTilePos = Vec2(200, Scene::Height() - 50 - tileSize / 2);
	Rect playRect, tileBackgroundRect;
	double tileOffsetX = 0., targetTileOffsetX = 0., tileOffsetXVelocity = 0.;

public:
	// 初期化
	Games(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;
};
