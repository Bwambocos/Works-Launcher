// include
#include "Games.h"

// ゲーム選択画面

// 初期化
Games::Games(const InitData& init) : IScene(init)
{
	// ゲーム読み込み
	for (const FilePath& gameDirectory : FileSystem::DirectoryContents(FileSystem::CurrentDirectory() + U"Games", false))
	{
		if (!FileSystem::IsDirectory(gameDirectory)) continue;

		// config.ini を読み込む
		const INIData ini(gameDirectory + U"config.ini");
		if (ini.isEmpty()) continue;
		Game game;
		game.title = ini[U"Game.title"];
		game.path = gameDirectory + ini[U"Game.path"];
		game.icon = Texture(gameDirectory + ini[U"Game.icon"]);
		game.image = Texture(gameDirectory + ini[U"Game.image"]);
		game.desc = ini[U"Game.desc"];
		game.readme = gameDirectory + ini[U"Game.readme"];
		game.credit = ini[U"Game.credit"];
		game.tools = ini[U"Game.tools"];
		games << game;
	}
	if (!games)
	{
		System::ShowMessageBox(U"Not found games");
		System::Exit();
	}
	playRect = Rect(900, 340, 220, 85);
	tileBackgroundRect = Rect(0, Scene::Height() - 25 - tileSize / 2, Scene::Width(), 25 + tileSize / 2);
}

// 更新
void Games::update()
{
	const Game& game = games[selectedGameIndex];

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

		g.icon(0, tileSize / 2, tileSize, tileSize / 2)
			.flipped()
			.draw(center.x - tileSize / 2, center.y + tileSize / 2, Arg::top = AlphaF(0.8), Arg::bottom = AlphaF(0.0));
		if (selectedGameIndex == i)
		{
			tile
				.stretched(6)
				.drawShadow(Vec2(0, 3), 8, 0)
				.draw(AppInfo::backgroundColor)
				.drawFrame(5, 0, ColorF(Palette::Red, 0.4 + Periodic::Sine0_1(1s) * 0.6));
		}
		tile(g.icon).drawAt(center);
	}
}
