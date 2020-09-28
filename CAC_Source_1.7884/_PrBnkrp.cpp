DEFINE_RESPONSE_TABLE1(TPrintBankruptcyWideDialog, TScaleDialog)
  EV_BN_CLICKED(IDOK, OKBNClicked),
  EV_BN_CLICKED(IDC_BANKRUPTCY_WIDE_COMPLETED, CompletedBNClicked),
  EV_BN_CLICKED(IDC_BANKRUPTCY_WIDE_NONCOMPLETED, NoncompletedBNClicked),
  EV_BN_CLICKED(IDC_BANKRUPTCY_WIDE_EXTRA_TITLE, ExtraTitleBNClicked),
END_RESPONSE_TABLE;

TPrintBankruptcyWideDialog :: TPrintBankruptcyWideDialog (
                              TWindow * parent,
					BankruptcyWideDlgData & rDlgData,
                              int resId) :
    TScaleDialog (parent, resId),
    pWantNoncompleted (NULL), pWantCompleted (NULL),
    pBegCompletD (NULL), pEndCompletD (NULL),
    pBegLawD (NULL), pEndLawD (NULL),
    pWantExtraTitle (NULL), pYourOutNo (NULL),
    rDialogData (rDlgData)
 {
  oldExtraTilleStatus = BF_CHECKED;
  pWantNoncompleted = new TCheckBox (this, IDC_BANKRUPTCY_WIDE_NONCOMPLETED, NULL) ;
  pBegLawD = new TDateFaceOldVal (this, IDC_BANKRUPTCY_WIDE_LAW_DATE_BEG,
                                  "LAW_DATE_BEG") ;
  pEndLawD = new TDateFaceOldVal (this, IDC_BANKRUPTCY_WIDE_LAW_DATE_END,
                                  "LAW_DATE_END") ;
  pWantCompleted = new TCheckBox (this, IDC_BANKRUPTCY_WIDE_COMPLETED, NULL) ;
  pBegCompletD = new TDateFaceOldVal (this, IDC_BANKRUPTCY_WIDE_COMPLETED_DATE_BEG,
                                      "COMPLET_DATE_BEG") ;
  pEndCompletD = new TDateFaceOldVal (this, IDC_BANKRUPTCY_WIDE_COMPLETED_DATE_END,
                                      "COMPLET_DATE_END") ;
  pWantExtraTitle = new TCheckBox (this, IDC_BANKRUPTCY_WIDE_EXTRA_TITLE, NULL) ;
  pYourOutNo = new TStringFaceOldVal(this, IDC_BANKRUPTCY_WIDE_EXTRA_TITLE_NO,
                                     "YOUR_OUT_NO",
						 sizeof (rDialogData.yourOutNo)) ;
 }  // TPrintBankruptcyWideDialog :: TPrintBankruptcyWideDialog

void TPrintBankruptcyWideDialog :: SetupWindow ()
 {
  TScaleDialog :: SetupWindow () ;
   {
    CDate cB = Today ;
    CDate cE = Today ;

    cB.day = cB.month = 1 ;
    cE.month = 12 ;  // 12
    cE = lastDayOfThisMonth (cE) ;
    pBegCompletD -> SetDate (cB) ;
    pEndCompletD -> SetDate (cE) ;
    pBegLawD -> SetDate (CDate ()) ;
    pEndLawD -> SetDate (CDate ()) ;
   }
  pWantNoncompleted -> Check () ;
  pWantCompleted -> Check () ;
  pWantExtraTitle -> Check () ;
  oldExtraTilleStatus = pWantExtraTitle -> GetCheck () ;
 }  // TPrintBankruptcyWideDialog :: SetupWindow

