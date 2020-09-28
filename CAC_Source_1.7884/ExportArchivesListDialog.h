//{{TDialog = TExportArchivesListDialog}}
class TExportArchivesListDialog : public TGroupListDialog
{
public:
	TExportArchivesListDialog(TWindow* parent);

protected:
	DEFINE_GLIST_TYPE(TExportArchivesListWindow, Lawsuits)

//{{TExportArchivesListDialogVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TExportArchivesListDialogVIRTUAL_END}}

//{{TExportArchivesListDialogRSP_TBL_BEGIN}}
	void ExportBNEnable(TCommandEnabler &tce);
	void ExportBNClicked();
//{{TExportArchivesListDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TExportArchivesListDialog);
};    //{{TExportArchivesListDialog}}
