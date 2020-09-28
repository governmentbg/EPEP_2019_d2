// ----- update_mini ---------------------------------------------------------
#if COURT_TYPE == COURT_AREAL || SPECIAL
struct TReportRangeData
{
	long subject;
	const char *name;
};

static bool insert_report_range(const TReportRangeData *rr, const char *types, const char *format)
{
	strcpy(ReportRange->types, types);
	ReportRange->instances = 1;
	ReportRange->rangeId = rr->subject * SUBJECT_MULTIPLY;

	if (!ReportRange->Try())
	{
		sprintf(ReportRange->subjects, "%ld00", rr->subject);
		strcpy(ReportRange->attrib, C2S[POST_BUILTIN]);
		strcpy(ReportRange->name, rr->name);
		sprintf(ReportRange->alias, format, rr->subject);

		if (ReportRange->Insert())
			return true;
	}

	return false;
}
#endif  // AREAL || SPECIAL

void update_mini(TWindow *parent)
{
	mstr m;

	{
		TSubpoenaKind sk;
		sk.subpoenaKind = '‘';

		if (sk.Try("F_NAME, F_ATTRIBUTES") && !strchr(sk.attributes, SK_ATTR_INDIRECT_EVENT))
		{
			sk.attributes[14] = SK_ATTR_INDIRECT_EVENT;

			if (sk.Update("F_ATTRIBUTES"))
			{
				m.sep("\n\n");
				m.printf("Променени са атрибутите на %s.", sk.name);
			}
		}
	}

	{
		struct TEISDictData
		{
			long nom;
			long code;
			char name[SIZE_OF_EISPP_NAME];
			char tlacr[SIZE_OF_ALIAS];
			char alias[SIZE_OF_EISPP_ALIAS];
		};

		// 2016:221 TRQ/FIX: Kxx -> akia only
		static const TEISDictData EISDictCompat[] =
		{
			{ 1101, 1900000054, "Сектор \"Оперативно-издирвателна дейност\" - РДГП - Кюстендил", "АКЯ", "str_akia" },
			{ 1097, 1900001779, "Следствен арест - Аспарухово", "ИГУ", "str_ar_asparyh" },
			{ 1097, 1900001778, "Следствен арест - Девня", "ИГА", "str_ar_devnya" },
			{ 1097, 1900001780, "Следствен арест - Димитровград", "ИГФ", "str_ar_dimitr" },
			{ 1097, 1900001783, "Следствен арест - Златоград", "ИГЧ", "str_ar_zltgr" },
			{ 1097, 1900001782, "Следствен арест - Луковит", "ИГЦ", "str_ar_lukov" },
			{ 1097, 1900001784, "Следствен арест - Мадан", "ИГШ", "str_ar_mdn" },
			{ 1097, 1900001781, "Следствен арест - Пещера", "ИГФ", "str_ar_pesht" },
			{ 1097, 1900001785, "Следствен арест - Чепеларе", "ИГЮ", "str_ar_chplr" },
			{ 0, 0, "", "", "" }
		};

		TEisDepart eisDep;
		int count = 0;

		for (const TEISDictData *edd = EISDictCompat; edd->nom; edd++)
		{
			eisDep.nom = edd->nom;
			eisDep.code = edd->code;

			if (!eisDep.Try("F_CODE"))
			{
				strcpy(eisDep.name, edd->name);
				strcpy(eisDep.tlacr, edd->tlacr);
				eisDep.flags = FLAG_ACTIVE;
				strcpy(eisDep.alias, edd->alias);

				if (eisDep.Insert())
					count++;
			}
		}

		if (count)
		{
			m.sep("\n\n");
			m.printf("Към ЕИСПП са добавени %d нови структури.", count);
		}
	}

#if COURT_TYPE == COURT_REGIONAL
	{
		Subject->Seek(TYPE_CITIZEN_LAW, 80000);

		if (Subject->flagsX & SUBJECT_X_HIDDEN)
		{
			Subject->flagsX &= ~SUBJECT_X_HIDDEN;

			if (Subject->Update("F_FLAGS_X"))
			{
				m.sep("\n\n");
				m.cat("Предмет 800 за граждански дела е активиран.");
			}
		}
	}
#endif  // REGIONAL

#if COURT_TYPE == COURT_AREAL
	{
		Subject->subject = 2211000;
		Subject->type = 'c';

		if (!Subject->Try())
		{
			if (Subject->DataBase->Execute("INSERT INTO T_SUBJECT (F_SUBJECT, F_TYPE, F_KIND_I, F_KIND_II, F_NAME)"
				" VALUES (2211000, 'c', 'Z',  NULL, 'Искове по Закона за авторското право и сродните му права')",
				ET_ERROR))
			{
				Subject->DataBase->Execute("UPDATE T_SUBJECT SET"
					" F_FLAGS_I = (SELECT F_FLAGS FROM T_OLD_SUBJECT WHERE F_SUBJECT = 800 AND F_TYPE = 'c'),"
					" F_ATTRIB_I = (SELECT F_ATTRIB FROM T_OLD_SUBJECT WHERE F_SUBJECT = 800 AND F_TYPE = 'c'),"
					" F_RESULTS_I = (SELECT F_RESULTS FROM T_OLD_SUBJECT WHERE F_SUBJECT = 800 AND F_TYPE = 'c'),"
					" F_CLOSEDS_I = (SELECT F_CLOSEDS FROM T_OLD_SUBJECT WHERE F_SUBJECT = 800 AND F_TYPE = 'c'),"
					" F_PENALITIES_I = (SELECT F_PENALITIES FROM T_OLD_SUBJECT WHERE F_SUBJECT = 800 AND F_TYPE = 'c'),"
					" F_REASONS_I = (SELECT F_REASONS FROM T_OLD_SUBJECT WHERE F_SUBJECT = 800 AND F_TYPE = 'c'),"
					" F_JUDGE_REPORT_I = (SELECT F_JUDGE_REPORT FROM T_OLD_SUBJECT WHERE F_SUBJECT = 800 AND F_TYPE = 'c'),"
					" F_QUICK_REPORT_I = (SELECT F_QUICK_REPORT FROM T_OLD_SUBJECT WHERE F_SUBJECT = 800 AND F_TYPE = 'c'),"
					" F_VID_I = (SELECT F_VID FROM T_OLD_SUBJECT WHERE F_SUBJECT = 800 AND F_TYPE = 'c'),"
					" F_SBES_I = (SELECT F_SBES FROM T_OLD_SUBJECT WHERE F_SUBJECT = 800 AND F_TYPE = 'c')"
					" WHERE F_SUBJECT = 2211000 AND F_TYPE = 'c'", ET_ERROR);
				Subject->DataBase->Execute("INSERT INTO T_ARCHIVE_SUBJECT (F_ARCHIVE_INDEX, F_SUBJECT, F_KIND)"
					" SELECT F_ARCHIVE_INDEX, 2211000, 'Z' FROM T_OLD_ARCHIVE_SUBJECT WHERE F_SUBJECT = 800 AND F_TYPE = 'c'",
					ET_ERROR);
				m.sep("\n\n");
				m.cat("Добавен е предмет 22110-1 за Граждански дела.");
			}
		}
	}
#endif  // AREAL

#if COURT_TYPE == COURT_ADMIN
	{
		Subject->subject = 450050;
		Subject->type = 'a';

		if (!Subject->Try())
		{
			if (Subject->DataBase->Execute("INSERT INTO T_SUBJECT (F_SUBJECT, F_TYPE, F_KIND_I, F_KIND_II, F_NAME)"
				" VALUES (450050, 'a', NULL, 'E',  'чл. 281 от ЗИНЗС')", ET_ERROR))
			{
				Subject->DataBase->Execute("UPDATE T_SUBJECT SET"
					" F_FLAGS_II = (SELECT F_FLAGS FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 'a'),"
					" F_ATTRIB_II = (SELECT F_ATTRIB FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 'a'),"
					" F_RESULTS_II = (SELECT F_RESULTS FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 'a'),"
					" F_CLOSEDS_II = (SELECT F_CLOSEDS FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 'a'),"
					" F_PENALITIES_II = (SELECT F_PENALITIES FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 'a'),"
					" F_REASONS_II = (SELECT F_REASONS FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 'a'),"
					" F_JUDGE_REPORT_II = (SELECT F_JUDGE_REPORT FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 'a'),"
					" F_QUICK_REPORT_II = (SELECT F_QUICK_REPORT FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 'a'),"
					" F_VID_II = (SELECT F_VID FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 'a'),"
					" F_SBES_II = (SELECT F_SBES FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 'a')"
					" WHERE F_SUBJECT = 450050 AND F_TYPE = 'a'", ET_ERROR);

				Subject->DataBase->Execute("INSERT INTO T_ARCHIVE_SUBJECT (F_ARCHIVE_INDEX, F_SUBJECT, F_KIND)"
					" SELECT F_ARCHIVE_INDEX, 450050, 'E' FROM T_OLD_ARCHIVE_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 'a'",
					ET_ERROR);

				m.sep("\n\n");
				m.cat("Добавен е предмет 4500 за Административни дела.");
			}
		}

		Subject->type = 't';

		if (!Subject->Try())
		{
			if (Subject->DataBase->Execute("INSERT INTO T_SUBJECT (F_SUBJECT, F_TYPE, F_KIND_I, F_KIND_II, F_NAME) "
				" VALUES (450050, 't', NULL, 'z',  'чл. 281 от ЗИНЗС')", ET_ERROR))
			{
				Subject->DataBase->Execute("UPDATE T_SUBJECT SET"
					" F_FLAGS_II = (SELECT F_FLAGS FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 't'),"
					" F_ATTRIB_II = (SELECT F_ATTRIB FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 't'),"
					" F_RESULTS_II = (SELECT F_RESULTS FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 't'),"
					" F_CLOSEDS_II = (SELECT F_CLOSEDS FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 't'),"
					" F_PENALITIES_II = (SELECT F_PENALITIES FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 't'),"
					" F_REASONS_II = (SELECT F_REASONS FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 't'),"
					" F_JUDGE_REPORT_II = (SELECT F_JUDGE_REPORT FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 't'),"
					" F_QUICK_REPORT_II = (SELECT F_QUICK_REPORT FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 't'),"
					" F_VID_II = (SELECT F_VID FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 't'),"
					" F_SBES_II = (SELECT F_SBES FROM T_OLD_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 't')"
					" WHERE F_SUBJECT = 450050 AND F_TYPE = 't'", ET_ERROR);

				Subject->DataBase->Execute("INSERT INTO T_ARCHIVE_SUBJECT (F_ARCHIVE_INDEX, F_SUBJECT, F_KIND) "
					"SELECT F_ARCHIVE_INDEX, 450050, 'z' FROM T_OLD_ARCHIVE_SUBJECT WHERE F_SUBJECT = 3100 AND F_TYPE = 't'",
					ET_ERROR);

				m.sep("\n\n");
				m.cat("Добавен е предмет 4500 за Наказателни дела.");
			}
		}
	}
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{
		msql w(SubpoenaKind->Name);
		int count = 0;

		w.AddLike("(F_NAME", "Увед", false);
		w.NextOr();
		w.AddChar("F_PROC", SK_PROC_NOTIFYING);
		w.NextOr();
		w.AddChars("F_SUBPOENA_KIND", "†р");	// 2017:341
		w.NextOr();
		w.AddString("F_TEMPLATE", "Anp_bs.htm");	// 2017:341
		w.cat(")");
		w.AddLike("F_KINDS", C2S[KIND_COURTPUN_SESSION], true);
		w.AddLike("NOT F_KINDS", C2S[KIND_PREORDER_SESSION], true);

		TQuery q(str(w));
		while (q.Read())
		{
			*SubpoenaKind << q;
			strcat(SubpoenaKind->kinds, C2S[KIND_PREORDER_SESSION]);

			if (SubpoenaKind->Update("F_KINDS"))
				count++;
		}

		if (count > 0)
		{
			m.sep("\n\n");
			m.printf("Към %s са добавени %d вида уведомявания.", Kind->Seek(KIND_PREORDER_SESSION), count);
		}
	}

	{
		enum { zasegnato = 'щ' };

		Kind->Seek('D');

		if (!strchr(Kind->others, zasegnato))
		{
			strcach(Kind->others, zasegnato);

			if (Kind->Update("F_OTHERS"))
			{
				m.sep("\n\n");
				m.printf("Към участията по %s е добавено %s.", Kind->name, Involvement->Seek(zasegnato));
			}
		}
	}


	{
		int count = 0;

		SubpoenaKind->subpoenaKind = '<';

		if (!SubpoenaKind->Try("F_NAME") && SubpoenaKind->DataBase->Execute("INSERT INTO T_SUBPOENA_KIND VALUES ('<',"
			"'Съобщение по чл.247б НПК за съдебен заседател', 'Съобщ. чл.247б', '”', 'f', 'я', 'O',"
			" 'anpmslP.L...t..G....', 'a', 1070, 'Smp1fs_r3.htm', 'M',  NULL, NULL, NULL)", ET_ERROR))
		{
			count++;
		}

		SubpoenaKind->subpoenaKind = '>';

		if (!SubpoenaKind->Try("F_NAME"))
		{
			mstr w;

			w.printf("INSERT INTO T_SUBPOENA_KIND VALUES ('>', 'Призовка за съдебен заседател',"
				" 'Приз. заседател', '%c%c', 'f', 'я', 'O', 'anpmslP.L...t..G....', 'a', 1075,"
				" 'Smp1fsj.htm', 'S',  NULL, NULL, NULL)", KIND_COURTPUN_SESSION, KIND_OTHER_SESSION);

			if (SubpoenaKind->DataBase->Execute(str(w), ET_ERROR))
				count++;
		}

		if (count > 0)
		{
			m.sep("\n\n");
			m.printf("Добавени са %d вида призовки/съобщения за съдебен заседател.", count);
		}
	}
#endif  // AREAL || MILITARY || SPECIAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{
		enum { nmk_dlosig = 204, sign_vnaxd = 1900001685 };
		static constant sign_vnaxd_str = "1900001685";

		EisVal->nom = nmk_dlosig;
		EisVal->code = sign_vnaxd;

		if (!EisVal->Try())
		{
			strcpy(EisVal->name, "ВНАХД");
			EisVal->flags = FLAG_ACTIVE;
			strcpy(EisVal->alias, "sign_vnaxd");

			if (EisVal->Insert())
			{
				m.sep("\n\n");
				m.printf("Към ЕИСПП е добавен код %d.", sign_vnaxd);
			}
		}

		if (EisVal->Try())
		{
		#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
			Kind->Seek(KIND_2ND_PUNISHMENT_VNAHD_LAW);

			if (!Kind->eispp)
			{
				Kind->eispp = sign_vnaxd;

				if (Kind->Update("F_EISPP"))
				{
					m.sep("\n\n");
					m.printf("ЕИСПП код %d е добавен към %s.", sign_vnaxd, Kind->name);
				}
			}
		#endif  // APPEAL || AREAL || MILITARY

			msql w(SbeDesc->Name);
			TIntegerArray vnaxd_sbes;

			vnaxd_sbes.Add(947, 1640, 943, 945, 953, 954, 958, 863, 859, 871, 870, 888, 890, 913, 926,
				915, 15347, 15348, 904, 910, INT_MAX);
			w.AddLongs("F_VID", vnaxd_sbes);
			w.AddString("F_NAME", "NPR.DLO.dlosig");
			w.AddLike("NOT F_VALUE", sign_vnaxd_str, true);

			TQuery q(str(w));
			w.clear();

			while (q.Read())
			{
				*SbeDesc << q;
				strcat(SbeDesc->value, ", ");
				strcat(SbeDesc->value, sign_vnaxd_str);

				if (SbeDesc->Update("F_VALUE"))
				{
					w.sep(", ");
					w.printf("%ld", SbeDesc->vid);
				}
			}

			if (any(w))
			{
				m.sep("\n\n");
				m.printf("ЕИСПП код %d е добавен към събития %s.", sign_vnaxd, str(w));
			}
		}
	}

	{
		enum { nmk_dvjprc = 218, ipz_za_uskoriavane_proizvodstvo = 1900001684 };
		static constant ipz_za_uskoriavane_proizvodstvo_str = "1900001684";

		EisVal->nom = nmk_dvjprc;
		EisVal->code = ipz_za_uskoriavane_proizvodstvo;

		if (!EisVal->Try())
		{
			strcpy(EisVal->name, "искане за ускоряване на производството(чл.368а,ал.1НПК)");
			EisVal->flags = FLAG_ACTIVE;
			strcpy(EisVal->alias, "ipz_za_uskoriavane_proizvodstvo");

			if (EisVal->Insert())
			{
				m.sep("\n\n");
				m.printf("Към ЕИСПП е добавен код %d.", ipz_za_uskoriavane_proizvodstvo);
			}
		}

		if (EisVal->Try())
		{
			SbeDesc->vid = 859;
			strcpy(SbeDesc->name, "DVJDLO.dvjprc");

			if (SbeDesc->Try() && !strstr(SbeDesc->value, ipz_za_uskoriavane_proizvodstvo_str))
			{
				strcat(SbeDesc->value, ", ");
				strcat(SbeDesc->value, ipz_za_uskoriavane_proizvodstvo_str);

				if (SbeDesc->Update("F_VALUE"))
				{
					m.sep("\n\n");
					m.printf("ЕИСПП код %d е добавен към събитие %d.", ipz_za_uskoriavane_proizvodstvo, SbeDesc->vid);
				}
			}
		}
	}

	{
		struct TEISValData
		{
			long nom;
			long code;
			char name[SIZE_OF_EISPP_NAME];
			char alias[SIZE_OF_EISPP_NAME];
		};

		static const TEISValData EISValDataNew[] =
		{
			{ 240, 1900001678, "деянието, описано в обвинителния акт или в тъжбата, съставлява административно нарушение(чл.250,ал.1,т.2 от НПК)", "osn_adm_nar_sad" },
			{ 240, 1900001674, "извършеното деяние съставлява административно нарушение, за което е приключило АНП(чл.24,ал.1, т.8а от НПК)", "osn_adm_narushenie" },
			{ 240, 1900001677, "липсва тъжба (чл.24,ал.5,т.1 от НПК)", "osn_lipsva_tujba_postradal" },
			{ 240, 1900001675, "не е направено предложение за възобновяване на АНП(чл.24,ал.4,пр.1, вр.ал.1,т.8а от НПК)", "osn_lipsva_vazobn_anp" },
			{ 240, 1900001676, "оставено без уважение предложение за възобновяване на АНП(чл.24,ал.4,пр.2 вр.ал.1,т.8а от НПК)", "osn_neuvaj_vazobn_anp" },
			{ 242, 1900001680, "за същото деяние, което е престъпление, е приключило АНП(чл.25,ал.1, т.5 от НПК)", "osn_prikl_anp" },
			{ 242, 1900001681, "отправена молба за международна правна помощ, по която се чака отговор(чл.25, ал.2 от НПК)", "osn_molba_pravna_pomosht" },
			{ 242, 1900001679, "отправено преюдициално запитване до Съда на ЕС(чл.25, ал.1, т.4 от НПК)", "osn_zapitvane_SES" },
			{ 0, 0, "", "" }
		};

		int count = 0;

		for (const TEISValData *eva = EISValDataNew; eva->nom; eva++)
		{
			EisVal->nom = eva->nom;
			EisVal->code = eva->code;

			if (!EisVal->Try("F_CODE"))
			{
				strcpy(EisVal->name, eva->name);
				EisVal->flags = FLAG_ACTIVE;
				strcpy(EisVal->alias, eva->alias);

				if (EisVal->Insert())
					count++;
			}
		}

		if (count)
		{
			m.sep("\n\n");
			m.printf("Към ЕИСПП са добавени %d нови основания за спиране/прекратяване.", count);
		}
	}

	{
		EisVal->nom = 253;
		EisVal->code = 1900001682;

		if (!EisVal->Try("F_CODE"))
		{
			strcpy(EisVal->name, "по тъжба на пострадалия от престъплението");
			EisVal->flags = FLAG_ACTIVE;
			strcpy(EisVal->alias, "ncn_po_tajba");

			if (EisVal->Insert())
			{
				m.sep("\n\n");
				m.printf("Към ЕИСПП е добавен код %ld.", EisVal->code);
			}
		}

		if (EisVal->Try())
		{
			int count = EisVal->DataBase->ExecCount("UPDATE T_SBE_DESC SET F_VALUE = '1900001682, ' || F_VALUE "
				"WHERE F_NAME = 'NPR.DLO.dloncnone' AND NOT F_VALUE IS NULL "
				"AND NOT F_VALUE LIKE '%1900001682%'", ET_ERROR);

			if (count > 0)
			{
				m.sep("\n\n");
				m.printf("ЕИСПП код %ld е добавен към %d събития.", EisVal->code, count);
			}
		}
	}

	{	// 2018-01-16
		enum { nmk_ptvnkzvid = 256, ptv_otkaz_pred_osv = 1900001686 };

		EisVal->nom = nmk_ptvnkzvid;
		EisVal->code = ptv_otkaz_pred_osv;

		if (!EisVal->Try())
		{
			strcpy(EisVal->name, "Потвърждава отказа от предсрочно освобождаване");
			EisVal->flags = FLAG_ACTIVE;
			strcpy(EisVal->alias, "ptv_otkaz_pred_osv");

			if (EisVal->Insert())
			{
				m.sep("\n\n");
				m.printf("Към ЕИСПП е добавен код %d.", ptv_otkaz_pred_osv);
			}
		}
	}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{
		Result->result = RESULT_UN_STOPPED;

		if (!Result->Try())
		{
			strcpy(Result->name, "Възобновяване на производството");
			Result->OrderField()->SetL(9993);
			Result->xfer = 11002;

			if (Result->Insert())
			{
				m.sep("\n\n");
				m.printf("Добавен е резултат %s.", Result->name);
			}
		}
	}

	{
		Result->result = RESULT_UN_STOPPED;

		if (Result->Try())
		{
			int count = 0;

			for (int i = 0; i < 2; i++)
			{
				constant CLOSEDS = Subject->FNI("F_CLOSEDS");
				mstr NOT_CLOSEDS("NOT ");
				msql w(Subject->Name, MQ_UPDATE);

				w.printf("%s = '%c' || %s", CLOSEDS, RESULT_UN_STOPPED, CLOSEDS);
				w.AddIsNull(CLOSEDS, false);
				NOT_CLOSEDS.cat(CLOSEDS);
				w.AddLike(str(NOT_CLOSEDS), C2S[RESULT_UN_STOPPED], true);

				int updated = Subject->DataBase->ExecCount(str(w), ET_ERROR);

				if (updated > 0)
					count += updated;
			}

			if (count)
			{
				m.sep("\n\n");
				m.printf("Резултат %s е добавен към %d предмета.", Result->name, count);
			}
		}
	}
