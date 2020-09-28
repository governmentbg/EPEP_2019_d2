#include "cac.h"

DEFINE_RESPONSE_TABLE1(TEditIncomingDialog, TTroubleDialog)
//{{TEditIncomingDialogRSP_TBL_BEGIN}}
	EV_EN_UPDATE(IDC_EDIT_INCOMING_DATE, DateENUpdate),
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEditIncomingDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEditIncomingDialog Implementation}}

TEditIncomingDialog::TEditIncomingDialog(TWindow* parent, TRCDKeyContainer *incoming, const char *kinds,
	const char *colleges, bool edit, int resId, long collegeFlags)
:
	TTroubleDialog(parent, incoming, edit, TYPE_IMAGE, resId)
{
	kind = new TCharAliasFace(this, IDC_EDIT_INCOMING_KIND, "F_KIND", Kind, kinds);
	no = new TLongFace(this, IDC_EDIT_INCOMING_NO, "F_NO", SIZE_OF_NO);
	date = new TDateFace(this, IDC_EDIT_INCOMING_DATE, "F_DATE");
	time = new TTimeFace(this, IDC_EDIT_INCOMING_TIME, "F_TIME", false);
	college = new TCharAliasFace(this, IDC_EDIT_INCOMING_COLLEGE, "F_COLLEGE", College, colleges, collegeFlags);
	direction = new TStringFace(this, IDC_EDIT_INCOMING_DIRECTION, "F_DIRECTION", SIZE_OF_DIRECTION);
}

bool TEditIncomingDialog::CheckDirection()
{
	return any(direction->S()) || ask("Не е въведено направление. Желаете ли да продължите?");
}

void TEditIncomingDialog::SetupWindow()
{
	TTroubleDialog::SetupWindow();

	if (edit)
	{
		kind->EnableWindow(false);
		no->EnableWindow(false);
		date->EnableWindow(false);
	}
	else
		date->EnableWindow(!LOCK_RECEIVE);

	time->EnableWindow(false);
}

bool TEditIncomingDialog::IsValid()
{
	bool isValid = TTroubleDialog::IsValid();

	if (isValid)
	{
		if (date->Date() > Today)
			isValid = error("Датата на документа е по-голяма от текущата дата.");
		else if (time->IsWindowEnabled() && time->Time().Empty())
			isValid = error("Часът е задължителен.");
		// 2015:138 FIX: direction check in derives OK/IsValid -> removed from here
	}

	return isValid;
}

void TEditIncomingDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Incoming() << this;

		if (Incoming()->Save(edit))
		{
			trouble.Move(Incoming(), date->Date());
			CmOk();
		}
	}
}

void TEditIncomingDialog::DateENUpdate()
{
	if (!edit && date_valid(date->S(), true))
	{
		if (date->Date() == Today)
		{
			time->EnableWindow(false);
			time->SetS("");
		}
		else
			time->EnableWindow(true);
	}
}
