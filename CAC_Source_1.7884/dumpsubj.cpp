#if TEST_DUMP_SUBJS
struct TSubjectDump
{
	char type;
	long oldSubject;
	const char *newSubjects;
};

static TISArray<TSubjectTKS> dumped_subjects;

static void dump_subject(const TSubjectDump *dump, FILE *upsb, FILE *inar, bool multiply)
{
	OldSubject->subject = dump->oldSubject;
	OldSubject->type = dump->type;

	if (!OldSubject->Try())
	{
		error("%c %ld: missing old subject", dump->type, dump->oldSubject);
		return;
	}

	long si = TSubject::SI(OldSubject->kind);
	enum { NAMES = 10 };
	static constant names[NAMES] = { "F_FLAGS", "F_ATTRIB", "F_RESULTS", "F_CLOSEDS", "F_PENALITIES",
		"F_REASONS", "F_JUDGE_REPORT", "F_QUICK_REPORT", "F_VID", "F_SBES" };

	fprintf(upsb, "UPDATE T_SUBJECT SET");

	for (int i = 0; i < NAMES; i++)
	{
		constant name = names[i];

		if (i)
			fputc(',', upsb);

		fprintf(upsb, "\n %s = (SELECT %s FROM T_OLD_SUBJECT WHERE F_SUBJECT = %ld AND F_TYPE = '%c')",
			TSubject::FNI(name, si), name, dump->oldSubject, dump->type);
	}

	fputs("\nWHERE F_SUBJECT ", upsb);

	TIntegerArray array;
	strvalue_to_lset(dump->newSubjects, &array, "dump:newSubjects");

	if (multiply)
	{
		for (int index = 0; index < array.Count(); index++)
		{
			long subject = array[index] * SUBJECT_MULTIPLY;

			array.Detach(index);
			array.Insert(subject, index);
		}
	}

	if (array.Count() == 1)
		fprintf(upsb, "= %ld", array[0]);
	else
	{
		fputs("IN (", upsb);

		for (int i = 0; i < array.Count(); i++)
		{
			if (i)
				fputs(", ", upsb);

			fprintf(upsb, "%ld", array[i]);
		}

		fputc(')', upsb);
	}

	fprintf(upsb, " AND F_TYPE = '%c';\n\n", dump->type);

	for (int i = 0; i < array.Count(); i++)
	{
		long newSubject = array[i];
		TSubjectTKS *stks = new TSubjectTKS;

		stks->subject = newSubject;
		stks->tork = si ? toupper(dump->type) : dump->type;

		if (dumped_subjects.Find(stks) == INT_MAX)
			dumped_subjects.Add(stks);
		else
		{
			delete stks;
			error("new %c %ld is re-targeted", dump->type, newSubject);
			return;
		}

		Subject->subject = newSubject;
		Subject->type = dump->type;

		if (!Subject->Try())
		{
			error("%c %ld: missing new subject", dump->type, newSubject);
			return;
		}

		char newKind = Subject->sd[si].kind;

		if (!newKind)
		{
			error("%c %ld: empty new subject kind", dump->type, newSubject);
			return;
		}

		// 2016:062 LPR: not using update-time database subjects
		fprintf(inar, "INSERT INTO T_ARCHIVE_SUBJECT (F_ARCHIVE_INDEX, F_SUBJECT, F_KIND) "
			"SELECT F_ARCHIVE_INDEX, %ld, '%c'\nFROM T_OLD_ARCHIVE_SUBJECT "
			"WHERE F_SUBJECT = %ld AND F_TYPE = '%c';\n", newSubject, newKind, dump->oldSubject,
			dump->type);

		if (!strcmp(C2S[newKind], KIND_ARTICLE_LAWS))
		{
			fprintf(inar, "INSERT INTO T_ARTICLE_SUBJECT (F_ARTICLE, F_SUBJECT) SELECT F_ARTICLE, %ld "
				"FROM T_OLD_ARTICLE_SUBJECT WHERE F_SUBJECT = %ld;\n", newSubject, dump->oldSubject);
		}

		if (!strcmp(C2S[newKind], KIND_DANGER_LAWS))
		{
			fprintf(inar, "INSERT INTO T_DANGER_SUBJECT (F_DANGER, F_SUBJECT) SELECT F_DANGER, %ld "
				"FROM T_OLD_DANGER_SUBJECT WHERE F_SUBJECT = %ld;\n", newSubject, dump->oldSubject);
		}

		fputc('\n', inar);
	}
}

