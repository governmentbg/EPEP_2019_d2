#include "cac.h"

// ------------------ PrtPenaltyBkDlgData - begin ------------------

class PrtPenaltyBkDlgData: public TDummyGroup
{
  public:  // Construction / destruction
    PrtPenaltyBkDlgData();
  public:  // Methods
    bool Output(ostream& os, char* const s, const size_t len, const int algn,
	const size_t skip, int dCiph) const;
  public:  // Data
    char types[SIZE_OF_TYPES];
    long composition;
    CDate begD;
    CDate endD;
    long begSerialNo;
    long allPenalityType;  // 20051214
};  // class PrtPenaltyBkDlgData

PrtPenaltyBkDlgData::PrtPenaltyBkDlgData()
{
  Add(
    new TCharList("TYPES", types, sizeof(types)),
    new TLongAlias("COMPOSITION", &composition),
    new TDate("BEG_D", &begD, true),
    new TDate("END_D", &endD, true),
    new TLong("BEG_SERIAL_NO", &begSerialNo),
    new TLong("ALL_PENALITY_TYPE", &allPenalityType),
    NULL);
}  // PrtPenaltyBkDlgData::PrtPenaltyBkDlgData

bool PrtPenaltyBkDlgData::Output(ostream& os, char* const s, const size_t len,
  const int algn, const size_t skip, int dCiph) const
{
  if(len && s)
  {
    bool perceived = true;
    const long int d_cph = ((long int)dCiph) - OFFSET_PENALTY_BK_DLG_DATA ;

    switch(d_cph)
    {
      case 1:
      {
        bool allTypesIncluded = false;

        if(strlen(types) == strlen(TYPE_LAWSUITS))
        {
          char tmpTypes[sizeof(types)];
          const char* const test = TYPE_LAWSUITS;

          tmpTypes[0] = '\0';
          strConcat(tmpTypes, types, sizeof(tmpTypes));
          difference(tmpTypes, test);
          if(tmpTypes[0] == '\0')
            allTypesIncluded = true;
        }
        s[0] = '\0';
        if(types[0] && allTypesIncluded == false)
        {
          ostrstream oss(s, len + 1);

          types_ToPlural(oss, types);
          oss << " дела" << ends;
          s[len] = '\0';
        }
        writeString (os, s, len, algn, skip) ;
      }
      break;
      case 2:
        s[0] = '\0';
        if(composition)
        {
	    if (use_area_compositions ()) // 20070806
	    {
		  compositionToString(composition, s, len + 1);
		  strConcat(s + strlen(s), " състав", len + 1 - strlen(s));
	    }
        }
        writeString(os, s, len, algn, skip);
      break;
      case 3:
        CDateToString(begD, s, len + 1);
        writeString(os, s, len, algn, skip);
      break;
      case 4:
        CDateToString(endD, s, len + 1);
        writeString(os, s, len, algn, skip);
      break;
      case 5:
        {
          ostrstream oss(s, len + 1);

          if(begD.Empty() == false)
            oss << "от " << begD;
          if(endD.Empty() == false)
            oss << " до " << endD;
          oss << ends;
          s[len] = '\0';
        }
        writeString(os, s, len, algn, skip);
      break;
      default:
        perceived = false;
	break;
    }
    return perceived;
  }
  else
    return true;  // To avoid any subsequent calls of ....Output
}  // PrtPenaltyBkDlgData::Output

// ------------------ PrtPenaltyBkDlgData - end ------------------

// ------------------ TPrintPenaltyBookDialog - begin ------------------

class TPrintPenaltyBookDialog: public TScaleDialog
{  // 12.jun.2003
  public:  // Construction / destruction
    TPrintPenaltyBookDialog(TWindow* parent, PrtPenaltyBkDlgData& dldDataRef);
  public:  // Methods
    virtual void SetupWindow();
  protected:  // Methods
    void OKBNClicked();
    void TypeCBNSelChange();
    void InvertTypesBNCkicked();
    bool UserInputOk();
    TCheckBox * pChkBx_All ;  // 20060523
  protected:  // Data
    PrtPenaltyBkDlgData& rDlgData;
  private:
    // 2009:065 LPR: eliminating GetDlgItemPtr
    TCharListFace *penalty_law_types;
    TLongAliasFace *penalty_composition;
    TDateFace *minDate;
    TDateFace *maxDate;
DECLARE_RESPONSE_TABLE(TPrintPenaltyBookDialog);
};  // class TPrintPenaltyBookDialog

DEFINE_RESPONSE_TABLE1(TPrintPenaltyBookDialog, TScaleDialog)
  EV_LBN_SELCHANGE(IDC_PENALTY_LAW_TYPES, TypeCBNSelChange),
  EV_BN_CLICKED(IDOK, OKBNClicked),
  EV_BN_CLICKED(IDC_PENALTY_INVERT_TYPES, InvertTypesBNCkicked),
END_RESPONSE_TABLE;

