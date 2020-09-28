//{{TDialog = TAddSchedDialog}}
class TAddSchedDialog : public TGroupDialog
{
public:
	TAddSchedDialog(TWindow* parent, TSched *tSched, const char *scheds);

protected:
	TCharAliasFace *sched;
	TDateFace *start;
	TDateFace *final;
	TPersonAliasFace *judge;

	virtual bool IsValid();

//{{TAddSchedDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAddSchedDialogVIRTUAL_END}}

//{{TAddSchedDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void SchedCBNSelChange();
//{{TAddSchedDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAddSchedDialog);
};    //{{TAddSchedDialog}}
