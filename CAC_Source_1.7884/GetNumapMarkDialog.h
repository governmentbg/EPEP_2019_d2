//{{TDialog = TGetNumapMarkDialog}}
class TGetNumapMarkDialog : public TGroupDialog
{
public:
	TGetNumapMarkDialog(TWindow* parent, TNumapMarkGroup *group, int resId = IDD_GET_NUMAP_MARK);

protected:
	TLongFace *start;
	TLongFace *final;

	virtual bool IsValid();
};    //{{TGetNumapMarkDialog}}
