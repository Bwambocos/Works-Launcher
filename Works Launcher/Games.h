﻿// include
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
		String desc;
		FilePath readme;
		String credit;
		String tools;
	};

	Array<Game>games;
	Optional<s3dx::ProcessInfo>process;
	size_t selectedGameIndex = 0;
	Vec2 baseTilePos;
	Rect tileBackgroundRect, imageBackgroundRect, playRect, readmeRect, titleRect, descRect, creditRect, toolsRect;
	Quad rectHeader;
	double tileOffsetX = 0., targetTileOffsetX = 0., tileOffsetXVelocity = 0.;

public:
	// 初期化
	Games(const InitData& init);

	// 更新
	void update() override;

	// 描画
	void draw() const override;

	// ボタン描画
	void drawButton(Rect rect, Color framecolor1, Color framecolor2, Color drawcolor) const;

	// 文字背景描画
	void drawStrBackground(Rect rect, String header) const;
};
