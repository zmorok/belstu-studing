#pragma once
#include "resource.h"
#include "CMatrix.h"
#include "LibGraph.h"
#include "LibPyramid.h"
#include <fstream>


class CMainWnd : public CFrameWnd
{
private:
	CPyramid PIR;  // Пирамида
	CRect WinRect; // Область в окне
	CMatrix PView; // Точка наблюдения в сферической СК
	CMatrix PLight;
	int Index;
	CMenu menu;
	DECLARE_MESSAGE_MAP()
	int OnCreate(LPCREATESTRUCT);
	COLORREF color;
	bool isLight;

public:
	CMainWnd::CMainWnd() 
	{
		Create(NULL, L"Lab09", WS_OVERLAPPEDWINDOW, CRect(10, 10, 700, 800), NULL, NULL);
		Index = 0;
		PView.RedimMatrix(3);
		PView(0) = 10; PView(1) = 45; PView(2) = 60;
		PLight.RedimMatrix(3);
		PLight(0) = 20; PLight(1) = 90; PLight(2) = 50;
		isLight = false;
	}

	void OnPaint();
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnSize(UINT nType, int cx, int cy);
	void OnPyramid();
	void Exit();
	void Cam();
	void Light();
	void ColRed();
	void ColBlue();
	void ColGreen();
	void ColOrange();
	void ColPink();
	void DoLight();
	void DoCam();
	void ColR();
	void ColG();
	void ColB();
};

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_COMMAND(ID_30002, OnPyramid)
	ON_COMMAND(ID_EXIT, Exit)
	ON_COMMAND(ID_30005, Cam)
	ON_COMMAND(ID_30006, Light)
	ON_COMMAND(ID_30008, ColRed)
	ON_COMMAND(ID_30009, ColGreen)
	ON_COMMAND(ID_30010, ColBlue)
	ON_COMMAND(ID_30012, ColOrange)
	ON_COMMAND(ID_30013, ColPink)
END_MESSAGE_MAP()

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	menu.LoadMenu(IDR_MENU1); // Загрузить меню из файла ресурса
	SetMenu(&menu); // Установить меню
	color = RGB(255, 0, 0);
	return 0;
}


void CMainWnd::OnPaint()
{
	CPaintDC dc(this);
	if (Index == 1) {
		PIR.ColorDraw(dc, PView, PLight, WinRect, color);	///коор.камеры, область в окне
		CString ss;
		double p = PView(0), fi = PView(1), th = PView(2);
		ss.Format(L"Наблюдатель: r=%.1f, fi=%.1f, th=%.1f", p, fi, th);
		dc.TextOutW(10, 0, ss);
		p = PLight(0), fi = PLight(1), th = PLight(2);
		ss.Format(L"Источник света: r=%.1f, fi=%.1f, th=%.1f", p, fi, th);
		dc.TextOutW(10, 50, ss);
		/*ss.Format(L"Цвет: r=%3d, g=%3d, b=%3d", GetRValue(color), GetGValue(color), GetBValue(color));
		dc.TextOutW(200, 0, ss);*/
	}
}

void CMainWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (Index == 1)
	{
		switch (nChar)
		{
		case VK_DOWN:
		{
			double d = PView(2) - 3;
			if (d >= 0)
				PView(2) = d;
			break;
		}
		case VK_UP:
		{
			double d = PView(2) + 3;
			if (d <= 180)
				PView(2) = d;
			break;
		}
		case VK_RIGHT:
		{
			double d = PView(1) - 3;
			if (d >= -180)
				PView(1) = d;
			else
				PView(1) = d + 360;
			break;
		}
		case VK_LEFT:
		{
			double d = PView(1) + 3;
			if (d <= 180)
				PView(1) = d;
			else
				PView(1) = d - 360;
			break;
		}
		case 'S':
		{
			double d = PLight(2) - 3;
			if (d >= 0)
				PLight(2) = d;
			break;
		}
		case 'W':
		{
			double d = PLight(2) + 3;
			if (d <= 180)
				PLight(2) = d;
			break;
		}
		case 'D':
		{
			double d = PLight(1) - 3;
			if (d >= -180)
				PLight(1) = d;
			else
				PLight(1) = d + 360;
			break;
		}
		case 'A':
		{
			double d = PLight(1) + 3;
			if (d <= 180)
				PLight(1) = d;
			else
				PLight(1) = d - 360;
			break;
		}
		case VK_NUMPAD1:
			ColRed();
			break;
		case VK_NUMPAD2:
			ColGreen();
			break;
		case VK_NUMPAD3:
			ColBlue();
			break;
		case VK_NUMPAD4:
			ColOrange();
			break;
		case VK_NUMPAD5:
			ColPink();
			break;
		case VK_NUMPAD7:
			ColR();
			break;
		case VK_NUMPAD8:
			ColG();
			break;
		case VK_NUMPAD9:
			ColB();
			break;
		}
		Invalidate();
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	WinRect.SetRect(100, 100, cx - 100, cy - 100);
}
void CMainWnd::OnPyramid()
{
	Index = 1;
	Invalidate();
}
void CMainWnd::Exit()
{
	DestroyWindow();
}

void CMainWnd::Cam() {
	std::fstream fin("dataCam.txt");
	fin >> PView(1) >> PView(2);
	Invalidate();
	OnPaint();
}

void CMainWnd::Light() {
	std::fstream fin("dataLight.txt");
	fin >> PLight(1) >> PLight(2);
	Invalidate();
	OnPaint();
}

void CMainWnd::ColRed() {
	color = RGB(255, 0, 0);
	Invalidate();
	OnPaint();
}

void CMainWnd::ColGreen() {
	color = RGB(0, 255, 0);
	Invalidate();
	OnPaint();
}

void CMainWnd::ColBlue() {
	color = RGB(0, 0, 255);
	Invalidate();
	OnPaint();
}

void CMainWnd::ColOrange() {
	color = RGB(255, 128, 0);
	Invalidate();
	OnPaint();
}

void CMainWnd::ColPink() {
	color = RGB(255, 0, 127);
	Invalidate();
	OnPaint();
}

void CMainWnd::DoCam() {
	isLight = false;
}

void CMainWnd::DoLight() {
	isLight = true;
}

void CMainWnd::ColR() {
	color = RGB((GetRValue(color) + 10) % 256, GetGValue(color), GetBValue(color));
	Invalidate();
	OnPaint();
}

void CMainWnd::ColG() {
	color = RGB(GetRValue(color), (GetGValue(color) + 10) % 256, GetBValue(color));
	Invalidate();
	OnPaint();
}

void CMainWnd::ColB() {
	color = RGB(GetRValue(color), GetGValue(color), (GetBValue(color) + 10) % 256);
	Invalidate();
	OnPaint();
}