#endif  // ADMIN || AREAL || MILITARY || REGIONAL

	{
		int count = 0;

	#if COURT_TYPE == COURT_AREAL || SPECIAL
		static const TReportRangeData rangeCrimes[] =
		{
			{ 712, "Длъжностно присвояване – чл. 201 НК (само за СпНС)" },
			{ 713, "Длъжностно присвояване, за улесняването на което е извършено и друго престъпление, не по-тежко наказуемо – чл. 201 НК (само за СпНС)" },
			{ 714, "Длъжностно присвояване в големи размери – чл. 202, ал. 1, т. 1 и т. 2 НК (само за СпНС)" },
			{ 716, "Маловажни случаи на длъжностно присвояване – чл. 204 НК (само за СпНС) – чл. 205 НК (само за СпНС)" },
			{ 726, "Документна измама – чл. 212, ал. 1 и ал. 2 НК (само за СпНС)" },
			{ 727, "Документна измама, при която полученото без правно основание имущество е от фондове на Европейския съюз или предоставено от тях на българската държава – чл. 212, ал. 3 НК (само за СпНС)" },
			{ 728, "Документна измама в големи размери или представляваща опасен рецидив – чл. 212, ал. 4 НК (само за СпНС)" },
			{ 730, "Измама по чл.212а НК (само за СпНС)" },
			{ 921, "Престъпление по служба - чл.285 НК (само за СпНС)" },
			{ 0, NULL }
		};

		for (const TReportRangeData *rr = rangeCrimes; rr->subject; rr++)
			if (insert_report_range(rr, "p", "%04ld"))
				count++;
	#endif  // AREAL || SPECIAL

	#if COURT_TYPE == COURT_AREAL
		static const TReportRangeData rangeOthers[] =
		{
			{ 21110, "\\bв.т.ч.\\c Молба за откриване на производство по несъстоятелност" },
			{ 21120, "Искове за прогласяване /обявяване на нищожност/ недействителност на действия и сделки по отношение на кредиторите на несъстоятелността" },
			{ 21130, "Иск за установяване съществуване на неприето или несъществуване на прието вземане, възражения по ЗБН" },
			{ 24100, "\\bв.т.ч.\\c Молба за откриване на производство по стабилизация" },
			{ 24200, "Иск за унищожаване на плана за стабилизация" },
			{ 0, NULL }
		};

		for (const TReportRangeData *rr = rangeOthers; rr->subject; rr++)
			if (insert_report_range(rr, "ctad", "%ld"))
				count++;
	#endif  // AREAL

		if (count)
		{
			m.sep("\n\n");
			m.printf("Добавени са %d отчетни обхвата.", count);
		}
	}

	{
		SubpoenaKind->subpoenaKind = '=';

		if (!SubpoenaKind->Try("F_NAME") && SubpoenaKind->DataBase->Execute("INSERT INTO T_SUBPOENA_KIND VALUES ('=',"
			"'Съобщение – с разпореждане/определение', 'Съобщ.разп/опр.', 'Й', 'ђы', NULL, 'ки',"
			" 'anpmslPML...t..G....', 'a', 65, 'aplan_req.htm', 'N',  NULL, '{F_EVENT_TEXT}', NULL)", ET_ERROR))
		{
			m.sep("\n\n"); //Съобщение – с разпореждане/с определение
			m.printf("Добавено е %s.", SubpoenaKind->Seek('='));
		}
	}

	// 2018-07-02
	{
		long count = AppName->ExecCount("UPDATE T_SUBPOENA_KIND "
			"SET F_ADDITIONAL_TEXT = '{F_LAWSUIT_TEXT}\v' || F_ADDITIONAL_TEXT "
			"WHERE (F_NAME LIKE '%стар ГПК%' OR F_INFO LIKE '%стар ГПК%') "
			"AND NOT F_ADDITIONAL_TEXT LIKE '{F_LAWSUIT_TEXT}%'",
			ET_UPDATE);

		if (count)
		{
			m.sep("\n\n");
			m.printf("Към %ld призовки/съобщения - стар ГПК е добавен текста по делото.", count);
		}
	}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	// 2018-07-14
	{
		msql w(SubpoenaKind->Name, "F_SUBPOENA_KIND, F_KINDS");
		int count = 0;
		mstr execs(SUBPOENA_KIND_EXEC_LISTS);

		execs.cat(SUBPOENA_KIND_EXECLETS);
		w.AddChars("F_SUBPOENA_KIND", str(execs));
		TQuery q(str(w));

		while (q.Read())
		{
			*SubpoenaKind << q;

			if (!strchr(SubpoenaKind->kinds, KIND_PREORDER_SESSION))
			{
				strcach(SubpoenaKind->kinds, KIND_PREORDER_SESSION);

				if (SubpoenaKind->Update("F_KINDS"))
					count++;
			}
		}

		if (count > 0)
		{
			m.sep("\n\n");
			m.printf("Към %d изпълнителни листи/писма е добавено %s.", count, Kind->Seek(KIND_PREORDER_SESSION));
		}
	}
