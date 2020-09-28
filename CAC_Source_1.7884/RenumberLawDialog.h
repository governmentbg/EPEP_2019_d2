//{{TDialog = TRenumberLawDialog}}
class TRenumberLawDialog : public TRenumberDocDialog
{
public:
	TRenumberLawDialog(TWindow* parent, TLawsuit *lawsuit, int resId = IDD_RENUMBER_LAW);

protected:
	TDateFace *previousDate;
	TDateFace *receivedDate;
	DEFINE_GROUP_TYPE(TLawsuit, Lawsuit)
	virtual bool IsValid();

//{{TRenumberLawDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TRenumberLawDialogVIRTUAL_END}}

//{{TRenumberLawDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TRenumberLawDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TRenumberLawDialog);
};    //{{TRenumberLawDialog}}
