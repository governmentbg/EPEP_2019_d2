//{{TDialog = TSelectSubjectsDialog}}
class TSelectSubjectsDialog : public TQuickDialog
{
public:
	TSelectSubjectsDialog(TWindow* parent, TGArray<TSubject> *tSubjects, const char *kinds, bool saas,
		bool hidden = false);

protected:
	TSelectSubjectsListBox *subjects;
	TGArray<TSubject> *array;

//{{TSelectSubjectsDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSelectSubjectsDialogVIRTUAL_END}}

//{{TSelectSubjectsDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TSelectSubjectsDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSelectSubjectsDialog);
};    //{{TSelectSubjectsDialog}}
