//{{TDialog = TEditLawerDataDialog}}
class TEditLawerDataDialog : public TGroupDialog
{
public:
	TEditLawerDataDialog(TWindow* parent, TKnownPerson *person, bool tEPortal);

protected:
	bool eportal;
	bool realUCN;
	TUCNFace *personalNo;
	TDateFace *birthDate;

	DEFINE_GROUP_TYPE(TKnownPerson, Person)
	virtual bool IsValid();

//{{TEditLawerDataDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditLawerDataDialogVIRTUAL_END}}

//{{TEditLawerDataDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditLawerDataDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditLawerDataDialog);
};    //{{TEditLawerDataDialog}}
