//{{TStatic = TColorStatic}}
class TColorStatic : public TStatic
{
public:
	TColorStatic(TWindow* parent, int resourceId, const char *tText, COLORREF tColor);

protected:
	constant text;
	const COLORREF color;

//{{TColorStaticVIRTUAL_BEGIN}}
	virtual bool ShowWindow(int cmdShow);
//{{TColorStaticVIRTUAL_END}}

//{{TColorStaticRSP_TBL_BEGIN}}
protected:
	void EvPaint();
//{{TColorStaticRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TColorStatic);
};    //{{TColorStatic}}
