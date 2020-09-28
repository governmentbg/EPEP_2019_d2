DEFINE_RESPONSE_TABLE1(TLegalResponseDialog, TQuickDialog)
//{{TLegalResponseDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDLOAD, LoadBNClicked),
	EV_COMMAND_ENABLE(IDLOAD, LoadBNEnable),
	EV_BN_CLICKED(IDVIEW, ViewBNClicked),
	EV_COMMAND_ENABLE(IDVIEW, ViewBNEnable),
	EV_LVN_ITEMCHANGING(IDC_LEGAL_RESPONSE_RESPONSES, ResponseLVNItemChanging),
	EV_LVN_ITEMCHANGED(IDC_LEGAL_RESPONSE_RESPONSES, ResponseLVNItemChanged),
//{{TLegalResponseDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TLegalResponseDialog Implementation}}

TLegalResponseDialog::TLegalResponseDialog(TWindow* parent, TGArray<TLegalResponse> *tResponses, int resId)
:
	TQuickDialog(parent, resId)
{
	responses = new TLegalResponseListWindow(this, IDC_LEGAL_RESPONSE_RESPONSES, tResponses);
	text = new TEdit(this, IDC_LEGAL_RESPONSE_TEXT, 0);
}

void TLegalResponseDialog::UpdateText()
{
	TLegalResponse *response = (TLegalResponse *) responses->Group();
	mstr m;

	if (response)
	{
		m.printf("%s %ld/%ld", Type->Seek(response->key.type), response->key.no, response->key.year);

		for (int i = 0; i < response->lawers.Count(); i++)
		{
			TAppointedLawer *lawer = response->lawers[i];
			mstr w;

			m.cat("\r\n");
			w.printf(" קנוח %s ", Involvement->Seek(response->involvement));

			size_t len = w.len();
			lawer->Print(w);
			mbk_remove(&w, len, SIZE_OF_UCN_TYPE_UCN);

			for (int n = 0; n < lawer->litigants.Count(); n++)
			{
				m.cat("\r\n");
				lawer->litigants[n]->PrintNames(m);
				m.cat(w);
			}
		}
	}

	text->Transfer(ncstr(m), tdSetData);
}

void TLegalResponseDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();
}

void TLegalResponseDialog::LoadBNClicked()
{
	if (responses->LoadGroup())
		UpdateText();
}

void TLegalResponseDialog::LoadBNEnable(TCommandEnabler &tce)
{
	tce.Enable(responses->Group());
}

void TLegalResponseDialog::ViewBNClicked()
{
	responses->ViewGroup();
}

void TLegalResponseDialog::ViewBNEnable(TCommandEnabler &tce)
{
	tce.Enable(responses->Group());
}

bool TLegalResponseDialog::ResponseLVNItemChanging(TLwNotify &lwn)
{
	return responses->ItemChanging(lwn);
}

void TLegalResponseDialog::ResponseLVNItemChanged(TLwNotify &)
{
	UpdateText();
}
