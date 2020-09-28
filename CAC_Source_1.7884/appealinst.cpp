#include "cac.h"

// 2012:300 military-appeal, special-appeal; copied from appeal and recoded
static const TAppealInstance AppealAppealInstances[] =
{
	{ "p",    "��",  NULL,         "z",                INSTANCE_SUPER, NULL,           "AEFJLK�v�",         0 }, // 1
	{ "p",    "��",  NULL,         "z",                INSTANCE_SUPER, NULL,           "ADHGBPjv�",         0 }, // 2
	{ "c",    "�",  NULL,         "z",                INSTANCE_SUPER, NULL,           "TVZbefigRmBv�",     0 }, // 3
	{ "c",    "�",   NULL,         "z",                INSTANCE_SUPER, NULL,           "TDHWBmkjv�",        0 }, // 4
	{ "c",    "�",   NULL,         "z",                INSTANCE_SUPER, NULL,           "z",                 0 }, // 5
	{ "c",    "�",   NULL,         "z",                INSTANCE_SUPER, NULL,           "z",                 0 }, // 6
	{ "i",    "��",  NULL,         "z",                INSTANCE_SUPER, NULL,           "ADGHBPv�",          0 }, // 7
	{ "r",    "��",  NULL,         "z",                INSTANCE_SUPER, NULL,           "ADGHBPv�",          0 }, // 8
};