#endif  // AREAL || MILITARY || REGIONAL

	{
		struct TEISCrimeData
		{
			long code;
			char name[SIZE_OF_EISPP_NAME];
			char alias[SIZE_OF_EISPP_ALIAS];
		};

		// 2018-07-23
		static const TEISCrimeData EISCrimeDataNew[] =
		{
			{ 1900000055, "чл.168а,ал.1, Отпечатване на бюлетини за гласуване в нарушение на установените правила", "mppne02457"},
			{ 1900000056, "чл.168а,ал.2, Противозаконно държане/разпространение на бюлетини за гласуване", "mppne02458"},
			{ 1900001642, "чл.159а,ал.1,пр.1, Трафик на лица/групи от хора, с цел разврат", "mppne024507"},
			{ 1900001643, "чл.159а,ал.1,пр.2.1, Трафик на лица/групи от хора, с цел принудителен труд", "mppne024508"},
			{ 1900001644, "чл.159а,ал.1,пр.2.2, Трафик на лица/групи от хора, с цел просия", "mppne024509"},
			{ 1900001645, "чл.159а,ал.1,пр.3, Трафик на лица/групи от хора, с цел отнемане на телесен орган/тъкан/клетка/телесна течност", "mppne024510"},
			{ 1900001646, "чл.159а,ал.1,пр.4, Трафик на лица/групи от хора, с цел държане в принудително подчинение", "mppne024511"},
			{ 1900001647, "чл.159а,ал.2,т.7, Трафик, извършен от длъжностно лице при/по повод изпълнение на службата му", "mppne024512"},
			{ 1900001648, "чл.255б,ал.1, Укриване задължителни осигурителни вноски, в големи размери", "mppne024513"},
			{ 1900001649, "чл.255б,ал.1,т.1, Обявяване осигурителен доход в по-малък размер от действителния за осигуреното лице", "mppne024514"},
			{ 1900001650, "чл.255б,ал.1,т.2, Неподаване декларация за задължителни осигурителни вноски", "mppne024515"},
			{ 1900001651, "чл.255б,ал.1,т.3, Потвърждаване неистина/затаяване истина в подадена декларация за задължителни осигурителни вноски", "mppne024516"},
			{ 1900001652, "чл.255б,ал.1,т.4, Съставяне или използване документ с невярно съдържание, неистински или преправен при упражняване на стоп.дейност/водене счетоводство/представяне информация пред органите по приходите", "mppne024517"},
			{ 1900001653, "чл.255б,ал.1,т.5, Унищожаване или укриване в законоустановените срокове счетоводни документи/регистри/ведомости за заплати", "mppne024518"},
			{ 1900001654, "чл.255б,ал.2, Укриване задължителни осигурителни вноски, с участието на орган по приходите или регистриран одитор", "mppne024519"},
			{ 1900001655, "чл.255б,ал.3, Укриване задължителни осигурителни вноски, в особено големи размери", "mppne024520"},
			{ 1900001656, "чл.255б,ал.4, Внасяне в бюджета,вкл.лихвите за задължителни осигурителни вноски до приключване на съд.следствие в първоинстанционен съд", "mppne024521"},
			{ 1900001687, "чл.142а,ал.3,т.2, Противозаконно лиш.свобода на лице,ползващо се с международна защита", "mppne02587"},
			{ 1900001688, "чл.171,ал.4, Противозаконно осъществяване достъп до/узнаване на комп.данни,трансферирани между една/повече инф.системи", "mppne02585"},
			{ 1900001689, "чл.171,ал.5, Противозаконно осъществяване достъп до/узнаване чуждо съобщение/комп.данни,др.,с користна цел/прич.знач.вреди", "mppne02586"},
			{ 1900001690, "чл.195,ал.1,т.11, Кражба с цел набавяне средства за извършване престъпление по чл.108а,ал.1,2,6 или 7", "mppne02584"},
			{ 1900001692, "чл.240а,ал.1, Незаконен добив подземни богатства", "mppne02507"},
			{ 1900001693, "чл.240а,ал.2, Държане,съхраняване,транспортиране, преработване/отчуждаване незаконно добити подземни богатства", "mppne02508"},
			{ 1900001694, "чл.240а,ал.3,т.1, Незаконен добив подземни богатства,в големи размери", "mppne02509"},
			{ 1900001695, "чл.240а,ал.3,т.2, Незаконен добив подземни богатства от защитена територия/зона", "mppne02510"},
			{ 1900001696, "чл.240а,ал.3,т.3, Незаконен добив подземни богатства с техн. средство/ППС", "mppne02511"},
			{ 1900001697, "чл.240а,ал.3,т.4, Незаконен добив подземни богатства чрез използване неистински/преправен/с невярно съдържание документ", "mppne02512"},
			{ 1900001698, "чл.240а,ал.3,т.5, Незаконен добив подземни богатства от две/повече лица с предварителен сговор", "mppne02513"},
			{ 1900001699, "чл.240а,ал.3,т.6, Незаконен добив подземни богатства с участие на длъжностно лице", "mppne02514"},
			{ 1900001700, "чл.240а,ал.3,т.7, Незаконен добив подземни богатства от лице,системно", "mppne02515"},
			{ 1900001701, "чл.240а,ал.3,т.8, Незаконен добив подземни богатства,повторно", "mppne02516"},
			{ 1900001702, "чл.240а,ал.4,т.1, Незаконен добив подземни богатства,особено големи размери,особено тежък случай", "mppne02517"},
			{ 1900001703, "чл.240а,ал.4,т.2, Незаконен добив подземни богатства по поръчение/в изпълнение решение ОПГ", "mppne02518"},
			{ 1900001704, "чл.240а,ал.4,т.3, Незаконен добив подземни богатства,опасен рецидив", "mppne02519"},
			{ 1900001705, "чл.240а,ал.5, Незаконен добив подземни богатства,маловажен случай", "mppne02520"},
			{ 1900001706, "чл.240а,ал.8, Изготвяне/придобиване/пазене/държане оръдия,др.за добив на подземни богатства без концесия", "mppne02521"},
			{ 1900001708, "чл.260а,ал.1,т.1, Злоупотреба с вътр.информация от член на орган на емитент, др.", "mppne02522"},
			{ 1900001709, "чл.260а,ал.1,т.2, Злоупотреба с вътр.информация от лице с дялово участие в капитала на емитент, др.", "mppne02523"},
			{ 1900001710, "чл.260а,ал.1,т.3, Злоупотреба с вътр.информация от лице с достъп до нея", "mppne02524"},
			{ 1900001711, "чл.260а,ал.2, Злоупотреба с вътр.информация,придобита извън случаите по ал.1", "mppne02525"},
			{ 1900001712, "чл.260а,ал.3, Злоупотреба с вътр.информация с отмяна/изменяне финан.инструмент,значителни вредни последици", "mppne02526"},
			{ 1900001713, "чл.260а,ал.4, Злоупотреба с вътр.информация с отмяна/изменяне финан.инструмент по тръжни процедури съгласно ПЕС,знач.вредни последици", "mppne02527"},
			{ 1900001714, "чл.260а,ал.5, Подбуждане към злоупотреба с вътр.информация", "mppne02528"},
			{ 1900001715, "чл.260а,ал.6, Злоупотреба с вътр.информация от склонено за това лице", "mppne02529"},
			{ 1900001716, "чл.260а,ал.7,т.1, Злоупотреба с вътр.информация от две/повече лица с предварителен сговор", "mppne02530"},
			{ 1900001717, "чл.260а,ал.7,т.2, Злоупотреба с вътр.информация по поръчение/изпълнение решение ОПГ", "mppne02531"},
			{ 1900001718, "чл.260а,ал.7,т.3, Злоупотреба с вътр.информация,повторно", "mppne02532"},
			{ 1900001719, "чл.260б,ал.1, Противозаконно разкриване вътр.информация, относно финан.инструмент", "mppne02533"},
			{ 1900001720, "чл.260б,ал.2, Разгласяване на склоняване към разкриване вътр.информация,относно финан.инструмент,знач.вредни последици", "mppne02534"},
			{ 1900001721, "чл.260в,ал.1, Манипулиране финан.пазар,знач.вредни последици", "mppne02535"},
			{ 1900001722, "чл.260в,ал.2, Манипулиране финан.пазар чрез използване фиктивно средство/въвеждане в заблуждение", "mppne02536"},
			{ 1900001723, "чл.260в,ал.3, Манипулиране финан.пазар чрез медии,др.,знач.вредни последици", "mppne02537"},
			{ 1900001724, "чл.260в,ал.4, Манипулиране изчисляването на бенчмарк,знач.вредни последици", "mppne02538"},
			{ 1900001725, "чл.309,ал.3, Подправка на частен документ, права върху акции,др.", "mppne02582"},
			{ 1900001726, "чл.309,ал.4, Подправка на частен документ,права върху акции,др.,особено големи размери", "mppne02583"},
			{ 1900001727, "чл.319б,ал.4, Неправомерна манипулация комп.данни чрез комп.програми,пароли,кодове,засягаща поведе от една инф.система,тежки последици", "mppne02539"},
			{ 1900001728, "чл.319б,ал.5,т.1, Неправомерна манипулация комп.данни/спиране достъпа до данни в инф.система,по поръчение/в изпълнение решение ОПГ", "mppne02540"},
			{ 1900001729, "чл.319б,ал.5,т.2, Неправомерна манипулация комп.данни/спиране достъпа до данни в инф.система,която е част от критична инфраструктура", "mppne02541"},
			{ 1900001730, "чл.319д,ал.3, Разпространяване комп.програми,пароли,кодове за достъп до инф.система/част от нея,с користна цел,значителни вреди", "mppne02542"},
			{ 1900001731, "чл.336а,ал.1,т.1,чл.336а,ал.5, Нарушаване правила за използване ООС,ядрено съоръжение във въздухоплавателно средство/кораб/платформа,опасност за живота,др.", "mppne02543"},
			{ 1900001732, "чл.336а,ал.1,т.2,чл.336а,ал.5, Нарушаване правила за съхранение на ООС, ядрено съоръжение във въздухоплавателно средство/кораб/платформа,опасност за живота,др.", "mppne02544"},
			{ 1900001733, "чл.336а,ал.1,т.3, чл.336а,ал.5, Нарушаване правила за съхранение на опасно/вредно вещество,извън случаите по т.2", "mppne02545"},
			{ 1900001734, "чл.336а,ал.1,т.4, чл.336а,ал.5, Нарушение правила за използване въздухоплавателно средство/кораб,опасност за живота,др.", "mppne02546"},
			{ 1900001735, "чл.336а,ал.2, чл.336а,ал.5, Противозаконно превозване общоопасно вещество,др.,във въздухоплавателно средство/кораб", "mppne02547"},
			{ 1900001736, "чл.336а,ал.3, чл.336а,ал.5, Нарушаване правила,противозаконно превозване ООС,др.,във въздухоплавателно средство/кораб,причинена телесна повреда", "mppne02548"},
			{ 1900001737, "чл.336а,ал.4,чл.336а,ал.5, Нарушаване правила/противозаконно превозване ООС,др.,във въздухоплавателно средство/кораб,причинена смърт", "mppne02549"},
			{ 1900001738, "чл.340,ал.3, Повреждане на кораб,др.", "mppne02550"},
			{ 1900001739, "чл.340,ал.4, Повреждане съоръженията на летище/въздухоплавателно средство,което не е в експлоатация,др.", "mppne02551"},
			{ 1900001740, "чл.356е,ал.2, буква\"а\", чл.356й, Унищожаване/Повреждане ядрен материал,последвала вреда на околна среда/знач.имущ.вреди,др.", "mppne02569"},
			{ 1900001741, "чл.341а,ал.4, Извършване насилие на територията на летище", "mppne02552"},
			{ 1900001742, "чл.341в,ал.1, чл.341,ал.6,т.1, Поставяне повреждащо/разрушаващо устройство/вещество на кораб, застрашаване,последвала телесна повреда,др.", "mppne02554"},
			{ 1900001743, "чл.341в,ал.1, чл.341,ал.6,т.2, Поставяне повреждащо/разрушаващо устройство/вещество на кораб, застрашаване,последвала смърт,др.", "mppne02555"},
			{ 1900001744, "чл.341в,ал.2, чл.341,ал.6,т.1, Поставяне разрушаващо устройство/вещество на платформа,застрашаване,последвала телесна повреда,др.", "mppne02556"},
			{ 1900001745, "чл.341в,ал.2, чл.341,ал.6,т.2, Поставяне разрушаващо устройство/вещество на платформа,застрашаване,последвала смърт,др.", "mppne02557"},
			{ 1900001746, "чл.341в,ал.3,т.1,чл.341,ал.6,т.1, Застрашаване плаването на кораб,разрушаване/повреждане навигац.съоръжения,последвала телесна повреда,др.", "mppne02558"},
			{ 1900001747, "чл.341в,ал.3,т.2, чл.341,ал.6,т.1, Застрашаване плаването на кораб,съобщаване лъжл.информация/знак,последвала телесна повреда,др.", "mppne02559"},
			{ 1900001748, "чл.341в,ал.3,т.1,чл.341,ал.6,т.2, Застрашаване плаването на кораб,разрушаване/повреждане навигац.съоръжения,последвала смърт,др.", "mppne02560"},
			{ 1900001749, "чл.341в,ал.3,т.2, чл.341,ал.6,т.2, Застрашаване плаването на кораб,съобщаване лъжл.информация/знак,последвала смърт,др.", "mppne02561"},
			{ 1900001750, "чл.341в,ал.4, чл.341,ал.6,т.1, Извършване насилие на борда на кораб,последвала телесна повреда,др.", "mppne02562"},
			{ 1900001751, "чл.341в,ал.4, чл.341,ал.6,т.2, Извършване насилие на борда на кораб,последвала смърт,др.", "mppne02563"},
			{ 1900001752, "чл.341в,ал.5, чл.341,ал.6,т.1,Извършване насилие на борда на неподвижна платформа,последвала телесна повреда,др.", "mppne02564"},
			{ 1900001753, "чл.341в,ал.5, чл.341,ал.6,т.2, Извършване насилие на борда на неподвижна платформа,последвала смърт,др.", "mppne02565"},
			{ 1900001754, "чл.341в,ал.7,т.1, Поставяне повреждащо/разрушаващо устройство/вещество на кораб по непредпазливост,настъпили знач.имущ.вреди", "mppne02566"},
			{ 1900001755, "чл.341в,ал.7,т.2, Поставяне повреждащо/разрушаващо устройство/вещество на кораб по непредпазливост,настъпили СТП/ТТП,последвала телесна повреда,др.", "mppne02567"},
			{ 1900001756, "чл.341в,ал.7,т.3, Поставяне повреждащо/разрушаващо устройство/вещество на кораб по непредпазливост,настъпила смърт", "mppne02568"},
			{ 1900001757, "чл.356л,ал.1, Противозаконно действие срещу ядрено съоръжение,др.", "mppne02570"},
			{ 1900001758, "чл.356л,ал.2,т.1, Противозаконно действие срещу ядрено съоръжение,др.,причинени знач.имущ.вреди/вреда за околна среда", "mppne02571"},
			{ 1900001759, "чл.356л,ал.2,т.2, Противозаконно действие срещу ядрено съоръжение,др.,последвала телесна повреда", "mppne02572"},
			{ 1900001760, "чл.356л,ал.2,т.3, Противозаконно действие срещу ядрено съоръжение,др.,причинена смърт", "mppne02573"},
			{ 1900001761, "чл.356л,ал.3,т.1, Противозаконно действие срещу ядрено съоръжение,др.,по непредпазливост,причинени знач.имущ.вреди/вреда за околна среда", "mppne02574"},
			{ 1900001762, "чл.356л,ал.3,т.2, Противозаконно действие срещу ядрено съоръжение,др.,по непредпазливост,последвала телесна повреда", "mppne02575"},
			{ 1900001763, "чл.356л,ал.3,т.3, Противозаконно действие срещу ядрено съоръжение,др.,по непредпазливост,причинена смърт", "mppne02576"},
			{ 1900001764, "чл.356м,ал.1, Употреба на сила/заплаха за предоставяне ядрен материал,др.", "mppne02577"},
			{ 1900001765, "чл.356м,ал.2,т.1б.\"а\", Заплаха за използване ядрен материал,др.,с цел причиняване смърт/телесна повреда", "mppne02578"},
			{ 1900001766, "чл.356м,ал.2,т.1б.\"б\", Заплаха за използване ядрен материал,др.,с цел причиняване на знач.имущ.вреди/вреда за околна среда", "mppne02579"},
			{ 1900001767, "чл.356м,ал.2,т.2, Заплаха за отнемане ядрен материал чрез кражба/грабеж", "mppne02580"},
			{ 1900001768, "чл.356м,ал.2,т.3, Заплаха за противозаконно действие срещу ядрено съоръжение,с цел принуда,др.", "mppne02581"},
			{ 0, "", "" }
		};

		TEisCrime eisCrime;
		int count = 0;

		for (const TEISCrimeData *ecd = EISCrimeDataNew; ecd->code; ecd++)
		{
			eisCrime.code = ecd->code;

			if (!eisCrime.Try("F_CODE"))
			{
				strcpy(eisCrime.name, ecd->name);
				eisCrime.flags = FLAG_ACTIVE;
				strcpy(eisCrime.alias, ecd->alias);

				if (eisCrime.Insert())
					count++;
			}
		}

		if (count)
		{
			m.sep("\n\n");
			m.printf("Към ЕИСПП са добавени %d нови престъпления.", count);
		}
	}

