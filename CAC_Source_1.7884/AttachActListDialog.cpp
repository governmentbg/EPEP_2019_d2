DEFINE_RESPONSE_TABLE1(TAttachActListDialog, TGroupListDialog)
//{{TAttachActListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDATTACH, SelectEnable),
	EV_BN_CLICKED(IDATTACH, AttachBNClicked),
//{{TAttachActListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAttachActListDialog Implementation}}

TAttachActListDialog::TAttachActListDialog(TWindow* parent, TGArray<TIndexContainer> *array)
:
	TGroupListDialog(parent, IDD_ATTACH_LIST)
{
	attaches = new TAttachActListWindow(this, IDC_GROUP_LIST_WINDOW, array);
}

void TAttachActListDialog::AttachBNClicked()
{
	attaches->Attach();
}
