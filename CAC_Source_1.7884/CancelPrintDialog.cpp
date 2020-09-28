//{{TCancelPrintDialog Implementation}}

TCancelPrintDialog::TCancelPrintDialog(TWindow* parent, const char *tTitle, int resId)
:
	TScaleDialog(parent, resId), count(0), title(tTitle)
{
	pages = new TStatic(this, IDC_CANCEL_PRINT_PAGES);
	name = new TStatic(this, IDC_CANCEL_PRINT_NAME);
	printer = new TStatic(this, IDC_CANCEL_PRINT_PRINTER);
}

void TCancelPrintDialog::SetupWindow()
{
	TScaleDialog::SetupWindow();

	mstr m;
	m.printf("от %s", title);
	name->SetCaption(str(m));
}

void TCancelPrintDialog::SetPageNumber(int page)
{
	mstr m;
	m.printf("Печат на страница %d/%d", page, count);
	pages->SetCaption(str(m));
}

void TCancelPrintDialog::SetPrinterName(const char *name)
{
	mstr m;
	m.printf("на %s", name);
	printer->SetCaption(str(m));
}
