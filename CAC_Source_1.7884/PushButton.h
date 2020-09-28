//{{TButton = TPushButton}}
class TScaleDialog;

class TPushButton : public TButton
{
public:
	TPushButton(TScaleDialog *parent, int resourceId, int tIconId = -1);
	virtual ~TPushButton();

	void SetLines(int tLines);	// 1..10
	void SetDrawIcon(bool tDrawIcon) { drawIcon = tDrawIcon; }

protected:
	int iconId;
	int lines;
	TIcon *icon;
	TIcon *icon1;
	bool drawIcon;		// 2015:113

//{{TPushButtonVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TPushButtonVIRTUAL_END}}

//{{TPushButtonRSP_TBL_BEGIN}}
	LRESULT EvDrawFace(WPARAM, LPARAM lParam);
	void EvSetFocus();
	void EvKillFocus();
//{{TPushButtonRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPushButton);
};    //{{TPushButton}}