#if COURT_TYPE == COURT_REGIONAL
	{
		int count = 0;

		SubpoenaKind->subpoenaKind = 'т';

		if (!SubpoenaKind->Try("F_NAME") && SubpoenaKind->DataBase->Execute("INSERT INTO T_SUBPOENA_KIND VALUES ('т', "
			"'Съобщение по чл.414a, ал. 3 ГПК', 'Съобщ.чл.414aл3', 'lVWБВГДЖИЛ', '!cPsdҐpZ', 'yqЗ', 'mcZpr', "
			"'anpmslP.L...t..G....', 'a', 0385, 'An_414a.htm', 'N', 'Приложение № 18а - съобщение за постъпило "
			"възражение и указания по чл. 414а, ал.3 ГПК', 'Допълнение съгл. чл.5 ал.2 от Наредба №7/2008 "
			"МП {F_EVENT_TEXT}', NULL)", ET_ERROR))
		{
			count++;
		}

		SubpoenaKind->subpoenaKind = 'у';

		if (!SubpoenaKind->Try("F_NAME") && SubpoenaKind->DataBase->Execute("INSERT INTO T_SUBPOENA_KIND VALUES ('у', "
			"'Съобщение по чл.415, ал. 1, т. 3 ГПК - ЗП', 'Съобщ.отказ ЗИ', '45689lLVWdЈБВГДЖИЛqSм', '!fЗҐ', NULL, "
			"'Ѓ', 'anpmslPML...t..G....', 'a', 0280, 'An_415_19b.htm', 'N',  'Приложение № 19б - съобщение за отказ "
			"за издаване на заповед за изпълнение и указания по чл. 415 ГПК', 'Допълнение съгл. чл.5 ал.2 от Наредба "
			"№7/2008 МП {F_EVENT_TEXT}', NULL)", ET_ERROR))
		{
			count++;
		}

		if (count > 0)
		{
			m.sep("\n\n");
			m.printf("Добавени са %d вида призовки/съобщения.", count);
		}

		SubpoenaKind->subpoenaKind = 'Ч';

		if (SubpoenaKind->Get("F_INFO") && !any(SubpoenaKind->info))
		{
			strcpy(SubpoenaKind->name, "Съобщение по чл.415, ал. 1, т. 2 ГПК - ЗП");
			strcpy(SubpoenaKind->info, "Приложение № 19а – съобщение за връчване на длъжника съгл. чл. 47, ал. 5 "
				"и указания по чл. 415 ГПК");
			SubpoenaKind->Update("F_NAME, F_INFO");
			m.sep("\n\n");
			m.cat("Извършени са промени в 'Съобщение по чл.415, ал. 1, т. 2 ГПК'");
		}
	}
