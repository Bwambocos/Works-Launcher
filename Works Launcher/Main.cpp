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

// CreateProcess
namespace s3d
{
	struct ProcessInfo::ProcessInfoDetail
	{
		HANDLE m_hProcess = nullptr;

		Optional<int32> m_exitCode;

		bool m_isRunning = false;
	};

	ProcessInfo::ProcessInfo()
		: m_detail(std::make_shared<ProcessInfoDetail>())
	{

	}

	ProcessInfo::ProcessInfoDetail* ProcessInfo::_detail() const
	{
		return m_detail.get();
	}

	bool ProcessInfo::isRunning() const
	{
		if (!m_detail->m_isRunning)
		{
			return false;
		}

		const DWORD r = ::WaitForSingleObject(m_detail->m_hProcess, 0);

		if (r == WAIT_TIMEOUT)
		{
			return true;
		}

		m_detail->m_isRunning = false;

		DWORD exitCode = 0;

		if (::GetExitCodeProcess(m_detail->m_hProcess, &exitCode))
		{
			m_detail->m_exitCode = exitCode;
		}

		return false;
	}

	Optional<int32> ProcessInfo::getExitCode() const
	{
		if (!m_detail->m_isRunning)
		{
			return m_detail->m_exitCode;
		}

		const DWORD r = ::WaitForSingleObject(m_detail->m_hProcess, 0);

		if (r == WAIT_TIMEOUT)
		{
			return none;
		}

		m_detail->m_isRunning = false;

		DWORD exitCode = 0;

		if (GetExitCodeProcess(m_detail->m_hProcess, &exitCode))
		{
			m_detail->m_exitCode = exitCode;
		}

		return m_detail->m_exitCode;
	}

	namespace System
	{
		ProcessInfo CreateProcess(const FilePath& applicationName, const String& params)
		{
			std::wstring command = params.toWstr();
			const std::wstring currentPath = FileSystem::ParentPath(applicationName).toWstr();

			STARTUPINFOW si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);

			PROCESS_INFORMATION pi = {};

			ProcessInfo info;

			if (::CreateProcessW(applicationName.toWstr().c_str(), &command[0], nullptr, nullptr, false, 0, nullptr, currentPath.c_str(), &si, &pi) == 0)
			{
				return info;
			}

			::CloseHandle(pi.hThread);

			info._detail()->m_isRunning = true;

			info._detail()->m_hProcess = pi.hProcess;

			return info;
		}
	}
}
