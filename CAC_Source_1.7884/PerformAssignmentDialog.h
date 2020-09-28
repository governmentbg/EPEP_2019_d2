//{{TDialog = TPerformAssignmentDialog}}
class TPerformAssignmentDialog : public TAssignmentDialog
{
public:
	TPerformAssignmentDialog(TWindow* parent, TGSArray<TAssignment> *array, TDistribution *distribution, const char *kinds, int resId = IDD_PERFORM_ASSIGNMENT);

protected:
	TUserLoadListBox *load;

	virtual void Resize();

//{{TPerformAssignmentDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TPerformAssignmentDialogVIRTUAL_END}}

//{{TPerformAssignmentRSP_TBL_BEGIN}}
	void ViewBNClicked();
	void HistoryBNClicked();
	void RightBNClicked();
//{{TPerformAssignmentRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPerformAssignmentDialog);
};    //{{TPerformAssignmentDialog}}
