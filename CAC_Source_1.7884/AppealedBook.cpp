#include "cac.h"

enum
{				// I - инстанция
	I1		= 0,	// Брой обж. актове с жалби и протести
	I1PRIVATE	= 1,	// Брой обж. актове с частни жалби и частни протести
	I1REJECT	= 2,	// Брой обж. актове с молби за отмяна и преглед
	I1RESULT	= 3,	// Брой обж. актове с резултат
	I2		= 4,
	I2PRIVATE	= 5,
	I2REJECT	= 6,
	I2RESULT	= 7,
	ALLSEND	= 8,	// Общо изпратени актове за обжалване
	ALLRECEIVE	= 9,	// Общо върнати изпратени обжалвани актове
	ARRAY_SIZE	= 10
};

// ----- TAppealedBookGroup --------------------------------------------------
class TAppealedBookGroup : public TBookGroup
{
public:
	TAppealedBookGroup();

	virtual void FormCriteria(msql &m);

	long columns;
	long recapitulation;
	long all[ARRAY_SIZE];
};

TAppealedBookGroup::TAppealedBookGroup()
	: TBookGroup("T_INREG I JOIN T_INREG_EVENT IE ON IE.F_NO = I.F_NO AND IE.F_YEAR = I.F_YEAR AND IE.F_TYPE = I.F_TYPE",
				" F_DATE", "F_DATE", false)
{
	Add(
		new TCheck("F_COLUMNS", &columns, TRUE),
		new TCheck("F_RECAPITULATION", &recapitulation, TRUE),
		NULL
	);

	memset(all, '\0', sizeof all);
	columns = TRUE;
	recapitulation = TRUE;
}

void TAppealedBookGroup::FormCriteria(msql &m)
{
	m.Begin(Name, "I.F_NO, I.F_YEAR, I.F_DATE, I.F_LAWSUIT_NO, I.F_LAWSUIT_YEAR, I.F_LAWSUIT_TYPE, "
		"I.F_SESSION_KIND AS F_SES_KIND, I.F_DECISION_DATE AS F_DEC_DATE, I.F_KIND AS F_INREG_KIND, I.F_COLLEGE, "
		"IE.F_OUT_NO AS F_OUT_NO, IE.F_OUT_YEAR AS F_OUT_YEAR");

	m.AddChars("I.F_COLLEGE", types, COLLEGE_ALLS);
	m.AddRange("IE.F_DATE", minDate, maxDate);
	// 2011:020 LPR: IN('%s') -> = '%c', single result
	m.AddChar("IE.F_RESULT", RESULT_IOREG_SEND);

	m.AddOrder("IE.F_DATE, I.F_YEAR, I.F_NO");	// == EndCriteria()
}

class TAppealedOutReg : public TOutReg
{
public:
	TAppealedOutReg();

	long ieNo;
	long outNo;
	long ieYear;
	long outYear;
	char sesKind;
	char lawKind;
	char inRegKind;
	CDate ieDate;
	long lawSubject;
	CDate decDate;

	TGArray<TAppealedOutReg> array;
};

TAppealedOutReg::TAppealedOutReg()
	:lawKind('\0')
{
	Add(
		new TDate("F_IE_DATE", &ieDate, false),
		new TChar("F_SES_KIND", &sesKind),
		new TDate("F_DEC_DATE", &decDate, false),
		new TChar("F_INREG_KIND", &inRegKind),
		new TLong("F_IE_NO", &ieNo),
		new TLong("F_IE_YEAR", &ieYear),
		new TLong("F_OUT_NO", &outNo),
		new TLong("F_OUT_YEAR", &outYear),
		NULL
	);
}

static void print_result(mstr &m, char resolution)
{
	if (resolution != RESOLUTION_FAKE)
	{
		m.sep("\v\v");
		m.cat(Resolution->Seek(resolution));
	}
}

