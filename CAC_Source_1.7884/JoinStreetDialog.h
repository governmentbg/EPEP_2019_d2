//{{TDialog = TJoinStreetDialog}}
class TJoinStreetDialog : public TAlterStreetDialog
{
public:
	TJoinStreetDialog(TWindow* parent, TStreet *source, TStreet *target, int resId = IDD_JOIN_STREET);

protected:
	TFindStreetAliasFace *targetStreet;

	virtual bool IsValid();

	virtual void ClearTarget();
	virtual long TargetValue();
};    //{{TJoinStreetDialog}}