static const TAppealInstance AppealInstances[] =
{
#if COURT_TYPE == COURT_APPEAL
	// 2001:031; 2004:169 ACT: +t where relevant; 2007:265 +ad where ct
	// 2008:070 LRQ: ��� changes
	{ "p",    "��",  NULL,         "z",                INSTANCE_SUPER, NULL,           "ABCDEFGv�",         0 }, // 1
	{ "p",    "��",  NULL,         "z",                INSTANCE_SUPER, NULL,           "AHIJKLjv�",         0 }, // 2
	{ "catd", "��",  NULL,         "z",                INSTANCE_SUPER, NULL,           "MNOPQRSTUVKqv�",    0 }, // 3; 2004:111 +Kq
	{ "catd", "�",   NULL,         "z",                INSTANCE_SUPER, NULL,           "WHIXKVkjv�",        0 }, // 4
	{ "catd", "�",   NULL,         "z",                INSTANCE_SUPER, NULL,           "z",                 0 }, // 5
	// 2009:169 LPR: removed �, �
	{ "catd", "�",   NULL,         "z",                INSTANCE_SUPER, NULL,           "z",                 0 }, // 6
	{ "i",    "��",  NULL,         "z",                INSTANCE_SUPER, NULL,           "AHJIKLv�",          0 }, // 7
	{ "r",    "��",  NULL,         "z",                INSTANCE_SUPER, NULL,           "AHJIKLv�",          0 }, // 8
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	{ "ct",   "�",   NULL,         "z",                INSTANCE_SUPER, NULL,           "kVSUXYcdfgiQomv�",  0 }, // 1
	{ "ct",   "�",   NULL,         "z",                INSTANCE_SUPER, NULL,           "kVSUXYcdfgiQomv�",  0 }, // 2
	{ "a",    "��", NULL,         "z",                INSTANCE_SELF,  NULL,           "TVXcBmv�9",         0 }, // 3; 2012:079 +� +9
	{ "a",    "��",  NULL,         "z",                INSTANCE_SELF,  NULL,           "TDHGBom9v�9",       0 }, // 4; 2012:079 +� +9
	{ "a",    "�",   NULL,         "z",                INSTANCE_SPEAL, NULL,           "SkXcfQv�",          0 }, // 5
	{ "a",    "�",   NULL,         "z",                INSTANCE_SPEAL, NULL,           "SkXcfQv�",          0 }, // 6
	{ "i",    "�",   NULL,         "z",                INSTANCE_SELF,  INSTANCE_SUPER, "TDHGBom9v�",        0 }, // 7; 2012:079 +SELF
	{ "i",    "�",   NULL,         "z",                INSTANCE_SELF,  INSTANCE_SUPER, "TDHGBomv�9",        0 }, // 8; 2012:079 +SELF, +(auto)SPEAL +9
	{ "r",    "�",   NULL,         "z",                INSTANCE_SUPER, INSTANCE_SPEAL, "TDHGBomz9v�",       0 }, // 9
	{ "r",    "�",   NULL,         "z",                INSTANCE_SUPER, INSTANCE_SPEAL, "TDHGBomzv�",        0 }, // 10; 2012:079 +9
	{ "c",    "��", NULL,         "z",                INSTANCE_SUPER, INSTANCE_SPEAL, "TVZbefigRomvj�",    0 }, // 11; 2008:086 LPR/LRQ: +�; 2009:278 +ta
	{ "c",    "��",  NULL,         "z",                INSTANCE_SUPER, INSTANCE_SPEAL, "TDHWBom9v�",        0 }, // 12; 2009:278 +ta
	// 2008:086 LRQ: +a, INSTANCE_III -> INSTANCE_SPEAL
	{ "cta",  "�",   NULL,         "z",                INSTANCE_SPEAL, NULL,           "z",                 0 }, // xD
	// 2008:206 LRQ: cp from areal 'a'; 2012:079 +�/�
	{ "t",    "��",  NULL,         "z",                INSTANCE_SUPER, INSTANCE_SPEAL, "TVXcBmv�9",         0 }, // 14; 2012:079 +9
	{ "t",    "��",  NULL,         "z",                INSTANCE_SUPER, INSTANCE_SPEAL, "TDHGBom9v�",        0 }, // 15
#endif  // ADMIN
	// 2012:003 LPR: -z for ks except if z only
#if COURT_TYPE == COURT_AREAL
	{ "p",    "��",  INSTANCE_III, "CAFKLMNIOv",       INSTANCE_SUPER, NULL,           "AEFJLKOv�",         0 }, // 1
	{ "p",    "�",   INSTANCE_III, "ADHGBP9v",         INSTANCE_SUPER, NULL,           "ADHGBP9v�",         0 }, // 2
	{ "p",    "�",   INSTANCE_III, "ADHGBPv",          INSTANCE_SUPER, NULL,           "ADHGBPv�",          0 }, // 3
	{ "p",    "�",   NULL,         "z",                INSTANCE_SPEAL, INSTANCE_SUPER, "AEFJLKOv�",         0 }, // 4
	{ "p",    "�",   NULL,         "z",                INSTANCE_SPEAL, INSTANCE_SUPER, "ADHGBP9v�",         0 }, // 5
	{ "p",    "�",   NULL,         "z",                INSTANCE_SUPER, NULL,           "AEFJLKOv�",         0 }, // 6
	{ "p",    "�",   NULL,         "z",                INSTANCE_SUPER, NULL,           "ADHGBPv�",          0 }, // 7
	{ "ctd",  "�",   INSTANCE_III, "VTUXYcdgiomjev",   INSTANCE_SUPER, NULL,           "TVZbefigRomjev�",   0 }, // 8
	{ "ctd",  "�",   INSTANCE_III, "TDHWBom9v",        INSTANCE_SUPER, NULL,           "TDHWBom9v�",        0 }, // 9
	{ "c",    "�",   INSTANCE_III, "VTUXYcdgiomjev",   INSTANCE_SUPER, NULL,           "TVZbefigRomjev�",   0 }, // 10
	{ "ctd",  "�",   NULL,         "z",                INSTANCE_SUPER, NULL,           "kVSUXYcdfgiQomv�",  0 }, // 11
	// 2008:028 � separated from �, INSTANCE_SUPER -> INSTANCE_III
	// 2008:0 72 re: +INSTANCE_SUPER and results
	{ "ctd",  "�",   INSTANCE_III, "kVSUXYcdfgiQomv",  INSTANCE_SUPER, NULL,           "kVSUXYcdfgiQomv�",  1 }, // 12
	{ "a",    "�",  NULL,         "z",                INSTANCE_SPEAL, NULL,           "TVXcBmv�",          0 }, // xD
	{ "a",    "�",   NULL,         "z",                INSTANCE_SPEAL, NULL,           "TDHGBom9v�",        0 }, // 14
	{ "a",    "�",   NULL,         "z",                INSTANCE_SPEAL, NULL,           "SkXcfQv�",          0 }, // 15
	// 2008:028 � separated from �, INSTANCE_SPEAL -> INSTANCE_III
	// 2008:072 re: +INSTANCE_SPEAL and results
	// 2008:343 LPR: not MILITARY, 'a'dmin anyway
	{ "a",    "�",   INSTANCE_III, "SkXcfQv",          INSTANCE_SPEAL, NULL,           "SkXcfQv�",          1 }, // 16
	{ "i",    "�",   INSTANCE_III, "ADHGBP9v",         INSTANCE_SUPER, NULL,           "ADHGBP9v�",         0 }, // 17
	{ "i",    "�",   INSTANCE_III, "ADHGBPv",          INSTANCE_SUPER, NULL,           "ADHGBPv�",          0 }, // 18
	{ "i",    "�",   NULL,         "z",                INSTANCE_SPEAL, INSTANCE_SUPER, "TDHGBom9v�",        0 }, // 19
	{ "i",    "�",   NULL,         "z",                INSTANCE_SUPER, NULL,           "TDHGBomv�",         0 }, // 20; 2011:349 "z", -z
	{ "r",    "�",   INSTANCE_III, "ADHGBP9v",         INSTANCE_SUPER, NULL,           "ADHGBP9v�",         0 }, // 21
	{ "r",    "�",   INSTANCE_III, "ADHGBPv",          INSTANCE_SUPER, NULL,           "ADHGBPv�",          0 }, // 22
	{ "r",    "�",   NULL,          "z",               INSTANCE_SPEAL, INSTANCE_SUPER, "TDHGBom9v�",        0 }, // 23; 2011:349 "z", -z
	{ "r",    "�",   NULL,          "z",               INSTANCE_SUPER, NULL,           "TDHGBomv�",         0 }, // 24; 2011:349 "z", -z
	{ "ctd",  "��",  NULL,          "z",               INSTANCE_SUPER, NULL,           "TVZbefigRomvj�",    0 }, // 25
	{ "ctd",  "��",  NULL,          "z",               INSTANCE_SUPER, NULL,           "TDHWBom9v�",        0 }, // 26
	// 2007:288
	{ "c",    "�",   NULL,          "z",               INSTANCE_III,   NULL,           "TVZbefigRomvj�",    0 }, // 27
	// 2011:175 +i; 2016:033 +t
	{ "ctir", "�",   NULL,          "z",               INSTANCE_III,   NULL,           "TDHWBom9v�",        0 }, // 28
	{ "ctad", "�",   INSTANCE_III,  "z",               NULL,           NULL,           "z",                 0 }, // 29
	{ "ct",   "�",   INSTANCE_III,  "VTUXYcdgiomjev",  INSTANCE_SUPER, NULL,           "TVZbefigRomjev�",   0 }, // 30
	{ "ct",   "�",   NULL,          "z",               INSTANCE_SUPER, NULL,           "TVZbefigRomvj�",    0 }, // 31
	{ "p",    "\"",  INSTANCE_III,  "AKLODHGBPEFJv",   INSTANCE_SUPER, INSTANCE_SPEAL, "AKLODHGBPEFJv�",    0 }, // 2015:174
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	{ "p",    "��",  INSTANCE_III,  "CAFKLMNIOv",      INSTANCE_SUPER, NULL,            "AEFJLKOv�",        0 }, // 1
	{ "p",    "�",   INSTANCE_III,  "ADHGBP9v",        INSTANCE_SUPER, NULL,            "ADHGBP9v�",        0 }, // 2
	{ "p",    "�",   INSTANCE_III,  "ADHGBPv",         INSTANCE_SUPER, NULL,            "ADHGBPv�",         0 }, // 3
	// 2008:343 MILITARY port: INSTANCE_SPEAL -> NULL, going to SUPER anyway
	{ "p",    "�",   NULL,          "z",               INSTANCE_SUPER, NULL,            "AEFJLKOv�",        0 }, // 4
	{ "p",    "�",   NULL,          "z",               INSTANCE_SUPER, NULL,            "ADHGBP9v�",        0 }, // 5
	{ "p",    "�",   NULL,          "z",               INSTANCE_SUPER, NULL,            "AEFJLKOv�",        0 }, // 6
	{ "p",    "�",   NULL,          "z",               INSTANCE_SUPER, NULL,            "ADHGBPv�",         0 }, // 7
	{ "c",    "�",   INSTANCE_III,  "VTUXYcdgiomjev",  INSTANCE_SUPER, NULL,            "TVZbefigRomjev�",  0 }, // 8
	{ "c",    "�",   INSTANCE_III,  "TDHWBom9v",       INSTANCE_SUPER, NULL,            "TDHWBom9v�",       0 }, // 9
	// 2002:232 LRQ: +� for c, VRQ: cp from � for c, for 152256784
	{ "c",    "�",   INSTANCE_III,  "VTUXYcdgiomjev",  INSTANCE_SUPER, NULL,            "TVZbefigRomjev�",  0 }, // 10
	// 2001:319 PRQ: +j for g; 2004:048 LRQ: � +�, � +�, LRQ cert for 'd'
	{ "c",    "�",   NULL,          "z",               INSTANCE_SUPER, NULL,            "kVSUXYcdfgiQomv�", 0 }, // 11
	// 2008:028 � separated from �, INSTANCE_SUPER -> INSTANCE_III
	// 2008:072 re: +INSTANCE_SUPER and results
	{ "c",    "�",   INSTANCE_III,  "kVSUXYcdfgiQomv", INSTANCE_SUPER, NULL,            "kVSUXYcdfgiQomv�", 1 }, // 12
	// 2009:169 LPR: removed �, �; 2012:100 -xD, 14, 15: 'a'
	{ "i",    "�",   INSTANCE_III,  "ADHGBP9v",        INSTANCE_SUPER, NULL,            "ADHGBP9v�",        0 }, // 16
	{ "i",    "�",   INSTANCE_III,  "ADHGBPv",         INSTANCE_SUPER, NULL,            "ADHGBPv�",         0 }, // 17
	// 2008:343 MILITARY port: INSTANCE_SPEAL -> NULL, going to SUPER anyway
	{ "i",    "�",   NULL,          "z",               INSTANCE_SUPER, NULL,            "TDHGBom9v�",       0 }, // 18
	{ "i",    "�",   NULL,          "z",               INSTANCE_SUPER, NULL,            "TDHGBomv�",        0 }, // 19; 2011:349 "z", -z
	{ "r",    "�",   INSTANCE_III,  "ADHGBP9v",        INSTANCE_SUPER, NULL,            "ADHGBP9v�",        0 }, // 20
	{ "r",    "�",   INSTANCE_III,  "ADHGBPv",         INSTANCE_SUPER, NULL,            "ADHGBPv�",         0 }, // 21
	// 2008:343 LPR/LRQ: -INSTANCE_SPEAL
	{ "r",    "�",   NULL,          "z",               INSTANCE_SUPER, NULL,            "TDHGBom9v�",       0 }, // 22
	{ "r",    "�",   NULL,          "z",               INSTANCE_SUPER, NULL,            "TDHGBomv�",        0 }, // 23; 2011:349 "z", -z
	{ "c",    "��",  NULL,          "z",               INSTANCE_SUPER, NULL,            "TVZbefigRomvj�",   0 }, // 24
	{ "c",    "��",  NULL,          "z",               INSTANCE_SUPER, NULL,            "TDHWBom9v�",       0 }, // 25
	// 2007:288
	{ "c",    "�",   NULL,          "z",               INSTANCE_III,   NULL,            "TVZbefigRomvj�",   0 }, // 26
	{ "cir",  "�",   NULL,          "z",               INSTANCE_III,   NULL,            "TDHWBom9v�",       0 }, // 27; 2011:175 +i
	{ "c",    "�",   INSTANCE_III,  "z",               NULL,           NULL,            "z",                0 }, // 28
	{ "c",    "�",   INSTANCE_III,  "VTUXYcdgiomjev",  INSTANCE_SUPER, NULL,            "TVZbefigRomjev�",  0 }, // 29
	{ "c",    "�",   NULL,          "z",               INSTANCE_SUPER, NULL,            "TVZbefigRomvj�",   0 }, // 30
	{ "p",    "\"",  INSTANCE_III,  "AKLODHGBPEFJv",   INSTANCE_SUPER, NULL,            "AKLODHGBPEFJv�",    0 }, // 2015:195
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ "p",    "��",  INSTANCE_III,  "ABCDEFGHIJKL8",   INSTANCE_SUPER, NULL,           "AbcdFEL8�",         0 }, // 1
	{ "p",    "��",  INSTANCE_III,  "AMNOPQ8",         INSTANCE_SUPER, NULL,           "AMNOPQ8�",          0 }, // 2
	// 2013:071 KS only, new results; 2013:177 FIX: SPEAL -> III; 2014:094 +a
	{ "pa",   "�",   NULL,          "z",               INSTANCE_III,   INSTANCE_ADMIN, "ARSTUVWXYZL8�",     0 }, // 3
	// 2013:071 KS only; 2013:177 FIX: SPEAL -> III; 2014:094 +a
	{ "pa",   "�",   NULL,          "z",               INSTANCE_III,   INSTANCE_ADMIN, "AMNOPQ8�",          0 }, // 4
	{ "cm",   "զ",  INSTANCE_III,  "efUgijkvwom8",    INSTANCE_SUPER, NULL,           "eUyi05k1wvPom8�",   0 }, // 5
	{ "cm",   "�",   INSTANCE_III,  "eMNxPom8",        INSTANCE_SUPER, NULL,           "eMNxPom8�",         0 }, // 6
	{ "cm",   "�",  NULL,          "z",               INSTANCE_III,   INSTANCE_ADMIN, "eUyi05k1wvPom8�",   2 }, // 7; 2013:178 KS only, like 'p'
	{ "cm",   "�",   NULL,          "z",               INSTANCE_III,   INSTANCE_ADMIN, "eMNxPom8�",         2 }, // 8; 2013:178 KS only, like 'p'
	{ "cm",   "��",  NULL,          "z",               INSTANCE_SUPER, NULL,           "2f3Ugijk1vw4om8�",  0 }, // 9
	// 2008:310 LRQ; 2013:008 PRQ: (regress FIX?) +INSTANCE_SPEAL
	{ "p",    "�",   INSTANCE_APP,  "AbcdFEL8MNOPQ",   INSTANCE_SUPER, INSTANCE_SPEAL, "AbcdFEL8MNOPQ�",    0 }, // 10
	// 2008:028 � separated from ��, INSTANCE_SUPER -> INSTANCE_III
	// 2008:072 re: +INSTANCE_SUPER and results
	{ "cm",   "�",   INSTANCE_III,  "2f3Ugijk1vw4om8", INSTANCE_SUPER, NULL,           "2f3Ugijk1vw4om8�",  1 }, // 11
	{ "cm",   "�",   INSTANCE_III,  "678",             NULL,           NULL,           "z",                 0 }, // 12
	// 2009:169 LPR: removed �, �
	{ "cm",   "�",   INSTANCE_III,  "z",               NULL,           NULL,           "z",                 0 }, // xD
	// 2001:092; 2001:109; 2006:018 +�
	{ "i",    "��",  INSTANCE_III,  "AMNOPQ8",         INSTANCE_SUPER, NULL,           "AMNOPQ8�",          0 }, // 14
	// 2008:075 separated � be able to skip leading z
	{ "i",    "�",   INSTANCE_III,  "AMNOPQ8",         INSTANCE_SUPER, NULL,           "AMNOPQ8�",          1 }, // 15
	{ "i",    "�",   INSTANCE_III,  "eMNOPQ8",         INSTANCE_SPEAL, NULL,           "eMNOPom8�",         2 }, // 16
	// 2001:094; 2001:109
	{ "r",    "��",  INSTANCE_III,  "AMNOPQ8",         INSTANCE_SUPER, NULL,           "AMNOPQ8�",          0 }, // 17
	{ "r",    "�",   INSTANCE_III,  "eMNOPQ8",         INSTANCE_SPEAL, NULL,           "eMNOPom8�",         2 }, // 18
