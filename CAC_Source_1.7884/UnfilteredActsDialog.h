//{{TDialog = TUnfilteredActsDialog}}
class TUnfilteredActsDialog : public TGroupDialog
{
public:
	TUnfilteredActsDialog(TWindow* parent, TUnfilteredActsGroup *group);

protected:
	TTypeListFace *types;
	TDateFace *minDate;
	TDateFace *maxDate;

	DEFINE_GROUP_TYPE(TUnfilteredActsGroup, UnfilteredActsGroup)
	virtual bool IsValid();
};    //{{TUnfilteredActsDialog}}
