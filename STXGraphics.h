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



#pragma once
#include <WTypes.h>
#include <Unknwn.h>
#include <tchar.h>
#include <gdiplus.h>
#if _WIN32_WINNT >= 0x0601
#include <d2d1.h>
#include <dwrite.h>
#endif
#include <atlcomcli.h>
#include <map>
#include <WinCodec.h>
#include <set>

#include <Richedit.h>
#include <imm.h>
#include <Textserv.h>
#include <vector>

//////////////////////////////////////////////////////////////////////////

// IID_ITextServices2 is for drawing rich text through Direct2D
DEFINE_GUID(IID_ITextServices2, 0x8D33F741, 0xCF58, 0x11CE, 0xA8, 0x9D, 0x00, 0xAA, 0x00, 0x6C, 0xAD, 0xC5);


//////////////////////////////////////////////////////////////////////////
// Forward declaration

class CSTXGraphics;
class CSTXLayeredGraphics;

//////////////////////////////////////////////////////////////////////////
// CSTXGraphicsObject
// Base class of all drawing objects such as Pen, Brush, Image etc.
// It provide basic reference counting. For convenient, I use IUnknown as base interface directly

class CSTXGraphicsObject : public IUnknown
{
public:
	CSTXGraphicsObject();
	virtual ~CSTXGraphicsObject();

protected:
	LONG m_nRef;

public:
	// this method is called when a drawing object is needed to be compared with the other ones
	// if two objects have the same properties and property values, they are considered equal
	// For optimization, there is a cache that holds all drawing objects used before.
	//   Creating a new drawing object will first try to reuse the ones already in the cache. 
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;

public:
	// IUnknown
	STDMETHOD_(ULONG, AddRef)(void);
	STDMETHOD_(ULONG, Release)(void);
	STDMETHOD(QueryInterface)(const IID &, void **);
};

//////////////////////////////////////////////////////////////////////////
// All drawing object class have a fixed prefix 'CSTX'
// the naming conventions is CSTX+[D2D/GdiPlus]+Graphics+Name
// e.g. CSTXGdiPlusGraphicsBrush, it is a Brush wrapper class specific for GDI+
// Base classes will nave no D2D or GdiPlus in names.

//////////////////////////////////////////////////////////////////////////
// CSTXGraphicsBrush
// Abstraction for Brush objects

class CSTXGraphicsBrush : public CSTXGraphicsObject
{
public:
	CSTXGraphicsBrush();
	virtual ~CSTXGraphicsBrush();

public:
	virtual void SetOpacity(byte alpha);		//0 to 255
	virtual void SetOpacityFloat(float alpha);		//0 to 1
};

class CSTXGdiPlusGraphicsBrush : public CSTXGraphicsBrush
{
	friend class CSTXGdiPlusGraphics;
public:
	CSTXGdiPlusGraphicsBrush();
	virtual ~CSTXGdiPlusGraphicsBrush();

protected:
	Gdiplus::Brush *_brush;

public:
	virtual Gdiplus::Brush* GetGdiPlusBrush();

};

class CSTXGdiPlusGraphicsSolidBrush : public CSTXGdiPlusGraphicsBrush
{
	friend class CSTXGdiPlusGraphics;
public:

protected:
	BYTE _r;
	BYTE _g;
	BYTE _b;
	BYTE _a;

public:
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;
	virtual void SetOpacityFloat(float alpha);		//0 to 1
};

class CSTXGdiPlusGraphicsButtonGradientBrush : public CSTXGdiPlusGraphicsBrush
{
	friend class CSTXGdiPlusGraphics;
public:

protected:
	int _x1;
	int _x2;
	int _y1;
	int _y2;
	BYTE _r;
	BYTE _g;
	BYTE _b;
	BYTE _a;
	float _angle;

public:
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;
	virtual void SetOpacityFloat(float alpha);		//0 to 1

};

