// include
#include "Main.h"
#include "Category.h"
#include "Game.h"

void Main()
{
	// 初期化処理
	Scene::SetBackground(ColorF(0.8, 0.9, 1.0));
	Window::SetTitle(AppInfo::windowTitle);
	Window::Resize(AppInfo::windowWidth, AppInfo::windowHeight);
	Window::SetStyle(WindowStyle::Frameless);

	// シーン管理初期化
	MyApp manager;
	manager.setFadeColor(AppInfo::fadeColor);
	manager.add<Category>(U"Category");
	manager.add<Games>(U"Games");

	// メインループ
	while (System::Update())
	{
		manager.update();
	}
}
