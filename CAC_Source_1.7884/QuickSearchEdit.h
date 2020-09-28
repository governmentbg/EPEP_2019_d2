// ----- TQuickSearchEdit ----------------------------------------------------
class TQuickSearchEdit : public TEdit
{
public:
	TQuickSearchEdit(TWindow *parent, int resourceId, bool tLeftButton);

	void PostClicked();

protected:
	bool leftButton;

	void EvLButtonDown(uint, TPoint&);
	void EvLButtonDblClk(uint, TPoint&);
	void EvRButtonDown(uint, TPoint&);
	void EvMouseMove(uint, TPoint &);
	bool EvSetCursor(THandle, uint, uint);

	DECLARE_RESPONSE_TABLE(TQuickSearchEdit);
};
