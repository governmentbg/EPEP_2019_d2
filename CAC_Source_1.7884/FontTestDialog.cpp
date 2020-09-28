TFontTestDialog::TFontTestDialog(TWindow* parent, int resId)
	: TScaleDialog(parent, resId)
{
	dlgScale.fsOrig = 0;
	orig = new TEdit(this, IDC_TEST_FONT_ORIG);
	over = new TComboBox(this, IDC_TEST_FONT_OVER);
	text = new TEdit(this, IDC_TEST_FONT_TEXT);
}

TFontScale TFontTestDialog::GetFontScale()
{
	dlgScale.fsOrig = orig->GetWindowRect().Height();
	dlgScale.fsOver = over->GetWindowRect().Height();
	dlgScale.fsText = text->GetWindowRect().Height();

	return dlgScale;
}
