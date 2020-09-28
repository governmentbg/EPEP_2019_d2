//{{TButton = TTwinButton}}
class TTwinButton : public TPushButton
{
public:
	TTwinButton(TScaleDialog *parent, int resourceId, int tSecondId, int iconId = IDI_RIGHTMOUSE);

	void SetCaption(const char *text);		// insert "[] " if needed

protected:
	int secondId;
	bool right;
	enum GRAB { GRAB_NONE = false, GRAB_KEY = true, GRAB_MOUSE = 2 } grab;

	bool Inside(TPoint &point) { return (unsigned) point.x < Attr.W && (unsigned) point.y < Attr.H; }
	void SetState(BOOL pushed);

//{{TTwinButtonVIRTUAL_BEGIN}}
	virtual void SetupWindow();			// SetCaption()
//{{TTwinButtonVIRTUAL_END}}

//{{TTwinButtonRSP_TBL_BEGIN}}
protected:
	void EvKeyDown(uint key, uint repeatCount, uint flags);
	void EvKeyUp(uint key, uint repeatCount, uint flags);
	void EvLButtonDown(uint modKeys, TPoint& point);
	void EvLButtonUp(uint modKeys, TPoint& point);
	void EvRButtonDown(uint modKeys, TPoint& point);
	void EvRButtonUp(uint modKeys, TPoint& point);
	void EvMouseMove(uint modKeys, TPoint& point);
//{{TTwinButtonRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TTwinButton);
};    //{{TTwinButton}}
