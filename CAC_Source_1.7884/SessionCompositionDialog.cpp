//{{TSessionCompositionDialog Implementation}}

TSessionCompositionDialog::TSessionCompositionDialog(TWindow* parent, TBaseSession *session, TLawsuit *tLawsuit, long edit,
	int resId)
:
	TTroubleDialog(parent, session, edit, TYPE_TEXT, resId), lawsuit(tLawsuit)
{
	if (lawsuit)
	{
		Subject->Seek(lawsuit->key.type, lawsuit->subject);
		posts.cat(Subject->Attrib(lawsuit->kind));
		jurors = posts.chr(strchr(KIND_OPEN_SESSIONS, session->kind) ? POST_JUROR : POST_CLOSE_JUROR);

	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		president = PostFace(IDC_SESSION_COMPOSITION_PRESIDENT, "F_PRESIDENT", POST_JUDGE);
		judgeX = PostFace(IDC_SESSION_COMPOSITION_JUDGE_X, "F_JUDGE_X", POST_APPRENTICE);
		reJudgeX = PostFace(IDC_SESSION_COMPOSITION_REJUDGE_X, "F_REJUDGE_X", POST_APPRENTICE);
	#endif  // APPEAL || ADMIN || AREAL || MILITARY
	#if COURT_TYPE == COURT_REGIONAL
		president = NULL;
		judgeX = NULL;
		reJudgeX = NULL;
	#endif  // REGIONAL

		// 2015:090
		if (strchr(KIND_OPEN_SESSIONS, session->kind))
			prosecutor = PostFace(IDC_SESSION_COMPOSITION_PROSECUTOR, "F_PROSECUTOR", POST_PROSECUTOR);
		else
			prosecutor = NULL;
	}
	else if (session->Excess())
	{
		posts.cat(POST_JURORS);
		posts.cach(POST_EXTENDED);
		posts.cach(POST_SECRETAR);	// 2017:090
		jurors = true;
	}

	// 2017:090 judge, secretar common to intern/extern
	judge = PostFace(IDC_SESSION_COMPOSITION_JUDGE, "F_JUDGE", POST_APPRENTICE);
	// 2015:090
	if (strchr(KIND_OPEN_SESSIONS, session->kind))
		secretar = PostFace(IDC_SESSION_COMPOSITION_SECRETAR, "F_SECRETAR", POST_SECRETAR);
	else
		secretar = NULL;

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	bool found = false;

	// 2010:167 jurors moved here from open result
	if (jurors)
	{
		for (int i = 0; i < JUROR_COUNT; i++)
			strcpy(distrib.jurors[i], EMPTY);
		found = distrib.Find(session, false);
		// 2017:150
		if (!found && strchr(KIND_CLOSED_SESSIONS, session->kind))
		{
			if (!edit)
			{
				msql m;

				distrib.FloatCriteria(m);
				m.AddZLong("F_CLOSED", TRUE);
				TQuery q(str(m));

				if (q.Read())
				{
					message(MB_ICONWARNING | MB_OK, "Няма разпределени заседатели за %s от %Dг.",
						Kind->Seek(KIND_CLOSED_SESSION), &session->date);
				}
			}

			TSession tSession;

			tSession << session;
			tSession.kind = *KIND_OPEN_SESSIONS;
			found = distrib.Find(&tSession, false);
		}
	}
#endif  // AREAL || MILITARY || REGIONAL

	// 2009:217 no jurors for AdmC
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN
	juror = NULL;
	reJuror = NULL;
	jurorX = NULL;
	reserve = NULL;
#endif  // APPEAL || ADMIN
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (found)
	{
		// 2017:152 LPR: we don't get last jurors for closed
		if (!edit && strchr(KIND_CLOSED_SESSIONS, session->kind))
			session->ImportFields(&distrib, JUROR_NAMES_UNQUOTED);

		juror = JurorFace(IDC_SESSION_COMPOSITION_JUROR, "F_JUROR", distrib.juror, distrib.reserve, session->juror);
		reJuror = JurorFace(IDC_SESSION_COMPOSITION_REJUROR, "F_REJUROR", distrib.reJuror, distrib.reserve, session->reJuror);
		jurorX = JurorFace(IDC_SESSION_COMPOSITION_JUROR_X, "F_JUROR_X", distrib.jurorX, distrib.reserve, session->jurorX);
		reserve = JurorFace(IDC_SESSION_COMPOSITION_RESERVE, "F_RESERVE", distrib.reserve, NULL, session->reserve);
	}
	else
	{
		juror = PostFace(IDC_SESSION_COMPOSITION_JUROR, "F_JUROR", POST_JUROR);
		reJuror = PostFace(IDC_SESSION_COMPOSITION_REJUROR, "F_REJUROR", POST_JUROR);
		jurorX = PostFace(IDC_SESSION_COMPOSITION_JUROR_X, "F_JUROR_X", POST_JUROR);
		reserve = PostFace(IDC_SESSION_COMPOSITION_RESERVE, "F_RESERVE", POST_JUROR);
	}
#endif  // AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	if (found)
	{
		juror = JurorFace(IDC_SESSION_COMPOSITION_JUROR, "F_JUROR", distrib.juror, distrib.reserve, session->juror);
		reJuror = JurorFace(IDC_SESSION_COMPOSITION_REJUROR, "F_REJUROR", distrib.reJuror, distrib.reserve, session->reJuror);
		jurorX = NULL;
		reserve = JurorFace(IDC_SESSION_COMPOSITION_RESERVE, "F_RESERVE", distrib.reserve, NULL, session->reserve);
	}
	else
	{
		juror = PostFace(IDC_SESSION_COMPOSITION_JUROR, "F_JUROR", POST_JUROR);
		reJuror = PostFace(IDC_SESSION_COMPOSITION_REJUROR, "F_REJUROR", POST_JUROR);
		jurorX = NULL;
		reserve = PostFace(IDC_SESSION_COMPOSITION_RESERVE, "F_RESERVE", POST_JUROR);
	}
#endif  // REGIONAL
	composition = new TLongAliasFace(this, IDC_SESSION_COMPOSITION_COMPOSITION, "F_COMPOSITION", Composition,
		Default->compositions, FLAG_NULL);
}

