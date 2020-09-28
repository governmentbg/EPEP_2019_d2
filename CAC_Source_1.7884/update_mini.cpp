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
		sk.subpoenaKind = '�';

		if (sk.Try("F_NAME, F_ATTRIBUTES") && !strchr(sk.attributes, SK_ATTR_INDIRECT_EVENT))
		{
			sk.attributes[14] = SK_ATTR_INDIRECT_EVENT;

			if (sk.Update("F_ATTRIBUTES"))
			{
				m.sep("\n\n");
				m.printf("��������� �� ���������� �� %s.", sk.name);
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
			{ 1101, 1900000054, "������ \"����������-������������ �������\" - ���� - ���������", "���", "str_akia" },
			{ 1097, 1900001779, "��������� ����� - ����������", "���", "str_ar_asparyh" },
			{ 1097, 1900001778, "��������� ����� - �����", "���", "str_ar_devnya" },
			{ 1097, 1900001780, "��������� ����� - ������������", "���", "str_ar_dimitr" },
			{ 1097, 1900001783, "��������� ����� - ���������", "���", "str_ar_zltgr" },
			{ 1097, 1900001782, "��������� ����� - �������", "���", "str_ar_lukov" },
			{ 1097, 1900001784, "��������� ����� - �����", "���", "str_ar_mdn" },
			{ 1097, 1900001781, "��������� ����� - ������", "���", "str_ar_pesht" },
			{ 1097, 1900001785, "��������� ����� - ��������", "���", "str_ar_chplr" },
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
			m.printf("��� ����� �� �������� %d ���� ���������.", count);
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
				m.cat("������� 800 �� ���������� ���� � ���������.");
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
				" VALUES (2211000, 'c', 'Z',  NULL, '������ �� ������ �� ���������� ����� � �������� �� �����')",
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
				m.cat("������� � ������� 22110-1 �� ���������� ����.");
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
				" VALUES (450050, 'a', NULL, 'E',  '��. 281 �� �����')", ET_ERROR))
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
				m.cat("������� � ������� 4500 �� ��������������� ����.");
			}
		}

		Subject->type = 't';

		if (!Subject->Try())
		{
			if (Subject->DataBase->Execute("INSERT INTO T_SUBJECT (F_SUBJECT, F_TYPE, F_KIND_I, F_KIND_II, F_NAME) "
				" VALUES (450050, 't', NULL, 'z',  '��. 281 �� �����')", ET_ERROR))
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
				m.cat("������� � ������� 4500 �� ����������� ����.");
			}
		}
	}
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{
		msql w(SubpoenaKind->Name);
		int count = 0;

		w.AddLike("(F_NAME", "����", false);
		w.NextOr();
		w.AddChar("F_PROC", SK_PROC_NOTIFYING);
		w.NextOr();
		w.AddChars("F_SUBPOENA_KIND", "��");	// 2017:341
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
			m.printf("��� %s �� �������� %d ���� ������������.", Kind->Seek(KIND_PREORDER_SESSION), count);
		}
	}

	{
		enum { zasegnato = '�' };

		Kind->Seek('D');

		if (!strchr(Kind->others, zasegnato))
		{
			strcach(Kind->others, zasegnato);

			if (Kind->Update("F_OTHERS"))
			{
				m.sep("\n\n");
				m.printf("��� ��������� �� %s � �������� %s.", Kind->name, Involvement->Seek(zasegnato));
			}
		}
	}


	{
		int count = 0;

		SubpoenaKind->subpoenaKind = '<';

		if (!SubpoenaKind->Try("F_NAME") && SubpoenaKind->DataBase->Execute("INSERT INTO T_SUBPOENA_KIND VALUES ('<',"
			"'��������� �� ��.247� ��� �� ������� ���������', '�����. ��.247�', '�', 'f', '�', 'O',"
			" 'anpmslP.L...t..G....', 'a', 1070, 'Smp1fs_r3.htm', 'M',  NULL, NULL, NULL)", ET_ERROR))
		{
			count++;
		}

		SubpoenaKind->subpoenaKind = '>';

		if (!SubpoenaKind->Try("F_NAME"))
		{
			mstr w;

			w.printf("INSERT INTO T_SUBPOENA_KIND VALUES ('>', '�������� �� ������� ���������',"
				" '����. ���������', '%c%c', 'f', '�', 'O', 'anpmslP.L...t..G....', 'a', 1075,"
				" 'Smp1fsj.htm', 'S',  NULL, NULL, NULL)", KIND_COURTPUN_SESSION, KIND_OTHER_SESSION);

			if (SubpoenaKind->DataBase->Execute(str(w), ET_ERROR))
				count++;
		}

		if (count > 0)
		{
			m.sep("\n\n");
			m.printf("�������� �� %d ���� ��������/��������� �� ������� ���������.", count);
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
			strcpy(EisVal->name, "�����");
			EisVal->flags = FLAG_ACTIVE;
			strcpy(EisVal->alias, "sign_vnaxd");

			if (EisVal->Insert())
			{
				m.sep("\n\n");
				m.printf("��� ����� � ������� ��� %d.", sign_vnaxd);
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
					m.printf("����� ��� %d � ������� ��� %s.", sign_vnaxd, Kind->name);
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
				m.printf("����� ��� %d � ������� ��� ������� %s.", sign_vnaxd, str(w));
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
			strcpy(EisVal->name, "������ �� ���������� �� ��������������(��.368�,��.1���)");
			EisVal->flags = FLAG_ACTIVE;
			strcpy(EisVal->alias, "ipz_za_uskoriavane_proizvodstvo");

			if (EisVal->Insert())
			{
				m.sep("\n\n");
				m.printf("��� ����� � ������� ��� %d.", ipz_za_uskoriavane_proizvodstvo);
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
					m.printf("����� ��� %d � ������� ��� ������� %d.", ipz_za_uskoriavane_proizvodstvo, SbeDesc->vid);
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
			{ 240, 1900001678, "��������, ������� � ������������ ��� ��� � �������, ���������� ��������������� ���������(��.250,��.1,�.2 �� ���)", "osn_adm_nar_sad" },
			{ 240, 1900001674, "����������� ������ ���������� ��������������� ���������, �� ����� � ���������� ���(��.24,��.1, �.8� �� ���)", "osn_adm_narushenie" },
			{ 240, 1900001677, "������ ����� (��.24,��.5,�.1 �� ���)", "osn_lipsva_tujba_postradal" },
			{ 240, 1900001675, "�� � ��������� ����������� �� ������������� �� ���(��.24,��.4,��.1, ��.��.1,�.8� �� ���)", "osn_lipsva_vazobn_anp" },
			{ 240, 1900001676, "�������� ��� �������� ����������� �� ������������� �� ���(��.24,��.4,��.2 ��.��.1,�.8� �� ���)", "osn_neuvaj_vazobn_anp" },
			{ 242, 1900001680, "�� ������ ������, ����� � ������������, � ���������� ���(��.25,��.1, �.5 �� ���)", "osn_prikl_anp" },
			{ 242, 1900001681, "��������� ����� �� ������������ ������ �����, �� ����� �� ���� �������(��.25, ��.2 �� ���)", "osn_molba_pravna_pomosht" },
			{ 242, 1900001679, "��������� ������������ ��������� �� ���� �� ��(��.25, ��.1, �.4 �� ���)", "osn_zapitvane_SES" },
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
			m.printf("��� ����� �� �������� %d ���� ��������� �� �������/������������.", count);
		}
	}

	{
		EisVal->nom = 253;
		EisVal->code = 1900001682;

		if (!EisVal->Try("F_CODE"))
		{
			strcpy(EisVal->name, "�� ����� �� ����������� �� ��������������");
			EisVal->flags = FLAG_ACTIVE;
			strcpy(EisVal->alias, "ncn_po_tajba");

			if (EisVal->Insert())
			{
				m.sep("\n\n");
				m.printf("��� ����� � ������� ��� %ld.", EisVal->code);
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
				m.printf("����� ��� %ld � ������� ��� %d �������.", EisVal->code, count);
			}
		}
	}

	{	// 2018-01-16
		enum { nmk_ptvnkzvid = 256, ptv_otkaz_pred_osv = 1900001686 };

		EisVal->nom = nmk_ptvnkzvid;
		EisVal->code = ptv_otkaz_pred_osv;

		if (!EisVal->Try())
		{
			strcpy(EisVal->name, "����������� ������ �� ���������� �������������");
			EisVal->flags = FLAG_ACTIVE;
			strcpy(EisVal->alias, "ptv_otkaz_pred_osv");

			if (EisVal->Insert())
			{
				m.sep("\n\n");
				m.printf("��� ����� � ������� ��� %d.", ptv_otkaz_pred_osv);
			}
		}
	}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	{
		Result->result = RESULT_UN_STOPPED;

		if (!Result->Try())
		{
			strcpy(Result->name, "������������� �� ��������������");
			Result->OrderField()->SetL(9993);
			Result->xfer = 11002;

			if (Result->Insert())
			{
				m.sep("\n\n");
				m.printf("������� � �������� %s.", Result->name);
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
				m.printf("�������� %s � ������� ��� %d ��������.", Result->name, count);
			}
		}
	}
#endif  // ADMIN || AREAL || MILITARY || REGIONAL

	{
		int count = 0;

	#if COURT_TYPE == COURT_AREAL || SPECIAL
		static const TReportRangeData rangeCrimes[] =
		{
			{ 712, "���������� ����������� � ��. 201 �� (���� �� ����)" },
			{ 713, "���������� �����������, �� ������������ �� ����� � ��������� � ����� ������������, �� ��-����� ��������� � ��. 201 �� (���� �� ����)" },
			{ 714, "���������� ����������� � ������ ������� � ��. 202, ��. 1, �. 1 � �. 2 �� (���� �� ����)" },
			{ 716, "��������� ������ �� ���������� ����������� � ��. 204 �� (���� �� ����) � ��. 205 �� (���� �� ����)" },
			{ 726, "���������� ������ � ��. 212, ��. 1 � ��. 2 �� (���� �� ����)" },
			{ 727, "���������� ������, ��� ����� ���������� ��� ������ ��������� ��������� � �� ������� �� ����������� ���� ��� ������������ �� ��� �� ����������� ������� � ��. 212, ��. 3 �� (���� �� ����)" },
			{ 728, "���������� ������ � ������ ������� ��� �������������� ������ ������� � ��. 212, ��. 4 �� (���� �� ����)" },
			{ 730, "������ �� ��.212� �� (���� �� ����)" },
			{ 921, "������������ �� ������ - ��.285 �� (���� �� ����)" },
			{ 0, NULL }
		};

		for (const TReportRangeData *rr = rangeCrimes; rr->subject; rr++)
			if (insert_report_range(rr, "p", "%04ld"))
				count++;
	#endif  // AREAL || SPECIAL

	#if COURT_TYPE == COURT_AREAL
		static const TReportRangeData rangeOthers[] =
		{
			{ 21110, "\\b�.�.�.\\c ����� �� ��������� �� ������������ �� ���������������" },
			{ 21120, "������ �� ������������ /��������� �� ���������/ ���������������� �� �������� � ������ �� ��������� �� ����������� �� �����������������" },
			{ 21130, "��� �� ������������ ������������ �� �������� ��� �������������� �� ������ �������, ���������� �� ���" },
			{ 24100, "\\b�.�.�.\\c ����� �� ��������� �� ������������ �� ������������" },
			{ 24200, "��� �� ����������� �� ����� �� ������������" },
			{ 0, NULL }
		};

		for (const TReportRangeData *rr = rangeOthers; rr->subject; rr++)
			if (insert_report_range(rr, "ctad", "%ld"))
				count++;
	#endif  // AREAL

		if (count)
		{
			m.sep("\n\n");
			m.printf("�������� �� %d ������� �������.", count);
		}
	}

	{
		SubpoenaKind->subpoenaKind = '=';

		if (!SubpoenaKind->Try("F_NAME") && SubpoenaKind->DataBase->Execute("INSERT INTO T_SUBPOENA_KIND VALUES ('=',"
			"'��������� � � ������������/�����������', '�����.����/���.', '�', '��', NULL, '��',"
			" 'anpmslPML...t..G....', 'a', 65, 'aplan_req.htm', 'N',  NULL, '{F_EVENT_TEXT}', NULL)", ET_ERROR))
		{
			m.sep("\n\n"); //��������� � � ������������/� �����������
			m.printf("�������� � %s.", SubpoenaKind->Seek('='));
		}
	}

	// 2018-07-02
	{
		long count = AppName->ExecCount("UPDATE T_SUBPOENA_KIND "
			"SET F_ADDITIONAL_TEXT = '{F_LAWSUIT_TEXT}\v' || F_ADDITIONAL_TEXT "
			"WHERE (F_NAME LIKE '%���� ���%' OR F_INFO LIKE '%���� ���%') "
			"AND NOT F_ADDITIONAL_TEXT LIKE '{F_LAWSUIT_TEXT}%'",
			ET_UPDATE);

		if (count)
		{
			m.sep("\n\n");
			m.printf("��� %ld ��������/��������� - ���� ��� � ������� ������ �� ������.", count);
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
			m.printf("��� %d ������������ �����/����� � �������� %s.", count, Kind->Seek(KIND_PREORDER_SESSION));
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
			{ 1900000055, "��.168�,��.1, ����������� �� �������� �� ��������� � ��������� �� ������������ �������", "mppne02457"},
			{ 1900000056, "��.168�,��.2, �������������� �������/��������������� �� �������� �� ���������", "mppne02458"},
			{ 1900001642, "��.159�,��.1,��.1, ������ �� ����/����� �� ����, � ��� �������", "mppne024507"},
			{ 1900001643, "��.159�,��.1,��.2.1, ������ �� ����/����� �� ����, � ��� ������������ ����", "mppne024508"},
			{ 1900001644, "��.159�,��.1,��.2.2, ������ �� ����/����� �� ����, � ��� ������", "mppne024509"},
			{ 1900001645, "��.159�,��.1,��.3, ������ �� ����/����� �� ����, � ��� �������� �� ������� �����/�����/������/������� �������", "mppne024510"},
			{ 1900001646, "��.159�,��.1,��.4, ������ �� ����/����� �� ����, � ��� ������� � ������������ ����������", "mppne024511"},
			{ 1900001647, "��.159�,��.2,�.7, ������, �������� �� ���������� ���� ���/�� ����� ���������� �� �������� ��", "mppne024512"},
			{ 1900001648, "��.255�,��.1, �������� ������������ ������������ ������, � ������ �������", "mppne024513"},
			{ 1900001649, "��.255�,��.1,�.1, ��������� ������������ ����� � ��-����� ������ �� ������������� �� ����������� ����", "mppne024514"},
			{ 1900001650, "��.255�,��.1,�.2, ���������� ���������� �� ������������ ������������ ������", "mppne024515"},
			{ 1900001651, "��.255�,��.1,�.3, ������������� ��������/��������� ������ � �������� ���������� �� ������������ ������������ ������", "mppne024516"},
			{ 1900001652, "��.255�,��.1,�.4, ��������� ��� ���������� �������� � ������� ����������, ���������� ��� ��������� ��� ����������� �� ����.�������/������ ������������/����������� ���������� ���� �������� �� ���������", "mppne024517"},
			{ 1900001653, "��.255�,��.1,�.5, ����������� ��� �������� � ������������������ ������� ���������� ���������/��������/��������� �� �������", "mppne024518"},
			{ 1900001654, "��.255�,��.2, �������� ������������ ������������ ������, � ��������� �� ����� �� ��������� ��� ����������� ������", "mppne024519"},
			{ 1900001655, "��.255�,��.3, �������� ������������ ������������ ������, � ������� ������ �������", "mppne024520"},
			{ 1900001656, "��.255�,��.4, ������� � �������,���.������� �� ������������ ������������ ������ �� ����������� �� ���.��������� � ����������������� ���", "mppne024521"},
			{ 1900001687, "��.142�,��.3,�.2, �������������� ���.������� �� ����,�������� �� � ������������ ������", "mppne02587"},
			{ 1900001688, "��.171,��.4, �������������� ������������� ������ ��/�������� �� ����.�����,������������� ����� ����/������ ���.�������", "mppne02585"},
			{ 1900001689, "��.171,��.5, �������������� ������������� ������ ��/�������� ����� ���������/����.�����,��.,� �������� ���/����.����.�����", "mppne02586"},
			{ 1900001690, "��.195,��.1,�.11, ������ � ��� �������� �������� �� ���������� ������������ �� ��.108�,��.1,2,6 ��� 7", "mppne02584"},
			{ 1900001692, "��.240�,��.1, ��������� ����� �������� ���������", "mppne02507"},
			{ 1900001693, "��.240�,��.2, �������,�����������,��������������, ������������/����������� ��������� ������ �������� ���������", "mppne02508"},
			{ 1900001694, "��.240�,��.3,�.1, ��������� ����� �������� ���������,� ������ �������", "mppne02509"},
			{ 1900001695, "��.240�,��.3,�.2, ��������� ����� �������� ��������� �� �������� ���������/����", "mppne02510"},
			{ 1900001696, "��.240�,��.3,�.3, ��������� ����� �������� ��������� � ����. ��������/���", "mppne02511"},
			{ 1900001697, "��.240�,��.3,�.4, ��������� ����� �������� ��������� ���� ���������� ����������/���������/� ������� ���������� ��������", "mppne02512"},
			{ 1900001698, "��.240�,��.3,�.5, ��������� ����� �������� ��������� �� ���/������ ���� � ������������� ������", "mppne02513"},
			{ 1900001699, "��.240�,��.3,�.6, ��������� ����� �������� ��������� � ������� �� ���������� ����", "mppne02514"},
			{ 1900001700, "��.240�,��.3,�.7, ��������� ����� �������� ��������� �� ����,��������", "mppne02515"},
			{ 1900001701, "��.240�,��.3,�.8, ��������� ����� �������� ���������,��������", "mppne02516"},
			{ 1900001702, "��.240�,��.4,�.1, ��������� ����� �������� ���������,������� ������ �������,������� ����� ������", "mppne02517"},
			{ 1900001703, "��.240�,��.4,�.2, ��������� ����� �������� ��������� �� ���������/� ���������� ������� ���", "mppne02518"},
			{ 1900001704, "��.240�,��.4,�.3, ��������� ����� �������� ���������,������ �������", "mppne02519"},
			{ 1900001705, "��.240�,��.5, ��������� ����� �������� ���������,��������� ������", "mppne02520"},
			{ 1900001706, "��.240�,��.8, ���������/�����������/������/������� ������,��.�� ����� �� �������� ��������� ��� ��������", "mppne02521"},
			{ 1900001708, "��.260�,��.1,�.1, ����������� � ����.���������� �� ���� �� ����� �� �������, ��.", "mppne02522"},
			{ 1900001709, "��.260�,��.1,�.2, ����������� � ����.���������� �� ���� � ������ ������� � �������� �� �������, ��.", "mppne02523"},
			{ 1900001710, "��.260�,��.1,�.3, ����������� � ����.���������� �� ���� � ������ �� ���", "mppne02524"},
			{ 1900001711, "��.260�,��.2, ����������� � ����.����������,��������� ����� �������� �� ��.1", "mppne02525"},
			{ 1900001712, "��.260�,��.3, ����������� � ����.���������� � ������/�������� �����.����������,���������� ������ ���������", "mppne02526"},
			{ 1900001713, "��.260�,��.4, ����������� � ����.���������� � ������/�������� �����.���������� �� ������ ��������� �������� ���,����.������ ���������", "mppne02527"},
			{ 1900001714, "��.260�,��.5, ���������� ��� ����������� � ����.����������", "mppne02528"},
			{ 1900001715, "��.260�,��.6, ����������� � ����.���������� �� �������� �� ���� ����", "mppne02529"},
			{ 1900001716, "��.260�,��.7,�.1, ����������� � ����.���������� �� ���/������ ���� � ������������� ������", "mppne02530"},
			{ 1900001717, "��.260�,��.7,�.2, ����������� � ����.���������� �� ���������/���������� ������� ���", "mppne02531"},
			{ 1900001718, "��.260�,��.7,�.3, ����������� � ����.����������,��������", "mppne02532"},
			{ 1900001719, "��.260�,��.1, �������������� ���������� ����.����������, ������� �����.����������", "mppne02533"},
			{ 1900001720, "��.260�,��.2, ������������ �� ���������� ��� ���������� ����.����������,������� �����.����������,����.������ ���������", "mppne02534"},
			{ 1900001721, "��.260�,��.1, ������������ �����.�����,����.������ ���������", "mppne02535"},
			{ 1900001722, "��.260�,��.2, ������������ �����.����� ���� ���������� �������� ��������/��������� � �����������", "mppne02536"},
			{ 1900001723, "��.260�,��.3, ������������ �����.����� ���� �����,��.,����.������ ���������", "mppne02537"},
			{ 1900001724, "��.260�,��.4, ������������ ������������� �� ��������,����.������ ���������", "mppne02538"},
			{ 1900001725, "��.309,��.3, ��������� �� ������ ��������, ����� ����� �����,��.", "mppne02582"},
			{ 1900001726, "��.309,��.4, ��������� �� ������ ��������,����� ����� �����,��.,������� ������ �������", "mppne02583"},
			{ 1900001727, "��.319�,��.4, ������������ ����������� ����.����� ���� ����.��������,������,������,�������� ������ �� ���� ���.�������,����� ���������", "mppne02539"},
			{ 1900001728, "��.319�,��.5,�.1, ������������ ����������� ����.�����/������� ������� �� ����� � ���.�������,�� ���������/� ���������� ������� ���", "mppne02540"},
			{ 1900001729, "��.319�,��.5,�.2, ������������ ����������� ����.�����/������� ������� �� ����� � ���.�������,����� � ���� �� �������� ��������������", "mppne02541"},
			{ 1900001730, "��.319�,��.3, ���������������� ����.��������,������,������ �� ������ �� ���.�������/���� �� ���,� �������� ���,���������� �����", "mppne02542"},
			{ 1900001731, "��.336�,��.1,�.1,��.336�,��.5, ���������� ������� �� ���������� ���,������ ���������� ��� ����������������� ��������/�����/���������,�������� �� ������,��.", "mppne02543"},
			{ 1900001732, "��.336�,��.1,�.2,��.336�,��.5, ���������� ������� �� ���������� �� ���, ������ ���������� ��� ����������������� ��������/�����/���������,�������� �� ������,��.", "mppne02544"},
			{ 1900001733, "��.336�,��.1,�.3, ��.336�,��.5, ���������� ������� �� ���������� �� ������/������ ��������,����� �������� �� �.2", "mppne02545"},
			{ 1900001734, "��.336�,��.1,�.4, ��.336�,��.5, ��������� ������� �� ���������� ����������������� ��������/�����,�������� �� ������,��.", "mppne02546"},
			{ 1900001735, "��.336�,��.2, ��.336�,��.5, �������������� ���������� ���������� ��������,��.,��� ����������������� ��������/�����", "mppne02547"},
			{ 1900001736, "��.336�,��.3, ��.336�,��.5, ���������� �������,�������������� ���������� ���,��.,��� ����������������� ��������/�����,��������� ������� �������", "mppne02548"},
			{ 1900001737, "��.336�,��.4,��.336�,��.5, ���������� �������/�������������� ���������� ���,��.,��� ����������������� ��������/�����,��������� �����", "mppne02549"},
			{ 1900001738, "��.340,��.3, ���������� �� �����,��.", "mppne02550"},
			{ 1900001739, "��.340,��.4, ���������� ������������ �� ������/����������������� ��������,����� �� � � ������������,��.", "mppne02551"},
			{ 1900001740, "��.356�,��.2, �����\"�\", ��.356�, �����������/���������� ����� ��������,���������� ����� �� ������ �����/����.����.�����,��.", "mppne02569"},
			{ 1900001741, "��.341�,��.4, ���������� ������� �� ����������� �� ������", "mppne02552"},
			{ 1900001742, "��.341�,��.1, ��.341,��.6,�.1, ��������� ����������/����������� ����������/�������� �� �����, ������������,���������� ������� �������,��.", "mppne02554"},
			{ 1900001743, "��.341�,��.1, ��.341,��.6,�.2, ��������� ����������/����������� ����������/�������� �� �����, ������������,���������� �����,��.", "mppne02555"},
			{ 1900001744, "��.341�,��.2, ��.341,��.6,�.1, ��������� ����������� ����������/�������� �� ���������,������������,���������� ������� �������,��.", "mppne02556"},
			{ 1900001745, "��.341�,��.2, ��.341,��.6,�.2, ��������� ����������� ����������/�������� �� ���������,������������,���������� �����,��.", "mppne02557"},
			{ 1900001746, "��.341�,��.3,�.1,��.341,��.6,�.1, ������������ ��������� �� �����,�����������/���������� �������.����������,���������� ������� �������,��.", "mppne02558"},
			{ 1900001747, "��.341�,��.3,�.2, ��.341,��.6,�.1, ������������ ��������� �� �����,���������� ����.����������/����,���������� ������� �������,��.", "mppne02559"},
			{ 1900001748, "��.341�,��.3,�.1,��.341,��.6,�.2, ������������ ��������� �� �����,�����������/���������� �������.����������,���������� �����,��.", "mppne02560"},
			{ 1900001749, "��.341�,��.3,�.2, ��.341,��.6,�.2, ������������ ��������� �� �����,���������� ����.����������/����,���������� �����,��.", "mppne02561"},
			{ 1900001750, "��.341�,��.4, ��.341,��.6,�.1, ���������� ������� �� ����� �� �����,���������� ������� �������,��.", "mppne02562"},
			{ 1900001751, "��.341�,��.4, ��.341,��.6,�.2, ���������� ������� �� ����� �� �����,���������� �����,��.", "mppne02563"},
			{ 1900001752, "��.341�,��.5, ��.341,��.6,�.1,���������� ������� �� ����� �� ���������� ���������,���������� ������� �������,��.", "mppne02564"},
			{ 1900001753, "��.341�,��.5, ��.341,��.6,�.2, ���������� ������� �� ����� �� ���������� ���������,���������� �����,��.", "mppne02565"},
			{ 1900001754, "��.341�,��.7,�.1, ��������� ����������/����������� ����������/�������� �� ����� �� ���������������,��������� ����.����.�����", "mppne02566"},
			{ 1900001755, "��.341�,��.7,�.2, ��������� ����������/����������� ����������/�������� �� ����� �� ���������������,��������� ���/���,���������� ������� �������,��.", "mppne02567"},
			{ 1900001756, "��.341�,��.7,�.3, ��������� ����������/����������� ����������/�������� �� ����� �� ���������������,��������� �����", "mppne02568"},
			{ 1900001757, "��.356�,��.1, �������������� �������� ����� ������ ����������,��.", "mppne02570"},
			{ 1900001758, "��.356�,��.2,�.1, �������������� �������� ����� ������ ����������,��.,��������� ����.����.�����/����� �� ������ �����", "mppne02571"},
			{ 1900001759, "��.356�,��.2,�.2, �������������� �������� ����� ������ ����������,��.,���������� ������� �������", "mppne02572"},
			{ 1900001760, "��.356�,��.2,�.3, �������������� �������� ����� ������ ����������,��.,��������� �����", "mppne02573"},
			{ 1900001761, "��.356�,��.3,�.1, �������������� �������� ����� ������ ����������,��.,�� ���������������,��������� ����.����.�����/����� �� ������ �����", "mppne02574"},
			{ 1900001762, "��.356�,��.3,�.2, �������������� �������� ����� ������ ����������,��.,�� ���������������,���������� ������� �������", "mppne02575"},
			{ 1900001763, "��.356�,��.3,�.3, �������������� �������� ����� ������ ����������,��.,�� ���������������,��������� �����", "mppne02576"},
			{ 1900001764, "��.356�,��.1, �������� �� ����/������� �� ������������ ����� ��������,��.", "mppne02577"},
			{ 1900001765, "��.356�,��.2,�.1�.\"�\", ������� �� ���������� ����� ��������,��.,� ��� ����������� �����/������� �������", "mppne02578"},
			{ 1900001766, "��.356�,��.2,�.1�.\"�\", ������� �� ���������� ����� ��������,��.,� ��� ����������� �� ����.����.�����/����� �� ������ �����", "mppne02579"},
			{ 1900001767, "��.356�,��.2,�.2, ������� �� �������� ����� �������� ���� ������/������", "mppne02580"},
			{ 1900001768, "��.356�,��.2,�.3, ������� �� �������������� �������� ����� ������ ����������,� ��� �������,��.", "mppne02581"},
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
			m.printf("��� ����� �� �������� %d ���� ������������.", count);
		}
	}

#if COURT_TYPE == COURT_REGIONAL
	{
		int count = 0;

		SubpoenaKind->subpoenaKind = '�';

		if (!SubpoenaKind->Try("F_NAME") && SubpoenaKind->DataBase->Execute("INSERT INTO T_SUBPOENA_KIND VALUES ('�', "
			"'��������� �� ��.414a, ��. 3 ���', '�����.��.414a�3', 'lVW�������', '!cPsd�pZ', 'yq�', 'mcZpr', "
			"'anpmslP.L...t..G....', 'a', 0385, 'An_414a.htm', 'N', '���������� � 18� - ��������� �� ��������� "
			"���������� � �������� �� ��. 414�, ��.3 ���', '���������� ����. ��.5 ��.2 �� ������� �7/2008 "
			"�� {F_EVENT_TEXT}', NULL)", ET_ERROR))
		{
			count++;
		}

		SubpoenaKind->subpoenaKind = '�';

		if (!SubpoenaKind->Try("F_NAME") && SubpoenaKind->DataBase->Execute("INSERT INTO T_SUBPOENA_KIND VALUES ('�', "
			"'��������� �� ��.415, ��. 1, �. 3 ��� - ��', '�����.����� ��', '45689lLVWd��������qS�', '!fǥ', NULL, "
			"'�', 'anpmslPML...t..G....', 'a', 0280, 'An_415_19b.htm', 'N',  '���������� � 19� - ��������� �� ����� "
			"�� �������� �� ������� �� ���������� � �������� �� ��. 415 ���', '���������� ����. ��.5 ��.2 �� ������� "
			"�7/2008 �� {F_EVENT_TEXT}', NULL)", ET_ERROR))
		{
			count++;
		}

		if (count > 0)
		{
			m.sep("\n\n");
			m.printf("�������� �� %d ���� ��������/���������.", count);
		}

		SubpoenaKind->subpoenaKind = '�';

		if (SubpoenaKind->Get("F_INFO") && !any(SubpoenaKind->info))
		{
			strcpy(SubpoenaKind->name, "��������� �� ��.415, ��. 1, �. 2 ��� - ��");
			strcpy(SubpoenaKind->info, "���������� � 19� � ��������� �� �������� �� �������� ����. ��. 47, ��. 5 "
				"� �������� �� ��. 415 ���");
			SubpoenaKind->Update("F_NAME, F_INFO");
			m.sep("\n\n");
			m.cat("��������� �� ������� � '��������� �� ��.415, ��. 1, �. 2 ���'");
		}
	}
#endif  // REGIONAL

#if COURT_TYPE == COURT_APPEAL
	{
		Kind->kind = '�';

		if (Kind->Get("F_OTHERS") && !strchr(Kind->others, '�'))
		{
			strcach(Kind->others, '�');

			if (Kind->Update("F_OTHERS"))
			{
				m.sep("\n\n");
				m.cat("��� '������ �� �������������' � �������� ������� '������'");
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
			{ '�', 8006 },
			{ '{', 8028 },
			{ '}', 8028 },
			{ '�', 8028 },
			{ '$', 8028 },
		#endif  // APPEAL
		#if COURT_TYPE == COURT_AREAL
			{ '�', 8048 },
			{ '�', 8050 },
			{ '�', 8088 },
			{ '.', 8088 },
			{ '�', 8063 },
			{ '�', 8088 },
			{ '�', 8028 }, 
		#endif  // AREAL
		#if COURT_TYPE == COURT_REGIONAL
			{ 'J', 8048 },
			{ '�', 8028 },
			{ '�', 8006 },
			{ '�', 8048 },
			{ 'y', 8006 },
			{ 'z', 8048 },
			{ '�', 8006 },
			{ '�', 8028 },
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
			m.printf("�������� �� ���������� ������ ��� %d ���� ���������.", count);
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
				m.printf("��� ����� ������� %d � ������� ��� ������������ 1018.", sbe_ptvpke);
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
			m.printf("��� ���������� �� ����� ������� %d �� �������� %d ��������.", sbe_ptvpke, count);
		}
	}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	// 2018-11-16
	{
		Right->right = RIGHT_EPORT_RES;

		if (!Right->Try())
		{
			strcpy(Right->name, "���������� �� ��������� �� ����������� � " EPORTAL_NAME);

			if (Right->Insert())
			{
				m.sep("\n\n");
				m.cat("��� ��������������� ����� � �������� 1 ���� �������.");

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
			m.printf("��� %d ��������� � �������� ���������� �� ��������� �� ������ ������ ���������.", count);
		}
	}
#endif  // REGIONAL

#if COURT_TYPE == COURT_AREAL
	// 2018-11-20
	{
		enum { law_kind = '!' };
		SubpoenaKind->Seek('�');

		if (!strchr(SubpoenaKind->lawsuitKinds, law_kind))
		{
			strcach(SubpoenaKind->lawsuitKinds, law_kind);

			if (SubpoenaKind->Update("F_LAWSUIT_KINDS"))
			{
				m.sep("\n\n");
				m.printf("��� 1 ��������� � ������� ��� ���� %s.", Kind->Seek(law_kind));
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
			m.cat("\n\n��������� �� ������� � ��������������. ������������� ���.");
			restart = str(m);
		}

		if (check_config_dates() && !restart)
			restart = "��������� �� ������� � ��������������. ������������� ���.";

		if (restart)
			TLongTextDialog(parent, restart).Execute();
	}
#else  // TESTVER
	if (any(m))
	{
		m.cat("\n\n��������� �� ������� � ��������������. ������������� ���.");
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
			info("��������� ��������� �� %u ���� �� �������������� � ������ �� ����������.", synced);
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
			info("%d ���(�) �� �������� ��� \"������� �� WEB �������� -> �� ������� ������, ��������� �� "
				"�����������\", � ������ ���� �� �����������.", count);
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
			m.printf("%d ���(�) �� �������� �� \"������� �� WEB �������� -> �� ������� ������, ��������� �� "
				"�����������\".", count);
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
				m.printf("������� ������: ���� �� ��������� ������������ �� ������� %s "
					"�� ����������� �� ���� �� ������.", Subject->Code(0, false));
			}
		}

		if (any(m))
			message(MB_ICONWARNING | MB_OK, "%s", str(m));
	}
#endif  // TESTVER
}