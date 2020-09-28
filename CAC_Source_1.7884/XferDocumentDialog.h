//{{TDialog = TXferDocumentDialog}}
class TXferDocumentDialog : public TSelectDialog
{
public:
	TXferDocumentDialog(TWindow* parent, TXferDocument *document, const TRequest *request);
	virtual ~TXferDocumentDialog();
};  //{{TXferDocumentDialog}}
