//{{TDialog = TProveBookDialog}}
class TProveBookDialog : public TBookDialog
{
public:
	TProveBookDialog(TWindow* parent, TProveBookGroup *group, int resId = IDD_BOOK_PROVE);

protected:
	TCheckFace *newFormat;
};    //{{TProveBookDialog}}