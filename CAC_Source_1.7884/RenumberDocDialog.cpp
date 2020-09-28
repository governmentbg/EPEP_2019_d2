#include "cac.h"

DEFINE_RESPONSE_TABLE1(TRenumberDocDialog, TGroupDialog)
//{{TRenumberDocDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TRenumberDocDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TRenumberDocDialog Implementation}}

TRenumberDocDialog::TRenumberDocDialog(TWindow* parent, TRCDKeyContainer *container, int resId)
:
	TGroupDialog(parent, container, resId)
{
	new TCharAliasFace(this, IDC_RENUMBER_DOC_KIND, "F_KIND", Kind, NULL);
	new TLongFace(this, IDC_RENUMBER_DOC_NO, "F_NO", SIZE_OF_NO);
	year = new TYearFace(this, IDC_RENUMBER_DOC_YEAR, "F_YEAR");
	date = new TDateFace(this, IDC_RENUMBER_DOC_DATE, "F_DATE");
}

void TRenumberDocDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;
		CmOk();
	}
}

bool TRenumberDocDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (date->Date() > Today)
			isValid = error("Датата е по-голяма от текущата дата.");
		else if (date->Date().year != year->L())
			isValid = error("Дата и година не си съответстват.");
	}

	return isValid;
}
