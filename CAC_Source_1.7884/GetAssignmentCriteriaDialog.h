//{{TDialog = TGetAssignmentCriteriaDialog}}
class TGetAssignmentCriteriaDialog : public TPeriodDialog
{
public:
	TGetAssignmentCriteriaDialog(TWindow* parent, TAssignmentGroup *group, int resId = IDD_GET_ASSIGNMENT_CRITERIA);

protected:
	TCharListFace *kinds;

	virtual bool IsValid();
};    //{{TGetAssignmentCriteriaDialog}}