TPrintPenaltyBookDialog::TPrintPenaltyBookDialog(TWindow* parent,
  PrtPenaltyBkDlgData& dldDataRef):
  TScaleDialog(parent, IDD_PENALTY_BOOK),
  rDlgData(dldDataRef)
  , pChkBx_All(NULL)          // 20060523
{
  penalty_law_types = new TCharListFace(this, IDC_PENALTY_LAW_TYPES, "_TYPES", Type, QUERY_COLLEGES, SIZE_OF_COLLEGES);
  penalty_composition = new TLongAliasFace(this, IDC_PENALTY_COMPOSITION, "COMPOSITION", Composition, QUERY_COMPOSITIONS, ALIAS_EMPTY);
  minDate = new TDateFace(this, IDC_PENALTY_BEG_DATE, "BEG_D", true);
  new TDateFaceUpDown(this, IDC_PENALTY_BEG_DATE_UPDN, minDate);
  maxDate = new TDateFace(this, IDC_PENALTY_END_DATE, "END_D", true);
  new TDateFaceUpDown(this, IDC_PENALTY_END_DATE_UPDN, maxDate);
  new TLongFace(this, IDC_PENALTY_BEG_SERIAL_NO, "BEG_SERIAL_NO", SIZE_OF_NO);

  pChkBx_All = new TCheckBox (this, IDC_CHECKBOX_ALL_ITEMS) ; // 20060523

}  // TPrintPenaltyBookDialog::TPrintPenaltyBookDialog

void TPrintPenaltyBookDialog::SetupWindow()
{
  TScaleDialog::SetupWindow();
  rDlgData >> (*this);
  penalty_law_types->SetS(rDlgData.types);
  select_de1st_comp(penalty_composition);
  pChkBx_All -> SetCheck (BF_CHECKED) ;  // 20060523

}  // TPrintPenaltyBookDialog::SetupWindow

void TPrintPenaltyBookDialog::TypeCBNSelChange()
{
  penalty_composition->EnableWindow(strlen(penalty_law_types->S()) < 2);
}  // TPrintPenaltyBookDialog::TypeCBNSelChange

void TPrintPenaltyBookDialog::InvertTypesBNCkicked()
{
  TCharListFace* pTLF = penalty_law_types;

  if(pTLF)
    invert_list_box_selection(pTLF);
}  // TPrintPenaltyBookDialog::InvertTypesBNCkicked

bool TPrintPenaltyBookDialog::UserInputOk()
{
  bool rT = true;

  rDlgData << (*this);
  strcpy(rDlgData.types, penalty_law_types->S());
  if(rDlgData.types[0] == '\0')
    rT = error( "Няма избран вид дело" );
  if(rDlgData.endD.Empty() == false && rDlgData.begD > rDlgData.endD)
    rT = error(INCORRECT_PERIOD_GIVEN);
  if(rDlgData.begSerialNo < 0)
    rT = error("Невалиден начален пореден No.");

  if(rT)  // This must be at the end
  {
    if(rDlgData.begSerialNo == 0)
	rDlgData.begSerialNo = 1;
  }
  return rT;
}  // TPrintPenaltyBookDialog::UserInputOk

void TPrintPenaltyBookDialog::OKBNClicked()
{
  if(UserInputOk())
  {
    rDlgData.allPenalityType = pChkBx_All -> GetCheck () == BF_CHECKED ; // 20060523
    rDlgData.composition = penalty_composition->L();

    CmOk();
  }
}  // TPrintPenaltyBookDialog::OKBNClicked

// ------------------ TPrintPenaltyBookDialog - end ------------------

static bool non_zero_sum(const double sum)
{
  return fabs(sum) >= 0.005;
}

// 2014:311 LPR: sum_in_words() -> mstr.cpp

// ------------------ PenaltyBookColumns - begin ------------------

class PenaltyBookEnum
 {
  protected :
    enum
     {
      iC_SerialNo = 0,                           //  1
      iC_LawKey,                                 //  2
      iC_InvolvementAndNames,                    //  3
      iC_SessKindDate,                           //  4
      iC_Sum,                                    //  5
      iC_Remark,                                 //  6
      iC_Sign,                                   //  7
      iC_Involvement,                            //  8
      iC_Names,                                  //  9
      iC_SumInWords,                             // 10
      iC_SessDate,                               // 11
      iC_FmtFileText,                            // 12

      i_columns
     } ;
 } ;  // class PenaltyBookEnum

class PenaltyBookColumns : public PenaltyBookEnum, public columnsArr
 {
  protected :
    void MakeColumns(const TExpense& expense,
      const int serialNo,
      const char* const aText = NULL);
  public :
    PenaltyBookColumns(const char* formatFN, TWindow* parent,
      const TExpense& expense,
      const int serialNo, const int minRows = 0,
      const char* const aText = NULL);
    static bool SeekDatedSide(const TExpense& expense, TSideWind* const pDS,
	bool* pManyInvolvs = NULL);  // When returns false, it also clears (*pDS)
 } ;  // class PenaltyBookColumns

PenaltyBookColumns::PenaltyBookColumns(const char* formatFN, TWindow* parent,
      const TExpense& expense,
      const int serialNo, const int minRows,
      const char* const aText):
    columnsArr(formatFN, parent, i_columns, 1 + OFFSET_PENALTY_BOOK, true)
{
  MakeColumns(expense,
              serialNo, aText);
  defineMaxRows();
  if(maxRows < minRows)
    maxRows = minRows;
}  // PenaltyBookColumns::PenaltyBookColumns

