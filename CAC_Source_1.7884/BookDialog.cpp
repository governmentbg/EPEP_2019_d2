#include "cac.h"

DEFINE_RESPONSE_TABLE1(TBookDialog, TFloatDialog)
//{{TBookDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_BOOK_BY_NO_YEAR, ByBNClicked),
	EV_BN_CLICKED(IDC_BOOK_BY_PERIOD, ByBNClicked),
//{{TBookDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TBookDialog Implementation}}

TBookDialog::TBookDialog(TWindow* parent, TBookGroup *group, TCharAliasGroup *tType, int resId, bool tAliasFace)
:
	TFloatDialog(parent, group, resId), aliasFace(tAliasFace), aliasType(tType != College && tType != CollegeRight)
{
	if (aliasFace)
		type = new TCharAliasFace(this, IDC_BOOK_TYPES, "F_TYPE", tType, group->types);
	else
		types = new TCharListFace(this, IDC_BOOK_TYPES, "F_TYPES", tType, group->types, SIZE_OF_TYPES);

	byNoYear = new TRadioFace(this, IDC_BOOK_BY_NO_YEAR, "F_BY_NO_YEAR");
	byPeriod = new TRadioFace(this, IDC_BOOK_BY_PERIOD, "F_BY_PERIOD");
	minNo = new TLongFace(this, IDC_BOOK_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNo = new TLongFace(this, IDC_BOOK_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	year = new TYearFace(this, IDC_BOOK_YEAR, "F_YEAR");
	minDate = new TDateFace(this, IDC_BOOK_MIN_DATE, "F_MIN_DATE", false);
	minDateUpDn = new TDateFaceUpDown(this, IDC_BOOK_MIN_DATE_UPDN, minDate);
	maxDate = new TDateFace(this, IDC_BOOK_MAX_DATE, "F_MAX_DATE", false);
	maxDateUpDn = new TDateFaceUpDown(this, IDC_BOOK_MAX_DATE_UPDN, maxDate);
}

TBookDialog::~TBookDialog()
{
}

bool TBookDialog::IsValid()
{
	bool isValid = TFloatDialog::IsValid();

	if (isValid)
	{
		char selected = aliasFace ? type->C() : *types->S();

		if (!selected)
			isValid = error(aliasType ? "Не е избран нито един вид дело." : "Не е избрана нито една колегия/отделение.");
		else if (byNoYear->GetCheck() == BF_CHECKED)
		{
			if (!minNo->L())
				isValid = error("Началният номер е задължителен.");
			else if (invalid_range(minNo, maxNo))
				isValid = error("Началният номер е по-голям от крайния.");
			else if (!year->L())
				isValid = error("Годината е задължителна.");
		}
		else if (byPeriod->GetCheck() == BF_CHECKED)
		{
			if (invalid_range(minDate, maxDate))
				isValid = error("Началната дата е по-голяма от крайната.");
		}
	}

	return isValid;
}

void TBookDialog::SetupWindow()
{
	TFloatDialog::SetupWindow();

	if (!aliasFace)
	{
		types->SetS("");
		types->SetCaretIndex(0, false);
	}

	ByBNClicked();
}

void TBookDialog::ByBNClicked()
{
	// N.B. both may be false in derived dialogs
	bool tByNoYear = byNoYear->GetCheck() == BF_CHECKED;
	bool tByPeriod = byPeriod->GetCheck() == BF_CHECKED;

	minNo->EnableWindow(tByNoYear);
	maxNo->EnableWindow(tByNoYear);
	year->EnableWindow(tByNoYear);
	minDate->EnableWindow(tByPeriod);
	minDateUpDn->EnableWindow(tByPeriod);
	maxDate->EnableWindow(tByPeriod);
	maxDateUpDn->EnableWindow(tByPeriod);
}