TPersonAliasFace *TSessionCompositionDialog::PostFace(int resourceId, const char *name, char post)
{
	const char EXTENDED_COMPOSITION_FIELD = 'X';
	const char *include = NULL;
	// N.B. Appeal POST_VISIBLE_JUDGES has no POST_APPRENTICE, but ctor uses it
	bool present = strchr(POST_VISIBLE_MATURES, post) || post == POST_APPRENTICE ||
		(post == POST_JUROR ? jurors : posts.chr(post) != NULL);
	const char *xCrit = "";

	if (present && strchr(name, EXTENDED_COMPOSITION_FIELD) && !posts.chr(POST_EXTENDED))
		present = false;

	if (present)
	{
		switch (post)
		{
			case POST_JUDGE : xCrit = POST_VISIBLE_MATURES; break;
			case POST_APPRENTICE : xCrit = POST_VISIBLE_JUDGES; break;
			case POST_SECRETAR : xCrit = POST_VISIBLE_SECRETARS; break;
			case POST_PROSECUTOR : xCrit = INVOLVEMENT_PROSECUTORS; break;
			case POST_JUROR :
			{
				const char *s = ((TString *) ((TBaseSession *) Group())->Find(name))->S();

				xCrit = INVOLVEMENT_JURORS;
				if (atob(s))
					include = s;

				break;
			}
			default : fatal("%d: post not supported by PostFace", post);
		}
	}

	return strchr(POST_KNOWN_PERSONS, post)
		? (TPersonAliasFace *) new TKnownAliasFace(this, resourceId, name, xCrit,
			edit ? ALIAS_EMPTY : FLAG_ACTIVE | ALIAS_EMPTY, include)
		: (TPersonAliasFace *) new TUserAliasFace(this, resourceId, name, xCrit,
			edit ? ALIAS_EMPTY | USER_INSECT : FLAG_ACTIVE | ALIAS_EMPTY | USER_INSECT);
}