void TPrintBankruptcyWideDialog :: ExtractData ()
 {
  rDialogData.wantNoncompleted =
    pWantNoncompleted -> GetCheck () == BF_CHECKED ;
  rDialogData.wantCompleted =
    pWantCompleted -> GetCheck () == BF_CHECKED ;
  rDialogData.completedBegDate = pBegCompletD -> Date () ;
  rDialogData.completedEndDate = pEndCompletD -> Date () ;
  rDialogData.lawBegDate = pBegLawD -> Date () ;
  rDialogData.lawEndDate = pEndLawD -> Date () ;
  rDialogData.wantExtraTitle =
    pWantExtraTitle -> GetCheck () == BF_CHECKED ;
  ostrstream (rDialogData.yourOutNo, sizeof (rDialogData.yourOutNo))
    << pYourOutNo -> S () << ends ;
  rDialogData.yourOutNo [sizeof (rDialogData.yourOutNo) - 1] = '\0' ;
 }  // TPrintBankruptcyWideDialog :: ExtractData

bool TPrintBankruptcyWideDialog :: ValidData ()
 {
  bool rT = true ;

  if (rDialogData.wantNoncompleted == false &&
      rDialogData.wantCompleted == false)
    rT = say_no_selection () ;
  if (rT)
    if (rDialogData.wantCompleted)
      if (rDialogData.completedBegDate.Empty () ||
          rDialogData.completedEndDate.Empty () ||
          rDialogData.completedBegDate > rDialogData.completedEndDate)
        rT = incorrect_period_given (this, IDC_BANKRUPTCY_WIDE_COMPLETED) ;
  if (rT)
    if (rDialogData.wantNoncompleted)
      if (rDialogData.lawEndDate.Empty () == false &&
          rDialogData.lawBegDate > rDialogData.lawEndDate)
        rT = incorrect_period_given (this, IDC_BANKRUPTCY_WIDE_NONCOMPLETED) ;
  return rT ;
 }  // TPrintBankruptcyWideDialog :: ValidData

void TPrintBankruptcyWideDialog :: OKBNClicked ()
 {
  ExtractData () ;
  if (ValidData ())
  {
    // 2011:307 FIX: empty date crashes
    if (rDialogData.lawEndDate.Empty())
	rDialogData.lawEndDate = Today;
    CmOk () ;
  }
  else
    rDialogData.Clear () ;
 }  // TPrintBankruptcyWideDialog :: OKBNClicked

void TPrintBankruptcyWideDialog :: CompletedBNClicked ()
 {
  const bool begEnabled = pBegCompletD -> IsWindowEnabled () ;
  const bool endEnabled = pEndCompletD -> IsWindowEnabled () ;

  if (pWantCompleted -> GetCheck () == BF_CHECKED)
   {
    if (begEnabled == false)
      pBegCompletD -> EnableFace () ;
    if (endEnabled == false)
      pEndCompletD -> EnableFace () ;
   }
  else
   {
    if (begEnabled)
      pBegCompletD -> DisableFace () ;
    if (endEnabled)
      pEndCompletD -> DisableFace () ;
   }
 }  // TPrintBankruptcyWideDialog :: CompletedBNClicked

void TPrintBankruptcyWideDialog :: NoncompletedBNClicked ()
 {
  const bool begEnabled = pBegLawD -> IsWindowEnabled () ;
  const bool endEnabled = pEndLawD -> IsWindowEnabled () ;

  if (pWantNoncompleted -> GetCheck () == BF_CHECKED)
   {
    if (begEnabled == false)
      pBegLawD -> EnableFace () ;
    if (endEnabled == false)
      pEndLawD -> EnableFace () ;
   }
  else
   {
    if (begEnabled)
      pBegLawD -> DisableFace () ;
    if (endEnabled)
      pEndLawD -> DisableFace () ;
   }
 }  // TPrintBankruptcyWideDialog :: NoncompletedBNClicked

void TPrintBankruptcyWideDialog :: ExtraTitleBNClicked ()
 {
  const bool yONEnabled = pYourOutNo -> IsWindowEnabled () ;

  oldExtraTilleStatus = pWantExtraTitle->GetCheck();  // 10.jun.2003
  if (pWantExtraTitle -> GetCheck () == BF_CHECKED)
   {
    if (yONEnabled == false)
      pYourOutNo -> EnableFace () ;
   }
  else
   {
    if (yONEnabled)
      pYourOutNo -> DisableFace () ;
   }
 }  // TPrintBankruptcyWideDialog :: ExtraTitleBNClicked

