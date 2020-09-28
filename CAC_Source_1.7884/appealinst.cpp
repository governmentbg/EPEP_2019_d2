#include "cac.h"

// 2012:300 military-appeal, special-appeal; copied from appeal and recoded
static const TAppealInstance AppealAppealInstances[] =
{
	{ "p",    "до",  NULL,         "z",                INSTANCE_SUPER, NULL,           "AEFJLKВvА",         0 }, // 1
	{ "p",    "нр",  NULL,         "z",                INSTANCE_SUPER, NULL,           "ADHGBPjvА",         0 }, // 2
	{ "c",    "д±",  NULL,         "z",                INSTANCE_SUPER, NULL,           "TVZbefigRmBvА",     0 }, // 3
	{ "c",    "н",   NULL,         "z",                INSTANCE_SUPER, NULL,           "TDHWBmkjvА",        0 }, // 4
	{ "c",    "е",   NULL,         "z",                INSTANCE_SUPER, NULL,           "z",                 0 }, // 5
	{ "c",    "‰",   NULL,         "z",                INSTANCE_SUPER, NULL,           "z",                 0 }, // 6
	{ "i",    "нр",  NULL,         "z",                INSTANCE_SUPER, NULL,           "ADGHBPvА",          0 }, // 7
	{ "r",    "нр",  NULL,         "z",                INSTANCE_SUPER, NULL,           "ADGHBPvА",          0 }, // 8
};

