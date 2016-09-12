//
//Copyright(c) 2016. Huan Xia
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//documentation files(the "Software"), to deal in the Software without restriction, including without limitation
//the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software,
//and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions
//of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
//TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL
//THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//DEALINGS IN THE SOFTWARE.


#include "StdAfx.h"
#include "STXGraphics.h"
#include <shlwapi.h>
#if _WIN32_WINNT >= 0x0601
#include <d2d1effects.h>
#include <D2d1_1.h>
#include <d2d1helper.h>
#include <VersionHelpers.h>
#endif
#include <map>
#include <typeinfo.h>

#define _USE_MATH_DEFINES
#include <math.h>

//////////////////////////////////////////////////////////////////////////

#define RETURN_NULL_IF_FAIL(xhresult)\
	if(FAILED(xhresult))\
		return NULL;

//////////////////////////////////////////////////////////////////////////

// 1 value only
template <typename A>
int CompareValue(A a1, A a2)
{
	if (a1 > a2)
		return 1;
	else if (a1 < a2)
		return -1;

	return 0;
}

// 2 values
template <typename A, typename B>
int CompareValue(A a1, A a2, B b1, B b2)
{
	if (a1 > a2)
		return 1;
	else if (a1 < a2)
		return -1;

	if (b1 > b2)
		return 1;
	else if (b1 < b2)
		return -1;

	return 0;
}

// 3 values
template <typename A, typename B, typename C>
int CompareValue(A a1, A a2, B b1, B b2, C c1, C c2)
{
	if (a1 > a2)
		return 1;
	else if (a1 < a2)
		return -1;

	if (b1 > b2)
		return 1;
	else if (b1 < b2)
		return -1;

	if (c1 > c2)
		return 1;
	else if (c1 < c2)
		return -1;

	return 0;
}

// 4 values
template <typename A, typename B, typename C, typename D>
int CompareValue(A a1, A a2, B b1, B b2, C c1, C c2, D d1, D d2)
{
	if (a1 > a2)
		return 1;
	else if (a1 < a2)
		return -1;

	if (b1 > b2)
		return 1;
	else if (b1 < b2)
		return -1;

	if (c1 > c2)
		return 1;
	else if (c1 < c2)
		return -1;

	if (d1 > d2)
		return 1;
	else if (d1 < d2)
		return -1;

	return 0;
}

// 5 values
template <typename A, typename B, typename C, typename D, typename E>
int CompareValue(A a1, A a2, B b1, B b2, C c1, C c2, D d1, D d2, E e1, E e2)
{
	if (a1 > a2)
		return 1;
	else if (a1 < a2)
		return -1;

	if (b1 > b2)
		return 1;
	else if (b1 < b2)
		return -1;

	if (c1 > c2)
		return 1;
	else if (c1 < c2)
		return -1;

	if (d1 > d2)
		return 1;
	else if (d1 < d2)
		return -1;

	if (e1 > e2)
		return 1;
	else if (e1 < e2)
		return -1;

	return 0;
}

#define PARAM_ENTRY(xxType, xxVar)	xxType xxVar##1,xxType xxVar##2

