DEFINE_RESPONSE_TABLE1(TEisppSideDialog, TQuickDialog)
//{{TEisppSideDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDIGNORE, IgnoreBNClicked),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TEisppSideDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEisppSideDialog Implementation}}

TEisppSideDialog::TEisppSideDialog(TWindow* parent, sNPRAKTSTS *tNPR, int resId)
:
	TQuickDialog(parent, resId), npr(tNPR)
{
	fzls = new TFzlListBox(this, IDC_EISPP_SIDE_SIDES, npr->fzl);
	text = new TEdit(this, IDC_EISPP_SIDE_TEXT, 0);
}

void TEisppSideDialog::SetupWindow()
{
	TQuickDialog::SetupWindow();

	fzls->SelectAll();

	mstr m;
	m.printf("НП %s\r\n\r\n", npr->nmr);

	for (int i = 0; i < npr->fzl->Count(); i++)
	{
		sFZL *fzl = (*npr->fzl)[i];

		m.cat(fzl->imecyr);
		if (any(fzl->egn))
			m.printf(", ЕГН %s", fzl->egn);
		if (any(fzl->lnc))
			m.printf(", ЛНЧ %s", fzl->lnc);
		if (fzl->nprfzlsta->kcv)
			m.printf(", %s", EisVal->Seek(nmk_nprfzlkcv, fzl->nprfzlsta->kcv));
		m.cat("\r\n");

		for (int n = 0; n < npr->nprfzlpne->Count(); n++)
		{
			sNPRFZLPNE *nprfzlpne = (*npr->nprfzlpne)[n];

			if (nprfzlpne->fzlsid == fzl->sid)
			{
				sPNE *pne = find_pne(npr->pne, nprfzlpne->pnesid);

				m.printf("\t");
				if (nprfzlpne->scq->rlq)
					m.printf("%s по ", EisVal->Seek(nmk_fzlpnerlq, nprfzlpne->scq->rlq));
				if (pne)
					m.cat(pne->nmr);
				else
					m.printf("%d", nprfzlpne->pnesid);
				m.cat("\r\n");
			}
		}
	}
	m.cat("\r\n");

	for (int i = 0; i < npr->pne->Count(); i++)
	{
		sPNE *pne = (*npr->pne)[i];
		mstr w;

		pne->Print(w);
		m.printf("Престъпление %s\r\n", str(w));

		for (int n = 0; n < pne->pnespf->Count(); n++)
		{
			PNESPF *spf = (*pne->pnespf)[n];

			if (any(spf->opi))
				m.printf("\t%s\r\n", spf->opi);
		}
	}

	text->Transfer((void *) str(m), tdSetData);
}

void TEisppSideDialog::OKBNClicked()
{
	if (IsValid() && fzls->GetSelCount() == npr->fzl->Count() ||
		warn("Данните за демаркираните страни няма да се пренесат в САС. Желаете ли да продължите?"))
	{
		fzls->CutMark();
		CmOk();
	}
}

void TEisppSideDialog::IgnoreBNClicked()
{
	CloseDialog(IDIGNORE);
}

void TEisppSideDialog::CancelBNClicked()
{
}

void TEisppSideDialog::EvClose()
{
}
