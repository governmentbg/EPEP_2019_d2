//{{TListWindow = TTestListWindow}}
class TTestListWindow : public TListWindow
{
public:
	TTestListWindow(TWindow* parent, int resourceId, TListWEmpty *tEmptyRect);

protected:
	TListWEmpty *emptyRect;

//{{TTestListWindowVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TTestListWindowVIRTUAL_END}}
};    //{{TNTListWindow}}