#endif  // REGIONAL
	{ NULL,   NULL,  NULL,          NULL,              NULL,           NULL,           NULL,                0 }
};

bool TAppealInstance::KSRecv(const char *receiver) const
{
	return
#if COURT_TYPE == COURT_ADMIN
		(ks0 && !strcmp(ks0, INSTANCE_SELF) && !strcmp(receiver, INSTANCE_SPEAL)) ||	// 2012:079
#endif  // ADMIN
		(ks0 && !strcmp(receiver, ks0)) || (ks1 && !strcmp(receiver, ks1));
}

// ----- functions -----------------------------------------------------------
const TAppealInstance *obtain_appeal_instance(char type, char kind, char decisionKind)
{
	const TAppealInstance *hinst;

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_MILITARY
	if (CourtType == COURT_APPEAL || CourtType == COURT_MILITARY_APPEAL)
		if (kind == KIND_CONNECT_APPEAL && decisionKind == KIND_SETTLEMENT)
			kind = KIND_PRIVATE_CONNECT_APPEAL;
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN
	(void) decisionKind;
#endif

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (CourtType != COURT_MILITARY_APPEAL)
	{
		if (kind == KIND_CALL_APPEAL && decisionKind == KIND_SETTLEMENT)
			kind = KIND_PRIVATE_CALL_APPEAL;
		else if (kind == KIND_PROTEST && decisionKind == KIND_SETTLEMENT)
			kind = KIND_PRIVATE_PROTEST;
	}
#endif  // AREAL || MILITARY || REGIONAL

	for (hinst = CourtType == COURT_MILITARY_APPEAL ? AppealAppealInstances : AppealInstances; hinst->types; hinst++)
		if (strchr(hinst->types, type) && strchr(hinst->kinds, kind))
			return hinst;

	return NULL;
}

