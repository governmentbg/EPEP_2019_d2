DEFINE_RESPONSE_TABLE1(TGetNIAProtocolDialog, TGroupDialog)
//{{TGetNIAProtocolDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TGetNIAProtocolDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TGetNIAProtocolDialog Implementation}}

TGetNIAProtocolDialog::TGetNIAProtocolDialog(TWindow* parent, TNIAProtocol *protocol, nia_match_func tMatch, int resId)
:
	TGroupDialog(parent, protocol, resId), match(tMatch)
{
	typeLabel = new TStatic(this, IDC_GET_NIA_PROTOCOL_NIA_TYPE_LABEL);
	type = new TCharAliasFace(this, IDC_GET_NIA_PROTOCOL_NIA_TYPE, "F_NIA_TYPE", College, Default->xColleges);
	no = new TLongFace(this, IDC_GET_NIA_PROTOCOL_NIA_NO, "F_NIA_NO", SIZE_OF_NO);
	year = new TYearFace(this, IDC_GET_NIA_PROTOCOL_NIA_YEAR, "F_NIA_YEAR");
}

bool TGetNIAProtocolDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (!no->L() || !year->L())
			isValid = error("Номерът и годината са задължителни.");
	}

	return isValid;
}

void TGetNIAProtocolDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	mstr m("Но&мерация от ");
	m.cat(COLLEGE);
	typeLabel->SetCaption(str(m));

	if (!NIA_PROT_NUMBERING)
		type->SetC(TYPE_COMMON_ADM);
}

void TGetNIAProtocolDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Protocol() << this;

		if (Protocol()->Get())
		{
			const char *ts = match ? match(Protocol()) : NULL;

			if (ts)
				error(ts);
			else
				CmOk();
		}
	}
}
