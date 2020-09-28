#include "cac.h"  // 18.may.2001
//# include "all.h"

// 2010:011 LPR: rewrote print snipped dlgData,dialog
class TSnippetDlgData : public TDummyGroup
{
public:
	TSnippetDlgData();

	long newFormat;
	long allColumns;
};

TSnippetDlgData::TSnippetDlgData()
{
	Add(
		new TRadio("F_NEW_FORMAT", &newFormat, TRUE),
		new TRadio("F_OLD_FORMAT", &newFormat, FALSE),
		new TRadio("F_ALL_COLUMNS", &allColumns, TRUE),
		new TRadio("F_SEL_COLUMNS", &allColumns, FALSE),
		NULL
	);
	
	newFormat = TRUE;
}

class TPrintSnippetDialog : public TGroupDialog
{
public:
	TPrintSnippetDialog(TWindow *parent, TSnippetDlgData &dlgData);
};

TPrintSnippetDialog::TPrintSnippetDialog(TWindow *parent, TSnippetDlgData &dlgData)
	: TGroupDialog(parent, &dlgData, IDD_PRINT_SNIPPET)
{
	new TRadioFace(this, IDC_PRINT_SNIPPET_NEW_FORMAT, "F_NEW_FORMAT");
	new TRadioFace(this, IDC_PRINT_SNIPPET_OLD_FORMAT, "F_OLD_FORMAT");
	new TRadioFace(this, IDC_PRINT_SNIPPET_ALL_COLUMNS, "F_ALL_COLUMNS");
	new TRadioFace(this, IDC_PRINT_SNIPPET_SEL_COLUMNS, "F_SEL_COLUMNS");
}

bool extractSession (
//                     const  09.jun.2000
			     TDatedContainer * pContainer, TSession & sess,
			   const char * & txt, char & dJResult
			   , bool & personalData  // 09.jun.2000
			   , long & event_no // 20051202
                    )
 {
  bool found = false ;
  char sessKind = '\0' ;  // 09.jun.2000

  personalData = true ;  // 09.jun.2000
  dJResult = '\0' ;
  txt = NULL ;

  if (! pContainer)
    return false ;
   {
    TDecision * pD = dynamic_cast<TDecision *> (pContainer);

    if (pD)
     {
      txt = str(pD -> text) ;
      dJResult = pD -> result ;
      sessKind = pD -> sessionKind ;  // 09.jun.2000
      personalData = pD -> PersonalData () ;  // 09.jun.2000
      event_no = pD -> eventNo;
     }
   }
  if (txt)
   {
    char criteria [128+32] ;

     {
      ostrstream oss (criteria, sizeof (criteria)) ;

      oss <<
        "SELECT * FROM T_SESSION "
          "WHERE F_NO = " << pContainer -> key.no <<
           " AND F_YEAR = " << pContainer -> key.year <<
           " AND F_TYPE = \'" << pContainer -> key.type << "\' " ;
      if (sessKind)
        oss <<
           " AND F_KIND = \'" << sessKind << "\' " ;
      oss <<
           " AND F_DATE = " << pContainer -> date.Value() << " ;"
          << ends ;
     }
     {
      TQuery q (criteria) ;
      unsigned recsCount = 0 ;

      while (q.Read ())
        recsCount ++ ;
      q.Restart () ;
      if (recsCount == 1 && q.Read ())
       {
        sess << q ;
        found = true ;
       }
     }
   }
  return (found && txt) ;
 }  // extractSession

