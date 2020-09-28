DEFINE_RESPONSE_TABLE1(TSubjectArchiveIndexDialog, TQuickDialog)
//{{TSubjectArchiveIndexDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TSubjectArchiveIndexDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSubjectArchiveIndexDialog Implementation}}

TSubjectArchiveIndexDialog::TSubjectArchiveIndexDialog(TWindow* parent, TGArray<TArchiveIndex> *tIndexes, int resId)
:
	TQuickDialog(parent, resId), array(tIndexes)
{
	indexes = new TArchiveIndexListBox(this, IDC_SUBJECT_ARCHIVE_INDEX_INDEXES, false);
}

void TSubjectArchiveIndexDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();
	indexes->SetMark(array);
}

void TSubjectArchiveIndexDialog::OKBNClicked()
{
	if (IsValid())
	{
		indexes->GetMark(array);
		CmOk();
	}
}
