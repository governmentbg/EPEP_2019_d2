#include "cac.h"

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
// ----- TProveBookGroup -----------------------------------------------------
class TProveBookGroup : public TBookGroup
{
public:
	TProveBookGroup();

	virtual void FormCriteria(msql &m);
	virtual void Title(mstr &m);

	long flags;
};

TProveBookGroup::TProveBookGroup()
	: TBookGroup("T_PROVE", "F_LAWSUIT_TYPE", "F_DATE", false)
{
	Add(new TCheck("F_NEW_FORMAT", &flags, BOOK_NEW_FORMAT));
	strcpy(types, C2S[TYPE_PUNISHMENT_LAW]);
	strcat(types, TYPE_REQUESTS);
	flags = BOOK_NEW_FORMAT;
}

void TProveBookGroup::FormCriteria(msql &m)
{
	TBookGroup::FormCriteria(m);
	EndCriteria(m);
}

void TProveBookGroup::Title(mstr &m)
{
	book_range(m, this);
}

// ----- TBookProve ----------------------------------------------------------
class TBookProve : public TProve
{
public:

	TBookProve();
	void LoadProveAction(TBookProve *prove);

	char lawKind;
	mstr send, receiveAndReturn, destroy;
	bool newFormat;
};

TBookProve::TBookProve()
	: lawKind('\0')
{
}

#define bookprove OFFICE_GROUP_CAST(TBookProve)

static void bookprove_no(mstr &m, TGroup *group) { itom(bookprove->key.no, m);}

static void bookprove_connect_no(mstr &m, TGroup *group)
{
	TDummyContainer container;
	TGArray<TConnect> connects;
	container.key = bookprove->lawsuit;

	if (load_connects(&container, &connects))
	{
		for (int i = 0; i < connects.Count(); i++)
		{
			if (strchr(CONNECT_TYPE_PRELIMITARY_ACTS, connects[i]->connectType))
			{
				connects[i]->composition = 0;
				connects[i]->Print(m);
				m.cat("\v");
			}
		}
	}

	if (!bookprove->newFormat && bookprove->lawKind)
		m.printf("\v%s %d/%dг.", Kind->Seek(bookprove->lawKind), bookprove->lawsuit.no, bookprove->lawsuit.year);
}

static void bookprove_lawsuit_no(mstr &m, TGroup *group)
{
	if (bookprove->lawKind)
		m.printf("%s %d/%dг.", Kind->Seek(bookprove->lawKind), flag_key_no(&bookprove->lawsuit), bookprove->lawsuit.year);
}

static void bookprove_name(mstr &m, TGroup *group)
{
	TGArray<TSideWind> *sideWinds = load_sidewinds(bookprove);

	for (int i = 0; i < sideWinds->Count(); i++)
	{
		if (strchr(UCN_CITIZENS, (*sideWinds)[i]->ucnType))
		{
			TCitizen citizen;
			citizen.ucnType = (*sideWinds)[i]->ucnType;
			strcpy(citizen.ucn, (*sideWinds)[i]->ucn);

			if (citizen.Try())
				m.printf("%s %s %s\v", citizen.name, citizen.reName, citizen.family);
		}		
		else if (strchr(UCN_FIRMS, (*sideWinds)[i]->ucnType))
		{
			TFirm firm;
			firm << (*sideWinds)[i];

			if(firm.Try())
				m.cat(firm.name);
		}
	}	

	delete sideWinds;
}

static void bookprove_description(mstr &m, TGroup *group) { m.cpy(bookprove->description); }
static void bookprove_date_send(mstr &m, TGroup *group) { m.cpy(bookprove->send); }
static void bookprove_date_receiveAndReturn(mstr &m, TGroup *group) { m.cpy(bookprove->receiveAndReturn); }
static void bookprove_date_destroy(mstr &m, TGroup *group) { m.cpy(bookprove->destroy); }

static void bookprove_text(mstr &m, TGroup *group)
{
	m.printf("%s%s%s", str(bookprove->send), str(bookprove->receiveAndReturn), str(bookprove->destroy));
}

#undef provebook

static const offield bookprove_fields[]=
{
	{ "F_NO", bookprove_no },
	{ "F_DATE", container_date },
	{ "F_CONNECT_NO", bookprove_connect_no },
	{ "F_LAWSUIT_NO", bookprove_lawsuit_no },
	{ "F_NAME", bookprove_name },
	{ "F_DESCRIPTION", bookprove_description },
	{ "F_DATE_SEND", bookprove_date_send },
	{ "F_DATE_RECEIVE", bookprove_date_receiveAndReturn },
	{ "F_DATE_DESTROY", bookprove_date_destroy },
	{ "F_TEXT", bookprove_text },
	{ NULL, NULL }
};

