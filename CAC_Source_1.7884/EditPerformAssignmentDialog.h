//{{TDialog = TEditPerformAssignmentDialog}}
class TEditPerformAssignmentDialog : public TGroupDialog
{
public:
	TEditPerformAssignmentDialog(TWindow* parent, TAssignment *assignment, TDistribution *distribution, int resId = IDD_EDIT_PERFORM_ASSIGNMENT);

protected:
	TStatic *label;
	TStatic *left;
	TStatic *right;
	TCharAliasFace *resolution;
	TSubjectAliasFace *subject;
	TPersonDistribAliasFace *assign;
	TStringFace *text;

	DEFINE_GROUP_TYPE(TAssignment, Assignment)
	virtual bool IsValid();

//{{TEditPerformAssignmentDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditPerformAssignmentDialogVIRTUAL_END}}

//{{TEditPerformAssignmentDialogRSP_TBL_BEGIN}}
	void ResolutionCBNSelChange();
	void OKBNClicked();
//{{TEditPerformAssignmentDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TEditPerformAssignmentDialog);
};    //{{TEditPerformAssignmentDialog}}
