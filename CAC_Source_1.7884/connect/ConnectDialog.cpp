#include "cac.h"

DEFINE_RESPONSE_TABLE1(TConnectDialog, TDialog)
//{{TConnectDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_CBN_SELCHANGE(IDC_CONNECT_ALIAS, AliasCBNSelChange),
//{{TConnectDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TConnectDialog Implementation}}

TConnectDialog::TConnectDialog(TWindow* parent, const TCACTarget *tTarget, int tVersion)
:
	TDialog(parent, IDD_CONNECT), target(tTarget), version(tVersion), duplicates(false)
{
	mstr prev;

	for (int index = 0; index < ALIAS_COUNT; index++)
	{
		int baseType = index % DBT_COUNT;
		mstr &m = tNames[index];

		if (baseType)
			m.cat("CAC_");

		m.cat(index < DBT_COUNT * TARGET_STEP ? "REGCOURT" : target->BaseAlias);

		if ((index / DBT_COUNT) % TARGET_STEP)
			itom(version, m);

		Connects.Add(NewConnect(str(m), index % DBT_COUNT));

		if (!duplicates)
		{
			if (m.cmp(prev))
				prev.cpy(m);
			else
				duplicates = true;
		}
	}

	alias = new TComboBox(this, IDC_CONNECT_ALIAS, ALIAS_SIZE);
	value = new TEdit(this, IDC_CONNECT_VALUE, DBIMAXSCFLDLEN);
}

TConnectDialog::~TConnectDialog()
{
	Destroy(IDCANCEL);
}

TCACConnect *TConnectDialog::NewConnect(const char *name, int baseType)
{
	TCACConnect *connect = NULL;

	switch (baseType)
	{
		case DBT_BDE_INTERBASE : connect = new TBDEConnect(name); break;
		case DBT_ODBC_FIREBIRD : connect = new TFireBirdConnect(name); break;
		case DBT_ODBC_POSTGRES : connect = new TPostGresConnect(name); break;
		default : fatal("NewConnect: %d: unknown data base type", baseType);
	}

	return connect;
}

void TConnectDialog::Load(int index, char *value)
{
	if (!Connects[index]->Load(value))
		strcpy(value, index ? "" : "C:\\Program Files\\IS - Varna\\Lawsuit Management\\AppName.gdb");
}

void TConnectDialog::Commit(TCACConnect *connect, int i)
{
	TWaitWindow wait(this);

	value->Transfer(tValue, tdGetData);

	if (connect->Save(tValue))
	{
		TCACDataBase *dataBase = Connects[i]->NewDataBase();
		bool connected = dataBase->DefaultConnect(ET_ERROR);

		if (connected)
		{
			wait.HideWindow();

			int tVersion = target->GetVersion(dataBase);

			if (version)
			{
				if (tVersion != version)
					error("Версиите на клиента и сървъра на %s не съвпадат!", AppNameName);
				else
				{
					info("САС сървър за %s съд, версия 1.%d.\n\nДиректория за временни файлове:\n%s",
						target->UserName, version, tempdir);
					CmOk();
				}
			}
		}

		delete dataBase;
	}
}

bool TConnectDialog::IsValid()
{
	int sel = alias->GetSelIndex();

	if (sel < 0)
		return error("Не е избран елемент.");	// shoudn't happen (tm)

	const char *syntax;

	value->Transfer(tValue, tdGetData);

	if (any(tValue) && (syntax = Connects[sel]->SyntaxError(tValue)) != NULL)
		return error("%s.", syntax);

	return true;
}

void TConnectDialog::SetupWindow()
{
	TDialog::SetupWindow();

	for (int index = 0; index < ALIAS_COUNT; index++)
	{
		TCACConnect *connect = Connects[index];
		mstr m(connect->Name);

		if (duplicates)
		{
			TCACDataBase *dataBase = connect->NewDataBase();
			m.printf(" (%s, %s)", dataBase->Protocol(), dataBase->BaseType());
			delete dataBase;
		}

		alias->AddString(str(m));
		Load(index, tValue);

		if (any(tValue))
		{
			alias->SetSelIndex(index);
			value->Transfer(tValue, tdSetData);
		}
	}
}

void TConnectDialog::OKBNClicked()
{
	if (IsValid())
	{
		int sel = alias->GetSelIndex();
		TCACConnect *connect = Connects[sel];

		value->Transfer(tValue, tdGetData);

		if (!any(tValue))
		{
			if (message(MB_VERIFY, "Адресът е празен. Желаете ли да изтриете името %s на тази работна станция?",
				connect->Name))
			{
				connect->Delete(tValue);
			}
		}
		else
		{
			mstr m;

			connect->SyntaxWarn(tValue, m);

			if (sel >= DBT_COUNT)
			{
				m.sep(".\n\n");
				m.printf("Името %s би трябвало да се използва само ако работите с две или повече версии на САС",
					connect->Name);
			}

			for (int index = ALIAS_COUNT - 1; index > sel; index--)
			{
				char tNoValue[DBIMAXSCFLDLEN];

				if (Connects[index]->Load(tNoValue))
				{
					m.sep(".\n\n");
					m.printf("Името %s е дефинирано, и ще се използва от САС дори да потвърдите %s",
						Connects[index]->Name, connect->Name);
					break;
				}
			}

			if (!any(m) || warn("%s.\n\nЖелаете ли да продължите?", str(m)))
				Commit(connect, sel);
		}
	}
}

void TConnectDialog::AliasCBNSelChange()
{
	int sel = alias->GetSelIndex();

	if (sel >= 0)
	{
		Load(sel, tValue);
		value->Transfer(tValue, tdSetData);
	}
}
