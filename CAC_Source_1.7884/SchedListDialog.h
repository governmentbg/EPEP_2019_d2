//{{TDialog = TSchedListDialog}}
class TSchedListDialog : public TGroupListDialog
{
public:
	TSchedListDialog(TWindow* parent, TSched *tSched, const char *tScheds, int resId = IDD_SCHED_LIST);

protected:
	TSched *sched;
	TStatic *header1;
	TStatic *header2;
	TSchedListWindow *scheds;

//{{TSchedListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSchedListDialogVIRTUAL_END}}

//{{TSchedListDialogRSP_TBL_BEGIN}}
	void AddBNEnable(TCommandEnabler &tce);
//{{TSchedListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSchedListDialog);
};    //{{TSchedListDialog}}
