//{{TDialog = TCadrListDialog}}
class TCadrListDialog : public TGroupListDialog
{
public:
	TCadrListDialog(TWindow* parent, int tStartTab, int resId = IDD_CADR_LIST);

protected:
	int startTab;

	long newArea;
	long newRegion;
	long newUCLP;

	TTabControl *pages;
	TStatic *range;
	TStatic *label;
	TEmptyListWindow *empty;
	TAreaListWindow *areas;
	TRegionListWindow *regions;
	TUCLPListWindow *uclps;
	TStreetListWindow *streets;
	TCadrListWindow *lists[TAB_COUNT];

	void Activate(int tab);

//{{TCadrListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TCadrListDialogVIRTUAL_END}}

//{{TCadrListDialogRSP_TBL_BEGIN}}
	bool AreasLVNItemChanging(TLwNotify &lwn);
	bool RegionsLVNItemChanging(TLwNotify &lwn);
	bool UCLPSLVNItemChanging(TLwNotify &lwn);
	bool StreetsLVNItemChanging(TLwNotify &lwn);
	void PagesTCNSelChange(TNotify &);
	void AreasLVNItemChanged(TLwNotify &lwn);
	void RegionsLVNItemChanged(TLwNotify &lwn);
	void UCLPSLVNItemChanged(TLwNotify &lwn);
	void AddBNEnable(TCommandEnabler &tce);
	void PrevBNClicked();
	void NextBNClicked();
//{{TCadrListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TCadrListDialog);
};    //{{TCadrListDialog}}