DEFINE_RESPONSE_TABLE1(TPrintBankruptcyWideWebDialog, TPrintBankruptcyWideDialog)
  EV_BN_CLICKED(IDOK, OKBNClicked),
  EV_BN_CLICKED(IDC_BANKRUPTCY_WIDE_COMPLETED, ProcessCheckboxes),
  EV_BN_CLICKED(IDC_BANKRUPTCY_WIDE_NONCOMPLETED, ProcessCheckboxes),
  EV_BN_CLICKED(IDC_BANKRUPTCY_WIDE_COMPLETED_WEB, ProcessCheckboxes),
  EV_BN_CLICKED(IDC_BANKRUPTCY_WIDE_NONCOMPLETED_WEB, ProcessCheckboxes),
END_RESPONSE_TABLE;

TPrintBankruptcyWideWebDialog::TPrintBankruptcyWideWebDialog(TWindow* parent,
  BankruptcyWideDlgData& rDlgData, int resId):
  TPrintBankruptcyWideDialog(parent, rDlgData, resId)
{
  oldCompletedPath[0] = '\0';
  oldNoncompletedPath[0] = '\0';
  bw_noncompleted_web = new TCheckBox(this, IDC_BANKRUPTCY_WIDE_NONCOMPLETED_WEB, NULL);
  bw_completed_web = new TCheckBox(this, IDC_BANKRUPTCY_WIDE_COMPLETED_WEB, NULL);
  bw_noncompleted_web_path = new TEdit(this, IDC_BANKRUPTCY_WIDE_NONCOMPLETED_WEB_PATH,
    sizeof(rDialogData.noncompletedWebPath));
  bw_completed_web_path = new TEdit(this, IDC_BANKRUPTCY_WIDE_COMPLETED_WEB_PATH,
    sizeof(rDialogData.completedWebPath));
}  // TPrintBankruptcyWideWebDialog::TPrintBankruptcyWideWebDialog

void TPrintBankruptcyWideWebDialog::SetupWindow()
{
  TPrintBankruptcyWideDialog::SetupWindow();
  bw_noncompleted_web->Check();
  bw_completed_web->Check();
  TransferPaths(tdSetData);
}  // TPrintBankruptcyWideWebDialog::SetupWindow

void TPrintBankruptcyWideWebDialog::EnableCompletedDates(bool enable)
{
  const bool cBegDEnabled = pBegCompletD->IsWindowEnabled();
  const bool cEndDEnabled = pEndCompletD->IsWindowEnabled();

  //info("EnableCompletedDates(%s)", enable ? "true" : "false");
  if(enable)
  {
    if(cBegDEnabled == false)
      pBegCompletD->EnableFace();
    if(cEndDEnabled == false)
      pEndCompletD->EnableFace();
  }
  else
  {
    if(cBegDEnabled)
      pBegCompletD->DisableFace();
    if(cEndDEnabled)
      pEndCompletD->DisableFace();
  }
}  // TPrintBankruptcyWideWebDialog::EnableCompletedDates

void TPrintBankruptcyWideWebDialog::EnableNoncompletedDates(bool enable)
{
  const bool ncBegDEnabled = pBegLawD->IsWindowEnabled();
  const bool ncEndDEnabled = pEndLawD->IsWindowEnabled();

  //info("EnableNonCompletedDates(%s)", enable ? "true" : "false");
  if(enable)
  {
    if(ncBegDEnabled == false)
      pBegLawD->EnableFace();
    if(ncEndDEnabled == false)
      pEndLawD->EnableFace();
  }
  else
  {
    if(ncBegDEnabled)
      pBegLawD->DisableFace();
    if(ncEndDEnabled)
      pEndLawD->DisableFace();
  }
}  // TPrintBankruptcyWideWebDialog::EnableNoncompletedDates