void PenaltyBookColumns::MakeColumns(const TExpense& expense,
     const int serialNo, const char* const aText)
{
  column * pColumn ;

  if(DatedContainer_(expense.key, expense.kind, expense.date).FilledOK() &&
    expense.ucnType && ucnPresent(expense.ucn))
  {
// 20060316    TSideWind* pDS = NULL;
    TSideWind* pDS = new TSideWind;  // 20060316
    bool manyInvolvs = false;
    TSession rSess;
    {
      try
      {
          // 20060411 ->
           if ( strchr(KIND_SESSIONS, expense.kind) )
           { // try to get session -> check composition
               extractSession (expense.key, expense.date, rSess, expense.kind);
           }
           else
           { // try to get decision -> session -> check composition
               TDecision * pD1 = new TDecision ;
	       pD1 -> key  = expense.key ;
               pD1 -> kind = expense.kind ;
               pD1 -> date = expense.date ;

               if (pD1 -> Try ())
                  extractSession (pD1 -> key, pD1 -> date, rSess, pD1 -> sessionKind);
               delete pD1 ;
           }
          // 20060411 <-
// 20060316     TSideWind* pDS1 = Container2SideWind(pSess.Ptr());

// 20060316     pDS = pDS1;
         pDS->key  = expense.key;
         pDS->kind = rSess.kind;    // 20060316
      }
      CATCH_ANYTHING
    }
    if(columns[iC_InvolvementAndNames] || columns[iC_Involvement])
    {
      if(pDS &&
	  expense.involvement == '\0'  // 28.jul.2003
        )
        SeekDatedSide(expense, pDS, &manyInvolvs);
    }
    if((pColumn = columns[iC_SerialNo]) != NULL)
    {  // -- iC_SerialNo --
      addLongToColumn(pColumn, serialNo);
    }  // -- iC_SerialNo --
    if((pColumn = columns[iC_LawKey]) != NULL)
    {  // -- iC_LawKey --
      char tmp[128];

      TRCDKey_(expense.key).Print(tmp, sizeof(tmp));
      pColumn->addText(tmp);
    }  // -- iC_LawKey --
    if((pColumn = columns[iC_InvolvementAndNames]) != NULL)
    {  // -- iC_InvolvementAndNames --
      char tmp[128];
      if(expense.involvement != '\0')  // 28.jul.2003 - begin
      {
        // 20060318 -> check this
        involvementToString(expense.involvement, tmp, sizeof(tmp));
        pColumn->addText(tmp);
      }
      else  // 28.jul.2003 - end
        if(pDS && pDS->ucnType)
        {
          involvementToString(pDS->involvement, tmp, sizeof(tmp));
	  if(manyInvolvs)
            strConcat(tmp + strlen(tmp), ", ...", sizeof(tmp) - strlen(tmp));
          pColumn->addText(tmp);
        }
      if(ucnAndUcnTypeToName(expense.ucn, expense.ucnType, tmp, sizeof(tmp)))
        pColumn->addText(tmp);
    }  // -- iC_InvolvementAndNames --
    if((pColumn = columns[iC_Sum]) != NULL)
    {  // -- iC_Sum --
      char tmp[128];
      double sumTotal = expense.sum1 + expense.sum2 ;

      ostrstream(tmp, sizeof(tmp))
        << resetiosflags(ios::scientific)
	<< setiosflags(ios::fixed | ios::showpoint)
        << setprecision(2) << sumTotal << ends;
      ENDING_ZERO(tmp);
      pColumn->addText(tmp);
    }  // -- iC_Sum --
    if((pColumn = columns[iC_SessDate]) != NULL)
    {  // -- iC_SessDate --
      add_date_to_column(pColumn, expense.date);
    }  // -- iC_SessDate --
    if((pColumn = columns[iC_SessKindDate]) != NULL)
    {  // -- iC_SessKindDate --
      char tmp[128];

      if(strchr(KIND_PSEUDO_SESSIONS, expense.kind) == NULL)
	//kind_to_alias(expense.kind, tmp, sizeof(tmp));    // 20060411 in comm
        kind_to_alias(rSess.kind, tmp, sizeof(tmp)); // 20060411
      else
        strCopy(tmp, "#", sizeof(tmp));
      add_date_to_column(pColumn, expense.date, tmp);
    }  // -- iC_SessKindDate --
    if((pColumn = columns[iC_Remark]) != NULL)
    {  // -- iC_Remark --

      PenalityType->penalityType = expense.penalityType;  // PenalityType = new TPenalityType;
      bool penalityTypeFound= PenalityType->penalityType && PenalityType->Try();  // 20060407

      if(penalityTypeFound)
         pColumn->addText(PenalityType->name);
      if(expense.text[0])
         pColumn->addText(expense.text);

      if(expense.rejectDate.Empty() == true)
      {
//         if(penalityTypeFound)
//            pColumn->addText(PenalityType->name);
//         if(expense.text[0])
//            pColumn->addText(expense.text);
         if(expense.paidDate.Empty() == false)
         {
            char tmp[sizeof(expense.paidNote) + 32];

            tmp[0] = '\0';
	    if(expense.paidNote[0])
            {
               strConcat(tmp, "вн.""\xA0""б. ", sizeof(tmp));
		   strConcat(tmp, expense.paidNote, sizeof(tmp));
	    }
            add_date_to_column(pColumn, expense.paidDate,
                         "Платена", tmp);  // 04.aug.2003  //  - " на",; + tmp
         }
      }
      else  // rejected
      {
        char tmp[128];    tmp[0] = '\0';
        const char* const vf = "ВвФф";
	const char* const zs = "ЗзСс";

//	char sumType;
//	char sumState;

//	char rejectCourt[SIZE_OF_UCN];		// 2003:350
//  expense.rejectKind -   вид заседание, с което се отменя
//                     ИЛИ вид на съдебния акт, с който по-висшата инстамция отменя глобата
//	CDate rejectDate;

        if( expense.sumState == SUMMONS_CANCELED ) // 'c' - сумата е Изменена
	{

	   // 2008:059 LPR: fixed
	   //if ( PenalityType->flags == 1 || PenalityType->flags == 3 ) // може да се изменя от по-висша инстанция
	   if (PenalityType->flags & PENALITY_TYPE_HIREJ)
	   {
               char tmp1[128];
               char snrd [SIZE_OF_NAME] ;
	       strCopy(tmp, "Изменена ", sizeof(tmp));
               kind_to_alias(expense.rejectKind, tmp1, sizeof(tmp1)); // вид съдебен акт
               senderUCNAndUCNTypeToName ( expense.rejectCourt,  // Съд - по-висша инстанция
                                           UXN_SENDER,           // за обжалване	= 's',
                                           snrd, SIZE_OF_NAME);
               ostrstream(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp))
                        << (tmp1[0] && strchr(zs, tmp1[0]) ? " със " : " с ")
                        << tmp1 << " от " << expense.rejectDate << " г." // дата на съдебния акт
                        << " на " <<  snrd
                        << ends;
	     }
           else
           {
               char tmp1[128];
	       strCopy(tmp, "Изменена ", sizeof(tmp));
               kind_to_alias(expense.rejectKind, tmp1, sizeof(tmp1)); // вид заседание

               ostrstream(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp))
                        << (tmp1[0] && strchr(vf, tmp1[0]) ? " във " : " в ")
                        << tmp1 << " от " << expense.rejectDate << " г." // дата на заседанието
                        << ends;
           }
           ENDING_ZERO(tmp);
           pColumn->addText(tmp);
        }

        if(expense.sumState==SUMMONS_UNMOVEABLE) // 'u' - сумата е Отменена
	{
	   // 2008:059 LPR: fixed
	   //if ( PenalityType->flags == 1 || PenalityType->flags == 3 ) // може да се изменя от по-висша инстанция
	   if (PenalityType->flags & PENALITY_TYPE_HIREJ)
           {
               char tmp1[128];
               char snrd [SIZE_OF_NAME] ;
               strCopy(tmp, "Отменена ", sizeof(tmp));
               kind_to_alias(expense.rejectKind, tmp1, sizeof(tmp1)); // вид съдебен акт
               senderUCNAndUCNTypeToName ( expense.rejectCourt,  // Съд - по-висша инстанция
                                           UXN_SENDER,           // за обжалване	= 's',
                                           snrd, SIZE_OF_NAME);
               ostrstream(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp))
                        << (tmp1[0] && strchr(zs, tmp1[0]) ? " със " : " с ")
                        << tmp1 << " от " << expense.rejectDate << " г." // дата на съдебния акт
                        << " на " <<  snrd
                        << ends;
           }
           else
           {
               char tmp1[128];
               strCopy(tmp, "Отменена ", sizeof(tmp));
               kind_to_alias(expense.rejectKind, tmp1, sizeof(tmp1)); // вид заседание

               ostrstream(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp))
                        << (tmp1[0] && strchr(vf, tmp1[0]) ? " във " : " в ")
                        << tmp1 << " от " << expense.rejectDate << " г." // дата на заседанието
                        << ends;
           }
           ENDING_ZERO(tmp);
           pColumn->addText(tmp);
        }
