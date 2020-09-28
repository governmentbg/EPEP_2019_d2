DEFINE_RESPONSE_TABLE1(TSurroundmentListDialog, TGroupListDialog)
//{{TSurroundmentListDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDPRINT, PrintBNClicked),
//{{TSurroundmentListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TSurroundmentListDialog Implementation}}

TSurroundmentListDialog::TSurroundmentListDialog(TWindow* parent, TSession *tSession, TDecision *decision, TGArray<TSurroundment> *tSurroundments, int resId)
:
	TGroupListDialog(parent, resId), session(tSession)
{
	new TCharAliasFace(this, IDC_EDIT_SURROUNDMENT_TYPE, "F_TYPE", Type, NULL);
	new TLongFace(this, IDC_EDIT_SURROUNDMENT_NO, "F_NO", SIZE_OF_NO);
	new TYearFace(this, IDC_EDIT_SURROUNDMENT_YEAR, "F_YEAR");
	new TDateFace(this, IDC_EDIT_SURROUNDMENT_DATE, "F_DATE");
	surroundments = new TSurroundmentListWindow(this, IDC_GROUP_LIST_WINDOW, session, decision, tSurroundments);
}

void TSurroundmentListDialog::PrintBNClicked()
{
	surroundments->Print();
}
