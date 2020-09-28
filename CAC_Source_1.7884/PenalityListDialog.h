//{{TDialog = TPenalityListDialog}}
class TPenalityListDialog : public TGroupListDialog
{
public:
	TPenalityListDialog(TWindow* parent, int resId = IDD_REALITY_LIST);

protected:
	TPenalityListWindow *penalities;

//{{TPenalityListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TPenalityListDialogVIRTUAL_END}}

//{{TPenalityListDialogRSP_TBL_BEGIN}}
	void SubjectBNClicked();
	void PrintBNClicked();
//{{TPenalityListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TPenalityListDialog);
};    //{{TPenalityListDialog}}