const TAppealInstance *obtain_appeal_instance(const TInReg *inReg)
{
	return obtain_appeal_instance(inReg->decision.type ? inReg->decision.type : inReg->lawsuit.type, inReg->kind,
		inReg->decisionKind);
}

bool fill_appeal_fixeivers(const TInReg *inReg, TFixeiver &fixeiver, char *receiver)
{
	const TAppealInstance *const hinst = obtain_appeal_instance(inReg);

	if (!hinst)
		return false;

	int index = 0, index_ks;

	if (hinst->vz0)
	{
		fixeiver.ucns[index++] = hinst->vz0;

		// auto instance
#if COURT_TYPE == COURT_AREAL
		if (!strcmp(hinst->vz0, INSTANCE_III))
		{
			// 2009:260 LRQ: add SOFAP only if exist
			TSender sender;

			sender.ucnType = UCN_INSTANCE_HIGHER;
			strcpy(sender.ucn, INSTANCE_SOFAP);

			if (sender.Try("F_UCN"))
				fixeiver.ucns[index++] = INSTANCE_SOFAP;
		}
#endif  // AREAL
#if COURT_TYPE == COURT_REGIONAL
		if (!strcmp(hinst->vz0, INSTANCE_III))
			fixeiver.ucns[index++] = INSTANCE_ADMIN;
#endif  // REGIONAL
	}

	// auto instance
#if COURT_TYPE == COURT_ADMIN
	if (hinst->ks0 && !strcmp(hinst->ks0, INSTANCE_SELF))
		fixeiver.ucns[index++] = INSTANCE_SPEAL;
#endif  // ADMIN

	index_ks = index;

	if (!hinst->vz0 || (hinst->flags & APPEAL_JUMP_KS))
	{
		fixeiver.ucns[index++] = hinst->ks0;

		if (hinst->ks1)
			fixeiver.ucns[index] = hinst->ks1;
	}

	strcpy(receiver, fixeiver.ucns[hinst->flags & APPEAL_JUMP_KS ? index_ks : 0]);
	return true;
}

