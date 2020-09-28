//{{TDialog = TOutRegBookDialog}}
class TOutRegBookDialog : public TBookDialog
{
public:
	TOutRegBookDialog(TWindow* parent, TOutRegBookGroup *group, int resId = IDD_OUTREG_BOOK);

protected:
	TCheckFace *newFormat;
};    //{{TOutRegBookDialog}}
