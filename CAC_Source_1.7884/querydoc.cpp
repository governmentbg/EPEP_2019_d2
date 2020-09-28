#include "cac.h"

static void print_user(THTMLFile &h, const char *prefix, const char *ucn, const char *suffix = NULL)
{
	if (atob(ucn))
	{
		mstr m;

		print_user(m, ucn);
		h.printf("%s %s", prefix, str(m));

		if (suffix)
			h.puts(suffix);
	}
}

static void print_name(THTMLFile &h, const char *prefix, const char *ucn, char ucnType)
{
	mstr m;
	print_name(m, ucn, ucnType);
	h.printf("%s %s", prefix, str(m));
}

static void print_extra(THTMLFile &h, TUCNGroup *ucnGroup)
{
	if (strchr(UCN_CITIZENS, ucnGroup->ucnType))
	{
		TCitizen citizen;
		citizen << ucnGroup;

		if (citizen.Get("F_LNC") && any(citizen.lnc))
			h.printf(" ��� %s", citizen.lnc);
	}
	else if (strchr(UCN_FIRMS, ucnGroup->ucnType))
	{
		TFirm firm;
		firm << ucnGroup;

		if (firm.Try("F_BULSTAT") && any(firm.bulstat))
			h.printf(" ���/������� %s", firm.bulstat);
	}
}

#if TESTVER
static void eportal_mark(mstr &m, size_t offset, const TSideWind *sideWind, const TRCDKeyContainer *container)
{
	TRCDKey origin;

	// 2018-10-29
	if (strchr(TYPE_IOREGS, container->key.type) || container->key.type == TYPE_SURROUND ||
		container->key.type == TYPE_PROVE)
	{
		origin = ((const TPrimeContainer *) container)->key;
	}
	else
		origin = container->key;

	if (origin.type && strchr(TYPE_LAWSUITS, origin.type) &&
		eportal_side_assigned(origin, sideWind->ucnType, sideWind->ucn, sideWind->involvement))
	{
		mbk_insert(&m, "�� ", offset, 3);
	}
}
#endif  // TESTVER

// 2014:107 LPR: public -> static
static void query_sidewinds(THTMLFile &h, const TRCDKeyContainer *container, char status = RESULT_NULL)
{
	// 2004:293 VRQ: dialog-like ordering, LPR: split on load and display
	TGArray<TSideWind> *sideWinds = load_sidewinds(container);

	for (int i = 0; i < sideWinds->Count(); i++)
	{
		TSideWind *sideWind = (*sideWinds)[i];
		bool print = false;
		mstr m;

		if (status)
		{
			print = !isspace(side_wind_notified_mark(sideWind, status, m));
		#if TESTVER
			if (print)
				eportal_mark(m, 0, sideWind, container);
		#endif
		}
		else if ((container->key.type != TYPE_INREG && container->key.type != TYPE_SURROUND) || sideWind->Appealer())
		{
			sideWind->Print(m);
		#if TESTVER
			eportal_mark(m, LENGTH_OF_SIDE_WIND_LEADING, sideWind, container);
		#endif  // TESTVER
			print = true;
		}

		if (print)
		{
			h.puts(str(m));
			print_extra(h, sideWind);
			h.putc('\n');

			// 2004:006 REM: was for sides only
			if (strchr(UCN_CITIZENS, sideWind->ucnType))
			{
				TOldCitizen oldCitizen;

				oldCitizen << sideWind;
				// 2009:327 TRQ: +rank
				if (oldCitizen.Try())
					h.printf("�������� �����: %s %s %s %s %s\n", oldCitizen.rank, oldCitizen.name,
						oldCitizen.reName, oldCitizen.family, oldCitizen.reFamily);
			}
			else if (strchr(UCN_FIRMS, sideWind->ucnType))
			{
				TOldFirm oldFirm;

				oldFirm << sideWind;

				if (oldFirm.Try())
					h.printf("�������� ���: %s\n", oldFirm.name);
			}
		}
	}

	delete sideWinds;
}

static void query_resolution(THTMLFile &h, TRequestResolution *resolution, bool full = false)
{
	if (resolution->resolution)
	{
		// 2007:185 LPR: lawed + assigned support; 2009:289 LPR: cleaned
		if (atob(resolution->assign) && resolution->resolution == RESOLUTION_LAWED)
		{
			TRequestResolution tResolution;
			const char *s;

			tResolution << resolution;
			tResolution.resolution = RESOLUTION_ASSIGNED;
			if ((s = tResolution.text.rchr('\n')) != NULL)
				tResolution.text.cut(s);
			tResolution.eventNo = 0;
			query_resolution(h, &tResolution, full);

			if ((s = resolution->text.rchr('\n')) != NULL)
				resolution->text.delead(s + 1);
		}

		// 2007:256 LRQ: bloat
		if (strchr(RESOLUTION_PERFASSIGNS, resolution->resolution))
		{
			h.printf("%s �� %D�.\n", Kind->Seek(resolution->kind), &resolution->date);
			h.printf("��������: ");
		}

		h.printf("%s ", Resolution->Seek(resolution->resolution));
		// 2004:365 LRQ: use PRINT_JUDGEMENTS for all events #s
		if (resolution->eventNo && PRINT_JUDGEMENTS)
			h.printf("� %ld/", resolution->eventNo);
		else if (resolution->resolution == RESOLUTION_ASSIGNED)
			h.printf(resolution->resolution == RESOLUTION_ASSIGNED ? "�� " : "�� ");

		h.putd("%D�.", resolution->date);

		if (resolution->resolution == RESOLUTION_ASSIGNED)
			print_user(h, " ��", resolution->assign);

		h.putc('\n');
		h.qutp(resolution->text, SIZE_OF_RESOLUTION_TEXT);
		print_user(h, "�����", resolution->judge, "\n");
	}

	// 2004:258 VRQ: query messages
	if (full)
	{
		h.putc('\n');
		query_sidewinds(h, resolution, resolution->Status());
	}
	h.putc('\n');	// 2007:243 LRQ: separator
}

void container_events(TDatedContainer *container, TGSArray<TRCDKeyContainer> *array)
{
	msql m;
	container->FloatCriteria(m);

	TQuery q(str(m), NULL);
	while (q.Read())
	{
		*container << q;
		TDatedContainer *tContainer = kind2DatedContainer(container->kind);
		*tContainer << container;
		array->Add(tContainer);
	}
}

void query_events(const TRequest *request, TGSArray<TRCDKeyContainer> *array)
{
	TInReg inReg;
	TOutReg outReg;
	TRequestResolution resolution;
	TSurround surround;

	inReg.lawsuit = request->key;
	container_events(&inReg, array);
	outReg.lawsuit = request->key;
	container_events(&outReg, array);
	resolution.key = request->key;
	container_events(&resolution, array);
	surround.lawsuit = request->key;
	container_events(&surround, array);
}

// 2004:218 LPR: used in 2+ queries, made a separate function
static void query_last_move(THTMLFile &h, const TRCDKeyGroup *group)
{
	// 2003:052 LRQ: non-judges only; 2012:006 LRQ: re-allow for all
	//if (!strchr(POST_JUDGES, Default->post))
	{
		TMovement *const movement = group->LastMove();
		TVirtPtr VP(movement);

		if (movement)
		{
			h.putc('\n');
			h.printf("%s �� %D�. %T�., %s\n", Kind->Seek(movement->kind), &movement->date, &movement->time,
				movement->text);
		}
	}
}

static void query_connects(THTMLFile &h, const TRCDKeyContainer *container)
{
	TGArray<TConnect> connects;
	mstr m;
	TString *eispp_nmr = (TString *) container->Find("F_EISPP_NMR");

	if (*eispp_nmr->S())
		h.printf("����� ����� �� �� %s\n", eispp_nmr->S());

	load_connects(container, &connects);
	for (int i = 0; i < connects.Count(); i++)
	{
		TConnect *connect = connects[i];

		m.clear();
		connect->Print(m);
		h.puts(str(m));

		// 2011:166
		if (connect->decisionKind)
			h.printf(", %s", Kind->Seek(connect->decisionKind));
		if (!connect->decisionDate.Empty())
		{
			if (!connect->decisionKind)
				h.puts(", �������� ���");
			h.putd(" �� %D�.", connect->decisionDate);
		}
		h.putc('\n');

		if (any(connect->text))
		{
			h.qutt(3);
			h.qutn(connect->text);
		}
	}
	// 2009:261 TRQ
	//if (connects.Count())
	//	h.putc('\n');
}

static void print_subject(THTMLFile &h, const TExternContainer *container, char type)
{
	Subject->type = type;
	Subject->subject = container->subject;
	h.printf("%s %s\n", container->Code(),
		Subject->Try("F_NAME") ? Subject->name : "�������� ������ � �������������� �� ����������");
}

