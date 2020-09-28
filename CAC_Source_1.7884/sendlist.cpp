#include "cac.h"

void init_event(TOutReg *outReg, TInReg *inReg, TInRegEvent *inRegEvent)
{
	inRegEvent->key = inReg->key;			// 2005:318 cut << to key =
	inRegEvent->kind = KIND_INREG_SEND_EVENT;	// 2011:019 new sends: SEND
	inRegEvent->result = RESULT_IOREG_SEND;
	inRegEvent->date = outReg->date;
	inRegEvent->text.cpy("");
	inRegEvent->outNo = outReg->key.no;
	inRegEvent->outYear = outReg->key.year;
	strcpy(inRegEvent->judge, inReg->judge);	// 2007:023 FIX: was missing
}

bool send_inreg(TOutReg *outReg, TInReg *inReg, TInRegEvent *inRegEvent)
{
	TInRegEvent tInRegEvent;

	if (!inRegEvent)
		inRegEvent = &tInRegEvent;

	init_event(outReg, inReg, inRegEvent);

	if (inRegEvent->Insert())
	{
		unschedule(SCHED_NULL, inReg);
		return true;
	}

	return false;
}

void load_list(TInReg *inReg, TGArray<TInReg> *inRegs)
{
	msql m(inReg->Name);	// 2012:152 LPR: s[] -> msql

	m.Add(inReg, "F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE, F_DECISION_DATE, F_DECISION_KIND, F_SESSION_KIND");
	TQuery q(str(m));
	TInReg tInReg;
	char status = inReg->Status();

	while (q.Read())
	{
		TInReg *tInReg = new TInReg;
		*tInReg << q;
		char tStatus = tInReg->Status();

		if (strchr(RESULT_SENDABLES, tStatus) && tInReg->SameComplect(inReg, status, tStatus) == NULL)
			inRegs->Add(tInReg);
		else
			delete tInReg;
	}
}

void send_list(TOutReg *outReg, TInReg *inReg)
{
	// 2012:152 LRQ/FIX: single send for all non-decision
	//if (strchr(KIND_TEXTUAL_IN_DOCS, inReg->kind))
	if (inReg->decisionDate.Empty() || !inReg->decisionKind)
		send_inreg(outReg, inReg);
	else
	{
		TGArray<TInReg> inRegs;
		load_list(inReg, &inRegs);

		for (int i = 0; i < inRegs.Count(); i++)
			send_inreg(outReg, inRegs[i]);
	}
}

void receive_list(TOutReg *outReg)
{
	TInRegEvent inRegEvent;
	char result = outReg->Status();	// 2009:194
	msql m;

	outReg->result = RESULT_IOREG_SEND;
	inRegEvent.OutCriteria(outReg);
	inRegEvent.FloatCriteria(m);
	outReg->result = result;

	TQuery q(str(m));
	while (q.Read())
	{
		inRegEvent << q;
		inRegEvent.result = outReg->Status();
		inRegEvent.date = outReg->returned;
		// 2002:031 PRQ: send + returned trick
		if (outReg->date == outReg->returned)
			inRegEvent.Update("F_RESULT, F_DATE");
		else
			inRegEvent.Insert();
	}
}

static void delete_list(TOutReg *outReg)
{
	if (outReg->inNo)
	{
		TInRegEvent inRegEvent;
		msql m;

		inRegEvent.OutCriteria(outReg);
		inRegEvent.FloatCriteria(m);

		TQuery q(str(m));
		while (q.Read())
		{
			inRegEvent << q;
			delete_inreg_event(&inRegEvent);
		}
	}
}

void delete_send_list(TOutReg *outReg)
{
#if RANGECHECK
	if (outReg->result != RESULT_IOREG_SEND)
		fatal("delete_send_list: result %d", outReg->result);
#endif  // RANGECHECK
	delete_list(outReg);
}

void delete_receive_list(TOutReg *outReg)
{
#if RANGECHECK
	if (!strchr(RESULT_IOREG_RETURNEDS, outReg->result))
		fatal("delete_receive_list: result %d", outReg->result);
#endif  // RANGECHECK
	delete_list(outReg);
}

void revert_receive_list(TOutReg *outReg)
{
#if RANGECHECK
	if (!strchr(RESULT_IOREG_RETURNEDS, outReg->result))
		fatal("revert_receive_list: result %d", outReg->result);
#endif  // RANGECHECK

	if (outReg->date != outReg->returned)
		delete_receive_list(outReg);
	else	// send + returned trick
	{
		TInRegEvent inRegEvent;
		msql m(inRegEvent.Name, MQ_UPDATE);

		m.printf("F_RESULT = '%c'", RESULT_IOREG_SEND);
		inRegEvent.OutCriteria(outReg);
		inRegEvent.FloatCriteria(m, MQ_SUBSEQ);
		inRegEvent.DataBase->Execute(str(m), ET_ERROR);
	}
}
