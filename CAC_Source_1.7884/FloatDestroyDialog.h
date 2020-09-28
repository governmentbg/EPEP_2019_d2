//{{TDialog = TFloatDestroyDialog}}
class TFloatDestroyDialog : public TFloatDialog
{
public:
	TFloatDestroyDialog(TWindow* parent, TFloatDestroyGroup *group);

protected:
	TTypeListFace *types;
	TDateFace *minKillDate;
	TDateFace *maxKillDate;
	TLongFace *minKillerNo;
	TLongFace *maxKillerNo;
	TDateFace *minKillerDate;
	TDateFace *maxKillerDate;
	TLongFace *minRemainderNo;
	TLongFace *maxRemainderNo;
	TYearFace *minRemainderYear;
	TYearFace *maxRemainderYear;
	TArchiveIndexListBox *indexes;

	DEFINE_GROUP_TYPE(TFloatDestroyGroup, FloatDestroyGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);
};    //{{TFloatDestroyDialog}}