#if 0
        {
          char tmp1[128];
          const char* const vf = "ВвФф";

          strCopy(tmp, "Отменена ", sizeof(tmp));
          kind_to_alias(expense.rejectKind, tmp1, sizeof(tmp1));
          ostrstream(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp))
            << (tmp1[0] && strchr(vf, tmp1[0]) ? " във " : " в ")
            << tmp1 << " от " << expense.rejectDate << " г." << ends;
          ENDING_ZERO(tmp);
          pColumn->addText(tmp);
        }
        // from Petyo
          DatedContainer_(expense.key, expense.rejectKind, expense.rejectDate).
            Print(tmp, sizeof(tmp));
        add_date_to_column(pColumn, expense.rejectDate, "Отменена в", tmp);
#endif  // of 0
      } // else  // rejected
    }  // -- iC_Remark --
    if((pColumn = columns[iC_Sign]) != NULL)
    {  // -- iC_Sign --
      // Nothing
    }  // -- iC_Sign --
    if((pColumn = columns[iC_Involvement]) != NULL)
    {  // -- iC_Involvement --
      char tmp[128];

      if(expense.involvement != '\0')  // 28.jul.2003 - begin
      {
        involvementToString(expense.involvement, tmp, sizeof(tmp));
        pColumn->addText(tmp);
      }
      else  // 28.jul.2003 - end
        if(pDS && pDS->ucnType)
        {
          involvementToString(pDS->involvement, tmp, sizeof(tmp));
          if(manyInvolvs)
            strConcat(tmp + strlen(tmp), ", ...", sizeof(tmp) - strlen(tmp));
          pColumn->addText(tmp);
        }
    }  // -- iC_Involvement --
    if((pColumn = columns[iC_Names]) != NULL)
    {  // -- iC_Names --
      char tmp[128];

      if(ucnAndUcnTypeToName(expense.ucn, expense.ucnType, tmp, sizeof(tmp)))
        pColumn->addText(tmp);
    }  // -- iC_Names --
    if((pColumn = columns[iC_SumInWords]) != NULL)  // print RKO txt mode
    {  // -- iC_SumInWords --
	mstr tmp("Словом: ");
	double sumTotal = expense.sum1 + expense.sum2;

	if (non_zero_sum(sumTotal))
	  sum_in_words(sumTotal, tmp);

	pColumn->addText(str(tmp));
    }  // -- iC_SumInWords --
    if((pColumn = columns[iC_FmtFileText]) != NULL)
    {  // -- iC_FmtFileText --
	if(aText && aText[0])
	  pColumn->addText(aText);
    }  // -- iC_FmtFileText --
    if(pDS)
	delete pDS;
  }
}  // PenaltyBookColumns::MakeColumns

