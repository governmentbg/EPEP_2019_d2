// ----- nmk_ ----------------------------------------------------------------
enum
{
	nmk_nkzvid_rzm_dsmg	= 190,
	nmk_nkzvid_rzm_leva	= 191,
	nmk_nkzvid_rjm		= 192,
	nmk_nkzncn_iptsrk		= 193,
	nmk_dlosig			= 204,
	nmk_dlosts			= 205,
	nmk_pnests			= 206,
	nmk_pnestpdvs		= 207,
	nmk_nkztip			= 208,
	nmk_nkzvid			= 209,
	nmk_nkzncn			= 210,
	nmk_nkzakt			= 211,
	nmk_mppvid			= 214,
	nmk_mppste			= 215,
	nmk_fzlpol			= 216,
	nmk_adrtip			= 217,
	nmk_dvjprc			= 218,
	nmk_zdjvid			= 219,
	nmk_fzlpnerlq		= 220,
	nmk_nprfzlkcv		= 221,
	//nmk_dlosprvid		= 223,	// const.h
	nmk_sbedkpvid		= 224,
	nmk_scqvid			= 225,
	nmk_sbhvid			= 228,
	nmk_srkvid			= 230,
	nmk_nkzrjm			= 234,
	nmk_nprfzlsts		= 244,
	nmk_dvjvid			= 246,
	nmk_dloncnone		= 253,
	nmk_sbeednfzl		= 272,
	nmk_sbcrcd			= 308,
	nmk_sbcple			= 309,
	nmk_sbcspj			= 310,
	nmk_sbcobr			= 311,
	nmk_sbctrd			= 312,
	nmk_sbcznq			= 313,
	nmk_sbcetn			= 314,
	nmk_dlodvj			= 1167,
	nmk_sbekor			= 1665,
	nmk_sbeitv			= 1667,
	nmk_pneotdtip		= 1632,
	nmk_adrloc			= 1724,
	nmk_obstina			= 8616,
	//nmk_oblast		= 8617,	// const.h
	nmk_raion			= 8618,
	//nmk_grj			= 8619,	// const.h
	nmk_adrmstvid		= 8624,
	nmk_sbcrge			= 12478,
	// 2017:316 LPR: .scqosn/.nprfzlosn only
	nmk_osnvzn			= 14113,
	nmk_osnpke			= 1865,
	nmk_osnsze			= 241,
	nmk_osnpke_bezsze		= 240,
	nmk_osnsne			= 242,
	// 2017:316 LPR: .sbhvid only
	nmk_ptvmjdstrvid		= 14125,
	nmk_izpsteprc		= 213,
	nmk_pklncn			= 229,
	nmk_psdtip			= 231,
	nmk_ptvosbvid		= 255,
	nmk_ptvnkzvid		= 256
};

// ----- sbx_ ----------------------------------------------------------------
enum
{
	sbx_ptv_izp_nkz		= 968,
	sbx_ptv_osobeni		= 969,
	sbx_mls_premestvane	= 964,
	sbx_pklncn			= 965,
	sbx_prc_otl_prk_izp_nkz	= 966,
	sbx_prdprc			= 12413,
	sbx_prk_nabl_izp_nkz	= 967,
	sbx_dkp_prt			= 11992,
	sbx_str_prt			= 11991,
	sbx_strstd			= 1663,
	sbx_str_po_podsadnost	= 970,
	sbx_tip_prisada		= 971,
	sbx_ptv_mjdstr		= 14124,
	sbx_nprtodrj		= 14137,
	sbx_nprfromdrj		= 14133
};

// ----- various -------------------------------------------------------------
enum
{
	grj_BGR			= 8805,
	pol_maj			= 706,
	pol_jena			= 691,
	pneotdper			= 1630,
	sts_obrazuvano		= 1113,
	ncn_po_podsadnost		= 723,
	ncn_vazobnoveno		= 730,
	ncn_varnato_ot_vks	= 728,
	ncn_varnato_ot_vzi	= 729,
	ncn_novoobrazuvano	= 722,
	ncn_po_tajba		= 1900001682,	// 2017:346 +related
	pkt_tip_vhd_data		= 134,
	mpp_garancia		= 709,
	izt_osaden_efektivno	= 763,
	scq_osajda			= 979,
	scq_opravdava		= 978,
	psd_osaditelna		= 825,
	psd_opravdatelna		= 824,
	srk_dta_prt			= 11994,
	srk_dta_prisada		= 1028,
	srk_objalvane		= 1029,
	sts_nalojena		= 1107,
	sts_otmenena		= 1116,
	grj_STL			= 14883,
	grj_UNK			= 14884,
	loc_v_nsm			= 1725,
	psl_pln_palnoleten	= 1577,
	dkp_jalba			= 642,
	dkp_chastna_jalba		= 641,
	dkp_protest			= 647,
	dkp_chasten_protest	= 640,
	ipr_jalba			= 673,
	ipr_jalba_protest		= 674,
	ipr_novo_razglejdane	= 675,
	ipr_po_podsadnost		= 676,
	ipr_protest			= 681,
	ipr_vazobnov_nd		= 685,
	ipr_za_podsadnost		= 688,
	ipr_dorazsledvane		= 671,
	ipr_razsledvane		= 682,
	nkz_probacia		= 14344,
	pkt_tip_zvk_spr_chj	= 148,
	kcv_ne_e_svarzan_s_np	= 692,
	kcv_obvinjaem		= 693,
	kcv_podsadim		= 696,
	adr_mestoprestaplenie	= 635,
	sts_prestaplenie		= 1121
};

