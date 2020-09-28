//{{TDialog = TSideWindListDialog}}
class TSideWindListDialog : public TGroupListDialog
{
public:
	typedef TSideWindListWindow *(*TSideWindListCreator)(TWindow*, long, TRCDKeyContainer *, TLawsuit *);

	TSideWindListDialog(TWindow* parent, TRCDKeyContainer *tContainer, TLawsuit *lawsuit, long tSideMode,
		const char *tWhat, int resId = IDD_SIDE_WIND_LIST, TSideWindListCreator createSideWinds = NULL);

protected:
	TRCDKeyContainer *container;	// 2010:074
	const bool edit;
	const char *what;
	const char *primaries;

private:
	const long sideMode;		// 2010:287 LPR: redesign; 2011:293 private

protected:
	TSideWindListWindow *sideWinds;
	TStatic *sideLabel;
	TStatic *header1;			// 2005:074 type|kind no year [date [time]]
	TStatic *header2;			// 2005:074 doc-specific, empty by default
	// 2016:349 LPR: for the local child menu
	DWORD lastTicks;
	TButton *child;

	// 2005:074; 2011:276 LPR: removed Title(m), never used
	virtual void Header1(mstr &m);
	virtual void Header2(mstr &);

	virtual const char *LackingPrimary() const;
	bool PrimaryEnable();
	// 2017:209 moved to window; 2017:244 moved back for derived dialogs
	virtual bool Primary(TSideWind *sideWind) const;
	virtual int PrimaryCount(char) const;

	// 2011:251
	virtual void Accept();
	virtual void Reject();

//{{TSideWindListDialogVIRTUAL_BEGIN}}
	virtual bool CanClose();
	virtual void SetupWindow();
//{{TSideWindListDialogVIRTUAL_END}}

//{{TSideWindListDialogRSP_TBL_BEGIN}}
	void DeleteBNEnable(TCommandEnabler &tce);
	void LinksBNEnable(TCommandEnabler &tce);
	void LinksBNClicked();
	void CancelBNClicked();
	void ChildBNEnable(TCommandEnabler &tce);
	void ChildBNClicked();
	void EvClose();
//{{TSideWindListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSideWindListDialog);
};    //{{TSideWindListDialog}}
