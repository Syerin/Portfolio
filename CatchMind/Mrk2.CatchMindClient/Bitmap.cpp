#include "Bitmap.h"



Bitmap::Bitmap()
{
}


Bitmap::~Bitmap()
{
	SelectObject(m_hMemDC, m_hOld);
	DeleteObject(m_hBitmap);
	SAFE_DELETE(m_hMemDC);
}

void Bitmap::Init(HDC hdc, string strFIleName)
{
	m_hMemDC = CreateCompatibleDC(hdc);
	m_hBitmap = (HBITMAP)LoadImage(NULL, strFIleName.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	BITMAP bmp_Info;
	GetObject(m_hBitmap, sizeof(bmp_Info), &bmp_Info);

	m_size.cx = (int)bmp_Info.bmWidth;
	m_size.cy = (int)bmp_Info.bmHeight;
}
void Bitmap::DrawMidPoint(HDC hdc, int x, int y)
{
	m_hOld = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	TransparentBlt(hdc, x-(m_size.cx/2), y- (m_size.cy / 2), m_size.cx, m_size.cy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
	SelectObject(m_hMemDC, m_hOld);

}

void Bitmap::DrawRTpoint(HDC hdc, int x, int y, int TextCx, int TextCy)
{
	m_hOld = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	TransparentBlt(hdc, x - (m_size.cx) - TextCx , y , m_size.cx + TextCx, m_size.cy + TextCy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
	SelectObject(m_hMemDC, m_hOld);

}
void Bitmap::DrawMidPoint(HDC hdc, int x, int y, int TextCx, int TextCy)
{
	m_hOld = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	TransparentBlt(hdc, x - (m_size.cx / 2) - TextCx/2, y - (m_size.cy / 2), m_size.cx + TextCx, m_size.cy + TextCy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
	SelectObject(m_hMemDC, m_hOld);

}
void Bitmap::DrawTransparent(HDC hdc, int x, int y)
{
	m_hOld = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	TransparentBlt(hdc, x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, m_size.cx,m_size.cy,RGB(255,0,255));
	SelectObject(m_hMemDC, m_hOld);
}
void Bitmap::DrawTransparent(HDC hdc, int x, int y,int TextCx,int TextCy)
{
	m_hOld = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	TransparentBlt(hdc, x, y, m_size.cx+TextCx, m_size.cy+ TextCy, m_hMemDC, 0, 0, m_size.cx, m_size.cy, RGB(255, 0, 255));
	SelectObject(m_hMemDC, m_hOld);
}

void Bitmap::DrawBitblt(HDC hdc, int x, int y)
{
	m_hOld = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	BitBlt(hdc, x, y, m_size.cx, m_size.cy, m_hMemDC, 0, 0, SRCCOPY);
	SelectObject(m_hMemDC, m_hOld);
}

