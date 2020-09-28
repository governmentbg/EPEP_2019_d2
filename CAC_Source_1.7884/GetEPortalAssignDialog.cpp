DEFINE_RESPONSE_TABLE1(TGetEPortalAssignDialog, TGetGroupDialog)
//{{TGetEPortalAssignDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TGetEPortalAssignDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetEPortalAssignDialog Implementation}}

TGetEPortalAssignDialog::TGetEPortalAssignDialog(TWindow* parent, TEPortAssign *tAssign, long tEPortalMode,
	const TSideWind *tSideWind)
:
	TGetGroupDialog(parent, &lawsuit, NULL, TYPE_LAWSUITS, KIND_LAWSUITS, NULL, match_null, 0, IDD_GET_EPORTAL_ASSIGN),
		assign(tAssign), eportalMode(tEPortalMode), sideWind(tSideWind)
{
	eportalSubpoena = new TCheckFace(this, IDC_GET_EPORTAL_ASSIGN_EPORTAL_SUBPOENA, "F_EPORTAL_SUBPOENA");
	involvement = new TCharAliasFace(this, IDC_GET_EPORTAL_ASSIGN_INVOLVEMENT, "F_INVOLVEMENT", Involvement,
		strchr(INVOLVEMENT_LAWERS, tSideWind->involvement) ? INVOLVEMENT_LAWERS : INVOLVEMENT_NON_LAWERS, ALIAS_EMPTY);
}

void TGetEPortalAssignDialog::SetupWindow()
{
	TGetGroupDialog::SetupWindow();

	eportalSubpoena->EnableWindow(eportalMode == EPORTAL_GRANT);
}

void TGetEPortalAssignDialog::OKBNClicked()
{
	lawsuit << this;
	assign->involvement = involvement->C();

	if (find_eportal_side(assign, involvement->XCrit(), sideWind, lawsuit))
	{
		if (lawsuit.Get() && eportal_check_send(this, &lawsuit, eportalMode))
		{
			assign->lawsuit = lawsuit.key;
			assign->flags = eportalSubpoena->GetCheck() == BF_CHECKED ? EPORTAL_SUBPOENA : 0;
			CmOk();
		}
	}
}
