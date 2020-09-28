//{{TDialog = TInRegEventListDialog}}
class TInRegEventListDialog : public TGroupListDialog
{
public:
	TInRegEventListDialog(TWindow* parent, TOutReg *tOutReg, TInReg *tInReg, TRCDKeyContainer *tEvent, sign tSave);

protected:
	TOutReg *outReg;
	TInReg *inReg;
	sign save;			// -1 == edit
	DWORD lastTicks;
	TRCDKeyContainer *event;
	TDecision decision;
	TLawsuit lawsuit;

	TStatic *header1;
	TStatic *header2;
	TInRegEventListWindow *inRegEvents;
	TButton *print;
	TCheckFace *publInet;	// 2017:339

	bool Appeal() const;
	void PrintOut();

//{{TInRegEventListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TInRegEventListDialogVIRTUAL_END}}

//{{TInRegEventListDialogRSP_TBL_BEGIN}}
	void AddBNEnable(TCommandEnabler &tce);
	void DeleteBNEnable(TCommandEnabler &tce);
	void PrintBNEnable(TCommandEnabler &tce);
	void PrintBNClicked();
	void CancelBNClicked();
	void EvClose();
//{{TInRegEventListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TInRegEventListDialog);
};    //{{TInRegEventListDialog}}
