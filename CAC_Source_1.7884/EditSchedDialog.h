//{{TDialog = TEditSchedDialog}}
class TEditSchedDialog : public TGroupDialog
{
public:
	TEditSchedDialog(TWindow* parent, TSched *sched, int resId = IDD_EDIT_SCHED);

protected:
	TDateFace *start;
	TDateFace *final;

	DEFINE_GROUP_TYPE(TSched, Sched)
	virtual bool IsValid();

//{{TEditSchedDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditSchedDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditSchedDialog);
};    //{{TEditSchedDialog}}
