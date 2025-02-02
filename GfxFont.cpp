#include "GfxFont.h"
#include <atlbase.h>
#include <stdio.h>


__inline float _floor(float f)
{
	static int _n;
	_asm fld f
	_asm fistp _n
	return (float)_n;
}

// 65级灰度表
const unsigned char g_byAlphaLevel[65] = 
{
	  0,  4,  8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48,
	 52, 56, 60, 64, 68, 72, 76, 80, 84, 88, 92, 96,100,
	104,108,112,116,120,124,128,132,136,140,144,148,152,
	156,160,164,168,172,176,180,184,188,192,196,200,204,
	208,212,216,220,224,228,232,236,240,244,248,252,255
};

GfxFont::GfxFont(const char* lpsFontName, int nFaceSize, BOOL bBold, BOOL bItalic, BOOL bAntialias)
{
	m_pHGE = hgeCreate(HGE_VERSION);

	// 创建GDI相关设备
	HDC hDC = GetDC(m_pHGE->System_GetState(HGE_HWND));
	m_hMemDC = CreateCompatibleDC(hDC);
	if (NULL == m_hMemDC) return;
	ReleaseDC(m_pHGE->System_GetState(HGE_HWND), hDC);

	::SetMapMode(m_hMemDC, MM_TEXT);
	::SetTextColor(m_hMemDC, RGB(255, 255, 255));
	::SetBkColor(m_hMemDC, RGB(0, 0, 0));

	m_hFont = CreateFont(
		-nFaceSize,
		0,
		0,
		0,
		(bBold) ? FW_BOLD : FW_NORMAL,
		bItalic,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		FF_DONTCARE | DEFAULT_PITCH,
		lpsFontName);
	if (NULL == (m_hFont)) return;
	SelectObject(m_hMemDC, m_hFont);

	memset(m_Glyphs, 0, sizeof(TENGINEFONTGLYPH)*font_count);

	m_nAntialias = bAntialias ? GGO_GRAY8_BITMAP : GGO_BITMAP;

	TEXTMETRIC tm;
	::GetTextMetrics(m_hMemDC, &tm);
	m_nAscent = tm.tmAscent;

	m_nFontSize = static_cast<float>(nFaceSize);
	m_nKerningWidth = 0;
	m_nKerningHeight = 0;

	m_pSprite = new hgeSprite(0, 0, 0, 0, 0);
	m_pSprite->SetColor(ARGB(255, 255, 255, 255));

}

GfxFont::~GfxFont(void)
{
	for (int nIdx = 0; nIdx < font_count; ++nIdx)
	{ if (m_Glyphs[nIdx].t) m_pHGE->Texture_Free(m_Glyphs[nIdx].t); }

	if ((m_hFont)) DeleteObject(m_hFont);
	if ((m_hMemDC)) DeleteDC(m_hMemDC);

	if(m_pSprite) delete m_pSprite;
	if(m_pHGE) m_pHGE->Release();
}

// 渲染文本
void GfxFont::Print( float x, float y, float size,const char *format, ... )
{
	char sBuffer[10240] = {0};
	char *lpsArg=(char*)&format+sizeof(format);
	vsprintf(sBuffer, format, lpsArg);

	Render(x,y,CA2W(sBuffer),size);
}

