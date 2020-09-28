#include "cac.h"

bool eispp_ucn_match(char ucnType, long nom)
{
	if (ucnType == UCN_INSTANCE_PROSEC)
		return nom == EISPP_PROSEC;

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	if (ucnType == UCN_INSTANCE_PROBAT)
		return nom == EISPP_PROBAT;
#endif  // AREAL || MILITARY

	if (ucnType == UCN_INSTANCE_PRECOURT && nom == EISPP_CUSTOM)
		return true;

	if (ucnType && strchr(EISPP_SENDERS, ucnType))
	{
		return nom == EISPP_MILPOL || nom == EISPP_POLICE || nom == EISPP_PROSEC || nom == EISPP_DETECT ||
			nom == EISPP_NSAINV;
	}

	fatal("%d: unsupported eispp sender type", ucnType);
	return false;
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL

#include "eisconst.h"
#include "eisconst.cpp"

long eispp_instance(long dlosprvid)
{
	return spr_1st_instance.Contains(dlosprvid) ? 1 : spr_2nd_instance.Contains(dlosprvid) ? 2 : 3;
}

#if TESTVER
bool eispp_matches(char lawsuitKind, long dlosprvid)
{
	Kind->Seek(lawsuitKind);

	for (const TSpr2Sig *spr2sig = spr2sigs; spr2sig->spr; spr2sig++)
		if (spr2sig->spr == dlosprvid && spr2sig->sig == Kind->eispp)
			return true;

	return false;
}
#endif  // TESTVER

#include "eisdoc.h"
#include "eisdoc.cpp"

static void print_nprfzlpne(mstr &m, TGroup *group, NPR *npr)
{
	NPRFZLPNE *nprfzlpne = (NPRFZLPNE *) group;
	PNE *pne = find_pne(npr->dlo->pne, nprfzlpne->pnesid);

	if (pne)
		pne->Print(m);
	else
		m.printf("%ld", nprfzlpne->pnesid);
}

#include "FzlDialog.h"
#include "FzlDialog.cpp"

#include "FzlGroupListDialog.h"
#include "FzlGroupListDialog.cpp"

#include "PneAliasFace.h"
#include "PneAliasFace.cpp"

#include "NprFzlPneComboBox.h"
#include "NprFzlPneComboBox.cpp"

#include "EditNprFzlPneDialog.h"
#include "EditNprFzlPneDialog.cpp"

#include "NprFzlPneListWindow.h"
#include "NprFzlPneListWindow.cpp"

#include "NprFzlPneListDialog.h"
#include "NprFzlPneListDialog.cpp"

#include "EditAdrDialog.h"
#include "EditAdrDialog.cpp"

#include "EditFzlDialog.h"
#include "EditFzlDialog.cpp"

#include "EditPneDialog.h"
#include "EditPneDialog.cpp"

#include "FzlListWindow.h"
#include "FzlListWindow.cpp"

#include "PneListWindow.h"
#include "PneListWindow.cpp"

#include "FzlPneListDialog.h"
#include "FzlPneListDialog.cpp"

#include "ConnectComboBox.h"
#include "ConnectComboBox.cpp"

#include "FzlComboBox.h"
#include "FzlComboBox.cpp"

#include "SbeDialog.h"
#include "SbeDialog.cpp"

void eispp_accept(TWindow *parent, TRequest *request)
{
	if (EISPP_ACTIVE && any(request->eispp_nmr))
	{
		SBE *sbe = EISPAKET::ReadEF(request, SBE::Index(sbe_plcdlo), false);

		if (!sbe)
		{
			sbe = new SBE(sbe_plcdlo);
			strcpy(sbe->npr->nmr, request->eispp_nmr);
			sbe->dkpdta = request->date;
			sbe->dvjdlo->dta = request->date;	// editable
		}

		sbe->npr->key = request->key;
		bool edit = sbe->npr->Try();

		if (TSbeDialog(parent, sbe, request, NULL, NULL, NULL, edit).Execute() == IDOK)
			EISPAKET(sbe).Insert(request);
		else
			delete sbe;
	}
}

static long dlo_ncnone(char main_connect_type, const TLawsuit *lawsuit)
{
	switch (main_connect_type)
	{
		case CONNECT_TYPE_JURISDICTION : return ncn_po_podsadnost;
		case CONNECT_TYPE_RENEWED_LAWSUIT : return ncn_vazobnoveno;
		case CONNECT_TYPE_RETURNED_LAWSUIT :
		{
		#if COURT_TYPE == COURT_APPEAL
			return ncn_varnato_ot_vks;
		#endif  // APPEAL
		// 2016:355 don't check lawsuit->kind, it's always "ot_vzi"
		#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
			return ncn_varnato_ot_vzi;
		#endif  // AREAL || MILITARY || REGIONAL
		}
	}

#if COURT_TYPE == COURT_APPEAL
	(void) lawsuit;
	return ncn_novoobrazuvano;
#endif  // APPEAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	return lawsuit->sourceKind == KIND_EISPP_COMPLAINT && lawsuit->date >= P64ActDate ? ncn_po_tajba : ncn_novoobrazuvano;
#endif  // AREAL || MILITARY || REGIONAL
}

static void eispp_copy(TEisGroup *group, const TRCDKey &source, const TRCDKey &target)
{
	msql m(((TGroup *) group)->Name);

	m.AddLong("F_NO", source.no);
	m.AddLong("F_YEAR", source.year);
	m.AddChar("F_TYPE", source.type);

	TQuery q(str(m));
	while (q.Read())
	{
		*group << q;
		group->key = target;

		if (!group->Try())
			group->Insert();
	}
}

#if COURT_TYPE == COURT_AREAL
static long sbe_vid_obrazuv(const TLawsuit *lawsuit)
{
	return lawsuit->sourceKind == KIND_START_EXTERNAL ? sbe_onesprtrnsfr : sbe_onedlo;
}
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static long sbe_vid_obrazuv(const TLawsuit *) { return sbe_onedlo; }
#endif  // APPEAL || MILITARY || REGONAL

void eispp_accept(TWindow *parent, TLawsuit *lawsuit, TSideWindArray *sideWinds)
{
	bool has_eispp_nmr = any(lawsuit->eispp_nmr);
	char main_connect_type = lawsuit->MainConnectType();
	bool lawsuit_starts_now = strchr(KIND_START_HERE_INDOCS, lawsuit->sourceKind) &&
		main_connect_type != CONNECT_TYPE_RETURNED_LAWSUIT;
	int pne_mode = lawsuit_starts_now ? (has_eispp_nmr ? PNE_NMR_ALLOWED : PNE_NMR_BLOCKED) : PNE_NMR_REQUIRED;

	Subject->Seek(lawsuit->key.type, lawsuit->subject);

	if (EISPP_ACTIVE && ((lawsuit_starts_now && *Subject->Sbes(lawsuit->kind)) || has_eispp_nmr))
	{
		SBE *sbe = new SBE(sbe_vid_obrazuv(lawsuit));
		bool edit;

		strcpy(sbe->npr->nmr, lawsuit->eispp_nmr);
		sbe->npr->key = lawsuit->key;
		edit = sbe->npr->Try();

		sbe->dkpdta = lawsuit->date;
		Kind->Seek(lawsuit->kind);
		sbe->npr->dlo->sig = Kind->eispp;
		sbe->npr->dlo->gdn = lawsuit->key.year;
		itoa(lawsuit->key.no, sbe->npr->dlo->nmr, 10);
		sbe->npr->dlo->ncnone = dlo_ncnone(main_connect_type, lawsuit);

		// 2015:222 LPR: was if (zvk) 2013:035, but and npr may be mismatched
		if (edit && sbe->npr->dlo->sprvid)
			sbe->npr->dlo->FindAttrib("dlosprvid")->flags &= ~XML_EDITABLE;
		else if (!sbe->npr->dlo->sprvid)	// 2013:024
			sbe->npr->dlo->sprvid = Subject->Vid(lawsuit->kind);

		// 2012:333 LPR/LRQ: npr{str,dta,drj} for our npr-s only
		// 2016:011 IRQ/FIX: skip if has_eispp_nmr
		if (lawsuit_starts_now)
		{
			if (!has_eispp_nmr)
			{
				Court->Seek(COURT_CODE);
				sbe->npr->str = Court->eispp;
				sbe->npr->dta = lawsuit->date;
				sbe->npr->drj = grj_BGR;
			}
			// 2016:161 TRQ/FIX: no DLOOSN regardless of has_eispp_nmr
			sbe->npr->dlo->FindObject("DLOOSN")->flags = 0;
		}

		TXMLInt *sprvid = (TXMLInt *) sbe->FindAttrib("dlosprvid");
	#if TESTVER
		TIntegerArray sigset;
		TIntegerArray *lset = &sigset;  // TODO: use sigset only	

		for (const TSpr2Sig *spr2sig = spr2sigs; spr2sig->spr; spr2sig++)
			if (spr2sig->sig == sbe->npr->dlo->sig)
				sigset.Add(spr2sig->spr);
	#else  // TESTVER
		TIntegerArray *lset;

	#if COURT_TYPE == COURT_APPEAL
		lset = &spr_2nd_instance;
	#endif  // APPEAL
	#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		lset = strchr(KIND_1ST_PUNISHMENT_LAWS, lawsuit->kind) ? &spr_1st_instance : &spr_2nd_instance;
	#endif  // AREAL || MILITARY
	#if COURT_TYPE == COURT_REGIONAL
		lset = &spr_1st_instance;
	#endif  // REGIONAL
	#endif  // TESTVER

		for (int i = 0; i < sprvid->lset.Count(); )
		{
			if (lset->Contains(sprvid->lset[i]))
				i++;
			else
				sprvid->lset.Detach(i);
		}

		if (TSbeDialog(parent, sbe, lawsuit, NULL, NULL, NULL, edit).Execute() == IDOK)
		{
			sNPRAKTSTS snpr;
			bool loaded;

			snpr.key = lawsuit->source;
			loaded = snpr.key.type && snpr.Try();

			if (!has_eispp_nmr)
			{
				strcpy(lawsuit->eispp_nmr, sbe->npr->nmr);
				lawsuit->Update("F_EISPP_NMR");
			}

			// 2014:021 FIX: don't reload snpr on edit
			if (loaded && !edit)
			{
				FZL fzl;
				PNE pne;
				NPRFZLPNE nprfzlpne;

				eispp_copy(&fzl, snpr.key, lawsuit->key);
				eispp_copy(&pne, snpr.key, lawsuit->key);
				eispp_copy(&nprfzlpne, snpr.key, lawsuit->key);
			}

			if (edit || loaded)
			{
				NPRFZLPNE nprfzlpne;
				nprfzlpne.LoadFloat(sbe->npr->dlo->nprfzlpne, sbe->npr);
			}
			else
				sbe->npr->LoadSideWinds(lawsuit, sideWinds);

			TFzlPneListDialog(parent, sbe, lawsuit, edit, pne_mode).Execute();

			if (sbe->npr->dlo->fzl->Count())
			{
				EISPAKET(sbe).Insert(lawsuit);
				sbe = NULL;
			}
			else
			{
				*lawsuit->eispp_nmr = '\0';
				lawsuit->Update("F_EISPP_NMR");
			}
		}

		delete sbe;
	}
}

#include "SbeAliasFace.h"
#include "SbeAliasFace.cpp"

#include "EditMppDialog.h"
#include "EditMppDialog.cpp"

#include "EditDsmgRzmDialog.h"
#include "EditDsmgRzmDialog.cpp"

#include "EditNkzDialog.h"
#include "EditNkzDialog.cpp"

#include "NkzListWindow.h"
#include "NkzListWindow.cpp"

#include "EditNkzPneDialog.h"
#include "EditNkzPneDialog.cpp"

#include "NkzPneListWindow.h"
#include "NkzPneListWindow.cpp"

#include "NkzPneListDialog.h"
#include "NkzPneListDialog.cpp"

#include "FzlNprPneListWindow.h"
#include "FzlNprPneListWindow.cpp"

#include "FzlNkzPneListDialog.h"
#include "FzlNkzPneListDialog.cpp"

#include "FzlNprPneListDialog.h"
#include "FzlNprPneListDialog.cpp"

#include "EditNprFzlStaDialog.h"
#include "EditNprFzlStaDialog.cpp"

#include "SbeFzlListWindow.h"
#include "SbeFzlListWindow.cpp"

#include "SbeFzlListDialog.h"
#include "SbeFzlListDialog.cpp"

#include "SbeListWindow.h"
#include "SbeListWindow.cpp"

#include "SbeListDialog.h"
#include "SbeListDialog.cpp"

struct TContainerSbes
{
	TRCDKeyContainer *container;
	TGArray<SBE> *sbes;
	NPR *npr;	// NULL == sbezvk
};

static void load_container_sbe(const char *name, const void *data)
{
	const TContainerSbes *cs = (const TContainerSbes *) data;
	SBE *sbe = EISPAKET::ReadEF(name, cs->container, !cs->npr);

	if (sbe)
	{
		sbe->npr->key = cs->container->key;

		if (cs->npr)
		{
			DLO *dlo = sbe->npr->dlo;

			if (EisVal->Contains(nmk_sbeednfzl, sbe->vid))
			{
				(*sbe->npr->dlo->fzl)[0]->key = cs->container->key;
				(*sbe->npr->dlo->fzl)[0]->Try();
			}
			else if (sbe_loadfzlpne.Contains(sbe->vid))
			{
				TXArray<FZL> *fzl = dlo->fzl;
				sbe->npr->fzlmin = INT_MAX;

				for (int i = 0; i < fzl->Count(); i++)
					if ((*fzl)[i]->sid < sbe->npr->fzlmin)
						sbe->npr->fzlmin = (*fzl)[i]->sid;

				fzl->Flush();
				dlo->pne->Flush();
				dlo->nprfzlpne->Flush();
				sbe->LoadFzlPneNpr();
			}

			for (int i = 0; i < sbe->sbe->Count(); i++)
				(*sbe->sbe)[i]->ApplyDesc((*sbe->sbe)[i]->vid);

			dlo->sprvid = cs->npr->dlo->sprvid;
		}

		cs->sbes->Add(sbe);
	}
}

static void load_container_sbes(TRCDKeyContainer *container, TGArray<SBE> *sbes, NPR *npr)
{
	char name[SIZE_OF_PATH];
	TContainerSbes cs = { container, sbes, npr };

	electric_name(container, EISPP_XMLPATH, name, TYPE_SBE_DESC);
	html_group_action(name, load_container_sbe, &cs);
}

static void delete_container_sbe(const char *name, const void *data)
{
	TEisPending pending;

	strcpy(pending.fileName, basename(name));
	pending.Delete(true);

	// 2013:002 LPR: simplify: ignore pending
	while (exist(name) && !DeleteFile(name))
	{
		mstr m;
		unsigned long dwError = GETLASTERROR;

		m.printf("%s: грешка %d при изтриване на ЕИСПП файл.", name, dwError);

		if (dwError == ERROR_SHARING_VIOLATION || dwError == ERROR_LOCK_VIOLATION)
			m.cat(" Вероятно се изпраща в момента.");

		if (!ask("%s\n\nЖелаете ли да опитате отново?", str(m)))
		{
			m.clear();
			m.printf("%s: ul %lu", pending.fileName, dwError);
			log(LOG_EISPP, 0, (TRCDKeyContainer *) data, str(m));
			break;
		}
	}
}

bool xml_int_is_valid(long vid, const char *name, long value)
{
	TSbeDesc desc;

	desc.vid = vid;
	strcpy(desc.name, name);

	if (desc.Try())
	{
		long data;
		TXMLInt attrib(name, &data);
		msql m;

		attrib.Apply(&desc);

		if (!attrib.IsValid(m, value))
			return false;
	}

	return true;
}

void eispp_accept(TWindow *parent, TInReg *inReg)
{
	long dkpvid = 0;

	if (strchr(KIND_APPEALS, inReg->kind))
		dkpvid = dkp_jalba;
	else if (strchr(KIND_PRIVATE_APPEALS, inReg->kind))
		dkpvid = dkp_chastna_jalba;
	//else if (strchr(KIND_PROTESTS, inReg->kind))
	//	dkpvid = strchr(KIND_PRIVATE_PROTESTS, inReg->kind) ? dkp_chasten_protest : dkp_protest;

	if (dkpvid && EISPP_ACTIVE && inReg->lawsuit.type == TYPE_PUNISHMENT_LAW)
	{
		TLawsuit lawsuit;
		NPR npr;

		npr.key = inReg->lawsuit;
		lawsuit.key = inReg->lawsuit;

		if (npr.Try() && lawsuit.Get() && any(lawsuit.eispp_nmr))
		{
			SBE *sbe = EISPAKET::ReadEF(inReg, SBE::Index(sbe_pmnjba), false);
			bool edit = sbe != NULL;

			if (!edit)
			{
				sbe = new SBE(sbe_pmnjba);
				sbe->dkpvid = dkpvid;
				sbe->dkpdta = inReg->date;
				*sbe->npr << npr;
			}

			if (TSbeDialog(parent, sbe, inReg, NULL, NULL, NULL).Execute() == IDOK)
			{
				TSbeFzlListDialog(parent, sbe, edit).Execute();

				if (sbe->npr->dlo->fzl->Count() > 0)
				{
					EISPAKET(sbe).Insert(inReg);
					sbe = NULL;
				}
			}

			delete sbe;
		}
	}
}

void eispp_accept(TWindow *parent, TLawsuit *lawsuit, TSession *session, TDecision *decision, bool complain)
{
	TIndexContainer *container = decision ? (TIndexContainer *) decision : (TIndexContainer *) session;

	if (EISPP_ACTIVE && any(lawsuit->eispp_nmr))
	{
		NPR npr;
		TIntegerArray lset;

		npr.key = lawsuit->key;
		Subject->Seek(lawsuit->key.type, lawsuit->subject);

		if (!strvalue_to_lset(Subject->Sbes(lawsuit->kind), &lset, NULL))
			lset.Clear();

		if (npr.Try())
		{
			TIntegerArray vids;
			TGArray<SBE> sbes;
			TIntegerArray zvkndx;
			FZL fzl;

			vids.Add(sbe_vzempp, sbe_vklnzv, sbe_psd, sbe_vnedrsprk, sbe_vnerzgpis, sbe_vznpkenpr, sbe_vznsnenpr,
				sbe_zmnrjm, sbe_iprprk, sbe_olqpdt, sbe_otkpmnchjpsd, sbe_otkreabil, sbe_zctrbt, sbe_otkmpp,
				sbe_otkzmnrjm, sbe_otkzmnnkz, sbe_otkpsoosv, sbe_otkpmm, sbe_otknknbgr, sbe_otmmpp, sbe_otmpmm,
				sbe_otmzctrbt, sbe_ptvmpp, sbe_ptvpke, sbe_ptvpispsd, sbe_izmpispsd, sbe_zmnrjm, sbe_zmnnkz,
				sbe_pispsd, sbe_psd, sbe_izmmpp, sbe_kmlnkz, sbe_otmpsoosv, sbe_vzipsd, sbe_otmpispsd, sbe_spopdt,
				sbe_ptvsne, sbe_ptvpismjdstr, sbe_ptvpisnkz, sbe_ptvpisosb, sbe_psoosv, sbe_pkenprtrnsfr,
				sbe_pkenprsze, sbe_pkenpr, sbe_pkepmm, sbe_pmnchjpsd, sbe_vzepmm, sbe_rshnknbgr, sbe_reabil,
				sbe_snenpr, sbe_iskprmdlo, sbe_ovadlo, sbe_izmchjpsd, sbe_otmpke, sbe_otmsne, sbe_pkenprpne,
				sbe_pklrbtdlo, sbe_vnerzgvzi, sbe_vzndlo, sbe_izmvzipsd, sbe_otkvzndlo, sbe_otlnkzlos,
				sbe_otmvzipsd, sbe_otmrshvzi, INT_MAX);

			for (int i = 0; i < vids.Count(); )
			{
				long vid = vids[i];
				Kind->Seek(container->IndexKind());

			#if TESTVER
				// or create a "..." button for sbes in lset?
				if (vid == -999)
				{
					message("lset: %d, sig: %d, sprvid: %d, dkp: %d", lset.Contains(vid),
						xml_int_is_valid(vid, "NPR.DLO.dlosig", npr.dlo->sig),
						xml_int_is_valid(vid, "NPR.DLO.dlosprvid", npr.dlo->sprvid),
						xml_int_is_valid(vid, "sbedkpvid", Kind->eispp));
				}
			#endif  // TESTVER

				if (lset.Contains(vid) &&
					xml_int_is_valid(vid, "NPR.DLO.dlosig", npr.dlo->sig) &&
					xml_int_is_valid(vid, "NPR.DLO.dlosprvid", npr.dlo->sprvid) &&
					xml_int_is_valid(vid, "sbedkpvid", Kind->eispp))
				{
					i++;
				}
				else
					vids.Detach(i);
			}

			load_container_sbes(container, &sbes, &npr);
			for (int i = 0; i < sbes.Count(); i++)
				zvkndx.Add(sbes[i]->Index());

			if (vids.Count() > 0 || sbes.Count() > 0)
			{
				fzl.LoadFloat(npr.dlo->fzl, &npr);		// for 1/select-fzl sbe-s
				TSbeListDialog(parent, &npr, lawsuit, container, &vids, &sbes).Execute();

				for (int i = 0; i < sbes.Count(); i++)
				{
					SBE *sbe = sbes[i];
					int index = sbe->Index();
					int zvkIndex;

					if (sbe->dirty)
					{
						EISPAKET eispaket(sbe);
						DLO *dlo = sbe->npr->dlo;

						cleanup_fzl_pne_npr(dlo->fzl, dlo->pne, dlo->nprfzlpne, sbe->npr->fzlmin, sbe->dkpdta);
						eispaket.Insert(container);
					}
					else
						delete sbe;

					if ((zvkIndex = zvkndx.Find(index)) != INT_MAX)
						zvkndx.Detach(zvkIndex);
				}

				for (int i = 0; i < zvkndx.Count(); i++)
					EISPAKET::Delete(container, zvkndx[i]);

				sbes.Clear();
			}
			else if (complain)
				error("Няма подходящи събития за добавяне.");
		}
		else if (complain)
			say_no_data("T_NPR");
		// LPR: we may have converted connect eispp #s
		//else
		//	xml_error_now(container, "eispp_accept", "npr %ld/%ld missing", lawsuit->key.no, lawsuit->key.year);
	}
}

void eispp_delete(TRCDKeyContainer *container, char lawsuitType)
{
	if (EISPP_ACTIVE && (lawsuitType == TYPE_PUNISHMENT_LAW || container->Find("F_EISPP_NMR")))
	{
		TGArray<SBE> sbes;
		char name[SIZE_OF_PATH];

		electric_name(container, EISPP_OUTPATH, name, TYPE_SBE_DESC);
		*strrchr(name, '\\') = '_';
		html_group_action(name, delete_container_sbe, container);

		load_container_sbes(container, &sbes, NULL);
		for (int i = 0; i < sbes.Count(); i++)
			EISPAKET::Delete(container, sbes[i]);
		sbes.Clear();
	}
}

void eispp_delete(TRCDKeyContainer *container)
{
	eispp_delete(container, container->key.type);
}

void eispp_delete(TPrimeContainer *container)
{
	eispp_delete(container, container->lawsuit.type);
}

void eispp_delete(TOutReg *outReg, bool full)
{
	outReg->aret = true;
	EISPAKET::Delete(outReg, SBE::Index(sbe_plcdlo));

	if (full)
	{
		outReg->aret = false;
		eispp_delete(outReg);
	}
}

void eispp_edit_lawsuit(TLawsuit *lawsuit)
{
	NPR npr;

	npr.key = lawsuit->key;

	if (npr.Get())
	{
		SBE *zvk = new SBE(sbe_edit_lawsuit);
		SBE *sbe = new SBE(sbe_edit_lawsuit);

		*zvk->npr << npr;
		*sbe->npr << npr;
		sbe->npr->dlo->Apply("dloncnone", XML_INCLUDED);
		sbe->npr->dlo->Apply("dlosprvid", XML_INCLUDED);

		EISPAKET(sbe).Update(lawsuit, zvk, mnu_reg_kordlo);
	}
}

void eispp_edit_fzlpne(TWindow *parent, TLawsuit *lawsuit)
{
	SBE sbe(sbe_edit_fzlpne);

	sbe.npr->key = lawsuit->key;
	sbe.vid = sbe_edit_fzlpne;

	if (sbe.npr->Try())
	{
		NPRFZLPNE nprfzlpne;
		nprfzlpne.LoadFloat(sbe.npr->dlo->nprfzlpne, sbe.npr);
		TFzlPneListDialog(parent, &sbe, lawsuit, true, PNE_NMR_BLOCKED).Execute();
	}
}

void eispp_renumber(TLawsuit *source, TLawsuit *target)
{
	NPR npr;

	npr.key = source->key;

	if (any(source->eispp_nmr) && npr.Try())
	{
		if (EISPP_ACTIVE)
		{
			long vid = sbe_vid_obrazuv(source);
			SBE *zvk = EISPAKET::ReadEF(source, SBE::Index(vid), true);
			SBE *sbe = EISPAKET::ReadEF(source, SBE::Index(vid), false);

			if (zvk && sbe)
			{
				EISPAKET::Delete(source, zvk);
				sbe->npr->dlo->gdn = target->key.year;
				itoa(target->key.no, sbe->npr->dlo->nmr, 10);
				EISPAKET(sbe).Insert(target);
			}
		}

		renumber_eispps(source, target);
	}
}

static bool get_outreg_data(TOutReg *outReg, TSender *sender, TLawsuit *lawsuit, NPR *npr)
{
	// 2013:029 LPR: outReg->receiverType == UCN_INSTANCE_HIGHER; 2013:070 only if receiver
	if (EISPP_ACTIVE && outReg->lawsuit.type == TYPE_PUNISHMENT_LAW && atob(outReg->receiver))
	{
		sender->ucnType = outReg->receiverType;
		strcpy(sender->ucn, outReg->receiver);

		// 2013:029 LPR: // && (outReg->receiverType == UCN_INSTANCE_PROSEC || sender->court)
		if (sender->Get())
		{
			npr->key = outReg->lawsuit;
			lawsuit->key = outReg->lawsuit;

			if (lawsuit->Get() && any(lawsuit->eispp_nmr) && npr->Try())
				return true;
		}
	}

	return false;
}

bool eispp_checkout(TOutReg *outReg)
{
	TSender sender;
	TLawsuit lawsuit;
	NPR npr;

	return get_outreg_data(outReg, &sender, &lawsuit, &npr);
}

void eispp_send(TWindow *parent, TOutReg *outReg, TInReg *inReg)
{
	TSender sender;
	TLawsuit lawsuit;
	NPR npr;

	if (get_outreg_data(outReg, &sender, &lawsuit, &npr))
	{
		SBE *sbe = EISPAKET::ReadEF(outReg, SBE::Index(sbe_iprdlo), false);

		if (!sbe)
		{
			sbe = new SBE(sbe_iprdlo);
			*sbe->npr << npr;
			sbe->dkpdta = outReg->date;
			sbe->dvjdlo->dta = outReg->date;
			sbe->dvjdlo->strplc = sender.eispp;
			itoa(lawsuit.key.no, sbe->dvjdlo->dvn, 10);

			switch (outReg->receiverType)
			{
				case UCN_INSTANCE_EQUAL :
				case UCN_INSTANCE_COMPETENT : sbe->dvjdlo->prc = ipr_po_podsadnost; break;
				case UCN_INSTANCE_CONFLICT : sbe->dvjdlo->prc = ipr_za_podsadnost; break;
				case UCN_INSTANCE_LOWER : sbe->dvjdlo->prc = ipr_novo_razglejdane; break;
				case UCN_INSTANCE_PROSEC : sbe->dvjdlo->prc = ipr_dorazsledvane; break;
				case UCN_INSTANCE_HIGHER :
					sbe->dvjdlo->prc = strchr(KIND_PROTESTS, inReg->kind) ? ipr_protest : ipr_jalba;
			}
		}

		if (TSbeDialog(parent, sbe, outReg, NULL, NULL, NULL).Execute() == IDOK)
			EISPAKET(sbe).Insert(outReg);
		else
			delete sbe;
	}
}

void eispp_receive(TWindow *parent, TOutReg *outReg)
{
	TSender sender;
	TLawsuit lawsuit;
	NPR npr;

	if (get_outreg_data(outReg, &sender, &lawsuit, &npr))
	{
		SBE *sbe = EISPAKET::ReadEF(outReg, SBE::Index(sbe_plcdlo), false);

		if (!sbe)
		{
			sbe = new SBE(sbe_plcdlo);
			*sbe->npr << npr;
			sbe->dkpdta = outReg->returned;
			sbe->dvjdlo->dta = outReg->returned;
		}

		sbe->npr->key = lawsuit.key;

		if (TSbeDialog(parent, sbe, outReg, NULL, NULL, NULL).Execute() == IDOK)
			EISPAKET(sbe).Insert(outReg);
		else
			delete sbe;
	}
}

bool eispp_stsakt(TWindow *parent, TOriginContainer *container)
{
	const char *nprnmr = ((TString *) container->Find("F_EISPP_NMR"))->S();

	if (EISPP_ACTIVE && any(nprnmr))
	{
		EISTSAKT eistsakt(parent, container, nprnmr);
		sNPRAKTSTS *npr = eistsakt.data->rzt->rztoprspr->npr;

		npr->key = container->key;

		if (!npr->Try())
		{
			sign result = eistsakt.Execute();

			if (result == true)
			{
				if (!eistsakt.Import())
				{
					eistsakt.Remove();
					return false;
				}
			}
			else if (!result)
			{
				eistsakt.Remove();
				return !ask("Няма данни за %s. Желаете ли да изтриете номера на НП на входящия документ?", nprnmr);
			}
		}
	}

	return true;
}

static bool print_pne_kcqs(mstr &m, const char *criteria)
{
	TQuery q(criteria);
	PNE pne;

	if (!q.Read())
		return false;

	do
	{
		pne << q;
		m.sep(", ");
		m.cat(EisCrime->Seek(pne.pnesta->kcq));
	} while (q.Read());

	return true;
}

bool eispp_print_pnes(mstr &m, const TSideWind *defendant)
{
	msql w;

	if (defendant->ucnType == UCN_CITIZEN_UCN)
	{
		w.Begin("T_FZL F JOIN T_NPRFZLPNE N ON N.F_NO = F.F_NO AND N.F_YEAR = F.F_YEAR AND N.F_TYPE = F.F_TYPE "
			"AND N.fzlsid = F.fzlsid JOIN T_PNE P ON P.F_NO = F.F_NO AND P.F_YEAR = F.F_YEAR AND P.F_TYPE = F.F_TYPE "
			"AND P.pnesid = N.pnesid", "DISTINCT P.pnekcq");

		w.AddLong("F.F_NO", defendant->key.no);
		w.AddLong("F.F_YEAR", defendant->key.year);
		w.AddChar("F.F_TYPE", defendant->key.type);
		w.AddString("F.fzlegn", defendant->ucn);

		if (print_pne_kcqs(m, str(w)))
			return true;
	}

	// 2017:221 TRQ/IRQ: skip law pnes
	//w.Begin("T_PNE", "DISTINCT pnekcq");
	//w.Add(defendant, "F_NO, F_YEAR, F_TYPE");
	//return print_pne_kcqs(m, str(w));
	return false;
}

#if TESTVER
long eispp_get_sbevid(long index)
{
	return index >= 0 && index < sbe_etypenomer.Count() ? sbe_etypenomer[index] : 0;
}

long eispp_get_sbe_index(const char *name)
{
	char base[SIZE_OF_BASE_NAME];
	char *endptr;

	strcpy(base, basename(name));
	base[8] = '\0';

	long index = strtol(base + 6, &endptr, 0x10);
	return endptr == base + 8 ? index : -1;
}
#endif  // TESTVER

long eispp_get_sbevid(const char *name)
{
#if TESTVER
	return eispp_get_sbevid(eispp_get_sbe_index(name));
#else  // TESTVER
	char base[SIZE_OF_BASE_NAME];

	strcpy(base, basename(name));
	base[8] = '\0';
	return sbe_etypenomer[strtol(base + 6, NULL, 0x10)];
#endif  // TESTVER	
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_ADMIN
void initialize_eispp() { }
long eispp_instance(long) { return 0; }
bool eispp_matches(char, long) { return true; }
void eispp_accept(TWindow *, TRequest *) { }
void eispp_accept(TWindow *, TInReg *) { }
void eispp_accept(TWindow *, TLawsuit *, TSideWindArray *) { }
void eispp_accept(TWindow *, TLawsuit *, TSession *, TDecision *, bool) { }
void eispp_delete(TRCDKeyContainer *) { }
void eispp_delete(TPrimeContainer *) { }
void eispp_delete(TOutReg *, bool) { }
void eispp_edit_lawsuit(TLawsuit *) { }
void eispp_edit_fzlpne(TWindow *, TLawsuit *) { }
void eispp_renumber(TLawsuit *, TLawsuit *) { }
bool eispp_checkout(TOutReg *) { return false; }
void eispp_send(TWindow *, TOutReg *, TInReg *) { }
void eispp_receive(TWindow *, TOutReg *) { }
bool eispp_stsakt(TWindow *, TOriginContainer *) { return false; }
bool eispp_print_pnes(mstr &, const TSideWind *) { return false; }
#if TESTVER
long eispp_get_sbevid(long) { return 0; }
long eispp_get_sbe_index(const char *) { return 0; }
#endif  // TESTVER
long eispp_get_sbevid(const char *) { return 0; }
#endif  // ADMIN
