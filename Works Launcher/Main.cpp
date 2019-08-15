// include
#include "Main.h"
#include "Category.h"
#include "Games.h"
#include "Background.h"

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
	Background bg;
	bg.init();
	while (System::Update())
	{
		auto flag = bg.update(*(manager.get()));
		bg.draw(*(manager.get()));
		if (flag) manager.update();
	}
}
