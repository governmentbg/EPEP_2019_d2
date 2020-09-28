#include "cac.h"

DEFINE_RESPONSE_TABLE1(TSideWindListDialog, TGroupListDialog)
//{{TSideWindListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDDELETE, DeleteBNEnable),
	EV_COMMAND_ENABLE(IDLINKS, LinksBNEnable),
	EV_COMMAND_ENABLE(IDMESSAGES, SelectEnable),
	EV_BN_CLICKED(IDLINKS, LinksBNClicked),
	EV_BN_CLICKED(IDCANCEL, CancelBNClicked),
	EV_WM_CLOSE,
//{{TSideWindListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSideWindListDialog Implementation}}

TSideWindListDialog::TSideWindListDialog(TWindow* parent, TRCDKeyContainer *tContainer, TLawsuit *lawsuit, long tSideMode,
	const char *tWhat, int resId, TSideWindListCreator createSideWinds)
:
	TGroupListDialog(parent, resId), container(tContainer), sideMode(tSideMode), edit(tSideMode & SIDE_MODE_EDIT),
		what(tWhat), primaries("страни"), child(NULL), lastTicks(0)
{
	// derived VM-s are not available in ctor, so use function pointer 
	sideWinds = createSideWinds ? createSideWinds(this, sideMode, container, lawsuit) :
		new TSideWindListWindow(this, sideMode, IDC_GROUP_LIST_WINDOW, container, lawsuit);
	sideLabel = new TStatic(this, IDC_SIDE_WIND_LIST_LABEL);
	header1 = new TStatic(this, IDC_SIDE_WIND_HEADER_1);
	header2 = new TStatic(this, IDC_SIDE_WIND_HEADER_2);
}

void TSideWindListDialog::Header1(mstr &m)
{
	m.printf("%s %ld/%ldг.", sideMode & SIDE_LIST_KIND ? Kind->Seek(container->kind) : Type->Seek(container->key.type),
		container->key.no, container->key.year);

	if (sideMode & SIDE_LIST_DATE)
	{
		container->date.Print(m, " от %Dг.");
		TTime *time = (TTime *) container->Find("F_TIME");
		if (time)
			time->Time().Print(m, ", %T часа");
	}
}

void TSideWindListDialog::Header2(mstr &)
{
}

const char *TSideWindListDialog::LackingPrimary() const
{
	return PrimaryCount('\0') >= 1 ? NULL : primaries;
}

bool TSideWindListDialog::PrimaryEnable()
{
	TSideWind *sideWind = sideWinds->SideWind();
	return sideWind && (!edit || !Primary(sideWind) || PrimaryCount(sideWind->involvement) > 1);
}

bool TSideWindListDialog::Primary(TSideWind *) const
{
	return true;
}

int TSideWindListDialog::PrimaryCount(char) const
{
	int count = 0;

	for (int index = 0; index < sideWinds->Count(); index++)
		if (Primary(sideWinds->SideWindAt(index)))
			count++;

	return count;
}

void TSideWindListDialog::Accept()
{
}

void TSideWindListDialog::Reject()
{
}

bool TSideWindListDialog::CanClose()
{
	bool result = TGroupListDialog::CanClose();

	if (result)
	{
		if (edit)
		{
			const char *lacking = LackingPrimary();

			if (lacking)
				result = error("Няма %s.", lacking);
		}
	}

	return result;
}

void TSideWindListDialog::SetupWindow()
{
	TGroupListDialog::SetupWindow();

	mstr m;

	Header1(m);
	if (any(m))
	{
		header1->SetText(str(m));
		m.clear();
	}

	Header2(m);
	if (any(m))
		header2->SetText(str(m));
}

void TSideWindListDialog::DeleteBNEnable(TCommandEnabler &tce)
{
	tce.Enable(can_func(RIGHT_KILL_SIDE) && PrimaryEnable());
}

void TSideWindListDialog::LinksBNClicked()
{
	sideWinds->Links();
}

void TSideWindListDialog::LinksBNEnable(TCommandEnabler &tce)
{
	tce.Enable(sideWinds->Count() >= 2);
}

void TSideWindListDialog::CancelBNClicked()
{
	EvClose();	// EvClose() is not virtual and must be reinvoked
}

void TSideWindListDialog::ChildBNEnable(TCommandEnabler &tce)
{
	TSideWind *sideWind = sideWinds->SideWind();
	// 2016:314 LPR: float young
	//tce.Enable(statable && sideWind && strchr(UCN_CITIZENS, sideWind->ucnType));
	tce.Enable(sideWind && strchr(TYPE_ORIGINS, sideWind->key.type) && strchr(UCN_CITIZENS, sideWind->ucnType));
}

void TSideWindListDialog::ChildBNClicked()
{
	if (GetTickCount() > lastTicks + 100)
	{
		TRect rect = child->GetWindowRect();
		uint flags = TPM_CENTERALIGN | TPM_TOPALIGN | TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON;
		TPopupMenu menu;

		menu.AppendMenu(MF_STRING, SIDE_WIND_MINOR, TSideWind::ChildText(SIDE_WIND_MINOR));
		menu.AppendMenu(MF_STRING, SIDE_WIND_INFANT, TSideWind::ChildText(SIDE_WIND_INFANT));
		menu.AppendMenu(MF_STRING, SIDE_WIND_ADMNU, TSideWind::ChildText(SIDE_WIND_ADMNU));

		int command = TrackPopupMenu(menu.GetHandle(), flags, (rect.left + rect.right) / 2,
			rect.bottom, 0, Handle, NULL);

		sideWinds->Child(command);
		lastTicks = GetTickCount();
	}
}

void TSideWindListDialog::EvClose()
{
	if (CanClose())
	{
		const char *lacking = LackingPrimary();

		if (!lacking)
			Accept();
		else if (warn("Няма %s.\n\nДа изтрия ли %s?", lacking, what))
		{
			if (delete_incomplete(container))
				Reject();
			else
				error("Грешка при изтриване на %s.", what);
		}
		else
			return;

		TGroupListDialog::EvClose();
	}
}