bool PenaltyBookColumns::SeekDatedSide(const TExpense& expense,
  TSideWind* const pDS, bool* pManyInvolvs)
{
  bool res = false;

  if(DatedContainer_(expense.key, expense.kind, expense.date).FilledOK() &&
    expense.ucnType && ucnPresent(expense.ucn) && pDS)
  {
    CharPtr crt(1024);

    if(pManyInvolvs)
	*pManyInvolvs = false;
    try
    {
	ostrstream(crt, crt.Size())
	  << "SELECT * FROM " << pDS->Name << " "
	  << "WHERE "
        << "F_TYPE = '" << expense.key.type << "' AND "
        << "F_YEAR = " << expense.key.year << " AND "
        << "F_NO = " << expense.key.no << " AND "
        << "F_KIND = '" << expense.kind << "' AND "
        << "F_DATE = " << expense.date.Value() << " AND "
        << "F_UCN = '" << expense.ucn << "' AND "
	  << "F_UCN_TYPE = '" << expense.ucnType << "' AND "
	  << "F_INVOLVEMENT = '" << expense.involvement << "' "
	  << ";"
        << ends;
      crt.End();
      if(criteria_finished_ok(crt, "PenaltyBookColumns", "SeekDatedSide"))
      {
        char* const c = crt.Str();
	  TCountRecsQuery q(c);

        if(q.getRecsCount() > 1)
          if(pManyInvolvs)
            *pManyInvolvs = true;
        if(q.Read())
        {
          (*pDS) << q;
          res = true;
        }
	}
    }
    CATCH_ANYTHING
    if(res == false)
      clearGroupData(*pDS);
  }
  return res;
}  // PenaltyBookColumns::SeekDatedSide

// ------------------ PenaltyBookColumns - end ------------------


// ------------------ PenaltyBookOutFile - begin ------------------

class PenaltyBookOutFile : public frmHeadOutText
{
  public:  // Construction / destruction
    PenaltyBookOutFile(TWindow* parent, const char* hFName);
  protected :
    virtual void fHOutput(const void* p, char* s, size_t len,
				   int algn, size_t skip, int dCipher
				   );
  public :
    int MinRowsForExpense() const;
    int FormattedHead(const PrtPenaltyBkDlgData& dlgData, bool write = true);
    void WriteItem(PenaltyBookColumns& aBC, const bool first);
    void NextPenaltyToPrint(const int linesNeed, const bool addHead = true);
    void SingneExpencePrtStartFinalChar(const bool start = true);
} ;  // class PenaltyBookOutFile

PenaltyBookOutFile::PenaltyBookOutFile(TWindow* parent,
  const char* hFName):
  frmHeadOutText(parent, hFName)
{
}  // PenaltyBookOutFile::PenaltyBookOutFile

void PenaltyBookOutFile::fHOutput(const void* p, char* s, size_t len,
  int algn, size_t skip, int dCipher)
{
  if(p && s)
    reinterpret_cast<const PrtPenaltyBkDlgData*>(p)->Output(* this, s, len,
      algn, skip, dCipher);
}  // PenaltyBookOutFile::fHOutput

void PenaltyBookOutFile::WriteItem(PenaltyBookColumns& aBC, const bool first)
{
  if(pM.newPageNeed())
    toBeContinued();
  aBC.notConstOutput(*this, first);
  pM.incLines(linesOfItem(first));
}  // PenaltyBookOutFile::WriteItem

