#pragma once
#ifndef _QGL_WINDOW_H_
#define _QGL_WINDOW_H_

#include <Reuben\Platform\Window.h>
#include "Render.h"

#define WM_SHORTCUTKEY		WM_APP + 1000

class QGLWindow
	: public Reuben::Platform::Window<QGLWindow>
{
public:

	REUBEN_DECLARE_WND_CLASS(QGLWindow);

private:

	struct PImpl;
	PImpl * _pImpl;

public:

	QGLWindow(VOID);
	virtual ~QGLWindow(VOID);

	Boolean OnCreate(VOID);
	VOID OnDispose(VOID);
	Boolean ProcessWindowMessage(UInt &uMsg, VOID * wParam, VOID * lParam, VOID * & lResult);

	CRender * GetRenderPtr();

	VOID MainKeyDown(WPARAM  wParam);

	BOOL m_AutoJumppointWalk;

	inline HWND GetWindow(){ return m_hWnd;}

};

#endif // _QGL_WINDOW_H_