class CSTXGdiPlusGraphicsSimpleLinearGradientBrush : public CSTXGdiPlusGraphicsBrush
{
	friend class CSTXGdiPlusGraphics;
public:

protected:
	int _x1;
	int _x2;
	int _y1;
	int _y2;
	BYTE _r1;
	BYTE _g1;
	BYTE _b1;
	BYTE _a1;
	BYTE _r2;
	BYTE _g2;
	BYTE _b2;
	BYTE _a2;
	float _angle;

public:
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;
	virtual void SetOpacityFloat(float alpha);		//0 to 1
};


#if _WIN32_WINNT >= 0x0601
class CSTXD2DGraphicsBrush : public CSTXGraphicsBrush
{
	friend class CSTXD2DGraphics;
public:
	CSTXD2DGraphicsBrush();
	virtual ~CSTXD2DGraphicsBrush();

protected:
	CComPtr<ID2D1Brush>_brush;

public:
	virtual void SetOpacityFloat(float alpha);
};

class CSTXD2DGraphicsSolidBrush : public CSTXD2DGraphicsBrush
{
	friend class CSTXD2DGraphics;
public:

protected:
	BYTE _r;
	BYTE _g;
	BYTE _b;
	BYTE _a;

public:
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;
};

class CSTXD2DGraphicsButtonGradientBrush : public CSTXD2DGraphicsBrush
{
	friend class CSTXD2DGraphics;
public:

protected:
	int _x1;
	int _x2;
	int _y1;
	int _y2;
	BYTE _r;
	BYTE _g;
	BYTE _b;
	BYTE _a;
	float _angle;

public:
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;

};

class CSTXD2DGraphicsSimpleLinearGradientBrush : public CSTXD2DGraphicsBrush
{
	friend class CSTXD2DGraphics;
public:

protected:
	int _x1;
	int _x2;
	int _y1;
	int _y2;
	BYTE _r1;
	BYTE _g1;
	BYTE _b1;
	BYTE _a1;
	BYTE _r2;
	BYTE _g2;
	BYTE _b2;
	BYTE _a2;
	BYTE _alpha;
	float _angle;

public:
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;
};
#endif

//////////////////////////////////////////////////////////////////////////

class CSTXGraphicsPen : public CSTXGraphicsObject
{
public:
	CSTXGraphicsPen();
	virtual ~CSTXGraphicsPen();
};

class CSTXGdiPlusGraphicsPen : public CSTXGraphicsPen
{
	friend class CSTXGdiPlusGraphics;
public:
	CSTXGdiPlusGraphicsPen();
	virtual ~CSTXGdiPlusGraphicsPen();

protected:
	Gdiplus::Pen *_pen;
	BYTE _r;
	BYTE _g;
	BYTE _b;
	BYTE _a;
	float _width;

public:
	virtual Gdiplus::Pen* GetGdiPlusPen();

public:
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;
};

#if _WIN32_WINNT >= 0x0601
class CSTXD2DGraphicsPen : public CSTXGraphicsPen
{
	friend class CSTXD2DGraphics;
protected:
	BYTE _r;
	BYTE _g;
	BYTE _b;
	BYTE _a;
	float _width;

	CComPtr<ID2D1Brush>_brush;

public:
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;
};
#endif
//////////////////////////////////////////////////////////////////////////

class CSTXGraphicsFont : public CSTXGraphicsObject
{
public:
	virtual ~CSTXGraphicsFont();

};

class CSTXGdiPlusGraphicsFont : public CSTXGraphicsFont
{
	friend class CSTXGdiPlusGraphics;
public:
	CSTXGdiPlusGraphicsFont();
	virtual ~CSTXGdiPlusGraphicsFont();

protected:
	Gdiplus::Font *_font;
	std::wstring _fontName;
	float _fontSize;
	BOOL _bold;
	BOOL _italic;

public:
	Gdiplus::Font* GetGdiPlusFont();

public:
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;

};

#if _WIN32_WINNT >= 0x0601
class CSTXD2DGraphicsFont : public CSTXGraphicsFont
{
	friend class CSTXD2DGraphics;
public:
	CSTXD2DGraphicsFont();
	virtual ~CSTXD2DGraphicsFont();

protected:
	CComPtr <IDWriteTextFormat> _font;
	std::wstring _fontName;
	float _fontSize;
	BOOL _bold;
	BOOL _italic;

public:
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;

};
#endif