int PenaltyBookOutFile::FormattedHead(const PrtPenaltyBkDlgData& dlgData,
  bool write)
{
  return formHead(&dlgData, write);
}  // PenaltyBookOutFile::FormattedHead

int PenaltyBookOutFile::MinRowsForExpense() const
{
  return extraVal1;
}  // PenaltyBookOutFile::MinRowsForExpense

void PenaltyBookOutFile::SingneExpencePrtStartFinalChar(const bool start)
{
  const char ch = start ?
                  (char)(extraVal / 256) : (char)(extraVal % 256);

  if(ch)
    (* this) << ch;
}  // PenaltyBookOutFile::SingneExpencePrtStartFinalChar

// ------------------ PenaltyBookOutFile - end ------------------


// ------------------ PenaltyBookCriteria - begin ------------------

class PenaltyBookCriteria: public SQLCriteria
{
  public:  // Construction / Destruction
    PenaltyBookCriteria(PrtPenaltyBkDlgData& dlgDataRef,
      const size_t size = 1024);
	protected:  // Methods
    virtual bool ArgsOk();
    virtual void What(ostream& os);
    virtual void From(ostream& os);
    virtual void Where(ostream& os);
    virtual void OrderBy(ostream& os);
    virtual void WhereComposition(ostream& os);
	protected:  // Data
    PrtPenaltyBkDlgData& rDlgData;
};  // class PenaltyBookCriteria

PenaltyBookCriteria::PenaltyBookCriteria(PrtPenaltyBkDlgData& dlgDataRef,
  const size_t size):
  SQLCriteria(size),
  rDlgData(dlgDataRef)
{
}  // PenaltyBookCriteria::PenaltyBookCriteria

bool PenaltyBookCriteria::ArgsOk()
{
  bool rT = SQLCriteria::ArgsOk();

  if(rT)
    if(rDlgData.types[0] == 0)
      rT = false;
  return rT;
}  // PenaltyBookCriteria::ArgsOk

void PenaltyBookCriteria::What(ostream& os)
{
  if (rDlgData.allPenalityType)                   // 20051214
      os << SQL_SELECT << " E.* ";
  else                                            // 20051214
      os << SQL_SELECT << " E.*, P.F_FLAGS ";     // 20051214

  os << " , T.F_ORDER"; // 20060410
}  // PenaltyBookCriteria::What

void PenaltyBookCriteria::From(ostream& os)
{
  os << " FROM ";
  os << "T_EXPENSE E";
  if (!rDlgData.allPenalityType)                   // 20051214
    os << ", T_PENALITY_TYPE P";                   // 20051214
/* 20060410 -> in comment
  if(rDlgData.composition)
  {  os << ", T_SESSION S";
     os << ", T_DECISION D";                       // 20060410
     os << ", T_JUDGEMENT J";                      // 20060410
  }
   20060410 <-*/
  os << ", T_SUM_STATE T";                       // 20060410

}  // PenaltyBookCriteria::From

void PenaltyBookCriteria::Where(ostream& os)
{
  if(ArgsOk())
  {
    os << SQL_WHERE;
    startWithAnd = true;
    os << "E.F_TYPE IN(" << SQLCharSet(rDlgData.types) << ")";
    And(os);
    os << "E.F_SUM_TYPE = '" << SUM_TYPE_PENALITY << "' ";
    if(rDlgData.begD.Empty() == false)
    {
      And(os);
      os << "E.F_DATE >= " << rDlgData.begD.Value() ;
    }
    if(rDlgData.endD.Empty() == false)
    {
      And(os);
      os << "E.F_DATE <= " << rDlgData.endD.Value() ;
    }
    // 20060410 in comment  WhereComposition(os);
  }

  // 20051214 -->
  if (! rDlgData.allPenalityType)
  {
    And(os);
    os << "P.F_PENALITY_TYPE = E.F_PENALITY_TYPE";

    And(os);
    os <<"P.F_FLAGS IN ("  ;
    {
      arrayOfLong flagsArr;

      add_set_bit_values(flagsArr, PENALITY_TYPE_BOOK, MAX_USER_FLAGS);
      os << flagsArr;
    }
    os << ")" ;
  }
  // 20051214 <--

  // 20060410 ->
  And(os);
  os << "T.F_SUM_STATE = E.F_SUM_STATE";
  // 20060410 <-

}  // PenaltyBookCriteria::Where


