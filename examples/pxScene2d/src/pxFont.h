// pxCore CopyRight 2007-2015 John Robinson
// pxFont.h

#ifndef PX_FONT_H
#define PX_FONT_H

// TODO it would be nice to push this back into implemention
#include <ft2build.h>
#include FT_FREETYPE_H

#include "rtString.h"
#include "rtRefT.h"
#include "pxScene2d.h"
#include <map>

class pxText;
class pxFont;

#define defaultPixelSize 16
#define defaultFace "FreeSans.ttf"

class pxFace;
typedef rtRefT<pxFace> pxFaceRef;

class pxFileDownloadRequest;
typedef struct _FontDownloadRequest
{
  pxFileDownloadRequest* fileDownloadRequest;
} FontDownloadRequest;

struct GlyphCacheEntry
{
  int bitmap_left;
  int bitmap_top;
  int bitmapdotwidth;
  int bitmapdotrows;
  int advancedotx;
  int advancedoty;
  int vertAdvance;

  pxTextureRef mTexture;
};

class pxFace
{
public:
  pxFace();
  virtual ~pxFace();
  
  rtError init(const char* n);
  rtError init(const FT_Byte*  fontData, FT_Long size, const char* n);
  
  void setFaceName(const char* n);
  bool isInitialized() { return mInitialized; }
  void onDownloadComplete(const FT_Byte* fontData, FT_Long size, const char* n);
  void addListener(pxFont* pFont);

  virtual unsigned long AddRef() 
  {
    return rtAtomicInc(&mRefCount);
  }
  
  virtual unsigned long Release() 
  {
    long l = rtAtomicDec(&mRefCount);
    if (l == 0) delete this;
    return l;
  }
    
  void setPixelSize(uint32_t s);  
  const GlyphCacheEntry* getGlyph(uint32_t codePoint);  
  void getMetrics(uint32_t size, float& height, float& ascender, float& descender, float& naturalLeading);
  void getHeight(uint32_t size, float& height);
  void measureText(const char* text, uint32_t size,  float sx, float sy, 
                   float& w, float& h);
  void measureTextChar(u_int32_t codePoint, uint32_t size,  float sx, float sy, 
                         float& w, float& h);                   
  void renderText(const char *text, uint32_t size, float x, float y, 
                  float sx, float sy, 
                  float* color, float mw);

private:
  uint32_t mFaceId;
  rtString mFaceName;
  FT_Face mFace;
  uint32_t mPixelSize;
  rtAtomic mRefCount;
  
  char* mFontData; // for remote fonts loaded into memory
  
  bool mInitialized;
  vector<pxFont*> mListeners;
};



/**********************************************************************
 * 
 * pxTextMetrics
 * 
 **********************************************************************/
class pxTextMetrics: public pxObject {

public:
	pxTextMetrics(pxScene2d* s): pxObject(s), mRefCount(0) {  }
	virtual ~pxTextMetrics() {}

	virtual unsigned long AddRef() 
	{
		return rtAtomicInc(&mRefCount);
	}

	virtual unsigned long Release() 
	{
		long l = rtAtomicDec(&mRefCount);
		if (l == 0) delete this;
			return l;
	}
	rtDeclareObject(pxTextMetrics, pxObject);
	rtReadOnlyProperty(height, height, float); 
	rtReadOnlyProperty(ascent, ascent, float);
	rtReadOnlyProperty(descent, descent, float);
  rtReadOnlyProperty(naturalLeading, naturalLeading, float);
  rtReadOnlyProperty(baseline, baseline, float);
 
	float height()             const { return mHeight; }
	rtError height(float& v)   const { v = mHeight; return RT_OK;   }
	rtError setHeight(float v)       { mHeight = v; return RT_OK;   }

	float ascent()             const { return mAscent; }
	rtError ascent(float& v)   const { v = mAscent; return RT_OK;   }
	rtError setAscent(float v)       { mAscent = v; return RT_OK;   } 

	float descent()             const { return mDescent; }
	rtError descent(float& v)   const { v = mDescent; return RT_OK;   }
	rtError setDescent(float v)       { mDescent = v; return RT_OK;   } 
 
 	float naturalLeading()             const { return mNaturalLeading; }
	rtError naturalLeading(float& v)   const { v = mNaturalLeading; return RT_OK;   }
	rtError setNaturalLeading(float v)       { mNaturalLeading = v; return RT_OK;   } 
  
 	float baseline()             const { return mBaseline; }
	rtError baseline(float& v)   const { v = mBaseline; return RT_OK;   }
	rtError setBaseline(float v)       { mBaseline = v; return RT_OK;   }   
   
  private:
    rtAtomic mRefCount;	
   	float mHeight;
    float mAscent;
    float mDescent;
    float mNaturalLeading;
    float mBaseline;
};

/**********************************************************************
 * 
 * pxTextMeasurements
 * 
 **********************************************************************/
class pxTextSimpleMeasurements: public pxObject {

public:
	pxTextSimpleMeasurements(pxScene2d* s): pxObject(s), mRefCount(0) { 

  }
	virtual ~pxTextSimpleMeasurements() {}

	virtual unsigned long AddRef() 
	{
		return rtAtomicInc(&mRefCount);
	}

	virtual unsigned long Release() 
	{
		long l = rtAtomicDec(&mRefCount);
		if (l == 0) delete this;
			return l;
	}
	rtDeclareObject(pxTextSimpleMeasurements, pxObject);

  private:
    rtAtomic mRefCount;	
    
};

/**********************************************************************
 * 
 * pxFont
 * 
 **********************************************************************/
class pxFont: public pxObject {

public:
	pxFont(pxScene2d* s, rtString faceURL);
	virtual ~pxFont() ;

	virtual unsigned long AddRef() 
	{
		return rtAtomicInc(&mRefCount);
	}

	virtual unsigned long Release() 
	{
		long l = rtAtomicDec(&mRefCount);
		if (l == 0) delete this;
			return l;
	}
	rtDeclareObject(pxFont, pxObject);
  rtMethod1ArgAndReturn("getFontMetrics", getFontMetrics, uint32_t, rtObjectRef);
  rtError getFontMetrics(uint32_t pixelSize, rtObjectRef& o);
  rtMethod2ArgAndReturn("measureText", measureText, uint32_t, rtString, rtObjectRef);
  rtError measureText(uint32_t, rtString, rtObjectRef& o);   
  // Should we have a getter so that users can query the fontFace?
  void fontLoaded();
  void onFontDownloadComplete(FontDownloadRequest fontDownloadRequest);
  static void checkForCompletedDownloads(int maxTimeInMilliseconds=10);
  pxFaceRef getFace() { return mFace;}
  void addListener(pxText* pText);
  bool isFontLoaded() { return mInitialized;}

  private:
 
    void sendReady(const char * value); 
    rtError loadFont();
    
    rtAtomic mRefCount;	

    pxFaceRef mFace;
    //uint32_t mFaceId;
    rtString mFaceName;
    
    pxFileDownloadRequest* mFontDownloadRequest;
    
    vector<pxText*> mListeners;
};

// Weak Map
typedef map<rtString, pxFont*> FontMap;

class pxFontManager
{
  
  public: 
  
    static rtObjectRef getFont(pxScene2d* scene, const char* s);
    static pxFont* getFontObj(pxScene2d* scene, const char* s);
    
  protected: 
    static void initFT(pxScene2d* scene);  
    static FontMap mFontMap;
    static bool init;
    
};
#endif

