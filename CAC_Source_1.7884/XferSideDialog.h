//{{TDialog = TXferSideDialog}}
class TXferSideDialog : public TQuickDialog
{
public:
	TXferSideDialog(TWindow* parent, TXferDocument *document, TRCDKeyContainer *container, TGSArray<TXferSide> *tXferSides);

protected:
	mstr involvements;

	TXferSideListBox *xferSides;
	TCharAliasFace *involvement;

//{{TXferSideDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void OKBNEnable(TCommandEnabler &tce);
//{{TXferSideDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TXferSideDialog);
};  //{{TXferSideDialog}}