//{{TDialog = TLaunchPerformAssignmentDialog}}
class TLaunchPerformAssignmentDialog : public TScaleDialog
{
public:
	TLaunchPerformAssignmentDialog(TWindow* parent, TGSArray<TAssignment> *tArray, const char *tLawKinds, int resId = IDD_LAUNCH_PERFORM_ASSIGNMENT);

protected:
	TGSArray<TAssignment> *array;
	const char *lawKinds;
	TStringListBox *kinds;

//{{TLaunchPerformAssignmentDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TLaunchPerformAssignmentDialogVIRTUAL_END}}

//{{TLaunchPerformAssignmentDialogRSP_TBL_BEGIN}}
	void AssignBNClicked();
	void AssignBNEnable(TCommandEnabler &tce);
//{{TLaunchPerformAssignmentDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TLaunchPerformAssignmentDialog);
};    //{{TLaunchPerformAssignmentDialog}}
