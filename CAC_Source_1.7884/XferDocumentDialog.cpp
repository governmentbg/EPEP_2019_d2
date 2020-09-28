//{{TXferDocumentDialog Implementation}}

TXferDocumentDialog::TXferDocumentDialog(TWindow* parent, TXferDocument *document, const TRequest *request)
:
	TSelectDialog(parent, document, IDD_XFER_DOCUMENT)
{
	glist = new TXferDocumentListBox(this, IDC_XFER_DOCUMENT_DOCUMENTS, request);
}

TXferDocumentDialog::~TXferDocumentDialog()
{
}
