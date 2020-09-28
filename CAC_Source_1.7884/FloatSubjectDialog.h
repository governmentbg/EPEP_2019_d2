//{{TDialog = TFloatSubjectDialog}}
class TFloatSubjectDialog : public TFloatDialog
{
public:
	TFloatSubjectDialog(TWindow* parent, TFloatGroup *group, int resId);

protected:
	TPushButton *pushFull;
	TCheckBox *checkOld;
	TSubjectListBox *subjectsNew;
	TOldSubjectListBox *subjectsOld;

	void GetSubjects(TGArray<TSubject> &newSubjects, TGArray<TOldSubject> &oldSubjects);

//{{TFloatSubjectDialogRSP_TBL_BEGIN}}
	void PushFullBNClicked();
	void CheckOldBNClicked();
//{{TFloatSubjectDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TFloatSubjectDialog);
};  //{{TFloatSubjectDialog}}