// ----- TESTVER -------------------------------------------------------------
#if TESTVER
static void dump_alias_set(FILE *f, const char *set, TCharAliasGroup *alias, const char *sep, const char *end)
{
	if (set && any(set))
	{
		for (const char *s = set; *s; s++)
		{
			if (s > set)
				fputs(sep, f);

			alias->ValueField()->SetC(*s);

			if (alias->Try())
				fputs(any(alias->NameField->S()) ? alias->NameField->S() : "''" , f);
			else
				fprintf(f, "������� %s '%c' (%d)", alias->Name, *s, *s);
		}
	}
	else
		fputs("����", f);

	fputs(end, f);
}

static void dump_instance(FILE *f, const char *instance)
{
	TSender sender;

	strcpy(sender.ucn, instance);
	sender.ucnType = UCN_INSTANCE_HIGHER;
	fputs(sender.Try() ? sender.name : instance, f);
}

static void dump_header(FILE *f)
{
#if COURT_TYPE == COURT_APPEAL
	TKind tKind;

	fprintf(f, "��: %s + ����������� -> %s\n", Kind->Seek(KIND_CONNECT_APPEAL), tKind.Seek(KIND_PRIVATE_CONNECT_APPEAL));
	fputc('\n', f);
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN
	fputs("����:\n\n", f);
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	TKind tKind;

	fprintf(f, "��/��/��: %s + ����������� -> %s\n", Kind->Seek(KIND_CALL_APPEAL), tKind.Seek(KIND_PRIVATE_CALL_APPEAL));
	fprintf(f, "��/��/��: %s + ����������� -> %s\n", Kind->Seek(KIND_PROTEST), tKind.Seek(KIND_PRIVATE_PROTEST));
	fputc('\n', f);
#endif  // AREAL || MILITARY || REGIONAL
}

