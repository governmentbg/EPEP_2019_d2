#include "cac.h"

// 2017:269 FIX: int date, int finished -> law date, law finished

// ----- ENUMS -------------------------------------------------------------
enum
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
	TAB_JUDGEMENT = -1,
#endif  // ADMIN
	TAB_LAWSUIT,
	TAB_SESSION,
	TAB_DECISION,
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	TAB_JUDGEMENT,
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	PAGE_COUNT
};

enum
{
	CUTDOWN_ANY,
	CUTDOWN_CUTDOWN,
	CUTDOWN_UNSPECIFIED,
	CUTDOWN_P1,
	CUTDOWN_P2,
	CUTDOWN_P1_AND_P2,
	CUTDOWN_NOT_CUTDOWN,
};

enum
{
	ORDER_BY_SEND	= ORDER_BY_USER,
	ORDER_BY_ART,
	ORDER_BY_FORCE
};

// ----- TFloatComboGroup --------------------------------------------------
class TFloatComboGroup : public TFloatGroup
{
public:
	TFloatComboGroup();

	virtual void FormCriteria(msql &m);

	//----------------------Lawsuit----------------------------------
	char lawKinds[SIZE_OF_KINDS];
	long lawMinNo;
	long lawMaxNo;
	long lawMinYear;
	long lawMaxYear;
	long firstSet;
	char colleges[SIZE_OF_COLLEGES];
	char lawCompositions[SIZE_OF_COMPOSITIONS];
	CDate lawMinDate;
	CDate lawMaxDate;
	long doublex;
	CDate minFinished;
	CDate maxFinished;
	TGArray<TSubject> newSubjects;
	TGArray<TOldSubject> oldSubjects;
	long subjectText;
	TGroupArray lawJudges;
	long partof;
	char ages[SIZE_OF_AGES];
	CDate lawMinForceDate;
	CDate lawMaxForceDate;
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	char grants[SIZE_OF_GRANTS];
#endif  // APPEAL || ADMIN || MILITARY || REGIONAL
	CDate minReRegister;
	CDate maxReRegister;
	TGroupArray dangers;
	char generalKinds[SIZE_OF_KINDS];
	long quick;
	long lawOrder;
	char lawText[SIZE_OF_ORIGIN_TEXT];
	long cutDown;
	long important;
	long newGPK;				// 2008:080 +related
	long limited;				// 2008:284 +related
	char eisppNmr[SIZE_OF_EISPP_NMR];
	long children;				// 2016:105 +related
	long lawsuitText;				// 2018-03-20 +related
	long pt329a3;				// 2018-06-08 +related

	//----------------------------Session--------------------------
	char sesKinds[SIZE_OF_KINDS];
	CDate sesMinDate;
	CDate sesMaxDate;
	CDate sesMinReturned;
	CDate sesMaxReturned;
	long sesMinEventNo;			// 2005:042
	long sesMaxEventNo;			// 2005:042
	CDate sesMinForceDate;			// 2011:343
	CDate sesMaxForceDate;			// 2011:343
	long invasj;
	TGroupArray secretars;
	TGroupArray sesJudges;
	char sesCompositions[SIZE_OF_COMPOSITIONS];	// 2003:273
	char sesResults[SIZE_OF_RESULTS];
	long sessionChanjed;
	long proceed;				// 2003:115
	long closedd;				// 2007:261
	long firstSes;
	long expert;
	char sesText[SIZE_OF_SESSION_TEXT];	// 2003:115
	long extended;
	long sesDateAfterFinish;
	long sesReturnAfterFinish;
	long sesFinalizing;			// 2012:046 LRQ: +related
	long sesAppeal;				// 2012:180 LRQ: +related
	TGArray<TSLCReason> sesSLCReasons;	// 2015:078 +related
	TGroupArray jurors;
	long sesPublTreg;

	//---------------------------Decision---------------------------
	char decResults[SIZE_OF_RESULTS];
	long surrouendocs;			// 2016:313 +related
	CDate minReturned;
	CDate maxReturned;
	char decKinds[SIZE_OF_KINDS];
	CDate minFromDate;
	CDate maxFromDate;
	CDate decMinForceDate;
	CDate decMaxForceDate;
	long confess;
	long absence;
	// 2008:133 +related
	long minPartition;
	long maxPartition;
	long minVolume;
	long maxVolume;
	bool variantReg;
	long motives;
	long opinion;
	char accomplys[SIZE_OF_ACCOMPLYS];
	char decText[SIZE_OF_DECISION_TEXT];	// 2003:115
	char debtorStatus[SIZE_OF_KINDS];
	long decAppeal;
	TGArray<TSLCReason> decSLCReasons;	// 2015:078 +related
	long decPublTreg;
	long decContract;
	char keyWords[SIZE_OF_KEYWORDS];

	//---------------------------Judgement---------------------------
	char judJudgeds[SIZE_OF_JUDGEDS];
	TGroupArray judArticles;
	char judPunishments[SIZE_OF_PUNISHMENTS];
	char judIntervals[SIZE_OF_INTERVALS];
	CDate judMinSend;
	CDate judMaxSend;
	CDate judMinForceDate;
	CDate judMaxForceDate;
	long appendix23;
	long appendix24;
	long appendix25;
	long appendix26;
	long judYoung;
	long judOrder;
	char judPunishmentsX[SIZE_OF_PUNISHMENTS];
	long judReport;	// 2018-02-09 +related

	int tabSelect;
};

