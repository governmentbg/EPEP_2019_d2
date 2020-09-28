//{{TDialog = TEditPrepareAssignmentDialog}}
class TEditPrepareAssignmentDialog : public TGroupDialog
{
public:
	TEditPrepareAssignmentDialog(TWindow* parent, TAssignment *assignment, TDistribution *distribution, int resId = IDD_EDIT_PREPARE_ASSIGNMENT);

protected:
	TStatic *label;
	TStatic *left;
	TStatic *right;
	TSubjectAliasFace *subject;
	TPersonAliasFace *judge;

	DEFINE_GROUP_TYPE(TAssignment, Assignment)
	virtual bool IsValid();

//{{TEditPrepareAssignmentDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TEditPrepareAssignmentDialogVIRTUAL_END}}
};    //{{TEditPrepareAssignmentDialog}}
