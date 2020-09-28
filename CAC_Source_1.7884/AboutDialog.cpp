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

	mstr m("����������: ");
	Default->Print(m);
	name->SetCaption(str(m));
}

void TAboutDialog::QueryBNClicked()
{
	mstr m;
	unsigned icon;

	m.printf("����������: %ld %s.\n", FONT_SCALE, FONT_SCALE ? "������" : "(����)");
	m.cat(SetThemeAppProperties ? "��������� �� ����������� ����.\n" : "");
	m.cat(EnableThemeDialogTexture ? "��������� �� ��������� ��������.\n" : "");
	m.printf("������� ���� �����: %s.\n", TCACDataBase::DefaultDB->BaseType());
	m.printf("���������� �� ������: %s.\n", TCACDataBase::DefaultDB->Protocol());
	m.cat(COURT_FLAT ? "������ �������.\n" : "");	// 2009:335
	m.cat(can_surrouendoc() ? "���������� � ������� ������.\n" : "");
	m.printf("�����������: %d, �������: %d.\n", CONFIG_MAX, COURT_FLAT + COURT_WRIT + COURT_N10K +
		(COURT_MINI ? 8 : 0) + COURT_LOCK);

	if (!strcmp(tempdir, TEMPDIR))
	{
		m.cat("\n��������! �������� �� ��������� ���������� �� �������� �������.");
		icon = MB_ICONWARNING;
	}
	else
	{
		m.printf("���������� �� �������� �������: %s", tempdir);
		icon = MB_ICONINFORMATION;
	}

	message(icon | MB_OK, str(m));
}

void TAboutDialog::OKBNClicked()
{
	CmOk();
}
