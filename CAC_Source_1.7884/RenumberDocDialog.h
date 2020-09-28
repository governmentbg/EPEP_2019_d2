//{{TDialog = TRenumberDocDialog}}
class TRenumberDocDialog : public TGroupDialog
{
public:
	TRenumberDocDialog(TWindow* parent, TRCDKeyContainer *container, int resId = IDD_RENUMBER_DOC);

protected:
	TYearFace *year;		// 2008:009 LRQ: check vs. date
	TDateFace *date;		// 2008:009 LPR

	virtual bool IsValid();

//{{TRenumberDocDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TRenumberDocDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TRenumberDocDialog);
};    //{{TRenumberDocDialog}}
