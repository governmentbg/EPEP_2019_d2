//{{TDialog = TEditProveDialog}}
class TEditProveDialog : public TTroubleDialog
{
public:
	TEditProveDialog(TWindow* parent, TProve *prove, TRCDKeyContainer *tContainer, bool edit, int resId = IDD_EDIT_PROVE);

protected:
	TDateFace *date;			// 2002:029
	TRCDKeyContainer *container;	// 2006:034 For date checks
	TLongFace *no;			// 2006:044 For en/disable
	TStringFace *name;		// 2006:186

	DEFINE_GROUP_TYPE(TProve, Prove)
	virtual bool IsValid();

//{{TEditProveDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditProveDialogVIRTUAL_END}}
};    //{{TEditProveDialog}}
