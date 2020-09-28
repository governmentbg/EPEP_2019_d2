//{{TDialog = TResizeDialog}}
class TResizeDialog : public TResourceDialog
{
public:
	TResizeDialog(TWindow* parent, int tShowMax, int resId);

protected:
	int showMax;			// SHOW_MAXIMIZED/ADJUSTED/NORMAL
	int minWidth, minHeight;
	int lastWidth, lastHeight;
	int dx, dy;				// only valid in Resize()

	TWindow *autoDown[DOWN_CONTROLS_MAX];
	TWindow *autoSize;

	void Resize(TWindow *window, int resize);
	virtual void Resize();

//{{TResizeDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TResizeDialogVIRTUAL_END}}

//{{TResizeDialogRSP_TBL_BEGIN}}
	void EvSize(uint sizeType, TSize& size);
	bool EvSizing(uint side, TRect& rect);
//{{TResizeDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TResizeDialog);
};    //{{TResizeDialog}}
