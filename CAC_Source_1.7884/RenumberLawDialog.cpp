#include "cac.h"

DEFINE_RESPONSE_TABLE1(TRenumberLawDialog, TGroupDialog)
//{{TRenumberLawDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TRenumberLawDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TRenumberLawDialog Implementation}}

TRenumberLawDialog::TRenumberLawDialog(TWindow* parent, TLawsuit *lawsuit, int resId)
:
	TRenumberDocDialog(parent, lawsuit, resId)
{
	previousDate = new TDateFace(this, IDC_RENUMBER_LAW_PREVIOUS_DATE, "F_PREVIOUS_DATE");
	if (lawsuit->source.type == TYPE_REQUEST)
		receivedDate = NULL;
	else
		receivedDate = new TDateFace(this, IDC_RENUMBER_LAW_RECEIVED_DATE, "F_RECEIVED_DATE");
}

void TRenumberLawDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;
		CmOk();
	}
}

bool TRenumberLawDialog::IsValid()
{
	// Note: renumber doc IsValid() is intentionally bypassed
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		// copied from new law dialog, except for connects and year
		if (date->Date() > Today)
			isValid = error("Дата на образуване е по-голяма от текущата дата.");
		else if (date->Date().year != year->L())
			isValid = error("Дата на образуване и година не си съответстват.");
		else if (!previousDate->Date().Empty() && previousDate->Date() > date->Date())
			isValid = error("Дата на разпореждане за образуване е след датата на образуване.");
		else if (receivedDate != NULL && !receivedDate->Date().Empty() && !previousDate->Date().Empty() && receivedDate->Date() > previousDate->Date())
			isValid = error("Дата на получаване в съда е след датата на разпореждане за образуване.");
		else if (receivedDate != NULL && !receivedDate->Date().Empty() && receivedDate->Date() > date->Date())
			isValid = error("Дата на получаване в съда е след датата на образуване.");
	}

	return isValid;
}

void TRenumberLawDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	if (receivedDate)
		receivedDate->EnableWindow(true);
}
