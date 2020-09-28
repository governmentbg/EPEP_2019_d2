DEFINE_RESPONSE_TABLE1(TOutRegReceiveDialog, TOutRegReceiveBaseDialog)
//{{TOutRegReceiveDialogRSP_TBL_BEGIN}}
	EV_CBN_SELCHANGE(IDC_OUTREG_RECEIVE_RESULT_I, ResultCBNSelChange),
	EV_CBN_SELCHANGE(IDC_OUTREG_RECEIVE_RESULT_II, ResultCBNSelChange),
	EV_CBN_SELCHANGE(IDC_OUTREG_RECEIVE_RESULT_KS, ResultCBNSelChange),
	EV_CBN_SELCHANGE(IDC_OUTREG_RECEIVE_VSS_INDEX, ResultCBNSelChange),
	EV_CBN_SELCHANGE(IDC_OUTREG_RECEIVE_VZ_COURT, VZCourtCBNSelChange),
	EV_CBN_SELCHANGE(IDC_OUTREG_RECEIVE_KS_COURT, KSCourtCBNSelChange),
	EV_CBN_SELCHANGE(IDC_OUTREG_RECEIVE_VZ_LAWSUIT_KIND, VZLawsuitKindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_OUTREG_RECEIVE_KS_LAWSUIT_KIND, KSLawsuitKindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_OUTREG_RECEIVE_VZ_XFER_KIND, VZLawsuitKindCBNSelChange),
	EV_CBN_SELCHANGE(IDC_OUTREG_RECEIVE_KS_XFER_KIND, KSLawsuitKindCBNSelChange),
	EV_BN_CLICKED(IDOK, OKBNClicked),
	EV_BN_CLICKED(IDLOAD, LoadBNClicked),
	EV_EN_UPDATE(IDC_OUTREG_RECEIVE_RETURNED, ReturnedENUpdate),
//{{TOutRegReceiveDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TOutRegReceiveDialog Implementation}}

TOutRegReceiveDialog::TOutRegReceiveDialog(TWindow* parent, TOutReg *outReg, const char *tResults, const char *tComplects,
	const char *tServices, const char *tProcedures, const char *tResultsVZ, const char *tResultsKS, bool tKSRecv,
	TReceiver *tReceiverVZ, TReceiver *tReceiverKS, int resId)
