#include "cac.h"

DEFINE_RESPONSE_TABLE1(TTroubleDialog, TGroupDialog)
//{{TTroubleDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDATTACH, AttachBNClicked),
	EV_BN_CLICKED(IDSWAP, SwapBNClicked),
	EV_BN_CLICKED(IDC_TROUBLE_PUBLINET, PublInetBNClicked),
	EV_BN_CLICKED(IDC_TROUBLE_PUBLTREG, PublTregBNClicked),
//{{TTroubleDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TTroubleDialog Implementation}}

TTroubleDialog::TTroubleDialog(TWindow* parent, TRCDKeyContainer *container, long tEdit, int type, int resId)
:
	TGroupDialog(parent, container, resId), edit(tEdit & true), trouble(this, type, tEdit & ATTACH_RIGHT),
		electricDate(&container->date), publInet(NULL), publTreg(NULL)
{
}

void TTroubleDialog::PostMove()
{
	trouble.PostMove(Container(), *electricDate);
}

void TTroubleDialog::SetupPublic(bool publres, bool portreg, const TLawsuit *lawsuit)
{
	if (edit)
	{
		// 2014:332 p.70: lock
		if (lawsuit->LimitedNotAdmin())
		{
			publInet->EnableWindow(false);
			publInet->SetCheck(BF_UNCHECKED);
		}
	}
	else
	{
		// 2009:198; 2011:341 LPR/LRQ: completizeds disable -> don't set checked
		// 2014:332 p.70: lock
		if (lawsuit->flags & LAWSUIT_LIMITED)
			publInet->EnableWindow(Default->post == POST_ADMIN);
		else if (publres)
			publInet->SetCheck(BF_CHECKED);
	}

	SetupPublic(portreg);
}

void TTroubleDialog::SetupPublic(bool portreg)
{
	allowPublInet = publInet->IsWindowEnabled();
	checkPublInet = publInet->GetCheck() == BF_CHECKED;
	PublInetBNClicked();

	if (publTreg)
	{
		publTreg->SetCheck(portreg && publTreg->IsWindowEnabled() ? BF_CHECKED : BF_UNCHECKED);
		publTreg->ShowWindow(SW_SHOW);
	}

	PublTregBNClicked();
}

bool TTroubleDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
		isValid = trouble.IsValid();

	return isValid;
}

void TTroubleDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	if (edit)
		trouble.Name(Container(), *electricDate);

	trouble.BackupNames();
}

void TTroubleDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;

		if (!trouble.Move(Container(), *electricDate))
			return;

		if (Group()->Save(edit))
		{
			PostMove();
			CmOk();
		}
	}
}

void TTroubleDialog::AttachBNClicked()
{
	trouble.Edit(edit);
}

void TTroubleDialog::SwapBNClicked()
{
	trouble.Swap();
}

void TTroubleDialog::PublInetBNClicked()
{
	if (publTreg)
	{
		if (publInet->GetCheck() == BF_UNCHECKED)
		{
			publTreg->EnableWindow(false);
			publTreg->SetCheck(BF_UNCHECKED);
			SetPortreg(false);
		}
		else
			publTreg->EnableWindow(can_portable());
	}
}

void TTroubleDialog::PublTregBNClicked()
{
	if (publTreg)
	{
		bool portreg = publTreg->GetCheck() == BF_CHECKED;

		if (portreg)
		{
			publInet->EnableWindow(false);
			publInet->SetCheck(BF_CHECKED);
		}
		else if (allowPublInet)
		{
			publInet->EnableWindow(true);
			publInet->SetCheck(checkPublInet);
		}

		SetPortreg(portreg);
	}
}