//////////////////////////////////////////////////////////////////////////

class CSTXGraphicsTextFormat
{
public:
	CSTXGraphicsTextFormat();
	virtual ~CSTXGraphicsTextFormat();

public:
	int _alignment;			//0: Left(Near),  1:Center,  2:Right(Far)
	int _valignment;		//0: Top,  1:Center,  2:Bottom
	BOOL _endEllipsis;
	BOOL _wordWrap;
};

class CSTXGdiPlusGraphicsTextFormat : public CSTXGraphicsTextFormat
{

public:
	CSTXGdiPlusGraphicsTextFormat();
	virtual ~CSTXGdiPlusGraphicsTextFormat();


public:

};

//////////////////////////////////////////////////////////////////////////

class CSTXGraphicsImage : public CSTXGraphicsObject
{
public:
	long long _uid;

public:
	CSTXGraphicsImage();
	virtual ~CSTXGraphicsImage();

public:
	virtual int GetWidth();
	virtual int GetHeight();

public:
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;

};

class CSTXGdiPlusGraphicsImage : public CSTXGraphicsImage
{
	friend class CSTXGdiPlusGraphics;
public:
	CSTXGdiPlusGraphicsImage();
	virtual ~CSTXGdiPlusGraphicsImage();

protected:
	Gdiplus::Image *_image;

public:
	virtual int GetWidth();
	virtual int GetHeight();

public:
	Gdiplus::Image* GetGdiPlusImage();

};

#if _WIN32_WINNT >= 0x0601
class CSTXD2DGraphicsImage : public CSTXGraphicsImage
{
	friend class CSTXD2DGraphics;
public:
	CSTXD2DGraphicsImage();
	virtual ~CSTXD2DGraphicsImage();

public:
	virtual int GetWidth();
	virtual int GetHeight();


protected:
	CComPtr<ID2D1Bitmap> _image;
};
#endif

//////////////////////////////////////////////////////////////////////////

class CSTXGraphicsImageFormat
{
public:
	CSTXGraphicsImageFormat();
	virtual ~CSTXGraphicsImageFormat();

public:
	float _opacity;		//0.0 (Transparent) to 1.0 (Opaque)
	BOOL _grey;
};

//////////////////////////////////////////////////////////////////////////

class CSTXGraphicsPolygon : public CSTXGraphicsObject
{
public:
	CSTXGraphicsPolygon();

public:
	BOOL _closePolygon;
	std::vector<POINT> _points;

public:
	virtual int CompareTo(const CSTXGraphicsObject *pObject) const;
};

class CSTXGdiPlusGraphicsPolygon : public CSTXGraphicsPolygon
{
public:
	std::vector<Gdiplus::Point> _gdiPlusPoints;

};


class CSTXD2DGraphicsPolygon : public CSTXGraphicsPolygon
{
public:
	CComPtr<ID2D1PathGeometry> _path;

};

//////////////////////////////////////////////////////////////////////////

class CSTXGraphicsMatrix
{
public:
	virtual ~CSTXGraphicsMatrix();

public:
	virtual CSTXGraphicsMatrix* Clone();

	// Reset this matrix to identity matrix
	virtual void Reset();

	// nMatrixOrder :  0 - Prepend, 1 - Append
	virtual CSTXGraphicsMatrix* Multiply(CSTXGraphicsMatrix *pMatrix, int nMatrixOrder);
	virtual CSTXGraphicsMatrix* Translate(int dx, int dy, int nMatrixOrder);
	virtual CSTXGraphicsMatrix* Rotate(double fAngle, int nMatrixOrder);
	virtual CSTXGraphicsMatrix* Scale(double scaleX, double scaleY, int nMatrixOrder);
};


class CSTXGdiPlusGraphicsMatrix : public CSTXGraphicsMatrix
{
	friend class CSTXGdiPlusGraphics;
public:
	CSTXGdiPlusGraphicsMatrix();
	virtual ~CSTXGdiPlusGraphicsMatrix();

protected:
	Gdiplus::Matrix *_matrix;

public:
	Gdiplus::Matrix* GetGdiPlusMatrix();

