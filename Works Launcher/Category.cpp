// include
#include "Category.h"
#include "CreateProcess.h"

// カテゴリー選択画面

// 初期化
Category::Category(const InitData& init) : IScene(init)
{
	FontAsset::Register(U"Category-largeFont", 42, U"data//fontR.ttc", FontStyle::Bold);
	FontAsset::Register(U"Category-midFont", 36, U"data//fontR.ttc", FontStyle::Bold);
	titleRect = Rect(5, 5, Scene::Width() - 10, FontAsset(U"Category-largeFont").height() + 5);
	exitRect = Rect(5, Scene::Height() - FontAsset(U"Category-largeFont").height() - 10, Scene::Width() - 10, FontAsset(U"Category-largeFont").height() + 5);
	choicesRects << Rect(25, titleRect.y + titleRect.h + 25, (Scene::Width() - 100) / 3, Scene::Height() - titleRect.h - exitRect.h - 60);
	choicesRects << Rect(choicesRects[0].x + choicesRects[0].w + 25, choicesRects[0].y, choicesRects[0].w, choicesRects[0].h);
	choicesRects << Rect(choicesRects[1].x + choicesRects[1].w + 25, choicesRects[0].y, choicesRects[0].w, choicesRects[0].h);
	TextureAsset::Register(U"choicesImage0", U"data//Category//musicImage.png");
	TextureAsset::Register(U"choicesImage1", U"data//Category//gamesImage.png");
	TextureAsset::Register(U"choicesImage2", U"data//Category//graphicsImage.png");
	choicesStrs << U"音楽";
	choicesStrs << U"ゲーム";
	choicesStrs << U"デザイン";
	const INIData configINI(U"data//config.ini");
	exitFlag = !configINI.get<bool>(U"Demo", U"flag");
	musicPass = configINI.get<String>(U"Pass", U"music");
	graphicsPass = configINI.get<String>(U"Pass", U"graphics");
}

// 更新
void Category::update()
{
	if (exitRect.mouseOver() || choicesRects[0].mouseOver() || choicesRects[1].mouseOver() || choicesRects[2].mouseOver()) Cursor::RequestStyle(CursorStyle::Hand);
	if (exitRect.leftClicked())
	{
		AudioAsset(U"cursorAudio").stop();
		AudioAsset(U"cursorAudio").play();
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
			AudioAsset(U"cursorAudio").stop();
			AudioAsset(U"cursorAudio").play();
			if (i == 0) s3dx::System::CreateProcess(U"Games//" + musicPass + U"//" + musicPass + U".exe", U"");
			if (i == 1) changeScene(U"Games");
			if (i == 2) s3dx::System::CreateProcess(U"Games//" + graphicsPass + U"//" + graphicsPass + U".exe", U"");
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
	titleRect.draw(Color(getData().schemeColor2, 200)).drawFrame(1, getData().schemeColor3);
	FontAsset(U"Category-largeFont")(U"作品の種類を選択して下さい").drawAt(titleRect.center(), getData().stringColor);
	exitRect.draw(Color(getData().schemeColor2, (exitRect.mouseOver() ? 200 : 100))).drawFrame(1, getData().schemeColor3);
	FontAsset(U"Category-largeFont")(U"終了する").drawAt(exitRect.center(), getData().stringColor);

	for (auto i : step(choicesNum))
	{
		if (choicesRects[i].mouseOver())
		{
			choicesRects[i]
				.stretched(6)
				.drawShadow(Vec2(0, 3), 8, 0)
				.draw(getData().schemeColor1)
				.drawFrame(5, 0, ColorF(getData().schemeColor5, 0.4 + Periodic::Sine0_1(1s) * 0.6));
		}
		else choicesRects[i].drawFrame(1, getData().schemeColor4);
		choicesRects[i](TextureAsset(U"choicesImage" + Format(i))).draw(Color(getData().schemeColor3, (choicesRects[i].mouseOver() ? 200 : 100)));
		FontAsset(U"Category-midFont")(choicesStrs[i]).drawAt(choicesRects[i].center(), getData().stringColor);
	}
}
