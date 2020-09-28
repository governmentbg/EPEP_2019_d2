//{{TDialog = TSelectContainerDialog}}
class TSelectContainerDialog : public TSelectEventDialog
{
public:
	TSelectContainerDialog(TWindow* parent, const char *label, TCollectEvents *e, int &choice,
		TGArray<TRCDKeyContainer> *basics, TGArray<TBaseWind> *tSideWinds, int resId = IDD_SELECT_CONTAINER);

protected:
	TGArray<TBaseWind> *sideWinds;
	TButton *query;

	virtual bool IsValid();

//{{TSelectContainerDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSelectContainerDialogVIRTUAL_END}}

//{{TSelectContainerDialogRSP_TBL_BEGIN}}
	void PrintBNClicked();
	void QueryBNClicked();
//{{TSelectContainerDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectContainerDialog);
};    //{{TSelectContainerDialog}}
