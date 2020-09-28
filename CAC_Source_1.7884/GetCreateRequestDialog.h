//{{TDialog = TGetCreateRequestDialog}}
class TGetCreateRequestDialog : public TGetGroupDialog
{
public:
	TGetCreateRequestDialog(TWindow* parent, TRequest *request, int resId = 0);

protected:
	virtual bool IsValid();
};    //{{TGetCreateRequestDialog}}
