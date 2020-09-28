//{{TDialog = TKillLawDialog}}
class TKillLawDialog : public TGroupDialog
{
public:
	TKillLawDialog(TWindow* parent, TArchive *archive, TLawsuit *tLawsuit, bool tEdit);

protected:
	TLawsuit *lawsuit;	// 2017:240 argument
	bool edit;
	TDateFace *archiveDate;
	TDateFace *killDate;
	// 2005:350 Checks
	TLongFace *killerNo;
	TDateFace *killerDate;
	// 2005:356 Checks
	TLongFace *remainderNo;
	TYearFace *remainderYear;
	// 2017:009 show keep/index
	bool hasKeep;
	TLongFace *keep;
	TStringFace *archiveIndex;

	DEFINE_GROUP_TYPE(TArchive, Archive)
	virtual bool IsValid();

//{{TKillLawDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TKillLawDialogVIRTUAL_END}}

//{{TKillLawDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TKillLawDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TKillLawDialog);
};    //{{TKillLawDialog}}
