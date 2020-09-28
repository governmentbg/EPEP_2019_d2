//{{TDialog = TFloatOutRegDialog}}
class TFloatOutRegDialog : public TFloatSubjectDialog
{
public:
	TFloatOutRegDialog(TWindow* parent, TFloatOutRegGroup *group);

protected:
	TLawKindsRTFace *lawKindsRT;
	TCharListFace *inRegKinds;
	TDateFace *minDate;
	TDateFace *maxDate;
	TDateFace *minReturned;
	TDateFace *maxReturned;
	TSenderListBox *receivers;
	TUserListBox *judges;
	TCharListFace *resultsIII;
	TCharListFace *resultsKS;
	TStatic *vssIndexesLabel;
	TVSSIndexListBox *vssIndexes;
	TCharListFace *decKinds;
	TStringFace *text;
	TCharAutoListFace *compositions;

	DEFINE_GROUP_TYPE(TFloatOutRegGroup, FloatOutRegGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);

	void ReturnedENUpdate();
DECLARE_RESPONSE_TABLE(TFloatOutRegDialog);
};    //{{TFloatOutRegDialog}}