TPersonAliasFace *TSessionCompositionDialog::JurorFace(int resourceId, const char *name, const char *ucn0, const char *ucn1,
	const char *include)
{
	if (!atob(ucn0))	// no JUROR_X, but maybe RESERVE
	{
		ucn0 = ucn1;
		ucn1 = NULL;
	}

	TJurorAliasFace *juror = new TJurorAliasFace(this, resourceId, name, ucn0, ucn1, ALIAS_EMPTY);

	if (edit && atob(include))
		juror->Include(include);

	return juror;
}

bool TSessionCompositionDialog::IsValid()
{
	bool isValid = TTroubleDialog::IsValid();

	if (isValid)
	{
		if (lawsuit)
		{
			TPersonAliasFace *judges[JUDGE_COUNT] = { president, judgeX, reJudgeX };
			static constant judgeNames[JUDGE_COUNT] = { SPERS_PRESIDENT, SPERS_SECOND, SPERS_THIRD };
			TPersonAliasFace *jurors[JUROR_COUNT] = { juror, reJuror, jurorX, reserve };
			static constant jurorNames[JUROR_COUNT] = { SPERS_FIRST, SPERS_SECOND, SPERS_THIRD, SPERS_RESERVE };

			if (equal_ses_person(judges, judgeNames, JUDGE_COUNT, SPERS_JUDGE))
				isValid = false;
			else if (!atob(judge))
				isValid = error("Докладчикът е задължителен.");
			else if (CourtType != COURT_REGIONAL &&
				(strchr(KIND_OPEN_SESSIONS, Session()->kind) || atobc(judges, JUDGE_COUNT)) &&
				!equal_ses_person(judge, judges, JUDGE_COUNT))
			{
				isValid = error("Докладчик трябва да е един от съдиите.");
			}
			else if (equal_ses_person(jurors, jurorNames, JUROR_COUNT, SPERS_JUROR))
				isValid = false;
			else if (reserve && atob(reserve) && atobc(jurors, JUROR_COUNT) == 1)
				isValid = error("Указан е само %s %s.", SPERS_RESERVE, SPERS_JUROR);
		}
	}

	return isValid;
}

void TSessionCompositionDialog::AdjustPresident()
{
	if (judge->IsWindowEnabled() && president && !judgeX->IsWindowEnabled() && !reJudgeX->IsWindowEnabled())
	{
		president->SetS(judge->S());
		president->EnableWindow(false);
	}
}

void TSessionCompositionDialog::SetupWindow()
{
	const TSession *session = (const TSession *) Session();

	if (!edit)
	{
		Composition2Group(Session(), lawsuit->college, session->composition, false,
			strchr(KIND_OPEN_SESSIONS, session->kind));
	}

	TTroubleDialog::SetupWindow();

	if (!edit)
	{
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		// Seek to empty 2nd and 3rd judge for 1st instance punishment laws
		if (strchr(KIND_1ST_PUNISHMENT_LAWS, lawsuit->kind))
		{
			if (judgeX && !any(session->judgeX))
				judgeX->SetS(EMPTY);
			if (reJudgeX && !any(session->reJudgeX))
				reJudgeX->SetS(EMPTY);
		}
#endif  // ADMIN || AREAL || MILITARY
	}

	if (lawsuit->key.type != TYPE_PUNISHMENT_LAW && prosecutor && !any(session->prosecutor))
		prosecutor->SetS(EMPTY);
}
