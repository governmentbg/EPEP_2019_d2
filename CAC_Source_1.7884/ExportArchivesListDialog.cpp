DEFINE_RESPONSE_TABLE1(TExportArchivesListDialog, TGroupListDialog)
//{{TExportArchivesListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDEXPORT, ExportBNEnable),
	EV_BN_CLICKED(IDEXPORT, ExportBNClicked),
//{{TExportArchivesListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TExportArchivesListDialog Implementation}}

TExportArchivesListDialog::TExportArchivesListDialog(TWindow* parent)
:
	TGroupListDialog(parent, IDD_EXPORT_ARCHIVES_LIST)
{
	glist = new TExportArchivesListWindow(this, IDC_GROUP_LIST_WINDOW);
}

void TExportArchivesListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();
}

void TExportArchivesListDialog::ExportBNEnable(TCommandEnabler &tce)
{
	tce.Enable(Lawsuits()->Count() > 0);
}

void TExportArchivesListDialog::ExportBNClicked()
{
	Lawsuits()->Export();
}
