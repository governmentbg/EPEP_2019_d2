//{{TDialog = TRandomActDialog}}
class TRandomActDialog : public TGroupDialog
{
public:
	TRandomActDialog(TWindow* parent, TRandomActGroup *group);

protected:
	TUserAliasFace *judge;
	TDateFace *minDate;
	TDateFace *maxDate;
	TCheckFace *forceDate;
	TGroupBox *dateGroup;

	virtual bool IsValid();

//{{TRandomActDialogRSP_TBL_BEGIN}}
	void ForceDateBNClicked();
//{{TRandomActDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TRandomActDialog);
};    //{{TRandomActDialog}}
