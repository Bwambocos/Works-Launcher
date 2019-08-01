// include
#pragma once
#include <Siv3D.hpp>
#include <Siv3D/Windows.hpp>
#undef CreateProcess

// 基本情報
namespace AppInfo
{
	const String windowTitle = U"Works Launcher v1.0";
	const int32 windowWidth = 800;
	const int32 windowHeight = 600;
	const Color fadeColor(200, 200, 200);
};

// 共有データ
struct AppData
{

};

using MyApp = SceneManager<String, AppData>;

// CreateProcess
namespace s3d
{
	class ProcessInfo
	{
	private:

		struct ProcessInfoDetail;

		std::shared_ptr<ProcessInfoDetail> m_detail;

	public:

		ProcessInfo();

		bool isRunning() const;

		Optional<int32> getExitCode() const;

		ProcessInfoDetail* _detail() const;
	};

	namespace System
	{
		ProcessInfo CreateProcess(const FilePath& applicationName, const String& params);
	}
}
