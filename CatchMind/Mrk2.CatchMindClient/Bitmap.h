#pragma once
#include "defines.h"
class Bitmap
{
private:
	HDC m_hMemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOld;

	SIZE m_size;
public:
	void Init(HDC hdc, string strFileName);

	void DrawMidPoint(HDC hdc, int x, int y);
	void DrawMidPoint(HDC hdc, int x, int y, int TextCx, int TextCy);

	void DrawRTpoint(HDC hdc, int x, int y, int TextCx, int TextCy);

	void DrawTransparent(HDC hdc, int x, int y);
	void DrawTransparent(HDC hdc, int x, int y, int TextCx, int TextCy);

	void DrawBitblt(HDC hdc, int x, int y);

	inline SIZE GetSizeBitmap()
	{
		return m_size;
	}
	Bitmap();
	~Bitmap();
};
