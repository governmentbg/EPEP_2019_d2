//{{TDialog = TRenumberStreetDialog}}
class TRenumberStreetDialog : public TAlterStreetDialog
{
public:
	TRenumberStreetDialog(TWindow* parent, TStreet *source, TStreet *target, int resId = IDD_RENUMBER_STREET);

protected:
	TLongFace *targetStreet;

	virtual bool IsValid();

	virtual void ClearTarget();
	virtual long TargetValue();
};    //{{TRenumberStreetDialog}}
