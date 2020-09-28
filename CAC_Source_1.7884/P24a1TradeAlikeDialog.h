//{{TDialog = TP24a1TradeAlikeDialog}}
class TP24a1TradeAlikeDialog : public TSelectContainerDialog
{
public:
	TP24a1TradeAlikeDialog(TWindow* parent, const char *title, TCollectEvents *e, TGArray<TRCDKeyContainer> *basics,
		TGArray<TBaseWind> *sideWinds);

protected:
	int choice;

//{{TP24a1TradeAlikeDialogRSP_TBL_BEGIN}}
	void OKBNClicked();
	void PrintBNClicked();
//{{TP24a1TradeAlikeDialogRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TP24a1TradeAlikeDialog);
};    //{{TP24a1TradeAlikeDialog}}
