//{{TDialog = TEditAccountDialog}}
class TEditAccountDialog : public TEditDialog
{
public:
	TEditAccountDialog(TWindow* parent, TAccount *account, bool edit);

	TAccount *Account() { return (TAccount *) Group(); }

protected:
	TSenderNameFace *name;
	TCharAliasFace *accountType;
	TCheckFace *judicial;

//{{TEditAccountDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditAccountDialogVIRTUAL_END}}
};    //{{TEditAccountDialog}}
