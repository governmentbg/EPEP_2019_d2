#include "cac.h"

DEFINE_RESPONSE_TABLE1(TLawsuitSideListDialog, TOriginSideListDialog)
//{{TLawsuitSideListDialogRSP_TBL_BEGIN}}
	EV_COMMAND_ENABLE(IDEXTRADATA, ExtraDataBNEnable),
	EV_BN_CLICKED(IDEXTRADATA, ExtraDataBNClicked),
	EV_BN_CLICKED(IDLIST, ListBNClicked),
//{{TLawsuitSideListDialogRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TLawsuitSideListDialog Implementation}}

TLawsuitSideListDialog::TLawsuitSideListDialog(TWindow* parent, TLawsuit *lawsuit, TRequest *tRequest, long sideMode)
:
	TOriginSideListDialog(parent, lawsuit, lawsuit, sideMode, "делото", IDD_LAWSUIT_SIDE_LIST), request(tRequest)
{
	extraData = new TButton(this, IDEXTRADATA);
}

void TLawsuitSideListDialog::Accept()
{
	xfer_bank_case(Lawsuit(), sideWinds->Array(), request, edit ? XFER_EDIT : XFER_ADD);
	eispp_accept(this, Lawsuit(), (TSideWindArray *) sideWinds->Array());
}

void TLawsuitSideListDialog::Reject()
{
	eportal_send(NULL, Lawsuit(), LOG_DELETE);
}

void TLawsuitSideListDialog::ExtraDataBNEnable(TCommandEnabler &tce)
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	tce.Enable((statable || strchr(KIND_REQUEST_152XS, Lawsuit()->sourceKind)) && sideWinds->SideWind());
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_ADMIN
	tce.Enable(false);
#endif  // ADMIN
}

void TLawsuitSideListDialog::ExtraDataBNClicked()
{
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	// 2003:310 BRW: FIX: arrest was missing; accuse for STATABLE
	if (strchr(KIND_REQUEST_152XS, Lawsuit()->sourceKind))
		arrest_sidewind(this, Lawsuit(), sideWinds->SideWind(), true);
	else	// 2010:077 LPR: accuse moved inside #if, extraData is blocked for other courts anyway
		accuse_sidewind(this, container, sideWinds->SideWind());
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
}

void TLawsuitSideListDialog::ListBNClicked()
{
	sideWinds->List(false);
}