void GfxFont::Render(float x, float y, const wchar_t* text,float size,bool shape)
{

	float offsetX = x;
	float offsetY = y;

	while(*text)
	{
		if (*text == L'\n' || *text == L'\r')
		{
			offsetX = x;
			offsetY += (m_nFontSize + m_nKerningHeight);
		}
		else
		{
			unsigned int idx = GetGlyphByCharacter(*text);
			if (idx > 0)
			{
				//包含描边
				DWORD color = m_pSprite->GetColor();
				float alpha = GETA(color);
				float r = GETR(color);
				float g = GETG(color);
				float b = GETB(color);
				m_pSprite->SetTexture(m_Glyphs[idx].t);
				m_pSprite->SetTextureRect(0, 0, m_Glyphs[idx].w, m_Glyphs[idx].h);
				m_pSprite->SetHotSpot(m_pSprite->GetWidth()/2,m_pSprite->GetHeight()/2);
				//255-((255-r)*((255-alpha)/255))=alpha+r-(alpha*r/255)????!!!!
				m_pSprite->SetColor(ARGB(alpha, alpha + r - (alpha*r / 255), alpha + g - (alpha*g / 255), alpha + b - (alpha*b / 255)));
				if (shape)
				{
					m_pSprite->RenderEx(offsetX - m_Glyphs[idx].x + 1, offsetY - m_Glyphs[idx].y, 0, size, size);
					m_pSprite->RenderEx(offsetX - m_Glyphs[idx].x - 1, offsetY - m_Glyphs[idx].y, 0, size, size);
					m_pSprite->RenderEx(offsetX - m_Glyphs[idx].x, offsetY - m_Glyphs[idx].y + 1, 0, size, size);
					m_pSprite->RenderEx(offsetX - m_Glyphs[idx].x, offsetY - m_Glyphs[idx].y - 1, 0, size, size);
				}
				m_pSprite->SetColor(color);
				m_pSprite->RenderEx(offsetX - m_Glyphs[idx].x, offsetY - m_Glyphs[idx].y, 0, size, size);
				offsetX += (GetWidthFromCharacter(*text) + m_nKerningWidth);
			}
			else
			{
				offsetX += (GetWidthFromCharacter(*text) + m_nKerningWidth);
			}
		}

		++text;
	}
}

// 设置与获取颜色
void GfxFont::SetColor( DWORD dwColor, int i )
{
	m_pSprite->SetColor(dwColor,i);
}

DWORD GfxFont::GetColor(int i)
{
	return m_pSprite->GetColor(i);
}

// 获取文本宽高
SIZE GfxFont::GetTextSize( const wchar_t* text )
{
	SIZE dim = {0, static_cast<LONG>(m_nFontSize)};
	float nRowWidth = 0;

	while(*text)
	{
		if (*text == L'\n' || *text == L'\r')
		{
			dim.cy += static_cast<LONG>(m_nFontSize + m_nKerningHeight);
			if (dim.cx < static_cast<LONG>(nRowWidth))
				dim.cx = static_cast<LONG>(nRowWidth);
			nRowWidth = 0;
		}
		else
			nRowWidth += (GetWidthFromCharacter(*text) + m_nKerningWidth);
		++text;
	}

	if (dim.cx < static_cast<LONG>(nRowWidth))
		dim.cx = static_cast<LONG>(nRowWidth);

	return dim;
}

// 根据坐标获取字符
wchar_t GfxFont::GetCharacterFromPos( const wchar_t* text, float pixel_x, float pixel_y )
{
	float x = 0;
	float y = 0;

	while (*text)
	{
		if (*text == L'\n' || *text == L'\r')
		{
			x = 0;
			y += (m_nFontSize+m_nKerningHeight);
			text++;

			if (!(*text))
				break;
		}

		float w = GetWidthFromCharacter(*text);
		if (pixel_x > x && pixel_x <= x + w &&
			pixel_y > y && pixel_y <= y + m_nFontSize)
			return *text;

		x += (w+m_nKerningWidth);

		text++;
	}

	return L'\0';
}

// 设置字间距
void GfxFont::SetKerningWidth( float kerning )
{
	m_nKerningWidth = kerning;
}
void GfxFont::SetKerningHeight( float kerning )
{
	m_nKerningHeight = kerning;
}

// 获取字间距
float GfxFont::GetKerningWidth()
{
	return m_nKerningWidth;
}
float GfxFont::GetKerningHeight()
{
	return m_nKerningHeight;
}	

// 字体大小
float GfxFont::GetFontSize()
{
	return m_nFontSize;
}

