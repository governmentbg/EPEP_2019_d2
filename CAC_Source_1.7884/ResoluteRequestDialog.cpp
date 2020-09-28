DEFINE_RESPONSE_TABLE1(TResoluteRequestDialog, TTroubleDialog)
//{{TResoluteRequestDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDRIGHT, RightBNClicked),
//{{TResoluteRequestDialogDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TResoluteRequestDialog Implementation}}

TResoluteRequestDialog::TResoluteRequestDialog(TWindow* parent, TRequestResolution *resolution, TRequest *tRequest, bool edit)
:
	TTroubleDialog(parent, resolution, edit | ATTACH_RIGHT, TYPE_TEXT, IDD_RESOLUTE_REQUEST), request(tRequest)
{
	requestKind = new TCharAliasFace(this, IDC_RESOLUTE_REQUEST_REQUEST_KIND, "F_REQUEST_KIND", Kind, NULL);
	new TLongFace(this, IDC_RESOLUTE_REQUEST_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_RESOLUTE_REQUEST_YEAR, "F_YEAR");
	new TCharAliasFace(this, IDC_RESOLUTE_REQUEST_RESOLUTION, "F_RESOLUTION", ::Resolution, RESOLUTION_RESOLUTIONS);
	date = new TDateFace(this, IDC_RESOLUTE_REQUEST_DATE, "F_DATE");
	new TStringFace(this, IDC_RESOLUTE_REQUEST_TEXT, "F_TEXT", SIZE_OF_RESOLUTION_TEXT);
	judge = new TUserAliasFace(this, IDC_RESOLUTE_REQUEST_JUDGE, "F_JUDGE", POST_VISIBLE_JUDGES,
		edit ? ALIAS_EMPTY | USER_INSECT : FLAG_ACTIVE | ALIAS_EMPTY | USER_INSECT);
	eventNo = new TLongFace(this, IDC_RESOLUTE_REQUEST_EVENT_NO, "F_EVENT_NO", SIZE_OF_EVENT_NO);
}

bool TResoluteRequestDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!atob(judge))
			isValid = error("Съдията е задължителен.");
	}

	return isValid;
}

void TResoluteRequestDialog::SetupWindow()
{
	TTroubleDialog::SetupWindow();

	if (!edit)
	{
		date->EnableWindow(true);
		eventNo->EnableWindow(true);
	}

	requestKind->SetC(request->kind);	// 2005:089
}

void TResoluteRequestDialog::RightBNClicked()
{
	if (IsValid())
	{
		attach_right(this, request->key, trouble, Resolution()->IndexKind(), date->Date(), judge->S());
		date->EnableWindow(!trouble.act);
		judge->EnableWindow(!trouble.act);
	}
}