#define COMPARE_ENTRY(xxVar)\
		if (xxVar##1 > xxVar##2)\
			return 1;\
		else if (xxVar##1 < xxVar##2)\
			return -1;

// 6 Values
template <typename A, typename B, typename C, typename D, typename E, typename F>
int CompareValue(PARAM_ENTRY(A, a), PARAM_ENTRY(B, b), PARAM_ENTRY(C, c), PARAM_ENTRY(D, d), PARAM_ENTRY(E, e), PARAM_ENTRY(F, f))
{
	COMPARE_ENTRY(a);
	COMPARE_ENTRY(b);
	COMPARE_ENTRY(c);
	COMPARE_ENTRY(d);
	COMPARE_ENTRY(e);
	COMPARE_ENTRY(f);
	return 0;
}

// 8 Values
template <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H>
int CompareValue(PARAM_ENTRY(A, a), PARAM_ENTRY(B, b), PARAM_ENTRY(C, c), PARAM_ENTRY(D, d), PARAM_ENTRY(E, e), PARAM_ENTRY(F, f), PARAM_ENTRY(G, g), PARAM_ENTRY(H, h))
{
	COMPARE_ENTRY(a);
	COMPARE_ENTRY(b);
	COMPARE_ENTRY(c);
	COMPARE_ENTRY(d);
	COMPARE_ENTRY(e);
	COMPARE_ENTRY(f);
	COMPARE_ENTRY(g);
	COMPARE_ENTRY(h);
	return 0;
}


// 9 Values
template <typename A, typename B, typename C, typename D, typename E, typename F, typename G, typename H, typename I>
int CompareValue(PARAM_ENTRY(A, a), PARAM_ENTRY(B, b), PARAM_ENTRY(C, c), PARAM_ENTRY(D, d), PARAM_ENTRY(E, e), PARAM_ENTRY(F, f), PARAM_ENTRY(G, g), PARAM_ENTRY(H, h), PARAM_ENTRY(I, i))
{
	COMPARE_ENTRY(a);
	COMPARE_ENTRY(b);
	COMPARE_ENTRY(c);
	COMPARE_ENTRY(d);
	COMPARE_ENTRY(e);
	COMPARE_ENTRY(f);
	COMPARE_ENTRY(g);
	COMPARE_ENTRY(h);
	COMPARE_ENTRY(i);
	return 0;
}

// 12 Values
template <typename A, typename B, typename C, typename D, typename E, typename F, typename A1, typename B1, typename C1, typename D1, typename E1, typename F1>
int CompareValue(PARAM_ENTRY(A, a), PARAM_ENTRY(B, b), PARAM_ENTRY(C, c), PARAM_ENTRY(D, d), PARAM_ENTRY(E, e), PARAM_ENTRY(F, f), PARAM_ENTRY(A1, a1), PARAM_ENTRY(B1, b1), PARAM_ENTRY(C1, c1), PARAM_ENTRY(D1, d1), PARAM_ENTRY(E1, e1), PARAM_ENTRY(F1, f1))
{
	COMPARE_ENTRY(a);
	COMPARE_ENTRY(b);
	COMPARE_ENTRY(c);
	COMPARE_ENTRY(d);
	COMPARE_ENTRY(e);
	COMPARE_ENTRY(f);
	COMPARE_ENTRY(a1);
	COMPARE_ENTRY(b1);
	COMPARE_ENTRY(c1);
	COMPARE_ENTRY(d1);
	COMPARE_ENTRY(e1);
	COMPARE_ENTRY(f1);
	return 0;
}

// 13 Values
template <typename A, typename B, typename C, typename D, typename E, typename F, typename A1, typename B1, typename C1, typename D1, typename E1, typename F1, typename G>
int CompareValue(PARAM_ENTRY(A, a), PARAM_ENTRY(B, b), PARAM_ENTRY(C, c), PARAM_ENTRY(D, d), PARAM_ENTRY(E, e), PARAM_ENTRY(F, f), PARAM_ENTRY(A1, a1), PARAM_ENTRY(B1, b1), PARAM_ENTRY(C1, c1), PARAM_ENTRY(D1, d1), PARAM_ENTRY(E1, e1), PARAM_ENTRY(F1, f1), PARAM_ENTRY(G, g))
{
	COMPARE_ENTRY(a);
	COMPARE_ENTRY(b);
	COMPARE_ENTRY(c);
	COMPARE_ENTRY(d);
	COMPARE_ENTRY(e);
	COMPARE_ENTRY(f);
	COMPARE_ENTRY(a1);
	COMPARE_ENTRY(b1);
	COMPARE_ENTRY(c1);
	COMPARE_ENTRY(d1);
	COMPARE_ENTRY(e1);
	COMPARE_ENTRY(f1);
	COMPARE_ENTRY(g);
	return 0;
}

//////////////////////////////////////////////////////////////////////////

CSTXGraphicsObject::CSTXGraphicsObject()
{
	m_nRef = 1;
}

CSTXGraphicsObject::~CSTXGraphicsObject()
{

}

STDMETHODIMP CSTXGraphicsObject::QueryInterface(const IID & iid, void **ppv)
{
	return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CSTXGraphicsObject::Release(void)
{
	ULONG l;
	l = InterlockedDecrement(&m_nRef);
	if (0 == l)
		delete this;
	return l;
}

STDMETHODIMP_(ULONG) CSTXGraphicsObject::AddRef(void)
{
	return InterlockedIncrement(&m_nRef);
}

int CSTXGraphicsObject::CompareTo(const CSTXGraphicsObject *pObject) const
{
	int nDirectCompare = strcmp(typeid(*this).raw_name(), typeid(*pObject).raw_name());
	if (nDirectCompare != 0)
		return nDirectCompare;

	//if (this < pObject)
	//	return -1;
	//else if (this > pObject)
	//	return 1;

	return 0;
}

//////////////////////////////////////////////////////////////////////////

CSTXGraphicsBrush::CSTXGraphicsBrush()
{
}

CSTXGraphicsBrush::~CSTXGraphicsBrush()
{
}

void CSTXGraphicsBrush::SetOpacity(byte alpha)
{
	SetOpacityFloat(alpha / 255.0f);
}

void CSTXGraphicsBrush::SetOpacityFloat(float alpha)
{

}



//////////////////////////////////////////////////////////////////////////

CSTXGdiPlusGraphicsBrush::CSTXGdiPlusGraphicsBrush()
{
	_brush = NULL;
}

CSTXGdiPlusGraphicsBrush::~CSTXGdiPlusGraphicsBrush()
{
	if (_brush)
		delete _brush;
}

Gdiplus::Brush* CSTXGdiPlusGraphicsBrush::GetGdiPlusBrush()
{
	return _brush;
}

//////////////////////////////////////////////////////////////////////////

int CSTXGdiPlusGraphicsSolidBrush::CompareTo(const CSTXGraphicsObject *pObject) const
{
	int nSuperCompare = __super::CompareTo(pObject);
	if (nSuperCompare != 0)
		return nSuperCompare;

	const CSTXGdiPlusGraphicsSolidBrush *pBrush = dynamic_cast<const CSTXGdiPlusGraphicsSolidBrush*>(pObject);
	if (pBrush)
	{
		return CompareValue(_r, pBrush->_r, _g, pBrush->_g, _b, pBrush->_b, _a, pBrush->_a);
	}
	return 0;
}

void CSTXGdiPlusGraphicsSolidBrush::SetOpacityFloat(float alpha)
{
	if (_brush)
		delete _brush;
	_brush = new Gdiplus::SolidBrush(Gdiplus::Color(static_cast<byte>(alpha * 255), _r, _g, _b));
}

//////////////////////////////////////////////////////////////////////////

int CSTXGdiPlusGraphicsButtonGradientBrush::CompareTo(const CSTXGraphicsObject *pObject) const
{
	int nSuperCompare = __super::CompareTo(pObject);
	if (nSuperCompare != 0)
		return nSuperCompare;

	const CSTXGdiPlusGraphicsButtonGradientBrush *pBrush = dynamic_cast<const CSTXGdiPlusGraphicsButtonGradientBrush*>(pObject);
	if (pBrush)
	{
		return CompareValue(_x1, pBrush->_x1, _y1, pBrush->_y1, _x2, pBrush->_x2, _y2, pBrush->_y2,
			_r, pBrush->_r, _g, pBrush->_g, _b, pBrush->_b, _a, pBrush->_a, _angle, pBrush->_angle);
	}
	return 0;
}

void CSTXGdiPlusGraphicsButtonGradientBrush::SetOpacityFloat(float alpha)
{
	if (_brush)
		delete _brush;

	byte topR = _r + (255 - _r) * 8 / 10;
	byte topG = _g + (255 - _g) * 8 / 10;
	byte topB = _b + (255 - _b) * 8 / 10;

	byte topCenterR = _r + (255 - _r) / 8;
	byte topCenterG = _g + (255 - _g) / 8;
	byte topCenterB = _b + (255 - _b) / 8;


	byte bottomCenterR = _r * 8 / 10;
	byte bottomCenterG = _g * 8 / 10;
	byte bottomCenterB = _b * 8 / 10;

	byte bottomR = _r / 3;
	byte bottomG = _g / 3;
	byte bottomB = _b / 3;

	Gdiplus::Color colors[] = {
		Gdiplus::Color(static_cast<byte>(alpha * 255), topR, topG, topB),
		Gdiplus::Color(static_cast<byte>(alpha * 255), topCenterR, topCenterG, topCenterB),
		Gdiplus::Color(static_cast<byte>(alpha * 255), bottomCenterR, bottomCenterG, bottomCenterB),
		Gdiplus::Color(static_cast<byte>(alpha * 255), bottomR, bottomG, bottomB) };

	Gdiplus::REAL positions[] = {
		0.0f,
		0.499f,
		0.501f,
		1.0f };


	int xpatch = _x2 > _x1 ? 1 : -1;
	int ypatch = _y2 > _y1 ? 1 : -1;

	//_angle = atan2((_y2 - _y1), (_x2 - _x1)) * 180 / M_PI;

	Gdiplus::LinearGradientBrush *pGradientBrush = new Gdiplus::LinearGradientBrush(Gdiplus::Rect(_x1 - xpatch, _y1 - ypatch, _x2 - _x1 + xpatch * 2, _y2 - _y1 + ypatch * 2),
		Gdiplus::Color(static_cast<byte>(alpha * 255), 0, 0, 0),
		Gdiplus::Color(static_cast<byte>(alpha * 255), 255, 25, 255),
		_angle);

	pGradientBrush->SetInterpolationColors(colors, positions, 4);

	_brush = pGradientBrush;
}

//////////////////////////////////////////////////////////////////////////

int CSTXGdiPlusGraphicsSimpleLinearGradientBrush::CompareTo(const CSTXGraphicsObject *pObject) const
{
	int nSuperCompare = __super::CompareTo(pObject);
	if (nSuperCompare != 0)
		return nSuperCompare;

	const CSTXGdiPlusGraphicsSimpleLinearGradientBrush *pBrush = dynamic_cast<const CSTXGdiPlusGraphicsSimpleLinearGradientBrush*>(pObject);
	if (pBrush)
	{
		int cmpResult = CompareValue(_x1, pBrush->_x1, _y1, pBrush->_y1, _x2, pBrush->_x2, _y2, pBrush->_y2,
			_r1, pBrush->_r1,
			_g1, pBrush->_g1,
			_b1, pBrush->_b1,
			_a1, pBrush->_a1,
			_r2, pBrush->_r2,
			_g2, pBrush->_g2,
			_b2, pBrush->_b2,
			_a2, pBrush->_a2);

		if (cmpResult == 0)
		{
			float fDiff = _angle - pBrush->_angle;
			if (fDiff < -0.001)
				return -1;
			if (fDiff > 0.001)
				return 1;
		}
		else
		{
			return cmpResult;
		}
	}
	return 0;
}

void CSTXGdiPlusGraphicsSimpleLinearGradientBrush::SetOpacityFloat(float alpha)
{
	if (_brush)
		delete _brush;

	int xpatch = _x2 >= _x1 ? 1 : -1;
	int ypatch = _y2 >= _y1 ? 1 : -1;

	int dx = _x2 - _x1;
	int dy = _y2 - _y1;

	float wdx = sqrt(dx * dx + dy * dy);
	float wdy = abs(dy);
	if (wdy == 0)
		wdy = 1;

	//_angle = atan2((_y2 - _y1), (_x2 - _x1)) * 180 / M_PI;

	_brush = new Gdiplus::LinearGradientBrush(Gdiplus::Rect(_x1, _y1, wdx, wdy),
		Gdiplus::Color(static_cast<byte>(_a1 * alpha), _r1, _g1, _b1),
		Gdiplus::Color(static_cast<byte>(_a2 * alpha), _r2, _g2, _b2),
		_angle);
}

//////////////////////////////////////////////////////////////////////////

int CSTXD2DGraphicsSolidBrush::CompareTo(const CSTXGraphicsObject *pObject) const
{
	int nSuperCompare = __super::CompareTo(pObject);
	if (nSuperCompare != 0)
		return nSuperCompare;

	const CSTXD2DGraphicsSolidBrush *pBrush = dynamic_cast<const CSTXD2DGraphicsSolidBrush*>(pObject);
	if (pBrush)
	{
		return CompareValue(_r, pBrush->_r, _g, pBrush->_g, _b, pBrush->_b);
		//return CompareValue(_r, pBrush->_r, _g, pBrush->_g, _b, pBrush->_b, _a, pBrush->_a);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

int CSTXD2DGraphicsButtonGradientBrush::CompareTo(const CSTXGraphicsObject *pObject) const
{
	int nSuperCompare = __super::CompareTo(pObject);
	if (nSuperCompare != 0)
		return nSuperCompare;

	const CSTXD2DGraphicsButtonGradientBrush *pBrush = dynamic_cast<const CSTXD2DGraphicsButtonGradientBrush*>(pObject);
	if (pBrush)
	{
		return CompareValue(_x1, pBrush->_x1, _y1, pBrush->_y1, _x2, pBrush->_x2, _y2, pBrush->_y2,
			_r, pBrush->_r, _g, pBrush->_g, _b, pBrush->_b, _angle, pBrush->_angle);

		//return CompareValue(_x1, pBrush->_x1, _y1, pBrush->_y1, _x2, pBrush->_x2, _y2, pBrush->_y2,
		//	_r, pBrush->_r, _g, pBrush->_g, _b, pBrush->_b, _a, pBrush->_a, _angle, pBrush->_angle);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

int CSTXD2DGraphicsSimpleLinearGradientBrush::CompareTo(const CSTXGraphicsObject *pObject) const
{
	int nSuperCompare = __super::CompareTo(pObject);
	if (nSuperCompare != 0)
		return nSuperCompare;

	const CSTXD2DGraphicsSimpleLinearGradientBrush *pBrush = dynamic_cast<const CSTXD2DGraphicsSimpleLinearGradientBrush*>(pObject);
	if (pBrush)
	{
		return CompareValue(_x1, pBrush->_x1, _y1, pBrush->_y1, _x2, pBrush->_x2, _y2, pBrush->_y2,
			_r1, pBrush->_r1,
			_g1, pBrush->_g1,
			_b1, pBrush->_b1,
			_a1, pBrush->_a1,
			_r2, pBrush->_r2,
			_g2, pBrush->_g2,
			_b2, pBrush->_b2,
			_a2, pBrush->_a2,
			_angle, pBrush->_angle);

	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT >= 0x0601
CSTXD2DGraphicsBrush::CSTXD2DGraphicsBrush()
{

}

CSTXD2DGraphicsBrush::~CSTXD2DGraphicsBrush()
{

}

void CSTXD2DGraphicsBrush::SetOpacityFloat(float alpha)
{
	_brush->SetOpacity(alpha);
}


#endif
//////////////////////////////////////////////////////////////////////////

CSTXGraphicsPen::CSTXGraphicsPen()
{

}

CSTXGraphicsPen::~CSTXGraphicsPen()
{

}


//////////////////////////////////////////////////////////////////////////

CSTXGdiPlusGraphicsPen::CSTXGdiPlusGraphicsPen()
{
	_pen = NULL;
}

CSTXGdiPlusGraphicsPen::~CSTXGdiPlusGraphicsPen()
{
	if (_pen)
		delete _pen;
}

Gdiplus::Pen* CSTXGdiPlusGraphicsPen::GetGdiPlusPen()
{
	return _pen;
}

int CSTXGdiPlusGraphicsPen::CompareTo(const CSTXGraphicsObject *pObject) const
{
	int nSuperCompare = __super::CompareTo(pObject);
	if (nSuperCompare != 0)
		return nSuperCompare;

	const CSTXGdiPlusGraphicsPen *pPen = dynamic_cast<const CSTXGdiPlusGraphicsPen*>(pObject);
	if (pPen)
	{
		return CompareValue(_r, pPen->_r, _g, pPen->_g, _b, pPen->_b, _a, pPen->_a, _width, pPen->_width);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

int CSTXD2DGraphicsPen::CompareTo(const CSTXGraphicsObject *pObject) const
{
	int nSuperCompare = __super::CompareTo(pObject);
	if (nSuperCompare != 0)
		return nSuperCompare;

	const CSTXD2DGraphicsPen *pPen = dynamic_cast<const CSTXD2DGraphicsPen*>(pObject);
	if (pPen)
	{
		return CompareValue(_r, pPen->_r, _g, pPen->_g, _b, pPen->_b, _a, pPen->_a, _width, pPen->_width);
		//return CompareValue(_r, pBrush->_r, _g, pBrush->_g, _b, pBrush->_b, _a, pBrush->_a);
	}
	return 0;

}


//////////////////////////////////////////////////////////////////////////


CSTXGraphicsFont::~CSTXGraphicsFont()
{

}

//////////////////////////////////////////////////////////////////////////


CSTXGdiPlusGraphicsFont::CSTXGdiPlusGraphicsFont()
{
	_font = NULL;
}

CSTXGdiPlusGraphicsFont::~CSTXGdiPlusGraphicsFont()
{
	if (_font)
		delete _font;
}

Gdiplus::Font* CSTXGdiPlusGraphicsFont::GetGdiPlusFont()
{
	return _font;
}

int CSTXGdiPlusGraphicsFont::CompareTo(const CSTXGraphicsObject *pObject) const
{
	int nSuperCompare = __super::CompareTo(pObject);
	if (nSuperCompare != 0)
		return nSuperCompare;

	const CSTXGdiPlusGraphicsFont *pFont = dynamic_cast<const CSTXGdiPlusGraphicsFont*>(pObject);
	if (pFont)
	{
		return CompareValue(_fontName, pFont->_fontName, _fontSize, pFont->_fontSize, _bold, pFont->_bold, _italic, pFont->_italic);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT >= 0x0601
CSTXD2DGraphicsFont::CSTXD2DGraphicsFont()
{

}

CSTXD2DGraphicsFont::~CSTXD2DGraphicsFont()
{

}

int CSTXD2DGraphicsFont::CompareTo(const CSTXGraphicsObject *pObject) const
{
	int nSuperCompare = __super::CompareTo(pObject);
	if (nSuperCompare != 0)
		return nSuperCompare;

	const CSTXD2DGraphicsFont *pFont = dynamic_cast<const CSTXD2DGraphicsFont*>(pObject);
	if (pFont)
	{
		return CompareValue(_fontName, pFont->_fontName, _fontSize, pFont->_fontSize, _bold, pFont->_bold, _italic, pFont->_italic);
	}
	return 0;
}

#endif

//////////////////////////////////////////////////////////////////////////

CSTXGraphicsTextFormat::CSTXGraphicsTextFormat()
{
	_alignment = 0;		//0:Left, 1:Center, 2:Right
	_valignment = 1;	//0:Top, 1:Center, 2:Bottom
	_endEllipsis = TRUE;
	_wordWrap = TRUE;
}

CSTXGraphicsTextFormat::~CSTXGraphicsTextFormat()
{

}

//////////////////////////////////////////////////////////////////////////

CSTXGraphicsImage::CSTXGraphicsImage()
{
	_uid = 0;
}

CSTXGdiPlusGraphicsTextFormat::CSTXGdiPlusGraphicsTextFormat()
{


}

CSTXGdiPlusGraphicsTextFormat::~CSTXGdiPlusGraphicsTextFormat()
{

}

//////////////////////////////////////////////////////////////////////////

CSTXGraphicsImage::~CSTXGraphicsImage()
{

}

int CSTXGraphicsImage::GetWidth()
{
	return -1;
}

int CSTXGraphicsImage::GetHeight()
{
	return -1;
}

int CSTXGraphicsImage::CompareTo(const CSTXGraphicsObject *pObject) const
{
	int nSuperCompare = __super::CompareTo(pObject);
	if (nSuperCompare != 0)
		return nSuperCompare;

	const CSTXGraphicsImage *pImage = dynamic_cast<const CSTXGraphicsImage*>(pObject);
	if (pImage)
	{
		return CompareValue(_uid, pImage->_uid);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

CSTXGdiPlusGraphicsImage::CSTXGdiPlusGraphicsImage()
{
	_image = NULL;
}

CSTXGdiPlusGraphicsImage::~CSTXGdiPlusGraphicsImage()
{
	if (_image)
		delete _image;
}

Gdiplus::Image* CSTXGdiPlusGraphicsImage::GetGdiPlusImage()
{
	return _image;
}

int CSTXGdiPlusGraphicsImage::GetWidth()
{
	if (_image)
		return _image->GetWidth();

	return -1;
}

int CSTXGdiPlusGraphicsImage::GetHeight()
{
	if (_image)
		return _image->GetHeight();

	return -1;
}

//////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT >= 0x0601
CSTXD2DGraphicsImage::CSTXD2DGraphicsImage()
{

}

CSTXD2DGraphicsImage::~CSTXD2DGraphicsImage()
{

}

int CSTXD2DGraphicsImage::GetWidth()
{
	if (_image)
		return _image->GetSize().width;

	return -1;
}

int CSTXD2DGraphicsImage::GetHeight()
{
	if (_image)
		return _image->GetSize().height;

	return -1;
}

#endif

//////////////////////////////////////////////////////////////////////////

CSTXGraphicsImageFormat::CSTXGraphicsImageFormat()
{
	_opacity = 1.0f;
	_grey = FALSE;
}

CSTXGraphicsImageFormat::~CSTXGraphicsImageFormat()
{

}

//////////////////////////////////////////////////////////////////////////

CSTXGraphicsPolygon::CSTXGraphicsPolygon()
{
	_closePolygon = false;
}

int CSTXGraphicsPolygon::CompareTo(const CSTXGraphicsObject * pObject) const
{
	int nSuperCompare = __super::CompareTo(pObject);
	if (nSuperCompare != 0)
		return nSuperCompare;

	const CSTXGraphicsPolygon *pPolygon = dynamic_cast<const CSTXGraphicsPolygon*>(pObject);
	if (pPolygon)
	{
		if (_points.size() < pPolygon->_points.size())
			return -1;
		else if (_points.size() > pPolygon->_points.size())
			return 1;

		for (size_t i = 0; i < _points.size(); i++)
		{
			if (_points[i].x < pPolygon->_points[i].x)
				return -1;
			else if (_points[i].x > pPolygon->_points[i].x)
				return 1;

			if (_points[i].y < pPolygon->_points[i].y)
				return -1;
			else if (_points[i].y > pPolygon->_points[i].y)
				return 1;

		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

CSTXGraphicsMatrix::~CSTXGraphicsMatrix()
{

}

CSTXGraphicsMatrix* CSTXGraphicsMatrix::Multiply(CSTXGraphicsMatrix *pMatrix, int nMatrixOrder)
{
	return this;
}

CSTXGraphicsMatrix* CSTXGraphicsMatrix::Translate(int dx, int dy, int nMatrixOrder)
{
	return this;
}

CSTXGraphicsMatrix* CSTXGraphicsMatrix::Rotate(double fAngle, int nMatrixOrder)
{
	return this;
}

CSTXGraphicsMatrix* CSTXGraphicsMatrix::Scale(double scaleX, double scaleY, int nMatrixOrder)
{
	return this;
}

CSTXGraphicsMatrix* CSTXGraphicsMatrix::Clone()
{
	return new CSTXGraphicsMatrix();
}

void CSTXGraphicsMatrix::Reset()
{

}

//////////////////////////////////////////////////////////////////////////

CSTXGdiPlusGraphicsMatrix::CSTXGdiPlusGraphicsMatrix()
{
	_matrix = NULL;
}

CSTXGdiPlusGraphicsMatrix::~CSTXGdiPlusGraphicsMatrix()
{
	if (_matrix)
		delete _matrix;
}

Gdiplus::Matrix* CSTXGdiPlusGraphicsMatrix::GetGdiPlusMatrix()
{
	return _matrix;
}

CSTXGraphicsMatrix* CSTXGdiPlusGraphicsMatrix::Multiply(CSTXGraphicsMatrix *pMatrix, int nMatrixOrder)
{
	if (_matrix == NULL)
		return NULL;

	CSTXGdiPlusGraphicsMatrix *pGdiPlusMatrix = dynamic_cast<CSTXGdiPlusGraphicsMatrix*>(pMatrix);
	if (pGdiPlusMatrix == NULL)
		return NULL;

	_matrix->Multiply(pGdiPlusMatrix->_matrix, (Gdiplus::MatrixOrder)nMatrixOrder);
	return this;
}

CSTXGraphicsMatrix* CSTXGdiPlusGraphicsMatrix::Translate(int dx, int dy, int nMatrixOrder)
{
	if (_matrix == NULL)
		return NULL;

	_matrix->Translate(static_cast<Gdiplus::REAL>(dx), static_cast<Gdiplus::REAL>(dy), (Gdiplus::MatrixOrder)nMatrixOrder);
	return this;
}

CSTXGraphicsMatrix* CSTXGdiPlusGraphicsMatrix::Rotate(double fAngle, int nMatrixOrder)
{
	if (_matrix == NULL)
		return NULL;

	_matrix->Rotate(static_cast<Gdiplus::REAL>(fAngle), (Gdiplus::MatrixOrder)nMatrixOrder);
	return this;
}

CSTXGraphicsMatrix* CSTXGdiPlusGraphicsMatrix::Scale(double scaleX, double scaleY, int nMatrixOrder)
{
	if (_matrix == NULL)
		return NULL;

	_matrix->Scale(static_cast<Gdiplus::REAL>(scaleX), static_cast<Gdiplus::REAL>(scaleY), (Gdiplus::MatrixOrder)nMatrixOrder);
	return this;
}

CSTXGraphicsMatrix* CSTXGdiPlusGraphicsMatrix::Clone()
{
	CSTXGdiPlusGraphicsMatrix *pMatrix = new CSTXGdiPlusGraphicsMatrix();
	pMatrix->_matrix = _matrix->Clone();
	return pMatrix;
}

void CSTXGdiPlusGraphicsMatrix::Reset()
{
	if (_matrix == NULL)
		return;

	_matrix->Reset();
}

//////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT >= 0x0601
CSTXD2DGraphicsMatrix::CSTXD2DGraphicsMatrix()
{
	_matrix = NULL;
}

CSTXD2DGraphicsMatrix::~CSTXD2DGraphicsMatrix()
{
	if (_matrix)
		delete _matrix;
}

CSTXGraphicsMatrix* CSTXD2DGraphicsMatrix::Clone()
{
	CSTXD2DGraphicsMatrix *pMatrix = new CSTXD2DGraphicsMatrix();
	pMatrix->_matrix = new D2D1_MATRIX_3X2_F();
	memcpy(pMatrix->_matrix, _matrix, sizeof(*pMatrix->_matrix));
	return pMatrix;
}

void CSTXD2DGraphicsMatrix::Reset()
{
	*_matrix = D2D1::Matrix3x2F::Identity();
}

CSTXGraphicsMatrix* CSTXD2DGraphicsMatrix::Multiply(CSTXGraphicsMatrix *pMatrix, int nMatrixOrder)
{
	CSTXD2DGraphicsMatrix *pD2DMatrix = dynamic_cast<CSTXD2DGraphicsMatrix*>(pMatrix);
	if (pD2DMatrix)
	{
		if (nMatrixOrder == 0)		//Prepend
			*_matrix = *pD2DMatrix->_matrix * (*_matrix);
		else if (nMatrixOrder == 1)
			*_matrix = *_matrix * (*pD2DMatrix->_matrix);
	}
	return this;
}

CSTXGraphicsMatrix* CSTXD2DGraphicsMatrix::Translate(int dx, int dy, int nMatrixOrder)
{
	D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Translation(static_cast<FLOAT>(dx), static_cast<FLOAT>(dy));

	if (nMatrixOrder == 0)		//Prepend
		*_matrix = matrix * (*_matrix);
	else if (nMatrixOrder == 1)
		*_matrix = *_matrix * matrix;

	return this;

}

CSTXGraphicsMatrix* CSTXD2DGraphicsMatrix::Rotate(double fAngle, int nMatrixOrder)
{
	D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Rotation(static_cast<FLOAT>(fAngle));

	if (nMatrixOrder == 0)		//Prepend
		*_matrix = matrix * (*_matrix);
	else if (nMatrixOrder == 1)
		*_matrix = *_matrix * matrix;

	return this;
}

CSTXGraphicsMatrix* CSTXD2DGraphicsMatrix::Scale(double scaleX, double scaleY, int nMatrixOrder)
{
	D2D1_MATRIX_3X2_F matrix = D2D1::Matrix3x2F::Scale(static_cast<FLOAT>(scaleX), static_cast<FLOAT>(scaleY));

	if (nMatrixOrder == 0)		//Prepend
		*_matrix = matrix * (*_matrix);
	else if (nMatrixOrder == 1)
		*_matrix = *_matrix * matrix;

	return this;
}
#endif

//////////////////////////////////////////////////////////////////////////

CGraphicsRoundRectPath::CGraphicsRoundRectPath(INT x, INT y, INT width, INT height, INT cornerX, INT cornerY)
	:Gdiplus::GraphicsPath()
{
	AddRoundRect(x, y, width, height, cornerX, cornerY);
}
void CGraphicsRoundRectPath::AddRoundRect(INT x, INT y, INT width, INT height, INT cornerX, INT cornerY)
{
	INT elWid = 2 * cornerX;
	INT elHei = 2 * cornerY;

	AddArc(x, y, elWid, elHei, 180, 90); // ◊Û…œΩ«‘≤ª°
	AddLine(x + cornerX, y, x + width - cornerX, y); // …œ±ﬂ

	AddArc(x + width - elWid, y, elWid, elHei, 270, 90); // ”“…œΩ«‘≤ª°
	AddLine(x + width, y + cornerY, x + width, y + height - cornerY);// ”“±ﬂ

	AddArc(x + width - elWid, y + height - elHei, elWid, elHei, 0, 90); // ”“œ¬Ω«‘≤ª°
	AddLine(x + width - cornerX, y + height, x + cornerX, y + height); // œ¬±ﬂ

	AddArc(x, y + height - elHei, elWid, elHei, 90, 90);
	AddLine(x, y + cornerY, x, y + height - cornerY);
}

//////////////////////////////////////////////////////////////////////////


std::map<UINT, std::set<CSTXGraphicsObject*, CSTXGraphics::CObjectLess>> CSTXGraphics::_sobjectCache;
std::map<UINT, std::map<std::wstring, IUnknown*>> CSTXGraphics::_sptrCache;

CSTXGraphics::CSTXGraphics()
{
	_hwnd = NULL;
	_hdc = NULL;
	_cx = _cy = 0;
	_cacheId = 0;		//0 = default cache id
}

CSTXGraphics::~CSTXGraphics()
{

}

void CSTXGraphics::Create(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID)
{
	_cx = cx;
	_cy = cy;
	_hwnd = hWnd;
	_hdc = hDC;
	_cacheId = cacheUniqueID;
}

CSTXGraphics* CSTXGraphics::CreateGdiPlusGraphics(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID)
{
	CSTXGdiPlusGraphics *pGraphics = new CSTXGdiPlusGraphics();
	pGraphics->Create(hWnd, hDC, cx, cy, cacheUniqueID);
	return pGraphics;
}

#if _WIN32_WINNT >= 0x0601
CSTXGraphics* CSTXGraphics::CreateD2DGraphics(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID)
{
	CSTXD2DGraphics *pGraphics = new CSTXD2DGraphics();
	pGraphics->Create(hWnd, hDC, cx, cy, cacheUniqueID);
	return pGraphics;
}
#endif

CSTXGraphics* CSTXGraphics::CreateAutoGraphics(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID)
{
	CSTXGraphics *pGraphics = NULL;

	BOOL bUseDirect2D = FALSE;

#if _WIN32_WINNT >= 0x0601
	if (IsWindows7OrGreater())
	{
		bUseDirect2D = TRUE;
	}
	if (bUseDirect2D)
	{
		//OutputDebugString(_T("Using Direct2D\n"));
		pGraphics = new CSTXD2DGraphics();
	}
	else
#endif	
	{
		//OutputDebugString(_T("Using GdiPlus\n"));
		pGraphics = new CSTXGdiPlusGraphics();
	}

	pGraphics->Create(hWnd, hDC, cx, cy, cacheUniqueID);
	return pGraphics;
}

CSTXLayeredGraphics* CSTXGraphics::CreateAutoLayeredGraphics(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID)
{
	CSTXLayeredGraphics *pGraphics = NULL;

	BOOL bUseDirect2D = FALSE;

#if _WIN32_WINNT >= 0x0601
	if (IsWindows7OrGreater())
	{
		bUseDirect2D = TRUE;
	}
	if (bUseDirect2D)
	{
		//OutputDebugString(_T("Using Direct2D\n"));
		pGraphics = new CSTXD2DDCGraphics();
	}
	else
#endif	
	{
		//OutputDebugString(_T("Using GdiPlus\n"));
		pGraphics = new CSTXGdiPlusDCGraphics();
	}

	CSTXGraphics *pBase = dynamic_cast<CSTXGraphics*>(pGraphics);
	if (pBase)
	{
		pBase->Create(hWnd, hDC, cx, cy, cacheUniqueID);
	}
	return pGraphics;
}

HRESULT CSTXGraphics::EndDraw()
{
	return S_OK;
}

void CSTXGraphics::FillRectangle(int x, int y, int width, int height, byte r, byte g, byte b, byte a)
{
	CSTXGraphicsBrush *pBrush = CreateSolidBrush(r, g, b, a);
	if (pBrush)
	{
		FillRectangle(x, y, width, height, pBrush);
	}
}

void CSTXGraphics::FillRectangle(int x, int y, int width, int height, CSTXGraphicsBrush *pBrush)
{

}

void CSTXGraphics::BeginDraw()
{

}

CSTXGraphicsBrush* CSTXGraphics::CreateSolidBrush(byte r, byte g, byte b, byte a)
{
	return NULL;
}

CSTXGraphicsBrush* CSTXGraphics::CreateSimpleLinearGradientBrush(int x1, int y1, byte r1, byte g1, byte b1, byte a1, int x2, int y2, byte r2, byte g2, byte b2, byte a2, byte alpha, FLOAT angle /*= 0*/)
{
	return NULL;
}

CSTXGraphicsFont* CSTXGraphics::CreateDrawingFont(LPCTSTR lpszFontName, float fSize, BOOL bBold, BOOL bItalic)
{
	return NULL;
}

void CSTXGraphics::DrawString(LPCTSTR lpszString, int x, int y, int width, int height, CSTXGraphicsFont *pFont, CSTXGraphicsBrush *pBrush, CSTXGraphicsTextFormat *pFormat)
{

}

void CSTXGraphics::DrawImage(int x, int y, int width, int height, CSTXGraphicsImage *pImage, CSTXGraphicsImageFormat* pImageFormat)
{

}

CSTXGraphicsImage* CSTXGraphics::CreateDrawingImage(IStream *pStream, long long imageUniqueId)
{
	return NULL;
}

CSTXGraphicsImage* CSTXGraphics::CreateDrawingImage(LPCTSTR lpszImageFile)
{
	if (lpszImageFile == NULL)
		return NULL;

	IStream *pStream = NULL;
	SHCreateStreamOnFile(lpszImageFile, STGM_READ | STGM_SHARE_DENY_WRITE, &pStream);
	if (pStream)
	{
		CSTXGraphicsImage *pImageObject = CreateDrawingImage(pStream, std::hash<std::wstring>()(lpszImageFile));
		pStream->Release();
		return pImageObject;
	}

	return NULL;
}

CSTXGraphicsImage* CSTXGraphics::CreateDrawingImage(LPCTSTR lpszImageFile, long long imageUniqueId)
{
	if (lpszImageFile == NULL)
		return NULL;

	IStream *pStream = NULL;
	SHCreateStreamOnFile(lpszImageFile, STGM_READ | STGM_SHARE_DENY_WRITE, &pStream);
	if (pStream)
	{
		CSTXGraphicsImage *pImageObject = CreateDrawingImage(pStream, imageUniqueId);
		pStream->Release();
		return pImageObject;
	}

	return NULL;
}

CSTXGraphicsPolygon* CSTXGraphics::CreatePolygon(POINT *pt, UINT ptCount, BOOL closePolygon)
{
	return NULL;
}

CSTXGraphicsPen* CSTXGraphics::CreateDrawingPen(byte r, byte g, byte b, byte a, float fWidth)
{
	return NULL;
}

void CSTXGraphics::DrawRoundedRectangle(int x, int y, int width, int height, int d, CSTXGraphicsPen *pPen)
{

}

void CSTXGraphics::FillRoundedRectangle(int x, int y, int width, int height, int d, CSTXGraphicsBrush *pBrush)
{

}

CSTXGraphicsBrush* CSTXGraphics::CreateButtonGradientBrush(int x1, int y1, int x2, int y2, byte r, byte g, byte b, byte a, FLOAT angle /*= 0*/)
{
	return NULL;
}

void CSTXGraphics::ScaleRect(LPRECT lpRect, DOUBLE fScale)
{
	LONG w = lpRect->right - lpRect->left;
	LONG h = lpRect->bottom - lpRect->top;

	DOUBLE fDeltaW = w * (1 - fScale);
	DOUBLE fDeltaH = h * (1 - fScale);

	DOUBLE fDeltaX = fDeltaW / 2;
	DOUBLE fDeltaY = fDeltaH / 2;

	lpRect->left += static_cast<LONG>(fDeltaX);
	lpRect->right -= static_cast<LONG>(fDeltaX);
	lpRect->top += static_cast<LONG>(fDeltaY);
	lpRect->bottom -= static_cast<LONG>(fDeltaY);
}

void CSTXGraphics::ScaleRect(Gdiplus::Rect *lpRect, DOUBLE fScale)
{
	INT w = lpRect->Width;
	INT h = lpRect->Height;

	DOUBLE fDeltaW = w * (1 - fScale);
	DOUBLE fDeltaH = h * (1 - fScale);

	DOUBLE fDeltaX = fDeltaW / 2;
	DOUBLE fDeltaY = fDeltaH / 2;

	lpRect->X += static_cast<INT>(fDeltaX);
	lpRect->Width -= static_cast<INT>(fDeltaW);
	lpRect->Y += static_cast<INT>(fDeltaY);
	lpRect->Height -= static_cast<INT>(fDeltaH);
}

void CSTXGraphics::ScaleRect(Gdiplus::RectF *lpRect, DOUBLE fScale)
{
	Gdiplus::REAL w = lpRect->Width;
	Gdiplus::REAL h = lpRect->Height;

	DOUBLE fDeltaW = w * (1 - fScale);
	DOUBLE fDeltaH = h * (1 - fScale);

	DOUBLE fDeltaX = fDeltaW / 2;
	DOUBLE fDeltaY = fDeltaH / 2;

	lpRect->X += static_cast<Gdiplus::REAL>(fDeltaX);
	lpRect->Width -= static_cast<Gdiplus::REAL>(fDeltaW);
	lpRect->Y += static_cast<Gdiplus::REAL>(fDeltaY);
	lpRect->Height -= static_cast<Gdiplus::REAL>(fDeltaH);
}

void CSTXGraphics::TranslateTransform(int dx, int dy, int nMatrixOrder)
{

}

void CSTXGraphics::RotateTransform(double fAngle, int nMatrixOrder)
{

}

CSTXGraphicsMatrix* CSTXGraphics::GetTransform()
{
	return NULL;
}

void CSTXGraphics::SetTransform(CSTXGraphicsMatrix *pMatrix)
{

}

void CSTXGraphics::ResetTransform()
{

}

void CSTXGraphics::AddGraphicsObjectToCache(CSTXGraphicsObject *pObject)
{
	std::set<CSTXGraphicsObject*, CObjectLess> &innerMap = _sobjectCache[_cacheId];
	std::set<CSTXGraphicsObject*, CObjectLess>::iterator it = innerMap.find(pObject);
	if (it != innerMap.end())
	{
		(*it)->Release();
		innerMap.erase(it);
	}
	innerMap.insert(pObject);
	pObject->AddRef();
}

CSTXGraphicsObject* CSTXGraphics::GetGraphicsObjectFromCache(CSTXGraphicsObject *pObject)
{
	std::set<CSTXGraphicsObject*, CObjectLess> &innerMap = _sobjectCache[_cacheId];
	std::set<CSTXGraphicsObject*, CObjectLess>::iterator it = innerMap.find(pObject);
	if (it != innerMap.end())
	{
		(*it)->AddRef();
		return (*it);
	}

	return NULL;
}

void CSTXGraphics::ClearCachedGraphicsObjects(UINT cacheUniqueID)
{
	std::map<UINT, std::set<CSTXGraphicsObject*, CObjectLess>>::iterator it = _sobjectCache.find(cacheUniqueID);
	if (it != _sobjectCache.end())
	{
		std::set<CSTXGraphicsObject*, CObjectLess>::iterator itInner = it->second.begin();
		for (; itInner != it->second.end(); itInner++)
		{
			(*itInner)->Release();
		}
		_sobjectCache.erase(it);
	}
}

IUnknown* CSTXGraphics::GetCachedPtr(LPCTSTR lpszKey, UINT cacheUniqueID)
{
	if (lpszKey == NULL)
	{
		return NULL;
	}
	std::map<UINT, std::map<std::wstring, IUnknown*>>::iterator it = _sptrCache.find(cacheUniqueID);

	if (it == _sptrCache.end())
	{
		return NULL;
	}

	std::map<std::wstring, IUnknown*>::iterator itInner = it->second.find(lpszKey);
	if (itInner == it->second.end())
	{
		return NULL;
	}

	return itInner->second;
}

void CSTXGraphics::SetCachedPtr(LPCTSTR lpszKey, IUnknown* ptr, UINT cacheUniqueID)
{
	if (lpszKey == NULL)
	{
		return;
	}
	_sptrCache[cacheUniqueID][lpszKey] = ptr;
	if (ptr == NULL)
	{
		_sptrCache[cacheUniqueID].erase(lpszKey);
	}
}

void CSTXGraphics::DiscardDeviceDependentResource()
{

}

HDC CSTXGraphics::GetSafeHDC()
{
	return _hdc;
}

void CSTXGraphics::DrawRichText(ITextServices *pTextService, int x, int y, int width, int height)
{

}


void CSTXGraphics::FillPolygon(CSTXGraphicsPolygon *pPolygon, CSTXGraphicsBrush *pBrush)
{

}

//////////////////////////////////////////////////////////////////////////

CSTXGdiPlusGraphics::CSTXGdiPlusGraphics()
{
	_graphics = NULL;
	_memGraphics = NULL;
	_memBitmap = NULL;
	_hdc = NULL;
}

CSTXGdiPlusGraphics::~CSTXGdiPlusGraphics()
{
	if (_memGraphics)
		delete _memGraphics;

	if (_memBitmap)
		delete _memBitmap;

	if (_graphics)
	{
		_graphics->ReleaseHDC(_hdc);
		delete _graphics;
	}
}

void CSTXGdiPlusGraphics::Create(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID)
{
	__super::Create(hWnd, hDC, cx, cy, cacheUniqueID);

	_hdc = hDC;
	_graphics = new Gdiplus::Graphics(hDC);
	_memBitmap = new Gdiplus::Bitmap(cx, cy);
	_memGraphics = Gdiplus::Graphics::FromImage(_memBitmap);

	_memGraphics->SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
}

void CSTXGdiPlusGraphics::BeginDraw()
{
	_memGraphics->Clear(Gdiplus::Color::Transparent);
}

HRESULT CSTXGdiPlusGraphics::EndDraw()
{
	Gdiplus::Rect rectDraw(0, 0, _cx, _cy);
	Gdiplus::TextureBrush brushContent(_memBitmap);
	_graphics->FillRectangle(&brushContent, rectDraw);
	return S_OK;
}

void CSTXGdiPlusGraphics::FillRectangle(int x, int y, int width, int height, CSTXGraphicsBrush *pBrush)
{
	CSTXGdiPlusGraphicsBrush *pGdiPlusBrush = dynamic_cast<CSTXGdiPlusGraphicsBrush*>(pBrush);
	if (pGdiPlusBrush)
	{
		Gdiplus::RectF rect(static_cast<Gdiplus::REAL>(x), static_cast<Gdiplus::REAL>(y), static_cast<Gdiplus::REAL>(width), static_cast<Gdiplus::REAL>(height));
		_memGraphics->FillRectangle(pGdiPlusBrush->GetGdiPlusBrush(), rect);
	}
}

CSTXGraphicsBrush* CSTXGdiPlusGraphics::CreateSolidBrush(byte r, byte g, byte b, byte a)
{
	CSTXGdiPlusGraphicsSolidBrush *pBrush = new CSTXGdiPlusGraphicsSolidBrush();
	pBrush->_r = r;
	pBrush->_g = g;
	pBrush->_b = b;
	pBrush->_a = a;

	CSTXGdiPlusGraphicsSolidBrush *pBrushCached = dynamic_cast<CSTXGdiPlusGraphicsSolidBrush*>(GetGraphicsObjectFromCache(pBrush));	//AdRef called
	if (pBrushCached)
	{
		delete pBrush;
		return pBrushCached;
	}

	pBrush->_brush = new Gdiplus::SolidBrush(Gdiplus::Color(a, r, g, b));
//	AddGraphicsObjectToCache(pBrush);
	return pBrush;
}

CSTXGraphicsFont* CSTXGdiPlusGraphics::CreateDrawingFont(LPCTSTR lpszFontName, float fSize, BOOL bBold, BOOL bItalic)
{
	CSTXGdiPlusGraphicsFont *pFont = new CSTXGdiPlusGraphicsFont();
	pFont->_fontName = lpszFontName;
	pFont->_fontSize = fSize;
	pFont->_bold = bBold;
	pFont->_italic = bItalic;

	CSTXGdiPlusGraphicsFont *pFontCached = dynamic_cast<CSTXGdiPlusGraphicsFont*>(GetGraphicsObjectFromCache(pFont));	//AdRef called
	if (pFontCached)
	{
		delete pFont;
		return pFontCached;
	}

	Gdiplus::FontFamily  fontFamily(lpszFontName);

	INT nFontStyle = Gdiplus::FontStyleRegular;
	if (bBold)
		nFontStyle = Gdiplus::FontStyleBold;
	if (bItalic)
		nFontStyle = Gdiplus::FontStyleItalic;
	if (bBold && bItalic)
		nFontStyle = Gdiplus::FontStyleBoldItalic;

	pFont->_font = new Gdiplus::Font(&fontFamily, fSize, nFontStyle, Gdiplus::UnitPixel);
//	AddGraphicsObjectToCache(pFont);
	return pFont;
}

CSTXGraphicsBrush* CSTXGdiPlusGraphics::CreateSimpleLinearGradientBrush(int x1, int y1, byte r1, byte g1, byte b1, byte a1, int x2, int y2, byte r2, byte g2, byte b2, byte a2, byte alpha, FLOAT angle /*= 0*/)
{
	CSTXGdiPlusGraphicsSimpleLinearGradientBrush *pBrush = new CSTXGdiPlusGraphicsSimpleLinearGradientBrush();
	pBrush->_x1 = x1;
	pBrush->_x2 = x2;
	pBrush->_y1 = y1;
	pBrush->_y2 = y2;
	pBrush->_r1 = r1;
	pBrush->_g1 = g1;
	pBrush->_b1 = b1;
	pBrush->_a1 = a1;
	pBrush->_r2 = r2;
	pBrush->_g2 = g2;
	pBrush->_b2 = b2;
	pBrush->_a2 = a2;
	//angle = atan2((y2 - y1), (x2 - x1)) * 180;
	pBrush->_angle = 0;

	CSTXGdiPlusGraphicsSimpleLinearGradientBrush *pBrushCached = dynamic_cast<CSTXGdiPlusGraphicsSimpleLinearGradientBrush*>(GetGraphicsObjectFromCache(pBrush));	//AdRef called
	if (pBrushCached)
	{
		delete pBrush;
		return pBrushCached;
	}

	int xpatch = x2 >= x1 ? 1 : -1;
	int ypatch = y2 >= y1 ? 1 : -1;

	pBrush->_brush = new Gdiplus::LinearGradientBrush(Gdiplus::Rect(x1 - xpatch, y1 - ypatch, x2 - x1 + xpatch * 2, y2 - y1 + ypatch * 2),
		Gdiplus::Color(static_cast<byte>(a1 * (alpha / 255.0f)), r1, g1, b1),
		Gdiplus::Color(static_cast<byte>(a2 * (alpha / 255.0f)), r2, g2, b2),
		pBrush->_angle);

	//((Gdiplus::LinearGradientBrush*)(pBrush->_brush))->SetWrapMode(Gdiplus::WrapModeTileFlipXY);

//	AddGraphicsObjectToCache(pBrush);

	return pBrush;
}

void CSTXGdiPlusGraphics::DrawString(LPCTSTR lpszString, int x, int y, int width, int height, CSTXGraphicsFont *pFont, CSTXGraphicsBrush *pBrush, CSTXGraphicsTextFormat *pFormat)
{
	CSTXGdiPlusGraphicsFont *pGdiPlusFont = dynamic_cast<CSTXGdiPlusGraphicsFont*>(pFont);
	CSTXGdiPlusGraphicsBrush *pGdiPlusBrush = dynamic_cast<CSTXGdiPlusGraphicsBrush*>(pBrush);
	if (pGdiPlusFont && pGdiPlusBrush)
	{
		Gdiplus::StringFormat strFormat;
		if (pFormat)
		{
			if (!pFormat->_wordWrap)
				strFormat.SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);

			if (pFormat->_endEllipsis)
				strFormat.SetTrimming(Gdiplus::StringTrimmingEllipsisWord);

			switch (pFormat->_valignment)
			{
			case 0:
				strFormat.SetLineAlignment(Gdiplus::StringAlignmentNear); break;
			case 1:
				strFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter); break;
			case 2:
				strFormat.SetLineAlignment(Gdiplus::StringAlignmentFar); break;
			}

			switch (pFormat->_alignment)
			{
			case 0:
				strFormat.SetAlignment(Gdiplus::StringAlignmentNear); break;
			case 1:
				strFormat.SetAlignment(Gdiplus::StringAlignmentCenter); break;
			case 2:
				strFormat.SetAlignment(Gdiplus::StringAlignmentFar); break;
			}

		}
		Gdiplus::RectF rect(static_cast<Gdiplus::REAL>(x), static_cast<Gdiplus::REAL>(y), static_cast<Gdiplus::REAL>(width), static_cast<Gdiplus::REAL>(height));
		_memGraphics->DrawString(lpszString, -1, pGdiPlusFont->_font, rect, &strFormat, pGdiPlusBrush->GetGdiPlusBrush());
	}
}

CSTXGraphicsImage* CSTXGdiPlusGraphics::CreateDrawingImage(IStream *pStream, long long imageUniqueId)
{
	CSTXGdiPlusGraphicsImage *pImage = new CSTXGdiPlusGraphicsImage();
	pImage->_uid = imageUniqueId;

	CSTXGdiPlusGraphicsImage *pImageCached = dynamic_cast<CSTXGdiPlusGraphicsImage*>(GetGraphicsObjectFromCache(pImage));	//AdRef called
	if (pImageCached)
	{
		delete pImage;
		return pImageCached;
	}

	pImage->_image = new Gdiplus::Image(pStream);
	AddGraphicsObjectToCache(pImage);
	return pImage;
}

CSTXGraphicsPolygon* CSTXGdiPlusGraphics::CreatePolygon(POINT *pt, UINT ptCount, BOOL closePolygon)
{
	CSTXGdiPlusGraphicsPolygon *pPolygon = new CSTXGdiPlusGraphicsPolygon();
	CSTXGdiPlusGraphicsPolygon *pPolygonCached = dynamic_cast<CSTXGdiPlusGraphicsPolygon*>(GetGraphicsObjectFromCache(pPolygon));	//AdRef called
	if (pPolygonCached)
	{
		delete pPolygon;
		return pPolygonCached;
	}

	for (UINT i = 0; i < ptCount; i++)
	{
		pPolygon->_points.push_back(pt[i]);
		pPolygon->_gdiPlusPoints.push_back(Gdiplus::Point(pt[i].x, pt[i].y));
	}
	pPolygon->_closePolygon = closePolygon;

	AddGraphicsObjectToCache(pPolygon);
	return pPolygon;
}

void CSTXGdiPlusGraphics::DrawImage(int x, int y, int width, int height, CSTXGraphicsImage *pImage, CSTXGraphicsImageFormat* pImageFormat)
{
	CSTXGdiPlusGraphicsImage *pGdiPlusImage = dynamic_cast<CSTXGdiPlusGraphicsImage*>(pImage);
	if (pGdiPlusImage == NULL)
		return;

	Gdiplus::REAL rOpacity = 1.0f;
	if (pImageFormat)
	{
		rOpacity = pImageFormat->_opacity;
	}

	Gdiplus::ColorMatrix *pCMUse = NULL;
	Gdiplus::ColorMatrix cm = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, rOpacity, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	pCMUse = &cm;

	Gdiplus::ColorMatrix cmGray = {
		0.299f, 0.299f, 0.299f, 0.0f, 0.0f,
		0.587f, 0.587f, 0.587f, 0.0f, 0.0f,
		0.114f, 0.114f, 0.114f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, rOpacity, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	if (pImageFormat && pImageFormat->_grey)
	{
		pCMUse = &cmGray;
	}

	Gdiplus::ImageAttributes ImgAttr;

	ImgAttr.SetColorMatrix(pCMUse, Gdiplus::ColorMatrixFlagsDefault,
		Gdiplus::ColorAdjustTypeBitmap);

	Gdiplus::RectF rectImage(static_cast<Gdiplus::REAL>(x), static_cast<Gdiplus::REAL>(y), static_cast<Gdiplus::REAL>(width), static_cast<Gdiplus::REAL>(height));
	UINT nImgWidth = pGdiPlusImage->_image->GetWidth();
	UINT nImgHeight = pGdiPlusImage->_image->GetHeight();

	_memGraphics->DrawImage(pGdiPlusImage->_image, rectImage, 0, 0, static_cast<Gdiplus::REAL>(nImgWidth), static_cast<Gdiplus::REAL>(nImgHeight)
		, Gdiplus::UnitPixel, &ImgAttr);
}

void CSTXGdiPlusGraphics::DrawRoundedRectangle(int x, int y, int width, int height, int d, CSTXGraphicsPen *pPen)
{
	CSTXGdiPlusGraphicsPen *pGdiPlusPen = dynamic_cast<CSTXGdiPlusGraphicsPen*>(pPen);
	if (pGdiPlusPen)
	{
		CGraphicsRoundRectPath path(x, y, width, height, d, d);
		_memGraphics->DrawPath(pGdiPlusPen->GetGdiPlusPen(), &path);
	}
}

CSTXGraphicsPen* CSTXGdiPlusGraphics::CreateDrawingPen(byte r, byte g, byte b, byte a, float fWidth)
{
	CSTXGdiPlusGraphicsPen *pPen = new CSTXGdiPlusGraphicsPen();
	pPen->_r = r;
	pPen->_g = g;
	pPen->_b = b;
	pPen->_a = a;
	pPen->_width = fWidth;

	CSTXGdiPlusGraphicsPen *pPenCached = dynamic_cast<CSTXGdiPlusGraphicsPen*>(GetGraphicsObjectFromCache(pPen));	//AdRef called
	if (pPenCached)
	{
		delete pPen;
		return pPenCached;
	}

	pPen->_pen = new Gdiplus::Pen(Gdiplus::Color(a, r, g, b), fWidth);
//	AddGraphicsObjectToCache(pPen);
	return pPen;
}

void CSTXGdiPlusGraphics::FillRoundedRectangle(int x, int y, int width, int height, int d, CSTXGraphicsBrush *pBrush)
{
	// anti alias distorts fill so remove it.
	CSTXGdiPlusGraphicsBrush *pGdiPlusBrush = dynamic_cast<CSTXGdiPlusGraphicsBrush*>(pBrush);
	if (pGdiPlusBrush)
	{
		CGraphicsRoundRectPath path(x, y, width, height, d, d);
		_memGraphics->FillPath(pGdiPlusBrush->GetGdiPlusBrush(), &path);
	}
}

CSTXGraphicsBrush* CSTXGdiPlusGraphics::CreateButtonGradientBrush(int x1, int y1, int x2, int y2, byte r, byte g, byte b, byte a, FLOAT angle /*= 0*/)
{
	CSTXGdiPlusGraphicsButtonGradientBrush *pBrush = new CSTXGdiPlusGraphicsButtonGradientBrush();
	pBrush->_x1 = x1;
	pBrush->_x2 = x2;
	pBrush->_y1 = y1;
	pBrush->_y2 = y2;
	pBrush->_r = r;
	pBrush->_g = g;
	pBrush->_b = b;
	pBrush->_a = a;
	pBrush->_angle = angle;

	CSTXGdiPlusGraphicsButtonGradientBrush *pBrushCached = dynamic_cast<CSTXGdiPlusGraphicsButtonGradientBrush*>(GetGraphicsObjectFromCache(pBrush));	//AdRef called
	if (pBrushCached)
	{
		delete pBrush;
		return pBrushCached;
	}


	byte topR = r + (255 - r) * 8 / 10;
	byte topG = g + (255 - g) * 8 / 10;
	byte topB = b + (255 - b) * 8 / 10;

	byte topCenterR = r + (255 - r) / 8;
	byte topCenterG = g + (255 - g) / 8;
	byte topCenterB = b + (255 - b) / 8;


	byte bottomCenterR = r * 8 / 10;
	byte bottomCenterG = g * 8 / 10;
	byte bottomCenterB = b * 8 / 10;

	byte bottomR = r / 3;
	byte bottomG = g / 3;
	byte bottomB = b / 3;

	Gdiplus::Color colors[] = {
		Gdiplus::Color(a, topR, topG, topB),
		Gdiplus::Color(a, topCenterR, topCenterG, topCenterB),
		Gdiplus::Color(a, bottomCenterR, bottomCenterG, bottomCenterB),
		Gdiplus::Color(a, bottomR, bottomG, bottomB) };

	Gdiplus::REAL positions[] = {
		0.0f,
		0.499f,
		0.501f,
		1.0f };


	int xpatch = x2 > x1 ? 1 : -1;
	int ypatch = y2 > y1 ? 1 : -1;

	Gdiplus::LinearGradientBrush *pGradientBrush = new Gdiplus::LinearGradientBrush(Gdiplus::Rect(x1 - xpatch, y1 - ypatch, x2 - x1 + xpatch * 2, y2 - y1 + ypatch * 2),
		Gdiplus::Color(a, 0, 0, 0),
		Gdiplus::Color(a, 255, 25, 255),
		angle);

	pBrush->_brush = pGradientBrush;

	pGradientBrush->SetInterpolationColors(colors, positions, 4);

//	AddGraphicsObjectToCache(pBrush);
	return pBrush;
}

void CSTXGdiPlusGraphics::TranslateTransform(int dx, int dy, int nMatrixOrder)
{
	_memGraphics->TranslateTransform(static_cast<Gdiplus::REAL>(dx), static_cast<Gdiplus::REAL>(dy), (Gdiplus::MatrixOrder)nMatrixOrder);
}

void CSTXGdiPlusGraphics::RotateTransform(double fAngle, int nMatrixOrder)
{
	_memGraphics->RotateTransform(static_cast<Gdiplus::REAL>(fAngle), (Gdiplus::MatrixOrder)nMatrixOrder);
}

CSTXGraphicsMatrix* CSTXGdiPlusGraphics::GetTransform()
{
	CSTXGdiPlusGraphicsMatrix *pMatrix = new CSTXGdiPlusGraphicsMatrix();
	pMatrix->_matrix = new Gdiplus::Matrix();
	_memGraphics->GetTransform(pMatrix->_matrix);
	return pMatrix;
}

void CSTXGdiPlusGraphics::SetTransform(CSTXGraphicsMatrix *pMatrix)
{
	CSTXGdiPlusGraphicsMatrix *pGdiPlusMatrix = dynamic_cast<CSTXGdiPlusGraphicsMatrix*>(pMatrix);
	if (pGdiPlusMatrix && pGdiPlusMatrix->_matrix)
	{
		_memGraphics->SetTransform(pGdiPlusMatrix->_matrix);
	}
}

void CSTXGdiPlusGraphics::ResetTransform()
{
	_memGraphics->ResetTransform();
}

void CSTXGdiPlusGraphics::DrawRichText(ITextServices *pTextService, int x, int y, int width, int height)
{
	RECTL rcl = { x, y, x + width, y + height };
	//RECT rcUpdate = { x, y, x + width, y + height };
	
	HDC hDCRender = _memGraphics->GetHDC();
	HRESULT hr = pTextService->TxDraw(DVASPECT_CONTENT, 0, NULL, NULL, hDCRender, 0,
		&rcl, NULL, NULL, NULL, 0, TXTVIEW_ACTIVE);

	_memGraphics->ReleaseHDC(hDCRender);
}

void CSTXGdiPlusGraphics::FillPolygon(CSTXGraphicsPolygon *pPolygon, CSTXGraphicsBrush *pBrush)
{
	CSTXGdiPlusGraphicsBrush *pGdiPlusBrush = dynamic_cast<CSTXGdiPlusGraphicsBrush*>(pBrush);
	CSTXGdiPlusGraphicsPolygon *pGdiPlusPolygon = dynamic_cast<CSTXGdiPlusGraphicsPolygon*>(pPolygon);
	if (pGdiPlusBrush && pGdiPlusPolygon && pGdiPlusPolygon->_gdiPlusPoints.size() > 0)
	{
		_memGraphics->FillPolygon(pGdiPlusBrush->GetGdiPlusBrush(), &pGdiPlusPolygon->_gdiPlusPoints[0], pGdiPlusPolygon->_gdiPlusPoints.size());
	}
}

//////////////////////////////////////////////////////////////////////////

void CSTXGdiPlusDCGraphics::Create(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID)
{
	_sizeDC.cx = cx;
	_sizeDC.cy = cy;
	_bitmap.Create(cx, cy);
	__super::Create(hWnd, hDC, cx, cy, cacheUniqueID);
}


BOOL CSTXGdiPlusDCGraphics::ApplyToLayeredWindow(HWND hWnd)
{
	BLENDFUNCTION blend = {};
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	POINT ptSrc = { 0, 0 };
	UPDATELAYEREDWINDOWINFO info = {};
	info.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
	info.pptSrc = &ptSrc;
	info.pptDst = NULL;
	info.psize = &_sizeDC;
	info.pblend = &blend;
	info.dwFlags = ULW_ALPHA;

	Gdiplus::Graphics g(_bitmap.GetDC());
	g.Clear(Gdiplus::Color::Transparent);
	g.DrawImage(_memBitmap, 0, 0, _sizeDC.cx, _sizeDC.cy);

	info.hdcSrc = _bitmap.GetDC();
	//BOOL bResult = UpdateLayeredWindowIndirect(hWnd, &info);

	BOOL bResult = UpdateLayeredWindow(hWnd, NULL, NULL, &_sizeDC, _bitmap.GetDC(), &ptSrc, 0, &blend, ULW_ALPHA);

	return bResult;
}

//////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT >= 0x0601
CSTXD2DGraphics::CSTXD2DGraphics()
{

}
CSTXD2DGraphics::~CSTXD2DGraphics()
{

}

void CSTXD2DGraphics::Create(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID)
{
	__super::Create(hWnd, hDC, cx, cy, cacheUniqueID);

	HRESULT hr = S_OK;

	ID2D1Factory *pFactory = (ID2D1Factory*)GetCachedPtr(_T("_D2DFactory"), _cacheId);
	if (pFactory)
	{
		_factory = pFactory;
	}
	else
	{
		hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&pFactory
			);
		SetCachedPtr(_T("_D2DFactory"), pFactory, _cacheId);
		_factory = pFactory;
	}


	IDWriteFactory *pDWFactory = (IDWriteFactory*)GetCachedPtr(_T("_DWFactory"), _cacheId);
	if (pDWFactory)
	{
		_dwfactory = pDWFactory;
	}
	else
	{
		hr = DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&pDWFactory)
			);
		SetCachedPtr(_T("_DWFactory"), pDWFactory, _cacheId);
		_dwfactory = pDWFactory;
	}

	IWICImagingFactory *pWICFactory = (IWICImagingFactory*)GetCachedPtr(_T("_WICFactory"), _cacheId);
	if (pWICFactory)
	{
		_wicfactory = pWICFactory;
	}
	else
	{
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID*)&pWICFactory
			);
		SetCachedPtr(_T("_WICFactory"), pWICFactory, _cacheId);
		_wicfactory = pWICFactory;
	}

	ID2D1RenderTarget *pRenderTarget = (ID2D1RenderTarget*)GetCachedPtr(_T("_D2DRenderTarget"), _cacheId);
	if (pRenderTarget)
	{
		_renderTarget = pRenderTarget;
	}
	else
	{
		D2D1_SIZE_U size = D2D1::SizeU(
			cx,
			cy
			);

		D2D1_HWND_RENDER_TARGET_PROPERTIES prop = D2D1::HwndRenderTargetProperties(
			hWnd,
			size
			);

		D2D1_RENDER_TARGET_PROPERTIES propBase = D2D1::RenderTargetProperties();
		propBase.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;

		ID2D1HwndRenderTarget *pHwndRenderTarget = NULL;
		hr = _factory->CreateHwndRenderTarget(
			&propBase,
			&prop,
			&pHwndRenderTarget
			);

		SetCachedPtr(_T("_D2DRenderTarget"), pHwndRenderTarget, _cacheId);
		_renderTarget = pHwndRenderTarget;
	}
}

HRESULT CSTXD2DGraphics::EndDraw()
{
	HRESULT hr = _renderTarget->EndDraw();
	if (FAILED(hr))
	{
		OutputDebugString(_T("EndDraw Failed!\n"));
	}
	return hr;
}

void CSTXD2DGraphics::FillRectangle(int x, int y, int width, int height, CSTXGraphicsBrush *pBrush)
{
	CSTXD2DGraphicsBrush *pD2DBrush = dynamic_cast<CSTXD2DGraphicsBrush*>(pBrush);
	if (pD2DBrush)
	{
		_renderTarget->FillRectangle(
			D2D1::RectF(
			static_cast<FLOAT>(x),
			static_cast<FLOAT>(y),
			static_cast<FLOAT>(x + width),
			static_cast<FLOAT>(y + height)
			),
			pD2DBrush->_brush
			);
	}

}

void CSTXD2DGraphics::BeginDraw()
{
	_renderTarget->BeginDraw();
	_renderTarget->Clear();
}

CSTXGraphicsMatrix* CSTXD2DGraphics::GetTransform()
{
	CSTXD2DGraphicsMatrix *pMatrix = new CSTXD2DGraphicsMatrix();
	pMatrix->_matrix = new D2D1_MATRIX_3X2_F();
	_renderTarget->GetTransform(pMatrix->_matrix);
	return pMatrix;
}

CSTXGraphicsBrush* CSTXD2DGraphics::CreateSolidBrush(byte r, byte g, byte b, byte a)
{
	CSTXD2DGraphicsSolidBrush *pBrush = new CSTXD2DGraphicsSolidBrush();
	pBrush->_r = r;
	pBrush->_g = g;
	pBrush->_b = b;
	pBrush->_a = a;

	CSTXD2DGraphicsSolidBrush *pBrushCached = dynamic_cast<CSTXD2DGraphicsSolidBrush*>(GetGraphicsObjectFromCache(pBrush));	//AdRef called
	if (pBrushCached)
	{
		delete pBrush;
		pBrushCached->_brush->SetOpacity(a / 255.0f);
		return pBrushCached;
	}

	CComPtr<ID2D1SolidColorBrush> spBrush;
	HRESULT hr = _renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(static_cast<FLOAT>(r / 255.0f), static_cast<FLOAT>(g / 255.0f), static_cast<FLOAT>(b / 255.0f), static_cast<FLOAT>(a / 255.0f)), &spBrush);
	if (SUCCEEDED(hr))
	{
		spBrush->QueryInterface(&pBrush->_brush);
		spBrush->SetOpacity(a / 255.0f);
	}
	AddGraphicsObjectToCache(pBrush);
	return pBrush;
}

CSTXGraphicsBrush* CSTXD2DGraphics::CreateSimpleLinearGradientBrush(int x1, int y1, byte r1, byte g1, byte b1, byte a1, int x2, int y2, byte r2, byte g2, byte b2, byte a2, byte alpha, FLOAT angle /*= 0*/)
{
	CSTXD2DGraphicsSimpleLinearGradientBrush *pBrush = new CSTXD2DGraphicsSimpleLinearGradientBrush();
	pBrush->_x1 = x1;
	pBrush->_x2 = x2;
	pBrush->_y1 = y1;
	pBrush->_y2 = y2;
	pBrush->_r1 = r1;
	pBrush->_g1 = g1;
	pBrush->_b1 = b1;
	pBrush->_a1 = a1;
	pBrush->_r2 = r2;
	pBrush->_g2 = g2;
	pBrush->_b2 = b2;
	pBrush->_a2 = a2;
	pBrush->_alpha = alpha;
	pBrush->_angle = angle;

	CSTXD2DGraphicsSimpleLinearGradientBrush *pBrushCached = dynamic_cast<CSTXD2DGraphicsSimpleLinearGradientBrush*>(GetGraphicsObjectFromCache(pBrush));	//AdRef called
	if (pBrushCached)
	{
		delete pBrush;
		pBrushCached->_brush->SetOpacity(alpha / 255.0f);
		return pBrushCached;
	}

	const D2D1_GRADIENT_STOP gradientStops[] =
	{
		{ 0.0f, D2D1::ColorF(static_cast<float>(r1) / 255, static_cast<float>(g1) / 255, static_cast<float>(b1) / 255, static_cast<float>(a1) / 255) },
		{ 1.0f, D2D1::ColorF(static_cast<float>(r2) / 255, static_cast<float>(g2) / 255, static_cast<float>(b2) / 255, static_cast<float>(a2) / 255) }
	};

	CComPtr<ID2D1GradientStopCollection> gradientStopsCollection;

	HRESULT hr = _renderTarget->CreateGradientStopCollection(gradientStops,
		_countof(gradientStops),
		&gradientStopsCollection);

	CComPtr<ID2D1LinearGradientBrush> spBrush;
	const D2D1_POINT_2F start = D2D1::Point2F(static_cast<FLOAT>(x1), static_cast<FLOAT>(y1));
	const D2D1_POINT_2F end = D2D1::Point2F(static_cast<FLOAT>(x2), static_cast<FLOAT>(y2));
	const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES properties = D2D1::LinearGradientBrushProperties(start, end);
	hr = _renderTarget->CreateLinearGradientBrush(properties,
		gradientStopsCollection,
		&spBrush);

	if (SUCCEEDED(hr))
	{
		spBrush->QueryInterface(&pBrush->_brush);
	}
	AddGraphicsObjectToCache(pBrush);
	return pBrush;
}

void CSTXD2DGraphics::SetTransform(CSTXGraphicsMatrix *pMatrix)
{
	CSTXD2DGraphicsMatrix *pD2DMatrix = dynamic_cast<CSTXD2DGraphicsMatrix*>(pMatrix);
	if (pD2DMatrix && pD2DMatrix->_matrix)
	{
		_renderTarget->SetTransform(pD2DMatrix->_matrix);
	}
}

void CSTXD2DGraphics::ResetTransform()
{
	_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void CSTXD2DGraphics::TranslateTransform(int dx, int dy, int nMatrixOrder)
{
	D2D1_MATRIX_3X2_F matrix;
	_renderTarget->GetTransform(&matrix);
	D2D1_MATRIX_3X2_F matrixTransform = D2D1::Matrix3x2F::Translation(static_cast<FLOAT>(dx), static_cast<FLOAT>(dy));
	if (nMatrixOrder == 0)
		matrix = matrixTransform * matrix;
	else if (nMatrixOrder == 1)
		matrix = matrix * matrixTransform;

	_renderTarget->SetTransform(matrix);
}

CSTXGraphicsBrush* CSTXD2DGraphics::CreateButtonGradientBrush(int x1, int y1, int x2, int y2, byte r, byte g, byte b, byte a, FLOAT angle /*= 0*/)
{
	
	CSTXD2DGraphicsButtonGradientBrush *pBrush = new CSTXD2DGraphicsButtonGradientBrush();
	pBrush->_x1 = x1;
	pBrush->_x2 = x2;
	pBrush->_y1 = y1;
	pBrush->_y2 = y2;
	pBrush->_r = r;
	pBrush->_g = g;
	pBrush->_b = b;
	pBrush->_a = a;
	pBrush->_angle = angle;

	CSTXD2DGraphicsButtonGradientBrush *pBrushCached = dynamic_cast<CSTXD2DGraphicsButtonGradientBrush*>(GetGraphicsObjectFromCache(pBrush));	//AdRef called
	if (pBrushCached)
	{
		delete pBrush;
		pBrushCached->_brush->SetOpacity(a / 255.0f);
		return pBrushCached;
	}

	byte topR = r + (255 - r) * 8 / 10;
	byte topG = g + (255 - g) * 8 / 10;
	byte topB = b + (255 - b) * 8 / 10;

	byte topCenterR = r + (255 - r) / 8;
	byte topCenterG = g + (255 - g) / 8;
	byte topCenterB = b + (255 - b) / 8;


	byte bottomCenterR = r * 8 / 10;
	byte bottomCenterG = g * 8 / 10;
	byte bottomCenterB = b * 8 / 10;

	byte bottomR = r / 3;
	byte bottomG = g / 3;
	byte bottomB = b / 3;


	const D2D1_GRADIENT_STOP gradientStops[] =
	{
		{ 0.0f, D2D1::ColorF(static_cast<float>(topR) / 255, static_cast<float>(topG) / 255, static_cast<float>(topB) / 255, 1.0f) },
		{ 0.499f, D2D1::ColorF(static_cast<float>(topCenterR) / 255, static_cast<float>(topCenterG) / 255, static_cast<float>(topCenterB) / 255, 1.0f) },
		{ 0.501f, D2D1::ColorF(static_cast<float>(bottomCenterR) / 255, static_cast<float>(bottomCenterG) / 255, static_cast<float>(bottomCenterB) / 255, 1.0f) },
		{ 1.0f, D2D1::ColorF(static_cast<float>(bottomR) / 255, static_cast<float>(bottomG) / 255, static_cast<float>(bottomB) / 255, 1.0f) }
	};

	CComPtr<ID2D1GradientStopCollection> gradientStopsCollection;

	HRESULT hr = _renderTarget->CreateGradientStopCollection(gradientStops,
		_countof(gradientStops),
		&gradientStopsCollection);

	CComPtr<ID2D1LinearGradientBrush> spBrush;

	const D2D1_POINT_2F start = D2D1::Point2F(static_cast<FLOAT>(x1), static_cast<FLOAT>(y1));
	const D2D1_POINT_2F end = D2D1::Point2F(static_cast<FLOAT>(x1), static_cast<FLOAT>(y2));
	const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES properties = D2D1::LinearGradientBrushProperties(start, end);
	hr = _renderTarget->CreateLinearGradientBrush(properties,
		gradientStopsCollection,
		&spBrush);

	if (SUCCEEDED(hr))
	{
		spBrush->QueryInterface(&pBrush->_brush);
		spBrush->SetOpacity(a / 255.0f);
	}
	AddGraphicsObjectToCache(pBrush);
	return pBrush;
}

void CSTXD2DGraphics::OnSize(HWND hWnd, int cx, int cy, UINT cacheUniqueId)
{
	ID2D1HwndRenderTarget *pRenderTarget = (ID2D1HwndRenderTarget*)GetCachedPtr(_T("_D2DRenderTarget"), cacheUniqueId);
	if (pRenderTarget)
	{
		pRenderTarget->Resize(D2D1::SizeU(cx, cy));
		return;
	}
}

void CSTXD2DGraphics::DrawString(LPCTSTR lpszString, int x, int y, int width, int height, CSTXGraphicsFont *pFont, CSTXGraphicsBrush *pBrush, CSTXGraphicsTextFormat *pFormat)
{
	if (width < 0 || height < 0)
		return;

	CSTXD2DGraphicsBrush *pD2DBrush = dynamic_cast<CSTXD2DGraphicsBrush*>(pBrush);
	CSTXD2DGraphicsFont *pD2DFont = dynamic_cast<CSTXD2DGraphicsFont*>(pFont);
	if (pD2DBrush && pD2DFont)
	{
		if (pFormat)
		{
			if(pFormat->_wordWrap)
				pD2DFont->_font->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
			else
				pD2DFont->_font->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

			if (pFormat->_valignment == 1)
				pD2DFont->_font->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			else if (pFormat->_valignment == 0)
				pD2DFont->_font->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			else if (pFormat->_valignment == 2)
				pD2DFont->_font->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);


			if (pFormat->_alignment == 1)
				pD2DFont->_font->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			else if (pFormat->_alignment == 0)
				pD2DFont->_font->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			else if (pFormat->_alignment == 2)
				pD2DFont->_font->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);

		}

		//D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(x), static_cast<FLOAT>(y), static_cast<FLOAT>(x + width), static_cast<FLOAT>(y + height));
		//_renderTarget->DrawText(
		//	lpszString,           // Text to render
		//	_tcslen(lpszString),       // Text length
		//	pD2DFont->_font,     // Text format
		//	rect,    // The region of the window where the text will be rendered
		//	pD2DBrush->_brush	// The brush used to draw the text
		//	);
		
		CComPtr<IDWriteTextLayout> spTextLayout;
		_dwfactory->CreateTextLayout(lpszString, _tcslen(lpszString), pD2DFont->_font, static_cast<FLOAT>(width), static_cast<FLOAT>(height), &spTextLayout);

		if (spTextLayout && pFormat && pFormat->_endEllipsis)
		{
			CComPtr<IDWriteInlineObject> inlineObject;
			_dwfactory->CreateEllipsisTrimmingSign(
				spTextLayout,
				&inlineObject);
			DWRITE_TRIMMING trimming =
			{ DWRITE_TRIMMING_GRANULARITY_CHARACTER, 0, 0 };
			spTextLayout->SetTrimming(&trimming, inlineObject);
		}

		if (spTextLayout)
		{
			D2D1_POINT_2F origin = D2D1::Point2F(static_cast<FLOAT>(x), static_cast<FLOAT>(y));
			_renderTarget->DrawTextLayout(origin, spTextLayout, pD2DBrush->_brush);
		}
	}
}

CSTXGraphicsFont* CSTXD2DGraphics::CreateDrawingFont(LPCTSTR lpszFontName, float fSize, BOOL bBold, BOOL bItalic)
{
	CSTXD2DGraphicsFont *pFont = new CSTXD2DGraphicsFont();
	pFont->_fontName = lpszFontName;
	pFont->_fontSize = fSize;
	pFont->_bold = bBold;
	pFont->_italic = bItalic;

	CSTXD2DGraphicsFont *pFontCached = dynamic_cast<CSTXD2DGraphicsFont*>(GetGraphicsObjectFromCache(pFont));	//AdRef called
	if (pFontCached)
	{
		delete pFont;
		return pFontCached;
	}

	_dwfactory->CreateTextFormat(
		lpszFontName,                   // Font family name
		NULL,                          // Font collection(NULL sets it to the system font collection)
		bBold ? DWRITE_FONT_WEIGHT_BOLD : DWRITE_FONT_WEIGHT_REGULAR,    // Weight
		bItalic ? DWRITE_FONT_STYLE_ITALIC : DWRITE_FONT_STYLE_NORMAL,      // Style
		DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		fSize,                         // Size    
		L"en-us",                      // Local
		&pFont->_font                 // Pointer to recieve the created object
		);

	AddGraphicsObjectToCache(pFont);
	return pFont;
}

CSTXGraphicsImage* CSTXD2DGraphics::CreateDrawingImage(IStream *pStream, long long imageUniqueId)
{
	CSTXD2DGraphicsImage *pImage = new CSTXD2DGraphicsImage();
	pImage->_uid = imageUniqueId;

	CSTXD2DGraphicsImage *pImageCached = dynamic_cast<CSTXD2DGraphicsImage*>(GetGraphicsObjectFromCache(pImage));	//AdRef called
	if (pImageCached)
	{
		delete pImage;
		return pImageCached;
	}

	LARGE_INTEGER seekPos;
	seekPos.QuadPart = 0;
	pStream->Seek(seekPos, STREAM_SEEK_SET, NULL);
	CComPtr<IWICBitmapDecoder> spDecoder;
	CComPtr<IWICFormatConverter> spConverter;
	CComPtr<IWICBitmapFrameDecode> spBitmapFrame;

	HRESULT hr = S_OK;
	hr = _wicfactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnDemand, &spDecoder);
	RETURN_NULL_IF_FAIL(hr);
	hr = spDecoder->GetFrame(0, &spBitmapFrame);
	RETURN_NULL_IF_FAIL(hr);
	hr = _wicfactory->CreateFormatConverter(&spConverter);
	RETURN_NULL_IF_FAIL(hr);
	hr = spConverter->Initialize(spBitmapFrame, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
	RETURN_NULL_IF_FAIL(hr);

	hr = _renderTarget->CreateBitmapFromWicBitmap(spConverter, NULL, &pImage->_image);
	if (FAILED(hr))
	{
		delete pImage;
		return NULL;
	}
	
	AddGraphicsObjectToCache(pImage);
	return pImage;
}

CSTXGraphicsPolygon* CSTXD2DGraphics::CreatePolygon(POINT *pt, UINT ptCount, BOOL closePolygon)
{
	CSTXD2DGraphicsPolygon *pPolygon = new CSTXD2DGraphicsPolygon();
	CSTXD2DGraphicsPolygon *pPolygonCached = dynamic_cast<CSTXD2DGraphicsPolygon*>(GetGraphicsObjectFromCache(pPolygon));	//AdRef called
	if (pPolygonCached)
	{
		delete pPolygon;
		return pPolygonCached;
	}

	for (UINT i = 0; i < ptCount; i++)
	{
		pPolygon->_points.push_back(pt[i]);
	}

	_factory->CreatePathGeometry(&pPolygon->_path);

	CComPtr<ID2D1GeometrySink> sink;
	pPolygon->_path->Open(&sink);

	//  π”√BEGIN_HOLLOWΩ´≤ªª·ÃÓ≥‰∂‡±ﬂ–Œ
	sink->BeginFigure(D2D1::Point2F(pt[0].x, pt[0].y), D2D1_FIGURE_BEGIN_FILLED);
	for (UINT i = 1; i < ptCount; i++)
	{
		sink->AddLine(D2D1::Point2F(pt[i].x, pt[i].y));
	}

	if (closePolygon)
	{
		sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	}
	else
	{
		sink->EndFigure(D2D1_FIGURE_END_OPEN);
	}

	sink->Close();

	pPolygon->_closePolygon = closePolygon;

	AddGraphicsObjectToCache(pPolygon);
	return pPolygon;
}

void CSTXD2DGraphics::DrawImage(int x, int y, int width, int height, CSTXGraphicsImage *pImage, CSTXGraphicsImageFormat* pImageFormat)
{
	CSTXD2DGraphicsImage *pD2DImage = dynamic_cast<CSTXD2DGraphicsImage*>(pImage);
	if (pD2DImage)
	{
		FLOAT fOpacity = pImageFormat ? pImageFormat->_opacity : 1.0f;
		CComQIPtr<ID2D1DeviceContext> spDeviceContext = _renderTarget;

		if (spDeviceContext /*pImageFormat && pImageFormat->_grey*/)
		{
			D2D1_SIZE_F imgSize = pD2DImage->_image->GetSize();

			CComPtr<ID2D1Effect> spFinalEffect;
			CComPtr<ID2D1Effect> spScaleEffect;
			CComPtr<ID2D1Effect> spSaturationEffect;
			if (spDeviceContext)
			{
				spDeviceContext->CreateEffect(CLSID_D2D1Scale, &spScaleEffect);
				if (spScaleEffect)
				{
					spFinalEffect = spScaleEffect;
					spScaleEffect->SetInput(0, pD2DImage->_image);
					spScaleEffect->SetValue(D2D1_SCALE_PROP_CENTER_POINT, D2D1::Vector2F(0.0f, 0.0f));
					spScaleEffect->SetValue(D2D1_SCALE_PROP_SCALE, D2D1::Vector2F(width / imgSize.width, height / imgSize.height));
					CComPtr<ID2D1Image> spScaledImage;
					spScaleEffect->GetOutput(&spScaledImage);

					if (pImageFormat && pImageFormat->_grey)
					{
						spSaturationEffect->SetInput(0, spScaledImage);
						spSaturationEffect->SetValue(D2D1_SATURATION_PROP_SATURATION, 0.0f);
						spFinalEffect = spSaturationEffect;
					}

					CComPtr<ID2D1Effect> spColorMatrixEffect;
					spDeviceContext->CreateEffect(CLSID_D2D1ColorMatrix, &spColorMatrixEffect);
					if (spColorMatrixEffect)
					{
						spColorMatrixEffect->SetInput(0, spScaledImage);
						D2D1_MATRIX_5X4_F matrix = D2D1::Matrix5x4F(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, fOpacity, 0, 0, 0, 0);
						spColorMatrixEffect->SetValue(D2D1_COLORMATRIX_PROP_COLOR_MATRIX, matrix);
						spFinalEffect = spColorMatrixEffect;
					}

					D2D1_POINT_2F pt = D2D1::Point2F(static_cast<FLOAT>(x), static_cast<FLOAT>(y));
					D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(x), static_cast<FLOAT>(y), static_cast<FLOAT>(x + width), static_cast<FLOAT>(y + height));
					spDeviceContext->DrawImage(spFinalEffect, pt, rect, D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC);
					return;

					/*
					spDeviceContext->CreateEffect(CLSID_D2D1Saturation, &spSaturationEffect);
					if (spSaturationEffect)
					{
						spSaturationEffect->SetInput(0, spScaledImage);
						spSaturationEffect->SetValue(D2D1_SATURATION_PROP_SATURATION, 0.0f);
						D2D1_POINT_2F pt = D2D1::Point2F(static_cast<FLOAT>(x), static_cast<FLOAT>(y));
						D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(x), static_cast<FLOAT>(y), static_cast<FLOAT>(x + width), static_cast<FLOAT>(y + height));
						spDeviceContext->DrawImage(spSaturationEffect, pt, rect, D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC);
						return;
					}
					*/
				}
			}
		}

		D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(x), static_cast<FLOAT>(y), static_cast<FLOAT>(x + width), static_cast<FLOAT>(y + height));
		_renderTarget->DrawBitmap(pD2DImage->_image, rect, fOpacity, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR);
	}
}

void CSTXD2DGraphics::Clear(UINT cacheUniqueId)
{
	ID2D1Factory *pFactory = (ID2D1Factory*)GetCachedPtr(_T("_D2DFactory"), cacheUniqueId);
	IDWriteFactory *pDWFactory = (IDWriteFactory*)GetCachedPtr(_T("_DWFactory"), cacheUniqueId);
	IWICImagingFactory *pWICFactory = (IWICImagingFactory*)GetCachedPtr(_T("_WICFactory"), cacheUniqueId);
	ID2D1RenderTarget *pRenderTarget = (ID2D1RenderTarget*)GetCachedPtr(_T("_D2DRenderTarget"), cacheUniqueId);

	if (pRenderTarget)
		pRenderTarget->Release();
	if (pFactory)
		pFactory->Release();
	if (pDWFactory)
		pDWFactory->Release();
	if (pWICFactory)
		pWICFactory->Release();

	SetCachedPtr(_T("_D2DFactory"), NULL, cacheUniqueId);
	SetCachedPtr(_T("_DWFactory"), NULL, cacheUniqueId);
	SetCachedPtr(_T("_WICFactory"), NULL, cacheUniqueId);
	SetCachedPtr(_T("_D2DRenderTarget"), NULL, cacheUniqueId);
}

void CSTXD2DGraphics::Clear()
{
	Clear(_cacheId);
}

void CSTXD2DGraphics::DiscardDeviceDependentResource()
{
	if(_renderTarget)
		_renderTarget.Detach()->Release();

	SetCachedPtr(_T("_D2DRenderTarget"), NULL, _cacheId);
	ClearCachedGraphicsObjects(_cacheId);
}

CSTXGraphicsPen* CSTXD2DGraphics::CreateDrawingPen(byte r, byte g, byte b, byte a, float fWidth /*= 1.0f*/)
{
	CSTXD2DGraphicsPen *pPen = new CSTXD2DGraphicsPen();
	pPen->_r = r;
	pPen->_g = g;
	pPen->_b = b;
	pPen->_a = a;
	pPen->_width = fWidth;

	CSTXD2DGraphicsPen *pPenCached = dynamic_cast<CSTXD2DGraphicsPen*>(GetGraphicsObjectFromCache(pPen));	//AdRef called
	if (pPenCached)
	{
		delete pPen;
		return pPenCached;
	}

	CComPtr<ID2D1SolidColorBrush> spBrush;
	HRESULT hr = _renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(static_cast<FLOAT>(r / 255.0f), static_cast<FLOAT>(g / 255.0f), static_cast<FLOAT>(b / 255.0f), static_cast<FLOAT>(a / 255.0f)), &spBrush);
	if (SUCCEEDED(hr))
	{
		spBrush->QueryInterface(&pPen->_brush);
	}
	AddGraphicsObjectToCache(pPen);
	return pPen;
}

void CSTXD2DGraphics::DrawRichText(ITextServices *pTextService, int x, int y, int width, int height)
{
	HRESULT hr = S_OK;
	RECTL rcl = { x, y, x + width, y + height };
	RECT rcUpdate = { x, y, x + width, y + height };

	//ITextServices2* spTextService2 = NULL;
	//pTextService->QueryInterface(IID_ITextServices2, (void**)&spTextService2);
	//if (spTextService2)
	//{
	//	hr = spTextService2->TxDrawD2D(_renderTarget, &rcl, &rcUpdate, TXTVIEW_ACTIVE);
	//	spTextService2->Release();
	//	return;
	//}

	CComQIPtr<ID2D1GdiInteropRenderTarget> spGDIRenderTarget = _renderTarget;
	HDC hDCRender = NULL;
	hr = spGDIRenderTarget->GetDC(D2D1_DC_INITIALIZE_MODE_COPY, &hDCRender);

	hr = pTextService->TxDraw(DVASPECT_CONTENT, 0, NULL, NULL, hDCRender, 0,
		&rcl, NULL, NULL, NULL, 0, TXTVIEW_ACTIVE);

	hr = spGDIRenderTarget->ReleaseDC(&rcUpdate);
}

void CSTXD2DGraphics::FillPolygon(CSTXGraphicsPolygon *pPolygon, CSTXGraphicsBrush *pBrush)
{
	CSTXD2DGraphicsBrush *pD2DBrush = dynamic_cast<CSTXD2DGraphicsBrush*>(pBrush);
	CSTXD2DGraphicsPolygon *pD2DPolygon = dynamic_cast<CSTXD2DGraphicsPolygon*>(pPolygon);
	if (pD2DBrush && pD2DPolygon)
	{
		_renderTarget->FillGeometry(pD2DPolygon->_path, pD2DBrush->_brush);
	}
}

void CSTXD2DGraphics::DrawRoundedRectangle(int x, int y, int width, int height, int d, CSTXGraphicsPen *pPen)
{
	CSTXD2DGraphicsPen *pD2DPen = dynamic_cast<CSTXD2DGraphicsPen*>(pPen);
	if (pD2DPen)
	{
		D2D1_RECT_F rect = D2D1::RectF(static_cast<FLOAT>(x), static_cast<FLOAT>(y), static_cast<FLOAT>(x + width), static_cast<FLOAT>(y + height));
		D2D1_ROUNDED_RECT rounded_rect = D2D1::RoundedRect(rect, d, d);
		_renderTarget->DrawRoundedRectangle(rounded_rect, pD2DPen->_brush, pD2DPen->_width);
	}
}

//////////////////////////////////////////////////////////////////////////

void CSTXD2DDCGraphics::Create(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID)
{
	_cacheId = cacheUniqueID;

	CComPtr<ID2D1SolidColorBrush> pWhiteBrush;
	D2D1_SIZE_U size = D2D1::SizeU(cx, cy);
	_sizeDC.cx = cx;
	_sizeDC.cy = cy;

	HRESULT hr = S_OK;
	ID2D1Factory *pFactory = NULL;

	pFactory = (ID2D1Factory*)GetCachedPtr(_T("_D2DFactory"), _cacheId);
	if (pFactory)
	{
		_factory = pFactory;
	}
	else
	{
		hr = D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			&pFactory
			);
		SetCachedPtr(_T("_D2DFactory"), pFactory, _cacheId);
		_factory = pFactory;
	}

	IWICImagingFactory *pWICFactory = (IWICImagingFactory*)GetCachedPtr(_T("_WICFactory"), _cacheId);
	if (pWICFactory)
	{
		_wicfactory = pWICFactory;
	}
	else
	{
		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID*)&pWICFactory
			);
		SetCachedPtr(_T("_WICFactory"), pWICFactory, _cacheId);
		_wicfactory = pWICFactory;
	}

	ID2D1RenderTarget *pRenderTarget = (ID2D1RenderTarget*)GetCachedPtr(_T("_D2DRenderTarget"), _cacheId);
	if (pRenderTarget)
	{
		_renderTarget = pRenderTarget;
		_dcRenderTarget = _renderTarget;
	}
	else
	{
		ID2D1DCRenderTarget *pDCRenderTarget = NULL;
		hr = _factory->CreateDCRenderTarget(
			&D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
			&pDCRenderTarget);

		SetCachedPtr(_T("_D2DRenderTarget"), pDCRenderTarget, _cacheId);
		_dcRenderTarget = pDCRenderTarget;
		_renderTarget = _dcRenderTarget;
		_renderTarget->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	}


	_bitmap.Create(cx, cy);
	RECT rc = { 0, 0, cx, cy };
	_dcRenderTarget->BindDC(_bitmap.GetDC(), &rc);

	//hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0xffffffff, 1.0), &pWhiteBrush);

	//BeginDraw();
	//CSTXGraphicsBrush *pBrush = CreateSimpleLinearGradientBrush(0, 0, 0, 114, 198, 255, 100, 100, 255, 255, 255, 64, 255);
	//FillRectangle(0, 0, static_cast<int>(100), 100, pBrush);
	//pBrush->Release();

	//EndDraw();
}

BOOL CSTXD2DDCGraphics::ApplyToLayeredWindow(HWND hWnd)
{
	BLENDFUNCTION blend = {};
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	POINT ptSrc = { 0, 0 };
	UPDATELAYEREDWINDOWINFO info = {};
	info.cbSize = sizeof(UPDATELAYEREDWINDOWINFO);
	info.pptSrc = &ptSrc;
	info.pptDst = NULL;
	info.psize = &_sizeDC;
	info.pblend = &blend;
	info.dwFlags = ULW_ALPHA;

	info.hdcSrc = _bitmap.GetDC();

	return UpdateLayeredWindow(hWnd, NULL, NULL, &_sizeDC, _bitmap.GetDC(), &ptSrc, 0, &blend, ULW_ALPHA);

	//return UpdateLayeredWindowIndirect(hWnd, &info);
}

#endif