	virtual CSTXGraphicsMatrix* Clone();
	virtual void Reset();

	// nMatrixOrder :  0 - Prepend, 1 - Append
	virtual CSTXGraphicsMatrix* Multiply(CSTXGraphicsMatrix *pMatrix, int nMatrixOrder);
	virtual CSTXGraphicsMatrix* Translate(int dx, int dy, int nMatrixOrder);
	virtual CSTXGraphicsMatrix* Rotate(double fAngle, int nMatrixOrder);
	virtual CSTXGraphicsMatrix* Scale(double scaleX, double scaleY, int nMatrixOrder);

};

#if _WIN32_WINNT >= 0x0601
class CSTXD2DGraphicsMatrix : public CSTXGraphicsMatrix
{
	friend class CSTXD2DGraphics;
public:
	CSTXD2DGraphicsMatrix();
	virtual ~CSTXD2DGraphicsMatrix();

protected:
	D2D1_MATRIX_3X2_F *_matrix;

public:
	virtual CSTXGraphicsMatrix* Clone();
	virtual void Reset();

	// nMatrixOrder :  0 - Prepend, 1 - Append
	virtual CSTXGraphicsMatrix* Multiply(CSTXGraphicsMatrix *pMatrix, int nMatrixOrder);
	virtual CSTXGraphicsMatrix* Translate(int dx, int dy, int nMatrixOrder);
	virtual CSTXGraphicsMatrix* Rotate(double fAngle, int nMatrixOrder);
	virtual CSTXGraphicsMatrix* Scale(double scaleX, double scaleY, int nMatrixOrder);

};
#endif

//////////////////////////////////////////////////////////////////////////

class CGraphicsRoundRectPath : public Gdiplus::GraphicsPath
{
public:
	CGraphicsRoundRectPath();
	CGraphicsRoundRectPath(INT x, INT y, INT width, INT height, INT cornerX, INT cornerY);

public:
	void AddRoundRect(INT x, INT y, INT width, INT height, INT cornerX, INT cornerY);
};

//////////////////////////////////////////////////////////////////////////
// GdiBitmap - DIB Wrapper Class

class GdiBitmap
{

	UINT m_width;
	UINT m_height;
	UINT m_stride;
	void* m_bits;
	HBITMAP m_oldBitmap;

	HDC m_dc;
	HBITMAP m_bitmap;

public:

	BOOL Create(__in UINT width, __in UINT height)
	{
		m_width = width;
		m_height = height;
		m_stride = (width * 32 + 31) / 32 * 4;
		m_bits = 0;
		m_oldBitmap = 0;

		BITMAPINFO bitmapInfo = {};
		bitmapInfo.bmiHeader.biSize =
			sizeof(bitmapInfo.bmiHeader);
		bitmapInfo.bmiHeader.biWidth =
			width;
		bitmapInfo.bmiHeader.biHeight =
			0 - height;
		bitmapInfo.bmiHeader.biPlanes = 1;
		bitmapInfo.bmiHeader.biBitCount = 32;
		bitmapInfo.bmiHeader.biCompression =
			BI_RGB;

		m_bitmap = CreateDIBSection(
			0, // device context
			&bitmapInfo,
			DIB_RGB_COLORS,
			&m_bits,
			0, // file mapping object
			0); // file offset
		if (0 == m_bits)
		{
			return FALSE;
		}

		HDC hDCDesktop = ::GetDC(::GetDesktopWindow());
		m_dc = ::CreateCompatibleDC(hDCDesktop);
		::ReleaseDC(::GetDesktopWindow(), hDCDesktop);

		if (0 == m_dc)
		{
			DeleteObject(m_bitmap);
			return FALSE;
		}

		m_oldBitmap = (HBITMAP)::SelectObject(m_dc, m_bitmap);

		return TRUE;
	}

	GdiBitmap()
	{
		m_bitmap = NULL;
		m_dc = NULL;
	}

	~GdiBitmap() {
		SelectObject(m_dc, m_oldBitmap);
		::DeleteDC(m_dc);
		DeleteObject(m_bitmap);
	}

	UINT GetWidth() const {
		return m_width;
	}