TFloatComboGroup::TFloatComboGroup()
	: TFloatGroup("T_LAWSUIT L")
{
	Add(
		//-------------------------Lawsuit--------------------------
		new TCharList("F_KINDS", lawKinds, sizeof lawKinds),
		new TLong("F_MIN_NO", &lawMinNo),
		new TLong("F_MAX_NO", &lawMaxNo),
		new TLong("F_MIN_YEAR", &lawMinYear),
		new TLong("F_MAX_YEAR", &lawMaxYear),
		new TCheck("F_FIRST_SET", &firstSet, TRUE),	// 2004:007
		new TCharList("F_COLLEGES", colleges, sizeof colleges),
		new TCharList("F_LAW_COMPOSITIONS", lawCompositions, sizeof lawCompositions),
		new TDate("F_LAW_MIN_DATE", &lawMinDate, false),
		new TDate("F_LAW_MAX_DATE", &lawMaxDate, false),
		new TDate("F_MIN_FINISHED", &minFinished, false),
		new TDate("F_MAX_FINISHED", &maxFinished, false),
		new TCheck("F_SUBJECT_TEXT", &subjectText, TRUE),
		new TCheck("F_PART_OF", &partof, TRUE),		// 2004:007
		new TCharList("F_AGES", ages, sizeof ages),
		new TDate("F_LAW_MIN_FORCE_DATE", &lawMinForceDate, false),
		new TDate("F_LAW_MAX_FORCE_DATE", &lawMaxForceDate, false),
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		new TCharList("F_GRANTS", grants, sizeof grants),
#endif  // APPEAL || ADMIN || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
		new TDate("F_MIN_REREGISTER", &minReRegister, false),
		new TDate("F_MAX_REREGISTER", &maxReRegister, false),
#endif  // AREAL
		new TCharList("F_GENERAL_KINDS", generalKinds, sizeof generalKinds),
		new TRadio("F_SLOW", &quick, QUICK_NULL),
		new TRadio("F_QUICK", &quick, QUICK_QUICK),
		new TRadio("F_IMMEDIATE", &quick, QUICK_QUICK | QUICK_IMMEDIATE),
		new TRadio("F_ANY_SPEED", &quick, QUICK_ANY_SPEED),
		new TString("F_TEXT", lawText, sizeof lawText),		// 2003:114
		new TCheck("F_IMPORTANT", &important, LAWSUIT_IMPORTANT),
		new TCheck("F_NEWGPK", &newGPK, LAWSUIT_NEWGPK),
		new TCheck("F_LIMITED", &limited, LAWSUIT_LIMITED),
		new TRadio("F_LAW_BY_TYN", &lawOrder, ORDER_BY_TYN),
		new TRadio("F_LAW_BY_TSYN", &lawOrder, ORDER_BY_TSYN),
		new TRadio("F_LAW_BY_TKYN", &lawOrder, ORDER_BY_TKYN),
		new TRadio("F_LAW_BY_JTYN", &lawOrder, ORDER_BY_JTYN),
		new TString("F_EISPP_NMR", eisppNmr, sizeof eisppNmr),
		// 2014:163 was cutdown flag
		new TRadio("F_CUTDOWN_ANY", &cutDown, CUTDOWN_ANY),
		new TRadio("F_CUTDOWN_CUTDOWN", &cutDown, CUTDOWN_CUTDOWN),
		new TRadio("F_CUTDOWN_UNSPECIFIED", &cutDown, CUTDOWN_UNSPECIFIED),
		new TRadio("F_CUTDOWN_P1", &cutDown, CUTDOWN_P1),
		new TRadio("F_CUTDOWN_P2", &cutDown, CUTDOWN_P2),
		new TRadio("F_CUTDOWN_P1_AND_P2", &cutDown, CUTDOWN_P1_AND_P2),
		new TRadio("F_CUTDOWN_NOT_CUTDOWN", &cutDown, CUTDOWN_NOT_CUTDOWN),
		new TCheck("F_DOUBLEX", &doublex, LAWSUIT_X_DOUBLEX),
		new TCheck("F_CHILDREN", &children, LAWSUIT_X_CHILDREN),
		new TCheck("F_LAWSUIT_TEXT", &lawsuitText, TRUE),
		new TCheck("F_PT329A3", &pt329a3, LAWSUIT_Y_PT329A3),

		//------------------------Session------------------------------
		new TCharList("F_SES_KINDS", sesKinds, sizeof sesKinds),
		new TDate("F_SES_MIN_DATE", &sesMinDate, false),
		new TDate("F_SES_MAX_DATE", &sesMaxDate, false),
		new TDate("F_SES_MIN_RETURNED", &sesMinReturned, false),
		new TDate("F_SES_MAX_RETURNED", &sesMaxReturned, false),
		new TLong("F_SES_MIN_EVENT_NO", &sesMinEventNo),
		new TLong("F_SES_MAX_EVENT_NO", &sesMaxEventNo),
		new TDate("F_SES_MIN_FORCE_DATE", &sesMinForceDate, false),
		new TDate("F_SES_MAX_FORCE_DATE", &sesMaxForceDate, false),
		new TCharList("F_SES_COMPOSITIONS", sesCompositions, sizeof sesCompositions),
		new TRadio("F_AS_JUDGE", &invasj, TRUE),
		new TRadio("F_AS_ANY", &invasj, FALSE),
		new TCharList("F_SES_RESULTS", sesResults, sizeof sesResults),
		new TCheck("F_SESSION_CHANJED", &sessionChanjed, SESSION_CHANJED),
		new TCheck("F_PROCEED", &proceed, SESSION_PROCEED),
		new TCheck("F_EXPERT", &expert, SESSION_EXPERT),
		new TString("F_SES_TEXT", sesText, sizeof sesText),
		new TCheck("F_EXTENDED", &extended, TRUE),
		new TCheck("F_SES_DATE_AFTER_FINISH", &sesDateAfterFinish, TRUE),
		new TCheck("F_SES_RETURN_AFTER_FINISH", &sesReturnAfterFinish, TRUE),
		new TCheck("F_SES_FINALIZING", &sesFinalizing, TRUE),
		new TCheck("F_CLOSEDD", &closedd, SESSION_X_CLOSEDD),
		new TCheck("F_FIRST_SES", &firstSes, SESSION_X_FIRSET),
		new TCheck("F_SES_APPEAL", &sesAppeal, SESSION_X_APPEAL),
		new TCheck("F_SES_PUBLTREG", &sesPublTreg, SESSION_Y_PUBLTREG),

		//-------------------------Decision------------------------
		new TCharList("F_DEC_RESULTS", decResults, sizeof decResults),
		new TCheck("F_DEC_SURROUENDOCS", &surrouendocs, TRUE),
		new TDate("F_MIN_RETURNED", &minReturned, false),
		new TDate("F_MAX_RETURNED", &maxReturned, false),
		new TCharList("F_DEC_KINDS", decKinds, sizeof decKinds),
		new TDate("F_MIN_FROM_DATE", &minFromDate, false),
		new TDate("F_MAX_FROM_DATE", &maxFromDate, false),
		new TDate("F_DEC_MIN_FORCE_DATE", &decMinForceDate, false),
		new TDate("F_DEC_MAX_FORCE_DATE", &decMaxForceDate, false),
		new TCheck("F_CONFESS", &confess, DECISION_CONFESS),
		new TCheck("F_ABSENCE", &absence, DECISION_ABSENCE),
		new TLong("F_MIN_PARTITION", &minPartition),
		new TLong("F_MAX_PARTITION", &maxPartition),
		new TLong("F_MIN_VOLUME", &minVolume),
		new TLong("F_MAX_VOLUME", &maxVolume),
		new TCheck("F_MOTIVES", &motives, DECISION_MOTIVES),
		new TCheck("F_OPINION", &opinion, TRUE),
		new TCharList("F_ACCOMPLYS", accomplys, sizeof accomplys),
		new TString("F_DEC_TEXT", decText, sizeof decText),
		new TCharList("F_DEBTOR_STATUS", debtorStatus, sizeof debtorStatus),
		new TCheck("F_DEC_APPEAL", &decAppeal, DECISION_X_APPEAL),
		new TCheck("F_DEC_PUBLTREG", &decPublTreg, DECISION_X_PUBLTREG),
		new TCheck("F_DEC_CONTRACT", &decContract, DECISION_X_CONTRACT),
		new TKeyWords("F_KEYWORDS", keyWords),

		//-------------------------Judgements------------------------
		new TCharList("F_JUDGEDS", judJudgeds, sizeof judJudgeds),
		new TCharList("F_PUNISHMENTS", judPunishments, sizeof judPunishments),
		new TCharList("F_INTERVALS", judIntervals, sizeof judIntervals),
		new TDate("F_MIN_SEND", &judMinSend, false),
		new TDate("F_MAX_SEND", &judMaxSend, false),
		new TDate("F_MIN_FORCE_DATE", &judMinForceDate, false),
		new TDate("F_MAX_FORCE_DATE", &judMaxForceDate, false),
		new TRadio("F_BY_SEND", &judOrder, ORDER_BY_SEND),
		new TRadio("F_BY_ART", &judOrder, ORDER_BY_ART),
		new TRadio("F_BY_FORCE", &judOrder, ORDER_BY_FORCE),
		new TCheck("F_APPENDIX_23", &appendix23, JUDGEMENT_Y_APPENDIX_23),
		new TCheck("F_APPENDIX_24", &appendix24, JUDGEMENT_Y_APPENDIX_24),
		new TCheck("F_APPENDIX_25", &appendix25, JUDGEMENT_Y_APPENDIX_25),
		new TCheck("F_APPENDIX_26", &appendix26, JUDGEMENT_Y_APPENDIX_26),
		new TCheck("F_JUD_YOUNG", &judYoung, SIDEWIND_YOUNG),
		new TCharList("F_PUNISHMENTS_X", judPunishmentsX, sizeof judPunishmentsX),
		new TRadio("F_JUD_REPORT_IGNORE", &judReport, -1),
		new TRadio("F_JUD_REPORT_COUNTED", &judReport, TRUE),
		new TRadio("F_JUD_REPORT_UNCOUNTED", &judReport, FALSE),
		NULL
	);

	tabSelect = 0;
	important = newGPK = limited = -1;
	doublex = children = -1;
	pt329a3 = -1;
	sesDateAfterFinish = sesReturnAfterFinish = -1;
	sesFinalizing = -1;
	sesAppeal = -1;
	sesPublTreg = -1;
	quick = QUICK_ANY_SPEED;
	invasj = TRUE;
	//firstSet = -1;	// 2017:097 auto-3-state +related
	firstSet = TRUE;	// 2017:206 disabled 3-state support
	proceed = closedd = extended = firstSes = expert = sessionChanjed = -1;
	confess = -1;
	absence = -1;
	appendix23 = appendix24 = appendix25 = appendix26 = -1;
	motives = opinion = -1;
	decAppeal = -1;
	decPublTreg = -1;
	decContract = -1;
	judYoung = -1;
	cutDown = CUTDOWN_ANY;
	judOrder = ORDER_BY_SEND;
	lawOrder = ORDER_BY_TYN;
	judReport = -1;
}

