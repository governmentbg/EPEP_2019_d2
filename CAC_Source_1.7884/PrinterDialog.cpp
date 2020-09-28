DEFINE_RESPONSE_TABLE1(TPrinterDialog, TGroupDialog)
//{{TPrinterDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDC_PRINTER_WINDOWS, WindowsBNClicked),
//{{TPrinterDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TPrinterDialog Implementation}}

TPrinterDialog::TPrinterDialog(TWindow* parent, TPrinterGroup *group, int resId)
:
	TGroupDialog(parent, group, resId)
{
	windows = new TCheckFace(this, IDC_PRINTER_WINDOWS, "WINDOWS");
	pagelen = new TLongFace(this, IDC_PRINTER_PAGE_LEN, "PAGE_LEN", 3 + 1);

	port = new TLongFace(this, IDC_PRINTER_PORT, "PORT", 1 + 1);
	cp1251 = new TCheckFace(this, IDC_PRINTER_CP1251, "CP1251");
	check = new TCheckFace(this, IDC_PRINTER_CHECK, "CHECK");
	pseudo = new TCheckFace(this, IDC_PRINTER_PSEUDO, "PSEUDO");

	setupex = new TCheckFace(this, IDC_PRINTER_SETUPEX, "SETUPEX");
	justify = new TLongFace(this, IDC_PRINTER_JUSTIFY, "JUSTIFY", 4 + 1);
	alignto = new TLongFace(this, IDC_PRINTER_ALIGN_TO, "ALIGN_TO", 1 + 1);
	linewidth = new TLongFace(this, IDC_PRINTER_LINE_WIDTH, "LINE_WIDTH", 3 + 1);
}

void TPrinterDialog::OKBNClicked()
{
	if (IsValid())
	{
		*Group() << this;
		CmOk();
	}
}

void TPrinterDialog::SetupWindow()
{
	TGroupDialog::SetupWindow();
	WindowsBNClicked();
}

bool TPrinterDialog::IsValid()
{
	bool isValid = TGroupDialog::IsValid();

	if (isValid)
	{
		if (pagelen->L() < 30 || pagelen->L() > 240)
			isValid = error("Невалиден брой редове на страница.");
		else if (port->L() > 3)
			isValid = error("Невалиден порт.");
		else if (justify->L() > 2400)
			isValid = error("Отстъпа за печат е невалиден.");
		else if (alignto->L() > FLOAT_ALIGNMENT)
			isValid = error("Посоката на отстъпа за печат е невалидна.");
		else if (linewidth->L() > 720)
			isValid = error("Ширината на линията за печат е невалидна.");
	}

	return isValid;
}

void TPrinterDialog::WindowsBNClicked()
{
	bool isWindows = windows->GetCheck() == BF_CHECKED;

	port->EnableWindow(!isWindows);
	cp1251->EnableWindow(!isWindows);
	check->EnableWindow(!isWindows);
	pseudo->EnableWindow(!isWindows);

	setupex->EnableWindow(isWindows);
	justify->EnableWindow(isWindows);
	alignto->EnableWindow(isWindows);
	linewidth->EnableWindow(isWindows);
}