static void test_dump_subjs()
{
	// ----- TYPE_ -------------------------------------------------------------
	// ----- SUBJECT_ ----------------------------------------------------------
	static const TSubjectDump Dumps[] =
	{
	#if COURT_TYPE == COURT_APPEAL
		{ 'p', 9005, "2000" },
		{ 'p', 9000, "50000" },	// 2016:099 was 5000
		{ 'p', 9400, "6000" },
		{ 'p', 9004, "7000, 7010, 7020, 7030, 7040, 7050, 7060, 7070, 7080, 7090" },
		{ 'p', 9004, "7100, 7110, 7120, 7130, 7140, 7150, 7160, 7170, 7180, 7190, 7200, 7300" },
		{ 'p', 9004, "8000, 8040, 8050, 8060, 8070, 8080, 8090" },
		{ 'p', 9002, "8010" },
		{ 'p', 9003, "8020" },
		{ 'p', 9005, "8030" },
		{ 'p', 9200, "90000" },	// 2016:099 was 9000
		{ 'p', 9600, "5900" },
		{ 'p', 9700, "7900" },
		{ 'c', 9000, "100, 101, 106...110, 113, 114, 200...204, 206, 207, 209...213, 215, 218" },
		{ 'c', 9000, "300, 301, 303, 400, 600, 602...604, 900, 1300, 1500, 1600" },
		{ 'c', 9100, "903, 904, 1000, 1007, 1100, 1400, 1700" },
		{ 't', 9000, "2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 21000" },
		{ 't', 9050, "21100, 21110, 21120, 21130" },
		{ 'c', 9050, "21100" },			// ext
		{ 't', 9000, "21200, 21300, 21400, 21500, 21600, 21700, 21800, 21900" },
		{ 't', 9000, "22100, 22110, 22120, 22130, 22140, 22200" },
		{ 't', 9080, "22210, 22300, 22310, 22400, 22410, 22600, 22610" },
		{ 't', 9100, "22800, 22820, 22830, 22840, 22900" },
		{ 'c', 9100, "22800" },			// ext
		{ 'a', 9500, "1300" },			// ext
		{ 'a', 9550, "1400" },			// ext
		{ 'd', 9500, "22900" },			// ext
		{ 'd', 9550, "22800" },			// ext
		// 2016:098 50k W <- O, 90k � <- (partial) D
		{ 'p', 9000, "50100...50114, 50200...50216, 50300...50312" },
		{ 'p', 9000, "50400...50447, 50500...50510, 50600...50607" },
		{ 'p', 9000, "50700...50741, 50800...50842, 50900...50941" },
		{ 'p', 9000, "51000...51007, 51100, 51200...51212, 51300...51340" },	// 2016:099 was 51339
		{ 'p', 9000, "51400...51402, 51500...51505, 51600...51603" },
		{ 'p', 9200, "94350, 94360, 94370, 94380, 94381" },
		// 2016:280 from appc_news_6975.sql
		{ 'p', 9400, "6010, 6020" },
		{ 'c', 9100, "1014" },
		{ 't', 9000, "21140" },
		{ 't', 9100, "22860, 22870" },
		{ 't', 9000, "23000" },
		// 2016:356 VSS changes from 20.12
		{ 'p', 9400, "6030, 6040, 6050" },
		{ 'p', 9004, "7220, 7230" },
		// 2016:357 VSS changes from 20.12
		{ 'c', 9000, "117, 118, 219" },
		{ 'c', 9100, "1015" },
		{ 't', 9100, "1015, 1800" },
		{ 'c', 9000, "1900" },
		{ 'p', 9004, "7210" },		// 2017:004 VSS changes from 20/29.12
		{ 'p', 9000, "51213" },		// 2017:023 IRQ/VSS
		{ 't', 9100, "22871" },		// 2017:198 IRQ/VSS, from update_mini
		{ 't', 9100, "24100" },		// 2017:198 IRQ/VSS, from update_mini
		{ 't', 9100, "24200" },		// 2017:198 IRQ/VSS, from update_mini
	#endif  // APPEAL
	#if COURT_TYPE == COURT_ADMIN
		{ 'a', 200, "100, 110, 120, 130, 140, 150, 160" },
		{ 'a', 200, "170" },			// 2016:086 was 210
		{ 'a', 840, "180" },
		{ 'a', 840, "190" },			// 2016:200
		{ 'a', 500, "200" },
		{ 'a', 520, "210, 220, 230" },
		{ 'a', 900, "240, 250" },		// 2016:086 was 930
		{ 'a', 900, "300, 310, 320, 330" },
		{ 'a', 620, "400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 491, 492, 493" },
		{ 'a', 1000, "500" },
		{ 'a', 900, "600, 800, 900, 920, 2400" },
		{ 'a', 900, "700" },			// 2016:086 was 910
		{ 'a', 900, "930" },			// 2016:200
		{ 'a', 800, "910" },
		{ 'a', 150, "1000" },
		{ 'a', 420, "1010, 1020, 1030, 1040, 1050, 1060, 1070, 1080, 1090" },
		{ 'a', 400, "1100, 1200" },
		{ 'a', 1100, "1300" },
		{ 'a', 600, "1400, 1410, 1420, 1430, 1440, 1450, 1500, 1510, 1520, 1530, 1540, 1550" },
		{ 'a', 600, "1600, 1610, 1620, 1630, 1640" },
		{ 'a', 730, "1700, 2000, 2100, 2200" },
		{ 'a', 700, "2300" },
		{ 'a', 300, "3000, 3100, 3200" },
		{ 'a', 1200, "4000" },
		{ 'a', 3200, "4100, 4200, 4300" },
		{ 'a', 3100, "4400" },			// 2016:200
		{ 't', 3100, "4400" },			// 2016:200
		{ 'a', 3100, "5000" },
		{ 't', 3100, "5000" },
		{ 'a', 3100, "5100, 5110, 5200, 5210, 5220, 5230, 5300, 5310, 5320, 5330, 5340" },
		{ 't', 3100, "5100, 5110, 5200, 5210, 5220, 5230, 5300, 5310, 5320, 5330, 5340" },
		{ 'a', 3100, "5350, 5360, 5370, 5380, 5390, 5391, 5392, 5393, 5400, 5410, 5420" },
		{ 't', 3100, "5350, 5360, 5370, 5380, 5390, 5391, 5392, 5393, 5400, 5410, 5420" },
		{ 'a', 3100, "5430, 5440, 5450, 5460, 5461, 5462, 5463, 5464, 5470, 5480, 5500" },
		{ 't', 3100, "5430, 5440, 5450, 5460, 5461, 5462, 5463, 5464, 5470, 5480, 5500" },
		{ 'a', 3150, "5600" },
		{ 't', 3150, "5600" },
		{ 'c', 100, "600" },
		{ 'c', 120, "920" },
		{ 'c', 150, "1000" },
		{ 't', 980, "2400" },
		// 2016:316 VSS
		{ 'a', 900, "260, 340, 810" },
		{ 'a', 600, "1560, 1650, 1660, 1670" },
		// 2016:357 VSS changes from 20.12
		{ 'a', 3100, "5394" },
		{ 't', 3100, "5394" },
		// 2017:178 VSS changes from 19.06
		{ 'a', 900, "610" },
		{ 'a', 3100, "4500" },
		{ 't', 3100, "4500" },
		{ 'a', 1200, "4100" },		// 2017:229 +4100
	#endif  // ADMIN
	#if COURT_TYPE == COURT_AREAL
		{ 'p', 100, "100...112, 114" },
		{ 'p', 101, "113" },
		{ 'p', 200, "200, 205...209, 211...216, 300...312, 400...406, 416...424, 426...430, 432...447" },
		{ 'p', 200, "500...510, 600...607" },
		{ 'p', 201, "201...203" },
		{ 'p', 202, "204" },
		{ 'p', 203, "210" },
		{ 'p', 204, "407...415" },
		{ 'p', 206, "425" },
		{ 'p', 207, "431" },
		{ 'p', 500, "700...705, 707, 708, 711...714, 716...719, 721...728, 730...734, 737...741" },
		{ 'p', 504, "706" },
		{ 'p', 505, "709, 710" },
		{ 'p', 507, "715, 720" },
		{ 'p', 511, "729" },
		{ 'p', 513, "735, 736" },
		{ 'p', 600, "800...808, 810...821, 824, 825, 833, 834" },
		{ 'p', 601, "809" },
		{ 'p', 602, "822, 823" },
		{ 'p', 603, "826...832" },
		{ 'p', 700, "835, 838, 840...842" },
		{ 'p', 701, "836" },
		{ 'p', 702, "837" },
		{ 'p', 703, "839" },
		{ 'p', 800, "900...912, 918...931, 937" },
		{ 'p', 801, "913...917" },
		{ 'p', 802, "932...934" },
		{ 'p', 803, "935" },
		{ 'p', 804, "936" },
		{ 'p', 806, "938, 939, 941" },
		{ 'p', 805, "940" },
		{ 'p', 900, "1000...1007" },
		{ 'p', 910, "1100" },
		{ 'p', 1000, "1200, 1201, 1204...1212" },
		{ 'p', 1002, "1202, 1203" },
		{ 'p', 1100, "1300, 1301, 1303...1310, 1312, 1314, 1316...1331, 1335, 1337...1340" },  // 2016:099 was 1339
		{ 'p', 1101, "1302" },
		{ 'p', 1102, "1311" },
		{ 'p', 1104, "1313" },
		{ 'p', 1105, "1315" },
		{ 'p', 1106, "1332...1334, 1336" },
		{ 'p', 1200, "1400, 1402, 1500...1505" },
		{ 'p', 1201, "1401" },
		{ 'p', 1300, "1600...1603" },
		{ 'p', 1300, "1700" },			// 2016:055 was 1604
		{ 'p', 1800, "2000, 2110" },
		{ 'p', 1400, "2120, 2130" },
		{ 'p', 1858, "2200" },
		{ 'p', 1855, "2210" },
		{ 'p', 1850, "2220, 2230, 2240, 2330, 2340, 2410, 2420, 2430" },
		{ 'p', 1850, "2500, 2510, 2520, 2540, 2550, 2560, 2570, 2580" },
		{ 'p', 1853, "2300" },
		{ 'p', 1856, "2310" },
		{ 'p', 1859, "2320" },
		{ 'p', 1500, "2400" },
		{ 'p', 1700, "2530" },
		{ 'p', 1900, "3000, 3080, 3140...3142" },	// -3150; 2016:099 ...3142
		{ 'p', 1910, "3010" },
		{ 'p', 1932, "3020" },
		{ 'p', 1920, "3030" },
		{ 'p', 1905, "3040" },
		{ 'p', 1933, "3050" },
		{ 'p', 1980, "3060" },
		{ 'p', 1941, "3070" },
		{ 'p', 1930, "3090" },
		{ 'p', 1950, "3100" },
		{ 'p', 1960, "3110" },
		{ 'p', 1963, "3120" },
		{ 'p', 1962, "3130" },
		{ 'p', 2000, "4000, 4010, 4020, 4030, 4040, 4050, 4060, 4070, 4080, 4090" },
		{ 'p', 2000, "4100, 4110, 4120, 4130, 4140, 4150, 4160, 4170, 4180, 4190" },
		{ 'p', 2000, "4200, 4210, 4220, 4230, 4240, 4250, 4260, 4270, 4280, 4290" },
		{ 'p', 2000, "4300, 4310, 4320, 4330, 4340, 4350, 4360, 4370, 4380, 4381" },
		{ 'p', 3400, "4390" },		// 2016:055
		{ 'p', 9000, "50000" },		// 2016:098 was 5000
		{ 'p', 9100, "6000" },
		{ 'p', 9200, "7000, 7010, 7020, 7030, 7040, 7050, 7060, 7070, 7080, 7090" },
		{ 'p', 9200, "7100, 7110, 7120, 7130, 7140, 7150, 7160, 7170, 7180, 7190, 7200, 7300" },
		{ 'p', 9201, "8000, 8010, 8040, 8050, 8060, 8070, 8080" },
		{ 'p', 9202, "8020" },
		{ 'p', 9205, "8030" },
		{ 'p', 9204, "8090" },
		{ 'p', 9500, "90000" },		// 2015:095 was 9000
		{ 'p', 1911, "3150" },
		{ 'c', 100, "100, 101, 108, 114" },
		{ 't', 200, "114" },		// 2016:100
		{ 't', 9000, "114" },		// 2016:100
		{ 'c', 110, "106" },
		{ 'c', 140, "107" },
		{ 'c', 120, "109" },
		{ 'c', 130, "110" },
		{ 'c', 814, "113" },
		{ 't', 814, "113" },			// ext
		{ 'c', 9000, "100...114" },
		{ 'c', 200, "200...202, 204, 206, 207, 209...213, 215" },
		{ 'c', 200, "208, 214" },		// 2016:095
		{ 't', 200, "200" },			// ext
		{ 't', 200, "212" },			// 2016:092
		{ 'c', 220, "203" },
		{ 't', 220, "203" },			// ext
		{ 'c', 800, "218" },
		{ 'c', 9000, "200...218" },
		{ 'c', 300, "300, 301, 303" },
		{ 'c', 300, "306" },			// 2016:095
		{ 't', 300, "300" },			// ext
		{ 'c', 9000, "300...306" },
		{ 'c', 420, "400" },
		{ 't', 420, "400" },			// ext
		{ 'c', 9000, "400, 500...505" },
		{ 'c', 800, "600, 602... 604" },
		{ 'c', 9000, "600...604" },
		{ 'c', 400, "700, 701" },		// 2016:092
		{ 'c', 9000, "700" },
		{ 'c', 9000, "701...708, 800" },
		{ 'a', 900, "800" },			// 2016:055
		{ 'a', 3300, "800" },			// 2016:055
		{ 'c', 810, "900, 903" },
		{ 't', 810, "903" },			// ext
		{ 'c', 810, "904" },
		{ 'c', 9001, "900, 901, 903, 904" },
		{ 'c', 815, "1000" },
		{ 'c', 815, "1007" },
		{ 'c', 813, "1008" },
		{ 't', 813, "1008" },			// ext
		{ 'c', 815, "1009, 1010" },
		{ 'c', 811, "1011" },
		{ 't', 811, "1011" },			// ext
		{ 'c', 809, "1012" },
		{ 't', 809, "1012" },			// ext
		{ 'c', 815, "1013" },
		{ 'c', 9001, "1000, 1002...1005, 1007" },
		{ 'c', 815, "1100, 1103, 1104" },
		{ 'c', 9001, "1103, 1104, 1013" },	// 2016:095
		{ 'c', 9005, "1100" },
		{ 't', 9005, "1100" },			// ext
		{ 'c', 9001, "1101, 1102" },
		{ 'c', 9000, "1200" },
		{ 'c', 800, "1300" },
		{ 'c', 9000, "1300" },
		{ 'c', 9001, "1400" },
		{ 'c', 9000, "1500, 1600" },
		{ 'c', 9040, "1700" },
		{ 't', 9040, "1700" },			// ext
		{ 't', 600, "2100" },
		{ 'c', 600, "2100" },			// ext
		{ 't', 200, "2200" },
		{ 't', 700, "2300" },
		{ 'c', 700, "2300" },			// ext
		{ 't', 610, "2400, 2500, 2900, 21000" },
		{ 'c', 610, "2400" },			// ext
		{ 't', 620, "2600, 2700" },
		{ 'c', 620, "2600" },			// ext
		{ 't', 640, "2800" },
		{ 'c', 640, "2800" },
		{ 'c', 9000, "2800" },
		//{ 't', 640, "2890" },		// 2016:055; 2016:113 comout
		//{ 'c', 640, "2890" },		// 2016:055; 2016:113 comout
		{ 't', 9000, "2100, 2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900, 21000" },
		{ 't', 500, "21100, 21110, 21120, 21130" },
		{ 'c', 500, "21100" },			// ext
		{ 't', 600, "21200, 21400" },
		{ 't', 650, "21300" },
		{ 'c', 650, "21300" },			// ext
		{ 't', 800, "21500, 21600, 21700, 21800, 21900" },
		{ 't', 9000, "21100, 21110, 21120, 21130, 21200, 21300, 21400, 21500, 21600, 21700, 21800, 21900" },
		{ 't', 800, "22000, 22100, 22110, 22120, 22130, 22140" },
		{ 't', 9000, "22100, 22110, 22120, 22130, 22140" },
		{ 'd', 1700, "22200, 22210, 22300, 22310, 22500, 22510" },
		{ 'd', 1720, "22400, 22410" },
		{ 'd', 2080, "22600, 22610" },
		{ 'd', 2000, "22700, 22710" },
		{ 't', 815, "22800, 22810, 22820, 22830, 22840, 22850" },
		{ 't', 9001, "22800, 22820, 22830, 22840" },
		{ 't', 800, "22900" },
		{ 't', 9000, "22900" },
		{ 'd', 1100, "22900" },
		// 2016:096 +II instance
		{ 't', 9000, "200, 203, 212, 300, 400" },
		{ 't', 9001, "903" },
		// 2016:096 +I instance
		{ 't', 815, "1100" },
		// 2016:096 -> X 3
		{ 't', 200, "201, 202, 204, 206, 207, 208, 209, 210, 211, 213, 214, 215" },
		{ 't', 9000, "201, 202, 204, 205, 206, 207, 208, 209, 210, 211, 213...218" },
		{ 't', 800, "218, 600, 602, 603, 604, 1300" },
		{ 't', 300, "301, 303, 306" },
		{ 't', 9000, "301, 302, 303, 304, 305, 306, 500...505, 600...604, 700, 701...708, 800" },
		{ 't', 400, "700, 701" },
		{ 't', 810, "900, 904" },
		{ 't', 9001, "900, 901, 904, 1000, 1002...1005, 1007, 1013, 1101, 1102, 1103, 1104, 1400" },
		{ 't', 815, "1000, 1007, 1009, 1010, 1013, 1103, 1104" },
		{ 't', 9000, "1200, 1300, 1500, 1600" },
		// 2016:098 50k W <- O, 90k � <- (partial) D
		{ 'p', 9000, "50100...50114, 50200...50216, 50300...50312" },
		{ 'p', 9000, "50400...50447, 50500...50510, 50600...50607" },
		{ 'p', 9000, "50700...50741, 50800...50842, 50900...50941" },
		{ 'p', 9000, "51000...51007, 51100, 51200...51212, 51300...51340" },	// 2016:099 was 51339
		{ 'p', 9000, "51400...51402, 51500...51505, 51600...51603" },
		{ 'p', 9500, "94350, 94360, 94370, 94380, 94381" },
		{ 'p', 1900, "3061" },		// 2016:182
		// 2016:278 from oc_news_6975.sql
		{ 'p', 1300, "1710, 1720" },
		{ 'c', 815, "1014" },
		{ 'c', 9001, "1014" },
		{ 'p', 9100, "6010, 6020" },
		{ 't', 9000, "21140" },
		{ 't', 9001, "22860" },
		{ 't', 815, "22870" },
		{ 't', 9001, "22870" },
		{ 'd', 1100, "23000" },
		// 2016:356 VSS changes from 20.12
		{ 'p', 1300, "1730, 1740, 1750" },
		{ 'p', 1850, "2590" },
		{ 'p', 9100, "6030, 6040, 6050" },
		{ 'p', 9200, "7220, 7230" },
		// 2016:357 VSS changes from 20.12
		{ 'c', 9000, "115, 116, 118, 219, 1900" },
		{ 'c', 100, "117" },
		{ 'c', 200, "219" },
		{ 'c', 9001, "1006, 1015, 1016, 1017" },
		{ 'c', 811, "1015" },
		{ 't', 9001, "1015" },
		{ 't', 811, "1015" },
		{ 'p', 9200, "7210" },		// 2017:004 VSS changes from 20/29.12
		{ 'p', 1000, "1213" },		// 2017:023 IRQ/VSS
		{ 'p', 9000, "51213" },		// 2017:023 IRQ/VSS
		{ 't', 815, "24100" },		// 2017:198 IRQ/VSS, from update_mini
		{ 't', 815, "24200" },		// 2017:198 IRQ/VSS, from update_mini
		{ 'c', 800, "22110" },		// 2017:324 URQ/VSS
	#endif  // AREAL
	#if COURT_TYPE == COURT_MILITARY
	#if SPECIAL
		{ 'p', 100, "100...112, 114" },
		{ 'p', 101, "113" },
		{ 'p', 200, "200, 205...209, 211...216, 300...304, 306...312, 400...406, 416...418, 420...427, 428" },
		{ 'p', 200, "432...434, 436, 437, 441...443, 447" },
		{ 'p', 201, "201...203" },
		{ 'p', 202, "204" },
		{ 'p', 203, "210, 305" },
		{ 'p', 204, "407...411, 413...415, 438...440" },
		{ 'p', 205, "412" },
		{ 'p', 207, "419, 429...431" },
		{ 'p', 208, "435" },
		{ 'p', 206, "444...446" },
		{ 'p', 300, "500...510" },
		{ 'p', 400, "600...607" },
		{ 'p', 500, "700, 723...725, 732, 739...741" },
		{ 'p', 504, "701...707" },
		{ 'p', 505, "708...711" },
		{ 'p', 507, "712...722" },
		{ 'p', 511, "726...731" },
		{ 'p', 513, "733...738" },
		{ 'p', 600, "800...806, 810...819, 821" },
		{ 'p', 601, "807...809" },
		{ 'p', 602, "820, 822...825" },
		{ 'p', 603, "826...834" },
		{ 'p', 700, "835, 838, 840...842" },
		{ 'p', 701, "836" },
		{ 'p', 702, "837" },
		{ 'p', 703, "839" },
		{ 'p', 800, "900...910, 912, 919...931" },
		{ 'p', 801, "911" },
		{ 'p', 802, "913...918" },
		{ 'p', 806, "932...936, 939, 941" },
		{ 'p', 802, "937" },
		{ 'p', 804, "938" },
		{ 'p', 805, "940" },
		{ 'p', 900, "1000...1007" },
		{ 'p', 910, "1100" },
		{ 'p', 1000, "1200, 1201, 1204...1212" },
		{ 'p', 1002, "1202, 1203" },
		{ 'p', 1100, "1300, 1301, 1303...1305, 1307...1312, 1314...1320, 1322...1331, 1339" },
		{ 'p', 1100, "1340" },	// 2016:099
		{ 'p', 1101, "1302" },
		{ 'p', 1102, "1306" },
		{ 'p', 1104, "1313" },
		{ 'p', 1105, "1321" },
		{ 'p', 1106, "1332...1337" },
		{ 'p', 1107, "1338" },
		{ 'p', 1200, "1400, 1402" },
		{ 'p', 1202, "1401" },
		{ 'p', 1300, "1500...1505, 1600...1603, 1700" },	// 2016:055 was 1604
		{ 'p', 2300, "2000, 2220, 2230, 2240, 2320, 2330, 2410, 2420, 2430" },
		{ 'p', 2300, "2500, 2510, 2520, 2530, 2540, 2550, 2560, 2570, 2580" },
		{ 'p', 2300, "2340" },
		{ 'p', 1800, "2110" },
		{ 'p', 2206, "2120" },
		{ 'p', 1400, "2130" },
		{ 'p', 2306, "2200" },
		{ 'p', 2305, "2210" },
		{ 'p', 2303, "2300" },
		{ 'p', 2304, "2310" },
		{ 'p', 1500, "2400" },
		{ 'p', 2200, "3000, 3080, 3140...3142, 3150" },	// 2016:099 ...3142
		{ 'p', 2204, "3010" },
		{ 'p', 2205, "3020" },
		{ 'p', 2216, "3030" },
		{ 'p', 2218, "3040, 3050" },
		{ 'p', 2213, "3060" },
		{ 'p', 2201, "3070" },
		{ 'p', 2222, "3090" },
		{ 'p', 2207, "3100" },
		{ 'p', 2208, "3110" },
		{ 'p', 2209, "3120" },
		{ 'p', 2210, "3130" },
		{ 'p', 2000, "4000, 4380, 4381" },
		{ 'p', 3400, "4390" },			// 2016:055
		{ 'p', 1900, "4350" },
		{ 'p', 9000, "50000" },	// 2016:099 was 5000
		{ 'p', 9100, "6000" },
		{ 'p', 9200, "7000, 7010, 7020, 7030, 7040, 7050, 7060, 7070, 7080, 7090" },
		{ 'p', 9200, "7100, 7110, 7120, 7130, 7140, 7150, 7160, 7170, 7180, 7190, 7200, 7300" },
		{ 'p', 9201, "8000, 8010, 8040, 8050, 8060, 8070, 8080" },
		{ 'p', 9202, "8020" },
		{ 'p', 9205, "8030" },
		{ 'p', 9204, "8090" },
		{ 'p', 9500, "90000" },	// 2016:099 was 9000
	#else  // SPECIAL
		{ 'p', 100, "100...112, 114" },
		{ 'p', 101, "113" },
		{ 'p', 200, "200, 205...209, 211...216, 300...312, 400...406, 416...427, 432...437, 441...443, 447" },
		{ 'p', 201, "201...203" },
		{ 'p', 202, "204" },
		{ 'p', 203, "210" },
		{ 'p', 204, "407...415, 438...440" },
		{ 'p', 207, "428...431" },
		{ 'p', 205, "444...446" },
		{ 'p', 300, "500...510" },
		{ 'p', 400, "600...607" },
		{ 'p', 500, "700, 723...725, 732, 739...741" },
		{ 'p', 504, "701...707" },
		{ 'p', 505, "708...711" },
		{ 'p', 507, "712...722" },
		{ 'p', 511, "726...731" },
		{ 'p', 513, "733...738" },
		{ 'p', 600, "800...806, 810...821" },
		{ 'p', 601, "807...809" },
		{ 'p', 602, "822...825" },
		{ 'p', 603, "826...834" },
		{ 'p', 700, "835, 838, 840...842" },
		{ 'p', 701, "836" },
		{ 'p', 702, "837" },
		{ 'p', 703, "839" },
		{ 'p', 800, "900...912, 919...931" },
		{ 'p', 801, "913...918" },
		{ 'p', 806, "932...936, 939, 941" },
		{ 'p', 802, "937" },
		{ 'p', 804, "938" },
		{ 'p', 805, "940" },
		{ 'p', 900, "1000...1007" },
		{ 'p', 910, "1100" },
		{ 'p', 1000, "1200, 1201, 1204...1212" },
		{ 'p', 1002, "1202, 1203" },
		{ 'p', 1100, "1300, 1301, 1303...1310, 1312, 1314, 1316...1331, 1338...1340" },  // 2016:099 was 1339
		{ 'p', 1101, "1302" },
		{ 'p', 1102, "1311" },
		{ 'p', 1104, "1313" },
		{ 'p', 1105, "1315" },
		{ 'p', 1106, "1332...1337" },
		{ 'p', 1200, "1400, 1402" },
		{ 'p', 1202, "1401" },
		{ 'p', 1300, "1500...1505, 1600...1603" },
		{ 'p', 2100, "1700" },					// 2015:055 was 1604
		{ 'p', 2300, "2000, 2220, 2230, 2240, 2320, 2330, 2410, 2420, 2430" },
		{ 'p', 2300, "2500, 2510, 2520, 2530, 2540, 2550, 2560, 2570, 2580" },
		{ 'p', 2300, "2340" },
		{ 'p', 1800, "2110" },
		{ 'p', 2206, "2120" },
		{ 'p', 1400, "2130" },
		{ 'p', 2306, "2200" },
		{ 'p', 2305, "2210" },
		{ 'p', 2303, "2300" },
		{ 'p', 2304, "2310" },
		{ 'p', 1500, "2400" },
		{ 'p', 2200, "3000, 3070, 3080, 3090, 3140...3142, 3150" },	// 2016:099 ...3142
		{ 'p', 2204, "3010" },
		{ 'p', 2205, "3020" },
		{ 'p', 2216, "3030" },
		{ 'p', 2218, "3040, 3050" },
		{ 'p', 2213, "3060" },
		{ 'p', 2207, "3100" },
		{ 'p', 2208, "3110" },
		{ 'p', 2209, "3120" },
		{ 'p', 2210, "3130" },
		{ 'p', 2000, "4000, 4380, 4381" },
		{ 'p', 3400, "4390" },			// 2016:055
		{ 'p', 1900, "4350" },
		{ 'p', 9000, "50000" },		// 2016:099 was 5000
		{ 'p', 9100, "6000" },
		{ 'p', 9200, "7000, 7010, 7020, 7030, 7040, 7050, 7060, 7070, 7080, 7090" },
		{ 'p', 9200, "7100, 7110, 7120, 7130, 7140, 7150, 7160, 7170, 7180, 7190, 7200, 7300" },
		{ 'p', 9201, "8000, 8010, 8030, 8040, 8050, 8060, 8070, 8080" },
		{ 'p', 9202, "8020" },
		{ 'p', 9204, "8090" },
		{ 'p', 9500, "90000" },		// 2016:099 was 9000
	#endif  // SPECIAL
		// 2016:099 50k W <- O, 90k � <- (partial) D
		{ 'p', 9000, "50100...50114, 50200...50216, 50300...50312" },
		{ 'p', 9000, "50400...50447, 50500...50510, 50600...50607" },
		{ 'p', 9000, "50700...50741, 50800...50842, 50900...50941" },
		{ 'p', 9000, "51000...51007, 51100, 51200...51212, 51300...51340" },	// 2016:099 was 51339
		{ 'p', 9000, "51400...51402, 51500...51505, 51600...51603" },
		{ 'p', 9500, "94350, 94380, 94381" },
		{ 'p', 2200, "3061" },		// 2016:182
		{ 'p', 1300, "1710, 1720" },	// 2016:203
		// 2016:356 VSS changes from 20.12
		{ 'p', 2100, "1730, 1740, 1750" },
		{ 'p', 2300, "2590" },
		{ 'p', 1000, "1213" },		// 2017:023 IRQ/VSS
		{ 'p', 9000, "51213" },		// 2017:023 IRQ/VSS
	#endif  // MILITARY
	#if COURT_TYPE == COURT_REGIONAL
		{ 'p', 200, "100...114, 200...216, 300, 301, 303...312, 400...410, 416...423, 426, 427, 431...434" },
		{ 'p', 200, "436, 437, 441, 442, 447" },
		{ 'p', 204, "302" },
		{ 'p', 205, "411...415" },
		{ 'p', 206, "424, 425" },
		{ 'p', 207, "428...430" },
		{ 'p', 208, "435" },
		{ 'p', 209, "438...440" },
		{ 'p', 201, "443" },
		{ 'p', 202, "444...446" },
		{ 'p', 300, "500...505, 509, 510" },
		{ 'p', 301, "506, 507" },
		{ 'p', 302, "508" },
		{ 'p', 400, "600...603, 605...607" },
		{ 'p', 401, "604" },
		{ 'p', 500, "700, 706, 715, 720, 729, 737...741" },
		{ 'p', 501, "701...705, 707" },
		{ 'p', 505, "708...711" },
		{ 'p', 506, "712...714, 716...719, 721, 722" },
		{ 'p', 507, "723...725" },
		{ 'p', 511, "726...728, 730, 731" },
		{ 'p', 512, "732" },
		{ 'p', 514, "733...736" },
		{ 'p', 600, "800...808, 810, 811, 816, 825" },
		{ 'p', 601, "809" },
		{ 'p', 604, "812...815, 817, 818" },
		{ 'p', 602, "819...821, 824" },
		{ 'p', 603, "822, 823" },
		{ 'p', 605, "826...834" },
		{ 'p', 700, "835...842" },
		{ 'p', 800, "900...909, 912...936" },
		{ 'p', 802, "910" },
		{ 'p', 803, "911" },
		{ 'p', 804, "937" },
		{ 'p', 807, "938...941" },
		{ 'p', 900, "1000...1007" },
		{ 'p', 910, "1100" },
		{ 'p', 1000, "1200...1205, 1208...1211" },
		{ 'p', 1005, "1206, 1207" },
		{ 'p', 1004, "1212" },
		{ 'p', 1100, "1300...1311, 1319, 1320, 1322...1332, 1334...1340" },  // 2016:099 was 1339
		{ 'p', 1103, "1312...1318" },
		{ 'p', 1106, "1321" },
		{ 'p', 1107, "1333" },
		{ 'p', 1200, "1400...1402" },
		{ 'p', 1250, "1500...1505" },
		{ 'p', 1300, "1600...1603" },
		{ 'p', 1400, "1700" },		// 2016:055 was 1604
		{ 'p', 1490, "2000, 2120, 2200, 2210, 2220, 2230, 2240, 2300, 2310, 2320, 2330, 3050" },
		{ 'p', 1490, "2340" },
		{ 'p', 1490, "2520, 2540, 2550, 2560, 2570, 2580" },
		{ 'p', 1470, "2110" },
		{ 'p', 1439, "2130" },
		{ 'p', 1450, "2400" },
		{ 'p', 1460, "2410, 2420" },
		{ 'p', 1491, "2430" },
		{ 'p', 1430, "2500, 2510, 2530" },
		{ 'p', 1480, "3000, 3070, 3080, 3090, 3100, 3110, 3120, 3130, 3140...3142, 3150" },  // 2016:099 ...3142
		{ 'p', 1481, "3010" },
		{ 'p', 1482, "3020" },
		{ 'p', 1486, "3030" },
		{ 'p', 1488, "3040" },
		{ 'p', 1484, "3060" },
		{ 'p', 1510, "4385" },		// 2016:091 ext
		{ 'p', 2000, "4000, 4020, 4030, 4080, 4090, 4100, 4110, 4130, 4150, 4160, 4170, 4190, 4200, 4220" },
		{ 'p', 2000, "4230, 4240, 4250, 4260, 4280, 4290, 4300, 4320, 4340, 4370, 4380, 4381" },
		{ 'p', 2003, "4010" },
		{ 'p', 2002, "4040, 4050" },
		{ 'p', 2006, "4060" },
		{ 'p', 2004, "4070" },
		{ 'p', 2013, "4120" },
		{ 'p', 2016, "4140" },
		{ 'p', 2005, "4180" },
		{ 'p', 2011, "4210" },
		{ 'p', 2014, "4270" },
		{ 'p', 2010, "4310" },
		{ 'p', 2001, "4330" },
		{ 'p', 1410, "4350" },
		{ 'p', 2015, "4360" },
		{ 'c', 100, "100, 101, 104" },
		{ 'm', 100, "100" },			// ext
		{ 'c', 110, "102" },
		{ 'm', 110, "102" },			// ext
		{ 'c', 120, "103" },
		{ 'm', 120, "103" },			// ext
		{ 'c', 130, "105" },
		{ 'm', 130, "105" },			// ext
		{ 'c', 801, "111" },
		{ 'c', 803, "112" },
		{ 'c', 200, "200, 202, 204, 206...208, 210...214, 216, 217" },
		{ 'c', 210, "201" },
		{ 'c', 220, "203" },
		{ 'c', 300, "300" },
		{ 'c', 802, "205" },
		{ 'c', 300, "303...306" },
		{ 'c', 310, "302" },
		{ 'c', 400, "500...505" },
		{ 'm', 400, "500" },			// ext; 2016:061 was 501
		{ 'c', 600, "600...601, 604" },
		{ 'c', 500, "700, 701, 703...705, 707, 708" },
		{ 'c', 510, "702" },
		{ 'c', 520, "706" },
		{ 'c', 900, "800" },
		{ 'a', 900, "800" },			// ext
		{ 'c', 880, "900...902" },
		{ 'c', 840, "1000" },
		{ 'a', 990, "1000" },			// ext
		{ 'c', 885, "1001...1005" },
		{ 'c', 881, "1006" },
		{ 'c', 1000, "1100" },
		{ 'c', 1061, "1101" },
		{ 'c', 1062, "1102" },
		{ 'c', 809, "1200" },
		{ 'c', 806, "1201" },
		{ 'c', 807, "1202" },
		{ 'c', 808, "1203, 1204" },
		{ 'm', 800, "1300" },			// ext
		{ 'c', 800, "1300" },
		{ 'm', 100, "101, 104, 112" },	// 2016:105 ext
		{ 'm', 800, "111" },			// 2016:105 ext
		{ 'p', 1480, "3061" },			// 2016:182
		// 2016:201
		{ 'p', 1400, "1710, 1720" },
		{ 'c', 885, "1014" },
		{ 'c', 880, "1103" },
		// 2016:356 VSS changes from 20.12
		{ 'p', 1400, "1730, 1740, 1750" },
		{ 'p', 1490, "2590" },
		// 2016:357 VSS changes from 20.12
		{ 'c', 100, "115, 116, 118" },
		{ 'c', 200, "219" },
		{ 'c', 808, "1015" },
		{ 'c', 881, "1016, 1017" },
		{ 'c', 808, "1205" },
		{ 'p', 1000, "1213" },		// 2017:023 IRQ/VSS
	#endif  // REGIONAL
		{ '\0', 0, NULL }
	};

	#if SPECIAL
	#undef COURT_LETTER
	#define COURT_LETTER 's'
	#endif

	static constant fn_suffix = COURT_CODE == COURT_SOFGS ? "_sgs" : "";

	mstr m;
	m.printf("c:\\tmp\\newsubj\\__upsb_%c%s.sql", COURT_LETTER, fn_suffix);
	FILE *upsb = fopen(str(m), "w");

	m.clear();
	m.printf("c:\\tmp\\newsubj\\__inar_%c%s.sql", COURT_LETTER, fn_suffix);
	FILE *inar = fopen(str(m), "w");

	for (const TSubjectDump *dump = Dumps; dump->type; dump++)
		dump_subject(dump, upsb, inar, true);

	static const TSubjectDump miniDumps[] =
	{
	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
		{ 'p', 700, "83450" },		// 800A
		{ 'p', 800, "99950" },		// 900A
	#endif  // AREAL || MILITARY || REGIONAL
	#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		{ 'p', 9000, "5083450" },	// 50800A
		{ 'p', 9000, "5099950" },	// 50900A
	#endif  // APPEAL || AREAL || MILITARY
	#if COURT_TYPE == COURT_ADMIN
		{ 'a', 3300, "430050" },	// 2016:110 IRQ/URQ: 4300
		{ 'a', 3150, "560050" },	// 2016:316 VSS
		{ 'a', 800,  "90050" },		// 2017:229
		{ 'a', 3100, "450050" },	// 2018-02-09
		{ 't', 3100, "450050" },	// 2018-02-09
	#endif  // ADMIN
		{ '\0', 0, NULL }
	};

	for (const TSubjectDump *dump = miniDumps; dump->type; dump++)
		dump_subject(dump, upsb, inar, false);

	if (COURT_CODE == COURT_SOFGS)
	{
		static const TSubjectDump sofgsDumps[] =
		{
			{ 'c', 610, "2900" },
			{ 'c', 800, "22100" },
			{ 'c', 9000, "2900, 22100, 22110" },
			{ '\0', 0, NULL }
		};

		for (const TSubjectDump *dump = sofgsDumps; dump->type; dump++)
			dump_subject(dump, upsb, inar, true);
	}

	fclose(upsb);
	fclose(inar);
	dumped_subjects.Flush();
}
#endif  // TEST_DUMP_SUBJS
