// ----- TTitleGroup ---------------------------------------------------------
class TTitleGroup : public TDummyGroup
{
public:
	TTitleGroup(const char *tTitle);

	char title[0x100];
};

extern const offield counter_fields[];	// "F_COUNT" count, "F_MATCHES" with text
void counter_matches(mstr &m, TGroup *group);

// 2008:064 LPR: moved to offields; N.B. should be used by functions only
extern char *AppNameCourt;
extern char AppNameTitle[SIZE_OF_APPNAME_TITLE];
extern char InstanceIII[SIZE_OF_COURT_NAME];	// empty for APPEAL || ADMIN
extern char InstanceSuper[SIZE_OF_COURT_NAME];
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
extern char InstanceSpeal[SIZE_OF_COURT_NAME];
#endif  // APPEAL || ADMIN || AREAL || REGIONAL

void reset_user_names();
void reset_cache_data();

void print_types_list(mstr &m, const char *types, bool college);
void print_compositions_list(mstr &m, const char *compositions, const char *suffix = NULL);
void print_kinds_list(mstr &m, const char *kinds, bool types, bool college = false);
void print_college(mstr &m, char college);
void print_composition(mstr &m, long composition, const char *suffix = NULL);
void print_sender(mstr &m, char ucnType, const char *ucn);
void print_type_or_kind(mstr &m, TRCDKey &key);
// 2008:280 LRQ: extra +related; 2011:283 _tostr -> offields
void print_link_address(mstr &m, const TLinkWind &link, long address = 1, long flags = 0);
// 2011:283 LPR; may remain the only variant someday
void print_link_addresses(mstr &m, const TLinkWind &link, const char *sep, long flags = 0);
void print_links_names(mstr &m, TSideWind *sideWind, bool useNewLines = false, bool ordinaryName = true);
void print_court_account(mstr &m, char accountType, const char *fieldName);
void print_exec_list_no(mstr &m, long execListNo, long execListKind);
void print_connects(mstr &m, const TRCDKey &key, bool alias, bool cyrno, constant compos);
void print_quick_short(mstr &m, long quick);
void print_recipient(mstr &m, long recipient);
void print_known_person_csjid(mstr &m, const char *ucn, char ucnType);
void print_exec_list_sendrecv(mstr &m, const char *ucn, char ucnType);
void print_sum_type_lower(mstr &m, const TExecListSum *sum);
void print_old_subject(mstr &m, long oldSubject);	// "\v(%ld)"
void print_college_initials(mstr &m, char college);
void print_with_directives(mstr &m, const char *text);
void print_without_directives(mstr &m, const char *text);	

// 2016:097 LPR: generic_court_*(), generic_user_post() cpy -> cat
void generic_court_type(mstr &m, TGroup *);
void generic_judge_type(mstr &m, TGroup *);		// 2016:106
void generic_judge_type_court(mstr &m, TGroup *);	// 2016:106
void generic_court_name(mstr &m, TGroup *);
void generic_court_uclp(mstr &m, TGroup *);
void generic_camel_uclp(mstr &m, TGroup *);
void generic_court_address(mstr &m, TGroup *);
void generic_court_phone(mstr &m, TGroup *);
void generic_court_email(mstr &m, TGroup *);
void generic_court_president(mstr &m, TGroup *);
void generic_court_area(mstr &m, TGroup *);
void generic_court_region(mstr &m, TGroup *);
void generic_court_logo(mstr &m, TGroup *);	// 2016:320
void generic_user_post(mstr &m, TGroup *);
void generic_user_name(mstr &m, TGroup *);
void generic_user_initials(mstr &m, TGroup *);
void generic_full_user_name(mstr &m, TGroup *);
void generic_instance_III(mstr &m, TGroup *);
void generic_today(mstr &m, TGroup *);
void generic_now(mstr &m, TGroup *);
void generic_login_name(mstr &m, TGroup *);
void generic_null(mstr &m, TGroup *);
void generic_judge(mstr &m, TGroup *);		// "Judge" or "Arbiter"
void generic_slash(mstr &m, TGroup *);
void generic_service_phone(mstr &m, TGroup *);
void generic_service_fax(mstr &m, TGroup *);
void generic_college_type(mstr &m, TGroup *);
void generic_god_suffix(mstr &m, TGroup *);
void generic_court_address_1(mstr &m, TGroup *);
void generic_court_address_2(mstr &m, TGroup *);
void generic_court_addr_skip_uclp(mstr &m, TGroup *);
void generic_nothing(mstr &, TGroup *);

void group_print(mstr &m, TGroup *group);
void group_active(mstr &m, TGroup *group);	// "Yes" | "No"
void group_inactive(mstr &m, TGroup *group);	// "" | "n"
void group_bulpref(mstr &m, TGroup *group);	// ", UIC/BULSTAT " F_BULSTAT if any
void group_bulstat(mstr &m, TGroup *group);	// F_BULSTAT if any
void group_title(mstr &m, TGroup *group);		// F_TITLE if any
void group_extra_text(mstr &m, TGroup *group);	// F_EXTRA_TEXT if any

void ucngroup_name(mstr &m, TGroup *group);
void ucngroup_ucn_type(mstr &m, TGroup *group);
void ucngroup_ucn(mstr &m, TGroup *group);
void ucngroup_extra(mstr &m, TGroup *group);	// " " descript LNC / BULSTAT if any

void basewind_key_no(mstr &m, TGroup *group);
void basewind_flag_no(mstr &m, TGroup *group);
void basewind_key_year(mstr &m, TGroup *group);
void basewind_key_type(mstr &m, TGroup *group);
void basewind_kind(mstr &m, TGroup *group);
void basewind_ucn_type(mstr &m, TGroup *group);
void basewind_date(mstr &m, TGroup *group);
void basewind_involvement(mstr &m, TGroup *group);
void basewind_key_no_year(mstr &m, TGroup *group);
void basewind_names(mstr &m, TGroup *group);

void keygroup_key_no(mstr &m, TGroup *group);
void keygroup_flag_no(mstr &m, TGroup *group);
void keygroup_key_year(mstr &m, TGroup *group);
void keygroup_key_type(mstr &m, TGroup *group);
void keygroup_key_no_year(mstr &m, TGroup *group);
void keygroup_result_status(mstr &m, TGroup *group);

void container_kind(mstr &m, TGroup *group);
void container_date(mstr &m, TGroup *group);
void container_time(mstr &m, TGroup *group);
void container_event_no(mstr &m, TGroup *group);
void container_event_date(mstr &m, TGroup *group);
void container_event_text(mstr &m, TGroup *group);
void container_text(mstr &m, TGroup *group);
void container_lawsuit_kind(mstr &m, TGroup *group);	// type if no kind found
void container_attached(mstr &m, TGroup *group);

void sidewind_involvement(mstr &m, TGroup *group);	// cut to 22 characters
void sidewind_state(mstr &m, TGroup *group);		// state, flags, judged

void subpoena_side_no(mstr &m, TGroup *group);
void subpoena_kind(mstr &m, TGroup *group);
void subpoena_receiver(mstr &m, TGroup *group);
void subpoena_subpoena_date(mstr &m, TGroup *group);
void subpoena_lreceiver(mstr &m, TGroup *group);

void origin_quick_short(mstr &m, TGroup *group);
void outreg_receivers(mstr &m, TGroup *group);
void outreg_lawsuit(mstr &m, TGroup *group);
void legalside_address(mstr &m, TGroup *group);
void execlink_address(mstr &m, TGroup *group);
