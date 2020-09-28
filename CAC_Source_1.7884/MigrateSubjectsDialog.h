//{{TDialog = TMigrateSubjectsDialog}}
class TMigrateSubjectsDialog : public TQuickDialog
{
public:
	TMigrateSubjectsDialog(TWindow* parent, TUpdateSubjects *tUpdate);

protected:
	TUpdateSubjects *update;
	TCharAliasFace *kind;
	TSelectSubjectsListBox *sources;
	TSelectSubjectsListBox *targets;

	void CheckTargets();

//{{TMigrateSubjectsDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TMigrateSubjectsDialogVIRTUAL_END}}

//{{TMigrateSubjectsDialogRSP_TBL_BEGIN}}
	void KindCBNSelChange();
	void MigrateBNEnable(TCommandEnabler &tce);
	void MigrateBNClicked();
//{{TMigrateSubjectsDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TMigrateSubjectsDialog);
};    //{{TMigrateSubjectsDialog}}
