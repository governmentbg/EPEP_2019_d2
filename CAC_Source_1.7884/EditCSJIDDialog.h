//{{TDialog = TEditCSJIDDialog}}
class TEditCSJIDDialog : public TGroupDialog
{
public:
	TEditCSJIDDialog(TWindow* parent, TKnownPerson *person, int resId = IDD_EDIT_CSJID);

protected:
	TLongFace *csjid;

	DEFINE_GROUP_TYPE(TKnownPerson, Person)
	virtual bool IsValid();

//{{TEditCSJIDDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditCSJIDDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditCSJIDDialog);
};    //{{TEditCSJIDDialog}}