static void print_subjects(THTMLFile &h, const TOriginContainer *container, char type)
{
	if (container->subject)
		print_subject(h, container, type);
	else if (container->key.type != TYPE_REQUEST)
		h.puts("���� ��� �������\n");

	if (container->oldSubject)
	{
		OldSubject->type = type;
		OldSubject->subject = container->oldSubject;
		h.printf("\x06\x06\x06 ���� �������: %ld %s\n", container->oldSubject,
			OldSubject->Try("F_NAME") ? OldSubject->name : "�������� ������ � �������������� �� ����������");
	}
}

static void print_college_composition(THTMLFile &h, const char *prefix, const TLawsuit *lawsuit)
{
	h.printf("%s %s %s", prefix, College->Seek(lawsuit->college), COLLEGE);

	if (PRINT_COMPOSITIONS)
		h.printf(", %s ������", Composition->Seek(lawsuit->composition));
}

static void query_administer(THTMLFile &h, const TRCDKeyGroup *group)
{
	TConnect connect;

	if (connect.TryBy(group, CONNECT_TYPE_AFTER_ADMINISTER))
	{
		char type = connectKind2Type(connect.connectKind);
		TOriginContainer *container = (TOriginContainer *) type2RCDKeyContainer(type);

		container->key.no = atoi(connect.connectNo);
		container->key.year = connect.connectYear;

		h.printf("������%s ���� �������������� %s %s/%ld�. ", type == TYPE_REQUEST ? "" : "�", Type->Seek(type),
			connect.connectNo, connect.connectYear);

		if (type == TYPE_REQUEST)
		{
			if (container->Try("F_COLLEGE"))
				h.printf("%s %s", College->Seek(container->college), COLLEGE);
		}
		else if (container->Try("F_COLLEGE, F_COMPOSITION"))
			print_college_composition(h, "", (TLawsuit *) container);

		h.putc('\n');
		delete container;
	}
}

static void query_doc(THTMLFile &h, const TRequest *request)
{
	// Base data
	h.printf("%s ", Kind->Seek(request->kind));
	// 2006:072 Immediate support
	if (request->quick & QUICK_QUICK)
		h.puts(request->quick & QUICK_IMMEDIATE ? "��������� ������������ " : "����� ������������ ");

	h.printf("- %s %s\n", College->Seek(request->college), COLLEGE);
	h.printf("������ �������� %ld �� %ld�.\n", request->key.no, request->key.year);
	print_subjects(h, request, kind2type(request->lawKind));
	h.putd("���� �� ����������: %D�.\n", request->date);

	// 2007:015 TESTVER only; 2007:198 revived, connects will be altered
#if TESTVER
	{
		char *ct;

		if (request->MainConnectType() == CONNECT_TYPE_JURISDICTION)
			ct = "�������� �� ����������";
		else
		{
#if COURT_TYPE == COURT_APPEAL
			ct = "����������������� ��������";
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
			ct = strchr(KIND_1ST_INDOCS, request->kind) ? "����������������� ��������" : "����������������� ��������";
#endif  // ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
			ct = "����������������� ��������";
#endif  // REGIONAL
		}

		h.qutn(ct);
	}
#endif  // TESTVER

	// 2007:148 LPR: now a function
	query_connects(h, request);

	if (any(request->text))
	{
		h.putc('\n');		// 2009:261 TRQ
		h.qutn(request->text);
	}
	h.putc('\n');

	// 2005:350 LRQ
	{
		TProve prove;

		if (prove.TryBy(request))
		{
			// copied from query prove
			h.printf("%s %ld �� %ld�., ��������� �� %D�.\n", Kind->Seek(prove.kind), prove.key.no, prove.key.year,
				&prove.date);
			h.qutp(prove.description, SIZE_OF_PROVE_DESCRIPTION);
			h.putc('\n');
		}
	}
	// Sides
	query_sidewinds(h, request);
	h.putc('\n');

	// Events
	//if (full)		// 2009:149 LPR: full not used, disable support
	{
		// 2007:243 events have their own \n-s now
		//h.putc('\n');
		TGSArray<TRCDKeyContainer> array;

		query_events(request, &array);
		for (int i = 0; i < array.Count(); i++)
			query_event(h, array[i]);
	}
	h.putc('\n');

	query_last_move(h, request);

	h.font(true);
	query_administer(h, request);

	if (request->oldKind)
	{
		h.printf("�������� ��� �������� �� %s �� ", Kind->Seek(request->oldKind));
		h.printf("%s\n", Kind->Seek(request->kind));
	}

	h.font(false);
}

static void query_judgement(THTMLFile &h, const TJudgement *judgement)
{
	h.printf("����������� �� %s\n", judgement->orderFrom);
	h.putd("� ������� ���� �� %D�.\n", judgement->forceDate);
	// 2008:299 LPR: complect II synced with exec judgement
	h.putd("��������� �� ���������� �� %D�.\n", judgement->givenDate);
	if (!judgement->sendDate.Empty())
	{
		h.printf("��������� �� %s �� %D�.\n", judgement->sendTo, &judgement->sendDate);
		h.qutp(judgement->execText, SIZE_OF_JUDGEMENT_TEXT);
		// complect III
		if (!judgement->execDate.Empty())
		{
			h.printf("��������� � ���������� �� %D�., �������� %s\n", &judgement->execDate, judgement->execBind);
			h.printf("������ �� �� %D�., ����� �� ���������� %s\n", &judgement->countDate, judgement->execPlace);
		}
		if (any(judgement->forgive))
			h.qutn(judgement->forgive);
	}
}

static void query_subevents(THTMLFile &h, TDecision *decision, bool personalData)
{
	msql m;
	TSideWind *const sideWind = personalData ? (TSideWind *) new TJudgement : new TSideWind;
	TVirtPtr VP(sideWind);
	char status = decision->Status();

	*sideWind << decision;
	sideWind->FloatCriteria(m);

	TQuery q(str(m));
	while (q.Read())
	{
		*sideWind << q;
		m.clear();

		if (personalData)
		{
			TChar *judged = (TChar *) sideWind->Find("F_JUDGED");
			TBaseString *text = (TBaseString *) sideWind->Find("F_TEXT");
			TLong *flagsY = (TLong *) sideWind->Find("F_FLAGS_Y");
			TAccusation accusation;
			accusation << sideWind;
			bool tAccusation = accusation.Try() && accusation.resolution;
			bool appendixes = flagsY && flagsY->L();
			bool tJudged = !judged || judged->C() != JUDGED_NOT_KNOWN;
			bool tTexted = text && any(text->S());
			bool t152AS = tAccusation && strchr(RESOLUTION_REQUEST_152AS, accusation.resolution);

			if (tJudged || tTexted || appendixes || t152AS)
			{
				// Base data - UXN, names
				sideWind->Print(m);
				{
					// 2009:281 FIX: regression, copied from judgement list box
					int leading = LENGTH_OF_SIDE_WIND_LEADING + SIZE_OF_JUDGEMENT_STAT;

					mbk_insert(&m, NULL, leading, SIZE_OF_SIDE_WIND_STAT);
					ncstr(m)[leading] = side_wind_notified_mark(sideWind, status);
					ncstr(m)[leading + 1] = ' ';
				}

				h.qutn(str(m));

				if (appendixes)
				{
					static const int mask[] = { JUDGEMENT_Y_APPENDIX_23, JUDGEMENT_Y_APPENDIX_24,
						JUDGEMENT_Y_APPENDIX_25, JUDGEMENT_Y_APPENDIX_26, 0 };
					char sep = ':';	// initial

					h.qutt(SIZE_OF_BASE_WIND_STAT);
					h.puts("���������� ��");

					for (int i = 0; mask[i]; i++)
					{
						if (flagsY->L() & mask[i])
						{
							h.printf("%c ��. 2%c �� ��", sep, '3' + i);
							sep = ',';
						}
					}
					h.putc('\n');
				}

				if (t152AS)
				{
					h.qutt(SIZE_OF_BASE_WIND_STAT);
					h.printf("����� �� ���: %s", Resolution->Seek(accusation.resolution));
					h.putd(" �� %D�.", accusation.arrestDate);
					h.putc('\n');
				}
				else if (tAccusation && strchr(RESOLUTION_REQUEST_152BS, accusation.resolution))
				{
					h.qutt(SIZE_OF_BASE_WIND_STAT);
					h.puts("����� �� ���: ������� �� ��� \"��������� ��� ������\"");
					h.putd(" �� %D�.", accusation.arrestDate);
					h.putc('\n');
					h.qutt(SIZE_OF_BASE_WIND_STAT);
					h.qutn(Resolution->Seek(accusation.resolution));
				}

				if (text)
					h.qutp(text);
			}

			TDate *forceDate = (TDate *) sideWind->Find("F_FORCE_DATE");
			if (forceDate && !forceDate->Date().Empty())
				query_judgement(h, (const TJudgement *) sideWind);

			TString *execRemark = (TString *) sideWind->Find("F_EXEC_REMARK");
			if (execRemark && *execRemark->S())
				h.qutn(execRemark->S());

			if ((!judged || judged->C() != JUDGED_NOT_KNOWN) || (forceDate && !forceDate->Date().Empty()) ||
				(text && *text->S() != '\0'))
			{
				h.putc('\n');
			}
		}
		// 2004:022 LPR: removed non-judgements display (STATUS_1 ...)
		// 2004:219 LRQ: restored non-judgements display based on _
		else if (!isspace(side_wind_notified_mark(sideWind, status, m)))
			h.qutn(str(m));
	}
}

