DEFINE_RESPONSE_TABLE1(TAttachExtraDialog, TTroubleDialog)
//{{TAttachExtraDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TAttachExtraDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAttachExtraDialog Implementation}}

TAttachExtraDialog::TAttachExtraDialog(TWindow* parent, TElectric *electric)
:
	TTroubleDialog(parent, electric, electric->extraNo != 0, TYPE_IMAGE, IDD_ATTACH_EXTRA)
{
	descript = new TStringFace(this, IDC_ATTACH_EXTRA_DESCRIPT, "F_DESCRIPT", SIZE_OF_ELECTRIC_DESCRIPT);
	trouble.primary = false;
}

bool TAttachExtraDialog::Move(int type)
{
	TElectric *electric = Electric();
	const char *name = trouble.name[type]->S();

	if (electric_move(this, electric, electric->date, name, type, NULL, electric->extraNo))
	{
		electric_name(electric, electric->extraName, type, electric->extraNo);
		strcpy(file_ext(electric->extraName), file_ext(name));
		electric->extraType = type;
		return true;
	}

	return false;
}

bool TAttachExtraDialog::IsValid()
{
	bool isValid = TTroubleDialog::IsValid();

	if (isValid)
	{
		unsigned count = 0;
		int type;

		for (int tType = 0; tType < TROUBLE_COUNT; tType++)
		{
			if (*trouble.name[tType]->S())
			{
				type = tType;
				count++;
			}
		}

		if (!count)
			isValid = error("Името на документа е задължително.");
		else if (count >= 2)
			isValid = error("Зададени са имена за повече от един тип документ.");
		else if (!any(descript))
			isValid = error("Описанието на документа е задължително.");
		else
		{
			const char *name = trouble.name[type]->S();

			if (edit && strcmpi(file_ext(name), file_ext(Electric()->extraName)))
				isValid = error("При редактиране на документ, разширението не може да бъде променяно.");
			else if (!electric_matches(name, type, true, false))
				isValid = error("Разширението на файла не е от разрешените типове.");
		}
	}

	return isValid;
}

void TAttachExtraDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();

	while (trouble.type != Electric()->extraType)
		trouble.Swap();

	if (edit)
	{
		trouble.name[trouble.type]->SetS(Electric()->extraName);
		trouble.swap->EnableWindow(false);
	}
}

void TAttachExtraDialog::OKBNClicked()
{
	if (IsValid())
	{
		TElectric *electric = Electric();
		int type;

		*electric << this;

		for (type = 0; type < TROUBLE_COUNT; type++)
			if (*trouble.name[type]->S())
				break;

		if (edit)
		{
			bool exist = electric->Try("F_EXTRA_NO");

			if (Move(type) && electric->Save(exist))
				CmOk();
		}
		else if (electric->Insert())	// unlikely
		{
			if (!Move(type))
				electric->Delete(true);
		}

		CmOk();
	}
}
