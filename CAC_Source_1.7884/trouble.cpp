#include "cac.h"

TElectricTrouble::TElectricTrouble(TScaleDialog *tParent, int tType, bool tExtraDB)
	: parent(tParent), type(tType), act(NULL), primary(true), portreg(false)
{
	swap = new TButton(parent, IDSWAP);
	name[TYPE_TEXT] = new TStringFace(parent, IDC_TROUBLE_TEXT_NAME, "F_TEXT_NAME", SIZE_OF_PATH);
	name[TYPE_IMAGE] = new TStringFace(parent, IDC_TROUBLE_IMAGE_NAME, "F_IMAGE_NAME", SIZE_OF_PATH);
	extraDB = tExtraDB && can_extra_db();
	attach = extraDB ? (TButton *) new TTwinButton(parent, IDATTACH, IDRIGHT) : new TButton(parent, IDATTACH);
}

TElectricTrouble::~TElectricTrouble()
{
	delete act;
}

void TElectricTrouble::BackupNames()
{
	for (int type = 0; type < TROUBLE_COUNT; type++)
		strcpy(backup[type], name[type]->S());
}

void TElectricTrouble::Enable(bool enable)
{
	if (enable != swap->IsWindowEnabled())
	{
		attach->EnableWindow(enable);
		swap->EnableWindow(enable);

		for (int type = 0; type < TROUBLE_COUNT; type++)
		{
			name[type]->EnableWindow(enable);
			name[type]->SetS(backup[type]);
		}
	}
}

void TElectricTrouble::ActChanged()
{
	attach->SetCaption(act ? "Показване" : "Присъединяване");
	attach->EnableWindow(!act || *name[type]->S());

	for (int type = 0; type < TROUBLE_COUNT; type++)
	{
		if (act)
			name[type]->EnableWindow(false);
		else
		{
			name[type]->SetS(backup[type]);
			name[type]->EnableWindow(true);
		}
	}
}

void TElectricTrouble::ClearAct()
{
	delete act;
	act = NULL;
}

void TElectricTrouble::IndexChanged(char indexKind)
{
	if (act && indexKind != act->kind)
	{
		ClearAct();
		ActChanged();
	}
}

void TElectricTrouble::Swap()
{
	mstr m;

	type = (type + 1) % TROUBLE_COUNT;
	attach->EnableWindow(!act || *name[type]->S());

	for (int tType = 0; tType < TROUBLE_COUNT; tType++)
		name[tType]->ShowWindow(tType == type ? SW_SHOW : SW_HIDE);

	m.printf("Име на %s документ", Electrics[type].awhat);
	swap->SetCaption(str(m));
}

void TElectricTrouble::Name(const TRCDKeyContainer *container, const CDate &date)
{
	for (int type = 0; type < TROUBLE_COUNT; type++)
		electric_name(container, date, name[type], type);
}

bool TElectricTrouble::Move(const TRCDKeyContainer *container, const CDate &date)
{
	for (int type = 0; type < TROUBLE_COUNT; type++)
	{
		const char *tName = name[type]->S();

		if (!electric_move(parent, container, date, tName, FxType(type), act))
			return false;

		if (any(tName))
			electric_name(container, date, name[type], FxType(type));
	}

	return true;
}

void TElectricTrouble::PostMove(const TRCDKeyContainer *container, const CDate &date, char targetKind)
{
	if (act)
	{
		CDateTime::SyncServer(SYNC_PRECISE);
		act->targetStamp = _CurrentDateTime;

		if (act->ReWriteFiles() && Move(container, date))
		{
			for (int type = 0; type < TROUBLE_COUNT; type++)
			{
				act->mtime[type] = 0;
				mbk_clear(act->fileData + type);
			}

			act->target = container->key;
			act->targetKind = targetKind ? targetKind : container->kind;
			act->targetDate = container->date;
			act->Update();
		}
	}
	else
		Move(container, date);
}

void TElectricTrouble::SetPortreg(const TRCDKeyContainer *container, const CDate &date, bool tPortreg, bool edit)
{
	if (SIGN_PDFS && tPortreg != portreg)
	{
		portreg = tPortreg;

		if (strcmpi(Electrics[TYPE_IMAGE].ext[0].type, PORTABLE_TYPE))
		{
			name[TYPE_IMAGE]->SetS("");

			if (edit)
				electric_name(container, date, name[TYPE_IMAGE], FxType(TYPE_IMAGE));

			strcpy(backup[TYPE_IMAGE], name[TYPE_IMAGE]->S());
		}
	}
}

void TElectricTrouble::Edit(bool update)
{
	if (act)
		attach_view(parent, name[type]->S(), type);
	else
		electric_edit(parent, name[type], update, FxType(type), primary);
}

bool TElectricTrouble::IsValid()
{
	if (!portreg)
		return true;

	const char *tName = name[TYPE_IMAGE]->S();

	if (!any(tName))
		return error("Липсва присъединен %s файл.", PORTABLE_TYPE);

	TWaitWindow *wait = new TWaitWindow(parent, IDD_WAIT_HIDDEN);
	char *const command = new char[COMMAND_LINE_SIZE];
	PROCESS_INFORMATION procInfo;
	TCharPtr CP(command);

	wait->SetText("Проверка на цифровия подпис...");
	wait->ShowWindow(SW_SHOWNOACTIVATE);
	*command = '\0';
	command_record(command, SIGN_TEST);
	command_record(command, tName);

	if (command_launch(command, procInfo))
	{
		WaitForInputIdle(procInfo.hProcess, CHROME_WAIT_FOR_IDLE * 1000);
		wait->ShowWindow(SW_HIDE);

		int result = command_waitfor(procInfo.hProcess);
		SetForegroundWindow(parent->HWindow);
		delete wait;

		switch (result)
		{
			case 0 : return true;
			case -1 : error("%s: грешка %lu при изчакване.", SIGN_TEST, GETLASTERROR);
			default : return false;
		}
	}
	else
	{
		delete wait;
		return error("%s: грешка %lu при стартиране.", SIGN_TEST, GETLASTERROR);
	}
}

int TElectricTrouble::FxType(int type)
{
	return type == TYPE_IMAGE && portreg ? TYPE_PORTREG : type;
}
