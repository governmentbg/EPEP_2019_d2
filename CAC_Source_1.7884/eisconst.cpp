constant egn_neizvestno = "1111111110";

TIntegerArray srk_vid_dta_only;
TIntegerArray grj_excl_bulgaria;
TIntegerArray grj_excl_bg_unkn;

TIntegerArray sbe_etypenomer;
TIntegerArray sbe_loadfzlpne;
TIntegerArray sbe_dlosnsbhstn;
TIntegerArray sbe_zamiana_nkz;
TIntegerArray sbe_destroy_srk;

// ----- spr_ ----------------------------------------------------------------
const TSpr2Mnu spr2mnus[] =
{
	// 1st instance
	{ spr_kontrol_pis_dsp,    mnu_reg_kontrol_pis_dsp },
	{ spr_pis_izp_nkz,        mnu_reg_pis_izp_nkz },
	{ spr_pis_nakaz_delo,     mnu_reg_pis_nakaz_delo },
	{ spr_pis_osobeni,        mnu_reg_pis_osobeni },
	{ spr_privejdane_v_sila,  mnu_reg_privejdane_v_sila },
	{ spr_pis_mjdstr,         mnu_reg_pis_mjdstr },
	// 2nd instance
	{ spr_kontrol_vzi_dsp,    mnu_reg_kontrol_vzi_dsp },
	{ spr_vzi_izp_nkz,        mnu_reg_vzi_izp_nkz },
	{ spr_vzi_nakaz_delo,     mnu_reg_vzi_nakaz_delo },
	{ spr_vzi_osobeni,        mnu_reg_vzi_osobeni },
	{ spr_vzi_mjdstr,         mnu_reg_vzi_mjdstr },
	// 3rd instance
	{ spr_kontrol_vks_dsp,    mnu_reg_kontrol_vks_dsp },
	{ spr_vks_nakaz_delo,     mnu_reg_vks_nakaz_delo },
	{ spr_vks_podsadnost,     mnu_reg_vks_podsadnost },
	{ spr_vks_vazobnovjavane, mnu_reg_vks_vazobnovjavane },
	{ spr_as_vazobnovjavane,  mnu_reg_as_vazobnovjavane },
	{ 0, 0 }
};

TIntegerArray spr_1st_instance;
TIntegerArray spr_2nd_instance;

#if TESTVER
const TSpr2Sig spr2sigs[] =
{
	{ spr_as_vazobnovjavane,  sign_nvahd },
	{ spr_as_vazobnovjavane,  sign_nvohd },
	{ spr_as_vazobnovjavane,  sign_nvcd },
	{ spr_vzi_nakaz_delo,     sign_vnaxd },
	{ spr_vzi_nakaz_delo,     sign_vnoxd },
	{ spr_vzi_nakaz_delo,     sign_vncd },
	{ spr_vzi_nakaz_delo,     sign_vncxd },
	{ spr_vzi_mjdstr,         sign_vnoxd },
	{ spr_vzi_mjdstr,         sign_vncd },
	{ spr_vzi_izp_nkz,        sign_vncd },
	{ spr_vzi_osobeni,        sign_vncd },
	{ spr_kontrol_vzi_dsp,    sign_vncd },
	{ spr_vks_nakaz_delo,     sign_knaxd },
	{ spr_vks_nakaz_delo,     sign_knoxd },
	{ spr_vks_vazobnovjavane, sign_kncd },
	{ spr_vks_podsadnost,     sign_kncd },
	{ spr_kontrol_vks_dsp,    sign_kncd },
	{ spr_pis_osobeni,        sign_nvcd },
	{ spr_pis_osobeni,        sign_ncd },
	{ spr_pis_mjdstr,         sign_nvohd },
	{ spr_pis_mjdstr,         sign_nvcd },
	{ spr_pis_mjdstr,         sign_noxd },
	{ spr_pis_mjdstr,         sign_ncd },
	{ spr_pis_izp_nkz,        sign_nvcd },
	{ spr_pis_izp_nkz,        sign_ncd },
	{ spr_pis_nakaz_delo,     sign_naxd },
	{ spr_pis_nakaz_delo,     sign_nvahd },
	{ spr_pis_nakaz_delo,     sign_nvohd },
	{ spr_pis_nakaz_delo,     sign_noxd },
	{ spr_pis_nakaz_delo,     sign_ncxd },
	{ spr_kontrol_pis_dsp,    sign_nvcd },
	{ spr_kontrol_pis_dsp,    sign_ncd },
	{ spr_privejdane_v_sila,  sign_naxd },
	{ spr_privejdane_v_sila,  sign_nvahd },
	{ spr_privejdane_v_sila,  sign_nvohd },
	{ spr_privejdane_v_sila,  sign_noxd },
	{ spr_privejdane_v_sila,  sign_ncxd },
	{ 0, 0 }
};
#endif  // TESTVER

