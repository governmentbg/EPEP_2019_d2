//{{TDialog = TOutRegReceiveDialog}}
class TOutRegReceiveDialog : public TOutRegReceiveBaseDialog
{
public:
	TOutRegReceiveDialog(TWindow* parent, TOutReg *outReg, const char *tResults, const char *tComplects,
		const char *tServices, const char *tProcedures, const char *resultsVZ, const char *resultsKS, bool tKSRecv,
		TReceiver *tReceiverVZ, TReceiver *tReceiverKS, int resId = IDD_OUTREG_RECEIVE);

protected:
	TReceiveInstance vz;
	TCharAliasFace *resultI;
	TReceiveInstance ks;
	TCharAliasFace *resultKS;
	TCharAliasFace *resultII;
	TVSSIndexAliasFace *vssIndex;

	const char *resultsVZ;
	const char *resultsKS;
	bool ksRecv;
	const char *complects;
	const char *services;
	const char *procedures;
	TButton *load;		// 2009:286
	mstr resultsVZopt;
	TReceiver *receiverVZ;
	TReceiver *receiverKS;
	bool hasVZConn;
	bool hasKSConn;
	// 2015:161 for vss indexes
	char lawsuitKind;
	bool initialized;

	virtual bool IsValid();

	bool HaveResult(char resolution);
	void VZCourtChange(bool setDefaultKind);
	void VZResultChange();
	void KSCourtChange(bool setDefaultKind);
	void KSResultChange();

//{{TOutRegReceiveDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TOutRegReceiveDialogVIRTUAL_END}}

//{{TOutRegReceiveDialogRSP_TBL_BEGIN}}
	void ResultCBNSelChange();
	void OKBNClicked();
	void LoadBNClicked();
	void VZCourtCBNSelChange();
	void KSCourtCBNSelChange();
	void VZLawsuitKindCBNSelChange();
	void KSLawsuitKindCBNSelChange();
	void ReturnedENUpdate();
//{{TOutRegReceiveDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TOutRegReceiveDialog);
};    //{{TOutRegReceiveDialog}}