void PenaltyBookCriteria::WhereComposition(ostream& os)
{
  if(rDlgData.composition)
  {
    And(os);
    os << "( (S.F_NO = E.F_NO";
    And(os);
    os <<  "S.F_YEAR = E.F_YEAR";
    And(os);
    os <<  "S.F_TYPE = E.F_TYPE";
    And(os);
    os <<  "S.F_KIND = E.F_KIND";
    And(os);
    os <<  "S.F_DATE = E.F_DATE";
    And(os);
    os <<  "S.F_COMPOSITION = " << rDlgData.composition;
    os << ")";

    // 20060410 ->
    os << " OR " ;
    os << "(J.F_NO = E.F_NO";
    And(os);
    os <<  "J.F_YEAR = E.F_YEAR";
    And(os);
    os <<  "J.F_TYPE = E.F_TYPE";
    And(os);
    os <<  "J.F_KIND = E.F_KIND";
    And(os);
    os <<  "J.F_DATE = E.F_DATE ";
    And(os);
    os <<  "J.F_UCN_TYPE = E.F_UCN_TYPE ";
    And(os);
    os <<  "J.F_UCN = E.F_UCN ";
    And(os);
    os <<  "J.F_INVOLVEMENT = E.F_INVOLVEMENT ";

    And(os);
    os <<     " D.F_NO = J.F_NO AND D.F_YEAR = J.F_YEAR AND D.F_TYPE = J.F_TYPE AND D.F_KIND = J.F_KIND AND D.F_DATE = J.F_DATE ";
    os << " AND S.F_NO = D.F_NO AND S.F_YEAR = D.F_YEAR AND S.F_TYPE = D.F_TYPE AND S.F_KIND = D.F_SESSION_KIND AND S.F_DATE = D.F_DATE ";

    And(os);
    os << " S.F_COMPOSITION = " << rDlgData.composition;

    os << ") )";
    // 20060410 <-
  }
}  // PenaltyBookCriteria::WhereComposition


void PenaltyBookCriteria::OrderBy(ostream& os)
{
  os << SQL_ORDER_BY;
//  os << " E.F_DATE, E.F_TYPE, E.F_YEAR, E.F_NO";
  os << " E.F_DATE, E.F_TYPE, E.F_YEAR, E.F_NO, T.F_ORDER";   // 20060410
}  // PenaltyBookCriteria::OrderBy

// ------------------ PenaltyBookCriteria - end ------------------


// ------------------ PenaltyBook - begin ------------------

class TPenaltyBookThread: public TWaitThread
{
  public:  // Construction / destruction
    TPenaltyBookThread(const PrtPenaltyBkDlgData *dlgDataPtr = NULL);
  public:  // Methods
    void PrintPenaltyBook();
    void PrintSinglePenalty(const TExpense& expense);
  protected:  // Methods;
    virtual void Execute();
    bool MakeFormatFN(const bool single);
    void WriteOnePenalty(const TExpense& expense,
				  const int serialNo );
  protected:  // Data
    bool fakeRun;
    char formatFN[128];
    PrtPenaltyBkDlgData dlgData;
    PenaltyBookOutFile of;
} ;  // class TPenaltyBookThread

TPenaltyBookThread::TPenaltyBookThread(const PrtPenaltyBkDlgData *dlgDataPtr):
  of(wtdl->Parent, NULL)
{
  fakeRun = false;
  formatFN[0] = '\0';

  if (dlgDataPtr)
	dlgData << dlgDataPtr;
}

bool TPenaltyBookThread::MakeFormatFN(const bool single)
{
  const char* const commonPart = "text\\penalt";

  formatFN[0] = '\0';
  {
    ostrstream oss(formatFN, sizeof(formatFN));
    const size_t typesSelected = strlen(dlgData.types);
    const char typeMark = typesSelected == 1 ? dlgData.types[0] : 'g';
    const char outputKind = single ? '_' : 'y';

    if(typesSelected)
    {
	oss << commonPart << outputKind << typeMark
	  << DEFAULT_FORMAT_FILE_EXTENSION << ends;
    }
    oss << ends;
  }
  ENDING_ZERO(formatFN);
  {
    const int desiredLen =
	strlen(commonPart) + 2 + strlen(DEFAULT_FORMAT_FILE_EXTENSION);

    return (strlen(formatFN) == desiredLen);
  }
}

void TPenaltyBookThread::WriteOnePenalty(const TExpense& expense,
  const int serialNo)
{
  PenaltyBookColumns cols(formatFN, wtdl->Parent, expense, serialNo,
    of.MinRowsForExpense());

  if(of.pageLinesLeft() < cols.getMaxRows() + 1)
  {
    of.footherLine();
    of.newPage();
    of.FormattedHead(dlgData);
    of.head();
  }
  of.horzLine();
  for(int i = 0; i < cols.getMaxRows(); i ++)
    of.WriteItem(cols, true);
}

