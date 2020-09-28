//{{TDialog = TArchiveIndexListDialog}}
class TArchiveIndexListDialog : public TGroupListDialog
{
public:
	TArchiveIndexListDialog(TWindow* parent, int resId = IDD_ARCHIVE_INDEX_LIST);

protected:
	TArchiveIndexListWindow *indexes;

	virtual bool CanClose();
	
//{{TArchiveIndexListDialogRSP_TBL_BEGIN}}
	void SubjectBNClicked();
	void PrintBNClicked();
	void CancelBNClicked();
	void EvClose();
//{{TArchiveIndexListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TArchiveIndexListDialog);
};    //{{TArchiveIndexListDialog}}