static void query_resolution(THTMLFile &h, const char *name, char resolution)
{
	if (resolution != RESOLUTION_FAKE)
		h.printf("%s: %s\n", name, Resolution->Seek(resolution));
}

// 2012:003 +related
static void query_connect(THTMLFile &h, const TOutReg *outReg, const mstr &remark, char connectType)
{
	TConnect connect;

	connect.key = outReg->key;
	connect.connectType = connectType;

	if (connect.Try())
	{
		TSender sender;

		if (connect.decisionKind || !connect.decisionDate.Empty())
		{
			h.printf("%s ", connect.decisionKind ? Kind->Seek(connect.decisionKind) : "������� ��� ");
			h.putd("�� %D�. ", connect.decisionDate);
			h.puts("�� ");
		}

		sender.ucnType = UCN_INSTANCE_HIGHER;
		strcpy(sender.ucn, connect.ucn);

		h.printf("%s %s/%ld %s\n", ConnectKind->Seek(connect.connectKind), connect.connectNo, connect.connectYear,
			sender.Get() ? sender.name : sender.ucn);
	}

	h.qutp(remark, SIZE_OF_OUTREG_REMARK);
}

static void query_returneds(THTMLFile &h, const TOutReg *outReg)
{
	char result = outReg->Status();

	if (strchr(RESULT_IOREG_RETURNEDS, result))
	{
		if (!strchr(RESULT_IOREG_FINISHEDS, result) && !strchr(RESULT_IOREG_CONTINUEDS, result))
			h.printf("��������: %s\n", Result->Seek(result));
		query_resolution(h, "�������� �� �������� ���������", outReg->resultI);
		if (outReg->receiverType == UCN_INSTANCE_HIGHER)
			query_connect(h, outReg, outReg->remarkVZ, CONNECT_TYPE_VZ_LAWSUIT);
		query_resolution(h, "�������� �� ���������� ���������", outReg->resultKS);
		if (outReg->receiverType == UCN_INSTANCE_HIGHER)
			query_connect(h, outReg, outReg->remarkKS, CONNECT_TYPE_KS_LAWSUIT);
		query_resolution(h, "II �������� �� �������� ���������", outReg->resultII);
		query_resolution(h, "�������� �� ��", outReg->resultMJ);
	}

	if (any(outReg->vssIndex))
		h.printf("�������� - ������: %s\n", outReg->vssIndex);
}

static void query_receiver(THTMLFile &h, const TOutReg *outReg)
{
	TSender sender;

	if (get_receiver(outReg, &sender))
		h.printf(" �%s %s\n", *sender.name == '�' ? "��" : "", sender.name);
	else
	{
		h.putc('\n');
		query_sidewinds(h, outReg);
	}
}

static void query_srcv(THTMLFile &h, TInRegEvent *inRegEvent, TOutReg *outReg, char status)
{
	h.printf("%s ", Result->Seek(status));
	// 2004:365 LRQ: use PRINT_JUDGEMENTS for all events #s
	if (inRegEvent->eventNo && PRINT_JUDGEMENTS)
		h.printf("� %ld ", inRegEvent->eventNo);

	// 2011:129 SGS: bankrupts
	h.printf("�� %D�.", &inRegEvent->date);

	bool tReceiver = false;

	if (status == RESULT_IOREG_SEND)
	{
		tReceiver = outReg && outReg->Try("F_KIND, F_DATE, F_RECEIVER, F_RECEIVER_TYPE");
		h.printf(" � %s %ld �� %ld�.", tReceiver ? Kind->Seek(outReg->kind) : Type->Seek(outReg->key.type),
			outReg->key.no, outReg->key.year);

		if (tReceiver)
			query_receiver(h, outReg);
	}

	if (!tReceiver)
		h.putc('\n');
}

// 2005:335 LRP: a separate method now, feeding electric query
static void query_inreg_event(THTMLFile &h, TInRegEvent *inRegEvent, bool full = false)
{
	if (inRegEvent->Status() == RESULT_IOREG_SEND)
	{
		TOutReg outReg;

		outReg.key.type = TYPE_OUTREG;
		outReg.key.no = inRegEvent->outNo;
		outReg.key.year = inRegEvent->outYear;

		// 2013:077 LRQ: show as lawsuit event; 2013:119 reverted
		query_srcv(h, inRegEvent, &outReg, inRegEvent->Status());
		//query_lawsuit_event(h, &outReg, full); return;
	}
	else if (strchr(RESULT_IOREG_RETURNEDS, inRegEvent->Status()) && inRegEvent->outNo)
	{
		TOutReg outReg;

		outReg.key.type = TYPE_OUTREG;
		outReg.key.no = inRegEvent->outNo;
		outReg.key.year = inRegEvent->outYear;

		if (outReg.Get())
		{
			if (outReg.date == outReg.returned)
				query_srcv(h, inRegEvent, &outReg, RESULT_IOREG_SEND);
			query_srcv(h, inRegEvent, &outReg, inRegEvent->Status());
			query_returneds(h, &outReg);
		}
	}
	else
		query_srcv(h, inRegEvent, NULL, inRegEvent->Status());

	h.qutp(inRegEvent->text, SIZE_OF_INREG_EVENT_TEXT);

	if (full)
	{
		// 2004:258 VRQ: query the messages
		query_sidewinds(h, inRegEvent, inRegEvent->Status());
		h.putc('\n');
	}
}

void query_events(const TInReg *inReg, TGSArray<TRCDKeyContainer> *array)
{
	TInRegEvent inRegEvent;
	TSurround surround;

	inRegEvent.key = inReg->key;
	container_events(&inRegEvent, array);
	surround.lawsuit = inReg->key;
	container_events(&surround, array);
}

static void query_subevents(THTMLFile &h, const TInReg *inReg, bool full)
{
	TGSArray<TRCDKeyContainer> array;

	query_events(inReg, &array);
	for (int i = 0; i < array.Count(); i++)
	{
		TRCDKeyContainer *container = array[i];

		if (container->key.type == TYPE_INREG)
			query_inreg_event(h, (TInRegEvent *) container, full);
		else //if (full)		// 2002:031 always
			query_event(h, container, full ? full : -1);
	}
}

static void print_send(THTMLFile &h, const char *format, const TOutReg *outReg)
{
	mstr m;
	print_send(m, outReg);
	h.printf(format, str(m));
}

void surround_act(mstr &m, const TSurround *surround)
{
	if (surround->decisionKind)
	{
		m.printf("������� %s ", Kind->Seek(surround->decisionKind));
		// 2009:153 FIX: was missing
		if (strchr(KIND_ENDOCS, surround->decisionKind))
		{
			TDecision decision;

			decision.key = surround->lawsuit;
			decision.date = surround->decisionDate;
			decision.kind = surround->decisionKind;

			if (decision.Get("F_SESSION_KIND"))
				m.printf("�� %s ", Kind->Seek(decision.sessionKind));
		}
		m.printf("%D�.", &surround->decisionDate);
	}
}

static void print_triad(THTMLFile &h, const char *prefix, const char *ucn0, const char *ucn1, const char *ucn2,
	const char *skip)
{
	const char *ucns[3] = { ucn0, ucn1, ucn2 };
	int count = 0;

	for (int i = 0; i < 3; i++)
		if (atob(ucns[i]) && strcmp(ucns[i], skip))
			print_user(h, count++ ? "," : prefix, ucns[i]);

	if (count)
		h.putc('\n');
}

// 2016:169 separate function
static void print_accomplys(THTMLFile &h, const TRCDKeyContainer *container)
{
	TCharList *accomplys = (TCharList *) container->Find("F_ACCOMPLYS");
	if (accomplys && *accomplys->S())
	{
		h.qutn("�������� �� ������:");
		for (const char *s = accomplys->S(); *s; s++)
			h.qutn(Accomply->Seek(*s));
	}
}

