class TJoinStreetsDialog : public TQuickDialog
{
public:
	TJoinStreetsDialog(TWindow *parent, TGSArray<TJoinStreet> *tSources, TGSArray<TJoinStreet> *tTargets, int resId = IDD_JOIN_STREET_TO_STREET);

private:
	TJoinStreetsListWindow *sources;
	TJoinStreetsListWindow *targets;

	void JoinBNEnable(TCommandEnabler &tce);
	void JoinBNClicked();
	bool SourceLVNItemChanging(TLwNotify &lwn);
	bool TargetLVNItemChanging(TLwNotify &lwn);
	DECLARE_RESPONSE_TABLE(TJoinStreetsDialog);
};   

