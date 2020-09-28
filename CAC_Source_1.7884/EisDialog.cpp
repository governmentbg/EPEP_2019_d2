DEFINE_RESPONSE_TABLE1(TEisDialog, TEditDialog)
//{{TEisDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TEisDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TEisDialog Implementation}}

TEisDialog::TEisDialog(TWindow* parent, TXMLGroup *group, sign edit, const char *tNmrnpr, int resId)
:
	TEditDialog(parent, group, edit != false, resId), save(edit != -1), nmrnpr(tNmrnpr)
{
	nprnmr = new TEisNmrFace(this, IDC_EIS_NPRNMR, "F_NPRNMR");
}

void TEisDialog::EnableEis(TWindow *window, bool enable)
{
	window->EnableWindow(enable);
	SetupEis(window, this);
}

bool TEisDialog::WrongEis(TWindow *window, void *data)
{
	const char *name;
	TEisDialog *dialog = (TEisDialog *) data;

	if (window->SendMessage(WM_FACE_NAME, 0, (LPARAM) &name) == MW_NAMED_FACE && strncmp(name, "F_", 2))
	{
		TXMLAttrib *attrib = dialog->XMLGroup()->FindAttrib(name);
		msql m;

		if (attrib && !attrib->IsValid(m))
		{
			error("%s.", str(m));
			return true;
		}
	}

	return false;
}

void TEisDialog::SetupEis(TWindow *window, void *data)
{
	const char *name;
	TEisDialog *dialog = (TEisDialog *) data;

	if (window->SendMessage(WM_FACE_NAME, 0, (LPARAM) &name) == MW_NAMED_FACE && strncmp(name, "F_", 2))
	{
		TXMLAttrib *attrib = dialog->XMLGroup()->FindAttrib(name);

#if RANGECHECK
		if (!attrib && dialog->XMLGroup()->FindObject(name))
			fatal("interface xml object: %s", name);
#endif  // RANGECHECK

		// 2012:271 LPR: assume enabled by default
		if (!attrib || !(attrib->flags & XML_EDITABLE))
			window->EnableWindow(false);
	}
}

bool TEisDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		mstr m;

		*Group() << this;

		if (FirstThat(WrongEis, this))
			isValid = false;
	}

	return isValid;
}

void TEisDialog::SetupWindow()
{
	TEditDialog::SetupWindow();
	ForEach(SetupEis, this);

	if (nmrnpr)
		nprnmr->SetS(nmrnpr);
}

void TEisDialog::OKBNClicked()
{
	if (save)
		TEditDialog::OKBNClicked();
	else
		TGroupDialog::OKBNClicked();
}
