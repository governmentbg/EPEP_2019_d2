DEFINE_RESPONSE_TABLE1(TNFBPuniAdm2ndDialog, TScaleDialog)
//{{TNFBPuniAdm2ndDialogRSP_TBL_BEGIN}}
  EV_BN_CLICKED(IDOK, OKBNClicked),
//{{TNFBPuniAdm2ndDialogRSP_TBL_END}}
END_RESPONSE_TABLE;


//{{TNFBPuniAdm2ndDialog Implementation}}


TNFBPuniAdm2ndDialog :: TNFBPuniAdm2ndDialog (long int & cntNFBPA2,
							    TWindow * parent) :
				  TScaleDialog (parent, IDD_EDT_NFB_2ND_PUNI_ADM),
                          pNFBPA2 (NULL), nFBPA2cnt (cntNFBPA2)
{
  // INSERT>> Your constructor code here.

  pNFBPA2 = new TLongFace (this, IDC_ENFB2_PUNI_ADM_COUNT,
				   "NOT_FINISHED_BEG_PA2_CNT", SIZE_OF_NO) ;
}


void TNFBPuniAdm2ndDialog::OKBNClicked()
{
  // INSERT>> Your code here.

  nFBPA2cnt = pNFBPA2 -> L () ;
  CmOk () ;
}


void TNFBPuniAdm2ndDialog::SetupWindow()
{
  TScaleDialog::SetupWindow();

  // INSERT>> Your code here.

  pNFBPA2 -> SetL (0) ;
}