static void print_judge_or_date(TAppealedOutReg *outReg, mstr &m, bool judge)
{
	if (outReg->lawsuit.type)
	{
		if (outReg->sesKind && !outReg->decDate.Empty())
		{
			TSession session;

			session.key = outReg->lawsuit;
			session.date = outReg->decDate;
			session.kind = outReg->sesKind;

			if (session.Try("F_DATE, F_JUDGE"))
			{
				if (judge)
				{
					TCitizen citizen;
					strcpy(citizen.ucn, session.judge);
					citizen.ucnType = UCN_CITIZEN_UCN;

					if(citizen.Try())
					{
						if (strlen(citizen.reName) > 2)
							strcpy(citizen.reName + 1, ".");

						m.printf("%s %s %s ", citizen.name, *citizen.reName ? citizen.reName : "", citizen.family);
					}
				}
				else
				{
					m.cat(" заседание ");
					session.date.Print(m);
				}
			}
		}
	}
}

#define outReg OFFICE_GROUP_CAST(TAppealedOutReg)

static void appealed_lawsuit(mstr &m, TGroup *group)
{
	if (outReg->lawsuit.type)
	{
		m.printf("%s %d/%d ",Kind->Seek(outReg->lawKind), flag_key_no(&outReg->lawsuit), outReg->lawsuit.year);
		print_judge_or_date(outReg, m, false);
	}
}

static void appealed_subject(mstr &m, TGroup *group)
{
	if (outReg->lawsuit.no && outReg->lawsuit.no)
	{
		if (outReg->lawSubject && strchr(KIND_LAWSUITS, outReg->lawKind))
		{
			m.printf("%s\v%s", TSubject::Code(outReg->lawSubject, outReg->lawKind),
				Subject->Seek(outReg->lawsuit.type, outReg->lawSubject));
		}
	}
}

static void appealed_date(mstr &m, TGroup *group)
{
	for (int i = 0; i < outReg->array.Count(); i++)
		outReg->array[i]->ieDate.Print(m, "%D\v\v");
}

static void appealed_returned(mstr &m, TGroup *group)
{
	for (int i = 0; i < outReg->array.Count(); i++)
	{
		TSender sender;
		strcpy(sender.ucn, outReg->array[i]->receiver);
		sender.ucnType = UCN_INSTANCE_HIGHER;

		if (any(outReg->array[i]->receiver))
			if (sender.Try())
				m.printf("%s\v\v", sender.name);
	}		
}

static void appealed_receiver(mstr &m, TGroup *group)
{
	for (int i = 0; i < outReg->array.Count(); i++)
		if (strchr (RESULT_IOREG_RETURNEDS, outReg->array[i]->result))
			outReg->array[i]->returned.Print(m, "%D\v\v");
}

static void appealed_result_I(mstr &m, TGroup *group)
{
	for (int i = 0; i < outReg->array.Count(); i++)
	{
		print_result(m, outReg->array[i]->resultI);

		if (any(outReg->array[i]->remarkVZ))
			m.printf("\v%s", str(outReg->array[i]->remarkVZ));
	}
}

static void appealed_result_II(mstr &m, TGroup *group)
{
	for (int i = 0; i < outReg->array.Count(); i++)
		print_result(m, outReg->array[i]->resultII);
}