#endif  // REGIONAL

#if COURT_TYPE == COURT_APPEAL
	{
		Kind->kind = '€';

		if (Kind->Get("F_OTHERS") && !strchr(Kind->others, 'Е'))
		{
			strcach(Kind->others, 'Е');

			if (Kind->Update("F_OTHERS"))
			{
				m.sep("\n\n");
				m.cat("Към 'Искане за възобновяване' е добавено участие 'Страна'");
			}
		}
	}
#endif  // APPEAL

	{
		// 2018-10-25: FIX: were missing
		struct TKinfXfer
		{
			char kind;
			long xfer;
		};

		static const TKinfXfer KindXfers[] =
		{
		#if COURT_TYPE == COURT_APPEAL
			{ 'ф', 8006 },
			{ '{', 8028 },
			{ '}', 8028 },
			{ '€', 8028 },
			{ '$', 8028 },
		#endif  // APPEAL
		#if COURT_TYPE == COURT_AREAL
			{ 'В', 8048 },
			{ 'ђ', 8050 },
			{ 'Ђ', 8088 },
			{ '.', 8088 },
			{ '„', 8063 },
			{ '‚', 8088 },
			{ 'ї', 8028 }, 
		#endif  // AREAL
		#if COURT_TYPE == COURT_REGIONAL
			{ 'J', 8048 },
			{ 'ш', 8028 },
			{ 'і', 8006 },
			{ 'ї', 8048 },
			{ 'y', 8006 },
			{ 'z', 8048 },
			{ 'Ђ', 8006 },
			{ 'щ', 8028 },
			{ 'F', 8006 },
		#endif  // REGIONAL
			{ '\0', 0 }
		};

		int count = 0;

		for (const TKinfXfer *kx = KindXfers; kx->kind; kx++)
		{
			Kind->Seek(kx->kind);

			if (!Kind->xfer)
			{
				Kind->xfer = kx->xfer;

				if (Kind->Update("F_XFER"))
					count++;
			}
		}

		if (count > 0)
		{
			m.sep("\n\n");
			m.printf("Добавени са трансферни кодове към %d вида документи.", count);
		}
	}

	// 2018-11-21: LRQ/IRQ: unblocked
	{
		enum { sbe_ptvpke = 930 };

		SbeDesc->vid = sbe_ptvpke;
		strcpy(SbeDesc->name, "NPR.DLO.dlosprvid");

		if (SbeDesc->Get("F_VALUE") && !strstr(SbeDesc->value, "1018"))
		{
			strcat(SbeDesc->value, ", 1018");

			if (SbeDesc->Update("F_VALUE"))
			{
				m.sep("\n\n");
				m.printf("Към ЕИСПП събитие %d е добавен вид производство 1018.", sbe_ptvpke);
			}
		}

		struct TSbeDescData
		{
			const char *name;
			long flags;
			const char *value;
		};

		static const TSbeDescData SbeDescData[] =
		{
			{ "NPR.DLO.PNE",                  3, "" },
			{ "NPR.DLO.PNE.pnenmr",           3, "" },
			{ "NPR.DLO.PNE.PNESTA.*",         1, "" },
			{ "NPR.DLO.NPRFZLPNE",            3, "" },
			{ "NPR.DLO.NPRFZLPNE.SCQ",        3, "" },
			{ "NPR.DLO.NPRFZLPNE.SCQ.scqvid", 3, "986" },
			{ "NPR.DLO.NPRFZLPNE.SCQ.scqstv", 1, "" },
			{ "NPR.DLO.NPRFZLPNE.SCQ.scqstr", 3, "-1" },
			{ "NPR.DLO.NPRFZLPNE.SCQ.scqdta", 1, "" },
			{ "NPR.DLO.NPRFZLPNE.SCQ.scqosn", 7, "" },
			{ "NPR.DLO.NPRFZLPNE.SCQ.scqrlq", 7, "846...848, 851, 852" },
			{ "NPR.DLO.NPRFZLPNE.SBC",        1, "" },
			{ "NPR.DLO.NPRFZLPNE.SBC.sbcrcd", 5, "1587...1591" },
			{ "NPR.DLO.NPRFZLPNE.SBC.sbcple", 5, "1575...1578" },
			{ "NPR.DLO.NPRFZLPNE.SBC.sbcspj", 5, "" },
			{ "NPR.DLO.NPRFZLPNE.SBC.sbcobr", 5, "" },
			{ "NPR.DLO.NPRFZLPNE.SBC.sbctrd", 5, "" },
			{ "NPR.DLO.NPRFZLPNE.SBC.sbcznq", 5, "" },
			{ "NPR.DLO.NPRFZLPNE.SBC.sbcetn", 5, "" },
			{ "NPR.DLO.NPRFZLPNE.SBC.sbcrge", 5, "" },
			{ "NPR.DLO.NPRFZLPNE.NPL",        1, "" },
			{ "NPR.DLO.NPRFZLPNE.NPL.*",      7, "" },
			{ "NPR.DLO.NPRFZLPNE.VSL",        1, "" },
			{ "NPR.DLO.NPRFZLPNE.VSL.vslpdl", 5, "" },
			{ "NPR.DLO.NPRFZLPNE.VSL.vsldlj", 5, "" },
			{ "NPR.DLO.NPRFZLPNE.VSL.vslzvn", 7, "" },
			{ "NPR.DLO.NPRFZLPNE.VSL.vslktg", 7, "" },
			{ ".scqosn",                      7, "240" },
			{ NULL, 0, NULL }
		};

		int count = 0;

		for (const TSbeDescData *sdd = SbeDescData; sdd->name; sdd++)
		{
			strcpy(SbeDesc->name, sdd->name);

			if (!SbeDesc->Try("F_VID"))
			{
				SbeDesc->flags = sdd->flags;
				strcpy(SbeDesc->value, sdd->value);

				if (SbeDesc->Insert())
					count++;
			}
		}

		if (count > 0)
		{
			m.sep("\n\n");
			m.printf("Към описанието на ЕИСПП събитие %d са добавени %d елемента.", sbe_ptvpke, count);
		}
	}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	// 2018-11-16
	{
		Right->right = RIGHT_EPORT_RES;

		if (!Right->Try())
		{
			strcpy(Right->name, "Резолиране на заявление за регистрация в " EPORTAL_NAME);

			if (Right->Insert())
			{
				m.sep("\n\n");
				m.cat("Към потребителските права е добавена 1 нова функция.");

				msql w(User->Name, "F_UCN, F_FUNCTIONS");

				w.AddChars("F_POST", POST_BOSSES);
				w.AddLike("NOT F_FUNCTIONS", C2S[RIGHT_EPORT_RES], true);
				w.Add("F_FLAGS");
				User->DataBase->CondActive(w, "F_FLAGS");

				TQuery q(str(w));
				while (q.Read())
				{
					*User << q;
					strcach(User->functions, RIGHT_EPORT_RES);
					User->Update("F_FUNCTIONS");
				}
			}
		}
	}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_REGIONAL
	// 2018-11-20
	{
		int count = 0;

		for (int i = 0; SUBPOENA_KIND_HERITAGES[i]; i++)
		{
			SubpoenaKind->Seek(SUBPOENA_KIND_HERITAGES[i]);

			if (!any(SubpoenaKind->statuses))
			{
				strcpy(SubpoenaKind->statuses, "!f");
				strcat(SubpoenaKind->kinds, KIND_SESSIONS);

				if (SubpoenaKind->Update("F_KINDS, F_STATUSES"))
					count++;
			}
		}

		if (count > 0)
		{
			m.sep("\n\n");
			m.printf("Към %d съобщения е добавена възможност за изготвяне по всички видове заседания.", count);
		}
	}
