//{{TDialog = TEquateLoadDialog}}
class TEquateLoadDialog : public TGroupDialog
{
public:
	TEquateLoadDialog(TWindow* parent, TGArray<TUser> *tUsers, int resId = IDD_EQUATE_LOAD);

protected:
	TGArray<TUser> *users;
	TUserLoadListBox *load;

//{{TEquateLoadDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void EquateBNClicked();
//{{TEquateLoadDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEquateLoadDialog);
};    //{{TEquateLoadDialog}}
