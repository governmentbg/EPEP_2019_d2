//{{TListBox = TXferDocumentListBox}}
class TXferDocumentListBox : public TCloneListBox
{
public:
	TXferDocumentListBox(TWindow* parent, int resourceId, const TRequest *tRequest);

	virtual void Criteria(msql &m, TGroup *);

protected:
	const TRequest *request;

//{{TXferDocumentListBoxVIRTUAL_BEGIN}}
	virtual void SetupWindow();
//{{TAboutDialogVIRTUAL_END}}
};  //{{TXferDocumentListBoxListBox}}
