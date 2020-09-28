//{{TDialog = TFloatImportantDialog}}
class TFloatImportantDialog : public TFloatDialog
{
public:
	TFloatImportantDialog(TWindow* parent, TFloatImportantGroup *group, int resId = IDD_FLOAT_IMPORTANT);

protected:
	TTypeListFace *types;
	TDateFace *start;
	TDateFace *final;

	virtual bool IsValid();
};    //{{TFloatImportantDialog}}
