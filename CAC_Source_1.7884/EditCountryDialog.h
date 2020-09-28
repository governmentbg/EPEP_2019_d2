//{{TDialog = TEditCountryDialog}}
class TEditCountryDialog : public TEditDialog
{
public:
	TEditCountryDialog(TWindow* parent, TCountry *tCountry, bool edit);

protected:
	TStringFace *country;

//{{TEditCountryDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditCountryDialogVIRTUAL_END}}
};    //{{TEditCountryDialog}}