void TPrintBankruptcyWideWebDialog::EnableCompletedPath(bool enable)
{
  TEdit* pCPEdit = bw_completed_web_path;

  //info("EnableCompletedPath(%s)", enable ? "true" : "false");
  if(pCPEdit &&
    sizeof(rDialogData.completedWebPath) == sizeof(oldCompletedPath))
  {
    if(enable)
    {
	if(pCPEdit->IsWindowEnabled() == false)
	  pCPEdit->Transfer(oldCompletedPath, tdSetData);
    }
    else
    {
	if(pCPEdit->IsWindowEnabled())
	{
	  pCPEdit->Transfer(oldCompletedPath, tdGetData);
	  pCPEdit->SetWindowText("");
	}
    }
  }
  pCPEdit->EnableWindow(enable);
}  // TPrintBankruptcyWideWebDialog::EnableCompletedPath

void TPrintBankruptcyWideWebDialog::EnableNoncompletedPath(bool enable)
{
  TEdit* pCPEdit = bw_noncompleted_web_path;

  //info("EnableNonCompletedPath(%s)", enable ? "true" : "false");
  if(pCPEdit &&
    sizeof(rDialogData.noncompletedWebPath) == sizeof(oldNoncompletedPath))
  {
    if(enable)
    {
	if(pCPEdit->IsWindowEnabled() == false)
	  pCPEdit->Transfer(oldNoncompletedPath, tdSetData);
    }
    else
    {
	if(pCPEdit->IsWindowEnabled())
	{
	  pCPEdit->Transfer(oldNoncompletedPath, tdGetData);
	  pCPEdit->SetWindowText("");
	}
    }
  }
  pCPEdit->EnableWindow(enable);
}  // TPrintBankruptcyWideWebDialog::EnableNoncompletedPath

void TPrintBankruptcyWideWebDialog::ProcessCheckboxes()
{
  const bool noncompletedWeb = bw_noncompleted_web->GetCheck() == BF_CHECKED;
  const bool noncompleted = pWantNoncompleted->GetCheck() == BF_CHECKED;

  const bool completedWeb = bw_completed_web->GetCheck() == BF_CHECKED;
  const bool completed = pWantCompleted->GetCheck() == BF_CHECKED;

  //info("ProcessCheckboxes\n"
  //  "noncompletedWeb = %s\n"
  //  "noncompleted = %s\n"
  //  "completedWeb = %s\n"
  //  "completed = %s",
  //  (noncompletedWeb ? "true" : "false"),
  //  (noncompleted ? "true" : "false"),
  //  (completedWeb ? "true" : "false"),
  //  (completed ? "true" : "false")
  //  );
  if(noncompletedWeb == true && noncompleted == true)
  {
    EnableNoncompletedDates(true);
    EnableNoncompletedPath(true);
  }
  else
    if(noncompletedWeb == true && noncompleted == false)
    {
      EnableNoncompletedDates(true);
      EnableNoncompletedPath(true);
    }
    else
      if(noncompletedWeb == false && noncompleted == true)
      {
        EnableNoncompletedDates(true);
        EnableNoncompletedPath(false);
      }
      else
      {
        EnableNoncompletedDates(false);
	  EnableNoncompletedPath(false);
      }
  if(completedWeb == true && completed == true)
  {
    EnableCompletedDates(true);
    EnableCompletedPath(true);
  }
  else
    if(completedWeb == true && completed == false)
    {
      EnableCompletedDates(true);
      EnableCompletedPath(true);
    }
    else
      if(completedWeb == false && completed == true)
      {
        EnableCompletedDates(true);
        EnableCompletedPath(false);
      }
      else
	{
        EnableCompletedDates(false);
        EnableCompletedPath(false);
      }
  if(noncompleted == false  && completed == false)
  {
    //oldExtraTilleStatus = pWantExtraTitle->GetCheck();
    pWantExtraTitle->SetCheck(BF_UNCHECKED);
    pWantExtraTitle->EnableWindow(false);
    ExtraTitleBNClicked();
  }
  else
  {
    pWantExtraTitle->EnableWindow(true);
    pWantExtraTitle->SetCheck(oldExtraTilleStatus);
    ExtraTitleBNClicked();
  }
}  // TPrintBankruptcyWideWebDialog::ProcessCheckboxes

