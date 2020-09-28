DEFINE_RESPONSE_TABLE1(TCancelEisppDialog, TScaleDialog)
//{{TCancelEisppDialogRSP_TBL_BEGIN}}
	EV_WM_TIMER,
//{{TCancelEisppDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TCancelEisppDialog Implementation}}

TCancelEisppDialog::TCancelEisppDialog(TWindow* parent, const char *tNprnmr, const char *tXmlName, const char *tErrName,
	const char *tTmpName, long tTimeout, int resId)
:
	TScaleDialog(parent, resId), nprnmr(tNprnmr), xmlName(tXmlName), errName(tErrName), tmpName(tTmpName),
		timeout(tTimeout), counter(0)
{
	text = new TStatic(this, IDC_CANCEL_EISPP_TEXT);
	new TPushButton(this, IDCANCEL, IDI_CANCEL);
}

void TCancelEisppDialog::SetupWindow()
{
	TScaleDialog::SetupWindow();

	mstr m;
	m.printf("Справка за %s...", nprnmr);
	text->SetCaption(str(m));

	timerId = SetTimer(0, 1000, NULL);

	if (!timerId)
		fatal("Грешка при създаване на таймер.");
}

void TCancelEisppDialog::EvTimer(uint timerId)
{
	if (CopyFile(xmlName, tmpName, FALSE))
		CmOk();
	else
	{
		DWORD lastError = GetLastError();

		if (exist(errName))
			CloseDialog(IDIGNORE);

		if (lastError != ERROR_SHARING_VIOLATION && lastError != ERROR_LOCK_VIOLATION && ++counter == timeout)
			CloseDialog(IDRETRY);
	}

	TScaleDialog::EvTimer(timerId);
}
