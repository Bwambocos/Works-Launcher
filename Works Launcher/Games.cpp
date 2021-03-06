﻿// include
#include "Games.h"

// ゲーム選択画面

// 初期化
Games::Games(const InitData& init) : IScene(init)
{
	// ゲーム読み込み
	size_t index = 0;
	for (const FilePath& gameDirectory : FileSystem::DirectoryContents(FileSystem::CurrentDirectory() + U"Games", false))
	{
		if (!FileSystem::IsDirectory(gameDirectory)) continue;

		// config.ini を読み込む
		const INIData ini(gameDirectory + U"config.ini");
		if (ini.isEmpty()) continue;
		Game game;
		game.title = ini[U"Game.title"];
		game.path = gameDirectory + ini[U"Game.path"];
		TextureAsset::Register(U"games" + Format(index) + U"_icon", gameDirectory + ini[U"Game.icon"], TextureDesc::Mipped);
		TextureAsset::Register(U"games" + Format(index) + U"_image", gameDirectory + ini[U"Game.image"], TextureDesc::Mipped);
		game.desc = ini[U"Game.desc"];
		game.readme = gameDirectory + ini[U"Game.readme"];
		game.credit = ini[U"Game.credit"];
		game.tools = ini[U"Game.tools"];
		game.time = ini[U"Game.time"];
		games << game;
		++index;
	}
	if (!games)
	{
		System::ShowMessageBox(U"Not found games");
		System::Exit();
	}

	FontAsset::Register(U"Games-largeFont", 36, U"data//fontR.ttc", FontStyle::Bold);
	FontAsset::Register(U"Games-midFont", 24, U"data//fontR.ttc");
	FontAsset::Register(U"Games-smallFont", 18, U"data//fontR.ttc");
	TextureAsset::Register(U"Games-playIcon", Icon(0xf144, FontAsset(U"Games-largeFont").height() - 8));
	TextureAsset::Register(U"Games-readmeIcon", Icon(0xf05a, FontAsset(U"Games-largeFont").height() - 8));
	TextureAsset::Register(U"Games-imageIcon", Icon(0xf03e, FontAsset(U"Games-smallFont").height() - 8));
	TextureAsset::Register(U"Games-titleIcon", Icon(0xf11b, FontAsset(U"Games-smallFont").height() - 8));
	TextureAsset::Register(U"Games-descIcon", Icon(0xf075, FontAsset(U"Games-smallFont").height() - 8));
	TextureAsset::Register(U"Games-creditIcon", Icon(0xf0c0, FontAsset(U"Games-smallFont").height() - 8));
	TextureAsset::Register(U"Games-toolsIcon", Icon(0xf7d9, FontAsset(U"Games-smallFont").height() - 8));
	TextureAsset::Register(U"Games-timeIcon", Icon(0xf1da, FontAsset(U"Games-smallFont").height() - 8));
	TextureAsset::Register(U"Games-leftIcon", Icon(0xf137, 42));
	TextureAsset::Register(U"Games-rightIcon", Icon(0xf138, 42));
	TextureAsset::Register(U"Games-homeIcon", Icon(0xf015, 48));
	baseTilePos = Vec2(100, Scene::Height() - 50 - tileSize / 2);
	tileBackgroundRect = Rect(0, Scene::Height() - 25 - (int)tileSize / 2, Scene::Width(), 25 + (int)tileSize / 2);
	imageBackgroundRect = Rect(50, 50, 640, 360);
	playRect = Rect(50, imageBackgroundRect.y + imageBackgroundRect.h + 25, (imageBackgroundRect.w - 25) / 2, FontAsset(U"Games-largeFont").height() + 5);
	readmeRect = Rect(playRect.x + playRect.w + 25, imageBackgroundRect.y + imageBackgroundRect.h + 25, playRect.w, FontAsset(U"Games-largeFont").height() + 5);
	titleRect = Rect(imageBackgroundRect.x + imageBackgroundRect.w + 50, imageBackgroundRect.y, Scene::Width() - 50 - (imageBackgroundRect.x + imageBackgroundRect.w + 50), FontAsset(U"Games-largeFont").height() + 5);
	descRect = Rect(titleRect.x, titleRect.y + titleRect.h + 35, titleRect.w, FontAsset(U"Games-midFont").height() * 5 + 5);
	creditRect = Rect(titleRect.x, descRect.y + descRect.h + 35, titleRect.w, FontAsset(U"Games-smallFont").height() * 2 + 5);
	toolsRect = Rect(titleRect.x, creditRect.y + creditRect.h + 35, (titleRect.w - 25) / 2, FontAsset(U"Games-smallFont").height() + 5);
	timeRect = Rect(toolsRect.x + toolsRect.w + 25, toolsRect.y, toolsRect.w, FontAsset(U"Games-smallFont").height() + 5);
	leftIconPos = Vec2(5, Scene::Height() - TextureAsset(U"Games-leftIcon").height() - 2.5);
	rightIconPos = Vec2(Scene::Width() - TextureAsset(U"Games-rightIcon").width() - 5, Scene::Height() - TextureAsset(U"Games-rightIcon").height() - 2.5);
	homeIconPos = Vec2(Scene::Width() - TextureAsset(U"Games-homeIcon").width() - TextureAsset(U"lightIcon").width() - 15, 5);
	rectHeader = Quad(Vec2(0, 0), Vec2(125, 0), Vec2(125 + FontAsset(U"Games-smallFont").height(), FontAsset(U"Games-smallFont").height()), Vec2(0, FontAsset(U"Games-smallFont").height()));
}

