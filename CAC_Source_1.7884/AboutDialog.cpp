#include "cac.h"

DEFINE_RESPONSE_TABLE1(TAboutDialog, TScaleDialog)
//{{TAboutDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDQUERY, QueryBNClicked),
//{{TAboutDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAboutDialog Implementation}}

TAboutDialog::TAboutDialog(TWindow* parent)
:
	TScaleDialog(parent, IDD_ABOUT)
{
	name = new TStatic(this, IDC_ABOUT_NAME);
}

void TAboutDialog::SetupWindow()
{
	TScaleDialog::SetupWindow();

	mstr m("Потребител: ");
	Default->Print(m);
	name->SetCaption(str(m));
}

void TAboutDialog::QueryBNClicked()
{
	mstr m;
	unsigned icon;

	m.printf("Мащабиране: %ld %s.\n", FONT_SCALE, FONT_SCALE ? "пункта" : "(няма)");
	m.cat(SetThemeAppProperties ? "Поддържат се интерфейсни теми.\n" : "");
	m.cat(EnableThemeDialogTexture ? "Поддържат се диалогови текстури.\n" : "");
	m.printf("Основна база данни: %s.\n", TCACDataBase::DefaultDB->BaseType());
	m.printf("Библиотека за връзка: %s.\n", TCACDataBase::DefaultDB->Protocol());
	m.cat(COURT_FLAT ? "Плоски състави.\n" : "");	// 2009:335
	m.cat(can_surrouendoc() ? "Възражения с отделни актове.\n" : "");
	m.printf("Потребители: %d, флагове: %d.\n", CONFIG_MAX, COURT_FLAT + COURT_WRIT + COURT_N10K +
		(COURT_MINI ? 8 : 0) + COURT_LOCK);

	if (!strcmp(tempdir, TEMPDIR))
	{
		m.cat("\nВНИМАНИЕ! Използва се локалната директория за временни файлове.");
		icon = MB_ICONWARNING;
	}
	else
	{
		m.printf("Директория за временни файлове: %s", tempdir);
		icon = MB_ICONINFORMATION;
	}

	message(icon | MB_OK, str(m));
}

void TAboutDialog::OKBNClicked()
{
	CmOk();
}