	UINT GetHeight() const {
		return m_height;
	}

	UINT GetStride() const {
		return m_stride;
	}

	void* GetBits() const {
		return m_bits;
	}

	HDC GetDC() const {
		return m_dc;
	}
};

//////////////////////////////////////////////////////////////////////////

class CSTXGraphics
{
public:
	CSTXGraphics();
	virtual ~CSTXGraphics();

	struct CObjectLess
	{
		bool operator()(const CSTXGraphicsObject* v1, const CSTXGraphicsObject* v2) const
		{
			return v1->CompareTo(v2) < 0;
		}
	};

protected:
	HWND _hwnd;
	HDC _hdc;
	int _cx;
	int _cy;
	UINT _cacheId;
	static std::map<UINT, std::set<CSTXGraphicsObject*, CObjectLess>> _sobjectCache;		//Cache UniqueId -> Objects
	static std::map<UINT, std::map<std::wstring, IUnknown*>> _sptrCache;						//Cache UniqueId -> ptr

public:
	virtual void AddGraphicsObjectToCache(CSTXGraphicsObject *pObject);
	virtual CSTXGraphicsObject* GetGraphicsObjectFromCache(CSTXGraphicsObject *pObject);
	static void ClearCachedGraphicsObjects(UINT cacheUniqueID = 0);
	static IUnknown* GetCachedPtr(LPCTSTR lpszKey, UINT cacheUniqueID = 0);
	static void SetCachedPtr(LPCTSTR lpszKey, IUnknown* ptr, UINT cacheUniqueID = 0);

public:
	static CSTXGraphics* CreateGdiPlusGraphics(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID);
#if _WIN32_WINNT >= 0x0601
	static CSTXGraphics* CreateD2DGraphics(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID);
#endif
	
	// This will call CreateD2DGraphics if running on Windows 7 and later, otherwise, call CreateGdiPlusGraphics for older system.
	static CSTXGraphics* CreateAutoGraphics(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID = 0);
	static CSTXLayeredGraphics* CreateAutoLayeredGraphics(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID = 0);

public:
	static void ScaleRect(LPRECT lpRect, DOUBLE fScale);
	static void ScaleRect(Gdiplus::Rect *lpRect, DOUBLE fScale);
	static void ScaleRect(Gdiplus::RectF *lpRect, DOUBLE fScale);

public:
	virtual CSTXGraphicsBrush* CreateSolidBrush(byte r, byte g, byte b, byte a);
	virtual CSTXGraphicsBrush* CreateSimpleLinearGradientBrush(int x1, int y1, byte r1, byte g1, byte b1, byte a1, int x2, int y2, byte r2, byte g2, byte b2, byte a2, byte alpha, FLOAT angle = 0);
	virtual CSTXGraphicsBrush* CreateButtonGradientBrush(int x1, int y1, int x2, int y2, byte r, byte g, byte b, byte a, FLOAT angle = 0);
	virtual CSTXGraphicsFont* CreateDrawingFont(LPCTSTR lpszFontName, float fSize, BOOL bBold, BOOL bItalic);
	virtual CSTXGraphicsImage* CreateDrawingImage(IStream *pStream, long long imageUniqueId);
	virtual CSTXGraphicsImage* CreateDrawingImage(LPCTSTR lpszImageFile);
	virtual CSTXGraphicsImage* CreateDrawingImage(LPCTSTR lpszImageFile, long long imageUniqueId);
	virtual CSTXGraphicsPolygon* CreatePolygon(POINT *pt, UINT ptCount, BOOL closePolygon);
	virtual CSTXGraphicsPen* CreateDrawingPen(byte r, byte g, byte b, byte a, float fWidth = 1.0f);
	virtual void TranslateTransform(int dx, int dy, int nMatrixOrder);
	virtual void RotateTransform(double fAngle, int nMatrixOrder);
	virtual CSTXGraphicsMatrix* GetTransform();
	virtual void SetTransform(CSTXGraphicsMatrix *pMatrix);
	virtual void ResetTransform();
	virtual HDC GetSafeHDC();

public:
	virtual void Create(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID = 0);
	virtual void BeginDraw();
	virtual HRESULT EndDraw();
	virtual void DiscardDeviceDependentResource();

public:
	virtual void FillRectangle(int x, int y, int width, int height, byte r, byte g, byte b, byte a);
	virtual void FillRectangle(int x, int y, int width, int height, CSTXGraphicsBrush *pBrush);
	virtual void DrawString(LPCTSTR lpszString, int x, int y, int width, int height, CSTXGraphicsFont *pFont, CSTXGraphicsBrush *pBrush, CSTXGraphicsTextFormat *pFormat = NULL);
	virtual void DrawImage(int x, int y, int width, int height, CSTXGraphicsImage *pImage, CSTXGraphicsImageFormat* pImageFormat = NULL);
	virtual void DrawRoundedRectangle(int x, int y, int width, int height, int d, CSTXGraphicsPen *pPen);
	virtual void FillRoundedRectangle(int x, int y, int width, int height, int d, CSTXGraphicsBrush *pBrush);
	virtual void DrawRichText(ITextServices *pTextService, int x, int y, int width, int height);
	virtual void FillPolygon(CSTXGraphicsPolygon *pPolygon, CSTXGraphicsBrush *pBrush);


};

