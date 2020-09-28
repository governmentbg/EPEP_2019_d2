#include "cac.h"

DEFINE_RESPONSE_TABLE1(TFloatDialog, TGroupDialog)
//{{TFloatDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDFILTER, FilterBNClicked),
//{{TFloatDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFloatDialog Implementation}}

TFloatDialog::TFloatDialog(TWindow* parent, TFloatGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
}

#if TESTVER
void TFloatDialog::Message(mstr &m)
{
	m.cat(";");
	TLongTextDialog(this, str(m)).Execute();
}

void TFloatDialog::MessageEx(mstr &m)
{
	m.cat(";\n\nПровеждат се и допълнителни оценки.");
	TLongTextDialog(this, str(m)).Execute();
}
#endif  // TESTVER

void TFloatDialog::Action(bool execute)
{
	if (execute)
		CmOk();
	else
	{
		msql m;
		FloatGroup()->FormCriteria(m);
		Message(m);
	}
}

void TFloatDialog::Clicked(bool execute)
{
	if (IsValid())
	{
		*Group() << this;
		Action(execute);
	}
}

void TFloatDialog::OKBNClicked()
{
	Clicked(true);
}

void TFloatDialog::FilterBNClicked()
{
	Clicked(false);
}
