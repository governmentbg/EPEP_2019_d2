//{{TButton = TColorButton}}
class TColorButton : public TButton
{
public:
	TColorButton(TWindow* parent, int resourceId, COLORREF tColor);

	void SetColor(COLORREF tColor) { color = tColor; Invalidate(); }

protected:
	COLORREF color;
	bool clicked;

	bool Styled() { return color != COLOR_BUTTON_TEXT; }

//{{TColorButtonRSP_TBL_BEGIN}}
	LRESULT EvDrawFace(WPARAM, LPARAM lParam);
	void EvSetFocus();
	void EvKillFocus();
//{{TColorButtonRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TColorButton);
};    //{{TColorButton}}
