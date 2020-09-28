//{{TDialog = TElectricItemListDialog}}
class TElectricItemListDialog : public TGroupListDialog
{
public:
	TElectricItemListDialog(TWindow* parent, TRCDKeyContainer *container, TGSArray<TElectric> *tItems, bool tAttach,
		const TLawsuit *lawsuit);

protected:
	TElectricItemListWindow *items;
	bool attach;
	TButton *add;
	TButton *edit;

//{{TElectricItemListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TElectricItemListDialogVIRTUAL_END}}

//{{TElectricItemListDialogRSP_TBL_BEGIN}}
	void ViewBNEnable(TCommandEnabler &tce);
	void ViewBNClicked();
	void AddBNEnable(TCommandEnabler &tce);
	void EditBNEnable(TCommandEnabler &tce);
	void EditBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
//{{TElectricItemListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TElectricItemListDialog);
};    //{{TElectricItemListDialog}}