static void appealed_result_KS(mstr &m, TGroup *group)
{
	for (int i = 0; i < outReg->array.Count(); i++)
	{
		print_result(m, outReg->array[i]->resultKS);

		if (any(outReg->array[i]->remarkKS))
			m.printf("\v%s", str(outReg->array[i]->remarkKS));
	}
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
static void appealed_complect(mstr &m, TGroup *group)
{
	for (int i = 0; i < outReg->array.Count(); i++)
	{
		char s[5] = { outReg->array[i]->resultI, outReg->array[i]->resultII, outReg->array[i]->resultKS,
			outReg->array[i]->resultMJ, '\0' };

		if (strchr(s, RESOLUTION_RET_FOR_COMPLETE))
			m.cat("*\v\v");
	}
}
#endif  // APPEAL || ADMIN || AREAL || MILITARY

static void appealed_remark(mstr &m, TGroup *group)
{
	for (int i = 0; i < outReg->array.Count(); i++)
	{
		if (any(outReg->array[i]->vssIndex))
		{
			m.sep("\v\v");
			m.printf("Индекс %s", outReg->array[i]->vssIndex);
		}
	}
}

static void appealed_judge(mstr &m, TGroup *group) { print_judge_or_date(outReg, m, true);  }

#undef outReg


static const offield appealed_fields[]=
{
	{ "F_LAWSUIT", appealed_lawsuit },
	{ "F_SUBJECT", appealed_subject },
	{ "F_DATE", appealed_date },
	{ "F_RECEIVER", appealed_receiver },
	{ "F_RETURNED", appealed_returned },
	{ "F_RESULT_I", appealed_result_I },
	{ "F_RESULT_II", appealed_result_II },
	{ "F_RESULT_KS", appealed_result_KS },
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN|| COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	{ "F_COMPLECT", appealed_complect },
#endif  // APPEAL || ADMIN || AREAL || MILITARY
	{ "F_REMARK", appealed_remark },
	{ "F_JUDGE", appealed_judge },
	{ NULL, NULL }
};

#define bookGroup OFFICE_GROUP_CAST(TAppealedBookGroup)

static void aplBook_title(mstr &m, TGroup *group)
{
	book_types(m, group);
	m.printf("\v от %D до %D", &bookGroup->minDate, &bookGroup->maxDate);
}

static void book_court(mstr &m, TGroup *)
{
	m.cpy("Окръжен съд");
#if COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_AREAL
	m.cpy("Апелативен съд");
#endif
#if (COURT_TYPE == COURT_APPEAL)
	m.cpy("ВКС");
#endif
#if (COURT_TYPE == COURT_ADMIN)
	m.cpy("ВАС");
#endif
}

static void appeal_protest_1(mstr &m, TGroup *group) { itom(bookGroup->all[I1], m); }
static void private_appeal_protest_1(mstr &m, TGroup *group) { itom(bookGroup->all[I1PRIVATE], m); }
static void rescind_review_1(mstr &m, TGroup *group) { itom(bookGroup->all[I1REJECT], m); }
static void result_1(mstr &m, TGroup *group) { itom(bookGroup->all[I1RESULT], m); }

static void appeal_protest_2(mstr &m, TGroup *group) { itom(bookGroup->all[I2], m); }
static void private_appeal_protest_2(mstr &m, TGroup *group) { itom(bookGroup->all[I2PRIVATE], m); }
static void rescind_review_2(mstr &m, TGroup *group) { itom(bookGroup->all[I2REJECT], m); }
static void result_2(mstr &m, TGroup *group) { itom(bookGroup->all[I2RESULT], m); }

static void count_all(mstr &m, TGroup *group) { itom(bookGroup->all[ALLSEND], m); }
static void returned(mstr &m, TGroup *group) { itom(bookGroup->all[ALLRECEIVE], m); }

#undef bookGroup

static const offield all_fields[] =
{
	{ "F_TITLE", aplBook_title },
	{ "F_COURT_NAME", book_court },
	{ "F_1ST_APPEAL_PROTEST", appeal_protest_1 },
	{ "F_1ST_PRIVATE_APPEAL_PROTEST", private_appeal_protest_1 },
	{ "F_1ST_RESCIND_REVIEW", rescind_review_1 },
	{ "F_1ST_RESULT", result_1 },
	{ "F_2ND_APPEAL_PROTEST", appeal_protest_2 },
	{ "F_2ND_PRIVATE_APPEAL_PROTEST", private_appeal_protest_2 },
	{ "F_2ND_RESCIND_REVIEW", rescind_review_2 },
	{ "F_2ND_RESULT", result_2 },
	{ "F_COUNT", count_all },
	{ "F_RETURNED", returned },
	{ NULL, NULL }
};

#include "AppealedBookDialog.h"
#include "AppealedBookDialog.cpp"

// 2011:269 LPR: *_appeal: &count -> return
static int kind_appeal(TAppealedOutReg *outReg, const bool prv)
{
	if (outReg->inRegKind)
	{
		if (prv)
		{
			if (strchr(KIND_INREG_PRIVATE_COMPLAINTS, outReg->inRegKind) || strchr (KIND_INREG_PRIVATE_OBJECTIONS, outReg->inRegKind) || strchr(KIND_PRIVATE_CONNECT_APPROTS, outReg->inRegKind) || strchr(KIND_PRIVATE_CALL_APPROTS, outReg->inRegKind))
				return 1;
		}
		else
		{
			if (strchr(KIND_INREG_EXACT_COMPLAINTS, outReg->inRegKind) || strchr (KIND_INREG_EXACT_OBJECTIONS, outReg->inRegKind) || strchr(KIND_CONNECT_APPROTS, outReg->inRegKind) || strchr(KIND_CALL_APPROTS, outReg->inRegKind))
				return 1;
		}
	}

	return 0;
}

static int kind_other_appeal(TAppealedOutReg *outReg, const char * const kind)
{
	return kind && outReg->inRegKind && strchr(kind, outReg->inRegKind);
}

static int all_appeal(TAppealedOutReg *outReg)
{
	int count = 0;

	for (int i = 0; i < outReg->array.Count(); i++)
	{
		TAppealedOutReg *tOutReg = outReg->array[i];

		// 2012:109 LRQ/LPR: count by outReg, not inReg
		//if (tOutReg->ieNo == outReg->key.no && tOutReg->ieYear == outReg->key.year &&
		if (tOutReg->ieNo == outReg->outNo && tOutReg->ieYear == outReg->outYear &&
			strchr(RESULT_IOREG_RETURNEDS, tOutReg->result))
		{
			if (tOutReg->resultI != RESOLUTION_FAKE || tOutReg->resultII != RESOLUTION_FAKE ||
				tOutReg->resultKS != RESOLUTION_FAKE || tOutReg->resultMJ != RESOLUTION_FAKE)
			{
				count++;
			}
		}
	}

	return count;
}

static void rec_appeal(TAppealedOutReg *outReg, long arr[])
{
#if COURT_TYPE == COURT_APPEAL
	arr[I2] += kind_appeal(outReg, false);
	arr[I2PRIVATE] += kind_appeal(outReg, true);
	arr[I2REJECT] += kind_other_appeal(outReg, KIND_INREG_REJ_RETR_COMPLAINTS);

	arr[ALLRECEIVE] += all_appeal(outReg);
	arr[I2RESULT] += all_appeal(outReg);
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (strchr (KIND_1ST_LAWSUITS, outReg->lawKind))
	{
		arr[I1] += kind_appeal(outReg, false);
		arr[I1PRIVATE] += kind_appeal(outReg, true);
		arr[I1REJECT] += kind_other_appeal(outReg, KIND_INREG_REJ_RETR_COMPLAINTS);
	}
	else
	{
		arr[I2] += kind_appeal(outReg, false);
		arr[I2PRIVATE] += kind_appeal(outReg, true);
		arr[I2REJECT] += kind_other_appeal(outReg, KIND_INREG_REJ_RETR_COMPLAINTS);
	}

	arr[strchr(KIND_1ST_LAWSUITS, outReg->lawKind) ? I1RESULT : arr[I2RESULT]] += all_appeal(outReg);
	arr[ALLRECEIVE] += all_appeal(outReg);
#endif  // ADMIN || AREAL || MILITARY

#if COURT_TYPE == COURT_REGIONAL
	arr[I1] += kind_appeal(outReg, false);
	arr[I1PRIVATE] += kind_appeal(outReg, true);
	arr[I1REJECT] += kind_other_appeal(outReg, KIND_INREG_REJ_RETR_COMPLAINTS);

	arr[ALLRECEIVE] += all_appeal(outReg);
	arr[I1RESULT] += all_appeal(outReg);
#endif  // REGIONAL

}

class TAppealedBook : public TBook
{
public:
	TAppealedBook(TWindow *parent, TBookGroup *tGroup, const char *fileName);

protected:
	virtual TGroup *Process(TQuery &q);

	TLawsuit lawsuit;
	TIntegerArray array;
	const char *tr1break;
	virtual void Markers();
	virtual void EmitRow(TGroup *group);
};

TAppealedBook::TAppealedBook(TWindow *parent, TBookGroup *tGroup, const char *fileName)
	: TBook(parent, tGroup, fileName, appealed_fields, all_fields, BOOK_FIXED_DATES)
{
}

void TAppealedBook::Markers()
{
	tr1start = office.Find("tr1 start");
	tr1final = office.Find("tr1 final");
	tr1break = office.Find("tr1 break");
}

void TAppealedBook::EmitRow(TGroup *element)
{
	office.Emit(tr1start, tr1final, body_fields, element);


	if (element == (*TBook::array)[TBook::array->Count() - 1])
	{
		if ( ((TAppealedBookGroup *) group)->recapitulation )
			office.Emit(tr1final, tr1break, all_fields, group);
		tr1final = tr1break;
	}
}

TGroup *TAppealedBook::Process(TQuery &q)
{
	TAppealedOutReg *aplOutReg = new TAppealedOutReg;
	TAppealedBookGroup *aplGroup = (TAppealedBookGroup *) group;

	*aplOutReg << q;

	if (strchr(COLLEGE_ALLS, aplOutReg->college) && !strchr(KIND_TEXTUAL_IN_DOCS, aplOutReg->inRegKind) && strchr(TYPE_LAWSUITS, aplOutReg->lawsuit.type))
	{
		if (array.Find((aplOutReg->outYear - YEAR_MIN) * (XFER_NO_MAX + 1) + aplOutReg->outNo) != INT_MAX)
		{
			count--;
			delete aplOutReg;
			return NULL;
		}
		else
		{
			if (aplOutReg->lawsuit.no && aplOutReg->lawsuit.year)
			{
				lawsuit.key = aplOutReg->lawsuit;

				if (lawsuit.Try("F_KIND, F_SUBJECT"))
				{
					aplOutReg->lawKind = lawsuit.kind;
					aplOutReg->lawSubject = lawsuit.subject;
				}
			}

			// 2011:020 IN ('%s') -> = '%c'; 2011:343 msql -> msql, order
			msql m("T_INREG_EVENT IE LEFT JOIN T_OUTREG O ON O.F_NO = IE.F_OUT_NO AND O.F_YEAR = IE.F_OUT_YEAR",
				"IE.F_DATE AS F_IE_DATE, O.F_RESULT, O.F_RETURNED, O.F_RESULT_I, O.F_RESULT_KS, O.F_RESULT_II, "
				// 2012:109 LRQ/LPR: count inregs for outreg only, not for inreg; see all_appeal()
				//"F_RECEIVER, F_REMARK, IE.F_NO AS F_IE_NO, IE.F_YEAR AS F_IE_YEAR, F_VSS_INDEX");
				"O.F_RECEIVER, O.F_REMARK, O.F_NO AS F_IE_NO, O.F_YEAR AS F_IE_YEAR, O.F_VSS_INDEX");

			m.AddLong("IE.F_NO", aplOutReg->key.no);
			m.AddLong("IE.F_YEAR", aplOutReg->key.year);
			m.AddChar("IE.F_RESULT", RESULT_IOREG_SEND);
			m.AddOrder("IE.F_DATE");

			TQuery p(str(m));
			while (p.Read())
			{
				TAppealedOutReg *outReg = new TAppealedOutReg;
				*outReg << p;
				aplOutReg->array.Add(outReg);
			}

			if (aplGroup->recapitulation)
			{
				rec_appeal(aplOutReg, aplGroup->all);
				aplGroup->all[ALLSEND] = count;
			}

			array.Add((aplOutReg->outYear - YEAR_MIN) * (XFER_NO_MAX + 1) + aplOutReg->outNo);
			return aplOutReg;
		}
	}
	else
	{
		count--;
		delete aplOutReg;
		return NULL;
	}
}

void appealed_book(TWindow *parent)
{
	TAppealedBookGroup group;
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
	constant longName = "AplBookA.htm";
	constant shortName = "AplBkA.htm";
#endif  // APPEAL || ADMIN
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	constant longName = "AplBookO.htm";
	constant shortName = "AplBkO.htm";
#endif  // AREAL || MILITARY || REGIONAL

	if (TAppealedBookDialog(parent, &group).Execute() == IDOK)
		TAppealedBook(parent, &group, group.columns ? longName : shortName).Execute(BOOK_EXEC_HEADER);
}