void printDecisionOrJudgement(TWindow * pParent, TDatedContainer * pContainer, TLawsuit *pLawsuit, const CDate *pDateReturned)
{
	if (!(pContainer && pLawsuit))
		return ;

	TSnippetDlgData dlgData;

	if (TPrintSnippetDialog(pParent, dlgData).Execute() != IDOK)
		return;

	register_usage(REGIME_DECISION_OR_JUDGEM_PATCH, pContainer->key.type, 1, "Фиш краен док.");

	char *formatFN = makeTimeBookFormatFN(
	#if FOLLOW_LAWSUIT_TYPES
		pContainer->key.type,
	#else
		pLawsuit->college,
	#endif
	#if INSTANCE
	#if APPEAL
		false, false,
	#else  // of APPEAL
		strchr(KIND_1ST_LAWSUITS, pLawsuit->kind) != NULL,
		(pLawsuit->key.type == TYPE_PUNISHMENT_LAW) &&
		strchr(KIND_2ND_PUNISHMENT_LAWS, pLawsuit -> kind) &&
		(pLawsuit->sourceKind == KIND_CONCERN_COMPLAINT_PUN_INDOC),
	#endif  // of APPEAL
	#endif
		dlgData.newFormat); // false, //const bool jan2005Format,

	if (formatFN)
	{ // -- 1 --
		//timeBookOutFile outF(pParent, formatFN);
		timeBookOutFile outF(pParent);

		if (strlen(formatFN) > 6)
		{ // -- 1a --
			if (dlgData.allColumns)
			{
			}
			else
			{
				formatFN[5] = 's';
				formatFN [6] = '_';
			}

			outF.initFF(formatFN);

			const char * text;
			long event_no;
			TSession sess;
			char dJResult = '\0';
			bool personalData = true;

			if (extractSession(pContainer, sess, text, dJResult, personalData, event_no))
			{  // -- 2 --
				timeBookHeap tBH;
				accusedsDistribution accDistrHeap;

				if (pDateReturned)
					sess.returned = (*pDateReturned);  // !!!!!!!!!!!!!

				timeBookColumns tBC(formatFN, pParent, sess, * pLawsuit, 0, tBH, accDistrHeap,
					NULL,  // TStringArray *itemsPtr,  // 27.jan.2005
					NULL, outF.minRowsForSess ()
					//, personalData
					);

				tBC.arrangeDesJudgColumns(sess.result, pContainer -> kind,
			#if INSTANCE
			#if APPEAL
				 dJResult,
			#else  // of APPEAL
				 strchr (KIND_2ND_LAWSUITS,
					   pLawsuit -> kind) ?
				 dJResult : '\0' ,
			#endif  // of APPEAL
			#else
				 '\0',
			#endif
				 text, ! personalData,
				 outF.minRowsForSess(),
				 event_no);

				const int rows = tBC.getMaxRows();

				outF.oneSessPrtStartFinalChar();
				// outF << DatedContainer_ (* pContainer) << endl;     // 20051114

			#if 20160126
				{
					const probableMaxCols = dlgData.allColumns ? 220 :
						pLawsuit->key.type == TYPE_PUNISHMENT_LAW ? 95 : 105;
					mstr m;

					m.printf("%s %ld/%ld г.", Type->Seek(pContainer->key.type), pContainer->key.no,
						pContainer->key.year);
					m.printf(", Предмет %s %s", pLawsuit->Code(),
						Subject->Seek(pLawsuit->key.type, pLawsuit->subject));
					m.printf(", %s от %0D г.", Kind->Seek(pContainer->kind), &pContainer->date);

					while (m.len())
					{
						int i, w;

						for (i = 0; i < probableMaxCols && i < m.len(); i++)
							if (isspace(m.str()[i]))
								w = i;

						if (i == m.len())
							w = i;

						for (i = 0; i < w; i++)
							outF << m.str()[i];

						outF << endl;
						m.delead(i);

						for (i = 0; i < m.len() && isspace(m.str()[i]); i++);
						m.delead(i);
					}
				}
			#else  // 20160126
				outF << TRCDKey_ (pContainer->key.type, pContainer->key.no, pContainer->key.year);
				outF << ", " ;
				{  // subject
					const size_t buffSz = MAKE_COLUMNS_BUFF_SIZE;
					const size_t buffL = buffSz - 1;
					char *buff = new char[buffSz];

					subjectToString(pLawsuit->key.type, pLawsuit->subject, buff, buffL);
					outF << "Предмет " << pLawsuit->Code() << " " << buff << ", ";
					delete[] buff;
				}
				{  // decision
					TKind &k = *Kind;
					k.kind = pContainer->kind;
					if (k.kind && k.Try())
						outF << k.name;
					else
						outF << "?";
				}
				outF << " от " << pContainer->date;
				outF << " г.";
				outF << endl;
			#endif  // 20160126

				outF.horzLine();
				for (int i = 0; i < rows; i++)
					outF.writeItem (tBC, true);
				outF.horzLine();
				outF.oneSessPrtStartFinalChar(false);
				outF.newPage();
				outF << endl;
			}  // -- 2 --
		}  // -- 1a --

		show_text_stream_use_settings(NULL, outF, formatFN, FLOAT_ALIGNMENT);
		delete[] formatFN;
	}  // -- 1 --
}  // printDecisionOrJudgement