// 根据字符获取轮廓
unsigned int GfxFont::GetGlyphByCharacter( wchar_t c )
{
	unsigned int idx = (unsigned int)c;
	if (NULL == (m_Glyphs[idx].t)) CacheCharacter(idx,c);
	return idx;
}
inline float GfxFont::GetWidthFromCharacter( wchar_t c, bool original )
{
	unsigned int idx = GetGlyphByCharacter(c);
	if (original && idx > 0 && idx < font_count) return m_Glyphs[idx].c;
	return	(idx >= 0x2000) ? m_nFontSize : _floor(m_nFontSize / 2);
}
inline void GfxFont::CacheCharacter(unsigned int idx, wchar_t c)
{
	if (idx < font_count && NULL == m_Glyphs[idx].t)
	{
		UINT nChar = (UINT)c;

		MAT2 mat2 = {{0,1},{0,0},{0,0},{0,1}};
		GLYPHMETRICS gm;
		DWORD nLen = ::GetGlyphOutlineW(m_hMemDC,nChar,m_nAntialias,&gm,0,NULL,&mat2);

		HTEXTURE hTex = m_pHGE->Texture_Create(gm.gmBlackBoxX,gm.gmBlackBoxY);
		if (NULL == hTex) return;

		if((signed)nLen > 0)
		{
			LPBYTE lpBuf = new BYTE[nLen];

			if (nLen == ::GetGlyphOutlineW(m_hMemDC,nChar,m_nAntialias,&gm,nLen,lpBuf,&mat2))
			{
				BYTE*	lpSrc = lpBuf;
				DWORD*	lpDst = m_pHGE->Texture_Lock(hTex,FALSE);

				if (GGO_BITMAP == m_nAntialias)
				{
					LONG nSrcPitch = (gm.gmBlackBoxX / 32 + (gm.gmBlackBoxX % 32 == 0 ? 0 : 1)) * 4;
					LONG nDstPitch = m_pHGE->Texture_GetWidth(hTex);

					for (UINT y = 0; y < gm.gmBlackBoxY; ++y)
					{
						for (UINT x = 0; x < gm.gmBlackBoxX; ++x)
						{
							for(UINT k = 0; k < 8; ++k)   
							{
								UINT i = 8 * x + k;
								if (i >= gm.gmBlackBoxX)
								{
									x+=7;
									break;
								}
								lpDst[i] = ((lpSrc[x] >> (7 - k)) & 1) ? 0xFFFFFFFF : 0x0;
							}
						}

						lpSrc += nSrcPitch;
						lpDst += nDstPitch;
					}
				}
				else
				{
					LONG nSrcPitch = (gm.gmBlackBoxX / 4 + (gm.gmBlackBoxX % 4 == 0 ? 0 : 1)) * 4;
					LONG nDstPitch = m_pHGE->Texture_GetWidth(hTex);

					for (UINT y = 0; y < gm.gmBlackBoxY; ++y)
					{
						for (UINT x = 0; x < gm.gmBlackBoxX; ++x)
						{
							lpDst[x] = ARGB(g_byAlphaLevel[lpSrc[x]],0xFF,0xFF,0xFF);
						}

						lpSrc += nSrcPitch;
						lpDst += nDstPitch;
					}
				}

				m_pHGE->Texture_Unlock(hTex);
			}

			delete lpBuf;
		}
		else
		{
			// 非正常显示字符
		}

		m_Glyphs[idx].t = hTex;
		m_Glyphs[idx].w = static_cast<float>(gm.gmBlackBoxX);
		m_Glyphs[idx].h = static_cast<float>(gm.gmBlackBoxY);
		m_Glyphs[idx].x = static_cast<float>(-gm.gmptGlyphOrigin.x);
		m_Glyphs[idx].y = static_cast<float>(-m_nAscent + gm.gmptGlyphOrigin.y);
		m_Glyphs[idx].c = static_cast<float>(gm.gmCellIncX);
	}
}