// 更新
void Games::update()
{
	// ウィンドウの最小化・復帰
	if (process)
	{
		if (process->isRunning())
		{
			if (!gameplayStopwatch.isRunning()) gameplayStopwatch.restart();
			return;
		}
		else
		{
			gameplayStopwatch.pause();
			INIData ini(U"data//Games//logs.ini");
			ini.write<int32>(games[selectedGameIndex].title, U"count", ini.get<int32>(games[selectedGameIndex].title, U"count") + 1);
			ini.write<int32>(games[selectedGameIndex].title, U"time", ini.get<int32>(games[selectedGameIndex].title, U"time") + gameplayStopwatch.ms());
			ini.save(U"data//Games//logs.ini");
			Window::Restore();
			process = none;
		}
	}

	// メニューへ戻る
	if (TextureAsset(U"Games-homeIcon").region(homeIconPos).leftClicked())
	{
		AudioAsset(U"cursorAudio").stop();
		AudioAsset(U"cursorAudio").play();
		changeScene(U"Category");
	}

	// ゲーム起動・説明書表示
	Game& game = games[selectedGameIndex];
	if (playRect.mouseOver() || readmeRect.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
	if (playRect.leftClicked())
	{
		AudioAsset(U"cursorAudio").stop();
		AudioAsset(U"cursorAudio").play();
		process = s3dx::System::CreateProcess(game.path);
	}
	if (readmeRect.leftClicked())
	{
		AudioAsset(U"cursorAudio").stop();
		AudioAsset(U"cursorAudio").play();
		process = s3dx::System::CreateProcess(U"C://Windows//system32//notepad.exe", U"/A " + game.readme);
	}

	// 選択しているタイルの変更
	auto temp = selectedGameIndex;
	for (auto i : step(games.size()))
	{
		const Vec2 center = baseTilePos.movedBy(tileOffsetX + i * (tileSize * 2), 0);
		const RectF tile(Arg::center = center, tileSize);
		if (tile.leftClicked())
		{
			AudioAsset(U"cursorAudio").stop();
			AudioAsset(U"cursorAudio").play();
			selectedGameIndex = i;
		}
		if (tile.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
	}
	if (KeyLeft.down() || TextureAsset(U"Games-leftIcon").region(leftIconPos).leftClicked()) selectedGameIndex--;
	if (KeyRight.down() || TextureAsset(U"Games-rightIcon").region(rightIconPos).leftClicked()) selectedGameIndex++;
	selectedGameIndex += (size_t)Mouse::Wheel();
	selectedGameIndex = Max((long long)selectedGameIndex, 0LL);
	selectedGameIndex = Min<unsigned long long>(selectedGameIndex, (long long)games.size() - 1);
	if (temp != selectedGameIndex)
	{
		AudioAsset(U"cursorAudio").stop();
		AudioAsset(U"cursorAudio").play();
	}

	// タイル表示のスクロール更新
	const Vec2 center = baseTilePos.movedBy(targetTileOffsetX + selectedGameIndex * (tileSize * 2), 0);
	const RectF tile(Arg::center = center, tileSize);
	if (tile.x <= 0) targetTileOffsetX += tileSize;
	else if (Scene::Width() <= tile.tr().x) targetTileOffsetX -= tileSize;
	tileOffsetX = Math::SmoothDamp(tileOffsetX, targetTileOffsetX, tileOffsetXVelocity, 0.1, Scene::DeltaTime());
}

// 描画
void Games::draw() const
{
	// メニューへ戻る
	TextureAsset(U"Games-homeIcon").draw(homeIconPos, (TextureAsset(U"Games-homeIcon").region(homeIconPos).mouseOver() ? getData().schemeColor5 : getData().schemeColor4));

	// タイル
	tileBackgroundRect.draw(getData().schemeColor2);
	for (auto [i, g] : Indexed(games))
	{
		const Vec2 center = baseTilePos.movedBy(tileOffsetX + i * (tileSize * 2), 0);
		const RectF tile(Arg::center = center, tileSize);

		TextureAsset(U"games" + Format(i) + U"_icon")
			.flipped()
			.resized(tileSize, tileSize)
			.draw(center.x - tileSize / 2, center.y + tileSize / 2, Arg::top = AlphaF(0.8), Arg::bottom = AlphaF(0.0));
		drawButton(tile, i == selectedGameIndex);
		tile(TextureAsset(U"games" + Format(i) + U"_icon")).drawAt(center);
	}
	if (selectedGameIndex > 0) TextureAsset(U"Games-leftIcon").draw(leftIconPos, (TextureAsset(U"Games-leftIcon").region(leftIconPos).mouseOver() ? getData().schemeColor5 : getData().schemeColor4));
	if (selectedGameIndex + 1 < games.size()) TextureAsset(U"Games-rightIcon").draw(rightIconPos, (TextureAsset(U"Games-rightIcon").region(rightIconPos).mouseOver() ? getData().schemeColor5 : getData().schemeColor4));

	// イメージ画像
	rectHeader
		.movedBy(imageBackgroundRect.x, imageBackgroundRect.y - FontAsset(U"Games-smallFont").height() + 3)
		.draw(getData().schemeColor2)
		.drawFrame(3, 0, getData().schemeColor3);
	TextureAsset(U"Games-imageIcon").drawAt(imageBackgroundRect.x + FontAsset(U"Games-smallFont").height() / 2 + 5, imageBackgroundRect.y - FontAsset(U"Games-smallFont").height() / 2 + 3, getData().stringColor);
	FontAsset(U"Games-smallFont")(U"ゲーム画面").draw(imageBackgroundRect.x + FontAsset(U"Games-smallFont").height() + 5, imageBackgroundRect.y - FontAsset(U"Games-smallFont").height() + 3, getData().stringColor);
	imageBackgroundRect.drawShadow(Vec2(0, 3), 8, 0);
	imageBackgroundRect(TextureAsset(U"games" + Format(selectedGameIndex) + U"_image")).draw();
	imageBackgroundRect.drawFrame(3, 0, getData().schemeColor3);

	// ゲーム起動・説明書表示
	drawButton(playRect);
	TextureAsset(U"Games-playIcon").drawAt(playRect.center().movedBy(-FontAsset(U"Games-largeFont")(U"起動する").region().w / 2, 0), getData().stringColor);
	FontAsset(U"Games-largeFont")(U"起動する").drawAt(playRect.center().movedBy(FontAsset(U"Games-largeFont").height() / 2, 0), getData().stringColor);
	drawButton(readmeRect);
	TextureAsset(U"Games-readmeIcon").drawAt(readmeRect.center().movedBy(-FontAsset(U"Games-largeFont")(U"説明書を開く").region().w / 2, 0), getData().stringColor);
	FontAsset(U"Games-largeFont")(U"説明書を開く").drawAt(readmeRect.center().movedBy(FontAsset(U"Games-largeFont").height() / 2, 0), getData().stringColor);

	// ゲーム情報
	const Game& game = games[selectedGameIndex];
	drawStrBackground(titleRect, U"ゲーム名", U"Games-titleIcon");
	FontAsset(U"Games-largeFont")(game.title).draw(titleRect.stretched(-5, -2), getData().stringColor);
	drawStrBackground(descRect, U"ゲーム説明", U"Games-descIcon");
	FontAsset(U"Games-midFont")(game.desc).draw(descRect.stretched(-5, -2), getData().stringColor);
	drawStrBackground(creditRect, U"クレジット", U"Games-creditIcon");
	FontAsset(U"Games-smallFont")(game.credit).draw(creditRect.stretched(-5, -2), getData().stringColor);
	drawStrBackground(toolsRect, U"開発ツール", U"Games-toolsIcon");
	FontAsset(U"Games-smallFont")(game.tools).draw(toolsRect.stretched(-5, -2), getData().stringColor);
	drawStrBackground(timeRect, U"開発期間", U"Games-timeIcon");
	FontAsset(U"Games-smallFont")(game.time).draw(timeRect.stretched(-5, -2), getData().stringColor);
}

// ボタン描画
void Games::drawButton(Rect rect, bool highlight) const
{
	rect
		.stretched(4)
		.drawShadow(Vec2(0, 3), 8, 0)
		.draw(getData().schemeColor3)
		.drawFrame(3, 0, ColorF((rect.mouseOver() || highlight ? getData().schemeColor5 : getData().schemeColor4), 0.4 + (rect.mouseOver() ? 1 : Periodic::Sine0_1(1s)) * 0.6));
	rect.draw(getData().schemeColor3);
}

// 文字背景描画
void Games::drawStrBackground(Rect rect, String header, String icon) const
{
	rectHeader
		.movedBy(rect.x, rect.y - FontAsset(U"Games-smallFont").height() + 3)
		.draw(getData().schemeColor2)
		.drawFrame(3, 0, getData().schemeColor3);
	TextureAsset(icon).drawAt(rect.x + FontAsset(U"Games-smallFont").height() / 2 + 5, rect.y - FontAsset(U"Games-smallFont").height() / 2 + 3, getData().stringColor);
	FontAsset(U"Games-smallFont")(header).draw(rect.x + FontAsset(U"Games-smallFont").height() + 5, rect.y - FontAsset(U"Games-smallFont").height() + 3, getData().stringColor);
	rect
		.drawShadow(Vec2(0, 3), 8, 0)
		.draw(getData().schemeColor2)
		.drawFrame(3, 0, getData().schemeColor3);
}