// 2016:169 separate function
static void print_debtor_status(THTMLFile &h, const TRCDKeyContainer *container)
{
	TCharAlias *debtorStatus = (TCharAlias *) container->Find("F_DEBTOR_STATUS");
	if (debtorStatus && debtorStatus->C())
		h.printf("������ �� ��������: %s\n", DebtorStatus->Seek(debtorStatus->C()));
}

static void query_surrouendoc(THTMLFile &h, const TSurroundment *surrouendoc, bool compact)
{
	if (!compact)
		h.putc('\n');

	h.printf("%s ", Kind->Seek(surrouendoc->decisionKind));

	if (surrouendoc->eventNo && PRINT_JUDGEMENTS)
		h.printf("� %ld/", surrouendoc->eventNo);
	else
		h.puts("�� ");

	mstr m;
	surrouendoc_text(m, surrouendoc);

	h.printf("%D�. ������� %s � %ld/%ld�.\n", &surrouendoc->decisionDate, str(m), surrouendoc->key.no,
		surrouendoc->key.year);
	h.qutp(surrouendoc->text, SIZE_OF_SURROUNDMENT_TEXT);
	// after the text, like TDecision
	print_accomplys(h, surrouendoc);
	print_debtor_status(h, surrouendoc);
}

static void query_surrouendocs(THTMLFile &h, const TSession *session, bool compact)
{
	TGArray<TSurroundment> surrouendocs;
	try2surround(session, &surrouendocs, NULL, KIND_SURROUENDOCS, true);

	for (int i = 0; i < surrouendocs.Count(); i++)
		query_surrouendoc(h, surrouendocs[i], compact);
}

