//{{TListWindow = TResizeListWindow}}
class TResizeListWindow : public TListWindow
{
public:
	TResizeListWindow(TWindow* parent, int resourceId);

protected:
	int sizedAtWidth[10];
	int initialWidth;

//{{TResizeListWindowVIRTUAL_BEGIN}}
public:
	virtual void SetupWindow();
//{{TResizeListWindowVIRTUAL_END}}

//{{TResizeListWindowRSP_TBL_BEGIN}}
protected:
	void EvSize(uint sizeType, TSize& size);
//{{TResizeListWindowRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TResizeListWindow);
};    //{{TResizeListWindow}}