void TFloatComboGroup::FormCriteria(msql &m)
{
	bool tabSession = tabSelect == TAB_SESSION || *sesKinds || !sesMinDate.Empty() || !sesMaxDate.Empty() ||
		!sesMinReturned.Empty() || !sesMaxReturned.Empty() || sesMinEventNo || sesMaxEventNo ||
		!sesMinForceDate.Empty() || !sesMaxForceDate.Empty() || sesJudges.Count() || *sesCompositions || *sesResults ||
		secretars.Count() || jurors.Count() || sessionChanjed != -1 || proceed != -1 || closedd != -1 ||
		extended != -1 || firstSes != -1 || expert != -1 || *sesText || sesDateAfterFinish != -1 ||
		sesReturnAfterFinish != -1 || sesFinalizing != -1 || sesAppeal != -1 || sesSLCReasons.Count();

	bool tabDecision;
	bool tabJudgement;

	if (surrouendocs)
	{
		variantReg = false;
		tabDecision = tabSelect == TAB_DECISION || !minFromDate.Empty() || !maxFromDate.Empty() || *decKinds || *decText;
		tabJudgement = false;
	}
	else
	{
		variantReg = minPartition || maxPartition || minVolume || maxVolume;
		tabDecision = tabSelect == TAB_DECISION || *decResults || !minFromDate.Empty() || !maxFromDate.Empty() ||
			!decMinForceDate.Empty() || !decMaxForceDate.Empty() || *decKinds || *accomplys || !minReturned.Empty() ||
			!maxReturned.Empty() || confess != -1 || variantReg || *decText || motives != -1 || absence != -1 ||
			opinion != -1 || *debtorStatus || decAppeal != -1 || decSLCReasons.Count();

		tabJudgement = tabSelect == TAB_JUDGEMENT || *judJudgeds || judArticles.Count() || appendix23 != -1 ||
			appendix24 != -1 || appendix25 != -1 || appendix26 != -1 || *judPunishments || *judIntervals ||
			!judMinSend.Empty() || !judMaxSend.Empty() || !judMinForceDate.Empty() || !judMaxForceDate.Empty() ||
			judYoung != -1 || judReport != -1;
	}

	mstr tables(Name);
	mstr fields("DISTINCT L.F_KIND AS F_LAW_KIND, L.F_NO, L.F_YEAR, L.F_TYPE, L.F_EISPP_NMR");

	tables.cat(" JOIN T_LAW_INTERVAL I ON I.F_NO = L.F_NO AND I.F_YEAR = L.F_YEAR AND I.F_TYPE = L.F_TYPE");
	fields.cat(partof ? ", L.F_REJUDGE AS F_LAW_REJUDGE, " : ", L.F_JUDGE AS F_LAW_JUDGE, ");

	if (tabSession || tabDecision || tabJudgement)
	{
		tables.cat(" JOIN T_SESSION S ON S.F_NO = L.F_NO AND S.F_YEAR = L.F_YEAR AND S.F_TYPE = L.F_TYPE");
		// 2014:297 LPR: TCharAutoListFace always request sessions
		if (!any(sesCompositions))
			strcpy(sesCompositions, QUERY_COMPOSITIONS ? QUERY_COMPOSITIONS : COMPOSITION_ALLS);
	}

	if (tabDecision || tabJudgement)
	{
		if (surrouendocs)
		{
			tables.cat(" JOIN T_SURROUNDMENT D ON D.F_LAWSUIT_NO = L.F_NO AND D.F_LAWSUIT_YEAR = L.F_YEAR "
				"AND D.F_LAWSUIT_TYPE = L.F_TYPE AND D.F_SESSION_KIND = S.F_KIND AND D.F_DATE = S.F_DATE");
		}
		else
		{
			tables.cat(" JOIN T_DECISION D ON D.F_NO = L.F_NO AND D.F_YEAR = L.F_YEAR AND D.F_TYPE = L.F_TYPE "
				"AND D.F_SESSION_KIND = S.F_KIND AND D.F_DATE = S.F_DATE");
		}

	}

	if (tabJudgement)
	{
		tables.cat(" JOIN T_JUDGEMENT J ON J.F_NO = L.F_NO AND J.F_YEAR = L.F_YEAR AND J.F_TYPE = L.F_TYPE "
			"AND J.F_KIND = D.F_KIND AND J.F_DATE = D.F_DATE");

		if (judReport != -1)
			tables.cat(" JOIN T_SUBJECT U ON U.F_SUBJECT = L.F_SUBJECT AND U.F_TYPE = L.F_TYPE");
	}

	switch (tabSelect)
	{
		case TAB_SESSION :
		{
			fields.cat("L.F_DATE AS F_LAW_DATE, S.F_JUDGE AS F_SES_JUDGE, I.F_FINISHED AS F_LAW_FINISHED, "
				"S.F_KIND AS F_SES_KIND, S.F_DATE AS F_SES_DATE, S.F_RESULT AS F_SES_RESULT, "
				"S.F_RETURNED AS F_SES_RETURNED, S.F_FORCE_DATE AS F_SES_FORCE_DATE");
			break;
		}
		case TAB_DECISION :
		{
			const char *tFields;

			if (surrouendocs)
			{
				tFields = "L.F_DATE AS F_LAW_DATE, S.F_KIND AS F_SES_KIND, S.F_DATE AS F_SES_DATE, "
					"S.F_JUDGE AS F_SES_JUDGE, D.F_DECISION_KIND AS F_DEC_KIND, D.F_RESULT AS F_DEC_RESULT, "
					"D.F_DATE AS F_DEC_FROM_DATE, NULL AS F_DEC_RETURNED, NULL AS F_DEC_FORCE_DATE, "
					"D.F_EVENT_NO";	// event # for DISTINCT-ing only
			}
			else if (variantReg)
			{
				tFields = "D.F_PAGE AS F_PAGE, D.F_VOLUME AS F_VOLUME, D.F_PARTITION AS F_PARTITION, "
					"D.F_KIND AS F_DEC_KIND, D.F_EVENT_NO AS F_DEC_EVENT_NO, D.F_FROM_DATE AS F_DEC_FROM_DATE";
			}
			else
			{
				tFields = "L.F_DATE AS F_LAW_DATE, S.F_KIND AS F_SES_KIND, S.F_DATE AS F_SES_DATE, "
					"S.F_JUDGE AS F_SES_JUDGE, D.F_KIND AS F_DEC_KIND, D.F_RESULT AS F_DEC_RESULT, "
					"D.F_FROM_DATE AS F_DEC_FROM_DATE, D.F_RETURNED AS F_DEC_RETURNED, "
					"D.F_FORCE_DATE AS F_DEC_FORCE_DATE";
			}

			fields.cat(tFields);
			break;
		}
		case TAB_JUDGEMENT :
		{
			fields.cat("S.F_JUDGE AS F_SES_JUDGE, L.F_SUBJECT AS F_LAW_SUBJECT, L.F_OLD_SUBJECT AS F_LAW_OLD_SUBJECT, "
				"D.F_DATE AS F_DEC_DATE, D.F_KIND AS F_DEC_KIND, J.F_ARTICLE AS F_JUD_ARTICLE, "
				"D.F_FROM_DATE AS F_DEC_FROM_DATE, J.F_FORCE_DATE AS F_JUD_FORCE_DATE, "
				"J.F_UCN_TYPE AS F_JUD_UCN_TYPE, J.F_UCN AS F_JUD_UCN, J.F_INVOLVEMENT AS F_JUD_INVOLVEMENT, "
				"J.F_JUDGED AS F_JUD_JUDGED, J.F_SEND_DATE AS F_JUD_SEND_DATE");
			break;
		}
		default :
		{
			fields.cat("L.F_SUBJECT AS F_LAW_SUBJECT, L.F_OLD_SUBJECT AS F_LAW_OLD_SUBJECT, I.F_DATE AS F_LAW_DATE, "
				"I.F_FINISHED AS F_LAW_FINISHED, I.F_FLAGS AS F_INT_FLAGS");

			if (subjectText)
			{
				tables.cat(" JOIN T_SUBJECT U ON U.F_SUBJECT = L.F_SUBJECT AND U.F_TYPE = L.F_TYPE");
				fields.cat(", U.F_NAME AS F_SUBJECT_TEXT");
			}

			if (lawsuitText)
				fields.cat(", L.F_TEXT AS F_LAWSUIT_TEXT");
		}
	}

	if (!minReRegister.Empty() || !maxReRegister.Empty())
	{
		tables.cat(" JOIN T_DAMN M ON M.F_NO = L.F_NO AND M.F_YEAR = L.F_YEAR AND M.F_TYPE = L.F_TYPE");
		fields.cat(", M.F_REREGISTER");
	}

	m.Begin(str(tables), str(fields));

	m.AddChars("L.F_KIND", lawKinds, KIND_LAWSUITS);
	m.AddRange("L.F_NO", lawMinNo, lawMaxNo);
	m.AddRange("L.F_YEAR", lawMinYear, lawMaxYear);
	m.AddChars("L.F_COLLEGE", colleges, COLLEGE_ALLS);
	m.AddCompos("L.F_COMPOSITION", lawCompositions);
	m.AddRange("I.F_DATE", lawMinDate, lawMaxDate);
	m.AddRange("I.F_FINISHED", minFinished, maxFinished);
	m.AddArray("L.F_SUBJECT", newSubjects);
	m.AddArray("F_SUBJECT", oldSubjects, "L.F_OLD_SUBJECT");
	m.AddArray("F_UCN", lawJudges, partof ? "L.F_REJUDGE" : "L.F_JUDGE");
	m.AddChars("I.F_AGE", ages, AGE_AGES);	// 2010:166 LPR/LRQ: was AGE_ALLS
	m.AddRange("L.F_FORCE_DATE", lawMinForceDate, lawMaxForceDate);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	m.AddChars("L.F_GRANTED", grants, GRANT_ALLS);
#endif  // APPEAL || ADMIN || REGIONAL
#if COURT_TYPE == COURT_AREAL
	// 2008:073 LPR: using joined reregister
	m.AddRange("M.F_REREGISTER", minReRegister, maxReRegister);
#endif  // AREAL

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	m.AddArray("L.F_DANGER", dangers);
#endif  // AREAL || MILITARY

	m.AddChars("L.F_SOURCE_KIND", adjust_general_kinds(generalKinds), KIND_ALLDOCS);
	m.AddQuick("L.F_QUICK", quick);
	m.AddLike("L.F_EISPP_NMR", eisppNmr, true);

	if (any(lawText))
	{
		m.AddLike("(L.F_TEXT", lawText, true);
		m.NextOr();
		m.AddLike("L.F_IMPORTANT", lawText, true);
		m.cach(')');
	}

	long mask = 0, value = 0;

	if (important != -1)
	{
		mask |= LAWSUIT_IMPORTANT;
		value |= important;
	}

	switch (cutDown)
	{
		case CUTDOWN_CUTDOWN :
		case CUTDOWN_NOT_CUTDOWN : m.AddIsNull("L.F_CUTDOWN", cutDown == CUTDOWN_NOT_CUTDOWN); break;
		case CUTDOWN_UNSPECIFIED : m.AddLong("L.F_CUTDOWN", TRUE); break;
		case CUTDOWN_P1 : m.AddLong("L.F_CUTDOWN", LAWSUIT_CUTDOWN_P1); break;
		case CUTDOWN_P2 : m.AddLong("L.F_CUTDOWN", LAWSUIT_CUTDOWN_P2); break;
		case CUTDOWN_P1_AND_P2 :
		{
			TIntegerArray p;
			p.Add(LAWSUIT_CUTDOWN_P1, LAWSUIT_CUTDOWN_P2, INT_MAX);
			m.AddLongs("L.F_CUTDOWN", p);
		}
	}

	if (newGPK != -1)
	{
		mask |= LAWSUIT_NEWGPK;
		value |= newGPK;
	}
	if (limited != -1)
	{
		mask |= LAWSUIT_LIMITED;
		value |= limited;
	}
	m.AddFlags("L.F_FLAGS", mask, value, LAWSUIT_ALL);

	mask = value = 0;
	if (doublex != -1)
	{
		mask |= LAWSUIT_X_DOUBLEX;
		value |= doublex;
	}
	if (children != -1)
	{
		mask |= LAWSUIT_X_CHILDREN;
		value |= children;
	}
	m.AddFlags("L.F_FLAGS_X", mask, value, LAWSUIT_X_ALL);

	mask = value = 0;
	if (pt329a3 != -1)
	{
		mask |= LAWSUIT_Y_PT329A3;
		value |= pt329a3;
	}
	m.AddFlags("L.F_FLAGS_Y", mask, value, LAWSUIT_Y_ALL);

	if (firstSet != -1)
		m.AddIsNull("I.F_FLAGS", firstSet);

	if (tabSession || tabDecision)
	{
		m.AddCompos("S.F_COMPOSITION", sesCompositions);
		m.AddRange("S.F_DATE", sesMinDate, sesMaxDate);

		if (sesDateAfterFinish != -1)
		{
			m.Add("S.F_DATE");
			m.printf("%s I.F_FINISHED", sesDateAfterFinish ? ">" : "<=");
		}

		if (sesFinalizing != -1)
		{
			if (sesFinalizing)
			{
				m.AddIsNull("I.F_FINISHED", false);
				m.Add("S.F_FINAL");
				m.cat("= I.F_FINISHED");
			}
			else
			{
				m.AddIsNull("(I.F_FINISHED", true);
				m.NextOr();
				m.AddIsNull("S.F_FINAL", true);
				m.NextOr();
				m.Add("NOT S.F_FINAL");
				m.cat("= I.F_FINISHED)");
			}
		}

		m.AddRange("S.F_RETURNED", sesMinReturned, sesMaxReturned);

		if (sesReturnAfterFinish != -1)
		{
			m.Add("S.F_RETURNED");
			m.printf("%s I.F_FINISHED", sesReturnAfterFinish ? ">" : "<=");
		}

		m.AddChars("S.F_KIND", adjust_xxx_kinds(sesKinds, KIND_VISIBLE_CLOSEDS, KIND_CLOSED_SESSIONS), KIND_SESSIONS);
		m.AddRange("S.F_EVENT_NO", sesMinEventNo, sesMaxEventNo);
		m.AddRange("S.F_FORCE_DATE", sesMinForceDate, sesMaxForceDate);

		if (invasj)
			m.AddArray("F_UCN", sesJudges, "S.F_JUDGE");
		else
		{
			m.AddArray("F_UCN", sesJudges, "NOT S.F_JUDGE");
			m.AddTriad("S.F_REJUDGE_X", "S.F_JUDGE_X", "S.F_PRESIDENT", sesJudges, "F_UCN");
		}

		m.AddArray("F_UCN", secretars, "S.F_SECRETAR");
		m.AddChars("S.F_RESULT", sesResults, RESULT_SESSIONS);
	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	#if 20170174
		static constant JUROR_NAMES[] = { "S.F_JUROR", "S.F_REJUROR", "S.F_JUROR_X", NULL };
		m.AddJuSet(JUROR_NAMES, jurors, "F_UCN");
	#else  // 20170174
		m.AddTriad("S.F_JUROR", "S.F_REJUROR", "S.F_JUROR_X", jurors, "F_UCN");
	#endif  // 20170174
	#endif  // AREAL || MILITARY || REGIONAL
		m.AddArray("S.F_SLC_REASON", sesSLCReasons);

		mask = value = 0;
		if (sessionChanjed != -1)
		{
			mask |= SESSION_CHANJED;
			value |= sessionChanjed;
		}
		if (proceed != -1)
		{
			mask |= SESSION_PROCEED;
			value |= proceed;
		}
		if (expert != -1)
		{
			mask |= SESSION_EXPERT;
			value |= expert;
		}
		m.AddFlags("S.F_FLAGS", mask, value, SESSION_ALL);

		mask = value = 0;
		if (closedd != -1)
		{
			mask |= SESSION_X_CLOSEDD;
			value |= closedd;
		}
		if (firstSes != -1)
		{
			mask |= SESSION_X_FIRSET;
			value |= firstSes;
		}
		if (sesAppeal != -1)
		{
			mask |= SESSION_X_APPEAL;
			value |= sesAppeal;
		}
		m.AddFlags("S.F_FLAGS_X", mask, value, SESSION_X_ALL);

		if (sesPublTreg != -1)
			m.AddFlags("S.F_FLAGS_Y", SESSION_Y_PUBLTREG, sesPublTreg, SESSION_Y_ALL);

		m.AddILike1("S.F_TEXT", sesText, true);

		if (extended != -1)
		{
			m.Add("(S.F_JUDGE_X IS ");
			m.cat(extended ? "NOT NULL OR S.F_REJUDGE_X IS NOT NULL)" : "NULL AND S.F_REJUDGE_X IS NULL)");
		}
	}

	if (tabDecision)
	{
		if (surrouendocs)
		{
			if (!any(decKinds))
				strcpy(decKinds, KIND_SURROUENDOC_ACTS);
			// Note: decision kind must not be NULL, so apply always
			m.AddChars("D.F_DECISION_KIND", decKinds, KIND_ENDOCS);
			m.AddRange("D.F_DECISION_DATE", minFromDate, maxFromDate);
			m.AddILike1("D.F_TEXT", decText, true);

			if (decPublTreg != -1)
				m.AddFlags("D.F_FLAGS_X", DECISION_X_PUBLTREG, decPublTreg, DECISION_X_ALL);
		}
		else
		{
			m.AddRange("D.F_RETURNED", minReturned, maxReturned);
			m.AddChars("D.F_KIND", adjust_xxx_kinds(decKinds, KIND_VISIBLE_DECISIONS, KIND_DECISIONS), KIND_ENDOCS);
			m.AddRange("D.F_FROM_DATE", minFromDate, maxFromDate);
			m.AddRange("D.F_FORCE_DATE", decMinForceDate, decMaxForceDate);
			m.AddRange("D.F_PARTITION", minPartition, maxPartition);
			m.AddRange("D.F_VOLUME", minVolume, maxVolume);
			m.AddChars("D.F_RESULT", decResults, RESULT_ENDOCS);
			m.AddChars("D.F_DEBTOR_STATUS", debtorStatus, DEBTOR_STATUS_ALLS);

			// 2010:270 TRQ/IRQ: need_crit() -> any()
			//if (need_crit("F_ACCOMPLYS", accomplys, ACCOMPLY_INCONSISTENCES))
			if (any(accomplys))
			{
				if (strlen(accomplys) == 1)
				{
					m.Add("D.F_ACCOMPLYS");
					m.printf("LIKE '%%%c%%'", *accomplys);
				}
				else
				{
					for (const char *s = accomplys; *s; s++)
					{
						if (s > accomplys)
							m.NextOr();
						m.Add("(D.F_ACCOMPLYS" + (s > accomplys));
						m.printf("LIKE '%%%c%%'", *s);
					}
					m.cach(')');
				}
			}

			m.AddArray("D.F_SLC_REASON", decSLCReasons);

			long mask = 0, value = 0;

			if (confess != -1)
			{
				mask |= DECISION_CONFESS;
				value |= confess;
			}
			if (absence != -1)
			{
				mask |= DECISION_ABSENCE;
				value |= absence;
			}
			if (motives != -1)
			{
				mask |= DECISION_MOTIVES;
				value |= motives;
			}
			m.AddFlags("D.F_FLAGS", mask, value, DECISION_ALL);

			mask = value = 0;
			if (decAppeal != -1)
			{
				mask |= DECISION_X_APPEAL;
				value |= decAppeal;
			}
			if (decPublTreg != -1)
			{
				mask |= DECISION_X_PUBLTREG;
				value |= decPublTreg;
			}
			if (decContract != -1)
			{
				mask |= DECISION_X_CONTRACT;
				value |= decContract;
			}
			m.AddFlags("D.F_FLAGS_X", mask, value, DECISION_X_ALL);

			if (any(decText))
			{
				m.AddLike("(D.F_TEXT", decText, true);
				m.NextOr();
				m.AddLike("D.F_MOTIVES", decText, true);
				m.cach(')');
			}

			if (opinion != -1)
				m.AddIsNull("D.F_OPINION", !opinion);
			// 2017:082
			m.AddWords("F_KEYWORDS", keyWords);
		}
	}

	if (tabJudgement)
	{
		m.AddChars("J.F_JUDGED", judJudgeds, JUDGED_ALLS);
		m.AddArray("J.F_ARTICLE", judArticles);
		m.AddChars("J.F_PUNISHMENT", judPunishments, PUNISHMENT_BASICS);
		m.AddChars("J.F_INTERVAL", judIntervals, INTERVAL_DOCS);
		m.AddRange("J.F_SEND_DATE", judMinSend, judMaxSend);
		m.AddRange("J.F_FORCE_DATE", judMinForceDate, judMaxForceDate);
		m.AddChars("J.F_UCN_TYPE", UCN_CITIZENS);

		long mask = 0, value = 0;

		if (appendix23 != -1) // mark
		{ // mark
			mask |= JUDGEMENT_Y_APPENDIX_23;
			value |= appendix23;
		}

		if (appendix24 != -1)
		{
			mask |= JUDGEMENT_Y_APPENDIX_24;
			value |= appendix24;
		}

		if (appendix25 != -1)
		{
			mask |= JUDGEMENT_Y_APPENDIX_25;
			value |= appendix25;
		}

		if (appendix26 != -1)
		{
			mask |= JUDGEMENT_Y_APPENDIX_26;
			value |= appendix26;
		}

		m.AddFlags("J.F_FLAGS_Y", mask, value, JUDGEMENT_Y_ALL);

		if (judYoung != -1)
			m.AddFlags("J.F_FLAGS", SIDE_WIND_YOUNG, judYoung, SIDE_WIND_ALL);

		if (any(judPunishmentsX))
		{
			m.Add("");
			for (const char *s = judPunishmentsX; *s; s++)
				m.printf("%sJ.F_PUNISHMENTS_X LIKE '%%%c%%'", s == judPunishmentsX ? "(" : " OR ", *s);
			m.cach(')');
		}

		if (judReport != -1)
		{
		#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_REGIONAL
			m.AddLike(judReport ? "NOT U.F_ATTRIB_I" : "U.F_ATTRIB_I", C2S[ATTRIB_DO_NOT_COUNT_DEFENDANTS], true);
		#endif  // APPEAL || REGIONAL
		#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
			m.AddChars("((L.F_KIND", KIND_1ST_PUNISHMENT_LAWS);
			m.AddLike(judReport ? "NOT U.F_ATTRIB_I" : "U.F_ATTRIB_I", C2S[ATTRIB_DO_NOT_COUNT_DEFENDANTS], true);
			m.cat(")");
			m.NextOr();
			m.AddChars("(NOT L.F_KIND", KIND_1ST_PUNISHMENT_LAWS);
			m.AddLike(judReport ? "NOT U.F_ATTRIB_II" : "U.F_ATTRIB_II", C2S[ATTRIB_DO_NOT_COUNT_DEFENDANTS], true);
			m.cat("))");
		#endif  // ADMIN || AREAL || MILITARY
		}
	}

	const char *reorder;

	switch (tabSelect)
	{
		case TAB_DECISION :
		{
			reorder = variantReg ? "D.F_VOLUME, D.F_PAGE, D.F_PARTITION" : "S.F_DATE, L.F_TYPE, L.F_YEAR, L.F_NO";
			break;
		}
		case TAB_SESSION :
		{
			reorder = "L.F_TYPE, L.F_YEAR, L.F_NO, S.F_DATE";
			break;
		}
		case TAB_JUDGEMENT :
		{
			switch (judOrder)
			{
				case ORDER_BY_TYN   : reorder = "L.F_TYPE, L.F_YEAR, L.F_NO"; break;
				case ORDER_BY_SEND  : reorder = "J.F_SEND_DATE"; break;
				case ORDER_BY_ART   : reorder = "J.F_ARTICLE"; break;
				case ORDER_BY_FORCE : reorder = "J.F_FORCE_DATE"; break;
				default : reorder = NULL;
			}

			break;
		}
		default :
		{
			switch (lawOrder)
			{
				case ORDER_BY_TYN  : reorder = "L.F_TYPE, L.F_YEAR, L.F_NO"; break;
				case ORDER_BY_TSYN : reorder = "L.F_TYPE, L.F_SUBJECT, L.F_YEAR, L.F_NO"; break;
				case ORDER_BY_TKYN : reorder = "L.F_TYPE, L.F_KIND, L.F_YEAR, L.F_NO"; break;
				case ORDER_BY_JTYN : reorder = "L.F_JUDGE, L.F_TYPE, L.F_YEAR, L.F_NO"; break;
				default : reorder = NULL;
			}
		}
	}

	if (reorder)
		m.AddOrder(reorder);
}

