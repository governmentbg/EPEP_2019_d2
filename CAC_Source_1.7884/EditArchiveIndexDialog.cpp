//{{TEditArchiveIndexDialog Implementation}}

TEditArchiveIndexDialog::TEditArchiveIndexDialog(TWindow* parent, TArchiveIndex *tArchiveIndex, bool edit, int resId)
:
	TEditDialog(parent, tArchiveIndex, edit, resId)
{
	archiveIndex = new TStringFace(this, IDC_EDIT_ARCHIVE_INDEX_INDEX, "F_ARCHIVE_INDEX_STRING", SIZE_OF_ALIAS);
	new TLongFace(this, IDC_EDIT_ARCHIVE_INDEX_KEEP, "F_KEEP", SIZE_OF_ARCHIVE_KEEP);
	new TStringFace(this, IDC_EDIT_ARCHIVE_INDEX_NAME, "F_NAME", SIZE_OF_ARCHIVE_INDEX_NAME);
	new TCheckFace(this, IDC_EDIT_ARCHIVE_INDEX_ACTIVE, "F_FLAGS_ACTIVE");
	new TCheckFace(this, IDC_EDIT_ARCHIVE_INDEX_EC, "F_FLAGS_EC");
	new TCheckFace(this, IDC_EDIT_ARCHIVE_INDEX_FLOAT, "F_FLAGS_FLOAT");
}

void TEditArchiveIndexDialog::SetupWindow()
{
	TEditDialog::SetupWindow();
	archiveIndex->EnableWindow(!edit);
}
