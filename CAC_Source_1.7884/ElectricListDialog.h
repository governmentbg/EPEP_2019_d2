class TElectricListDialog : public TGroupListDialog
{
public:
	TElectricListDialog(TWindow* parent, TRCDKeyContainer *container, TPoint *tWhere);
	virtual ~TElectricListDialog();

	// 2010:166 LPR: multi-dialog signals
	static void Signal(int sigtype, const TRCDKeyContainer *container, const void *data);
	static int ForEach(int command, void *argument);
	void SetupGlobalText();

protected:
	TRCDKeyContainer *primary;
	TControl *global;
	DWORD lastTicks;
	TElectricListWindow *electrics;
	TButton *save;
	TButton *local;
	TButton *eispp;
	TStatic *label;	// 2012:347 LPR/LRQ: +related
	TPoint *where;

	void ElectricButton(void (TElectricListWindow::*ElectricAction)());
	void ElectraButton(TControl *button, TRCDKeyContainer *container, const TElectra *electras);

	// 2010:166 LPR: multi-dialog
	static TIArray<TElectricListDialog> Dialogs;
	static int lastWin;
	// 2017:082 LPR: exception handling
	void SignalElectric(int sigtype, const TRCDKeyContainer *container, const void *data);

//{{TElectricListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
	virtual void CleanupWindow();
	virtual TResult EvNotify(uint id, TNotify far& info);
//{{TElectricListDialogVIRTUAL_END}}

//{{TElectricListDialogRSP_TBL_BEGIN}}
	void ListBNClicked();
	void BlueBNEnable(TCommandEnabler &tce);
	void BlueBNClicked();
	void SaveBNEnable(TCommandEnabler &tce);
	void SaveBNClicked();
	void QueryBNClicked();
	void LocalBNClicked();
	void GlobalBNClicked();
	void EisppBNEnable(TCommandEnabler &tce);
	void EisppBNClicked();
DECLARE_RESPONSE_TABLE(TElectricListDialog);
//{{TElectricListDialogRSP_TBL_END}}
};    //{{TElectricListDialog}}