//---------------------------------------------------------------------------
class TComboGroup : public TRCDKeyGroup
{
public:
	TComboGroup();

	CDate //intDate,
		lawDate,
		lawFinished,
		sesDate,
		sesReturned,
		sesForceDate,
		decReturned,
		decFromDate,
		//intFinished,
		decDate,
		decForceDate,
		judForceDate;

	char lawKind,
		sesKind,
		decKind,
		sesResult,
		decResult,
		judUcnType,
		decSessionKind,
		sesJudge[SIZE_OF_UCN],
		lawJudge[SIZE_OF_UCN],
		lawRejudge[SIZE_OF_UCN],
		judUcn[SIZE_OF_UCN],
		judInvolvement,
		judJudged,
		eisppNmr[SIZE_OF_EISPP_NMR],
		subjectText[SIZE_OF_SUBJECT_NAME],
		lawsuitText[SIZE_OF_ORIGIN_TEXT];

	long eventNo,
		page,
		partition,
		volume,
		lawSubject,
		judArticle,
		decEventNo,
		flags,
		lawOldSubject;
};

TComboGroup::TComboGroup()
	: TRCDKeyGroup("T_COMBO", NULL)
{
	Add(
		new TChar("F_LAW_KIND", &lawKind),
		new TString("F_LAW_JUDGE", lawJudge, sizeof lawJudge),
		new TString("F_LAW_REJUDGE", lawRejudge, sizeof lawRejudge),
		new TLong("F_LAW_SUBJECT", &lawSubject),
		new TDate("F_LAW_FINISHED", &lawFinished, false),
		new TLong("F_LAW_OLD_SUBJECT", &lawOldSubject),

		//new TDate("F_INT_DATE", &intDate, false),
		new TDate("F_LAW_DATE", &lawDate, false),
		//new TDate("F_INT_FINISHED", &intFinished, false),
		new TLong("F_INT_FLAGS", &flags),

		new TString("F_SES_JUDGE", sesJudge, sizeof sesJudge),
		new TChar("F_SES_KIND", &sesKind),
		new TDate("F_SES_DATE", &sesDate, false),
		new TChar("F_SES_RESULT", &sesResult),
		new TDate("F_SES_RETURNED", &sesReturned, false),
		new TDate("F_SES_FORCE_DATE", &sesForceDate, false),

		new TChar("F_DEC_KIND", &decKind),
		new TDate("F_DEC_DATE", &decDate, false),
		new TChar("F_DEC_RESULT", &decResult),
		new TDate("F_DEC_FROM_DATE", &decFromDate, false),
		new TDate("F_DEC_RETURNED", &decReturned, false),
		new TDate("F_DEC_FORCE_DATE", &decForceDate, false),
		new TLong("F_DEC_EVENT_NO", &decEventNo),
		new TLong("F_EVENT_NO", &eventNo),
		new TLong("F_PAGE", &page),
		new TLong("F_PARTITION", &partition),
		new TLong("F_VOLUME", &volume),

		new TChar("F_JUD_UCN_TYPE", &judUcnType),
		new TString("F_JUD_UCN", judUcn, sizeof judUcn),
		new TChar("F_JUD_INVOLVEMENT", &judInvolvement),
		new TDate("F_JUD_FORCE_DATE", &judForceDate, false),
		new TLong("F_JUD_ARTICLE", &judArticle),
		new TCharAlias("F_JUD_JUDGED", &judJudged),
		new TString("F_EISPP_NMR", eisppNmr, sizeof eisppNmr),		
		new TLongString("F_SUBJECT_TEXT", subjectText, sizeof subjectText),
		new TString("F_LAWSUIT_TEXT", lawsuitText, sizeof lawsuitText),
		NULL
	);
}