static const TAppealInstance AppealInstances[] =
{
#if COURT_TYPE == COURT_APPEAL
	// 2001:031; 2004:169 ACT: +t where relevant; 2007:265 +ad where ct
	// 2008:070 LRQ: ‰±¦ changes
	{ "p",    "ьо",  NULL,         "z",                INSTANCE_SUPER, NULL,           "ABCDEFGvА",         0 }, // 1
	{ "p",    "нр",  NULL,         "z",                INSTANCE_SUPER, NULL,           "AHIJKLjvА",         0 }, // 2
	{ "catd", "ь±",  NULL,         "z",                INSTANCE_SUPER, NULL,           "MNOPQRSTUVKqvА",    0 }, // 3; 2004:111 +Kq
	{ "catd", "н",   NULL,         "z",                INSTANCE_SUPER, NULL,           "WHIXKVkjvА",        0 }, // 4
	{ "catd", "е",   NULL,         "z",                INSTANCE_SUPER, NULL,           "z",                 0 }, // 5
	// 2009:169 LPR: removed и, к
	{ "catd", "‰",   NULL,         "z",                INSTANCE_SUPER, NULL,           "z",                 0 }, // 6
	{ "i",    "нр",  NULL,         "z",                INSTANCE_SUPER, NULL,           "AHJIKLvА",          0 }, // 7
	{ "r",    "нр",  NULL,         "z",                INSTANCE_SUPER, NULL,           "AHJIKLvА",          0 }, // 8
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	{ "ct",   "з",   NULL,         "z",                INSTANCE_SUPER, NULL,           "kVSUXYcdfgiQomvА",  0 }, // 1
	{ "ct",   "е",   NULL,         "z",                INSTANCE_SUPER, NULL,           "kVSUXYcdfgiQomvА",  0 }, // 2
	{ "a",    "до±", NULL,         "z",                INSTANCE_SELF,  NULL,           "TVXcBmvА9",         0 }, // 3; 2012:079 +о +9
	{ "a",    "нр",  NULL,         "z",                INSTANCE_SELF,  NULL,           "TDHGBom9vА9",       0 }, // 4; 2012:079 +р +9
	{ "a",    "з",   NULL,         "z",                INSTANCE_SPEAL, NULL,           "SkXcfQvА",          0 }, // 5
	{ "a",    "е",   NULL,         "z",                INSTANCE_SPEAL, NULL,           "SkXcfQvА",          0 }, // 6
	{ "i",    "н",   NULL,         "z",                INSTANCE_SELF,  INSTANCE_SUPER, "TDHGBom9vА",        0 }, // 7; 2012:079 +SELF
	{ "i",    "р",   NULL,         "z",                INSTANCE_SELF,  INSTANCE_SUPER, "TDHGBomvА9",        0 }, // 8; 2012:079 +SELF, +(auto)SPEAL +9
	{ "r",    "н",   NULL,         "z",                INSTANCE_SUPER, INSTANCE_SPEAL, "TDHGBomz9vА",       0 }, // 9
	{ "r",    "р",   NULL,         "z",                INSTANCE_SUPER, INSTANCE_SPEAL, "TDHGBomzvА",        0 }, // 10; 2012:079 +9
	{ "c",    "до±", NULL,         "z",                INSTANCE_SUPER, INSTANCE_SPEAL, "TVZbefigRomvjА",    0 }, // 11; 2008:086 LPR/LRQ: +±; 2009:278 +ta
	{ "c",    "нр",  NULL,         "z",                INSTANCE_SUPER, INSTANCE_SPEAL, "TDHWBom9vА",        0 }, // 12; 2009:278 +ta
	// 2008:086 LRQ: +a, INSTANCE_III -> INSTANCE_SPEAL
	{ "cta",  "‰",   NULL,         "z",                INSTANCE_SPEAL, NULL,           "z",                 0 }, // xD
	// 2008:206 LRQ: cp from areal 'a'; 2012:079 +о/р
	{ "t",    "до",  NULL,         "z",                INSTANCE_SUPER, INSTANCE_SPEAL, "TVXcBmvА9",         0 }, // 14; 2012:079 +9
	{ "t",    "нр",  NULL,         "z",                INSTANCE_SUPER, INSTANCE_SPEAL, "TDHGBom9vА",        0 }, // 15
#endif  // ADMIN
	// 2012:003 LPR: -z for ks except if z only
#if COURT_TYPE == COURT_AREAL
	{ "p",    "Ха",  INSTANCE_III, "CAFKLMNIOv",       INSTANCE_SUPER, NULL,           "AEFJLKOvА",         0 }, // 1
	{ "p",    "Ь",   INSTANCE_III, "ADHGBP9v",         INSTANCE_SUPER, NULL,           "ADHGBP9vА",         0 }, // 2
	{ "p",    "в",   INSTANCE_III, "ADHGBPv",          INSTANCE_SUPER, NULL,           "ADHGBPvА",          0 }, // 3
	{ "p",    "д",   NULL,         "z",                INSTANCE_SPEAL, INSTANCE_SUPER, "AEFJLKOvА",         0 }, // 4
	{ "p",    "н",   NULL,         "z",                INSTANCE_SPEAL, INSTANCE_SUPER, "ADHGBP9vА",         0 }, // 5
	{ "p",    "о",   NULL,         "z",                INSTANCE_SUPER, NULL,           "AEFJLKOvА",         0 }, // 6
	{ "p",    "р",   NULL,         "z",                INSTANCE_SUPER, NULL,           "ADHGBPvА",          0 }, // 7
	{ "ctd",  "Х",   INSTANCE_III, "VTUXYcdgiomjev",   INSTANCE_SUPER, NULL,           "TVZbefigRomjevА",   0 }, // 8
	{ "ctd",  "Ь",   INSTANCE_III, "TDHWBom9v",        INSTANCE_SUPER, NULL,           "TDHWBom9vА",        0 }, // 9
	{ "c",    "а",   INSTANCE_III, "VTUXYcdgiomjev",   INSTANCE_SUPER, NULL,           "TVZbefigRomjevА",   0 }, // 10
	{ "ctd",  "з",   NULL,         "z",                INSTANCE_SUPER, NULL,           "kVSUXYcdfgiQomvА",  0 }, // 11
	// 2008:028 е separated from з, INSTANCE_SUPER -> INSTANCE_III
	// 2008:0 72 re: +INSTANCE_SUPER and results
	{ "ctd",  "е",   INSTANCE_III, "kVSUXYcdfgiQomv",  INSTANCE_SUPER, NULL,           "kVSUXYcdfgiQomvА",  1 }, // 12
	{ "a",    "д±",  NULL,         "z",                INSTANCE_SPEAL, NULL,           "TVXcBmvА",          0 }, // xD
	{ "a",    "н",   NULL,         "z",                INSTANCE_SPEAL, NULL,           "TDHGBom9vА",        0 }, // 14
	{ "a",    "з",   NULL,         "z",                INSTANCE_SPEAL, NULL,           "SkXcfQvА",          0 }, // 15
	// 2008:028 е separated from з, INSTANCE_SPEAL -> INSTANCE_III
	// 2008:072 re: +INSTANCE_SPEAL and results
	// 2008:343 LPR: not MILITARY, 'a'dmin anyway
	{ "a",    "е",   INSTANCE_III, "SkXcfQv",          INSTANCE_SPEAL, NULL,           "SkXcfQvА",          1 }, // 16
	{ "i",    "Ь",   INSTANCE_III, "ADHGBP9v",         INSTANCE_SUPER, NULL,           "ADHGBP9vА",         0 }, // 17
	{ "i",    "в",   INSTANCE_III, "ADHGBPv",          INSTANCE_SUPER, NULL,           "ADHGBPvА",          0 }, // 18
	{ "i",    "н",   NULL,         "z",                INSTANCE_SPEAL, INSTANCE_SUPER, "TDHGBom9vА",        0 }, // 19
	{ "i",    "р",   NULL,         "z",                INSTANCE_SUPER, NULL,           "TDHGBomvА",         0 }, // 20; 2011:349 "z", -z
	{ "r",    "Ь",   INSTANCE_III, "ADHGBP9v",         INSTANCE_SUPER, NULL,           "ADHGBP9vА",         0 }, // 21
	{ "r",    "в",   INSTANCE_III, "ADHGBPv",          INSTANCE_SUPER, NULL,           "ADHGBPvА",          0 }, // 22
	{ "r",    "н",   NULL,          "z",               INSTANCE_SPEAL, INSTANCE_SUPER, "TDHGBom9vА",        0 }, // 23; 2011:349 "z", -z
	{ "r",    "р",   NULL,          "z",               INSTANCE_SUPER, NULL,           "TDHGBomvА",         0 }, // 24; 2011:349 "z", -z
	{ "ctd",  "до",  NULL,          "z",               INSTANCE_SUPER, NULL,           "TVZbefigRomvjА",    0 }, // 25
	{ "ctd",  "нр",  NULL,          "z",               INSTANCE_SUPER, NULL,           "TDHWBom9vА",        0 }, // 26
	// 2007:288
	{ "c",    "µ",   NULL,          "z",               INSTANCE_III,   NULL,           "TVZbefigRomvjА",    0 }, // 27
	// 2011:175 +i; 2016:033 +t
	{ "ctir", "§",   NULL,          "z",               INSTANCE_III,   NULL,           "TDHWBom9vА",        0 }, // 28
	{ "ctad", "‰",   INSTANCE_III,  "z",               NULL,           NULL,           "z",                 0 }, // 29
	{ "ct",   "¦",   INSTANCE_III,  "VTUXYcdgiomjev",  INSTANCE_SUPER, NULL,           "TVZbefigRomjevА",   0 }, // 30
	{ "ct",   "±",   NULL,          "z",               INSTANCE_SUPER, NULL,           "TVZbefigRomvjА",    0 }, // 31
	{ "p",    "\"",  INSTANCE_III,  "AKLODHGBPEFJv",   INSTANCE_SUPER, INSTANCE_SPEAL, "AKLODHGBPEFJvА",    0 }, // 2015:174
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	{ "p",    "Ха",  INSTANCE_III,  "CAFKLMNIOv",      INSTANCE_SUPER, NULL,            "AEFJLKOvА",        0 }, // 1
	{ "p",    "Ь",   INSTANCE_III,  "ADHGBP9v",        INSTANCE_SUPER, NULL,            "ADHGBP9vА",        0 }, // 2
	{ "p",    "в",   INSTANCE_III,  "ADHGBPv",         INSTANCE_SUPER, NULL,            "ADHGBPvА",         0 }, // 3
	// 2008:343 MILITARY port: INSTANCE_SPEAL -> NULL, going to SUPER anyway
	{ "p",    "д",   NULL,          "z",               INSTANCE_SUPER, NULL,            "AEFJLKOvА",        0 }, // 4
	{ "p",    "н",   NULL,          "z",               INSTANCE_SUPER, NULL,            "ADHGBP9vА",        0 }, // 5
	{ "p",    "о",   NULL,          "z",               INSTANCE_SUPER, NULL,            "AEFJLKOvА",        0 }, // 6
	{ "p",    "р",   NULL,          "z",               INSTANCE_SUPER, NULL,            "ADHGBPvА",         0 }, // 7
	{ "c",    "Х",   INSTANCE_III,  "VTUXYcdgiomjev",  INSTANCE_SUPER, NULL,            "TVZbefigRomjevА",  0 }, // 8
	{ "c",    "Ь",   INSTANCE_III,  "TDHWBom9v",       INSTANCE_SUPER, NULL,            "TDHWBom9vА",       0 }, // 9
	// 2002:232 LRQ: +а for c, VRQ: cp from Х for c, for 152256784
	{ "c",    "а",   INSTANCE_III,  "VTUXYcdgiomjev",  INSTANCE_SUPER, NULL,            "TVZbefigRomjevА",  0 }, // 10
	// 2001:319 PRQ: +j for g; 2004:048 LRQ: д +о, н +р, LRQ cert for 'd'
	{ "c",    "з",   NULL,          "z",               INSTANCE_SUPER, NULL,            "kVSUXYcdfgiQomvА", 0 }, // 11
	// 2008:028 е separated from з, INSTANCE_SUPER -> INSTANCE_III
	// 2008:072 re: +INSTANCE_SUPER and results
	{ "c",    "е",   INSTANCE_III,  "kVSUXYcdfgiQomv", INSTANCE_SUPER, NULL,            "kVSUXYcdfgiQomvА", 1 }, // 12
	// 2009:169 LPR: removed и, к; 2012:100 -xD, 14, 15: 'a'
	{ "i",    "Ь",   INSTANCE_III,  "ADHGBP9v",        INSTANCE_SUPER, NULL,            "ADHGBP9vА",        0 }, // 16
	{ "i",    "в",   INSTANCE_III,  "ADHGBPv",         INSTANCE_SUPER, NULL,            "ADHGBPvА",         0 }, // 17
	// 2008:343 MILITARY port: INSTANCE_SPEAL -> NULL, going to SUPER anyway
	{ "i",    "н",   NULL,          "z",               INSTANCE_SUPER, NULL,            "TDHGBom9vА",       0 }, // 18
	{ "i",    "р",   NULL,          "z",               INSTANCE_SUPER, NULL,            "TDHGBomvА",        0 }, // 19; 2011:349 "z", -z
	{ "r",    "Ь",   INSTANCE_III,  "ADHGBP9v",        INSTANCE_SUPER, NULL,            "ADHGBP9vА",        0 }, // 20
	{ "r",    "в",   INSTANCE_III,  "ADHGBPv",         INSTANCE_SUPER, NULL,            "ADHGBPvА",         0 }, // 21
	// 2008:343 LPR/LRQ: -INSTANCE_SPEAL
	{ "r",    "н",   NULL,          "z",               INSTANCE_SUPER, NULL,            "TDHGBom9vА",       0 }, // 22
	{ "r",    "р",   NULL,          "z",               INSTANCE_SUPER, NULL,            "TDHGBomvА",        0 }, // 23; 2011:349 "z", -z
	{ "c",    "до",  NULL,          "z",               INSTANCE_SUPER, NULL,            "TVZbefigRomvjА",   0 }, // 24
	{ "c",    "нр",  NULL,          "z",               INSTANCE_SUPER, NULL,            "TDHWBom9vА",       0 }, // 25
	// 2007:288
	{ "c",    "µ",   NULL,          "z",               INSTANCE_III,   NULL,            "TVZbefigRomvjА",   0 }, // 26
	{ "cir",  "§",   NULL,          "z",               INSTANCE_III,   NULL,            "TDHWBom9vА",       0 }, // 27; 2011:175 +i
	{ "c",    "‰",   INSTANCE_III,  "z",               NULL,           NULL,            "z",                0 }, // 28
	{ "c",    "¦",   INSTANCE_III,  "VTUXYcdgiomjev",  INSTANCE_SUPER, NULL,            "TVZbefigRomjevА",  0 }, // 29
	{ "c",    "±",   NULL,          "z",               INSTANCE_SUPER, NULL,            "TVZbefigRomvjА",   0 }, // 30
	{ "p",    "\"",  INSTANCE_III,  "AKLODHGBPEFJv",   INSTANCE_SUPER, NULL,            "AKLODHGBPEFJvА",    0 }, // 2015:195
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	{ "p",    "Ха",  INSTANCE_III,  "ABCDEFGHIJKL8",   INSTANCE_SUPER, NULL,           "AbcdFEL8А",         0 }, // 1
	{ "p",    "Ьв",  INSTANCE_III,  "AMNOPQ8",         INSTANCE_SUPER, NULL,           "AMNOPQ8А",          0 }, // 2
	// 2013:071 KS only, new results; 2013:177 FIX: SPEAL -> III; 2014:094 +a
	{ "pa",   "д",   NULL,          "z",               INSTANCE_III,   INSTANCE_ADMIN, "ARSTUVWXYZL8А",     0 }, // 3
	// 2013:071 KS only; 2013:177 FIX: SPEAL -> III; 2014:094 +a
	{ "pa",   "н",   NULL,          "z",               INSTANCE_III,   INSTANCE_ADMIN, "AMNOPQ8А",          0 }, // 4
	{ "cm",   "Х¦",  INSTANCE_III,  "efUgijkvwom8",    INSTANCE_SUPER, NULL,           "eUyi05k1wvPom8А",   0 }, // 5
	{ "cm",   "Ь",   INSTANCE_III,  "eMNxPom8",        INSTANCE_SUPER, NULL,           "eMNxPom8А",         0 }, // 6
	{ "cm",   "д±",  NULL,          "z",               INSTANCE_III,   INSTANCE_ADMIN, "eUyi05k1wvPom8А",   2 }, // 7; 2013:178 KS only, like 'p'
	{ "cm",   "н",   NULL,          "z",               INSTANCE_III,   INSTANCE_ADMIN, "eMNxPom8А",         2 }, // 8; 2013:178 KS only, like 'p'
	{ "cm",   "оз",  NULL,          "z",               INSTANCE_SUPER, NULL,           "2f3Ugijk1vw4om8А",  0 }, // 9
	// 2008:310 LRQ; 2013:008 PRQ: (regress FIX?) +INSTANCE_SPEAL
	{ "p",    "о",   INSTANCE_APP,  "AbcdFEL8MNOPQ",   INSTANCE_SUPER, INSTANCE_SPEAL, "AbcdFEL8MNOPQА",    0 }, // 10
	// 2008:028 е separated from оз, INSTANCE_SUPER -> INSTANCE_III
	// 2008:072 re: +INSTANCE_SUPER and results
	{ "cm",   "е",   INSTANCE_III,  "2f3Ugijk1vw4om8", INSTANCE_SUPER, NULL,           "2f3Ugijk1vw4om8А",  1 }, // 11
	{ "cm",   "р",   INSTANCE_III,  "678",             NULL,           NULL,           "z",                 0 }, // 12
	// 2009:169 LPR: removed и, к
	{ "cm",   "‰",   INSTANCE_III,  "z",               NULL,           NULL,           "z",                 0 }, // xD
	// 2001:092; 2001:109; 2006:018 +е
	{ "i",    "Ьв",  INSTANCE_III,  "AMNOPQ8",         INSTANCE_SUPER, NULL,           "AMNOPQ8А",          0 }, // 14
	// 2008:075 separated е be able to skip leading z
	{ "i",    "е",   INSTANCE_III,  "AMNOPQ8",         INSTANCE_SUPER, NULL,           "AMNOPQ8А",          1 }, // 15
	{ "i",    "н",   INSTANCE_III,  "eMNOPQ8",         INSTANCE_SPEAL, NULL,           "eMNOPom8А",         2 }, // 16
	// 2001:094; 2001:109
	{ "r",    "Ьв",  INSTANCE_III,  "AMNOPQ8",         INSTANCE_SUPER, NULL,           "AMNOPQ8А",          0 }, // 17
	{ "r",    "н",   INSTANCE_III,  "eMNOPQ8",         INSTANCE_SPEAL, NULL,           "eMNOPom8А",         2 }, // 18
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
				fprintf(f, "Липсващ %s '%c' (%d)", alias->Name, *s, *s);
		}
	}
	else
		fputs("няма", f);

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

	fprintf(f, "АС: %s + Определение -> %s\n", Kind->Seek(KIND_CONNECT_APPEAL), tKind.Seek(KIND_PRIVATE_CONNECT_APPEAL));
	fputc('\n', f);
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN
	fputs("АдмС:\n\n", f);
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	TKind tKind;

	fprintf(f, "ОС/ВС/РС: %s + Определение -> %s\n", Kind->Seek(KIND_CALL_APPEAL), tKind.Seek(KIND_PRIVATE_CALL_APPEAL));
	fprintf(f, "ОС/ВС/РС: %s + Определение -> %s\n", Kind->Seek(KIND_PROTEST), tKind.Seek(KIND_PRIVATE_PROTEST));
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

		fputs("\tВъззивна инстанция: ", f);

		if (hinst->vz0)
		{
			dump_instance(f, hinst->vz0);

			#if COURT_TYPE == COURT_AREAL
			if (!strcmp(hinst->vz0, INSTANCE_III))
				fputs("  (и АС София)", f);
			#endif  // AREAL
			#if COURT_TYPE == COURT_REGIONAL
			if (!strcmp(hinst->vz0, INSTANCE_III))
				fputs(" (и АдмС Варна)", f);
			#endif  // REGIONAL
		}
		else
			fputs("няма", f);

		fputc('\n', f);

		fputs("\tРезултати от въззивна инстанция:\n\t\t", f);
		dump_alias_set(f, strcmp(hinst->vz_res, C2S[RESOLUTION_FAKE]) ? hinst->vz_res : NULL, Resolution, "\n\t\t", "\n");

		fputs("\tКасационни инстанции: ", f);

		if (hinst->ks0)
		{
			dump_instance(f, hinst->ks0);

		#if COURT_TYPE == COURT_ADMIN
			if (!strcmp(hinst->ks0, INSTANCE_SELF))
				fputs(", ВЪРХОВЕН АДМИНИСТРАТИВЕН СЪД", f);
		#endif  // ADMIN

			if (hinst->ks1)
			{
				fputs(", ", f);
				dump_instance(f, hinst->ks1);
			}
		}
		else
			fputs("няма", f);

		fputc('\n', f);

		fputs("\tРезултати от касационна инстанция:\n\t\t", f);
		dump_alias_set(f, strcmp(hinst->ks_res, C2S[RESOLUTION_FAKE]) ? hinst->ks_res : NULL, Resolution, "\n\t\t", "\n");

		if (hinst->flags & 1)
			fputs("\tДопуска изпращане директно в касационна инстанция\n", f);

		fputc('\n', f);
	}

	fputs("Край\n", f);
	tf.Show(NULL);
}
#endif  // TESTVER
