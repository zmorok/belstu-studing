#pragma once
#include "resource.h"
#include "CMatrix.h"
#include <float.h> // DBL_MAX, DBL_MIN
#include <math.h>
#include <vector>

#include "LibGraph.h"
#include "LibSurface.h"


class CMainWnd : public CFrameWnd
{
private:
	CRect WinRect; // Область в окне
	int Index;
	CMatrix PView;
	CMatrix PView2;
	CMenu menu;
	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT);

	int delta = 10;

public:
	CMainWnd::CMainWnd()
	{
		Create(NULL, L"Lab07", WS_OVERLAPPEDWINDOW, CRect(10, 10, 700, 700), NULL, NULL);
		Index = 0;
		PView.RedimMatrix(3);
		PView2.RedimMatrix(3);
		PView(0) = 10; PView(1) = 45; PView(2) = 45;
		PView2(0) = 15; PView2(1) = 30; PView2(2) = 60;
	}

	COLORREF LightColor = (RGB(255, 255, 0));

	afx_msg void OnKeyDown(UINT, UINT, UINT);
	void OnPaint();
	void OnSize(UINT nType, int cx, int cy);

	void OnDiffuseModel();
	void OnMirrorModel();

	void OnCamera();
	void OnLight();
	void OnLightColor();
	void Exit();
};

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_COMMAND(40015, OnDiffuseModel)
	ON_COMMAND(40016, OnMirrorModel)

	ON_COMMAND(ID_CAMERA, OnCamera)
	ON_COMMAND(ID_LIGHT, OnLight)
	ON_COMMAND(ID_LIGHT_COLOR, OnLightColor)

	ON_COMMAND(ID_EXIT, Exit)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()
int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	menu.LoadMenu(IDR_MENU1); // Загрузить меню из файла ресурса
	SetMenu(&menu); // Установить меню
	return 0;
}
void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	WinRect.SetRect(0, cy - 300, 300, 0);
}

void CMainWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	switch (nChar) {
	case VK_UP: 
		PView(1) += delta;
		Invalidate();
		return;

	case VK_DOWN:
		PView(1) -= delta;
		Invalidate();
		break;
	case VK_RIGHT:
		PView(2) += delta;
		Invalidate();
		break;
	case VK_LEFT:
		PView(2) -= delta;
		Invalidate();
		break;
	case 'W':
		PView2(1) += delta;
		Invalidate();
		break;
	case 'S':
		PView2(1) -= delta;
		Invalidate();
		break;
	case 'D': 
		PView2(2) += delta;
		Invalidate();
		break;
	case 'A':
		PView2(2) -= delta;
		Invalidate();
		break;
	case '1':
		LightColor = RGB(255, 0, 0); // red
		Invalidate();
		break;
	case '2':
		LightColor = RGB(0, 255, 0); // green
		Invalidate();
		break;
	case '3':
		LightColor = RGB(0, 0, 255); // blue
		Invalidate();
		break;
	case '4':
		LightColor = RGB(255, 255, 255); // white
		Invalidate();
		break;
	case '5':
		LightColor = RGB(255, 0, 255); // pink
		Invalidate();
		break;
	case '6':
		LightColor = RGB(0, 255, 255); // skyblue
		Invalidate();
		break;
	default:
		break;
	}
}

void CMainWnd::OnPaint()
{
	CPaintDC dc(this);

	CString ss;
	ss.Format(L"PView: (%.0f, %.0f, %.0f)	PView2: (%.0f, %.0f, %.0f)", PView(0), PView(1), PView(2), PView2(0), PView2(1), PView2(2));
	dc.TextOutW(5, 5, ss);


	if (Index == 1)	///диф
		DrawLightSphere(dc, 7, PView, PView2, WinRect, LightColor, 0);
	if (Index == 2)	///зерк
		DrawLightSphere(dc, 7, PView, PView2, WinRect, LightColor, 1);

}


void CMainWnd::OnDiffuseModel()
{
	Index = 1;
	Invalidate();
}
void CMainWnd::OnMirrorModel()
{
	Index = 2;
	Invalidate();
}
void CMainWnd::OnCamera()
{
	PView(0) = 5; PView(1) = 45; PView(2) = 45;
	Invalidate();
}

void CMainWnd::OnLight()
{
	PView2(0) = 15; PView2(1) = 30; PView2(2) = 60;
	Invalidate();
}

void CMainWnd::OnLightColor()
{
	LightColor = (RGB(255, 122, 0));
	Invalidate();
}


void CMainWnd::Exit()
{
	DestroyWindow();
}