// ----- TFloatComboQuery --------------------------------------------------
class TFloatComboQuery : public TFloatQuery
{
public:
	TFloatComboQuery(TWindow *parent, const char *criteria, const char *title, const char *name, long tPartof,
		int tTabSelect);

	long partof;
	int tabSelect;

protected:
	void Count();

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	TDecision decision;
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
};

TFloatComboQuery::TFloatComboQuery(TWindow *parent, const char *criteria, const char *title, const char *name, long tPartof,
	int tTabSelect)
		: TFloatQuery(parent, criteria, name, title), partof(tPartof), tabSelect(tTabSelect)
{
}

#define lawsuit OFFICE_GROUP_CAST(TComboGroup)

static void floatcombo_flags(mstr &m, TGroup *group) { m.cpy(lawsuit->flags ? "" : "*"); }

static void floatcombo_law_kind(mstr &m, TGroup *group)
{
	m.cpy(Kind->Seek(lawsuit->lawKind));
	m.sep_cat(", ", lawsuit->eisppNmr);
}

static void floatcombo_law_judge(mstr &m, TGroup *group) { print_user(m, lawsuit->lawJudge); }

static void floatcombo_law_subject(mstr &m, TGroup *group)
{
	if (lawsuit->lawSubject)	// not guaranteed before update
	{
		m.cat(TSubject::Code(lawsuit->lawSubject, lawsuit->lawKind));
		m.sep_cat(" ", lawsuit->subjectText);
		m.sep_cat(" ", lawsuit->lawsuitText);
	}

	print_old_subject(m, lawsuit->lawOldSubject);
}

