DEFINE_RESPONSE_TABLE1(TImportantDialog, TGroupDialog)
//{{TImportantDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TImportantDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TImportantDialog Implementation}}

TImportantDialog::TImportantDialog(TWindow* parent, TLawsuit *lawsuit, bool edit, int resId)
:
	TGroupDialog(parent, lawsuit, resId)
{
	new TCharAliasFace(this, IDC_IMPORTANT_TYPE, "F_TYPE", Type, NULL);
	new TLongFace(this, IDC_IMPORTANT_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_IMPORTANT_YEAR, "F_YEAR");
	new TCheckFace(this, IDC_IMPORTANT_IMPORTANT, "F_FLAGS_IMPORTANT");
	new TCheckFace(this, IDC_IMPORTANT_COMPLEX, "F_FLAGS_COMPLEX");
	new TStringFace(this, IDC_IMPORTANT_TEXT, "F_IMPORTANT", SIZE_OF_LAWSUIT_IMPORTANT);
	danger = new TDangerAliasFace(this, IDC_IMPORTANT_DANGER, "F_DANGER", lawsuit->subject,
		edit ? ALIAS_EMPTY : FLAG_ACTIVE | ALIAS_EMPTY);
}

void TImportantDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	if (!strchr(KIND_DANGER_LAWS, Lawsuit()->kind))
	{
		danger->SetL(0);
		danger->EnableWindow(false);
	}
}
