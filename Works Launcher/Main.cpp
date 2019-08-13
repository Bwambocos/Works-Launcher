// include
#include "Main.h"
#include "Category.h"
#include "Games.h"

void Main()
{
	// 初期化処理
	Window::SetTitle(AppInfo::windowTitle);
	Window::Resize(AppInfo::windowWidth, AppInfo::windowHeight);
	Window::SetStyle(WindowStyle::Frameless);

	// シーン管理初期化
	Scene::SetBackground((*manager.get()).schemeColor1);
	manager.setFadeColor((*manager.get()).schemeColor1);
	manager.add<Category>(U"Category");
	manager.add<Games>(U"Games");

	// メインループ
	while (System::Update())
	{
		manager.update();
	}
}

// ダークモードとライトモードを切り替える
void setDrawMode(AppData& data)
{
	(++data.drawMode) %= 2;
	if (data.drawMode == 0)
	{
		data.schemeColor1 = Color(22, 22, 22);
		data.schemeColor2 = Color(66, 66, 66);
		data.schemeColor3 = Color(110, 110, 110);
		data.schemeColor4 = Color(154, 154, 154);
		data.schemeColor5 = Color(198, 198, 198);
		data.stringColor = Palette::White;
	}
	else
	{
		data.schemeColor1 = Color(64, 138, 241);
		data.schemeColor2 = Color(90, 162, 246);
		data.schemeColor3 = Color(90, 162, 246);
		data.schemeColor4 = Color(136, 203, 250);
		data.schemeColor5 = Color(168, 215, 250);
		data.stringColor = Palette::Black;
	}
	Scene::SetBackground(data.schemeColor1);
	manager.setFadeColor(data.schemeColor1);
}
