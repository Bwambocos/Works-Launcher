//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2019 Ryo Suzuki
//	Copyright (c) 2016-2019 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include <Siv3D.hpp>

namespace s3dx
{
	class ProcessInfo
	{
	private:

		struct ProcessInfoDetail;

		std::shared_ptr<ProcessInfoDetail> m_detail;

	public:

		ProcessInfo();

		bool isRunning() const;

		Optional<s3d::int32> getExitCode() const;

		ProcessInfoDetail* _detail() const;
	};

	namespace System
	{
		ProcessInfo CreateProcess(const s3d::FilePath& applicationName, const s3d::String& params = U"");
	}
}
