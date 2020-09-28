//{{TListWindow = TOriginSideListWindow}}
class TOriginSideListWindow : public TSideWindListWindow
{
public:
	TOriginSideListWindow(TWindow *parent, long sideMode, int resourceId, TOriginContainer *container,
		TLawsuit *lawsuit);

	char OriginKind() const;

protected:
	virtual bool CheckInvolvement(const TSideWind *sideWind);
	virtual bool EditableInvolvement(const TSideWind *sideWind);
};    //{{TOriginSideListWindow}}
