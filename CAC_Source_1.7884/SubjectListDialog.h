//{{TDialog = TSubjectListDialog}}
class TSubjectListDialog : public TGroupListDialog
{
public:
	TSubjectListDialog(TWindow* parent);

protected:
	TCharAliasFace *kind;
	TSubjectListWindow *subjects;

//{{TSubjectListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSubjectListDialogVIRTUAL_END}}

//{{TSubjectListDialogRSP_TBL_BEGIN}}
	void KindCBNSelChange();
	void CopyBNClicked(); 
	void PrintBNClicked();
	void RightBNClicked();
	void DeleteBNEnable(TCommandEnabler &tce);
	void AddBNEnable(TCommandEnabler &tce);
	void CopyBNEnable(TCommandEnabler &tce);
//{{TSubjectListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSubjectListDialog);
};    //{{TSubjectListDialog}}
