//{{TDialog = TSubjectArchiveIndexDialog}}
class TSubjectArchiveIndexDialog : public TQuickDialog
{
public:
	TSubjectArchiveIndexDialog(TWindow* parent, TGArray<TArchiveIndex> *tIndexes, int resId = IDD_SUBJECT_ARCHIVE_INDEX);

protected:
	TGArray<TArchiveIndex> *array;
	TArchiveIndexListBox *indexes;

//{{TSubjectArchiveIndexDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TSubjectArchiveIndexDialogVIRTUAL_END}}

//{{TSubjectArchiveIndexDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
//{{TSubjectArchiveIndexDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TSubjectArchiveIndexDialog);
};    //{{TSubjectArchiveIndexDialog}}