#endif  // REGIONAL

#if COURT_TYPE == COURT_AREAL
	// 2018-11-20
	{
		enum { law_kind = '!' };
		SubpoenaKind->Seek('н');

		if (!strchr(SubpoenaKind->lawsuitKinds, law_kind))
		{
			strcach(SubpoenaKind->lawsuitKinds, law_kind);

			if (SubpoenaKind->Update("F_LAWSUIT_KINDS"))
			{
				m.sep("\n\n");
				m.printf("Към 1 съобщение е добавен вид дело %s.", Kind->Seek(law_kind));
			}
		}
	}
#endif  // AREAL

	// ----- display restart message if needed -----------------------------
#if TESTVER
	{
		// 2018-03-12: nom or dates changed
		const char *restart = NULL;

		if (any(m))
		{
			m.cat("\n\nИзвършени са промени в номенклатурите. Рестартирайте САС.");
			restart = str(m);
		}

		if (check_config_dates() && !restart)
			restart = "Извършени са промени в конфигурацията. Рестартирайте САС.";

		if (restart)
			TLongTextDialog(parent, restart).Execute();
	}
#else  // TESTVER
	if (any(m))
	{
		m.cat("\n\nИзвършени са промени в номенклатурите. Рестартирайте САС.");
		TLongTextDialog(parent, str(m)).Execute();
	}