//static void floatcombo_law_date(mstr &m, TGroup *group) { lawsuit->intDate.Print(m); }
static void floatcombo_law_date(mstr &m, TGroup *group) { lawsuit->lawDate.Print(m); }
//static void floatcombo_law_finished(mstr &m, TGroup *group) { lawsuit->intFinished.Print(m); }
static void floatcombo_law_finished(mstr &m, TGroup *group) { lawsuit->lawFinished.Print(m); }
static void floatcombo_law_create(mstr &m, TGroup *group) { lawsuit->lawDate.Print(m); }

static void floatcombo_ses_judge(mstr &m, TGroup *group) { print_user(m, lawsuit->sesJudge); }
static void floatcombo_ses_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(lawsuit->sesKind)); }
static void floatcombo_ses_date(mstr &m, TGroup *group) { lawsuit->sesDate.Print(m); }
static void floatcombo_ses_result(mstr &m, TGroup *group) { m.cpy(Result->Seek(lawsuit->sesResult)); }
static void floatcombo_ses_returned(mstr &m, TGroup *group) { lawsuit->sesReturned.Print(m); }
static void floatcombo_ses_force_date(mstr &m, TGroup *group) { lawsuit->sesForceDate.Print(m); }

static void floatcombo_dec_kind(mstr &m, TGroup *group) { m.cpy(Kind->Seek(lawsuit->decKind)); }
static void floatcombo_dec_result(mstr &m, TGroup *group) { m.cpy(Result->Seek(lawsuit->decResult)); }
static void floatcombo_dec_from_date(mstr &m, TGroup *group) { lawsuit->decFromDate.Print(m); }
static void floatcombo_dec_returned(mstr &m, TGroup *group) { lawsuit->decReturned.Print(m); }
static void floatcombo_dec_force_date(mstr &m, TGroup *group) { lawsuit->decForceDate.Print(m); }
static void floatcombo_dec_volume(mstr &m, TGroup *group) { itom(lawsuit->volume, m); }
static void floatcombo_dec_page(mstr &m, TGroup *group) { itom(lawsuit->page, m); }
static void floatcombo_dec_partition(mstr &m, TGroup *group) { itom(lawsuit->partition, m); }
static void floatcombo_dec_event_no(mstr &m, TGroup *group) { itom(lawsuit->decEventNo, m); }
static void floatcombo_dec_from_year(mstr &m, TGroup *group) { itom(lawsuit->decFromDate.year, m); }

