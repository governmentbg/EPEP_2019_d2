//{{TDialog = TOriginSideListDialog}}
class TOriginSideListDialog : public TSideWindListDialog
{
public:
	TOriginSideListDialog(TWindow* parent, TRCDKeyContainer *container, TLawsuit *lawsuit, long sideMode, const char *what,
		int resId);

protected:
	bool statable;
	char OriginKind() const { return ((TOriginSideListWindow *) sideWinds)->OriginKind(); }

	virtual void Header2(mstr &m);

	virtual const char *LackingPrimary() const;
	virtual int PrimaryCount(char involvement) const;

//{{TOriginSideListDialogVIRTUAL_BEGIN}}
	virtual bool CanClose();
//{{TOriginSideListDialogVIRTUAL_END}}

//{{TOriginSideListDialogRSP_TBL_BEGIN}}
	void StateBNEnable(TCommandEnabler &tce);
	void StateBNClicked();
	void MoneyBNClicked();
//{{TOriginSideListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TOriginSideListDialog);
};    //{{TOriginSideListDialog}}