static void query_lawsuit_event(THTMLFile &h, TDatedContainer *container, int full)
{
	// Exact or generic date
	TDate *date = (TDate *) container->Find("F_FROM_DATE");
	TBaseString *text = (TBaseString *) container->Find("F_TEXT");
	TLong *eventNo = NULL;
	TDecision *decision = strchr(KIND_ENDOCS, container->kind) ? (TDecision *) container : NULL;
	TSession *session = strchr(KIND_SESSIONS, container->kind) ? (TSession *) container : NULL;
	TInReg *inReg = strchr(KIND_IN_REG_DOCS, container->kind) ? (TInReg *) container : NULL;
	TOutReg *outReg = strchr(KIND_OUT_REG_DOCS, container->kind) ? (TOutReg *) container : NULL;
	bool compact = full == -1;

	if (compact)
		full = false;

	// 2004:365 LRQ: use PRINT_JUDGEMENTS for all events #s
	if (PRINT_JUDGEMENTS)
		eventNo = (TLong *) container->Find("F_EVENT_NO");

	if (!date || date->Date().Empty())
		date = (TDate *) container->Find("F_DATE");

	if (strchr(TYPE_IOREGS_SURROUNDS, container->key.type))
		h.printf("%s %ld ", container->key.type == TYPE_OUTREG ? "������� �" : "������ �", container->key.no);
	else
	{
		h.printf("%s ", Kind->Seek(container->kind));
		// 2004:363 LPR: ignore PRINT_JUDGEMENTS
		// 2004:364 LRQ: each event on it's own branch, as usual...
		//if (eventNo != NULL && eventNo->L() != 0 && PRINT_JUDGEMENTS)
		if (eventNo != NULL && eventNo->L() != 0 && !session && !decision)
			h.printf("� %ld ", eventNo->L());
	}

	// 2004:364 LRQ: it's no wonder this query has grown to 47kb...
	if (strchr(KIND_ENDOCS, container->kind) && eventNo != NULL && eventNo->L() != 0)
		h.printf("� %ld/", eventNo->L());
	else
		h.puts("�� ");

	h.putd("%D�.", date->Date());

	// 2006:017 LRQ: FIX: format text, it may be too long
	if (strchr(TYPE_IOREGS_SURROUNDS, container->key.type))
		h.printf("- %s", strchr(KIND_ANY_OTHER_DOCUMENTS, container->kind) && any(text->S()) ? text->S() :
			Kind->Seek(container->kind));

	// Optional time
	TTime *time = (TTime *) container->Find("F_TIME");
	if (time && !time->Time().Empty())
		h.printf(", %T�.", &time->Time());
	// Optional end date
	TDate *endDate = (TDate *) container->Find("F_END");
	if (endDate && !endDate->Date().Empty() && endDate->Date() != date->Date())
		h.printf(strchr(KIND_OPEN_SESSIONS, container->kind) ? " �� %D�." : " - %D�.", &endDate->Date());
	// Optional end time
	TTime *enTime = (TTime *) container->Find("F_ENTIME");
	if (enTime && !enTime->Time().Empty())
		h.printf(", %T�.", &enTime->Time());

	if (session)
	{
		if (session->hall)
		{
			h.printf("  %s", Hall->Seek(session->hall));
			if (any(Hall->address))
				h.printf("- %s", Hall->address);
		}

		static const TFlagName SessionFlagsX[] =
		{
			{ SESSION_X_CLOSEDD,	"����������� ��� ������� �����" },
			{ SESSION_X_FIRSET,	"���������� �� I-�� ���������" },
			{ 0,				NULL }
		};

		static const TFlagName SessionFlags[] =
		{
			{ SESSION_CHANJED,	"����� �� �������-���������" },
			{ SESSION_EXPERT,		"�����������/����� �� ���� ����" },
			{ SESSION_PROCEED,	"������ �� ���" },
			{ SESSION_PRIVATE,	"�� ������� �� ����������� � ��������" },
			{ 0,				NULL }
		};

		mstr m;

		print_flags(m, SessionFlagsX, session->flagsX, ", ", "\n");
		print_flags(m, SessionFlags, session->flags, ", ", any(m) ? "" : "\n");
		h.puts(str(m));
	}
	// 2008:072 Decision flags
	else if (decision)
	{
		// 2008:195 LPR: +leading text, +partition
		if (decision->volume)
		{
#if COURT_TYPE == COURT_AREAL
			if (!strchr(RESULT_FIRM_ACCEPTS, decision->result))
#endif  // AREAL
			{
				h.printf(" �������� �� ��������� ��� %ld, ���.%ld, � %ld", decision->volume, decision->page,
					decision->partition);
			}
		}

		static const TFlagName DecisionFlagsX[] =
		{
			{ DECISION_X_CONTRACT,	"��������� �� ���������� ������� / �����" },
			{ 0, NULL }
		};

		static const TFlagName DecisionFlags[] =
		{
			{ DECISION_CONFESS,	"��� ��������� �� ����" },
			{ DECISION_ABSENCE,	"�������������" },
			{ DECISION_PUBLIC,	"�� ������� �� ����������� � ��������" },
			{ 0, NULL }
		};

		mstr m;

		print_flags(m, DecisionFlagsX, decision->flagsX, ", ", "\n");
		print_flags(m, DecisionFlags, decision->flags ^ DECISION_PUBLIC, ", ", any(m) ? "" : "\n");
		h.puts(str(m));
	}
	else if (outReg)
		print_send(h, " - %s", outReg);
	h.putc('\n');

	if (strchr(TYPE_INREGS, container->key.type))
	{
		TInReg *inReg = (TInReg *) container;
		TSurround surround;

		surround.key.type = TYPE_SURROUND;
		surround.key.no = inReg->surroundNo;
		surround.key.year = inReg->surroundYear;
		surround.kind = inReg->surroundKind;

		if (inReg->surroundKind != KIND_NULL)
			h.printf("����� %s %ld �� %ld�.\n", surround.Get() && *surround.text ? surround.text :
				Kind->Seek(inReg->surroundKind), inReg->surroundNo, inReg->surroundYear);
	}

	if (strchr(TYPE_IOREGS_SURROUNDS, container->key.type) && !strchr(KIND_ANY_OTHER_DOCUMENTS, container->kind))
		h.qutp(text);

	// Optional enter [register] / exec date
	TDate *enterDate = (TDate *) container->Find("F_ENTER_DATE");
	if (enterDate)
		h.putd("��������� (�������) �� %D�.\n", enterDate->Date());

	if (!strchr(RESULT_WITH_READY_ANYS, container->Status()) && !strchr(TYPE_IOREGS_SURROUNDS, container->key.type))
	{
		const char *result = rrseek(container->key.type, container->Status());
		bool dirty = false;	// 2013:332 LPR: don't show empty line

		if (!text || strncmp(text->S(), result, strlen(result)))
		{
			h.printf("��������: %s", result);
			dirty = true;
		}

		// 2004:364 LRQ: duplicates the session date, but anyway...
		if (session && eventNo && eventNo->L())
		{
			h.printf(" � %ld/%D�.", eventNo->L(), &container->date);
			dirty = true;
		}

		if (dirty)
			h.putc('\n');
	}

	TLongAlias *slcReason = (TLongAlias *) container->Find("F_SLC_REASON");
	if (slcReason && slcReason->L())
		h.printf("���������: %s\n", SLCReason->Seek(slcReason->L()));

#if COURT_TYPE == COURT_AREAL
	if (strchr(RESULT_FIRM_ACCEPTS, container->Status()))
	{
		h.putd("������� � �� �� %D�.\n", decision->enterDate);
		h.printf("�������� %ld, ��� %ld, �������� %ld, ������� %ld, ���� %ld\n", decision->registry, decision->volume,
			decision->page, decision->partition, decision->level);
	}
#endif  // AREAL

	if (session && session->Status() != RESULT_FIXED)
	{
		print_user(h, "���������:", session->judge, "\n");
		print_triad(h, "�����:", session->judgeX, session->reJudgeX, session->president, session->judge);
		print_triad(h, "������� ����������:", session->juror, session->reJuror, session->jurorX, "");
		print_user(h, "�������� ������� ���������:", session->reserve, "\n");
		// 2011:334; 2011:340 for open sessions only
		if (strchr(KIND_OPEN_SESSIONS, session->kind))
		{
			print_user(h, "��������:", session->prosecutor, "\n");
			print_user(h, "��������:", session->secretar, "\n");
		}
	}

	// 2009:161 receiverType support
	TSender sender;
	if (outReg && get_receiver(outReg, &sender))
		h.printf("�������� �� %s\n", sender.name);

	// Optional text
	if (text && !strchr(TYPE_IOREGS_SURROUNDS, container->key.type))
		h.qutp(text);

	TSurround *surround = strchr(KIND_SURROUNDS, container->kind) ? (TSurround *) container : NULL;

	if (surround)
	{
#if COURT_TYPE == COURT_AREAL
		if (surround->lawsuit.type == TYPE_FIRM_LAW && surround->kind == KIND_CHANGE_SURROUND)
			print_user(h, "�����:", surround->judge, "\n");
#endif  // AREAL
		// 2009:147 TRQ: FIX: was missing
		{
			mstr m;
			surround_act(m, surround);
			if (any(m))
				h.qutn(str(m));
		}
		query_sidewinds(h, surround);

		msql m;
		TSurroundment surroundment;

		surroundment << surround;
		surroundment.FloatCriteria(m);

		TQuery q(str(m));
		while (q.Read())
		{
			surroundment << q;
			h.printf("��������� %s �� %D�.\n", Kind->Seek(surroundment.sessionKind), &surroundment.sessionDate);
			h.qutp(surroundment.text, SIZE_OF_SURROUNDMENT_TEXT);
		}
	}
	else if (inReg)
	{
		query_sidewinds(h, inReg);
		query_subevents(h, inReg, full);
	}
	else if (outReg)
	{
		query_sidewinds(h, outReg);
		query_returneds(h, outReg);
	}
	// 2008:009 LRQ: always show notifications for firm laws
	else if (session && (full || session->key.type == TYPE_FIRM_LAW))
	{
		// 2008:009 LPR: don't show sides if [firm law and] not full
		if (full)
		{
			h.putc('\n');
			query_sidewinds(h, session);
		}
		// 2004:258 VRQ: query the messages
		h.putc('\n');
		query_sidewinds(h, session, session->Status());
	}

	TBaseString *remark = (TBaseString *) container->Find("F_REMARK");
	if (remark)
		h.qutp(remark);

	// Optional subevents (they have a separator)

	print_accomplys(h, container);
	print_debtor_status(h, container);	// 2010:342

	TDate *returned = (TDate *) container->Find("F_RETURNED");
	if (returned && !returned->Date().Empty())
	{
		// 2006:188 LPR: settlements should be unmotivable anyway
		// 2006:188 LRQ: motives date moved below the act
		if (!decision || !(decision->flags & DECISION_MOTIVES))
		{
			h.printf("���� �� %s: %D�.\n", strchr(KIND_DECISIONS, container->kind) ? "���������" : "�������",
				&returned->Date());
		}
	}

	if (decision)
	{
		if (any(decision->syndicate))
		{
			h.qutn("������:");
			h.qutp(decision->syndicate, SIZE_OF_SYNDICATE);
		}
		// 2005:130 LRQ: FIX: query for both judgements and messages
		//query_subevents(f, decision, decision->PersonalData());
		if (decision->PersonalData())
			query_subevents(h, decision, true);
		query_subevents(h, decision, false);

		if (any(decision->opinion))
		{
			h.qutn("������� ������:");
			h.qutp(decision->opinion, SIZE_OF_OPINION);
		}
		// 2006:188 LRQ: display motives date and text below the act
		if (returned && !returned->Date().Empty() && (decision->flags & DECISION_MOTIVES))
		{
			h.putd("���� �� ��������� �� ��������: %D�.\n", returned->Date());
			h.qutp(decision->motives, SIZE_OF_MOTIVES);
		}

		// 2008:072
		h.putd("���� �� ������� � ������� ����: %D�.\n", decision->forceDate);
		h.putd("���� �� ����������� �� ������: %D�.\n", decision->completed);
	}
	else if (session)
		h.putd("���� �� ������� � ������� ����: %D�.\n", session->forceDate);

	if (session || decision)
	{
		TSubpoena subpoena;
		msql m("T_SUBPOENA S JOIN T_EXEC_LIST E ON E.F_NO = S.F_NO AND E.F_YEAR = S.F_YEAR "
			"AND E.F_TYPE = S.F_TYPE AND E.F_EXEC_LIST_NO = S.F_EXEC_LIST_NO "
			"AND E.F_EXEC_LIST_DATE = S.F_SUBPOENA_DATE AND E.F_EXEC_LIST_KIND = S.F_EXEC_LIST_KIND",
			"DISTINCT S.F_SUBPOENA_KIND, S.F_SUBPOENA_NO, S.F_SUBPOENA_DATE, S.F_EXEC_LIST_NO, "
			"S.F_EXEC_LIST_KIND, S.F_SUBPOENA_RECEIVED, E.F_NIA_NO, E.F_NIA_YEAR, E.F_NIA_TYPE");

		m.AddLong("S.F_NO", container->key.no);
		m.AddLong("S.F_YEAR", container->key.year);
		m.AddChar("S.F_TYPE", container->key.type);
		m.AddChar("S.F_KIND", container->kind);
		m.AddDate("S.F_DATE", container->date);
		m.AddIsNull("S.F_EXEC_LIST_NO", false);

		TQuery q(str(m));
		while (q.Read())
		{
			subpoena << q;
			h.printf("%s �������� �� %D�.", SubpoenaKind->Seek(subpoena.subpoenaKind), &subpoena.subpoenaDate);

			if (subpoena.subpoenaKind == SUBPOENA_KIND_EXEC_COUNTRY)
			{
				TExecList execList;

				execList << q;
				execList.no = container->key.no;
				execList.year = container->key.year;
				execList.type = container->key.type;
				execList.execListDate = subpoena.subpoenaDate;

				if (execList.nia.no)
				{
					TOutReg outReg;

					h.printf(" �������� � �������� � %ld/%ld�.", execList.nia.no, execList.nia.year);

					if (execList.CheckSend(&outReg))
					{
						m.printf(", ���. � %ld�." + !execList.nia.no, outReg.key.no);

						if (outReg.date.Empty())
							m.printf("%ld�.", outReg.key.year);
					}
				}
			}
			else if (!subpoena.subpoenaReceived.Empty())
				h.printf(" ������ �� %D�.", &subpoena.subpoenaReceived);

			h.putc('\n');

			// 2015:118 PRJ: collects
			TExecListCollect collect;

			collect << q;
			collect.type = container->key.type;
			collect.execListYear = subpoena.subpoenaDate.year;
			collect.FloatCriteria(m);
			m.AddFlags("F_FLAGS", EXEC_LIST_COLLECT_VOLUNTARY, EXEC_LIST_COLLECT_VOLUNTARY, EXEC_LIST_COLLECT_ALL);

			TQuery r(str(m));
			while (r.Read())
			{
				collect << r;
				h.printf("���������� ���������� �� %D ", &collect.date);

				if (collect.sum == 0.0)
					h.printf("%s\n", collect.sumText);
				else
				{
					Currency->Seek(collect.currency);
					h.printf("%.2lf %s\n", collect.sum, Currency->numeric);
				}
			}
		}

		if (session && strchr(RESULT_SURROUENDOCS, session->result))
			query_surrouendocs(h, session, compact);
	}

	if (!compact)
		h.putc('\n');
}

