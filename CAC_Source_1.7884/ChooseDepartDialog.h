//{{TDialog = TChooseDepartDialog}}
class TChooseDepartDialog : public TScaleDialog
{
public:
	TChooseDepartDialog(TWindow* parent, TSender *tSender, int resId = IDD_CHOOSE_DEPART);

protected:
	TSender *sender;
	TStringFace *ucnType;
	TLongFace *eispp;
	TStringFace *name;
	TEisDepartListBox *departs;
	TButton *ok;
	TButton *search;
	TButton *research;

	void SetDefault(int resourceId);

//{{TChooseDepartDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TChooseDepartDialogVIRTUAL_END}}

//{{TChooseDepartDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void DepartsLBNSelChange();
	void SearchBNClicked();
	void ResearchBNClicked();
	void NameENUpdate();
//{{TChooseDepartDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TChooseDepartDialog);
};    //{{TChooseDepartDialog}}
