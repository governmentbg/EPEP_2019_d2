TNTOpenSaveDialog::TNTOpenSaveDialog(TWindow* parent, TData& data, bool tOpen, int templateId, const char far* title,
	TModule* module)
	: TOpenSaveDialog(parent, data, templateId, title, module), open(tOpen)
{
}

int TNTOpenSaveDialog::DoExecute()
{
	ofn.lpfnHook = CHROME_CENTER_OPEN_SAVE ? OFNHookProc : LPOFNHOOKPROC(StdDlgProc);
#if 20170299
	if (open ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn))
	{
		Data.Flags = ofn.Flags;
		Data.FilterIndex = ofn.nFilterIndex;
		Data.Error = 0;
		return IDOK;
	}
	else
	{
		Data.Error = ::CommDlgExtendedError();
		return IDCANCEL;
	}
#else  // 20170299
	int ret = open ? GetOpenFileName(&ofn) : GetSaveFileName(&ofn);
	if (ret) {
		Data.Flags = ofn.Flags;
		Data.FilterIndex = ofn.nFilterIndex;
		Data.Error = 0;
	}
	else {
		Data.Error = ::CommDlgExtendedError();
	}
	return ret ? IDOK : IDCANCEL;
#endif  // 20170299
}