void print_undo_result(TWindow * pParent, const TDatedContainer *const pContainer, TLawsuit * const pLawsuit)
{
	if (!(pContainer && pLawsuit))
		return;

	const TSession *pSess = dynamic_cast<const TSession *>(pContainer);

	if (!(pSess && strchr(RESULT_UNDOS, pSess->result)))
		return;

	CharPtr crit(512);

	{
		const CDate ctrlDate = pSess->kind && strchr(KIND_CLOSED_SESSIONS, pSess->kind) ?
			pSess -> returned : pSess->date;
		ostrstream oss(crit, crit.Size());

		oss << "SELECT F_TYPE, F_YEAR, F_NO, F_DATE, F_KIND "
			"FROM T_SESSION WHERE "
			"F_TYPE = \'" << pSess->key.type << "\' AND "
			"F_YEAR = " << pSess->key.year << " AND "
			"F_NO = " << pSess->key.no << " AND "
			// 2008:315 IRQ: FINALIZEDS -> COMPLETIZEDS
			"F_RESULT IN (" << SQLCharSet(RESULT_COMPLETIZEDS)
			<< ") AND "
			"F_DATE < " << ctrlDate.Value() << " "
			"ORDER BY F_DATE DESC "";" << ends;
	}

	crit[crit.Size() - 1] = '\0';

	if (!criteria_finished_ok(crit, "print_undo_result, search rejected"))
		return;

	SessPtr rejected;

	clearGroupData(*rejected);
	rejected.Ref().key = pSess->key;
	{
		TQuery q(crit);

		if (q.Read())
			(*rejected) << q;
	}

	if (!_select_session(pParent, rejected, _KIND_V_ALL_SESSIONS, RESULT_COMPLETIZEDS))
		return;

	TSnippetDlgData dlgData;

	if (TPrintSnippetDialog(pParent, dlgData).Execute() == IDOK)
	{
		char *formatFN = makeTimeBookFormatFN(rejected.Ref().key.type,
		#if INSTANCE
		#if APPEAL
			false, false,
		#else  // of APPEAL
			strchr(KIND_1ST_LAWSUITS,
			pLawsuit->kind) != NULL,
			pLawsuit->key.type == TYPE_PUNISHMENT_LAW &&
			strchr(KIND_2ND_PUNISHMENT_LAWS, pLawsuit->kind) &&
			pLawsuit->sourceKind == KIND_CONCERN_COMPLAINT_PUN_INDOC,
		#endif  // of APPEAL
		#endif
			// false, false, false);  // 08.feb.2005 temporary
			dlgData.newFormat); // false, //const bool jan2005Format,

		register_usage(REGIME_UNDO_RESULT_PATCH, pSess->key.type, 1, "Фиш отмяна ход");

		if (formatFN)
		{  // -- 1 --
			timeBookOutFile outF(pParent);

			if (strlen(formatFN) > 6)
			{  // -- 1a --
				if (dlgData.allColumns)
				{
				}
				else
				{
					formatFN[5] = 's';
					formatFN[6] = '_';
				}

				outF.initFF(formatFN);

				timeBookHeap tBH;
				accusedsDistribution accDistrHeap;

				timeBookColumns tBC(formatFN, pParent, *rejected,
					*pLawsuit, 0, tBH, accDistrHeap,
					NULL,  // TStringArray *itemsPtr,  // 27.jan.2005
					NULL,  // 08.feb.2002
					outF.minRowsForSess(), true, &(pSess->date),
					&(pSess->returned), 0, pSess);
				const int rows = tBC.getMaxRows();

				outF.oneSessPrtStartFinalChar();
				//outF << DatedContainer_ (*rejected) << endl ; // 20051114
				outF << TRCDKey_(pSess->key.type, pSess->key.no, pSess->key.year);
				outF << ", " ;
				{  // subject
					const size_t buffSz = MAKE_COLUMNS_BUFF_SIZE;
					const size_t buffL = buffSz - 1;
					char *buff = new char [buffSz];

					subjectToString(pLawsuit->key.type, pLawsuit->subject, buff, buffL);
					outF << "Предмет " << pLawsuit->Code() << " " << buff << ", " ;
					delete[] buff;
				}
				{  // decision
					TKind &k = *Kind;

					k.kind = pContainer->kind;

					if (k.kind &&k.Try())
						outF << k.name;
					else
						outF << "?" ;
				}
				outF << " от " << pContainer->date;
				outF << " г.";
				outF << endl;
				outF.horzLine();
				for (int i = 0 ; i < rows; i++)
					outF.writeItem(tBC, true);
				outF.horzLine();
				outF.oneSessPrtStartFinalChar(false);
				outF.newPage();
				outF << endl;
			}  // -- 1a --

			show_text_stream_use_settings(NULL, outF, formatFN, FLOAT_ALIGNMENT);
			delete[] formatFN;
		}  // -- 1 --
	} // Snipped Dialog Executed
} // print_undo_result