#endif  // TESTVER

	{
		// direct statement is too slow
		//AppName->Execute("UPDATE T_LAW_INTERVAL I SET I.F_DATE = (SELECT L.F_DATE FROM T_LAWSUIT L WHERE "
		//	"L.F_NO = I.F_NO AND L.F_YEAR = I.F_YEAR AND L.F_TYPE = I.F_TYPE) WHERE I.F_FLAGS IS NULL", ET_ERROR);
		msql m("T_LAW_INTERVAL I JOIN T_LAWSUIT L ON L.F_NO = I.F_NO AND L.F_YEAR = I.F_YEAR AND L.F_TYPE = I.F_TYPE",
			"I.F_NO, I.F_YEAR, I.F_TYPE, L.F_DATE");
		TLawInterval interval;
		unsigned synced = 0;

		m.AddFlags("I.F_FLAGS", LAW_INTERVAL_SECONDARY, 0, LAW_INTERVAL_ALL);
		m.Add("NOT I.F_DATE");
		m.cat("= L.F_DATE");

		TQuery q(str(m));
		while (q.Read())
		{
			msql w(interval.Name, MQ_UPDATE);

			interval << q;
			interval.Quote(w, "F_DATE");
			w.Add(&interval, "F_NO, F_YEAR, F_TYPE, F_FLAGS");

			if (interval.DataBase->Execute(str(w), ET_ERROR))
				synced++;
		}

		if (synced)
			info("Началните интервали на %u дела са синхронизирани с датите на образуване.", synced);
	}

	{
		int count = 0;
		TDecision decision;
		msql w(decision.Name, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");

		w.AddFlags("F_FLAGS", DECISION_PUBLIC, DECISION_PUBLIC, DECISION_ALL);
		w.AddIsNull("F_PUBLIC_DATE", true);
		TQuery q(str(w));

		while (q.Read())
		{
			decision << q;
			decision.publicDate = Today;

			if (decision.Update("F_PUBLIC_DATE"))
				count++;
		}

		if (count)
		{
			info("%d акт(а) са добавени към \"Справка за WEB страниза -> За съдебни актове, подлежащи на "
				"публикуване\", с днешна дата на публикуване.", count);
		}
	}

#if TESTVER
#pragma message TODO: remove in the next version
#endif  // TESTVER
	{
		int count = 0;
		TDecision decision;
		msql w(decision.Name, "F_NO, F_YEAR, F_TYPE, F_KIND, F_DATE");

		w.AddFlags("F_FLAGS", DECISION_PUBLIC, 0, DECISION_ALL);
		w.AddIsNull("F_PUBLIC_DATE", false);
		TQuery q(str(w));

		while (q.Read())
		{
			decision << q;
			decision.publicDate.Clear();

			if (decision.Update("F_PUBLIC_DATE"))
				count++;
		}

		if (count)
		{
			m.clear();
			m.printf("%d акт(а) са извадени от \"Справка за WEB страница -> За съдебни актове, подлежащи на "
				"публикуване\".", count);
			//info("%s", str(m));
		}
	}

#if TESTVER
	if (!any(m))
	{
		msql w(Subject->Name, "F_SUBJECT, F_KIND_I, F_KIND_II, F_VID_I, F_VID_II");

		w.AddIsNull("F_VID_I", false);
		w.NextOr();
		w.AddIsNull("F_VID_II", false);
		w.AddOrder("F_SUBJECT");

		TQuery q(str(w));
		while (q.Read())
		{
			*Subject << q;

			if ((Subject->sd[0].kind && !eispp_matches(Subject->sd[0].kind, Subject->sd[0].vid)) ||
				(Subject->sd[1].kind && !eispp_matches(Subject->sd[1].kind, Subject->sd[1].vid)))
			{
				m.sep("\n");
				m.printf("Тестова версия: вида на съдебното производство за предмет %s "
					"не съответства на вида на делото.", Subject->Code(0, false));
			}
		}

		if (any(m))
			message(MB_ICONWARNING | MB_OK, "%s", str(m));
	}
#endif  // TESTVER
}