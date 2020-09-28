//{{TDialog = TFloatHeritageDialog}}
class TFloatHeritageDialog : public TFloatDialog
{
public:
	TFloatHeritageDialog(TWindow* parent, TFloatHeritageGroup *group);

protected:
	TCharListFace *heritages;
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TCharListFace *reqKinds;
	TDateFace *minFromDate;
	TDateFace *maxFromDate;

	DEFINE_GROUP_TYPE(TFloatHeritageGroup, FloatHeritageGroup)
	virtual bool IsValid();
};    //{{TFloatHeritageDialog}}