void query_event(THTMLFile &h, TRCDKeyContainer *container, sign full)
{
	if (strchr(KIND_REQUEST_RESOLUTIONS, container->kind))
		query_resolution(h, (TRequestResolution *) container, full);
	else if (strchr(KIND_INREG_EVENTS, container->kind))
		query_inreg_event(h, (TInRegEvent *) container, full);
	else
		query_lawsuit_event(h, (TDatedContainer *) container, full);
}

void query_event(TRCDKeyContainer *container)
{
	THTMLFile h;

	h.OpenTemp();
	query_event(h, strchr(KIND_ENDOC_RIDERS, container->kind) ? ((TDecisionRider *) container)->decision : container, true);
	h.Show();
}

void query_events(const TLawsuit *lawsuit, TGSArray<TRCDKeyContainer> *array)
{
	TSession session;
	TDecision decision;
	TSurround surround;
	TInReg inReg;
	TOutReg outReg;

	session.key = lawsuit->key;
	container_events(&session, array);
	decision.key = lawsuit->key;
	container_events(&decision, array);
	surround.lawsuit = lawsuit->key;
	container_events(&surround, array);
	inReg.lawsuit = lawsuit->key;
	container_events(&inReg, array);
	outReg.lawsuit = lawsuit->key;
	container_events(&outReg, array);
}

static void query_law_events(THTMLFile &h, const TLawsuit *lawsuit)
{
	TGSArray<TRCDKeyContainer> array;
	query_events(lawsuit, &array);

	if (!lawsuit->completed.Empty())
	{
		TDecision *last = NULL;

		for (int i = 0; i < array.Count(); i++)
			if (strchr(KIND_ENDOCS, array[i]->kind))
				last = (TDecision *) array[i];

		if (last)
			last->completed = lawsuit->completed;
	}

	for (int i = 0; i < array.Count(); i++)
		query_event(h, array[i], false);
}

static void query_arc(THTMLFile &h, TArchive *archive)
{
	// 2006:180 LRQ: small format spec changes
	h.printf("���������� �� %D�. � ������� � %ld/%ld�., ������ %ld\n", &archive->archiveDate, archive->archiveNo,
		archive->archiveYear, archive->linkNo);
	h.qutp(archive->archiveRemark, SIZE_OF_ARCHIVE_TEXT);
	// 2006:044 LRQ
	h.putc('\n');
	if (!archive->killDate.Empty())
	{
		h.putd("��������� �� %D�.", archive->killDate);
		if (archive->killerNo || !archive->killerDate.Empty())
		{
			h.puts(", �������� �� �����������");
			if (archive->killerNo)
				h.printf(" � %ld", archive->killerNo);
			h.putd(" �� %D�.", archive->killerDate);
			h.putc('\n');
		}
		if (archive->remainderNo || archive->remainderYear)
		{
			h.puts("� ��� ");
			if (archive->remainderNo)
				h.printf("%ld ", archive->remainderNo);
			if (archive->remainderYear)
				h.printf("�� %ld�. ", archive->remainderYear);
			h.qutn("�� ��������: ");
			h.qutp(archive->remainder, SIZE_OF_REMAINDER_TEXT);
		}
		h.putc('\n');
		h.qutp(archive->killRemark, SIZE_OF_ARCHIVE_TEXT);
	}
}

static void query_arc(THTMLFile &h, const TExternContainer *container)
{
	TArchive archive;

	if (archive.TryBy(container, NULL))
	{
		h.putc('\n');
		query_arc(h, &archive);
	}
}

static void query_payments(THTMLFile &h, const TOriginContainer *container)
{
	TPayment payment;
	msql m(payment.Name);
	bool first = true;

	m.AddLong("F_NO", container->key.no);
	m.AddLong("F_YEAR", container->key.year);
	m.AddChar("F_CONNECT_KIND", type2ConnectKind(container->key.type));
	m.AddLong("F_COURT", COURT_CODE);
	m.AddString("NOT F_TRANS_ID", TRANS_ID_PENDING);
	m.AddOrder("F_DATE, F_TIME");

	TQuery q(str(m));
	while (q.Read())
	{
		if (first)
		{
			h.puts("\n������� ���� ���� ��� ��������:\n");
			first = false;
		}

		payment << q;
		h.printf("%s %.2lf��., ������� �� %D�., %lT�.", SumType->Seek(payment.sumType), payment.sum, &payment.date,
			&payment.time);
		print_name(h, " ������: ", payment.ucn, payment.ucnType);
		print_name(h, ", ��������� ����: ", payment.obl, payment.oblType);
		h.putc('\n');
	}
}