void TBookProve::LoadProveAction(TBookProve *prove)
{
	// 2017:137 s[] -> m
	TProveAction action;
	msql m;

	action.key = prove->key;
	action.FloatCriteria(m);
	m.AddOrder("F_DATE");

	TQuery q(str(m));
	while (q.Read())
	{
		action << q;

		if (action.resolution)
		{
			m.clear();
			m.printf("%s на %Dг.\v", Resolution->Seek(action.resolution), &action.date);
		}

		if (action.resolution == PROVE_RESOLUTION_DESTROY || action.resolution == PROVE_RESOLUTION_RETURN )
		{
			prove->destroy.printf("%s%s%s", str(m), str(action.text), any(action.text) ? "\v" : "");

			if (action.decisionKind && action.sessionKind)
			{
				TKind tKind;
				prove->destroy.printf("съгласно %s от %s на %Dг.\v", Kind->Seek(action.decisionKind),
					tKind.Seek(action.sessionKind), &action.sessionDate);
			}
		}
		else if (action.resolution == PROVE_RESOLUTION_SEND)
		{
			prove->send.cpy(m);

			if (action.outNo && action.outYear)
				prove->send.printf("с Писмо No %d/%dг.\v на", action.outNo, action.outYear);

			msql w("T_SIDE_WIND", "F_UCN, F_UCN_TYPE, F_INVOLVEMENT");
			TSideWindArray sideWinds;

			w.AddLong("F_NO", action.outNo);
			w.AddLong("F_YEAR", action.outYear);
			w.AddChar("F_TYPE", TYPE_OUTREG);
			load_sidebasicv(sideWinds, str(w));

			for (int i = 0; i < sideWinds.Count(); i++)
			{
				TSideWind *sideWind = sideWinds[i];

				if (sideWind->involvement == INVOLVEMENT_RECEIVER)
				{
					add_side_name(prove->send, sideWind, "\v");
					prove->newFormat = true;
				}
			}

			if (prove->newFormat)
				prove->send.cat("\v");

			if (action.decisionKind && action.sessionKind)
			{
				TKind tKind;
				prove->send.printf("съгласно %s от %s на %Dг.\v", Kind->Seek(action.decisionKind),
					tKind.Seek(action.sessionKind), &action.sessionDate);
			}

			prove->send.printf("%s%s", str(action.text), any(action.text) ? "\v" : "");
		}
		else if (action.resolution == PROVE_RESOLUTION_RECEIVE)
			prove->receiveAndReturn.printf("%s%s\v", str(m), str(action.text));
	}
}

// ----- TProveBookGroup -----------------------------------------------------
class TProveBook : public TBook
{
public:
	TProveBook(TWindow *parent, TBookGroup *group, const char *fileName, const offield *bodyFields, bool tNewFromat);

protected:
	bool newFormat;
	TLawsuit lawsuit;
	TRequest request;

	virtual TGroup *Process(TQuery &q);
};

TProveBook::TProveBook(TWindow *parent, TBookGroup *group, const char *fileName, const offield *bodyFields, bool tNewFromat)
	: TBook(parent, group, fileName, bodyFields), newFormat(tNewFromat)
{
}

TGroup  *TProveBook::Process(TQuery &q)
{
	TBookProve *prove = new TBookProve;
	*prove << q;

	if (strchr(TYPE_LAWSUITS, prove->lawsuit.type))
	{
		lawsuit.key = prove->lawsuit;

		if (lawsuit.Try("F_KIND"))
			prove->lawKind = lawsuit.kind;
	    	else
			dprintf("Липсва %s %d/%d\n",  Type->Seek(lawsuit.key.type), lawsuit.key.no, lawsuit.key.year);
	}
	else if (prove->lawsuit.type == TYPE_REQUEST)
	{
		request.key = prove->lawsuit;

		if (request.Try("F_KIND"))
			prove->lawKind = request.kind;
		else
			dprintf("Липсва %s %d/%d\n",  Type->Seek(request.key.type), request.key.no, request.key.year);
	}

	MinMax(prove->key.no, prove->date);
	prove->LoadProveAction(prove);
	prove->newFormat = newFormat;

	return prove;
}

#include "ProveBookDialog.h"
#include "ProveBookDialog.cpp"

// ----- functions -----------------------------------------------------------
void prove_book(TWindow *parent)
{
	TProveBookGroup group;

	if (TProveBookDialog(parent, &group).Execute() == IDOK)
		TProveBook(parent, &group, group.flags & BOOK_NEW_FORMAT ? "ProvBook.htm" : "PrvBook_.htm", bookprove_fields, group.flags & BOOK_NEW_FORMAT ? true : false).Execute(BOOK_EXEC_HEADER);
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
