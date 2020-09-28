DEFINE_RESPONSE_TABLE1(TBankruptSendDialog, TScaleDialog)
//{{TBankruptSendDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDEXPORT, ExportBNClicked),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
//{{TBankruptSendDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TBankruptSendDialog Implementation}}

TBankruptSendDialog::TBankruptSendDialog(TWindow* parent, TLawsuit *tLawsuit, int resId)
	: TScaleDialog(parent, resId), lawsuit(tLawsuit)
{
	header = new TStatic(this, IDC_BANKRUPT_SEND_HEADER);
	events = new TBankruptSendListBox(this, IDC_BANKRUPT_SEND_EVENTS, lawsuit);
}

TBankruptSendDialog::~TBankruptSendDialog()
{
}

void TBankruptSendDialog::SetupWindow()
{
	TScaleDialog::SetupWindow();

	mstr m;
	m.printf("%s %d/%dã.", Kind->Seek(lawsuit->kind), lawsuit->key.no, lawsuit->key.year);
	header->SetCaption(str(m));
	events->SelectAll();
}

void TBankruptSendDialog::ExportBNClicked()
{
	events->Export();
}

void TBankruptSendDialog::QueryBNClicked()
{
	events->Query();
}
