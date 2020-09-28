//{{TDialog = TEditCompanyDialog}}
class TEditCompanyDialog : public TEditJudicialDialog
{
public:
	TEditCompanyDialog(TWindow* parent, TDamn *damn, bool edit, bool full, bool stat, bool tReReg);

protected:
	bool reReg;		// 2010:075

	TCharAliasFace *companyType;
	TDateFace *reRegister;

	virtual bool IsValid();

//{{TEditCompanyDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditCompanyDialogVIRTUAL_END}}
};    //{{TEditCompanyDialog}}