extern constant egn_neizvestno;

enum
{
	// 2013:018 LPR: for ovadlo
	scq_obedinjava_drugo_np	= 976,
	rlq_obedinen_kam_drugo_np = 849,
	scq_svarzva_s_np = 987,
	nakazanie_za_iztarpjavane = 721,
	obsto_nakazanie = 745
};

extern TIntegerArray srk_vid_dta_only;
extern TIntegerArray grj_excl_bulgaria;
extern TIntegerArray grj_excl_bg_unkn;

// ----- sbe_ ----------------------------------------------------------------
enum
{
	sbe_vzempp		= 947,
	sbe_vklnzv		= 1640,
	sbe_psd		= 924,
	sbe_vnedrsprk	= 943,
	sbe_vnerzgpis	= 945,
	sbe_vznpkenpr	= 953,
	sbe_vznsnenpr	= 954,
	sbe_zmnrjm		= 958,
	sbe_zmnnkz		= 957,
	sbe_izmchjpsd	= 14127,
	sbe_izmmpp		= 863,
	sbe_izmpispsd	= 865,
	sbe_iprprk		= 860,
	sbe_iprdlo		= 859,
	sbe_iskprmdlo	= 861,
	sbe_kmlnkz		= 1718,
	sbe_ovadlo		= 898,
	sbe_onedlo		= 871,
	sbe_onesprtrnsfr	= 14130,
	sbe_olqpdt		= 870,
	sbe_otkpmnchjpsd	= 14128,
	sbe_otkreabil	= 14118,
	sbe_zctrbt		= 955,
	sbe_otkmpp		= 877,
	sbe_otkzmnrjm	= 962,
	sbe_otkzmnnkz	= 885,
	sbe_otkpsoosv	= 883,
	sbe_otkpmm		= 880,
	sbe_otknknbgr	= 961,
	sbe_otmmpp		= 888,
	sbe_otmpsoosv	= 893,
	sbe_otmpmm		= 891,
	sbe_vzipsd		= 950,
	sbe_otmpispsd	= 890,
	sbe_otmzctrbt	= 896,
	sbe_spopdt		= 941,
	sbe_plcdlo		= 913,
	sbe_pispsd		= 901,
	sbe_ptvmpp		= 926,
	sbe_ptvpke		= 930,
	sbe_ptvpispsd	= 929,
	sbe_ptvsne		= 932,
	sbe_ptvpismjdstr	= 14123,
	sbe_ptvpisnkz	= 927,
	sbe_ptvpisosb	= 928,
	sbe_psoosv		= 925,
	sbe_pkenprtrnsfr	= 14136,
	sbe_pkenprsze	= 905,
	sbe_pkenpr		= 903,
	sbe_pkepmm		= 906,
	sbe_pmnjba		= 915,
	sbe_pmnchjpsd	= 914,
	sbe_vzepmm		= 948,
	sbe_rshnknbgr	= 938,
	sbe_reabil		= 14117,
	sbe_snenpr		= 940,
	sbe_pklova		= 1721,
	sbe_intern_desc	= -1,
	sbe_edit_delete	= -2,
	sbe_edit_lawsuit	= -3,
	sbe_edit_fzlpne	= -4,
	sbe_edit_fzl	= -5,
	sbe_edit_pne_zvk	= -6,
	sbe_edit_pne_reg	= -7,
	// 2014:217
	sbe_otmpke		= 15347,
	sbe_otmsne		= 15348,
	sbe_pkenprpne	= 904,
	sbe_pklrbtdlo	= 910,	// 2015:056
	// 2016:111
	sbe_vnerzgvzi	= 946,
	sbe_vzndlo		= 951,
	sbe_izmvzipsd	= 866,
	sbe_otkvzndlo	= 884,
	sbe_otlnkzlos	= 886,
	sbe_otmvzipsd	= 895,
	sbe_otmrshvzi	= 894
};

