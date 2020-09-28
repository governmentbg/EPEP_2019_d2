//{{TDialog = TAssignmentDialog}}
class TAssignmentDialog : public TResizeDialog
{
public:
	TAssignmentDialog(TWindow* parent, int resId);

protected:
	TAssignmentListWindow *assignments;

//{{TAssignmentRSP_TBL_BEGIN}}
	bool AssignmentsLVNItemChanging(TLwNotify &lwn);
	void OKBNClicked();
	void AddBNClicked();
	void EditBNClicked();
	void EditBNEnable(TCommandEnabler &tce);
//{{TAssignmentRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAssignmentDialog);
};    //{{TAssignmentDialog}}