:
	TOutRegReceiveBaseDialog(parent, outReg, tResults, resId), complects(tComplects), services(tServices),
		procedures(tProcedures), resultsVZ(tResultsVZ), resultsKS(tResultsKS), ksRecv(tKSRecv), receiverVZ(tReceiverVZ),
		receiverKS(tReceiverKS), vz(CONNECT_TYPE_VZ_LAWSUIT, outReg), ks(CONNECT_TYPE_KS_LAWSUIT, outReg),
		lawsuitKind('\0'), initialized(false)
{
	resultsVZopt.cat(resultsVZ);
	if (!resultsVZopt.chr(RESOLUTION_FAKE))
		resultsVZopt.cach(RESOLUTION_FAKE);

	vz.lawsuitKind = new TConnectKindAliasFace(this, IDC_OUTREG_RECEIVE_VZ_LAWSUIT_KIND, "F_VZ_LAWSUIT_KIND",
		CONNECT_TYPE_VZ_LAWSUIT, ALIAS_EMPTY);
	vz.lawsuitNo = new TLongFace(this, IDC_OUTREG_RECEIVE_VZ_LAWSUIT_NO, "F_VZ_LAWSUIT_NO", SIZE_OF_NO);
	vz.lawsuitYear = new TYearFace(this, IDC_OUTREG_RECEIVE_VZ_LAWSUIT_YEAR, "F_VZ_LAWSUIT_YEAR");
	vz.court = new TStringAliasFace(this, IDC_OUTREG_RECEIVE_VZ_COURT, "F_VZ_COURT", receiverVZ, C2S[UCN_INSTANCE_HIGHER],
		ALIAS_OPTION);
	vz.decisionKind = new TCharAliasFace(this, IDC_OUTREG_RECEIVE_VZ_DECISION_KIND, "F_DECISION_KIND", Kind,
		KIND_VISIBLE_ENDOCS, ALIAS_EMPTY);
	vz.decisionDate = new TDateFace(this, IDC_OUTREG_RECEIVE_VZ_DECISION_DATE, "F_DECISION_DATE", false);
	resultI = new TCharAliasFace(this, IDC_OUTREG_RECEIVE_RESULT_I, "F_RESULT_I", Resolution, resultsVZ);
	vz.remark = remark;
	ks.lawsuitKind = new TConnectKindAliasFace(this, IDC_OUTREG_RECEIVE_KS_LAWSUIT_KIND, "F_KS_LAWSUIT_KIND",
		CONNECT_TYPE_KS_LAWSUIT, ALIAS_EMPTY);
	ks.lawsuitNo = new TLongFace(this, IDC_OUTREG_RECEIVE_KS_LAWSUIT_NO, "F_KS_LAWSUIT_NO", SIZE_OF_NO);
	ks.lawsuitYear = new TYearFace(this, IDC_OUTREG_RECEIVE_KS_LAWSUIT_YEAR, "F_KS_LAWSUIT_YEAR");
	ks.court = new TStringAliasFace(this, IDC_OUTREG_RECEIVE_KS_COURT, "F_KS_COURT", receiverKS, C2S[UCN_INSTANCE_HIGHER],
		ksRecv ? ALIAS_OPTION : ALIAS_EMPTY);
	ks.decisionKind = new TCharAliasFace(this, IDC_OUTREG_RECEIVE_KS_DECISION_KIND, "F_DECISION_KIND", Kind,
		KIND_VISIBLE_ENDOCS, ALIAS_EMPTY);
	ks.decisionDate = new TDateFace(this, IDC_OUTREG_RECEIVE_KS_DECISION_DATE, "F_DECISION_DATE", false);
	resultKS = new TCharAliasFace(this, IDC_OUTREG_RECEIVE_RESULT_KS, "F_RESULT_KS", Resolution, resultsKS);
	ks.remark = new TStringFace(this, IDC_OUTREG_RECEIVE_REMARK_KS, "F_REMARK_KS", SIZE_OF_OUTREG_REMARK);
	resultII = new TCharAliasFace(this, IDC_OUTREG_RECEIVE_RESULT_II, "F_RESULT_II", Resolution, str(resultsVZopt));
	vssIndex = new TVSSIndexAliasFace(this, IDC_OUTREG_RECEIVE_VSS_INDEX, "F_VSS_INDEX");
	if (eispp_checkout(outReg))
	{
		vz.xferKind = new TXferKindAliasFace(this, IDC_OUTREG_RECEIVE_VZ_XFER_KIND, "F_VZ_XFER_KIND",
			CONNECT_TYPE_VZ_LAWSUIT, ALIAS_EMPTY);
		ks.xferKind = new TXferKindAliasFace(this, IDC_OUTREG_RECEIVE_KS_XFER_KIND, "F_KS_XFER_KIND",
			CONNECT_TYPE_KS_LAWSUIT, ALIAS_EMPTY);
	}
	else
	{
		vz.xferKind = NULL;
		ks.xferKind = NULL;
	}
	load = new TPushButton(this, IDLOAD);
}

void TOutRegReceiveDialog::OKBNClicked()
{
	if (IsValid() && (!vssIndex->IsWindowEnabled() || any(vssIndex->S()) ||
		ask("Не е попълнен резултат - индекс за справки за ВСС. Желаете ли да продължите?")))
	{
		*OutReg() << this;
		if (OutReg()->Update())
		{
			// 2003:171 date->Date() -> returned->Date()
			trouble.Move(OutReg(), returned->Date());
			vz.Save();
			ks.Save();
			CloseDialog(registry->GetCheck() == BF_CHECKED ? IDREGISTRY : IDOK);
		}
	}
}

bool TOutRegReceiveDialog::IsValid()
{
	bool isValid = TOutRegReceiveBaseDialog::IsValid();

	if (isValid)
	{
		bool otherCount = (resultI->C() == RESOLUTION_OTHER) + (resultII->C() == RESOLUTION_OTHER) +
			(resultKS->C() == RESOLUTION_OTHER);

		if (otherCount >= 2)
			isValid = error("Полетата резултат от АС, резултат от ВКС и II резултат от АС не си съответстват.");
		else if (!vz.Valid())
			isValid = error("Полетата за връщане от въззивна инстанция не си съответстват.");
		else if (!ks.Valid())
			isValid = error("Полетата за връщане от касационна инстанция не си съответстват.");
	}

	return isValid;
}

bool TOutRegReceiveDialog::HaveResult(char resolution)
{
	return resultI->C() == resolution || resultII->C() == resolution || resultKS->C() == resolution;
}

void TOutRegReceiveDialog::VZCourtChange(bool setDefaultKind)
{
	vz.CourtChange(setDefaultKind);
	VZResultChange();
}

void TOutRegReceiveDialog::VZResultChange()
{
	resultI->SetXCrit(atob(vz.court->S()) ? resultsVZ : C2S[RESOLUTION_FAKE]);

	if (resultI->C() == RESOLUTION_FAKE)
	{
		resultI->SetC(OutReg()->resultI);
		vz.remark->SetS("");
		vz.remark->EnableWindow(false);
	}
	else
		vz.remark->EnableWindow(true);
}

