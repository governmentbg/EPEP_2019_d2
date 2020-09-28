DEFINE_RESPONSE_TABLE1(TQueryArchiveDialog, TGroupDialog)
//{{TQueryArchiveDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TQueryArchiveDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TQueryArchiveDialog Implementation}}

TQueryArchiveDialog::TQueryArchiveDialog(TWindow* parent, int resId)
:
	TGroupDialog(parent, &archive, resId)
{
	build_ctypes(types, TYPE_LAWSUITS, false);
	new TCharAliasFace(this, IDC_GET_GROUP_TYPE, "F_TYPE", Type, types);
	no = new TLongFace(this, IDC_GET_GROUP_NO, "F_ARCHIVE_NO", SIZE_OF_NO);
	year = new TYearFace(this, IDC_GET_GROUP_YEAR, "F_ARCHIVE_YEAR");
}

void TQueryArchiveDialog::OKBNClicked()
{
	if (IsValid())
	{
		archive << this;
		query_archive(this, archive);
	}
}

bool TQueryArchiveDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!no->L() || !year->L())
			isValid = error("Архивният номер и година са задължителни.");
	}

	return isValid;
}