//////////////////////////////////////////////////////////////////////////

class CSTXLayeredGraphics
{
public:
	virtual BOOL ApplyToLayeredWindow(HWND hWnd) { return FALSE; };

};


//////////////////////////////////////////////////////////////////////////

class CSTXGdiPlusGraphics : public CSTXGraphics
{
public:
	CSTXGdiPlusGraphics();
	virtual ~CSTXGdiPlusGraphics();

public:
	Gdiplus::Graphics *_graphics;
	Gdiplus::Bitmap *_memBitmap;
	Gdiplus::Graphics *_memGraphics;

public:
	virtual CSTXGraphicsBrush* CreateSolidBrush(byte r, byte g, byte b, byte a);
	virtual CSTXGraphicsBrush* CreateSimpleLinearGradientBrush(int x1, int y1, byte r1, byte g1, byte b1, byte a1, int x2, int y2, byte r2, byte g2, byte b2, byte a2, byte alpha, FLOAT angle = 0);
	virtual CSTXGraphicsBrush* CreateButtonGradientBrush(int x1, int y1, int x2, int y2, byte r, byte g, byte b, byte a, FLOAT angle = 0);
	virtual CSTXGraphicsFont* CreateDrawingFont(LPCTSTR lpszFontName, float fSize, BOOL bBold, BOOL bItalic);
	virtual CSTXGraphicsImage* CreateDrawingImage(IStream *pStream, long long imageUniqueId);
	virtual CSTXGraphicsPolygon* CreatePolygon(POINT *pt, UINT ptCount, BOOL closePolygon);
	virtual CSTXGraphicsPen* CreateDrawingPen(byte r, byte g, byte b, byte a, float fWidth = 1.0f);
	virtual void TranslateTransform(int dx, int dy, int nMatrixOrder);
	virtual void RotateTransform(double fAngle, int nMatrixOrder);
	virtual CSTXGraphicsMatrix* GetTransform();
	virtual void SetTransform(CSTXGraphicsMatrix *pMatrix);
	virtual void ResetTransform();

public:
	virtual void Create(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID = 0);
	virtual void BeginDraw();
	virtual HRESULT EndDraw();

public:
	virtual void FillRectangle(int x, int y, int width, int height, CSTXGraphicsBrush *pBrush);
	virtual void DrawString(LPCTSTR lpszString, int x, int y, int width, int height, CSTXGraphicsFont *pFont, CSTXGraphicsBrush *pBrush, CSTXGraphicsTextFormat *pFormat = NULL);
	virtual void DrawImage(int x, int y, int width, int height, CSTXGraphicsImage *pImage, CSTXGraphicsImageFormat* pImageFormat = NULL);
	virtual void DrawRoundedRectangle(int x, int y, int width, int height, int d, CSTXGraphicsPen *pPen);
	virtual void FillRoundedRectangle(int x, int y, int width, int height, int d, CSTXGraphicsBrush *pBrush);
	virtual void DrawRichText(ITextServices *pTextService, int x, int y, int width, int height);
	virtual void FillPolygon(CSTXGraphicsPolygon *pPolygon, CSTXGraphicsBrush *pBrush);
};