void print_end_docs_and_surroundments (TWindow *pParent, TSession *pSess, const TGArray<TSurroundment> *pSurroundments)
{
	if (!(pSess && pSurroundments))
	{
		const char *const funcName = "print_end_docs_and_surroundments";
		const char *const isNull = " argument is NULL";

		if (pSess == NULL)
			error("%s:\n""the pSess%s", funcName, isNull);

		if (pSurroundments == NULL)
			error ("%s:\n""the pSurroundments%s", funcName, isNull) ;
	}


	if (!DatedContainer_(* pSess).FilledOK())
		return;

	TLawsuit law;
	law.key = pSess->key;

	if (!law.Get())
		return;

	TSnippetDlgData dlgData;

	if (TPrintSnippetDialog(pParent, dlgData).Execute() == IDOK)
	{
		char *formatFN = makeTimeBookFormatFN (pSess -> key.type,
		#if INSTANCE
		#if APPEAL
			false, false,
		#else  // of APPEAL
			strchr(KIND_1ST_LAWSUITS, law.kind) != NULL,
			law.key.type == TYPE_PUNISHMENT_LAW && strchr (KIND_2ND_PUNISHMENT_LAWS, law.kind) &&
			law.sourceKind == KIND_CONCERN_COMPLAINT_PUN_INDOC,
		#endif  // of APPEAL
		#endif
			dlgData.newFormat); // false, //const bool jan2005Format,

		register_usage(REGIME_ENDDOCS_SURRMENTS_PATCH, pSess->key.type, 1, "Фиш Съпр. док");

		if (formatFN)
		{  // -- 1 --
			timeBookOutFile outF(pParent);

			if (strlen(formatFN) > 6)
			{  // -- 1a --
				if (dlgData.allColumns)
				{
				}
				else
				{
					formatFN [5] = 's';
					formatFN [6] = '_';
				}

				outF.initFF(formatFN);

				timeBookHeap tBH;
				accusedsDistribution accDistrHeap;
				timeBookColumns tBC(formatFN, pParent, * pSess, law, 0, tBH, accDistrHeap,
						  NULL,  // TStringArray *itemsPtr,  // 27.jan.2005
						  NULL, outF.minRowsForSess());

				tBC.decisionsAndSurroundments(*pSurroundments, *pSess, law);

				const int rows = tBC.getMaxRows();

				outF.oneSessPrtStartFinalChar();
				//outF << DatedContainer_ (*pSess) << endl;      // 20051114
				outF << TRCDKey_ (pSess->key.type, pSess->key.no, pSess->key.year);
				outF << ", " ;
				{  // subject
					const size_t buffSz = MAKE_COLUMNS_BUFF_SIZE;
					const size_t buffL = buffSz - 1;
					char *buff = new char[buffSz];

					subjectToString (law.key.type, law.subject, buff, buffL);
					outF << "Предмет " << law.Code() << " " << buff << ", ";
					delete[] buff;
				}
				{  // decision
					TKind &k = *Kind;

					k.kind = pSess->kind;

					if (k.kind && k.Try())
						outF << k.name;
					else
						outF << "?";
				}
				outF << " от " << pSess->date;
				outF << " г.";
				outF << endl;
				outF.horzLine ();
				for (int i = 0 ; i < rows; i++)
					outF.writeItem(tBC, true);
				outF.horzLine();
				outF.oneSessPrtStartFinalChar(false);
				outF.newPage();
				outF << endl;
			}  // -- 1a --

			show_text_stream_use_settings (NULL, outF, formatFN, FLOAT_ALIGNMENT);
			delete[] formatFN ;
		}  // -- 1 --
	}  // if(TPrintSnippetDialog(pParent, dlgData).Execute() == IDOK)
}  // print_end_docs_and_surroundments
