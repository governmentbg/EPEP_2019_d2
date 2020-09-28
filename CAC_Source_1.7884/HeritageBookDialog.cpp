DEFINE_RESPONSE_TABLE1(THeritageBookDialog, TFloatDialog)
	EV_BN_CLICKED(IDC_HERITAGE_BOOK_BY_FINAL, ByFinalBNClicked),
	EV_BN_CLICKED(IDC_HERITAGE_BOOK_BY_FORCE, ByForceBNClicked),
END_RESPONSE_TABLE;

THeritageBookDialog::THeritageBookDialog(TWindow* parent, THeritageBookGroup *group, int resId)
	: TFloatDialog(parent, group, resId)
{
	byFinal = new TRadioFace(this, IDC_HERITAGE_BOOK_BY_FINAL, "F_BY_FINAL");
	minFinal = new TDateFace(this, IDC_HERITAGE_BOOK_MIN_FINAL, "F_MIN_FINAL");
	minFinalUpDn = new TDateFaceUpDown(this, IDC_HERITAGE_BOOK_MIN_FINAL_UPDN, minFinal);
	maxFinal = new TDateFace(this, IDC_HERITAGE_BOOK_MAX_FINAL, "F_MAX_FINAL");
	maxFinalUpDn = new TDateFaceUpDown(this, IDC_HERITAGE_BOOK_MAX_FINAL_UPDN, maxFinal);
	byForce = new TRadioFace(this, IDC_HERITAGE_BOOK_BY_FORCE, "F_BY_FORCE");
	minForce = new TDateFace(this, IDC_HERITAGE_BOOK_MIN_FORCE, "F_MIN_FORCE");
	minForceUpDn = new TDateFaceUpDown(this, IDC_HERITAGE_BOOK_MIN_FORCE_UPDN, minForce);
	maxForce = new TDateFace(this, IDC_HERITAGE_BOOK_MAX_FORCE, "F_MAX_FORCE");
	maxForceUpDn = new TDateFaceUpDown(this, IDC_HERITAGE_BOOK_MAX_FORCE_UPDN, maxForce);
	autogen = new TLongFace(this, IDC_HERITAGE_BOOK_AUTO_SERIAL_NO, "F_AUTO_SERIAL_NO", SIZE_OF_NO);
}

bool THeritageBookDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		if (byFinal->GetCheck() == BF_CHECKED)
		{
			if (minFinal->Date().Empty() || maxFinal->Date().Empty())
				isValid = error("Датите на свършване са задължителни.");
			else if (invalid_range(minFinal, maxFinal))
				isValid = error("Началната дата на свършване е по-голяма от крайната.");
		}
		else
		{
			if (minForce->Date().Empty() || maxForce->Date().Empty())
				isValid = error("Датите на влизане в законна сила са задължителни.");
			else if (invalid_range(minFinal, maxFinal))
				isValid = error("Началната дата на влизане в законна сила е по-голяма от крайната.");
		}
	}

	return isValid;
}

void THeritageBookDialog::Action(bool execute)
{
	THeritageBookGroup *group = (THeritageBookGroup *) Group();

	if (byFinal->GetCheck() == BF_CHECKED)
	{
		group->minDate = minFinal->Date();
		group->maxDate = maxFinal->Date();
	}
	else
	{
		group->minDate = minForce->Date();
		group->maxDate = maxForce->Date();
	}

	TFloatDialog::Action(execute);
}

void THeritageBookDialog::UpdateByDates()
{
	long tByFinal = byFinal->GetCheck() == BF_CHECKED;

	minFinal->EnableWindow(tByFinal);
	minFinalUpDn->EnableWindow(tByFinal);
	maxFinal->EnableWindow(tByFinal);
	maxFinalUpDn->EnableWindow(tByFinal);
	minForce->EnableWindow(!tByFinal);
	minForceUpDn->EnableWindow(!tByFinal);
	maxForce->EnableWindow(!tByFinal);
	maxForceUpDn->EnableWindow(!tByFinal);
}

void THeritageBookDialog::ByFinalBNClicked()
{
	UpdateByDates();
}

void THeritageBookDialog::ByForceBNClicked()
{
	UpdateByDates();
}
