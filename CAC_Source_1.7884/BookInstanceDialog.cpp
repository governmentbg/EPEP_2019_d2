#include "cac.h"

TBookInstanceDialog::TBookInstanceDialog(TWindow* parent, TBookGroup *group, TCharAliasGroup *type, int resId, bool aliasFace)
	: TBookDialog(parent, group, type, resId, aliasFace)
{
	labelCaption = new TStatic(this, IDC_BOOK_INSTANCE_LABEL);
	instance_1st = new TCheckFace(this, IDC_BOOK_INSTANCE_1ST, "F_INSTANCE_1ST");
	instance_2nd = new TCheckFace(this, IDC_BOOK_INSTANCE_2ND, "F_INSTANCE_2ND");
}

bool TBookInstanceDialog::IsValid()
{
	bool isValid = TBookDialog::IsValid();

	if (isValid)
	{
		if (instance_1st->GetCheck() == BF_UNCHECKED && instance_2nd->GetCheck() == BF_UNCHECKED)
			isValid = error("Не е избрана нито една инстанция.");
	}

	return isValid;
}

void TBookInstanceDialog::SetupWindow()
{
	TBookDialog::SetupWindow();

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY

#if COURT_TYPE == COURT_MILITARY
	if (CourtType == COURT_MILITARY_APPEAL)
		instance_2nd->SetCheck(BF_CHECKED);
	else
		instance_1st->SetCheck(BF_CHECKED);

	labelCaption->EnableWindow(false);
	instance_1st->EnableWindow(false);
	instance_2nd->EnableWindow(false);
#else // MILITARY
	instance_1st->SetCheck(BF_CHECKED);
	instance_2nd->SetCheck(BF_CHECKED);
#endif //ADMIN || AREAL

#else // ADMIN || AREAL || MILITARY

#if COURT_TYPE == COURT_APPEAL
	instance_2nd->SetCheck(BF_CHECKED);
#else // APPEAL
	instance_1st->SetCheck(BF_CHECKED);
#endif // REGIONAL
	labelCaption->EnableWindow(false);
	instance_1st->EnableWindow(false);
	instance_2nd->EnableWindow(false);
#endif // APPEAL || REGIONAL
}
