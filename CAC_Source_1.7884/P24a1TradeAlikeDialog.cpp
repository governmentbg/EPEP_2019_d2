DEFINE_RESPONSE_TABLE1(TP24a1TradeAlikeDialog, TSelectContainerDialog)
//{{TP24a1TradeAlikeDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TP24a1TradeAlikeDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TP24a1TradeAlikeDialog Implementation}}

TP24a1TradeAlikeDialog::TP24a1TradeAlikeDialog(TWindow* parent, const char *title, TCollectEvents *e,
	TGArray<TRCDKeyContainer> *basics, TGArray<TBaseWind> *sideWinds)
:
	TSelectContainerDialog(parent, title, e, choice, basics, sideWinds, IDD_SELECT_EVENT)
{
}

void TP24a1TradeAlikeDialog::OKBNClicked()
{
	if (ask("Лицето е страна по дел%c за несъстоятелност/ликвидация. Желаете ли да продължите?", "ао"[sideWinds->Count() == 1]))
		CmOk();
}

void TP24a1TradeAlikeDialog::PrintBNClicked()
{
	// 2004:292 LPR: ported a List() from 1.1820 printuxn
	// 2006:263 LPR: List() moved to print_trade_sides()
	print_trade_sides(sideWinds);
}
