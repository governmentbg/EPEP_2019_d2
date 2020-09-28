//{{TDialog = TFloatArchiveDialog}}
class TFloatArchiveDialog : public TFloatSubjectDialog
{
public:
	TFloatArchiveDialog(TWindow* parent, TFloatArchiveGroup *group);

protected:
	TLongFace *minNo;
	TLongFace *maxNo;
	TYearFace *minYear;
	TYearFace *maxYear;
	TLongFace *minLinkNo;
	TLongFace *maxLinkNo;
	TLongFace *minArchiveNo;
	TLongFace *maxArchiveNo;
	TYearFace *minArchiveYear;
	TYearFace *maxArchiveYear;
	TDateFace *minArchiveDate;
	TDateFace *maxArchiveDate;
	TLongFace *minKeep;
	TLongFace *maxKeep;
	TArchiveIndexListBox *indexes;
	TCheckFace *excess;

	virtual void SetupWindow();

	DEFINE_GROUP_TYPE(TFloatArchiveGroup, FloatArchiveGroup)
	virtual bool IsValid();
	virtual void Action(bool execute);
};    //{{TFloatArchiveDialog}}