void TPrintBankruptcyWideWebDialog::TransferPaths(TTransferDirection direction)
{
  TEdit* pCPEdit;

  pCPEdit = bw_completed_web_path;
  if(pCPEdit &&
    sizeof(rDialogData.completedWebPath) == sizeof(oldCompletedPath))
    pCPEdit->Transfer(rDialogData.completedWebPath, direction);

  pCPEdit = bw_noncompleted_web_path;
  if(pCPEdit &&
    sizeof(rDialogData.noncompletedWebPath) == sizeof(oldNoncompletedPath))
    pCPEdit->Transfer(rDialogData.noncompletedWebPath, direction);
}

void TPrintBankruptcyWideWebDialog::ExtractData()
{
  TPrintBankruptcyWideDialog::ExtractData();
  rDialogData.wantNoncompletedWeb = bw_noncompleted_web->GetCheck() == BF_CHECKED;
  rDialogData.wantCompletedWeb = bw_completed_web->GetCheck() == BF_CHECKED;
  TransferPaths(tdGetData);
//  info("NonCompl. path: %s\n""Compl. path: %s",
//    rDialogData.noncompletedWebPath, rDialogData.completedWebPath);
}  // TPrintBankruptcyWideWebDialog::ExtractData

bool TPrintBankruptcyWideWebDialog::ValidData()
{
  bool rT = true;
  // 2014:157
  //const char* const invalidFileName = "Невалидно име на файл";
  static constant invalidFileName = "Разширението на файлвете трябва да бъде htm или html";

  if(rDialogData.wantNoncompleted == false &&
    rDialogData.wantCompleted == false &&
    rDialogData.wantNoncompletedWeb == false &&
    rDialogData.wantCompletedWeb == false)
    rT = say_no_selection();
  if(rT)
    if(rDialogData.wantCompleted || rDialogData.wantCompletedWeb)
	if(rDialogData.completedBegDate.Empty() ||
	  rDialogData.completedEndDate.Empty() ||
	    rDialogData.completedBegDate > rDialogData.completedEndDate)
	  rT = incorrect_period_given(this, IDC_BANKRUPTCY_WIDE_COMPLETED);
  if(rT)
    if(rDialogData.wantNoncompleted || rDialogData.wantNoncompletedWeb)
	if (rDialogData.lawEndDate.Empty() == false &&
	    rDialogData.lawBegDate > rDialogData.lawEndDate)
	  rT = incorrect_period_given(this, IDC_BANKRUPTCY_WIDE_NONCOMPLETED);
  if(rT)	// 2014:157 strIsBlank() -> !html_name()
    if((rDialogData.wantCompletedWeb &&
	  !html_name(rDialogData.completedWebPath)) ||
	(rDialogData.wantNoncompletedWeb &&
	  !html_name(rDialogData.noncompletedWebPath)))
	rT = error(invalidFileName);
  if(rT)
    if(rDialogData.wantCompletedWeb && rDialogData.wantNoncompletedWeb &&
      strcmp(rDialogData.completedWebPath,
        rDialogData.noncompletedWebPath) == 0)
      rT = error("Имената на изходните файлове са еднакви:\n%s",
        rDialogData.noncompletedWebPath);
  if(rT)
    if(rDialogData.wantNoncompletedWeb &&
      access(rDialogData.noncompletedWebPath, 0) == 0)
      rT = confirm_file_overwrite(rDialogData.noncompletedWebPath);
  if(rT)
    if(rDialogData.wantCompletedWeb &&
      access(rDialogData.completedWebPath, 0) == 0)
      rT = confirm_file_overwrite(rDialogData.completedWebPath);
  return rT;
}  // TPrintBankruptcyWideWebDialog::ValidData