class CSTXGdiPlusDCGraphics : public CSTXGdiPlusGraphics, public CSTXLayeredGraphics
{

protected:
	GdiBitmap _bitmap;
	SIZE _sizeDC;

public:
	virtual void Create(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID = 0);

public:
	virtual BOOL ApplyToLayeredWindow(HWND hWnd);

};

//////////////////////////////////////////////////////////////////////////

#if _WIN32_WINNT >= 0x0601
class CSTXD2DGraphics : public CSTXGraphics
{
public:
	CSTXD2DGraphics();
	virtual ~CSTXD2DGraphics();

public:
	static void OnSize(HWND hWnd, int cx, int cy, UINT cacheUniqueId);

protected:
	CComPtr<ID2D1Factory> _factory;
	CComPtr<IDWriteFactory> _dwfactory;
	CComPtr<IWICImagingFactory> _wicfactory;
	CComQIPtr<ID2D1RenderTarget> _renderTarget;

public:
	void Clear();
	static void Clear(UINT cacheUniqueId);

public:
	virtual CSTXGraphicsMatrix* GetTransform();
	virtual void SetTransform(CSTXGraphicsMatrix *pMatrix);
	virtual void ResetTransform();
	virtual CSTXGraphicsBrush* CreateSolidBrush(byte r, byte g, byte b, byte a);
	virtual CSTXGraphicsBrush* CreateSimpleLinearGradientBrush(int x1, int y1, byte r1, byte g1, byte b1, byte a1, int x2, int y2, byte r2, byte g2, byte b2, byte a2, byte alpha, FLOAT angle = 0);
	virtual CSTXGraphicsBrush* CreateButtonGradientBrush(int x1, int y1, int x2, int y2, byte r, byte g, byte b, byte a, FLOAT angle = 0);
	virtual CSTXGraphicsFont* CreateDrawingFont(LPCTSTR lpszFontName, float fSize, BOOL bBold, BOOL bItalic);
	virtual CSTXGraphicsPen* CreateDrawingPen(byte r, byte g, byte b, byte a, float fWidth = 1.0f);
	virtual CSTXGraphicsImage* CreateDrawingImage(IStream *pStream, long long imageUniqueId);
	virtual CSTXGraphicsPolygon* CreatePolygon(POINT *pt, UINT ptCount, BOOL closePolygon);
	virtual void TranslateTransform(int dx, int dy, int nMatrixOrder);
	virtual void DiscardDeviceDependentResource();


public:
	virtual void Create(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID = 0);
	virtual void BeginDraw();
	virtual HRESULT EndDraw();

public:
	virtual void FillRectangle(int x, int y, int width, int height, CSTXGraphicsBrush *pBrush);
	virtual void DrawString(LPCTSTR lpszString, int x, int y, int width, int height, CSTXGraphicsFont *pFont, CSTXGraphicsBrush *pBrush, CSTXGraphicsTextFormat *pFormat = NULL);
	virtual void DrawImage(int x, int y, int width, int height, CSTXGraphicsImage *pImage, CSTXGraphicsImageFormat* pImageFormat = NULL);
	virtual void DrawRoundedRectangle(int x, int y, int width, int height, int d, CSTXGraphicsPen *pPen);
	virtual void DrawRichText(ITextServices *pTextService, int x, int y, int width, int height);
	virtual void FillPolygon(CSTXGraphicsPolygon *pPolygon, CSTXGraphicsBrush *pBrush);

};

class CSTXD2DDCGraphics : public CSTXD2DGraphics, public CSTXLayeredGraphics
{

protected:
	GdiBitmap _bitmap;
	SIZE _sizeDC;
	CComQIPtr<ID2D1DCRenderTarget> _dcRenderTarget;

public:
	virtual void Create(HWND hWnd, HDC hDC, int cx, int cy, UINT cacheUniqueID = 0);

public:
	virtual BOOL ApplyToLayeredWindow(HWND hWnd);

};

#endif