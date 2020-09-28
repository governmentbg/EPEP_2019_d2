class TLaunchJoinStreetsDialog : public TQuickDialog
{
public:
	TLaunchJoinStreetsDialog(TWindow *parent, int resId = IDD_JOIN_SET_OF_STREETS);

protected:
	TAreaAliasFace *area;
	TRegionAliasFace *region;
	TFindUCLPAliasFace *uclp;

	virtual bool IsValid();
	virtual void SetupWindow();

	void OKBNClicked();
	void QueryBNClicked();
	void AreaCBNSelChange();
	void RegionCBNSelChange();
	DECLARE_RESPONSE_TABLE(TLaunchJoinStreetsDialog);
};