extern TIntegerArray sbe_etypenomer;
extern TIntegerArray sbe_loadfzlpne;
extern TIntegerArray sbe_dlosnsbhstn;
extern TIntegerArray sbe_zamiana_nkz;
extern TIntegerArray sbe_akt_ipd_date;

// ----- mnu_ ----------------------------------------------------------------
enum
{
	// 1st instance
	mnu_reg_kontrol_pis_dsp		= 2042,
	mnu_reg_pis_izp_nkz		= 2045,
	mnu_reg_pis_nakaz_delo		= 2043,
	mnu_reg_pis_osobeni		= 2046,
	mnu_reg_privejdane_v_sila	= 2044,
	mnu_reg_pis_mjdstr		= 14121,
	// 2nd instance
	mnu_reg_kontrol_vzi_dsp		= 2047,
	mnu_reg_vzi_izp_nkz		= 2049,
	mnu_reg_vzi_nakaz_delo		= 2048,
	mnu_reg_vzi_osobeni		= 2050,
	mnu_reg_vzi_mjdstr		= 14122,
	// 3rd instance
	mnu_reg_kontrol_vks_dsp		= 2051,
	mnu_reg_vks_nakaz_delo		= 2052,
	mnu_reg_vks_podsadnost		= 2053,
	mnu_reg_vks_vazobnovjavane	= 2054,
	mnu_reg_as_vazobnovjavane	= 1900001641,	// 2017:318
	// others
	mnu_reg_korsbe	= 2017,
	mnu_reg_itvsbe	= 2018,
	mnu_reg_kordlo	= 2020,
	mnu_reg_korfzl	= 1636,
	mnu_reg_korpne	= 1637,
	mnu_spr_tip_new	= 1947,
	mnu_reg_oneprp	= 15326
};

// ----- sign_ ---------------------------------------------------------------
enum
{
	sign_nvahd	= 1900001639,
	sign_nvohd	= 1900001638,
	sign_nvcd	= 1900001640,
	sign_vnaxd  = 1900001685,  // 2017:325
	sign_vnoxd	= 1010,
	sign_vncd	= 1008,
	sign_vncxd	= 1009,
	sign_knaxd	= 15342,
	sign_knoxd	= 998,
	sign_kncd	= 997,
	sign_ncd	= 1000,
	sign_noxd	= 1002,
	sign_naxd	= 999,
	sign_ncxd	= 1001
};

// ----- spr_ ----------------------------------------------------------------
enum
{
	// 1st instance
	spr_kontrol_pis_dsp	= 1014,
	spr_pis_izp_nkz		= 1017,
	spr_pis_nakaz_delo	= 1018,
	spr_pis_osobeni		= 1019,
	spr_privejdane_v_sila	= 1020,
	spr_pis_mjdstr		= 14119,
	// 2nd instance
	spr_kontrol_vzi_dsp	= 1016,
	spr_vzi_izp_nkz		= 1024,
	spr_vzi_nakaz_delo	= 1025,
	spr_vzi_osobeni		= 1026,
	spr_vzi_mjdstr		= 14120,
	// 3rd instance
	spr_kontrol_vks_dsp	= 1015,
	spr_vks_nakaz_delo	= 1021,
	spr_vks_podsadnost	= 1022,
	spr_vks_vazobnovjavane	= 1023,
	spr_as_vazobnovjavane	= 1900001637	// 2017:313 +related
};

struct TSpr2Mnu
{
	long spr;
	long mnu;
};

extern const TSpr2Mnu spr2mnus[];
extern TIntegerArray spr_1st_instance;
extern TIntegerArray spr_2nd_instance;

#if TESTVER
// 2017:319
struct TSpr2Sig
{
	long spr;
	long sig;
};

extern const TSpr2Sig spr2sigs[];
#endif  // TESTVER

// ----- select_ -------------------------------------------------------------
enum
{
	SELECT_NOTHING,
	SELECT_CONNECT,
	SELECT_FZL
};

// ----- PNE_ ----------------------------------------------------------------
enum
{
	PNE_NMR_REQUIRED,
	PNE_NMR_BLOCKED,
	PNE_NMR_ALLOWED
};

// ----- SIZE_ ---------------------------------------------------------------
enum
{
	SIZE_OF_EISPP_RZM		= 3 + 1,
	SIZE_OF_PSTKOD		= 4 + 1,
	SIZE_OF_KODPDL		= 5 + 1
};
