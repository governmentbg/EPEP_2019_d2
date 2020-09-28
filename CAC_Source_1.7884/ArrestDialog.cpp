DEFINE_RESPONSE_TABLE1(TArrestDialog, TEditDialog)
//{{TArrestDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDDELETE, DeleteBNClicked),
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
//{{TArrestDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TArrestDialog Implementation}}

TArrestDialog::TArrestDialog(TWindow* parent, TLawsuit *lawsuit, TAccusation *accusation, bool edit, bool start)
:
	TEditDialog(parent, accusation, edit, IDD_ARREST)
{
	new TCharAliasFace(this, IDC_ARREST_UCN_TYPE, "F_UCN_TYPE", UCNType, NULL);
	new TStringFace(this, IDC_ARREST_UCN, "F_UCN", SIZE_OF_UCN);
	new TStringFace(this, IDC_ARREST_ACCUSATION, "F_ACCUSATION", SIZE_OF_ACCUSATION_TEXT);
	new TStringFace(this, IDC_ARREST_NAME, "F_NAME", SIZE_OF_CITIZEN_NAME);
	new TStringFace(this, IDC_ARREST_RENAME, "F_RENAME", SIZE_OF_CITIZEN_RENAME);
	new TStringFace(this, IDC_ARREST_FAMILY, "F_FAMILY", SIZE_OF_CITIZEN_FAMILY);

	// 2001:212 Re-written
	if (start)
	{
		old = new TCheckFace(this, IDC_ARREST_OLD, "F_FLAGS_OLD");
		accusationNo = new TLongFace(this, IDC_ARREST_ACCUSATION_NO, "F_ACCUSATION_NO", SIZE_OF_NO);
		noLabel = new TStatic(this, IDC_ARREST_NO_LABEL);
		arrestDate = NULL;
		dateLabel = NULL;
		resolution = NULL;
		resolutionLabel = NULL;
	}
	else
	{
		old = NULL;
		accusationNo = NULL;
		noLabel = NULL;
		arrestDate = new TDateFace(this, IDC_ARREST_ARREST_DATE, "F_ARREST_DATE", true);
		dateLabel = new TStatic(this, IDC_ARREST_DATE_LABEL);
		resolution = new TCharAliasFace(this, IDC_ARREST_RESOLUTION, "F_RESOLUTION", Resolution, lawsuit->sourceKind == KIND_REQUEST_152A ? RESOLUTION_REQUEST_152AS : RESOLUTION_REQUEST_152BS);
		resolutionLabel = new TStatic(this, IDC_ARREST_RESOLUTION_LABEL);
	}
}

void TArrestDialog::SetupWindow()
{
	TEditDialog::SetupWindow();

	// From enter local judgement
	TCitizen citizen;

	citizen.ucnType = Accusation()->ucnType;
	strcpy(citizen.ucn, Accusation()->ucn);
	citizen.Try();
	citizen >> this;

	if (arrestDate)
	{
		arrestDate->ShowWindow(SW_SHOW);
		dateLabel->ShowWindow(SW_SHOW);
	}
	if (old)
	{
		old->ShowWindow(SW_SHOW);
		//old->SetCheck(Accusation()->flags & ACCUSATION_OLD ? BF_CHECKED : BF_UNCHECKED);
	}
	if (accusationNo)
	{
		accusationNo->ShowWindow(SW_SHOW);
		noLabel->ShowWindow(SW_SHOW);
	}
	if (resolution)
	{
		resolution->ShowWindow(SW_SHOW);
		resolutionLabel->ShowWindow(SW_SHOW);
	}
}

void TArrestDialog::DeleteBNClicked()
{
	if (Group()->Delete(true))
		CmOk();
	else
		error("Данните НЕ са изтрити.");
}

void TArrestDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(edit);
}
