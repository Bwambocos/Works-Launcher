// include
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
		games << game;
		++index;
	}
	if (!games)
	{
		System::ShowMessageBox(U"Not found games");
		System::Exit();
	}
	baseTilePos = Vec2(100, Scene::Height() - 50 - tileSize / 2);
	playRect = Rect(900, 340, 220, 85);
	tileBackgroundRect = Rect(0, Scene::Height() - 25 - tileSize / 2, Scene::Width(), 25 + tileSize / 2);
	imageBackgroundRect = Rect(50, 50, 640, 360);
}

// 更新
void Games::update()
{
	Game& game = games[selectedGameIndex];

	// ウィンドウの最小化・復帰
	if (process)
	{
		if (process->isRunning())
		{
			Window::Minimize();
			return;
		}
		else
		{
			Window::Restore();
			process = none;
		}
	}

	// ゲームの起動
	if (playRect.leftClicked()) process = s3dx::System::CreateProcess(game.path);
	if (playRect.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);

	// 選択しているタイルの変更
	for (auto i : step(games.size()))
	{
		const Vec2 center = baseTilePos.movedBy(tileOffsetX + i * (tileSize * 2), 0);
		const RectF tile(Arg::center = center, tileSize);
		if (tile.leftClicked()) selectedGameIndex = i;
		if (tile.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
	}

	// [←][→] キーを押して選択の移動
	if (KeyLeft.down()) selectedGameIndex = Max((long long)selectedGameIndex - 1, 0LL);
	if (KeyRight.down()) selectedGameIndex = Min<unsigned long long>(selectedGameIndex + 1, (long long)games.size() - 1);

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
	// タイル
	tileBackgroundRect.draw(ColorF(0.2, 0.3, 0.4));
	for (auto [i, g] : Indexed(games))
	{
		const Vec2 center = baseTilePos.movedBy(tileOffsetX + i * (tileSize * 2), 0);
		const RectF tile(Arg::center = center, tileSize);

		TextureAsset(U"games" + Format(i) + U"_icon")
			.flipped()
			.resized(tileSize, tileSize)
			.draw(center.x - tileSize / 2, center.y + tileSize / 2, Arg::top = AlphaF(0.8), Arg::bottom = AlphaF(0.0));
		if (selectedGameIndex == i)
		{
			tile
				.stretched(6)
				.drawShadow(Vec2(0, 3), 8, 0)
				.draw(AppInfo::backgroundColor)
				.drawFrame(5, 0, ColorF(Palette::Red, 0.4 + Periodic::Sine0_1(1s) * 0.6));
		}
		tile(TextureAsset(U"games" + Format(i) + U"_icon")).drawAt(center);
	}

	// ゲーム説明
	imageBackgroundRect
		.stretched(4)
		.drawShadow(Vec2(0, 3), 8, 0)
		.draw(AppInfo::backgroundColor)
		.drawFrame(3, 0, ColorF(Palette::Gold, 0.4 + Periodic::Sine0_1(2s) * 0.6));
	imageBackgroundRect(TextureAsset(U"games" + Format(selectedGameIndex) + U"_image")).draw();
}