void query_law(THTMLFile &h, const TLawsuit *lawsuit, bool full)
{
#if 20170235
	TArchive archive;
	bool killed = archive.TryBy(lawsuit, "F_KILL_DATE") && !archive.killDate.Empty();

	if ((lawsuit->flags & LAWSUIT_LIMITED) || killed)
	{
		h.puts("\ab\b");
		if (lawsuit->flags & LAWSUIT_LIMITED)
		{
			h.puts("���� � ��������� ������");
			if (killed)
				h.puts(", ");
		}
		if (killed)
			h.puts("���������");//h.puts("\aspan style=\"color:red\"\b���������\a/span\b");
		h.puts("\a/b\b\n\n");
	}
#else  // 20170235
	if (lawsuit->flags & LAWSUIT_LIMITED)
		h.puts("���� � ��������� ������\n\n");
#endif  // 20170235
	h.printf("%s %ld �� %ld�.", Kind->Seek(lawsuit->kind), lawsuit->key.no, lawsuit->key.year);
	// 2006:072 Immediate support
	if (lawsuit->quick & QUICK_QUICK)
		h.puts(lawsuit->quick & QUICK_IMMEDIATE ? " ��������� ������������" : " ����� ������������");
	if (lawsuit->cutDown)
	{
		h.puts(" ��������� ������������");

		if (lawsuit->cutDown == LAWSUIT_CUTDOWN_P1)
			h.puts(" ��.371, �.1 ���");
		else if (lawsuit->cutDown == LAWSUIT_CUTDOWN_P2)
			h.puts(" ��.371, �.2 ���");
	}
	if (lawsuit->flags & LAWSUIT_NEWGPK)
		h.puts(" �� ����� ���");

	// 2008:080 LRQ: moved after subject/danger
	print_college_composition(h, "", lawsuit);
	h.putc('\n');

	// 2007:016 LRQ: show subject w/#
	print_subjects(h, lawsuit, lawsuit->key.type);
	if (lawsuit->danger)
		h.printf("�������� ���������� ��������: %s\n", Danger->Seek(lawsuit->danger));

	print_user(h, "����� ��� ����������", lawsuit->rejudge, "\n");
	print_user(h, "�����", lawsuit->judge, "\n");

	if (lawsuit->key.type == TYPE_FIRM_LAW)
	{
		TDamn damn;

		// 2004:167 LPR: conditional
		if (find_damn(lawsuit, &damn))
		{
			h.printf("����� %s %s\n", damn.name, CompanyType->Seek(damn.companyType));
			h.putd("��������������� � �� �� %D�.\n", damn.reRegister);
		}
	}
	h.putc('\n');

	// Creation
	h.putd("�������� � ���� �� %D�.\n", lawsuit->receivedDate);
	h.putd("������������ �� ���������� �� %D�.\n", lawsuit->previousDate);
	h.printf("���������� �� %D�. �� %s\n", &lawsuit->date, Kind->Seek(lawsuit->sourceKind));

	if (lawsuit->source.no)
	{
		TRequest request;
		request.key = lawsuit->source;

		if (request.Get("F_DATE"))
			h.printf("������ �������� %ld �� %D�.\n", lawsuit->source.no, &request.date);
		else
			h.printf("������ �������� %ld �� %ld�.\n", lawsuit->source.no, lawsuit->source.year);
	}

	// 2007:015 TESTVER only; 2007:198 revived, connects will be altered
#if TESTVER
	{
		const char *ct;

		switch (lawsuit->MainConnectType())
		{
			case CONNECT_TYPE_JURISDICTION : ct = "���� �� ����������"; break;
			case CONNECT_TYPE_RERETURNED_LAWSUIT : ct = "������� ���� �������������"; break;
			case CONNECT_TYPE_RENEWED_LAWSUIT :
		#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_REGIONAL
				ct = "����������� ����";
		#endif  // APPEAL || AREAL || REGIONAL
		#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
				ct = strchr(KIND_1ST_LAWSUITS, lawsuit->kind) ? "����������������� ����������� ����" :
					"����������������� ����������� ����";
		#endif  // AREAL || MILITARY
				break;
			case CONNECT_TYPE_RETURNED_LAWSUIT :
			{
		#if COURT_TYPE == COURT_APPEAL
				ct = "����������������� ������� ����";
		#endif  // APPEAL
		#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
				ct = strchr(KIND_1ST_LAWSUITS, lawsuit->kind) ? "����������������� ������� ����" :
					"����������������� ������� ����";
		#endif  // ADMIN || AREAL || MILITARY
		#if COURT_TYPE == COURT_REGIONAL
				ct = "����������������� ������� ����";
		#endif  // REGIONAL
				break;
			}
			default :
			{
		#if COURT_TYPE == COURT_APPEAL
				ct = "���������������� ����";
		#endif  // APPEAL
		#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
				ct = strchr(KIND_1ST_LAWSUITS, lawsuit->kind) ? "����������������� ����" : "����������������� ����";
		#endif  // ADMIN || AREAL || MILITARY
		#if COURT_TYPE == COURT_REGIONAL
				ct = "����������������� ����";
		#endif  // REGIONAL
				break;
			}
		}
		h.qutn(ct);
	}
#endif  // TESTVER

	// 2007:148 LPR: now a function
	query_connects(h, lawsuit);

	if (any(lawsuit->text))
		h.qutn(lawsuit->text);

	h.qutp(lawsuit->important, SIZE_OF_LAWSUIT_IMPORTANT);

	h.putc('\n');

	// 2005:350 LRQ
	{
		TProve prove;

		if (prove.TryBy(lawsuit))
		{
			// copied from query prove
			h.printf("%s %ld �� %ld�., ��������� �� %D�.\n", Kind->Seek(prove.kind), prove.key.no, prove.key.year,
				&prove.date);
			h.qutp(prove.description, SIZE_OF_PROVE_DESCRIPTION);
			h.putc('\n');
		}
	}

	// Sides
	query_sidewinds(h, lawsuit);
	h.putc('\n');

	if (full)
		query_law_events(h, lawsuit);

	h.font(true);
	query_administer(h, lawsuit);

	if (lawsuit->oldKind)
	{
		h.printf("�������� ��� ���� �� %s �� ", Kind->Seek(lawsuit->oldKind));
		h.printf("%s\n", Kind->Seek(lawsuit->kind));
	}

	if (full)
	{
		// Final data
		query_last_move(h, lawsuit);

		// 2012:046 LRQ: all users
		//if (Default->post == POST_ADMIN)
		{
		#if 20170047
			h.putc('\n');

			TGSArray<TLawInterval> intervals;
			{
				TLawInterval interval;
				interval.key = lawsuit->key;
				interval.LoadFloat(intervals);
			}

			for (int i = 0; i < intervals.Count(); i++)
			{
				const TLawInterval *interval = intervals[i];

				if (interval->flags & LAW_INTERVAL_SECONDARY)
					h.putd("������: %D�.\n", interval->date);
			#if TESTVER
				h.putd("���������� �� I-�� ���������: %D�.\n", interval->firstSet);
			#endif  // TESTVER
				h.putd("�������� �� %D�.\n", interval->finished);
				if (interval->age != AGE_NONE_YET)
					h.printf("���������������: %s\n", Age->Seek(interval->age));
				if (interval->grant != GRANT_NONE_YET)
					h.printf("������ �� ��������� �� ����: %s\n", Grant->Seek(interval->grant));
			}
		#else  // 20170047
			h.putc('\n');
			h.putd("�������� �� %D�.\n", lawsuit->finished);
			if (lawsuit->age != AGE_NONE_YET)
				h.printf("���������������: %s\n", Age->Seek(lawsuit->age));
			if (lawsuit->grant != GRANT_NONE_YET)
				h.printf("������ �� ��������� �� ����: %s\n", Grant->Seek(lawsuit->grant));
		#endif  // 20170047
		}
	}

	h.putd("������� �� ������������������ ��� �� %D�.\n", lawsuit->returnedDate);
	h.putd("������ � ������� ���� � ������ �� %D�.\n", lawsuit->forceDate);
	query_arc(h, lawsuit);

	// 2012:062
	{
		TConnect connect;
		msql m(connect.Name, "F_NO, F_YEAR, F_TYPE");

		itoa(lawsuit->key.no, connect.connectNo, 10);
		m.AddString("F_CONNECT_NO", connect.connectNo);
		m.AddLong("F_CONNECT_YEAR", lawsuit->key.year);
		// 2013:294 TRQ/IRQ: exact target lawsuit type -> any
		m.AddChars("F_TYPE", TYPE_LAWSUITS);
		m.AddChar("F_UCN_TYPE", UCN_INSTANCE_SELF);
		// 2012:140 LPR: check same lawsuit type -> check connect kind
		m.AddChar("F_CONNECT_KIND", type2ConnectKind(lawsuit->key.type));

		TQuery q(str(m));
		while (q.Read())
		{
			TLawsuit tLawsuit;

			connect << q;
			tLawsuit.key = connect.key;
			bool found = tLawsuit.Get("F_KIND, F_COLLEGE, F_COMPOSITION");

			h.printf("%s %s %ld �� %ld", strchr(CONNECT_TYPE_LAW_LINKS, connect.connectType) ? "������������� �" :
				"����������", found ? Kind->Seek(tLawsuit.kind) : Type->Seek(connect.key.type), connect.key.no,
				connect.key.year);

			if (found)
				print_college_composition(h, ",", &tLawsuit);

			h.putc('\n');
		}
	}

	query_payments(h, lawsuit);	// 2016:085
	h.font(false);
}

static void query_inr(THTMLFile &h, const TInReg *inReg)
{
	h.printf("%s %ld/%ld�. - %s", Kind->Seek(inReg->kind), inReg->key.no, inReg->key.year, College->Seek(inReg->college));
	if (!strchr(COLLEGE_COMMONS, inReg->college))
		h.printf(" %s", COLLEGE);
	h.putc('\n');

	h.putd("������� � ���� �� %D�.\n", inReg->date);
	if (inReg->lawsuit.type)
	{
		h.printf("�� %s", Type->Seek(inReg->lawsuit.type));
		if (inReg->lawsuit.no)
			h.printf(" %ld", inReg->lawsuit.no);
		if (inReg->lawsuit.year)
			h.printf("/%ld�.", inReg->lawsuit.year);
		h.putc('\n');
	}
	if (inReg->decisionKind)
	{
		h.printf("����� %s", Kind->Seek(inReg->decisionKind));
		if (inReg->decision.no)
			h.printf(" %ld", inReg->decision.no);
	#if TESTVER
		if (inReg->decision.year)
			h.printf("/%ld�.", inReg->decision.year);
	#endif  // TESTVER
		h.putc('\n');
	}
	if (inReg->sessionKind)
	{
		h.printf("�� %s", Kind->Seek(inReg->sessionKind));
		h.putd(" �� %D�.", inReg->decisionDate);
		h.putc('\n');
	}
	if (inReg->surroundKind)
	{
		h.puts(Kind->Seek(inReg->surroundKind));
		if (inReg->surroundNo)
			h.printf(" %ld", inReg->surroundNo);
		if (inReg->surroundYear)
			h.printf("/%ld�.", inReg->surroundYear);
		h.putc('\n');
	}
	if (any(inReg->text))
		h.qutn(inReg->text);
	h.putc('\n');

	query_sidewinds(h, inReg);
	if (any(inReg->eportalMail))
		h.printf("���������� ����: %s\n", inReg->eportalMail);
	// 2016:264
	if (inReg->eportalMode)
	{
		h.printf("%s:\n", inReg->EPortalModeText());

		TEPortAssign assign;
		msql m;

		assign.key = inReg->key;
		assign.FloatCriteria(m);

		TQuery q(str(m));
		while (q.Read())
		{
			assign << q;
			h.printf("- %s %ld/%ld\n", Type->Seek(assign.lawsuit.type), assign.lawsuit.no, assign.lawsuit.year);
		}
	}
	h.putc('\n');

	query_subevents(h, inReg, true);
}

static void query_outr(THTMLFile &h, const TOutReg *outReg)
{
	h.printf("%s %ld/%ld�. - %s", Kind->Seek(outReg->kind), outReg->key.no, outReg->key.year,
		College->Seek(outReg->college));
	if (!strchr(COLLEGE_COMMONS, outReg->college))
		h.printf(" %s", COLLEGE);
	h.putc('\n');
	print_send(h, "%s\n", outReg);
	if (any(outReg->text))
		h.qutp(outReg->text, SIZE_OF_OUTREG_REMARK);

	// InRegDocs
	TInRegEvent inRegEvent;
	TInReg inReg;
	msql m;

	inRegEvent.OutCriteria(outReg);
	inRegEvent.FloatCriteria(m, "F_NO, F_YEAR, F_TYPE");

	TQuery q(str(m));
	if (q.Read())
	{
		h.qutn("�� ��������� ��:");
		do
		{
			inRegEvent << q;
			inReg.key = inRegEvent.key;
			h.printf("%s %ld/%ld�.\n", inReg.Try("F_KIND") ? Kind->Seek(inReg.kind) : Type->Seek(inRegEvent.key.type),
				inRegEvent.key.no, inRegEvent.key.year);
		} while (q.Read());
	}

	if (outReg->lawsuit.no)
	{
		h.printf("�� %s %ld/%ld�.\n", outReg->lawsuit.type ? Type->Seek(outReg->lawsuit.type) : "��������",
			outReg->lawsuit.no, outReg->lawsuit.year);
	}
	h.putc('\n');
	h.putd("�������� �� %D�.", outReg->date);
	query_receiver(h, outReg);
	h.putc('\n');
	h.putd("������ �� %D�.\n", outReg->returned);
	query_returneds(h, outReg);
	h.qutp(outReg->remark, SIZE_OF_OUTREG_REMARK);
}