void TPenaltyBookThread::Execute()
{
  if(fakeRun == false)
  {  // -- -1 --
    try
    {  // -- 0 --
	{  // -- 1 --
	  of.initFF(formatFN);
	  PenaltyBookCriteria criteria(dlgData);
	  long int current = 0;

	  wtdl->SetHeader("Книга Глоби");
	  wtdl->PleaseWait();

	  if(of && criteria.Compose())
	  {  // -- 2 --
	    char* const crit = criteria.Criteria();
	    TCountRecsQuery q(crit);
	    const long int expTotal = q.getRecsCount();
	    TExpense expense;

	    wtdl->SetProgressRange(0, expTotal);
	    of.title();
	    of.FormattedHead(dlgData);
	    of.head();
	    while(q.Read() && wtdl->WantBreak() == false)
	    {  // -- 3 --
		expense << q;
		current++;
		if(expense.Get() == false)
		  continue;
		{
		  char sessDesc[160];

		  ostrstream(sessDesc, sizeof(sessDesc))
		     << DatedContainer_(expense.key, expense.kind, expense.date)
		     << " (" << (current)
		     << " от " << expTotal << ")" << ends;
		  sessDesc [sizeof(sessDesc) - 1] = '\0';
		  wtdl->SetText(sessDesc);
		}

		if ( dlgData.composition )  // 20060411 Check composition ->
		{
		    TSession rSess ;
		    if ( strchr(KIND_SESSIONS, expense.kind) )
		    { // try to get session -> check composition
			 extractSession (expense.key, expense.date, rSess, expense.kind);
		    }
		    else
		    { // try to get decision -> session -> check composition
			  TDecision * pD1 = new TDecision ;
			  pD1 -> key  = expense.key ;
			  pD1 -> kind = expense.kind ;
			  pD1 -> date = expense.date ;

			  if (pD1 -> Try ())
			  if (extractSession (pD1 -> key, pD1 -> date, rSess, pD1 -> sessionKind))
			  delete pD1 ;
		    }
		    if (rSess.composition == dlgData.composition)
			 WriteOnePenalty(expense, dlgData.begSerialNo + current - 1);
		}  // 20060411 Check composition <-
		else
		{
		   WriteOnePenalty(expense, dlgData.begSerialNo + current - 1);
		}

		wtdl->SetProgress(current);
	    }  // -- 3 --
	    wtdl->SetProgress(expTotal);
	    wtdl->AlmostDone();
	    of.footherAndTail () ;
	  }  // -- 2 --
	}  // -- 1 --
    }  // -- 0 --
    CATCH_ANYTHING
  }  // -- -1 --
}

void TPenaltyBookThread::PrintPenaltyBook()
{
    register_usage(REGIME_PENALTY_BOOK, 1, dlgData.types, "Книга глоби");
//  20060522 -> replaced with check box
//    dlgData.allPenalityType =
//        ask ("Потвърдете отпечатването на всички глоби и допълнителни ДТ.") ;
    if(MakeFormatFN(false))
    {
	try
	{
	  ExecWait();
	}
	CATCH_ANYTHING
	show_text_stream_use_settings(NULL, of, formatFN, LEFT_ALIGNMENT);
    }
}

void TPenaltyBookThread::PrintSinglePenalty(const TExpense& expense)
{
  if(expense.sumType == SUM_TYPE_PENALITY)
  {
    clearGroupData(dlgData);
    strConcatCh(dlgData.types, expense.key.type, sizeof(dlgData.types));
    dlgData.begD = dlgData.endD = expense.date;
    if(MakeFormatFN(ask("Част (Yes) или всички (No) колони от книга глоби?")))
    {
	{
	  of.initFF(formatFN);
	  of.SingneExpencePrtStartFinalChar();
	  of << DatedContainer_(expense.key, expense.kind, expense.date) << endl;
	  {
	    char tmp[SIZE_OF_NAME_FOR_BOOK];

	    if(ucnAndUcnTypeToName(expense.ucn, expense.ucnType, tmp,
		sizeof(tmp)))
		of << tmp << endl;
	  }
	  WriteOnePenalty(expense, 0);
	  of.horzLine();
	  of.SingneExpencePrtStartFinalChar(false);
	  of.newPage();
	  of << endl;
	}
	show_text_stream_use_settings(NULL, of, formatFN, LEFT_ALIGNMENT);
    }
  }
  else
    error("%s\n%s::%s",
	WRONG_ARGS_PASSED_TO, typeid(*this).name(), "PrintSinglePenalty");
}

// ------------------ PenaltyBook - end ------------------

// ------------------ some global functions - begin ------------------

void print_penalty_book(TWindow *pParent)
{
	PrtPenaltyBkDlgData dlgData;

	if (TPrintPenaltyBookDialog(pParent, dlgData).Execute() == IDOK)
	{
		TWaitThreadDialog wtdl;
		TPenaltyBookThread(&dlgData).PrintPenaltyBook();
	}
}

void print_single_penalty(TWindow* const,  // const  // 11.oct.2004
  TExpense* pExpense)
{
  if(pExpense &&
    DatedContainer_(pExpense->key, pExpense->kind, pExpense->date).FilledOK() &&
    pExpense->ucnType && ucnPresent(pExpense->ucn))
  {
    switch(pExpense->sumType)
    {
	case SUM_TYPE_PENALITY:
	{
	  register_usage(REGIME_PENALTY_BOOK_PATCH, pExpense->key.type, 1,
	    "Фиш глоба");
	  TWaitThreadDialog wtdl;
	  TPenaltyBookThread().PrintSinglePenalty(*pExpense);
	}
	break;
	case SUM_TYPE_REWARD:
	{
	  fatal("single_penalty(REWARD) is obsolete, use print_rko() instead");
	}
	break;
	default:
	// Nothing
	break;
    }
  }
  else
    error("%s\n%s", WRONG_ARGS_PASSED_TO, "print_single_penalty");
}  // print_single_penalty

// ------------------ some global functions - end ------------------
