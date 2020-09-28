DEFINE_RESPONSE_TABLE1(TFloatComboDialog, TFloatSubjectDialog)
//{{TFloatComboDialogRSP_TBL_BEGIN}}
	EV_BN_CLICKED(IDC_FLOAT_LAW_BASIC, BasicBNClicked),
	EV_TCN_SELCHANGE(IDC_FLOAT_COMBO_PAGES, PagesTCNSelChange),
	EV_BN_CLICKED(IDC_FLOAT_DEC_SURROUENDOCS, SurrouendocsBNClicked),
//{{TFloatComboDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TFloatComboDialog Implementation}}

TFloatComboDialog::TFloatComboDialog(TWindow* parent, TFloatComboGroup *group)
:
	TFloatSubjectDialog(parent, group, IDD_FLOAT_COMBO)
{
	constant GRANTING = CourtType == COURT_MILITARY ? "pd..r." : "pd.mr.";
	constant DANGRING = CourtType == COURT_MILITARY ? "..am.." : "..a...";
	constant CHILDREN = any(TYPE_CHILDREN_LAWS) ? "pdamrb" : "";
	constant SUROUEND = can_surrouendoc() ? "..a..." : "......";

	pages = new TTabControl(this, IDC_FLOAT_COMBO_PAGES);
	ok = new TPushButton(this, IDOK);

	// --- Lawsuit ---
	kindsLabel = new TStatic(this, IDC_FLOAT_LAW_KINDS_LABEL);
	kinds = new TCharAutoListFace(this, IDC_FLOAT_LAW_KINDS, "F_KINDS", Kind, Default->xKinds, SIZE_OF_KINDS);
	minNoLabel = new TStatic(this, IDC_FLOAT_LAW_MIN_NO_LABEL);
	minNo = new TLongFace(this, IDC_FLOAT_LAW_MIN_NO, "F_MIN_NO", SIZE_OF_NO);
	maxNoLabel = new TStatic(this, IDC_FLOAT_LAW_MAX_NO_LABEL);
	maxNo = new TLongFace(this, IDC_FLOAT_LAW_MAX_NO, "F_MAX_NO", SIZE_OF_NO);
	minYearLabel = new TStatic(this, IDC_FLOAT_LAW_MIN_YEAR_LABEL);
	minYear = new TYearFace(this, IDC_FLOAT_LAW_MIN_YEAR, "F_MIN_YEAR");
	maxYearLabel = new TStatic(this, IDC_FLOAT_LAW_MAX_YEAR_LABEL);
	maxYear = new TYearFace(this, IDC_FLOAT_LAW_MAX_YEAR, "F_MAX_YEAR");
	collegesLabel = new TStatic(this, IDC_FLOAT_LAW_COLLEGES_LABEL);
	colleges = new TCharAutoListFace(this, IDC_FLOAT_LAW_COLLEGES, "F_COLLEGES", College, QUERY_COLLEGES, SIZE_OF_COLLEGES);
	lawCompositionsLabel = new TStatic(this, IDC_FLOAT_LAW_COMPOSITIONS_LABEL);
	lawCompositions = new TCharAutoListFace(this, IDC_FLOAT_LAW_COMPOSITIONS, "F_LAW_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	firstSet = new TCheckFace(this, IDC_FLOAT_LAW_BASIC, "F_FIRST_SET");
	lawDateLabel = new TStatic(this, IDC_FLOAT_LAW_DATE_LABEL);
	lawMinDate = new TDateFace(this, IDC_FLOAT_LAW_MIN_DATE, "F_LAW_MIN_DATE", false);
	lawMaxDate = new TDateFace(this, IDC_FLOAT_LAW_MAX_DATE, "F_LAW_MAX_DATE", false);
	doublex = new TCheckFace(this, IDC_FLOAT_LAW_DOUBLEX, "F_DOUBLEX");
	subjectText = new TCheckFace(this, IDC_FLOAT_LAW_SUBJECT_TEXT, "F_SUBJECT_TEXT");
	lawFinishedLabel = new TStatic(this, IDC_FLOAT_LAW_FINISHED_LABEL);
	minFinished = new TDateFace(this, IDC_FLOAT_LAW_MIN_FINISHED, "F_MIN_FINISHED", false);
	maxFinished = new TDateFace(this, IDC_FLOAT_LAW_MAX_FINISHED, "F_MAX_FINISHED", false);
	// 2002:289 LRQ: INTER support; 2007:201 LPR: in constructor
	lawJudgesLabel = new TStatic(this, IDC_FLOAT_LAW_JUDGES_LABEL);
	lawJudges = new TUserListBox(this, IDC_FLOAT_LAW_JUDGES, POST_VISIBLE_JUDGES, USER_INTER | USER_VIMIT);
	partof = new TCheckFace(this, IDC_FLOAT_LAW_PART_OF, "F_PART_OF");
	agesLabel = new TStatic(this, IDC_FLOAT_LAW_AGES_LABEL);
	ages = new TCharListFace(this, IDC_FLOAT_LAW_AGES, "F_AGES", Age, AGE_AGES, SIZE_OF_AGES); // 2010:197 FIX: was NULL
	lawForceDateLabel = new TStatic(this, IDC_FLOAT_LAW_FORCE_DATE_LABEL);
	lawMinForceDate = new TDateFace(this, IDC_FLOAT_LAW_MIN_FORCE_DATE, "F_LAW_MIN_FORCE_DATE", false);
	lawMaxForceDate = new TDateFace(this, IDC_FLOAT_LAW_MAX_FORCE_DATE, "F_LAW_MAX_FORCE_DATE", false);
	reregisterLabel = new TStatic(this, IDC_FLOAT_LAW_REREGISTER_LABEL);
	minReregister = new TDateFace(this, IDC_FLOAT_LAW_MIN_REREGISTER, "F_MIN_REREGISTER", false);
	maxReregister = new TDateFace(this, IDC_FLOAT_LAW_MAX_REREGISTER, "F_MAX_REREGISTER", false);
	grantsLabel = new TStatic(this, IDC_FLOAT_LAW_GRANTS_LABEL);
	grants = new TCharListFace(this, IDC_FLOAT_LAW_GRANTS, "F_GRANTS", Grant, NULL, SIZE_OF_GRANTS);
	dangersLabel = new TStatic(this, IDC_FLOAT_LAW_DANGERS_LABEL);
	dangers = new TDangerListBox(this, IDC_FLOAT_LAW_DANGERS);
	generalKindsLabel = new TStatic(this, IDC_FLOAT_LAW_GENERAL_KINDS_LABEL);
	generalKinds = new TCharListFace(this, IDC_FLOAT_LAW_GENERAL_KINDS, "F_GENERAL_KINDS", Kind, KIND_ALL_GENERALS, SIZE_OF_KINDS);
	speedGroup = new TGroupBox(this, IDC_FLOAT_LAW_RADIOBOX_S_LABEL);
	slow = new TRadioFace(this, IDC_FLOAT_LAW_SLOW, "F_SLOW");
	quick = new TRadioFace(this, IDC_FLOAT_LAW_QUICK, "F_QUICK");
	immediate = new TRadioFace(this, IDC_FLOAT_LAW_IMMEDIATE, "F_IMMEDIATE");
	anySpeed = new TRadioFace(this, IDC_FLOAT_LAW_ANY_SPEED, "F_ANY_SPEED");
	lawTextLabel = new TStatic(this, IDC_FLOAT_LAW_TEXT_LABEL);
	lawText = new TStringFace(this, IDC_FLOAT_LAW_TEXT, "F_TEXT", SIZE_OF_ORIGIN_TEXT);
	cutDownGroup = new TGroupBox(this, IDC_FLOAT_LAW_CUTDOWN_GROUP);
	important = new TCheckFace(this, IDC_FLOAT_LAW_IMPORTANT, "F_IMPORTANT");
	newGPK = new TCheckFace(this, IDC_FLOAT_LAW_NEWGPK, "F_NEWGPK");
	limited = new TCheckFace(this, IDC_FLOAT_LAW_LIMITED, "F_LIMITED");
	lawOrderGroup = new TGroupBox(this, IDC_FLOAT_LAW_RADIOBOX_LABEL);
	lawByTYN = new TRadioFace(this, IDC_FLOAT_LAW_BY_TYN, "F_LAW_BY_TYN");
	lawByTSYN = new TRadioFace(this, IDC_FLOAT_LAW_BY_TSYN, "F_LAW_BY_TSYN");
	lawByTKYN = new TRadioFace(this, IDC_FLOAT_LAW_BY_TKYN, "F_LAW_BY_TKYN");
	lawByJTYN = new TRadioFace(this, IDC_FLOAT_LAW_BY_JTYN, "F_LAW_BY_JTYN");
	eisppNmrLabel = new TStatic(this, IDC_FLOAT_LAW_EISPP_NMR_LABEL);
	eisppNmr = new TEisPartFace(this, IDC_FLOAT_LAW_EISPP_NMR, "F_EISPP_NMR");
	cutDownAny = new TRadioFace(this, IDC_FLOAT_LAW_CUTDOWN_ANY, "F_CUTDOWN_ANY");
	cutDownCutDown = new TRadioFace(this, IDC_FLOAT_LAW_CUTDOWN_CUTDOWN, "F_CUTDOWN_CUTDOWN");
	cutDownUnspecified = new TRadioFace(this, IDC_FLOAT_LAW_CUTDOWN_UNSPECIFIED, "F_CUTDOWN_UNSPECIFIED");
	cutDownP1 = new TRadioFace(this, IDC_FLOAT_LAW_CUTDOWN_P1, "F_CUTDOWN_P1");
	cutDownP2 = new TRadioFace(this, IDC_FLOAT_LAW_CUTDOWN_P2, "F_CUTDOWN_P2");
	cutDownP1_P2 = new TRadioFace(this, IDC_FLOAT_LAW_CUTDOWN_P1_AND_P2, "F_CUTDOWN_P1_AND_P2");
	cutDownNotCutDown = new TRadioFace(this, IDC_FLOAT_LAW_CUTDOWN_NOT_CUTDOWN, "F_CUTDOWN_NOT_CUTDOWN");
	lawChildren = new TCheckFace(this, IDC_FLOAT_LAW_CHILDREN, "F_CHILDREN");
	lawsuitText = new TCheckFace(this, IDC_FLOAT_LAW_LAWSUIT_TEXT, "F_LAWSUIT_TEXT");
	pt329a3 = new TCheckFace(this, IDC_FLOAT_LAW_PT329A3, "F_PT329A3");

	// --- Session ---
	sesKindsLabel = new TStatic(this, IDC_FLOAT_SES_SES_KINDS_LABEL);
	sesKinds = new TCharListFace(this, IDC_FLOAT_SES_SES_KINDS, "F_SES_KINDS", Kind, KIND_VISIBLE_SESSIONS, SIZE_OF_KINDS);
	sesMinDateLabel = new TStatic(this, IDC_FLOAT_SES_MIN_DATE_LABEL);
	sesMinDate = new TDateFace(this, IDC_FLOAT_SES_MIN_DATE, "F_SES_MIN_DATE", false);
	sesMaxDateLabel = new TStatic(this, IDC_FLOAT_SES_MAX_DATE_LABEL);
	sesMaxDate = new TDateFace(this, IDC_FLOAT_SES_MAX_DATE, "F_SES_MAX_DATE", false);
	sesMinReturnedLabel = new TStatic(this, IDC_FLOAT_SES_MIN_RETURNED_LABEL);
	sesMinReturned = new TDateFace(this, IDC_FLOAT_SES_MIN_RETURNED, "F_SES_MIN_RETURNED", false);
	sesMaxReturnedLabel = new TStatic(this, IDC_FLOAT_SES_MAX_RETURNED_LABEL);
	sesMaxReturned = new TDateFace(this, IDC_FLOAT_SES_MAX_RETURNED, "F_SES_MAX_RETURNED", false);
	sesEventNoLabel = new TStatic(this, IDC_FLOAT_SES_EVENT_NO_LABEL);
	sesMinEventNo = new TLongFace(this, IDC_FLOAT_SES_MIN_EVENT_NO, "F_SES_MIN_EVENT_NO", SIZE_OF_EVENT_NO);
	sesMaxEventNo = new TLongFace(this, IDC_FLOAT_SES_MAX_EVENT_NO, "F_SES_MAX_EVENT_NO", SIZE_OF_EVENT_NO);
	sesJudgesLabel = new TStatic(this, IDC_FLOAT_SES_JUDGES_LABEL);
	sesJudges = new TUserListBox(this, IDC_FLOAT_SES_JUDGES, POST_VISIBLE_JUDGES, USER_VIMIT);
	sesCompositionsLabel = new TStatic(this, IDC_FLOAT_SES_COMPOSITIONS_LABEL);
	sesCompositions = new TCharListFace(this, IDC_FLOAT_SES_COMPOSITIONS, "F_SES_COMPOSITIONS", CharComposition,
		QUERY_COMPOSITIONS, SIZE_OF_COMPOSITIONS);
	sesResultsLabel = new TStatic(this, IDC_FLOAT_SES_RESULTS_LABEL);
	sesResults = new TCharListNameFace(this, IDC_FLOAT_SES_RESULTS, "F_SES_RESULTS", Result, RESULT_SESSIONS,
		SIZE_OF_RESULTS);
	sesJudgeGroup = new TGroupBox(this, IDC_FLOAT_SES_RADIOBOX_LABEL);
	asJudge = new TRadioFace(this, IDC_FLOAT_SES_AS_JUDGE, "F_AS_JUDGE");
	asAny = new TRadioFace(this, IDC_FLOAT_SES_AS_ANY, "F_AS_ANY");
	secretarsLabel = new TStatic(this, IDC_FLOAT_SES_SECRETARS_LABEL);
	secretars = new TUserListBox(this, IDC_FLOAT_SES_SECRETARS, POST_SECRETARS);
	jurorsLabel = new TStatic(this, IDC_FLOAT_SES_JURORS_LABEL);
	jurors = new TKnownPersonListBox(this, IDC_FLOAT_SES_JURORS, INVOLVEMENT_JUROR);
	sessionChanjed = new TCheckFace(this, IDC_FLOAT_SES_SESSION_CHANJED, "F_SESSION_CHANJED");
	proceed = new TCheckFace(this, IDC_FLOAT_SES_PROCEED, "F_PROCEED");
	closedd = new TCheckFace(this, IDC_FLOAT_SES_CLOSEDD, "F_CLOSEDD");
	extended = new TCheckFace(this, IDC_FLOAT_SES_EXTENDED, "F_EXTENDED");
	firstSes = new TCheckFace(this, IDC_FLOAT_SES_FIRST_SES, "F_FIRST_SES");
	expert = new TCheckFace(this, IDC_FLOAT_SES_EXPERT, "F_EXPERT");
	sesTextLabel = new TStatic(this, IDC_FLOAT_SES_TEXT_LABEL);
	sesText = new TStringFace(this, IDC_FLOAT_SES_TEXT, "F_SES_TEXT", SIZE_OF_SESSION_TEXT);
	sesDateAfterFinish = new TCheckFace(this, IDC_FLOAT_SES_DATE_AFTER_FINISH, "F_SES_DATE_AFTER_FINISH");
	sesReturnAfterFinish = new TCheckFace(this, IDC_FLOAT_SES_RETURN_AFTER_FINISH, "F_SES_RETURN_AFTER_FINISH");
	sesForceDateLabel = new TStatic(this, IDC_FLOAT_SES_FORCE_DATE_LABEL),
	sesMinForceDate = new TDateFace(this, IDC_FLOAT_SES_MIN_FORCE_DATE, "F_SES_MIN_FORCE_DATE", false);
	sesMaxForceDate = new TDateFace(this, IDC_FLOAT_SES_MAX_FORCE_DATE, "F_SES_MAX_FORCE_DATE", false);
	sesFinalizing = new TCheckFace(this, IDC_FLOAT_SES_FINALIZING, "F_SES_FINALIZING");
	sesAppeal = COURT_LOCK ? sesFinalizing : new TCheckFace(this, IDC_FLOAT_SES_APPEAL, "F_SES_APPEAL");
	sesSLCReasonsLabel = new TStatic(this, IDC_FLOAT_SES_SLC_REASONS_LABEL);
	sesSLCReasons = new TSLCReasonListBox(this, IDC_FLOAT_SES_SLC_REASONS, SLC_TYPE_STOPPED_OR_LEFTS, ALIAS_EMPTY);
	sesPublTreg = new TCheckFace(this, IDC_FLOAT_SES_PUBLTREG, "F_SES_PUBLTREG");

	// --- Decision ---
	decResultsLabel = new TStatic(this, IDC_FLOAT_DEC_RESULTS_LABEL);
	decResults = new TCharListNameFace(this, IDC_FLOAT_DEC_RESULTS, "F_DEC_RESULTS", Result, RESULT_ENDOCS, SIZE_OF_RESULTS);
	decSurrouendocs = new TCheckFace(this, IDC_FLOAT_DEC_SURROUENDOCS, "F_DEC_SURROUENDOCS");
	fromDateLabel = new TStatic(this, IDC_FLOAT_DEC_FROM_DATE_LABEL);
	minFromDate = new TDateFace(this, IDC_FLOAT_DEC_MIN_FROM_DATE, "F_MIN_FROM_DATE", false);
	maxFromDate = new TDateFace(this, IDC_FLOAT_DEC_MAX_FROM_DATE, "F_MAX_FROM_DATE", false);
	decForceDateLabel = new TStatic(this, IDC_FLOAT_DEC_FORCE_DATE_LABEL);
	decMinForceDate = new TDateFace(this, IDC_FLOAT_DEC_MIN_FORCE_DATE, "F_DEC_MIN_FORCE_DATE", false);
	decMaxForceDate = new TDateFace(this, IDC_FLOAT_DEC_MAX_FORCE_DATE, "F_DEC_MAX_FORCE_DATE", false);
	decKindsLabel = new TStatic(this, IDC_FLOAT_DEC_DEC_KINDS_LABEL);
	decKinds = new TCharListFace(this, IDC_FLOAT_DEC_DEC_KINDS, "F_DEC_KINDS", Kind, KIND_VISIBLE_ENDOCS, SIZE_OF_KINDS);
	accomplysLabel = new TStatic(this, IDC_FLOAT_DEC_ACT_ACCOMPLYS_LABEL);
	accomplys = new TCharListFace(this, IDC_FLOAT_DEC_ACT_ACCOMPLYS, "F_ACCOMPLYS", Accomply, ACCOMPLY_INCONSISTENCES, SIZE_OF_ACCOMPLYS);
	returnedLabel = new TStatic(this, IDC_FLOAT_DEC_RETURNED_LABEL);
	minReturned = new TDateFace(this, IDC_FLOAT_DEC_MIN_RETURNED, "F_MIN_RETURNED", false);
	maxReturned = new TDateFace(this, IDC_FLOAT_DEC_MAX_RETURNED, "F_MAX_RETURNED", false);
	confess = new TCheckFace(this, IDC_FLOAT_DEC_CONFESS, "F_CONFESS");
	partitionLabel = new TStatic(this, IDC_FLOAT_DEC_PARTITION_LABEL);
	minPartition = new TLongFace(this, IDC_FLOAT_DEC_MIN_PARTITION, "F_MIN_PARTITION", SIZE_OF_NO);
	maxPartition = new TLongFace(this, IDC_FLOAT_DEC_MAX_PARTITION, "F_MAX_PARTITION", SIZE_OF_NO);
	minVolumeLabel = new TStatic(this, IDC_FLOAT_DEC_MIN_VOLUME_LABEL);
	minVolume = new TLongFace(this, IDC_FLOAT_DEC_MIN_VOLUME, "F_MIN_VOLUME", SIZE_OF_NO);
	maxVolumeLabel = new TStatic(this, IDC_FLOAT_DEC_MAX_VOLUME_LABEL);
	maxVolume = new TLongFace(this, IDC_FLOAT_DEC_MAX_VOLUME, "F_MAX_VOLUME", SIZE_OF_NO);
	decTextLabel = new TStatic(this, IDC_FLOAT_DEC_TEXT_LABEL);
	decText = new TStringFace(this, IDC_FLOAT_DEC_TEXT, "F_DEC_TEXT", SIZE_OF_DECISION_TEXT);
	motives = new TCheckFace(this, IDC_FLOAT_DEC_MOTIVES, "F_MOTIVES");
	absence = new TCheckFace(this, IDC_FLOAT_DEC_ABSENCE, "F_ABSENCE");
	opinion = new TCheckFace(this, IDC_FLOAT_DEC_OPINION, "F_OPINION");
	debtorStatusLabel = new TStatic(this, IDC_FLOAT_DEC_DEBTOR_STATUS_LABEL);
	debtorStatus = new TCharListFace(this, IDC_FLOAT_DEC_DEBTOR_STATUS, "F_DEBTOR_STATUS", DebtorStatus, DEBTOR_STATUS_ALLS, SIZE_OF_KINDS);
	decAppeal = COURT_LOCK ? absence : new TCheckFace(this, IDC_FLOAT_DEC_APPEAL, "F_DEC_APPEAL");
	decSLCReasonsLabel = new TStatic(this, IDC_FLOAT_DEC_SLC_REASONS_LABEL);
	decSLCReasons = new TSLCReasonListBox(this, IDC_FLOAT_DEC_SLC_REASONS, C2S[SLC_TYPE_CANCELED], ALIAS_EMPTY);
	decPublTreg = new TCheckFace(this, IDC_FLOAT_DEC_PUBLTREG, "F_DEC_PUBLTREG");
	decContract = new TCheckFace(this, IDC_FLOAT_DEC_CONTRACT, "F_DEC_CONTRACT");
	keyWordsLabel = new TStatic(this, IDC_FLOAT_DEC_KEYWORDS_LABEL);
	keyWords = new TKeyWordsFace(this, IDC_FLOAT_DEC_KEYWORDS, "F_KEYWORDS");

	// --- Judgement ---
	judgedsLabel = new TStatic(this, IDC_FLOAT_JUD_JUDGEDS_LABEL);
	judgeds = new TCharListFace(this, IDC_FLOAT_JUD_JUDGEDS, "F_JUDGEDS", Judged, JUDGED_ALLS, SIZE_OF_JUDGEDS);
	articlesLabel = new TStatic(this, IDC_FLOAT_JUD_ARTICLES_LABEL);
	articles = new TAliasListBox(this, IDC_FLOAT_JUD_ARTICLES, Article, NULL, FLAG_NULL);
	appendix24 = new TCheckFace(this, IDC_FLOAT_JUD_APPENDIX_24, "F_APPENDIX_24");
	appendix25 = new TCheckFace(this, IDC_FLOAT_JUD_APPENDIX_25, "F_APPENDIX_25");
	appendix26 = new TCheckFace(this, IDC_FLOAT_JUD_APPENDIX_26, "F_APPENDIX_26");
	punishmentsLabel = new TStatic(this, IDC_FLOAT_JUD_PUNISHMENTS_LABEL);
	punishments = new TCharListFace(this, IDC_FLOAT_JUD_PUNISHMENTS, "F_PUNISHMENTS", Punishment, PUNISHMENT_BASICS, SIZE_OF_PUNISHMENTS);
	intervalsLabel = new TStatic(this, IDC_FLOAT_JUD_INTERVALS_LABEL);
	intervals = new TCharListFace(this, IDC_FLOAT_JUD_INTERVALS, "F_INTERVALS", Interval, INTERVAL_DOCS, SIZE_OF_INTERVALS);
	sendLabel = new TStatic(this, IDC_FLOAT_JUD_SEND_LABEL);
	minSend = new TDateFace(this, IDC_FLOAT_JUD_MIN_SEND, "F_MIN_SEND", false);
	maxSend = new TDateFace(this, IDC_FLOAT_JUD_MAX_SEND, "F_MAX_SEND", false);
	forceDateLabel = new TStatic(this, IDC_FLOAT_JUD_FORCE_DATE_LABEL);
	minForceDate = new TDateFace(this, IDC_FLOAT_JUD_MIN_FORCE_DATE, "F_MIN_FORCE_DATE", false);
	maxForceDate = new TDateFace(this, IDC_FLOAT_JUD_MAX_FORCE_DATE, "F_MAX_FORCE_DATE", false);
	judOrderGroup = new TGroupBox(this, IDC_FLOAT_JUD_BY);
	judByTYN = new TRadioFace(this, IDC_FLOAT_JUD_BY_TYN, "F_BY_TYN");
	judByART = new TRadioFace(this, IDC_FLOAT_JUD_BY_ART, "F_BY_ART");
	judByForce = new TRadioFace(this, IDC_FLOAT_JUD_BY_FORCE, "F_BY_FORCE");
	judBySend = new TRadioFace(this, IDC_FLOAT_JUD_BY_SEND, "F_BY_SEND");
	appendix23 = new TCheckFace(this, IDC_FLOAT_JUD_APPENDIX_23, "F_APPENDIX_23");
	judYoung = new TCheckFace(this, IDC_FLOAT_JUD_YOUNG, "F_JUD_YOUNG");
	punishmentsXLabel = new TStatic(this, IDC_FLOAT_JUD_PUNISHMENTS_X_LABEL);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	punishmentsX = new TCharListFace(this, IDC_FLOAT_JUD_PUNISHMENTS_X, "F_PUNISHMENTS_X", Punishment, PUNISHMENT_EXTRAS, SIZE_OF_PUNISHMENTS);
#endif  // APPEAL || AREAL || MILITARY
#if COURT_TYPE == COURT_REGIONAL
	punishmentsX = new TCharListFace(this, IDC_FLOAT_JUD_PUNISHMENTS_X, "F_PUNISHMENTS_X", Punishment, PUNISHMENT_EXTRA_OTHERS, SIZE_OF_PUNISHMENTS);
#endif  // REGIONAL
	reportLabel = new TStatic(this, IDC_FLOAT_JUD_REPORT_LABEL);
	reportIgnore = new TRadioFace(this, IDC_FLOAT_JUD_REPORT_IGNORE, "F_JUD_REPORT_IGNORE");
	reportCounted = new TRadioFace(this, IDC_FLOAT_JUD_REPORT_COUNTED, "F_JUD_REPORT_COUNTED");
	reportUncounted = new TRadioFace(this, IDC_FLOAT_JUD_REPORT_UNCOUNTED, "F_JUD_REPORT_UNCOUNTED");

	TViewControl vcLawsuits[] =
	{
		{ kindsLabel,		"pdamrb" }, { kinds,			"pdamrb" }, { minNoLabel,		"pdamrb" },
		{ minNo,			"pdamrb" }, { maxNoLabel,		"pdamrb" }, { maxNo,			"pdamrb" },
		{ minYearLabel,		"pdamrb" }, { minYear,			"pdamrb" }, { maxYearLabel,		"pdamrb" },
		{ maxYear,			"pdamrb" }, { collegesLabel,		"pdamrb" }, { colleges,			"pdamrb" },
		{ lawCompositionsLabel,	"pdamrb" }, { lawCompositions,	"pdamrb" }, { firstSet,			"pdamrb" },
		{ lawDateLabel,		"pdamrb" }, { lawMinDate,		"pdamrb" }, { lawMaxDate,		"pdamrb" },
		{ doublex,			"..a.r." }, { lawFinishedLabel,	"pdamrb" }, { minFinished,		"pdamrb" },
		{ maxFinished,		"pdamrb" }, { pushFull,			"pdamrb" }, { checkOld,			"pdamrb" },
		{ lawJudgesLabel,		"pdamrb" }, { lawJudges,		"pdamrb" }, { partof,			"pdamrb" },
		{ agesLabel,		"pdamrb" }, { ages,			"pdamrb" }, { lawForceDateLabel,	"pdamr." },
		{ lawMinForceDate,	"pdamr." }, { lawMaxForceDate,	"pdamr." }, { reregisterLabel,	"..a..." },
		{ minReregister,		"..a..." }, { maxReregister,		"..a..." }, { grantsLabel,		GRANTING },
		{ grants,			GRANTING }, { dangersLabel,		DANGRING }, { dangers,			DANGRING },
		{ generalKindsLabel,	"pdamrb" }, { generalKinds,		"pdamrb" }, { speedGroup,		"pdamrb" },
		{ slow,			"pdamrb" }, { quick,			"pdamrb" }, { immediate,		"pdamr." },
		{ anySpeed,			"pdamrb" }, { lawTextLabel,		"pdamrb" }, { lawText,			"pdamrb" },
		{ cutDownGroup,		"..amr." }, { important,		"pdamrb" }, { newGPK,			"pdamr." },
		{ limited,			"pdamrb" }, { lawOrderGroup,		"pdamrb" }, { lawByTYN,			"pdamrb" },
		{ lawByTSYN,		"pdamrb" }, { lawByTKYN,		"pdamrb" }, { lawByJTYN,		"pdamrb" },
		{ eisppNmrLabel,		"p.amrb" }, { eisppNmr,			"p.amrb" }, { cutDownAny,		"..amr." },
		{ cutDownCutDown,		"..amr." }, { cutDownUnspecified,	"..amr." }, { cutDownP1,		"..amr." },
		{ cutDownP2,		"..amr." }, { cutDownP1_P2,		"..amr." }, { cutDownNotCutDown,	"..amr." },
		{ subjectsNew,		"pdamrb" }, { pushFull,			"pdamrb" }, { checkOld,			"pdamrb" },
		{ subjectsOld,		"pdamrb" }, { lawChildren,		CHILDREN }, { subjectText,		"pdamrb" },
		{ lawsuitText,		"pdamrb" }, { pt329a3,			"pda.rb" },
		{ 0, NULL }
	};

	viewControls[TAB_LAWSUIT] = copy_controls(vcLawsuits);

	TViewControl vcSessions[] =
	{
		{ sesKindsLabel,		"pdamrb" }, { sesKinds, 		"pdamrb" }, { sesMinDateLabel,	"pdamrb" },
		{ sesMinDate,		"pdamrb" }, { sesMaxDateLabel,	"pdamrb" }, { sesMaxDate,		"pdamrb" },
		{ sesDateAfterFinish,	"pdamrb" }, { sesReturnAfterFinish,	"pdamrb" }, { sesMinReturnedLabel,	"pdamrb" },
		{ sesMinReturned,		"pdamrb" }, { sesMaxReturnedLabel,	"pdamrb" }, { sesMaxReturned,		"pdamrb" },
		{ sesEventNoLabel,	"pdamrb" }, { sesMinEventNo,		"pdamrb" }, { sesMaxEventNo,		"pdamrb" },
		{ sesJudgesLabel,		"pdamrb" }, { sesJudges,		"pdamrb" }, { sesCompositionsLabel,	"pdamrb" },
		{ sesCompositions,	"pdamrb" }, { sesResultsLabel,	"pdamrb" }, { sesResults,		"pdamrb" },
		{ sesJudgeGroup,		"pdamrb" }, { asJudge,			"pdamrb" }, { asAny,			"pdamrb" },
		{ secretarsLabel,		"pdamrb" }, { secretars,		"pdamrb" }, { jurorsLabel,		"..amr." },
		{ jurors,			"..amr." }, { sessionChanjed,		"pdamrb" },	{ proceed,			"pdamr." },
		{ closedd,			"pdamrb" },	{ extended,			"pdamrb" }, { firstSes,			"pda.r." },
		{ expert,			"..a.r." }, { sesTextLabel,		"pdamrb" },	{ sesText,			"pdamrb" },
		{ sesForceDateLabel,	"pdamrb" }, { sesMinForceDate,	"pdamrb" }, { sesMaxForceDate,	"pdamrb" },
		{ sesFinalizing,		"pdamrb" }, { sesAppeal,		"pdamrb" }, { sesSLCReasonsLabel,	"pdamrb" },
		{ sesSLCReasons,		"pdamrb" }, { sesPublTreg,		"p.a..." },
		{ 0, NULL }
	};

	viewControls[TAB_SESSION] = copy_controls(vcSessions);

	TViewControl vcDecisions[] =
	{
		{ decResultsLabel,	"pdamrb" }, { decResults,		"pdamrb" }, { fromDateLabel,		"pdamrb" },
		{ minFromDate,		"pdamrb" }, { maxFromDate,		"pdamrb" }, { decForceDateLabel,	"pdamr." },
		{ decMinForceDate,	"pdamr." }, { decMaxForceDate,	"pdamr." }, { decKindsLabel,		"pdamrb" },
		{ decKinds,			"pdamrb" }, { accomplysLabel,		"..a..." }, { accomplys,		"..a..." },
		{ returnedLabel,		"pdamrb" }, { minReturned,		"pdamrb" }, { maxReturned,		"pdamrb" },
		{ confess,			"pdamr." }, { partitionLabel,		"pdamrb" }, { minPartition,		"pdamrb" },
		{ maxPartition,		"pdamrb" }, { minVolumeLabel,		"pdamrb" }, { minVolume,		"pdamrb" },
		{ maxVolumeLabel,		"pdamrb" }, { maxVolume,		"pdamrb" }, { decTextLabel,		"pdamrb" },
		{ decText,			"pdamrb" }, { motives,			"p.amrb" }, { absence,			"pdamr." },
		{ opinion,			"pdamrb" }, { debtorStatusLabel,	"..a..." }, { debtorStatus,		"..a..." },
		{ decAppeal,		"pdamrb" }, { decSLCReasonsLabel,	"pdamrb" }, { decSLCReasons,		"pdamrb" },
		{ decSurrouendocs,	SUROUEND },	{ decPublTreg,		"p.a..." }, { decContract,		"p.a..." },
		{ keyWordsLabel,		"pdamrb" },	{ keyWords,			"pdamrb" },
		{ 0, NULL }
	};

	viewControls[TAB_DECISION] = copy_controls(vcDecisions);

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	TViewControl vcJudgements[] =
	{
		{ judgedsLabel,		"pdamrb" }, { judgeds,			"pdamrb" },
		{ articlesLabel,		"pdamrb" }, { articles,			"pdamrb" },
		{ appendix24,		"pdamrb" }, { appendix25,		"pdamrb" },
		{ appendix26,		"pdamrb" }, { punishmentsLabel,	"pdamrb" },
		{ punishments,		"pdamrb" }, { intervalsLabel,		"pdamrb" },
		{ intervals,		"pdamrb" }, { sendLabel,		"pdamrb" },
		{ minSend,			"pdamrb" }, { maxSend,			"pdamrb" },
		{ forceDateLabel,		"pdamrb" }, { minForceDate,		"pdamrb" },
		{ maxForceDate,		"pdamrb" }, { judOrderGroup,		"pdamrb" },
		{ judByTYN,			"pdamrb" }, { judByART,			"pdamrb" },
		{ judByForce,		"pdamrb" }, { judBySend,		"pdamrb" },
		{ appendix23,		"pdamrb" }, { judYoung,			"pdamrb" },
		{ punishmentsXLabel,	"pdamrb" }, { punishmentsX,		"pdamrb" },
		{ reportLabel,		"pdamrb" }, { reportIgnore,		"pdamrb" },
		{ reportCounted,		"pdamrb" }, { reportUncounted,	"pdamrb" },
		{ 0, NULL }
	};

	viewControls[TAB_JUDGEMENT] = copy_controls(vcJudgements);
#endif  // AREAL || MILITARY || REGIONAL
}

TFloatComboDialog::~TFloatComboDialog()
{
	for (int page = 0; page < PAGE_COUNT; page++)
		delete[] viewControls[page];
}

bool TFloatComboDialog::IsValid()
{
	bool isValid = TFloatSubjectDialog::IsValid();

	if (isValid)
	{
		if (invalid_range(minNo, maxNo))
			isValid = error("Началния номер дело е по-голям от крайния.");
		else if (invalid_range(minYear, maxYear))
			isValid = error("Началната година е по-голяма от крайната.");
		else if (invalid_range(lawMinDate, lawMaxDate))
			isValid = error("Началната дата на образуване е по-голяма от крайната.");
		else if (invalid_range(minFinished, maxFinished))
			isValid = error("Началната дата на свършване е по-голяма от крайната.");
		else if (invalid_range(sesMinDate, sesMaxDate))
			isValid = error("Началната дата е по-голяма от крайната.");
		else if (invalid_range(sesMinEventNo, sesMaxEventNo))
			isValid = error("Началния номер на съдебен акт е по-голям от крайния.");
		else if (invalid_range(sesMinForceDate, sesMaxForceDate))
			isValid = error("Началната дата на влизане в законна сила е по-голяма от крайната.");
		else if (invalid_range(minReturned, maxReturned))
			isValid = error("Началната дата на връщане е по-голяма от крайната.");
		else if (invalid_range(minFromDate, maxFromDate))
			isValid = error("Началната дата на съдебния акт е по-голяма от крайната.");
		else if (invalid_range(decMinForceDate, decMaxForceDate))
			isValid = error("Началната дата на влизане в законна сила е по-голяма от крайната.");
		else if (invalid_range(minPartition, maxPartition))
			isValid = error("Началния номер от регистъра е по-голям от крайния.");
		else if (invalid_range(minVolume, maxVolume))
			isValid = error("Началния том е по-голям от крайния.");
		else if (invalid_range(minSend, maxSend))
			isValid = error("Началната дата на изпращане е по-голяма от крайната.");
		else if (invalid_range(minForceDate, maxForceDate))
			isValid = error("Началната дата на влизане в законна сила е по-голяма от крайната.");
	}

	return isValid;
}

void TFloatComboDialog::Action(bool execute)
{
	msql m;
	TFloatComboGroup *comboGroup = FloatComboGroup();

	TFloatSubjectDialog::GetSubjects(comboGroup->newSubjects, comboGroup->oldSubjects);
	lawJudges->GetMark(&comboGroup->lawJudges);
	sesJudges->GetMark(&comboGroup->sesJudges);
	articles->GetMark(&comboGroup->judArticles);
	secretars->GetMark(&comboGroup->secretars);
	jurors->GetMark(&comboGroup->jurors);
	dangers->GetMark(&comboGroup->dangers);
	sesSLCReasons->GetMark(&comboGroup->sesSLCReasons);
	decSLCReasons->GetMark(&comboGroup->decSLCReasons);

	comboGroup->FormCriteria(m);

	if (execute)
		query_tab(this, str(m), NULL, comboGroup->variantReg, pages->GetSel(), comboGroup->partof);
	else
		Message(m);
}

void TFloatComboDialog::SetupWindow()
{
	TFloatSubjectDialog::SetupWindow();

#if COURT_TYPE == COURT_ADMIN
	returnedLabel->SetText("\nДата на връщане");
#endif  // ADMIN

#if COURT_TYPE == COURT_REGIONAL
	lawChildren->SetCaption("Специализиран състав");
#endif  // REGIONAL

	for (int i = 0; i < PAGE_COUNT; i++)
		pages->Add(FloatPages[i].tabTitle);

	pages->SetSel(TAB_LAWSUIT);
	PagesTCNSelChange(TNotify());
	BasicBNClicked();
}

void TFloatComboDialog::BasicBNClicked()
{
	switch (firstSet->GetCheck())
	{
		case BF_CHECKED : lawDateLabel->SetText("\n&Образувано в периода"); break;
		case BF_UNCHECKED : lawDateLabel->SetText("\nЗап&очнало в периода"); break;
		default : lawDateLabel->SetText("&Образувано или\nзапочнало в периода");
	}
}

void TFloatComboDialog::PagesTCNSelChange(TNotify far&)
{
	FloatComboGroup()->tabSelect = pages->GetSel();

	for (int page = 0; page < PAGE_COUNT; page++)
	{
		bool precond = page == FloatComboGroup()->tabSelect;

		for (const TViewControl *vc = viewControls[page]; vc->control; vc++)
		{
			bool visible = precond && strchr(vc->courts, COURT_LETTER);

			vc->control->ShowWindow(visible ? SW_SHOW : SW_HIDE);
			vc->control->EnableWindow(visible);
		}
	}

	if (FloatComboGroup()->tabSelect == TAB_LAWSUIT)
		CheckOldBNClicked();

#if COURT_TYPE == COURT_AREAL
	if (FloatComboGroup()->tabSelect >= TAB_DECISION)
		SurrouendocsBNClicked();
	else
		ok->EnableWindow(true);
#endif  // AREAL
}

void TFloatComboDialog::SurrouendocsBNClicked()
{
	bool surrouendocs = decSurrouendocs->GetCheck() == BF_CHECKED;
	TControl *ignoreds[] = { decSurrouendocs, minFromDate, maxFromDate, decKinds, decText, decPublTreg, NULL };

	for (const TViewControl *vc = viewControls[FloatComboGroup()->tabSelect]; vc->control; vc++)
	{
		TControl *ignored;

		for (int i = 0; (ignored = ignoreds[i]) != NULL; i++)
			if (ignored == vc->control)
				break;

		if (ignored)
			continue;

		char className[0x10];

		if (vc->control->GetClassName(className, sizeof className) && !strcmpi(className, "static"))
			continue;

		vc->control->EnableWindow(!surrouendocs);
	}

	ok->EnableWindow(!surrouendocs || FloatComboGroup()->tabSelect != TAB_JUDGEMENT);
	decKinds->SetXCrit(surrouendocs ? KIND_SURROUENDOC_ACTS : KIND_VISIBLE_ENDOCS);
}