static void query_prove_action(THTMLFile &h, TProveAction *proveAction, bool = false)
{
	h.printf("%s �� %D�. ", Resolution->Seek(proveAction->resolution), &proveAction->date);
	if (proveAction->outNo)
	{
		TOutReg outReg;

		outReg.key.no = proveAction->outNo;
		outReg.key.year = proveAction->outYear;
		h.printf("� %s %ld �� %ld�.", outReg.Get() ? Kind->Seek(outReg.kind) : "������� ��������", proveAction->outNo,
			proveAction->outYear);
	}
	h.putc('\n');
	if (!proveAction->sessionDate.Empty())
	{
		if (proveAction->decisionKind)
		{
			h.printf("�������� %s ", Kind->Seek(proveAction->decisionKind));
			h.printf("�� %s ", Kind->Seek(proveAction->sessionKind));
		}
		else
			h.printf("�������� %s ", Kind->Seek(proveAction->sessionKind));
		h.putd("�� %D�.\n", proveAction->sessionDate);
	}
	h.qutp(proveAction->text, SIZE_OF_PROVE_ACTION_TEXT);
	h.putc('\n');
}

void query_events(const TProve *prove, TGSArray<TRCDKeyContainer> *array)
{
	TProveAction proveAction;
	proveAction.key = prove->key;
	container_events(&proveAction, array);
}

// 2005:349 copied from query inreg event
static void query_subevents(THTMLFile &h, const TProve *prove, bool full)
{
	TGSArray<TRCDKeyContainer> array;

	query_events(prove, &array);
	for (int i = 0; i < array.Count(); i++)
		query_prove_action(h, ((TProveAction *) array[i]), full);
}

static void query_prove(THTMLFile &h, const TProve *prove)
{
	h.printf("%s %ld �� %ld�., ��������� �� %D�.\n", Kind->Seek(prove->kind), prove->key.no, prove->key.year, &prove->date);
	h.qutp(prove->description, SIZE_OF_PROVE_DESCRIPTION);
	if (any(prove->remark))
		h.qutn(prove->remark);
	h.putc('\n');

	// LRQ: before the request/lawsuit data
	query_sidewinds(h, prove);
	h.putc('\n');

	// LPR: following header code is from query law/doc with minor changes
	if (strchr(TYPE_LAWSUITS, prove->lawsuit.type))
	{
		h.printf("%s %ld �� %ld�.", Type->Seek(prove->lawsuit.type), prove->lawsuit.no, prove->lawsuit.year);

		TLawsuit lawsuit;
		lawsuit.key = prove->lawsuit;
		if (lawsuit.Get())
		{
			// lawsuit header, copied from query law
			print_college_composition(h, "-", &lawsuit);
			h.putc('\n');

			// following paragraphs copied from query law
			h.printf("���������� �� %D�. �� %s\n", &lawsuit.date, Kind->Seek(lawsuit.sourceKind));

			if (lawsuit.source.no)
			{
				TRequest request;

				h.printf("������ �������� %ld �� ", lawsuit.source.no);
				request.key = lawsuit.source;

				if (request.Get("F_DATE"))
					h.printf("%D", &request.date);
				else
					h.printf("%ld�.\n", lawsuit.source.year);
			}
		}
	}
	else
	{
		TRequest request;

		request.key = prove->lawsuit;
		if (request.Get())
		{
			// request header, copied from query doc, remopved quick
			h.printf("%s ", Kind->Seek(request.kind));
			h.printf("- %s %s\n", College->Seek(request.college), COLLEGE);
		}
		h.printf("������ �������� %ld �� %ld�.\n", request.key.no, request.key.year);
	}

	h.putc('\n');
	query_subevents(h, prove, false);
}

static void query_law(THTMLFile &h, const TExternLawsuit *lawsuit)
{
	h.printf("�������� %s %ld �� %ld�.", Type->Seek(lawsuit->key.type), lawsuit->key.no, lawsuit->key.year);
	if (lawsuit->quick & QUICK_QUICK)
		h.puts(lawsuit->quick & QUICK_IMMEDIATE ? " ��������� ������������" : " ����� ������������");
	h.putc('\n');
	print_subject(h, lawsuit, lawsuit->key.type);
	h.putc('\n');
	h.printf("���������� �� %D�.", &lawsuit->date);
	if (lawsuit->sourceKind)
		h.printf(" �� %s\n", Kind->Seek((char) lawsuit->sourceKind));
	h.putc('\n');
	h.putd("������ � ������� ���� � ������ �� %D�.\n", lawsuit->forceDate);
	query_arc(h, lawsuit);
}

static void query_group(THTMLFile &h, const TRCDKeyGroup *group, bool full)
{
	if (LOG_QUERIES)
		log(LOG_QUERY, 0, &group->key, "�� ����/��������");

	switch (group->key.type)
	{
		case TYPE_REQUEST : query_doc(h, (const TRequest *) group); break;
		case TYPE_INREG :
		{
			query_inr(h, (const TInReg *) group);
			query_last_move(h, group);
			break;
		}
		case TYPE_OUTREG : query_outr(h, (const TOutReg *) group); break;
		case TYPE_PROVE :
		{
			query_prove(h, (const TProve *) group);
			query_last_move(h, group);
			break;
		}
		case TYPE_SURROUND :
		{
			TRCDKey key = ((const TSurround *) group)->lawsuit;
			// shadow parameter group
			TRCDKeyGroup *group = type2RCDKeyContainer(key.type);

			group->key = key;

			if (group->Get(group->key.type == TYPE_SURROUND ? "F_TYPE, F_LAWSUIT_NO, F_LAWSUIT_YEAR, F_LAWSUIT_TYPE" :
				NULL))
			{
				query_group(h, group, true);
			}

			delete group;
			break;
		}
		default :
		{
			if (!strchr(TYPE_LAWSUITS, group->key.type))
				fatal("%d: type not supported by query group", group->key.type);

			const TExternContainer *container = (const TExternContainer *) group;

			if (container->Excess())
				query_law(h, (const TExternLawsuit *) container);
			else
				query_law(h, (const TLawsuit *) container, full);
		}
	}
}

void query_group(TWindow *parent, const TRCDKeyGroup *group, bool full)
{
	THTMLFile h;

	h.OpenTemp();
	TWaitWindow wait(parent, WAIT);
	query_group(h, group, full);
	h.Show();
}

void query_key(TWindow *parent, const TRCDKey &key)
{
	TRCDKeyContainer *const container = can_access_basic(key, false, false, NULL);

	if (container)
	{
		TVirtPtr VP(container);
		query_group(parent, container, true);
	}
}

void query_surrouendoc(const TSurroundment *surrouendoc)
{
	THTMLFile h;

	h.OpenTemp();
	query_surrouendoc(h, surrouendoc, true);
	h.Show();
}

void query_surrouendocs(TWindow *parent, const TSession *session)
{
	THTMLFile h;

	h.OpenTemp();
	TWaitWindow wait(parent, WAIT);
	// some session header here?..
	query_surrouendocs(h, session, false);
	h.Show();
}

void query_show_hide(TWindow *parent, const TRCDKeyGroup *group)
{
	parent->ShowWindow(SW_HIDE);
	query_group(parent, group);
	parent->ShowWindow(SW_SHOW);
}

void query_archive(TWindow *parent, TArchive &archive)
{
	msql m(archive.Name);

	m.Add(&archive, "F_ARCHIVE_NO, F_ARCHIVE_YEAR, F_TYPE");
	TQuery q(str(m));

	if (q.Read())
	{
		archive << q;

		if (archive.excess)
		{
			TExternLawsuit lawsuit;
			lawsuit.key = archive.key;

			if (lawsuit.Get())
				query_show_hide(parent, &lawsuit);
		}
		else
		{
			TRCDKeyGroup *const tGroup = can_access_basic(&archive, false, false, NULL);
			TVirtPtr VP(tGroup);

			if (tGroup)
				query_show_hide(parent, tGroup);
		}
	}
	else
		say_no_data(archive.Name);
}
