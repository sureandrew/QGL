#pragma once
#ifndef _REUBEN_PLATFORM_WINDOW_H_
#define _REUBEN_PLATFORM_WINDOW_H_

namespace Reuben { namespace Platform {

	//-- Must declare in subclass
#define REUBEN_DECLARE_WND_CLASS(ClassName)						\
	static PCSTRING GetWndClassName(VOID) { return (_T(#ClassName)); }

	template<typename _TYPE_>
	class Window
	{

		//-- Template Required Interface
		//{
		//	Boolean OnCreate(VOID);
		//	VOID OnDispose(VOID);
		//	Boolean ProcessWindowMessage(UInt uMsg, VOID* wParam, VOID* lParam, VOID*& lResult);
		//}

	protected:

		HWND m_hWnd;
		UInt m_uWidth;
		UInt m_uHeight;
		UInt32 m_uExStyle;
		UInt32 m_uStyle;

		static _TYPE_* sm_pWindow;

	public:

		virtual ~Window(VOID);

		Boolean Create(PCSTRING cszTitle, UInt uWidth, UInt uHeight, UInt32 uExStyle = 0,
			UInt32 uStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE);
		VOID Dispose(VOID);

		INLINE VOID SetTitle(PCSTRING pTitle);
		INLINE Boolean SetIcon(UInt32 uBigIcon, UInt32 uSmallIcon);
		INLINE VOID Show(VOID);
		INLINE VOID Hide(VOID);

		INLINE operator HWND(VOID) CONST throw();

	protected:
		
		Window(VOID);

	private:

		INLINE LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

		INLINE _TYPE_ & _This(VOID)
		{
			return (*((_TYPE_*)this));
		}

	private:

		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	};

	template<typename _TYPE_>
		_TYPE_* Window<_TYPE_>::sm_pWindow = NULL;

	template<typename _TYPE_>
		Window<_TYPE_>::Window(VOID)
		: m_hWnd(NULL)
		, m_uWidth(0)
		, m_uHeight(0)
		, m_uExStyle(0L)
		, m_uStyle(0L)
	{}

	template<typename _TYPE_>
		Window<_TYPE_>::~Window(VOID)
	{
		Dispose();
	}

	template<typename _TYPE_>
		Boolean Window<_TYPE_>::Create(
		PCSTRING pTitle, 
		UInt uWidth, 
		UInt uHeight, 
		UInt32 uExStyle,		//0
		UInt32 uStyle			//WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE
		)
	{
		if (uWidth == 0 || uHeight == 0)
			return (BOOLEAN_FALSE);

		HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(0);
		WNDCLASSEX wcex;
		if (!::GetClassInfoEx(hInstance, _TYPE_::GetWndClassName(), &wcex))
		{
			wcex.cbSize			= sizeof(WNDCLASSEX); 
			wcex.style			= CS_VREDRAW | CS_HREDRAW | CS_OWNDC | CS_DBLCLKS;
			wcex.lpfnWndProc	= (WNDPROC)Window<_TYPE_>::WindowProc;
			wcex.cbClsExtra		= 0;
			wcex.cbWndExtra		= 0;
			wcex.hInstance		= hInstance;
			wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(0));
			wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
			wcex.lpszMenuName	= NULL;
			wcex.lpszClassName	= _TYPE_::GetWndClassName();
			wcex.hIconSm		= LoadIcon(hInstance, MAKEINTRESOURCE(0));
			if(!RegisterClassEx(&wcex))
				return (BOOLEAN_FALSE);
		}

		RECT rect = {0, 0, uWidth, uHeight};
		::AdjustWindowRectEx(&rect, uStyle, FALSE, uExStyle);

		sm_pWindow = (_TYPE_*)this;

		m_hWnd = ::CreateWindowEx(uExStyle, _TYPE_::GetWndClassName(), pTitle, uStyle,
			(GetSystemMetrics(SM_CXSCREEN) - uWidth) / 2,
			(GetSystemMetrics(SM_CYSCREEN) - uHeight) / 2,
			rect.right - rect.left, rect.bottom- rect.top, 
			NULL, NULL, hInstance, NULL);

		if (m_hWnd == NULL)
			return (BOOLEAN_FALSE);

		m_uWidth = uWidth;
		m_uHeight = uHeight;
		m_uExStyle = uExStyle;
		m_uStyle = uStyle;

		if (_This().OnCreate() == BOOLEAN_FALSE)
		{
			Dispose();
			return (BOOLEAN_FALSE);
		}
		return (BOOLEAN_TRUE);
	}

	template<typename _TYPE_>
		VOID Window<_TYPE_>::Dispose()
	{
		if (m_hWnd == NULL)
			return;
		_This().OnDispose();
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		m_uWidth = C_UINT(0);
		m_uHeight = C_UINT(0);
		m_uExStyle = C_UINT32(0);
		m_uStyle = C_UINT32(0);
		DestroyWindow(hWnd);
		sm_pWindow = NULL;
		UnregisterClass(_TYPE_::GetWndClassName(), GetModuleHandle(0));
	}

	template<typename _TYPE_>
		INLINE VOID Window<_TYPE_>::SetTitle(PCSTRING pTitle)
	{
		if (m_hWnd == NULL)
			return;
		::SetWindowText(m_hWnd, pTitle);
	}

	template<typename _TYPE_>
		Boolean Window<_TYPE_>::SetIcon(UInt32 uBigIcon, UInt32 uSmallIcon)
	{
		if (m_hWnd == NULL)
			return (BOOLEAN_FALSE);

		HICON hIcon;
		HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(0);

		if ((hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(uBigIcon))))
			SendMessage(m_hWnd, WM_SETICON, TRUE, (LPARAM)hIcon);

		if ((hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(uSmallIcon))))
			SendMessage(m_hWnd, WM_SETICON, FALSE, (LPARAM)hIcon);

		return (BOOLEAN_TRUE);
	}

	template<typename _TYPE_>
		INLINE VOID Window<_TYPE_>::Show()
	{
		if (m_hWnd == NULL)
			return;
		::ShowWindow(m_hWnd, SW_SHOW);
	}

	template<typename _TYPE_>
		INLINE VOID Window<_TYPE_>::Hide()
	{
		if (m_hWnd == NULL)
			return;
		::ShowWindow(m_hWnd, SW_HIDE);
	}

	template<typename _TYPE_>
		INLINE Window<_TYPE_>::operator HWND() CONST throw()
	{
		return (m_hWnd);
	}

	template<typename _TYPE_>
		INLINE LRESULT Window<_TYPE_>::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return (::DefWindowProc(m_hWnd, uMsg, wParam, lParam));
	}

	template<typename _TYPE_>
		LRESULT CALLBACK Window<_TYPE_>::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		_TYPE_ * p = Window<_TYPE_>::sm_pWindow;
		if (p->m_hWnd == NULL)
			p->m_hWnd = hWnd;
		TRACE_CHECK(p->m_hWnd == hWnd);
		LRESULT lRes = 0;
		Boolean bRet = p->ProcessWindowMessage((UInt)uMsg, (VOID*)wParam, (VOID*)lParam , (VOID*&)lRes);
		if (uMsg == WM_CLOSE)
			p->Dispose();
		if (!bRet)
			return (p->DefWindowProc(uMsg, wParam, lParam));
		return (lRes);
	}

}; };  // Reuben::Platform

#endif // _REUBEN_PLATFORM_WINDOW_H_