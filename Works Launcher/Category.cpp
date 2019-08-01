// include
#include "Category.h"

// カテゴリー選択画面

// 初期化
Category::Category(const InitData& init) : IScene(init)
{
	largeFont = Font(42, U"data//fontR.ttc", FontStyle::Bold);
	midFont = Font(32, U"data//fontR.ttc", FontStyle::Bold);
	titleRect = Rect(5, 5, Scene::Width() - 10, largeFont.height() + 5);
	exitRect = Rect(5, Scene::Height() - largeFont.height() - 10, Scene::Width() - 10, largeFont.height() + 5);
	musicRect = Rect(10, titleRect.y + titleRect.h + 10, (Scene::Width() - 40) / 3, Scene::Height() - titleRect.h * 2 - 30);
	gamesRect = Rect(musicRect.x + musicRect.w + 10, musicRect.y, musicRect.w, musicRect.h);
	graphicsRect = Rect(gamesRect.x + gamesRect.w + 10, musicRect.y, musicRect.w, musicRect.h);
	musicImage = Texture(U"data//Category//musicImage.png");
	gamesImage = Texture(U"data//Category//gamesImage.png");
	graphicsImage = Texture(U"data//Category//graphicsImage.png");
}

// 更新
void Category::update()
{
	if (exitRect.mouseOver() || musicRect.mouseOver() || gamesRect.mouseOver() || graphicsRect.mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
	if (exitRect.leftClicked())
	{
		if (System::ShowMessageBox(U"終了確認", U"本当に終了しますか？", MessageBoxButtons::YesNo) == MessageBoxSelection::Yes) System::Exit();
	}
}

// 描画
void Category::draw() const
{
	titleRect.draw(Color(255, 255, 255, 200)).drawFrame(1, Palette::Dimgray);
	largeFont(U"作品の種類を選択して下さい").drawAt(titleRect.center(), Palette::Black);
	exitRect.draw(Color(255, 255, 255, (exitRect.mouseOver() ? 200 : 100))).drawFrame(1, Palette::Dimgray);
	largeFont(U"終了する").drawAt(exitRect.center(), Palette::Black);
	musicRect(musicImage).draw(Color(255, 255, 255, (musicRect.mouseOver() ? 200 : 100))).drawFrame(1, Palette::Dimgray);
	midFont(U"音楽").drawAt(musicRect.center(), Palette::Black);
	gamesRect(gamesImage).draw(Color(255, 255, 255, (gamesRect.mouseOver() ? 200 : 100))).drawFrame(1, Palette::Dimgray);
	midFont(U"ゲーム").drawAt(gamesRect.center(), Palette::Black);
	graphicsRect(graphicsImage).draw(Color(255, 255, 255, (graphicsRect.mouseOver() ? 200 : 100))).drawFrame(1, Palette::Dimgray);
	midFont(U"画像").drawAt(graphicsRect.center(), Palette::Black);
}