void dump_appeal_instances()
{
	TTextFile tf;
	FILE *f = tf.OpenTemp();

	dump_header(f);
	const TAppealInstance *hinst;

	for (hinst = CourtType == COURT_MILITARY_APPEAL ? AppealAppealInstances : AppealInstances; hinst->types; hinst++)
	{
		fprintf(f, "%d. ", (hinst - AppealInstances) + 1);
		dump_alias_set(f, hinst->types, Type, ", ", "\n");
		dump_alias_set(f, hinst->kinds, Kind, ", ", "\n");

		fputs("\t�������� ���������: ", f);

		if (hinst->vz0)
		{
			dump_instance(f, hinst->vz0);

			#if COURT_TYPE == COURT_AREAL
			if (!strcmp(hinst->vz0, INSTANCE_III))
				fputs("  (� �� �����)", f);
			#endif  // AREAL
			#if COURT_TYPE == COURT_REGIONAL
			if (!strcmp(hinst->vz0, INSTANCE_III))
				fputs(" (� ���� �����)", f);
			#endif  // REGIONAL
		}
		else
			fputs("����", f);

		fputc('\n', f);

		fputs("\t��������� �� �������� ���������:\n\t\t", f);
		dump_alias_set(f, strcmp(hinst->vz_res, C2S[RESOLUTION_FAKE]) ? hinst->vz_res : NULL, Resolution, "\n\t\t", "\n");

		fputs("\t���������� ���������: ", f);

		if (hinst->ks0)
		{
			dump_instance(f, hinst->ks0);

		#if COURT_TYPE == COURT_ADMIN
			if (!strcmp(hinst->ks0, INSTANCE_SELF))
				fputs(", �������� ��������������� ���", f);
		#endif  // ADMIN

			if (hinst->ks1)
			{
				fputs(", ", f);
				dump_instance(f, hinst->ks1);
			}
		}
		else
			fputs("����", f);

		fputc('\n', f);

		fputs("\t��������� �� ���������� ���������:\n\t\t", f);
		dump_alias_set(f, strcmp(hinst->ks_res, C2S[RESOLUTION_FAKE]) ? hinst->ks_res : NULL, Resolution, "\n\t\t", "\n");

		if (hinst->flags & 1)
			fputs("\t������� ��������� �������� � ���������� ���������\n", f);

		fputc('\n', f);
	}

	fputs("����\n", f);
	tf.Show(NULL);
}
#endif  // TESTVER
