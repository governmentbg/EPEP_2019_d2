//{{TDialog = TQueryInconSideDialog}}
class TQueryInconSideDialog : public TFloatDialog
{
public:
	TQueryInconSideDialog(TWindow* parent, TInconSideGroup *group, int resId = IDD_QUERY_INCONSIDE);

protected:
	TRadioFace *creditor;
	TRadioFace *deptor;
	TDateFace *minDate;
	TDateFace *maxDate;

	virtual bool IsValid();
};    //{{TQueryInconSideDialog}}
