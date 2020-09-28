//{{TDialog = TWaitThreadDialog}}
class TWaitThreadDialog : public TScaleDialog
{
public:
	TWaitThreadDialog(int resId = IDD_WAIT_THREAD);
	virtual ~TWaitThreadDialog();

	void StartStep();
	void SetHeader(const char *tHeader);
	void SetProgressRange(int min, int max);
	void SetProgress(int value);
	void SetText(const char *tText);
	void PleaseWait();
	void AlmostDone();
	void CheckBreak();
	bool WantBreak() { return wantBreak; }
	void GetText(char *s, int maxChars);
	void PumpWaitingMessages();

protected:
	TStatic *header;
	TGauge *progress;
	TStatic *text;
	TButton *cancel;

	bool initialized;
	bool wantBreak;
	DWORD lastText;

	// from _waitwnd
	enum { dfltMaxPBVal = 100 };
	int minPBVal;
	int maxPBVal;

//{{TWaitThreadDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TWaitThreadDialogVIRTUAL_END}}

//{{TWaitThreadDialogRSP_TBL_BEGIN}}
	void CancelBNClicked();
	void EvClose();
//{{TWaitThreadDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TWaitThreadDialog);
};    //{{TWaitThreadDialog}}

// ----- TWaitThread ---------------------------------------------------------
class TWaitThread : protected TThread
{
public:
	TWaitThread();

	void ExecWait();
	virtual void Tick(const bool completed);
#if RANGECHECK
	static void SetWTDL(TWaitThreadDialog *tWTDL);
#else  // RANGECHECK
	static void SetWTDL(TWaitThreadDialog *tWTDL) { wtdl = tWTDL; }
#endif  // RANGECHECK

protected:
	static TWaitThreadDialog *wtdl;

	virtual void Execute() = 0;
	virtual int Run();
};

// ----- TCheckupThread ------------------------------------------------------
class TCheckupThread : public TWaitThread
{
public:
	virtual void Tick(const bool completed);
	virtual bool Generate();

	enum CheckupActions
	{
		GatheringData = 0,
		MakingGeneralRow,
		CreatingOutDoc
	};

protected:
	TCheckupThread(int generalRowTicks = 0, int createOutDocTicks = 0);

	virtual bool ArgsOk();
	virtual void MakeGeneralRow();
	virtual void CreateOutDoc();

	const int barLowVal;
	const int barHiVal;
	const int generalRowSteps;
	const int createOutDocSteps;

	int currentTickRate;
	int lastShownVal;
	int startingVal;
	int endingVal;
	int totalPartSteps;
	int currentPartStep;
	const char *passeds;
	CTime lastGossipTime;
	bool reportFinishedItems;
	bool generateRes;
};
