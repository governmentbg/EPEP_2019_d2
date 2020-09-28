//{{TDialog = TClosedResultDialog}}
class TClosedResultDialog : public TSessionResultDialog
{
public:
	TClosedResultDialog(TWindow* parent, TSession *session, TLawsuit *lawsuit, bool edit, bool tLockDate = false);

protected:
	TStatic *header;		// 2009:148
	TDateFace *date;
	TIndexFace *index;
	TLongFace *eventNo;	// 2005:089
	TButton *print;
	// 2004:362 LPR: edit only, for reindexing
	const TBackupIndex backupIndex;	// 2018-04-19: was index{No,Year,Type,Kind}
	TCheckFace *firset;	// 2009:202
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	TButton *launch;		// 2009:260 moved here from session result +related
#endif  // APPEAL || AREAL || REGIONAL
	CDate forceDate;
	long oldFirstSet;
	// 2017:037 +related; 2017:040 for all courts
	TCheckFace *restart;
	bool oldRestart;
	bool lockDate;

	DEFINE_GROUP_TYPE(TSession, Session)
	virtual bool IsValid();

	void UpdateHours();

	void SetFinished();
	void CommitInterval();

//{{TClosedResultDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TClosedResultDialogVIRTUAL_END}}

//{{TClosedResultDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void PrintBNClicked();
	void ReturnedKillFocus();
	void LaunchBNClicked();
	void ResultCBNSelChange();
	void RightBNClicked();
	void SurroundsBNClicked();
	void ChanjedBNClicked();
	void RestartBNClicked();
//{{TClosedResultDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TClosedResultDialog);
};    //{{TClosedResultDialog}}