static void floatcombo_jud_force_date(mstr &m, TGroup *group) { lawsuit->judForceDate.Print(m); }
static void floatcombo_jud_judgement(mstr &m, TGroup *group) { print_user(m, lawsuit->judUcn, lawsuit->judUcnType); }

static void floatcombo_jud_reason(mstr &m, TGroup *group)
{
	if (lawsuit->judArticle)
		m.cpy(Article->Seek(lawsuit->judArticle));
}

static void floatcombo_jud_result(mstr &m, TGroup *group) { m.cpy(Judged->Seek(lawsuit->judJudged)); }

#undef lawsuit

static const offield combo_fields[]=
{
	{ "F_NO", NULL },
	{ "F_YEAR", NULL },
	{ "F_FLAGS", floatcombo_flags },
//lawsuit-----------------------------------------------------------------------
	{ "F_LAW_KIND", floatcombo_law_kind },
	{ "F_LAW_JUDGE", floatcombo_law_judge },
	{ "F_LAW_SUBJECT", floatcombo_law_subject },
	{ "F_LAW_DATE", floatcombo_law_date },
	{ "F_LAW_FINISHED", floatcombo_law_finished },
	{ "F_LAW_CREATE", floatcombo_law_create },
//session-----------------------------------------------------------------------
	{ "F_SES_JUDGE", floatcombo_ses_judge },
	{ "F_SES_KIND", floatcombo_ses_kind },
	{ "F_SES_DATE", floatcombo_ses_date },
	{ "F_SES_RESULT", floatcombo_ses_result },
	{ "F_SES_RETURNED", floatcombo_ses_returned },
	{ "F_SES_FORCE_DATE", floatcombo_ses_force_date },
//decision----------------------------------------------------------------------
	{ "F_DEC_KIND", floatcombo_dec_kind },
	{ "F_DEC_RESULT", floatcombo_dec_result },
	{ "F_DEC_FROM_DATE", floatcombo_dec_from_date },
	{ "F_DEC_RETURNED", floatcombo_dec_returned },
	{ "F_DEC_FORCE_DATE", floatcombo_dec_force_date },

	{ "F_DEC_VOLUME", floatcombo_dec_volume },
	{ "F_DEC_PAGE", floatcombo_dec_page },
	{ "F_DEC_PARTITION", floatcombo_dec_partition },
	{ "F_DEC_EVENT_NO", floatcombo_dec_event_no },
	{ "F_DEC_FROM_YEAR", floatcombo_dec_from_year },

	{ "F_JUD_REASON", floatcombo_jud_reason },
	{ "F_JUD_FORCE_DATE", floatcombo_jud_force_date },
	{ "F_JUD_JUDGEMENT", floatcombo_jud_judgement },
	{ "F_JUD_RESULT", floatcombo_jud_result },
	{ NULL, NULL }
};

