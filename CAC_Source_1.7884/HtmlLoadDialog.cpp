DEFINE_RESPONSE_TABLE1(THtmlLoadDialog, THtmlBaseDialog)
//{{THtmlLoadDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_HTML_LOAD_SET_SESSION, SetSessionBNClicked),
	EV_BN_CLICKED(IDC_HTML_LOAD_VARIANT, VariantBNClicked),
	EV_BN_KILLFOCUS(IDC_HTML_LOAD_VARIANT, BrowserBNKillFocus),
	EV_BN_KILLFOCUS(IDC_HTML_LOAD_SET_SESSION, BrowserBNKillFocus),
//{{THtmlLoadDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{THtmlLoadDialog Implementation}}

THtmlLoadDialog::THtmlLoadDialog(TWindow* parent, TLoadGroup *tLoadGroup, TLoadGroup *tInvisible, int resId)
:
	THtmlBaseDialog(parent, SHOW_ADJUSTED, tLoadGroup->names[0], true, resId), loadGroup(tLoadGroup),
		invisible(tInvisible), variant(0)
{
	autoDown[0] = new TButton(this, IDC_HTML_LOAD_SET_SESSION);
	autoDown[1] = new TButton(this, IDPRINT);
	autoDown[2] = new TButton(this, IDVIEW);
	autoDown[3] = new TButton(this, IDEDIT);
	autoDown[4] = new TButton(this, IDSAVE);
	autoDown[5] = new TButton(this, IDCANCEL);
	variantBN = new TButton(this, IDC_HTML_LOAD_VARIANT);
}

THtmlLoadDialog::~THtmlLoadDialog()
{
	remove_file(loadGroup->names[variant ^ 1], NULL, true);
}

HRESULT THtmlLoadDialog::CallBack(const char *url)
{
	TLawsuit lawsuit;
	TSession session;
	unsigned bef;
	int n;

	if ((url = strrchr(url, '#')) != NULL && url[1] == 's' && sscanf(url + 2, "%02d%02d%02d%02d%01u%n", &session.date.year,
		&session.date.month, &session.date.day, &session.hall, &bef, &n) == 5 && bef <= 2 && n == strlen(url + 2))
	{
		session.date.year += YEAR_MIN;

		if (session.date.Valid())
		{
			for (int i = 0; i < loadGroup->halls.Count(); i++)
			{
				if (loadGroup->halls[i] == session.hall)
				{
					session.time.hour = bef == TRUE ? 8 : bef == FALSE ? 12 : 0;
					SetSession(session);
					break;
				}
			}
		}
	}

	return S_OK;
}

void THtmlLoadDialog::SetSession(TSession &session)
{
	TLawsuit lawsuit;

	if (get_group(this, &lawsuit, TYPE_LAWSUITS, NULL) && set_session(this, &session, &lawsuit, loadGroup, invisible))
	{
		loadGroup->AddSession(session);
		loadGroup->FixupUnits();

		if (loadGroup->Generate())
			RefreshBNClicked();
	}
}

void THtmlLoadDialog::UpdateVariantText()
{
	mstr m("Разпределение на откритите заседания - Вариант ");
	itom(variant + 1, m);
	variantBN->SetCaption(str(m));
}

void THtmlLoadDialog::SwitchVariant()
{
	variant ^= 1;
	strcpy(name, loadGroup->names[variant]);
}

void THtmlLoadDialog::SetupWindow()
{
	THtmlBaseDialog::SetupWindow();
	PostMessage(WM_FOCUS_FACE);
	UpdateVariantText();
}

void THtmlLoadDialog::SetSessionBNClicked()
{
	TSession session;

	session.date = Today;
	session.time.hour = 8;
	SetSession(session);
}

void THtmlLoadDialog::VariantBNClicked()
{
	SwitchVariant();

	if (Navigate(name))
		UpdateVariantText();
	else
		SwitchVariant();
}
