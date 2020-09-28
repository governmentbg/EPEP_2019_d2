//{{TDialog = TFontTestDialog}}
class TFontTestDialog : public TScaleDialog
{
public:
	TFontTestDialog(TWindow* parent, int resId);

	TFontScale GetFontScale();

protected:
	TComboBox *over;
	TEdit *orig;
	TEdit *text;
};    //{{TFontTestDialog}}