// ----- KIND_ ---------------------------------------------------------------
static constant
#if COURT_TYPE == COURT_APPEAL
	KIND_APPEALS			= "ü±",
	KIND_PRIVATE_APPEALS		= "í",
	KIND_PROTESTS			= "îð";
	//KIND_PRIVATE_PROTESTS		= "ð";
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN
	KIND_APPEALS			= "ä±",
	KIND_PRIVATE_APPEALS		= "í",
	KIND_PROTESTS			= "îð";
	//KIND_PRIVATE_PROTESTS		= "ð";
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL
	KIND_APPEALS			= "Õäµ¦±",
	KIND_PRIVATE_APPEALS		= "Üí§",
	KIND_PROTESTS			= "àâîð";
	//KIND_PRIVATE_PROTESTS		= "âð";
#endif  // AREAL

#if COURT_TYPE == COURT_MILITARY
	KIND_APPEALS			= "Õäµ¦±",
	KIND_PRIVATE_APPEALS		= "Üí§",
	KIND_PROTESTS			= "àâîð";
	//KIND_PRIVATE_PROTESTS		= "âð";
#endif  // MILITARY

#if COURT_TYPE == COURT_REGIONAL
	KIND_APPEALS			= "Õä¦±",
	KIND_PRIVATE_APPEALS		= "Üí",
	KIND_PROTESTS			= "à";
	//KIND_PRIVATE_PROTESTS		= "â";
#endif  // REGIONAL

// ----- functions -----------------------------------------------------------
void initialize_eispp()
{
	srk_vid_dta_only.Add(srk_dta_prt, srk_dta_prisada, INT_MAX);
	grj_excl_bulgaria.Add(grj_BGR, INT_MAX);
	grj_excl_bg_unkn.Add(grj_BGR, grj_STL, grj_UNK, INT_MAX);

	sbe_etypenomer.Add(
		-1, sbe_vzempp, sbe_vklnzv, sbe_psd, sbe_vnedrsprk, 0, sbe_vnerzgpis, 0, sbe_vznpkenpr, sbe_vznsnenpr,
		sbe_zmnrjm, sbe_zmnnkz, sbe_izmchjpsd, sbe_izmmpp, 0, sbe_izmpispsd, sbe_iprprk, sbe_iprdlo, sbe_iskprmdlo, sbe_kmlnkz,
		sbe_ovadlo, sbe_onedlo, sbe_onesprtrnsfr, sbe_olqpdt, 0, sbe_otkpmnchjpsd, sbe_otkreabil, sbe_zctrbt, sbe_otkmpp, 0,
		sbe_otkzmnrjm, sbe_otkzmnnkz, sbe_otkpsoosv, 0, 0, sbe_otkpmm, sbe_otknknbgr, 0, sbe_otmmpp, sbe_otmpsoosv,
		sbe_otmpmm, 0, sbe_vzipsd, sbe_otmpispsd, 0, sbe_otmzctrbt, sbe_spopdt, sbe_plcdlo, sbe_pispsd, sbe_ptvmpp,
		sbe_ptvpke, sbe_ptvpispsd, 0, sbe_ptvsne, sbe_ptvpismjdstr, sbe_ptvpisnkz, sbe_ptvpisosb, sbe_psoosv, sbe_pkenprtrnsfr, sbe_pkenprsze,
		sbe_pkenpr, sbe_pkepmm, 0, 0, sbe_pmnjba, sbe_pmnchjpsd, sbe_vzepmm, 0, sbe_rshnknbgr, sbe_reabil,
		0, sbe_snenpr, sbe_otmpke, sbe_otmsne, sbe_pkenprpne, sbe_pklrbtdlo, INT_MAX);

	sbe_loadfzlpne.Add(sbe_vklnzv, sbe_ovadlo, INT_MAX);
	sbe_dlosnsbhstn.Add(sbe_vnedrsprk, sbe_vnerzgpis, INT_MAX);
	sbe_zamiana_nkz.Add(sbe_zmnrjm, sbe_zmnnkz, INT_MAX);
	sbe_destroy_srk.Add(sbe_izmmpp, sbe_otmmpp, sbe_ptvmpp, INT_MAX);

	spr_1st_instance.Add(spr_kontrol_pis_dsp, spr_pis_izp_nkz, spr_pis_nakaz_delo, spr_pis_osobeni, spr_privejdane_v_sila,
		spr_pis_mjdstr, INT_MAX);
	spr_2nd_instance.Add(spr_kontrol_vzi_dsp, spr_vzi_izp_nkz, spr_vzi_nakaz_delo, spr_vzi_osobeni, spr_vzi_mjdstr, INT_MAX);
#if COURT_TYPE == COURT_APPEAL
	spr_2nd_instance.Add(spr_as_vazobnovjavane);
#endif  // APPEAL
}
