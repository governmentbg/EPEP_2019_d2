//{{TDialog = TEditExecListTDepDialog}}
class TEditExecListTDepDialog : public TEditDialog
{
public:
	TEditExecListTDepDialog(TWindow* parent, TPrintExecList *execList);

protected:
	TStatic *header1;
	TStatic *header2;
	TStatic *header3;
	TNIATDepAliasFace *tdep;
	TNIATDepInNoFace *tdepNo;
	TDateFace *tdepDate;

	DEFINE_GROUP_TYPE(TPrintExecList, ExecList)
	virtual bool IsValid();

//{{TEditExecListTDepDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditExecListTDepDialogVIRTUAL_END}}

//{{TEditExecListTDepDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TEditExecListTDepDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditExecListTDepDialog);
};    //{{TEditExecListTDepDialog}}