void TFloatComboQuery::Count()
{
	TQuery q(criteria);

	while (q.Read() && Increment())
	{
		TComboGroup lawsuit;
		lawsuit << q;

		if (partof)
			strcpy(lawsuit.lawJudge, lawsuit.lawRejudge);

	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		if (tabSelect == TAB_JUDGEMENT && lawsuit.judJudged == JUDGED_NOT_KNOWN)
		{
			decision.key = lawsuit.key;
			decision.kind = lawsuit.decKind;
			decision.date = lawsuit.decDate;

			if (!mustbe_personal_data(&decision, lawsuit.judInvolvement))
			{
				count--;
				continue;
			}
		}
	#endif  // APPEAL || AREAL || MILITARY || REGIONAL

		office.Emit(tr1start, tr1final, combo_fields, &lawsuit);
	}
}

struct TFloatPage
{
	const char *tabTitle;
	const char *fileName;
	const char *queryTitle;
};

static const TFloatPage FloatPages[PAGE_COUNT] =
{
	{ " Дела",			"FloatLaw.htm",	"Нерегламентирана справка за дела" },
	{ "Заседания",		"FloatSes.htm",	"Нерегламентирана справка за заседания" },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
	{ "Съдебни актове",	"FloatDec.htm",	"Нерегламентирана справка за съдебни актове",  }
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{ "Съдебни актове",	"FloatDec.htm",	"Нерегламентирана справка за съдебни актове" },
	{ "Индивидуални данни",	"FloatJud.htm",	"Нерегламентирана справка за изпълнение на присъди" }
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
};

static void query_tab(TWindow *parent, const char *criteria, const char *tTitle, bool variantReg, int tabSelect, long partof)
{
	const char *title = tTitle ? tTitle : FloatPages[tabSelect].queryTitle;
	const char *name = (tabSelect == TAB_DECISION && variantReg) ? "FloatReg.htm" : FloatPages[tabSelect].fileName;

	TFloatComboQuery(parent, criteria, title, name, partof, tabSelect).Execute();
}

void query_session_tab(TWindow *parent, const char *criteria, const char *title, bool variant, long partof)
{
	query_tab(parent, criteria, title, variant, TAB_SESSION, partof);
}

void query_lawsuit_tab(TWindow *parent, const char *criteria, const char *title, bool variant, long partof)
{
	query_tab(parent, criteria, title, variant, TAB_LAWSUIT, partof);
}

// used by float combo only
#include "DangerListBox.h"
#include "DangerListBox.cpp"

struct TViewControl
{
	TControl *control;
	const char *courts;
};

static TViewControl *copy_controls(const TViewControl *vcs)
{
	int count = 0;
	while (vcs[count++].control);

	TViewControl *copy = new TViewControl[count];
	memcpy(copy, vcs, count * sizeof(TViewControl));
	return copy;
}

#include "FloatComboDialog.h"
#include "FloatComboDialog.cpp"

void float_combo(TWindow *parent)
{
	TFloatComboGroup group;
	TFloatComboDialog(parent, &group).Execute();
}
