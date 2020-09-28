//{{TDialog = TLegalAidDialog}}
class TLegalAidDialog : public TGroupDialog
{
public:
	TLegalAidDialog(TWindow* parent, TPrintSubpoena *printsub, int resId = IDD_LEGAL_AID);

protected:
	TAliasListBox *legalAidType;
	TCheckFace *specialRepresentative;
	TCheckFace *reserveDefender;
	TCheckFace *onDuty;
	TCheckFace *conflictingInterests;
	TLegalAidSideListWindow *laids;
	TLongFace *lawerCount;

	DEFINE_GROUP_TYPE(TPrintSubpoena, PrintSub)
	virtual bool IsValid();

//{{TLegalAidDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TLegalAidDialogVIRTUAL_END}}

//{{TLegalAidDialogRSP_TBL_BEGIN}}
	//void OKBNEnable(TCommandEnabler &tce);
	void OKBNClicked();
	void AddressBNEnable(TCommandEnabler &tce);
	void AddressBNClicked();
	void LawerBNEnable(TCommandEnabler &tce);
	void LawerBNClicked();
	void RightBNClicked();
	bool ListLVNItemChanging(TLwNotify &lwn);
//{{TLegalAidDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TLegalAidDialog);
};    //{{TLegalAidDialog}}
