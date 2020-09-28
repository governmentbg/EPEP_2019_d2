DEFINE_RESPONSE_TABLE1(TEditCurrencyDialog, TEditDialog)
//{{TEditCurrencyDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditCurrencyDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditCurrencyDialog Implementation}}

TEditCurrencyDialog::TEditCurrencyDialog(TWindow* parent, TCurrency *tCurrency, bool edit)
:
	TEditDialog(parent, tCurrency, edit, IDD_EDIT_CURRENCY)
{
	currency = new TLatinFace(this, IDC_EDIT_CURRENCY_CURRENCY, "F_CURRENCY_STRING", SIZE_OF_CURRENCY);
	new TStringFace(this, IDC_EDIT_CURRENCY_NAME, "F_NAME", SIZE_OF_NAME);
	new TLongFace(this, IDC_EDIT_CURRENCY_ORDER, "F_ORDER", SIZE_OF_ORDER);
	numeric = new TStringFace(this, IDC_EDIT_CURRENCY_NUMERIC, "F_NUMERIC", SIZE_OF_CURRENCY_PART);

	new TRadioFace(this, IDC_EDIT_CURRENCY_INTEGER_MALE, "F_INTEGER_MALE");
	new TRadioFace(this, IDC_EDIT_CURRENCY_INTEGER_FEMALE, "F_INTEGER_FEMALE");
	new TRadioFace(this, IDC_EDIT_CURRENCY_INTEGER_OTHER, "F_INTEGER_OTHER");
	new TStringFace(this, IDC_EDIT_CURRENCY_INTEGER_2, "F_INTEGER_2", SIZE_OF_CURRENCY_PART);
	new TStringFace(this, IDC_EDIT_CURRENCY_INTEGER_1, "F_INTEGER_1", SIZE_OF_CURRENCY_PART);
	new TStringFace(this, IDC_EDIT_CURRENCY_INTEGER_0, "F_INTEGER_0", SIZE_OF_CURRENCY_PART);

	new TRadioFace(this, IDC_EDIT_CURRENCY_FRACTION_MALE, "F_FRACTION_MALE");
	new TRadioFace(this, IDC_EDIT_CURRENCY_FRACTION_FEMALE, "F_FRACTION_FEMALE");
	new TRadioFace(this, IDC_EDIT_CURRENCY_FRACTION_OTHER, "F_FRACTION_OTHER");
	new TStringFace(this, IDC_EDIT_CURRENCY_FRACTION_2, "F_FRACTION_2", SIZE_OF_CURRENCY_PART);
	new TStringFace(this, IDC_EDIT_CURRENCY_FRACTION_1, "F_FRACTION_1", SIZE_OF_CURRENCY_PART);
	new TStringFace(this, IDC_EDIT_CURRENCY_FRACTION_0, "F_FRACTION_0", SIZE_OF_CURRENCY_PART);
}

bool TEditCurrencyDialog::IsValid()
{
	bool isValid = TEditDialog::IsValid();

	if (isValid)
	{
		if (strchr(numeric->S(), '?'))
			isValid = error("Суфикса за числом съдържа невалидни символи.");
	}

	return isValid;
}

void TEditCurrencyDialog::SetupWindow()
{
	TEditDialog::SetupWindow();
	currency->EnableWindow(!edit);
}

void TEditCurrencyDialog::OKBNClicked()
{
	// TODO? should be at DB level
	if (!*numeric->S())
		numeric->SetS(currency->S());

	TEditDialog::OKBNClicked();
}
