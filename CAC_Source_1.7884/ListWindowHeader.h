//{{TListWindowHeader = TWindow}}
class TListWindowHeader : public TWindow
{
public:
	TListWindowHeader(THandle handle);

//{{TListWindowHeaderRSP_TBL_BEGIN}}
protected:
	void EvLButtonDown(uint, TPoint &);
	void EvLButtonUp(uint, TPoint &);
	void EvLButtonDblClk(uint, TPoint &);
	void EvMouseMove(uint, TPoint &);
	bool EvSetCursor(THandle, uint, uint);
//{{TListWindowHeaderRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TListWindowHeader);
};    //{{TListWindowHeader}}
