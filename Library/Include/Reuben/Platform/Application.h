#pragma once
#ifndef _REUBEN_PLATFORM_APPLICATION_H_
#define _REUBEN_PLATFORM_APPLICATION_H_

namespace Reuben { namespace Platform {

	template<typename _TYPE_>
	class Application
	{

		//-- Template Required Interface
		//{
		//	Boolean OnCreate(VOID);
		//	VOID OnDispose(VOID);
		//	VOID OnUpdate(UInt32 uTimeDelta);
		//}

	protected:

		HINSTANCE m_hInstance;
		DWORD m_dwThreadId;

	public:

		VOID Run(VOID);
		INLINE VOID Terminate(Int nExitCode);

	protected:

		Application(VOID);

	private:

		INLINE _TYPE_ & _This(VOID)
		{
			return (*((_TYPE_ *)this));
		}

	};

	template<typename _TYPE_>
		Application<_TYPE_>::Application(VOID)
	{
		m_hInstance = (HINSTANCE)::GetModuleHandle(NULL);
		m_dwThreadId = ::GetCurrentThreadId();
	}

	template<typename _TYPE_>
		VOID Application<_TYPE_>::Run(VOID)
	{
		if (_This().OnCreate() == BOOLEAN_FALSE)
			PostQuitMessage(-1);

		MSG msg;
		memset(&msg, 0, sizeof(MSG));

		UInt32 uLastTime = ::timeGetTime();

		while(msg.message != WM_QUIT)
		{
			while (!::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE))
			{
				UInt32 uCurrentTime = ::timeGetTime();
				UInt32 uTimeDelta = uCurrentTime - uLastTime;

				_This().OnUpdate(uTimeDelta);

				uLastTime = uCurrentTime;
			}

			do
			{
				if (::GetMessage(&msg, NULL, NULL, NULL))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
				else if (msg.message == WM_QUIT)
					goto LABEL_EXIT;
			}
			while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE));
		}

LABEL_EXIT:

		_This().OnDispose();
	}

	template<typename _TYPE_>
		INLINE VOID Application<_TYPE_>::Terminate(Int nExitCode)
	{
		::PostThreadMessage(m_dwThreadId, WM_QUIT, (WPARAM)nExitCode, 0);
	}

}; }; // Reuben::Platform

#endif // _REUBEN_PLATFORM_APPLICATION_H_
