//{{TDialog = TSelectEventDialog}}
class TSelectEventDialog : public TSelectStringDialog
{
public:
	TSelectEventDialog(TWindow* parent, const char *label, TCollectEvents *te, int &choice, TGArray<TRCDKeyContainer> *tArray, int resId = IDD_SELECT_EVENT);

protected:
	TCollectEvents *e;
	TGArray<TRCDKeyContainer> *array;

	TButton *print;
	TButton *other;

	TRCDKeyContainer *Container() { return (*array)[list->GetSelIndex()]; }

//{{TSelectEventDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSelectEventDialogVIRTUAL_END}}

//{{TSelectEventDialogRSP_TBL_BEGIN}}
	void PrintBNClicked();
	void OtherBNClicked();
//{{TSelectEventDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectEventDialog);
};    //{{TSelectEventDialog}}
