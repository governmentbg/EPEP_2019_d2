//{{TDialog = TPrepareAssignmentDialog}}
class TPrepareAssignmentDialog : public TAssignmentDialog
{
public:
	TPrepareAssignmentDialog(TWindow* parent, TGSArray<TAssignment> *array, TAssignmentGroup *tCriteria, TDistribution *distribution, int resId = IDD_PREPARE_ASSIGNMENT);

protected:
	TAssignmentGroup *criteria;

//{{TPrepareAssignmentDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TPrepareAssignmentDialogVIRTUAL_END}}
};    //{{TPrepareAssignmentDialog}}
