//{{TDialog = TSelectOldSubjectsDialog}}
class TSelectOldSubjectsDialog : public TQuickDialog
{
public:
	TSelectOldSubjectsDialog(TWindow* parent, TGArray<TOldSubject> *tOldSubjects, const char *kinds);

protected:
	TOldSubjectListBox *oldSubjects;
	TGArray<TOldSubject> *array;

//{{TSelectOldSubjectsDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSelectOldSubjectsDialogVIRTUAL_END}}

//{{TSelectOldSubjectsDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TSelectOldSubjectsDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectOldSubjectsDialog);
};    //{{TSelectOldSubjectsDialog}}
