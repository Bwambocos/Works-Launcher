// include
#include "Category.h"
#include "CreateProcess.h"

// カテゴリー選択画面

// 初期化
Category::Category(const InitData& init) : IScene(init)
{
	largeFont = Font(42, U"data//fontR.ttc", FontStyle::Bold);
	midFont = Font(36, U"data//fontR.ttc", FontStyle::Bold);
	titleRect = Rect(5, 5, Scene::Width() - 10, largeFont.height() + 5);
	exitRect = Rect(5, Scene::Height() - largeFont.height() - 10, Scene::Width() - 10, largeFont.height() + 5);
	choicesRects << Rect(25, titleRect.y + titleRect.h + 25, (Scene::Width() - 100) / 3, Scene::Height() - titleRect.h - exitRect.h - 60);
	choicesRects << Rect(choicesRects[0].x + choicesRects[0].w + 25, choicesRects[0].y, choicesRects[0].w, choicesRects[0].h);
	choicesRects << Rect(choicesRects[1].x + choicesRects[1].w + 25, choicesRects[0].y, choicesRects[0].w, choicesRects[0].h);
	choicesImages << Texture(U"data//Category//musicImage.png");
	choicesImages << Texture(U"data//Category//gamesImage.png");
	choicesImages << Texture(U"data//Category//graphicsImage.png");
	choicesStrs << U"音楽";
	choicesStrs << U"ゲーム";
	choicesStrs << U"デザイン";
	const INIData configINI(U"data//config.ini");
	exitFlag = !configINI.get<bool>(U"Demo", U"flag");
}

// 更新
void Category::update()
{
	if (exitRect.mouseOver() || choicesRects[0].mouseOver() || choicesRects[1].mouseOver() || choicesRects[2].mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
	if (exitRect.leftClicked())
	{
		if (exitFlag)
		{
			if (System::ShowMessageBox(U"終了確認", U"本当に終了しますか？", MessageBoxButtons::YesNo) == MessageBoxSelection::Yes) System::Exit();
		}
		else System::ShowMessageBox(U"", U"展示用なので終了出来ません");
	}

	for (auto i : step(choicesNum))
	{
		if (choicesRects[i].leftClicked())
		{
			if (i == 0) s3dx::System::CreateProcess(U"data//MusicRoom//MusicRoom.exe", U"");
			if (i == 1) changeScene(U"Games");
			if (i == 2) s3dx::System::CreateProcess(U"data//CombViewer//CombViewer.exe", U"");
			INIData countINI(U"data//Category//logs.ini");
			if (countINI.isEmpty())
			{
				Print << U"Not found count.ini";
				continue;
			}
			countINI.write<int32>(U"count", choicesStrs[i], countINI.get<int32>(U"count", choicesStrs[i]) + 1);
			countINI.save(U"data//Category//logs.ini");
		}
	}
}

// 描画
void Category::draw() const
{
	titleRect.draw(Color(Palette::White, 200)).drawFrame(1, Palette::Dimgray);
	largeFont(U"作品の種類を選択して下さい").drawAt(titleRect.center(), Palette::Black);
	exitRect.draw(Color(Palette::White, (exitRect.mouseOver() ? 200 : 100))).drawFrame(1, Palette::Dimgray);
	largeFont(U"終了する").drawAt(exitRect.center(), Palette::Black);

	for (auto i : step(choicesNum))
	{
		if (choicesRects[i].mouseOver())
		{
			choicesRects[i]
				.stretched(6)
				.drawShadow(Vec2(0, 3), 8, 0)
				.draw(AppInfo::backgroundColor)
				.drawFrame(5, 0, ColorF(Palette::Red, 0.4 + Periodic::Sine0_1(1s) * 0.6));
		}
		else choicesRects[i].drawFrame(1, Palette::Dimgray);
		choicesRects[i](choicesImages[i]).draw(Color(255, 255, 255, (choicesRects[i].mouseOver() ? 200 : 100)));
		midFont(choicesStrs[i]).drawAt(choicesRects[i].center(), Palette::Black);
	}
}
