#include "cac.h"

DEFINE_RESPONSE_TABLE1(TOriginSideListDialog, TSideWindListDialog)
//{{TOriginSideListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDSTATE, StateBNEnable),
	EV_COMMAND_ENABLE(IDCHILD, ChildBNEnable),
	EV_COMMAND_ENABLE(IDMONEY, SelectEnable),
	EV_BN_CLICKED(IDSTATE, StateBNClicked),
	EV_BN_CLICKED(IDCHILD, ChildBNClicked),
	EV_BN_CLICKED(IDMONEY, MoneyBNClicked),
//{{TOriginSideListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TOriginSideListDialog Implementation}}

static TSideWindListWindow *createSideWinds(TWindow* parent, long sideMode, TRCDKeyContainer *container, TLawsuit *lawsuit)
{
	return new TOriginSideListWindow(parent, sideMode, IDC_GROUP_LIST_WINDOW, (TOriginContainer *) container, lawsuit);
}

TOriginSideListDialog::TOriginSideListDialog(TWindow* parent, TRCDKeyContainer *container, TLawsuit *lawsuit, long sideMode,
	const char *what, int resId)
:
	TSideWindListDialog(parent, container, lawsuit, sideMode, what, resId, createSideWinds),
		statable(strchr(KIND_STATABLE, OriginKind()))
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	sideWinds->stat = strchr(KIND_INCONSISTENCE_REQUESTS, OriginKind());
#endif  // APPEAL || AREAL
	child = new TPushButton(this, IDCHILD);
}

void TOriginSideListDialog::Header2(mstr &m)
{
	TLong *quick = (TLong *) container->Find("F_QUICK");

	if (quick->L() & QUICK_QUICK)
		m.cat(quick->L() & QUICK_IMMEDIATE ? "НЕЗАБАВНО ПРОИЗВОДСТВО" : "БЪРЗО ПРОИЗВОДСТВО");
	else
		TSideWindListDialog::Header2(m);
}

const char *TOriginSideListDialog::LackingPrimary() const
{
#if COURT_TYPE == COURT_REGIONAL
	if (OriginKind() == KIND_EUHER_REQUEST)
	{
		bool count[SIZE_OF_INVOLVEMENT_ALLS];
		memset(count, '\0', sizeof count);

		for (int index = 0; index < sideWinds->Count(); index++)
		{
			TSideWind *sideWind = sideWinds->SideWindAt(index);
			int sideIndex = strindex(INVOLVEMENT_EUHER_MANDATORYS, sideWind->involvement);

			if (sideIndex != INT_MAX)
				count[sideIndex]++;
		}

		for (int index = 0; index < strlen(INVOLVEMENT_EUHER_MANDATORYS); index++)
			if (!count[index])
				return Involvement->Seek(INVOLVEMENT_EUHER_MANDATORYS[index]);
	}
#endif  // REGIONAL
	return TSideWindListDialog::LackingPrimary();
}

int TOriginSideListDialog::PrimaryCount(char involvement) const
{
#if COURT_TYPE == COURT_REGIONAL
	if (involvement && OriginKind() == KIND_EUHER_REQUEST && strchr(INVOLVEMENT_EUHER_MANDATORYS, involvement))
	{
		int count = 0;

		for (int index = 0; index < sideWinds->Count(); index++)
			if (sideWinds->SideWindAt(index)->involvement == involvement)
				count++;

		return count;
	}
#endif  // REGIONAL
	return TSideWindListDialog::PrimaryCount(involvement);
}

bool TOriginSideListDialog::CanClose()
{
#if COURT_TYPE == COURT_AREAL
	if (sideWinds->stat)
	{
		TFirm firm;
		mstr m;

		for (int index = 0; index < sideWinds->Count(); index++)
		{
			TSideWind *sideWind = sideWinds->SideWindAt(index);

			if (strchr(UCN_FIRMS, sideWind->ucnType))
			{
				firm.ucnType = sideWind->ucnType;
				strcpy(firm.ucn, sideWind->ucn);

				if (firm.Get() && !strcmp(firm.country, BUILDIN_COUNTRY) && !any(firm.bulstat))
					m.printf("%s няма въведен ЕИК/БУЛСТАТ\n", firm.name);
			}
		}

		if (any(m))
			return error("%s\nЗа производствата по несъстоятелност данните за ЕИК/БУЛСТАТ на страните юридически лица са задължителни.", str(m));
	}
#endif  // AREAL
	return TSideWindListDialog::CanClose();
}

void TOriginSideListDialog::StateBNEnable(TCommandEnabler &tce)
{
	TSideWind *sideWind = sideWinds->SideWind();
	tce.Enable(statable && sideWind && container->Defendant(sideWind->involvement));
}

void TOriginSideListDialog::StateBNClicked()
{
	TSideWind *sideWind = sideWinds->SideWind();
	char tState = sideWind->state;

	sideWind->state = tState == STATE_NONE ? STATE_HOLD : STATE_NONE;

	if (sideWind->Update("F_STATE"))
		sideWinds->RedrawGroup();
	else
		sideWind->state = tState;
}

void TOriginSideListDialog::MoneyBNClicked()
{
	sideWinds->Expend(NULL, NULL);
}