void TOutRegReceiveDialog::KSCourtChange(bool setDefaultKind)
{
	ks.CourtChange(setDefaultKind);
	KSResultChange();
}

void TOutRegReceiveDialog::KSResultChange()
{
	resultKS->SetXCrit(atob(ks.court->S()) ? resultsKS : C2S[RESOLUTION_FAKE]);

	if (resultKS->C() == RESOLUTION_FAKE)
	{
		resultKS->SetC(OutReg()->resultKS);
		ks.remark->SetS("");
		ks.remark->EnableWindow(false);
	}
	else
		ks.remark->EnableWindow(true);
}

void TOutRegReceiveDialog::ResultCBNSelChange()
{
	// Result II is available if result I is set and resultKS is 2ndable
	// 2013:233 FIX: resultII is allowed for regional
	if (resultI->C() != RESOLUTION_FAKE && strchr(RESOLUTION_2NDABLES, resultKS->C()))
		resultII->SetXCrit(str(resultsVZopt));
	else
		resultII->SetXCrit(C2S[RESOLUTION_FAKE]);

	ReturnedENUpdate();

	if (HaveResult(RESOLUTION_COMPLECT))
		result->SetXCrit(complects);
	else if (HaveResult(RESOLUTION_SERVICE))
		result->SetXCrit(services);
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	else if (HaveResult(RESOLUTION_PROCEDURE))
		result->SetXCrit(procedures);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	else
		result->SetXCrit(results);
}

void TOutRegReceiveDialog::SetupWindow()
{
	TOutRegReceiveBaseDialog::SetupWindow();

	// 2009:286 LPR: HIGHER only; 2010:334 LPR: LAWSUITS only; 2011:175 RESTARTS
	if (strchr(UCN_INSTANCE_RESTARTS, OutReg()->receiverType) && strchr(TYPE_LAWSUITS, OutReg()->lawsuit.type))
		load->EnableWindow(true);

	vz.Try();
	ks.Try();

	if (vz.xferKind)
	{
		vz.lawsuitKind->ShowWindow(SW_HIDE);
		vz.xferKind->ShowWindow(SW_SHOW);
		ks.lawsuitKind->ShowWindow(SW_HIDE);
		ks.xferKind->ShowWindow(SW_SHOW);
	}

	// 2012:069 LPR: compatibility: preserve resultKS without court
	if (!atob(ks.court->S()) && OutReg()->resultKS != RESOLUTION_FAKE)
		ks.court->SetSelIndex(1);
	// 2012:055 FIX: always invoke these, to clear the fields if not appeal return
	VZCourtChange(!edit);
	KSCourtChange(!edit);
	initialized = true;
	ResultCBNSelChange();
	vssIndex->SetS(OutReg()->vssIndex);
}

void TOutRegReceiveDialog::LoadBNClicked()
{
	// 2012:003 LPR: receive everything except vss index in vz/ks
	if (xfer_receive(this, OutReg(), ksRecv ? &ks : &vz, &trouble))
		vssIndex->SetS(OutReg()->vssIndex);
}

void TOutRegReceiveDialog::VZCourtCBNSelChange()
{
	VZCourtChange(true);
	ResultCBNSelChange();
}

void TOutRegReceiveDialog::KSCourtCBNSelChange()
{
	KSCourtChange(true);
	ResultCBNSelChange();
}

void TOutRegReceiveDialog::VZLawsuitKindCBNSelChange()
{
	vz.LawsuitKindChange();
	VZResultChange();
	ResultCBNSelChange();
}

void TOutRegReceiveDialog::KSLawsuitKindCBNSelChange()
{
	ks.LawsuitKindChange();
	KSResultChange();
	ResultCBNSelChange();
}

void TOutRegReceiveDialog::ReturnedENUpdate()
{
	if (initialized)
	{
		// 2008:190 LPR: vss index is available if any other result isn't empty
		if (resultI->C() != RESOLUTION_FAKE || resultII->C() != RESOLUTION_FAKE || resultKS->C() != RESOLUTION_FAKE)
		{
			if (date_valid(returned->S(), true))
			{
				if (!lawsuitKind)
					lawsuitKind = origin2LawsuitKind(OutReg()->lawsuit, true);

				vssIndex->SetCrit(COURT_TYPE + '0', C2S[lawsuitKind ? kind2type(lawsuitKind) : OutReg()->college],
					lawsuitKind, returned->Date());
			}
		}
		else
			vssIndex->SetCrit('\0', NULL, '\0', Today);
	}
}
