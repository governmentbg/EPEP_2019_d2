#include "cac.h"

bool WinXPOrLater = false;
bool Win7OrLater = false;
bool Windows64Bit = false;
TRect AdjustedRect(0, 50, 640, 480);
WORD MainShowWindow = SW_MAXIMIZE;
void WINAPI (*SetThemeAppProperties)(DWORD) = NULL;
HRESULT WINAPI (*EnableThemeDialogTexture)(HWND, DWORD) = NULL;
DWORD WINAPI (*GetProcessId)(HANDLE) = NULL;
NET_API_STATUS WINAPI (*NetValidateName)(LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, NETSETUP_NAME_TYPE) = NULL;
DWORD WINAPI (*DsGetDcNameA)(LPCSTR, LPCSTR, GUID *, LPCSTR, ULONG, PDOMAIN_CONTROLLER_INFOA *) = NULL;
int AppMainBottom = 0;
TWindow *AppMainWindow = NULL;
HICON AppNameIcon = NULL;
TPostGresExtraDataBase *ExtraDataBase = NULL;
BOOL WINAPI (*PathIsRelativeA)(LPCTSTR lpszPath);
static long AppIdleCount = 0;

#define SET_REPEATER_TRUE(COMMAND) (repeat_func) = (COMMAND)
#define SET_REPEATER(COMMAND, REPEAT) if (REPEAT) SET_REPEATER_TRUE(COMMAND)
#define CHECK_REPEATER() if (!(repeat_func)) break

// {{TDecoratedFrame = TAppNameFrame}}
class TAppNameFrame : public TDecoratedFrame
{
public:
	TAppNameFrame(TWindow* parent, const char far* title, TWindow* clientWnd, bool trackMenuSelection = false);

//{{TAppNameFrameVIRTUAL_BEGIN}}
protected:
	HMENU GetMenu();

	void SetupWindow();
	virtual TResult EvCommand(uint Id, HWND hWndCtl, uint notifyCode);
	virtual void EvCommandEnable(TCommandEnabler& ce);
//{{TAppNameFrameVIRTUAL_BEGIN}}

//{{TAppNameFrameRSP_TBL_BEGIN}}
protected:
	void EvTimer(uint timerId);
	void EvClose();
//{{TAppNameFrameRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAppNameFrame);
};    //{{TAppNameFrame}}

// ----- TAppNameFrame -------------------------------------------------------
DEFINE_RESPONSE_TABLE1(TAppNameFrame, TDecoratedFrame)
//{{TAppNameFrameFaceRSP_TBL_BEGIN}}
	EV_WM_TIMER,
	EV_WM_CLOSE,
//{{TAppNameFrameFaceRSP_TBL_END}}
END_RESPONSE_TABLE;

TAppNameFrame::TAppNameFrame(TWindow* parent, const char far* title, TWindow* clientWnd, bool trackMenuSelection)
	: TDecoratedFrame(parent, title, clientWnd, trackMenuSelection)
{
}

HMENU TAppNameFrame::GetMenu()
{
	static HMENU hMenu = NULL;

	if (!hMenu)
		hMenu = TDecoratedFrame::GetMenu();

	return hMenu;
}

void TAppNameFrame::SetupWindow()
{
	TDecoratedFrame::SetupWindow();

	if (PING_DATABASE)
		SetTimer(0, 1000, NULL);
}

TResult TAppNameFrame::EvCommand(uint Id, HWND hWndCtl, uint notifyCode)
{
	TResult result = 0;

	if (big_sas_lock(AppMainWindow))
	{
		if (Id >= 10000 && Id < 20000)
		{
			HMENU hMenu = GetMenu();

			if (hMenu)
			{
				MENUITEMINFO item;
				char *s;

				item.cbSize = sizeof item;
				item.fMask = MIIM_TYPE;
				item.dwTypeData = PrimaryTitle;
				item.cch = sizeof PrimaryTitle;

				if (GetMenuItemInfo(hMenu, Id, FALSE, &item) && item.fType == MFT_STRING)
				{
					if ((s = strchr(PrimaryTitle, '&')) != NULL)
						strmove(s, s + 1);
					if ((s = strchr(PrimaryTitle, '\t')) != NULL)
						*s = '\0';
					// Hmmm... should have common logic here
					if (!strncmp(PrimaryTitle, "За ", 3))
					{
						strmove(PrimaryTitle + 8, PrimaryTitle);
						memcpy(PrimaryTitle, "Справка за ", 11);
					}
					else if (!strncmp(PrimaryTitle, "На ", 3))
					{
						strmove(PrimaryTitle + 6, PrimaryTitle);
						memcpy(PrimaryTitle, "Избор на ", 9);
					}
				}
				else
					*PrimaryTitle = '\0';
			}

			PrimaryCommand = Id;
			SET_REPEATER_TRUE(0);
		}

		electric_signal(ELECTRIC_START, NULL);

		try
		{
			result = TDecoratedFrame::EvCommand(Id, hWndCtl, notifyCode);
		}
		catch (xmsg& x)
		{
			handle_exception(x.why().c_str());
			AppMainWindow->SetActiveWindow();
		}
		catch (...)
		{
			handle_exception(UNHANDLED_EXCEPT);
			AppMainWindow->SetActiveWindow();
		}

		electric_signal(ELECTRIC_FINAL, NULL);
		big_sas_unlock();
	}

	return result;
}

void TAppNameFrame::EvCommandEnable(TCommandEnabler& tce)
{
	HMENU hMenu = GetMenu();
	MENUITEMINFO info = { 0 };

	if (hMenu)
	{
		info.cbSize = sizeof info;
		info.fMask = MIIM_SUBMENU;

		if (!GetMenuItemInfo(hMenu, tce.Id, MF_BYCOMMAND, &info))
			info.hSubMenu = hMenu;
	}

	tce.Enable(info.hSubMenu || command_enabled(tce.Id, MENU_AUTOMATIC));
}

void TAppNameFrame::EvTimer(uint timerId)
{
	AppIdleCount++;
	TDecoratedFrame::EvTimer(timerId);
}

void TAppNameFrame::EvClose()
{
	mstr m;

	if (electric_foreach(ELECTRIC_COUNT, NULL))
		m.cat("Има отворени електронни папки");

	if (TShowDialog::GetCount())
		m.cat(any(m) ? "и справки" : "Има отворени справки");

	m.sep(". ");

	if (ask("%sЖелаете ли да приключите работа с програмата?", str(m)))
	{
		electric_foreach(ELECTRIC_CLOSE, NULL);
		TShowDialog::CloseAll();
		TDecoratedFrame::EvClose();
	}
}

//{{TApplication = TAppNameApp}}
class TAppNameApp : public TApplication
{
private:
	// The main window; 2007:200 TWindow -> TDecoratedFrame
	TDecoratedFrame *parent;

public:
	TAppNameApp(const char far *name);
	virtual ~TAppNameApp();

protected:
//{{TAppNameAppVIRTUAL_BEGIN}}
	virtual void InitMainWindow();
	virtual void InitInstance();
	virtual bool IdleAction(long idleCount);
//{{TAppNameAppVIRTUAL_END}}

//{{TAppNameAppRSP_TBL_BEGIN}}
#if COURT_TYPE == COURT_APPEAL
	void CmRegNewPunishment();
	void CmRegNewCitizen();
	void CmRegNewReCitizen();
	void CmRegNewTrade();
	void CmRegNewReTrade();
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	void CmRegNewAdmin();
	void CmRegNewTricky();
	void CmRegNewCitizen();
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	void CmRegNewPunishment();
	void CmRegNewCitizen();
	void CmRegNewTrade();
	void CmRegNewAdmin();
	void CmRegNewFirm();
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	void CmRegNewPunishment();
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	void CmRegNewMarriage();
	void CmRegNewCitizen();
	void CmRegNewPunishment();
	void CmRegNewAdmin();
#endif  // REGIONAL
	void CmRegEdit();
	void CmRegEditRequest();
	void CmRegNewSurround();
	void CmRegEditSurround();
	void CmRegInNew();
	void CmRegInNewX();
	void CmRegInNewEPortal();
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	void CmRegInSurrouendocs();
#endif  // APPEAL || AREAL
	void CmRegInEdit();
	void CmRegNewInRegEvent();
	void CmRegEditInRegEvent();
	void CmRegOutAppeal();
	void CmRegOutJuriConf();
	void CmRegOutConfHigh();
	void CmRegOutSendOther();
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	void CmRegOutReturnLower();
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmRegOutReturnProsec();
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	void CmRegOutEdit();
	void CmRegOutReceive();
	void CmRegPayments();
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_REGIONAL
	void CmClerkNew();
#endif  // APPEAL || REGIONAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	void CmClerkNew1st();
	void CmClerkNew2nd();
#endif  // ADMIN || AREAL || MILITARY
	void CmClerkNewLawsuit();
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY  || COURT_TYPE == COURT_REGIONAL
	void CmClerkNewJurisd();
#endif  // ADMIN || AREAL || MILITARY || REGIONAL
	void CmClerkNewReturned();
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmClerkNewReReturned();
	void CmClerkNewReNewed();
#endif  // AREAL || MILITARY || REGIONAL
	void CmClerkEditLawsuit();
#if COURT_TYPE == COURT_AREAL
	void CmClerkEnterReregisterDate();
#endif  // AREAL
	void CmClerkNewResolutionUCN();
	void CmClerkNewRequestResolution();
	void CmClerkEditResolutionUCN();
	void CmClerkEditRequestResolution();
	void CmClerkClosedResult();
	void CmClerkEditClosed();
	void CmClerkSet();
	void CmClerkSetLoad();
	void CmClerkEditSet();
	void CmClerkReset();
	void CmClerkUnset();
	void CmClerkFixedList();
	void CmClerkPrintMessages();
	void CmClerkNewJudicialAct();
	void CmClerkEditJudicialAct();
	void CmClerkForceDate();
	void CmClerkInOutMovement();
	void CmClerkEditScheds();
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmClerkNewDistribJurors();
	void CmClerkEditDistribJurors();
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	void CmClerkExternSessions();
	void CmClerkExportArchives();
	void CmClerkImportArchived();
#endif  // AREAL
	void CmClerkLegalAid();
	void CmClerkNIAProtocol();
	void CmClerkNIACollect();
	void CmClerkBarcodeScan();
	void CmClerkOutMail();
	void CmClerkUnfilteredActs();
	void CmSecrOpenResult();
	void CmSecrClosedResult();
	void CmSecrEditClosed();
	void CmSecrSet();
	void CmSecrSetLoad();
	void CmSecrEditSet();
	void CmSecrReset();
	void CmSecrUnset();
	void CmSecrFixedList();
	void CmSecrPrintMessages();
	void CmSecrNewJudicialAct();
	void CmSecrEditJudicialAct();
	void CmSecrForceDate();
	void CmSecrInOutMovement();
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmSecrNewDistribJurors();
	void CmSecrEditDistribJurors();
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	void CmSecrExternSessions();
	void CmSecrExportArchives();
	void CmSecrImportArchived();
#endif  // AREAL
	void CmSecrLegalAid();
	void CmSecrNIAProtocol();
	void CmSecrNIACollect();
	void CmSecrUnfilteredActs();
	void CmJudgeNewJudicialAct();
	void CmJudgeEditJudicialAct();
	void CmJudgeClosedResult();
	void CmJudgeEditClosed();
	void CmJudgeSetLoad();
	void CmJudgeFixedList();
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmJudgeNewDistribJurors();
	void CmJudgeEditDistribJurors();
#endif  // AREAL || MILITARY || REGIONAL
	void CmJudgeRegisterAct();
	void CmJudgeUnfilteredActs();
	void CmArchNewArchive();
	void CmArchEditArchive();
#if COURT_TYPE == COURT_AREAL
	void CmArchImportArchives();
	void CmArchExportArchived();
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmArchNewProve();
	void CmArchEditProve();
	void CmArchNewProveAction();
	void CmArchEditProveAction();
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	void CmArchInOutMovement();
	void CmArchNewDestroy();
	void CmArchEditDestroy();
	void CmDelivExternal();
	void CmDelivInternal();
	void CmDelivBarcodeScan();
	void CmQueryUCN();
	void CmQueryGroup();
	void CmElectricBlue();
#if COURT_TYPE == COURT_AREAL
	void CmQueryFirm();
#endif  // AREAL
	void CmQueryConnect();
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmQueryJurorDistrib();
#endif  // AREAL || MILITARY || REGIONAL
	void CmQueryArchive();
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmQueryEisppLaws();
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	void CmFloatCombo();
	void CmFloatRequest();
	void CmFloatConnect();
	void CmFloatIndex();
	void CmFloatSubpoena();
	void CmFloatAnnounce();
	void CmFloatSurround();
	void CmFloatSenders();
	void CmFloatOutReg();
	void CmFloatUnappealed();
	void CmFloatMovement();
	void CmFloatSched();
	void CmFloatPenality();
	void CmFloatReward();
	void CmFloatTax();
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmFloatJuror();
#endif  // AREAL || MILITARY || REGIONAL
	void CmFloatExecList();
#if COURT_TYPE == COURT_REGIONAL
	void CmFloatHeritage();
#endif  // REGIONAL
	void CmFloatPayments();
	void CmFloatRegisterAct();
	void CmQueryRegisterAct();
	void CmQueryMovement();
	void CmExpireSched();
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	void CmQueryExchange();
#endif  // AREAL || REGIONAL
	void CmTimedSession();
	void CmTimedLawsuit();
	void CmUnfinishedLaw();
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmQueryJurorLoad();
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	void CmQueryBankruptcy();
	void CmQueryInconSide();
#endif  // AREAL
	void CmQueryImportant();
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmQueryCorrupt();
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	void CmPeriodSession();
	void CmYoungPeriod();
	void CmUnwrittenAct();
	void CmUnwrittenSes();
	void CmPeriodOutReg();
	void CmFloatArchive();
	void CmFloatDestroy();
	void CmQuerySubpoenas();
	void CmQueryPayments();
	void CmQueryNIAProtocols();
	void CmQueryJudges();
	void CmQueryActivity();
#if COURT_TYPE == COURT_AREAL
	void CmQueryP24a1();
	void CmQueryBanConSist();
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	void CmQueryStatFirst();
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmQueryStatSecond();
#endif  // ADMIN || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	void CmQueryStatThird();
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	void CmCombineCorrupt();
#endif  // APPEAL || AREAL
	void CmAdminObjectCount();
	void CmBookDesc();
	void CmBookDescHtml();
	void CmBookDescDeliv();
#if COURT_TYPE == COURT_REGIONAL
	void CmBookZZDN();
	void CmBookSuccess();
#endif  // REGIONAL
	void CmBookAbc();
	void CmBookAbcHtml();
	void CmBookTime();
	void CmBookTimeHtml();
	void CmBookClosed();
	void CmBookClosedHtml();
	void CmBookInDoc();
	void CmBookRequest();
	void CmBookInReg();
	void CmBookOutReg();
	void CmBookDecision();
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
	void CmBook39();
#endif  // APPEAL || ADMIN || AREAL
	void CmBookRecusal();
#if COURT_TYPE == COURT_REGIONAL
	void CmBookDivorce();
#endif  // REGIONAL
#if COURT_TYPE == COURT_ADMIN
	void CmBook251();
#endif  // ADMIN
	void CmBookExecList();
	void CmBookAppeal();
#if COURT_TYPE == COURT_REGIONAL
	void CmBookEuher();
#endif  // REGIONAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmBookExecution();
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	void CmBook634();
	void CmBookSlowdown();
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmBookProve();
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	void CmBookPenalities();
	void CmBookArchive();
	void CmBookExternal();
	void CmBookInternal();
	void CmBookPrint();
	void CmAdminUsers();
	void CmAdminKnownPersons();
	void CmAdminSubjects();
	void CmAdminCompositions();
	void CmAdminPenalities();
	void CmAdminReasons();
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmAdminDangers();
	void CmAdminArticles();
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	void CmAdminBanConSists();
#endif  // AREAL
	void CmAdminSlcReasons();
	void CmAdminArchiveIndexes();
	void CmAdminBankAccounts();
	void CmAdminCurrences();
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmAdminSessionPrices();
#endif  // AREAL || MILITARY || REGIONAL
	void CmAdminSenders();
	void CmAdminCalendar();
	void CmAdminHalls();
	void CmAdminConfig();
	void CmAdminEPortQuery();
	void CmAdminEPortLawsuitAssign();
	void CmAdminEPortPersonAssign();
	void CmAdminEPortSend();
	void CmAdminEPortDelete();
	void CmAdminCountries();
	void CmAdminInformeds();
	void CmAdminServiceUsers();
	void CmAdminAreas();
	void CmAdminRegions();
	void CmAdminUCLPS();
	void CmAdminStreets();
	void CmAdminLog();
	void CmAdminDeleteInDoc();
	void CmAdminDeleteRequestResolution();
	void CmAdminDeleteLawsuit();
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmAdminDeleteEisppNmr();
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	void CmAdminDeleteLawsuitMovement();
	void CmAdminDeleteLawsuitEvent();
	void CmAdminDeleteInRegEvent();
	void CmAdminDeleteOutreg();
	void CmAdminDeleteOutregReturn();
	void CmAdminDeleteNIAProtocol();
	void CmAdminDeleteForeignAddress();
	void CmAdminDeleteOldCitizen();
	void CmAdminDeleteMovement();
	void CmAdminDeleteArchive();
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmAdminDeleteProve();
	void CmAdminDeleteProveAction();
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	void CmAdminDeleteXfer();
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmAdminDeleteJurorDistrib();
#endif  // AREAL || MILITARY || REGIONAL
	void CmAdminDiscardCitizens();
	void CmAdminRenumberRequest();
	void CmAdminRenumberLawsuit();
	void CmAdminRenumberUXN();
	void CmAdminRenumberSurround();
	void CmAdminRenumberInreg();
	void CmAdminRenumberStreet();
	void CmAdminRenumberSubjects();
	void CmAdminJoinUXN();
	void CmAdminJoinStreet();
	void CmAdminJoinEqualStreets();
	void CmAdminCheckoutCitizens();
	void CmAdminCheckoutFirms();
	void CmAdminCheckoutStreetMaps();
	void CmAdminResetSession();
	void CmAdminRekindLawsuit();
	void CmAdminRecomposeLawsuit();
	void CmAdminCopySideWinds();
	void CmAdminRenumberSideWind();
	void CmAdminMoveJudicialAct();
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmAdminEisppEditLawsuit();
	void CmAdminEisppEditFzlPne();
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	void CmAdminUpdateRequests();
	void CmAdminUpdateLongTexts();
	void CmAdminUpdateLogKinds();
	void CmAdminUpdateEPortals();
#if COURT_TYPE == COURT_AREAL
	void CmAdminImportReRegistered();
#endif  // AREAL
	void CmAdminImportPersonalNo();
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	void CmAdminExportBankrupt();
#endif  // APPEAL || AREAL
#if COURT_TYPE == COURT_ADMIN
	void CmAdminExportCSV();
#endif  // ADMIN
	void CmAdminQueryWeb();
#if COURT_TYPE == COURT_AREAL
	void CmAdminQueryBankrupt();
	void CmAdminBankrupts();
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	void CmAdminQueryPolice();
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	void CmAdminQueryProsec();
#endif  // AREAL || REGIONAL
	void CmAdminFloatPublication();
	void CmAdminFloatElectric();
	void CmAdminQueryXferWeb();
	void CmAdminQueryYoung();
	void CmAdminRandomAct();
	void CmHelpAbout();
	void CmHelpRefresh();
	void CmHelpLogin();
	void CmHelpTesting();
//{{TAppNameAppRSP_TBL_END}}
DECLARE_RESPONSE_TABLE(TAppNameApp);
};    //{{TAppNameApp}}

// ----- TApplication --------------------------------------------------------
DEFINE_RESPONSE_TABLE1(TAppNameApp, TApplication)
//{{TAppNameAppRSP_TBL_BEGIN}}
#if COURT_TYPE == COURT_APPEAL
	EV_COMMAND(CM_REG_NEW_PUNISHMENT, CmRegNewPunishment),
	EV_COMMAND(CM_REG_NEW_CITIZEN, CmRegNewCitizen),
	EV_COMMAND(CM_REG_NEW_RECITIZEN, CmRegNewReCitizen),
	EV_COMMAND(CM_REG_NEW_TRADE, CmRegNewTrade),
	EV_COMMAND(CM_REG_NEW_RETRADE, CmRegNewReTrade),
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
	EV_COMMAND(CM_REG_NEW_ADMIN, CmRegNewAdmin),
	EV_COMMAND(CM_REG_NEW_TRICKY, CmRegNewTricky),
	EV_COMMAND(CM_REG_NEW_CITIZEN, CmRegNewCitizen),
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_REG_NEW_PUNISHMENT, CmRegNewPunishment),
	EV_COMMAND(CM_REG_NEW_CITIZEN, CmRegNewCitizen),
	EV_COMMAND(CM_REG_NEW_TRADE, CmRegNewTrade),
	EV_COMMAND(CM_REG_NEW_ADMIN, CmRegNewAdmin),
	EV_COMMAND(CM_REG_NEW_FIRM, CmRegNewFirm),
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
	EV_COMMAND(CM_REG_NEW_PUNISHMENT, CmRegNewPunishment),
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_REG_NEW_MARRIAGE, CmRegNewMarriage),
	EV_COMMAND(CM_REG_NEW_CITIZEN, CmRegNewCitizen),
	EV_COMMAND(CM_REG_NEW_PUNISHMENT, CmRegNewPunishment),
	EV_COMMAND(CM_REG_NEW_ADMIN, CmRegNewAdmin),
#endif  // REGIONAL
	EV_COMMAND(CM_REG_EDIT, CmRegEdit),
	EV_COMMAND(CM_REG_EDIT_REQUEST, CmRegEditRequest),
	EV_COMMAND(CM_REG_NEW_SURROUND, CmRegNewSurround),
	EV_COMMAND(CM_REG_EDIT_SURROUND, CmRegEditSurround),
	EV_COMMAND(CM_REG_IN_NEW, CmRegInNew),
	EV_COMMAND(CM_REG_IN_NEW_X, CmRegInNewX),
	EV_COMMAND(CM_REG_IN_NEW_EPORTAL, CmRegInNewEPortal),
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_REG_IN_SURROUENDOCS, CmRegInSurrouendocs),
#endif  // APPEAL || AREAL
	EV_COMMAND(CM_REG_IN_EDIT, CmRegInEdit),
	EV_COMMAND(CM_REG_NEW_INREG_EVENT, CmRegNewInRegEvent),
	EV_COMMAND(CM_REG_EDIT_INREG_EVENT, CmRegEditInRegEvent),
	EV_COMMAND(CM_REG_OUT_APPEAL, CmRegOutAppeal),
	EV_COMMAND(CM_REG_OUT_JURI_CONF, CmRegOutJuriConf),
	EV_COMMAND(CM_REG_OUT_CONF_HIGH, CmRegOutConfHigh),
	EV_COMMAND(CM_REG_OUT_SEND_OTHER, CmRegOutSendOther),
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	EV_COMMAND(CM_REG_OUT_RETURN_LOWER, CmRegOutReturnLower),
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_REG_OUT_RETURN_PROSEC, CmRegOutReturnProsec),
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_REG_OUT_EDIT, CmRegOutEdit),
	EV_COMMAND(CM_REG_OUT_RECEIVE, CmRegOutReceive),
	EV_COMMAND(CM_REG_PAYMENTS, CmRegPayments),
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_CLERK_NEW, CmClerkNew),
#endif  // APPEAL || REGIONAL
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	EV_COMMAND(CM_CLERK_NEW_1ST, CmClerkNew1st),
	EV_COMMAND(CM_CLERK_NEW_2ND, CmClerkNew2nd),
#endif  // ADMIN || AREAL || MILITARY
	EV_COMMAND(CM_CLERK_NEW_LAWSUIT, CmClerkNewLawsuit),
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_CLERK_NEW_JURISD, CmClerkNewJurisd),
#endif  // ADMIN || AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_CLERK_NEW_RETURNED, CmClerkNewReturned),
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_CLERK_NEW_RERETURNED, CmClerkNewReReturned),
	EV_COMMAND(CM_CLERK_NEW_RENEWED, CmClerkNewReNewed),
#endif  // AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_CLERK_EDIT_LAWSUIT, CmClerkEditLawsuit),
#if COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_CLERK_ENTER_REREGISTER_DATE, CmClerkEnterReregisterDate),
#endif  // AREAL
	EV_COMMAND(CM_CLERK_NEW_RESOLUTION_UCN, CmClerkNewResolutionUCN),
	EV_COMMAND(CM_CLERK_NEW_REQUEST_RESOLUTION, CmClerkNewRequestResolution),
	EV_COMMAND(CM_CLERK_EDIT_RESOLUTION_UCN, CmClerkEditResolutionUCN),
	EV_COMMAND(CM_CLERK_EDIT_REQUEST_RESOLUTION, CmClerkEditRequestResolution),
	EV_COMMAND(CM_CLERK_CLOSED_RESULT, CmClerkClosedResult),
	EV_COMMAND(CM_CLERK_EDIT_CLOSED, CmClerkEditClosed),
	EV_COMMAND(CM_CLERK_SET, CmClerkSet),
	EV_COMMAND(CM_CLERK_SET_LOAD, CmClerkSetLoad),
	EV_COMMAND(CM_CLERK_EDIT_SET, CmClerkEditSet),
	EV_COMMAND(CM_CLERK_RESET, CmClerkReset),
	EV_COMMAND(CM_CLERK_UNSET, CmClerkUnset),
	EV_COMMAND(CM_CLERK_FIXED_LIST, CmClerkFixedList),
	EV_COMMAND(CM_CLERK_PRINT_MESSAGES, CmClerkPrintMessages),
	EV_COMMAND(CM_CLERK_NEW_JUDICIAL_ACT, CmClerkNewJudicialAct),
	EV_COMMAND(CM_CLERK_EDIT_JUDICIAL_ACT, CmClerkEditJudicialAct),
	EV_COMMAND(CM_CLERK_FORCE_DATE, CmClerkForceDate),
	EV_COMMAND(CM_CLERK_IN_OUT_MOVEMENT, CmClerkInOutMovement),
	EV_COMMAND(CM_CLERK_EDIT_SCHEDS, CmClerkEditScheds),
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_CLERK_NEW_DISTRIB_JURORS, CmClerkNewDistribJurors),
	EV_COMMAND(CM_CLERK_EDIT_DISTRIB_JURORS, CmClerkEditDistribJurors),
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_CLERK_EXTERN_SESSIONS, CmClerkExternSessions),
	EV_COMMAND(CM_CLERK_EXPORT_ARCHIVES, CmClerkExportArchives),
	EV_COMMAND(CM_CLERK_IMPORT_ARCHIVED, CmClerkImportArchived),
#endif  // AREAL
	EV_COMMAND(CM_CLERK_LEGAL_AID, CmClerkLegalAid),
	EV_COMMAND(CM_CLERK_NIA_PROTOCOL, CmClerkNIAProtocol),
	EV_COMMAND(CM_CLERK_NIA_COLLECT, CmClerkNIACollect),
	EV_COMMAND(CM_CLERK_BARCODE_SCAN, CmClerkBarcodeScan),
	EV_COMMAND(CM_CLERK_OUT_MAIL, CmClerkOutMail),
	EV_COMMAND(CM_CLERK_UNFILTERED_ACTS, CmClerkUnfilteredActs),
	EV_COMMAND(CM_SECR_OPEN_RESULT, CmSecrOpenResult),
	EV_COMMAND(CM_SECR_CLOSED_RESULT, CmSecrClosedResult),
	EV_COMMAND(CM_SECR_EDIT_CLOSED, CmSecrEditClosed),
	EV_COMMAND(CM_SECR_SET, CmSecrSet),
	EV_COMMAND(CM_SECR_SET_LOAD, CmSecrSetLoad),
	EV_COMMAND(CM_SECR_EDIT_SET, CmSecrEditSet),
	EV_COMMAND(CM_SECR_RESET, CmSecrReset),
	EV_COMMAND(CM_SECR_UNSET, CmSecrUnset),
	EV_COMMAND(CM_SECR_FIXED_LIST, CmSecrFixedList),
	EV_COMMAND(CM_SECR_PRINT_MESSAGES, CmSecrPrintMessages),
	EV_COMMAND(CM_SECR_NEW_JUDICIAL_ACT, CmSecrNewJudicialAct),
	EV_COMMAND(CM_SECR_EDIT_JUDICIAL_ACT, CmSecrEditJudicialAct),
	EV_COMMAND(CM_SECR_FORCE_DATE, CmSecrForceDate),
	EV_COMMAND(CM_SECR_IN_OUT_MOVEMENT, CmSecrInOutMovement),
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_SECR_NEW_DISTRIB_JURORS, CmSecrNewDistribJurors),
	EV_COMMAND(CM_SECR_EDIT_DISTRIB_JURORS, CmSecrEditDistribJurors),
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_SECR_EXTERN_SESSIONS, CmSecrExternSessions),
	EV_COMMAND(CM_SECR_EXPORT_ARCHIVES, CmSecrExportArchives),
	EV_COMMAND(CM_SECR_IMPORT_ARCHIVED, CmSecrImportArchived),
#endif  // AREAL
	EV_COMMAND(CM_SECR_LEGAL_AID, CmSecrLegalAid),
	EV_COMMAND(CM_SECR_NIA_PROTOCOL, CmSecrNIAProtocol),
	EV_COMMAND(CM_SECR_NIA_COLLECT, CmSecrNIACollect),
	EV_COMMAND(CM_SECR_UNFILTERED_ACTS, CmSecrUnfilteredActs),
	EV_COMMAND(CM_JUDGE_NEW_JUDICIAL_ACT, CmJudgeNewJudicialAct),
	EV_COMMAND(CM_JUDGE_EDIT_JUDICIAL_ACT, CmJudgeEditJudicialAct),
	EV_COMMAND(CM_JUDGE_CLOSED_RESULT, CmJudgeClosedResult),
	EV_COMMAND(CM_JUDGE_EDIT_CLOSED, CmJudgeEditClosed),
	EV_COMMAND(CM_JUDGE_SET_LOAD, CmJudgeSetLoad),
	EV_COMMAND(CM_JUDGE_FIXED_LIST, CmJudgeFixedList),
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_JUDGE_NEW_DISTRIB_JURORS, CmJudgeNewDistribJurors),
	EV_COMMAND(CM_JUDGE_EDIT_DISTRIB_JURORS, CmJudgeEditDistribJurors),
#endif  // AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_JUDGE_REGISTER_ACT, CmJudgeRegisterAct),
	EV_COMMAND(CM_ARCH_NEW_ARCHIVE, CmArchNewArchive),
	EV_COMMAND(CM_ARCH_EDIT_ARCHIVE, CmArchEditArchive),
#if COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_ARCH_IMPORT_ARCHIVES, CmArchImportArchives),
	EV_COMMAND(CM_ARCH_EXPORT_ARCHIVED, CmArchExportArchived),
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_ARCH_NEW_PROVE, CmArchNewProve),
	EV_COMMAND(CM_ARCH_EDIT_PROVE, CmArchEditProve),
	EV_COMMAND(CM_ARCH_NEW_PROVE_ACTION, CmArchNewProveAction),
	EV_COMMAND(CM_ARCH_EDIT_PROVE_ACTION, CmArchEditProveAction),
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_ARCH_IN_OUT_MOVEMENT, CmArchInOutMovement),
	EV_COMMAND(CM_ARCH_NEW_DESTROY, CmArchNewDestroy),
	EV_COMMAND(CM_ARCH_EDIT_DESTROY, CmArchEditDestroy),
	EV_COMMAND(CM_DELIV_EXTERNAL, CmDelivExternal),
	EV_COMMAND(CM_DELIV_INTERNAL, CmDelivInternal),
	EV_COMMAND(CM_DELIV_BARCODE_SCAN, CmDelivBarcodeScan),
	EV_COMMAND(CM_QUERY_UCN, CmQueryUCN),
	EV_COMMAND(CM_QUERY_GROUP, CmQueryGroup),
	EV_COMMAND(CM_ELECTRIC_BLUE, CmElectricBlue),
#if COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_QUERY_FIRM, CmQueryFirm),
#endif  // AREAL
	EV_COMMAND(CM_QUERY_CONNECT, CmQueryConnect),
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_QUERY_JUROR_DISTRIB, CmQueryJurorDistrib),
#endif  // AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_QUERY_ARCHIVE, CmQueryArchive),
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_QUERY_EISPP_LAWS, CmQueryEisppLaws),
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_FLOAT_COMBO, CmFloatCombo),
	EV_COMMAND(CM_FLOAT_REQUEST, CmFloatRequest),
	EV_COMMAND(CM_FLOAT_CONNECT, CmFloatConnect),
	EV_COMMAND(CM_FLOAT_INDEX, CmFloatIndex),
	EV_COMMAND(CM_FLOAT_SUBPOENA, CmFloatSubpoena),
	EV_COMMAND(CM_FLOAT_ANNOUNCE, CmFloatAnnounce),
	EV_COMMAND(CM_FLOAT_SURROUND, CmFloatSurround),
	EV_COMMAND(CM_FLOAT_SENDERS, CmFloatSenders),
	EV_COMMAND(CM_FLOAT_OUTREG, CmFloatOutReg),
	EV_COMMAND(CM_FLOAT_UNAPPEALED, CmFloatUnappealed),
	EV_COMMAND(CM_FLOAT_MOVEMENT, CmFloatMovement),
	EV_COMMAND(CM_FLOAT_SCHED, CmFloatSched),
	EV_COMMAND(CM_FLOAT_PENALITY, CmFloatPenality),
	EV_COMMAND(CM_FLOAT_REWARD, CmFloatReward),
	EV_COMMAND(CM_FLOAT_TAX, CmFloatTax),
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_FLOAT_JUROR, CmFloatJuror),
#endif  // AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_FLOAT_EXEC_LIST, CmFloatExecList),
#if COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_FLOAT_HERITAGE, CmFloatHeritage),
#endif  // REGIONAL
	EV_COMMAND(CM_FLOAT_PAYMENTS, CmFloatPayments),
	EV_COMMAND(CM_FLOAT_REGISTER_ACT, CmFloatRegisterAct),
	EV_COMMAND(CM_QUERY_REGISTER_ACT, CmQueryRegisterAct),
	EV_COMMAND(CM_QUERY_MOVEMENT, CmQueryMovement),
	EV_COMMAND(CM_EXPIRE_SCHED, CmExpireSched),
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_QUERY_EXCHANGE, CmQueryExchange),
#endif  // AREAL || REGIONAL
	EV_COMMAND(CM_TIMED_SESSION, CmTimedSession),
	EV_COMMAND(CM_TIMED_LAWSUIT, CmTimedLawsuit),
	EV_COMMAND(CM_UNFINISHED_LAW, CmUnfinishedLaw),
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_QUERY_JUROR_LOAD, CmQueryJurorLoad),
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_QUERY_BANKRUPTCY, CmQueryBankruptcy),
	EV_COMMAND(CM_QUERY_INCONSIDE, CmQueryInconSide),
#endif  // AREAL
	EV_COMMAND(CM_QUERY_IMPORTANT, CmQueryImportant),
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_QUERY_CORRUPT, CmQueryCorrupt),
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_PERIOD_SESSION, CmPeriodSession),
	EV_COMMAND(CM_YOUNG_PERIOD, CmYoungPeriod),
	EV_COMMAND(CM_UNWRITTEN_ACT, CmUnwrittenAct),
	EV_COMMAND(CM_UNWRITTEN_SES, CmUnwrittenSes),
	EV_COMMAND(CM_PERIOD_OUTREG, CmPeriodOutReg),
	EV_COMMAND(CM_FLOAT_ARCHIVE, CmFloatArchive),
	EV_COMMAND(CM_FLOAT_DESTROY, CmFloatDestroy),
	EV_COMMAND(CM_QUERY_SUBPOENAS, CmQuerySubpoenas),
	EV_COMMAND(CM_QUERY_PAYMENTS, CmQueryPayments),
	EV_COMMAND(CM_QUERY_NIA_PROTOCOLS, CmQueryNIAProtocols),
	EV_COMMAND(CM_QUERY_JUDGES, CmQueryJudges),
	EV_COMMAND(CM_QUERY_ACTIVITY, CmQueryActivity),
#if COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_QUERY_P24A1, CmQueryP24a1),
	EV_COMMAND(CM_QUERY_BANCONSIST, CmQueryBanConSist),
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	EV_COMMAND(CM_QUERY_STAT_FIRST, CmQueryStatFirst),
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_QUERY_STAT_SECOND, CmQueryStatSecond),
#endif  // ADMIN || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
	EV_COMMAND(CM_QUERY_STAT_THIRD, CmQueryStatThird),
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_COMBINE_CORRUPT, CmCombineCorrupt),
#endif  // APPEAL || AREAL
	EV_COMMAND(CM_ADMIN_OBJECT_COUNT, CmAdminObjectCount),
	EV_COMMAND(CM_BOOK_DESC, CmBookDesc),
	EV_COMMAND(CM_BOOK_DESC_HTML, CmBookDescHtml),
	EV_COMMAND(CM_BOOK_DESC_DELIV, CmBookDescDeliv),
#if COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_BOOK_ZZDN, CmBookZZDN),
	EV_COMMAND(CM_BOOK_SUCCESS, CmBookSuccess),
#endif  // REGIONAL
	EV_COMMAND(CM_BOOK_ABC, CmBookAbc),
	EV_COMMAND(CM_BOOK_ABC_HTML, CmBookAbcHtml),
	EV_COMMAND(CM_BOOK_TIME, CmBookTime),
	EV_COMMAND(CM_BOOK_TIME_HTML, CmBookTimeHtml),
	EV_COMMAND(CM_BOOK_CLOSED, CmBookClosed),
	EV_COMMAND(CM_BOOK_CLOSED_HTML, CmBookClosedHtml),
	EV_COMMAND(CM_BOOK_INDOC, CmBookInDoc),
	EV_COMMAND(CM_BOOK_REQUEST, CmBookRequest),
	EV_COMMAND(CM_BOOK_INREG, CmBookInReg),
	EV_COMMAND(CM_BOOK_OUTREG, CmBookOutReg),
	EV_COMMAND(CM_BOOK_DECISION, CmBookDecision),
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_BOOK_39, CmBook39),
#endif  // APPEAL || ADMIN || AREAL
	EV_COMMAND(CM_BOOK_RECUSAL, CmBookRecusal),
#if COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_BOOK_DIVORCE, CmBookDivorce),
#endif  // REGIONAL
#if COURT_TYPE == COURT_ADMIN
	EV_COMMAND(CM_BOOK_251, CmBook251),
#endif  // ADMIN
	EV_COMMAND(CM_BOOK_EXEC_LIST, CmBookExecList),
	EV_COMMAND(CM_BOOK_APPEAL, CmBookAppeal),
#if COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_BOOK_EUHER, CmBookEuher),
#endif  // REGIONAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_BOOK_EXECUTION, CmBookExecution),
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_BOOK_634, CmBook634),
	EV_COMMAND(CM_BOOK_SLOWDOWN, CmBookSlowdown),
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_BOOK_PROVE, CmBookProve),
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_BOOK_PENALITIES, CmBookPenalities),
	EV_COMMAND(CM_BOOK_ARCHIVE, CmBookArchive),
	EV_COMMAND(CM_BOOK_EXTERNAL, CmBookExternal),
	EV_COMMAND(CM_BOOK_INTERNAL, CmBookInternal),
	EV_COMMAND(CM_BOOK_PRINT, CmBookPrint),
	EV_COMMAND(CM_ADMIN_USERS, CmAdminUsers),
	EV_COMMAND(CM_ADMIN_KNOWN_PERSONS, CmAdminKnownPersons),
	EV_COMMAND(CM_ADMIN_SUBJECTS, CmAdminSubjects),
	EV_COMMAND(CM_ADMIN_COMPOSITIONS, CmAdminCompositions),
	EV_COMMAND(CM_ADMIN_PENALITIES, CmAdminPenalities),
	EV_COMMAND(CM_ADMIN_REASONS, CmAdminReasons),
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_ADMIN_DANGERS, CmAdminDangers),
	EV_COMMAND(CM_ADMIN_ARTICLES, CmAdminArticles),
#endif  // AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_ADMIN_BANCONSISTS, CmAdminBanConSists),
#endif  // AREAL
	EV_COMMAND(CM_ADMIN_SLC_REASONS, CmAdminSlcReasons),
	EV_COMMAND(CM_ADMIN_ARCHIVE_INDEXES, CmAdminArchiveIndexes),
	EV_COMMAND(CM_ADMIN_BANK_ACCOUNTS, CmAdminBankAccounts),
	EV_COMMAND(CM_ADMIN_CURRENCES, CmAdminCurrences),
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_ADMIN_SESSION_PRICES, CmAdminSessionPrices),
#endif  // AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_ADMIN_SENDERS, CmAdminSenders),
	EV_COMMAND(CM_ADMIN_CALENDAR, CmAdminCalendar),
	EV_COMMAND(CM_ADMIN_HALLS, CmAdminHalls),
	EV_COMMAND(CM_ADMIN_CONFIG, CmAdminConfig),
	EV_COMMAND(CM_ADMIN_INFORMEDS, CmAdminInformeds),
	EV_COMMAND(CM_ADMIN_EPORT_QUERY, CmAdminEPortQuery),
	EV_COMMAND(CM_ADMIN_EPORT_LAWSUIT_ASSIGN, CmAdminEPortLawsuitAssign),
	EV_COMMAND(CM_ADMIN_EPORT_PERSON_ASSIGN, CmAdminEPortPersonAssign),
	EV_COMMAND(CM_ADMIN_EPORT_SEND, CmAdminEPortSend),
	EV_COMMAND(CM_ADMIN_EPORT_DELETE, CmAdminEPortDelete),
	EV_COMMAND(CM_ADMIN_COUNTRIES, CmAdminCountries),
	EV_COMMAND(CM_ADMIN_AREAS, CmAdminAreas),
	EV_COMMAND(CM_ADMIN_REGIONS, CmAdminRegions),
	EV_COMMAND(CM_ADMIN_UCLPS, CmAdminUCLPS),
	EV_COMMAND(CM_ADMIN_STREETS, CmAdminStreets),
	EV_COMMAND(CM_ADMIN_LOG, CmAdminLog),
	EV_COMMAND(CM_ADMIN_DELETE_INDOC, CmAdminDeleteInDoc),
	EV_COMMAND(CM_ADMIN_DELETE_REQUEST_RESOLUTION, CmAdminDeleteRequestResolution),
	EV_COMMAND(CM_ADMIN_DELETE_LAWSUIT, CmAdminDeleteLawsuit),
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_ADMIN_DELETE_EISPP_NMR, CmAdminDeleteEisppNmr),
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_ADMIN_DELETE_LAWSUIT_MOVEMENT, CmAdminDeleteLawsuitMovement),
	EV_COMMAND(CM_ADMIN_DELETE_LAWSUIT_EVENT, CmAdminDeleteLawsuitEvent),
	EV_COMMAND(CM_ADMIN_DELETE_INREG_EVENT, CmAdminDeleteInRegEvent),
	EV_COMMAND(CM_ADMIN_DELETE_OUTREG, CmAdminDeleteOutreg),
	EV_COMMAND(CM_ADMIN_DELETE_OUTREG_RETURN, CmAdminDeleteOutregReturn),
	EV_COMMAND(CM_ADMIN_DELETE_NIA_PROTOCOL, CmAdminDeleteNIAProtocol),
	EV_COMMAND(CM_ADMIN_DELETE_FOREIGN_ADDRESS, CmAdminDeleteForeignAddress),
	EV_COMMAND(CM_ADMIN_DELETE_OLD_CITIZEN, CmAdminDeleteOldCitizen),
	EV_COMMAND(CM_ADMIN_DELETE_MOVEMENT, CmAdminDeleteMovement),
	EV_COMMAND(CM_ADMIN_DELETE_ARCHIVE, CmAdminDeleteArchive),
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_ADMIN_DELETE_PROVE, CmAdminDeleteProve),
	EV_COMMAND(CM_ADMIN_DELETE_PROVE_ACTION, CmAdminDeleteProveAction),
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_ADMIN_DELETE_XFER, CmAdminDeleteXfer),
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_ADMIN_DELETE_JUROR_DISTRIB, CmAdminDeleteJurorDistrib),
#endif  // AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_ADMIN_DISCARD_CITIZENS, CmAdminDiscardCitizens),
	EV_COMMAND(CM_ADMIN_RENUMBER_REQUEST, CmAdminRenumberRequest),
	EV_COMMAND(CM_ADMIN_RENUMBER_LAWSUIT, CmAdminRenumberLawsuit),
	EV_COMMAND(CM_ADMIN_RENUMBER_UXN, CmAdminRenumberUXN),
	EV_COMMAND(CM_ADMIN_RENUMBER_SURROUND, CmAdminRenumberSurround),
	EV_COMMAND(CM_ADMIN_RENUMBER_INREG, CmAdminRenumberInreg),
	EV_COMMAND(CM_ADMIN_RENUMBER_STREET, CmAdminRenumberStreet),
	EV_COMMAND(CM_ADMIN_RENUMBER_SUBJECTS, CmAdminRenumberSubjects),
	EV_COMMAND(CM_ADMIN_JOIN_UXN, CmAdminJoinUXN),
	EV_COMMAND(CM_ADMIN_JOIN_STREET, CmAdminJoinStreet),
	EV_COMMAND(CM_ADMIN_JOIN_EQUAL_STREETS, CmAdminJoinEqualStreets),
	EV_COMMAND(CM_ADMIN_CHECKOUT_CITIZENS, CmAdminCheckoutCitizens),
	EV_COMMAND(CM_ADMIN_CHECKOUT_FIRMS, CmAdminCheckoutFirms),
	EV_COMMAND(CM_ADMIN_CHECKOUT_STREET_MAPS, CmAdminCheckoutStreetMaps),
	EV_COMMAND(CM_ADMIN_RESET_SESSION, CmAdminResetSession),
	EV_COMMAND(CM_ADMIN_REKIND_LAWSUIT, CmAdminRekindLawsuit),
	EV_COMMAND(CM_ADMIN_RECOMPOSE_LAWSUIT, CmAdminRecomposeLawsuit),
	EV_COMMAND(CM_ADMIN_COPY_SIDE_WINDS, CmAdminCopySideWinds),
	EV_COMMAND(CM_ADMIN_RENUMBER_SIDE_WIND, CmAdminRenumberSideWind),
	EV_COMMAND(CM_ADMIN_MOVE_JUDICIAL_ACT, CmAdminMoveJudicialAct),
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_ADMIN_EISPP_EDIT_LAWSUIT, CmAdminEisppEditLawsuit),
	EV_COMMAND(CM_ADMIN_EISPP_EDIT_FZLPNE, CmAdminEisppEditFzlPne),
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
	EV_COMMAND(CM_ADMIN_UPDATE_REQUESTS, CmAdminUpdateRequests),
	EV_COMMAND(CM_ADMIN_UPDATE_LONG_TEXTS, CmAdminUpdateLongTexts),
	EV_COMMAND(CM_ADMIN_UPDATE_LOG_KINDS, CmAdminUpdateLogKinds),
	EV_COMMAND(CM_ADMIN_UPDATE_EPORTALS, CmAdminUpdateEPortals),
#if COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_ADMIN_IMPORT_REREGISTERED, CmAdminImportReRegistered),
#endif  // AREAL
	EV_COMMAND(CM_ADMIN_IMPORT_PERSONAL_NO, CmAdminImportPersonalNo),
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_ADMIN_EXPORT_BANKRUPT, CmAdminExportBankrupt),
#endif  // APPEAL || AREAL
#if COURT_TYPE == COURT_ADMIN
	EV_COMMAND(CM_ADMIN_EXPORT_CSV, CmAdminExportCSV),
#endif  // ADMIN
	EV_COMMAND(CM_ADMIN_QUERY_WEB, CmAdminQueryWeb),
#if COURT_TYPE == COURT_AREAL
	EV_COMMAND(CM_ADMIN_QUERY_BANKRUPT, CmAdminQueryBankrupt),
	EV_COMMAND(CM_ADMIN_BANKRUPTS, CmAdminBankrupts),
#endif  // AREAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_ADMIN_QUERY_POLICE, CmAdminQueryPolice),
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
	EV_COMMAND(CM_ADMIN_QUERY_PROSEC, CmAdminQueryProsec),
#endif  // AREAL || REGIONAL
	EV_COMMAND(CM_ADMIN_FLOAT_PUBLICATION, CmAdminFloatPublication),
	EV_COMMAND(CM_ADMIN_FLOAT_ELECTRIC, CmAdminFloatElectric),
	EV_COMMAND(CM_ADMIN_QUERY_XFER_WEB, CmAdminQueryXferWeb),
	EV_COMMAND(CM_ADMIN_QUERY_YOUNG, CmAdminQueryYoung),
	EV_COMMAND(CM_ADMIN_RANDOM_ACT, CmAdminRandomAct),
	EV_COMMAND(CM_HELP_ABOUT, CmHelpAbout),
	EV_COMMAND(CM_HELP_REFRESH, CmHelpRefresh),
	EV_COMMAND(CM_HELP_LOGIN, CmHelpLogin),
	EV_COMMAND(CM_HELP_TESTING, CmHelpTesting),
//{{TAppNameAppRSP_TBL_END}}
END_RESPONSE_TABLE;

//{{TAppNameApp Implementation}}

TAppNameApp::TAppNameApp(const char far *name)
	: TApplication(name), parent(NULL)
{
}

TAppNameApp::~TAppNameApp()
{
}

void TAppNameApp::InitMainWindow()
{
	nCmdShow = SW_MAXIMIZE;
	AppMainWindow = parent = new TAppNameFrame(NULL, GetName(), 0, false);

	parent->SetFlag(wfShrinkToClient);
	parent->SetStyle(WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPSIBLINGS);

	AppNameIcon = LoadIcon(MAKEINTRESOURCE(IDI_APPNAMESDI));
	parent->SetIcon(this, IDI_APPNAMESDI);
	//parent->SetIconSm(this, IDI_APPNAMESDI);

	int menuId = COURT_MINI ? IDM_MINIMAL : IDM_APPNAME;
	parent->AssignMenu(menuId);
	parent->Attr.AccelTable = menuId;

	SetMainWindow(parent);
	parent->SetMenuDescr(TMenuDescr(menuId));
	EnableCtl3d(true);
}

void TAppNameApp::InitInstance()
{
	TApplication::InitInstance();

	if (COURT_MINI)	// 2011:130 LPR: FIX
		CHROME_EFILE_MENUS = 0;

	HMENU hMenu = parent->GetMenu();

	if (hMenu)
	{
		if (COURT_MINI)
		{
			if (COURT_FLAT)
				AppendMenu(hMenu, MF_STRING, CM_QUERY_UCN, "Справка за лице");

			if (COURT_WRIT)
				AppendMenu(hMenu, MF_STRING, CM_ELECTRIC_BLUE, "Електронна папка");

			if (COURT_EXAR)
				AppendMenu(hMenu, MF_STRING, CM_QUERY_CONNECT, "Справка по дело на друга институция/инстанция");
		}
		else	// 2016:160 don't edit/delete items missing in MINI
		{
			if (!COURT_WRIT)
			{
				DeleteMenu(hMenu, CM_DELIV_EXTERNAL, MF_BYCOMMAND);
				DeleteMenu(hMenu, CM_DELIV_INTERNAL, MF_BYCOMMAND);
				DeleteMenu(hMenu, 5, MF_BYPOSITION);
				DeleteMenu(hMenu, CM_FLOAT_ANNOUNCE, MF_BYCOMMAND);
				DeleteMenu(hMenu, CM_BOOK_DESC_DELIV, MF_BYCOMMAND);
				DeleteMenu(hMenu, CM_BOOK_EXTERNAL, MF_BYCOMMAND);
				DeleteMenu(hMenu, CM_BOOK_INTERNAL, MF_BYCOMMAND);
			}
			// 2011:122
		#if COURT_TYPE == COURT_AREAL
			if (COURT_UCLP != BUILDIN_UCLP)
				DeleteMenu(hMenu, CM_ADMIN_BANKRUPTS, MF_BYCOMMAND);
		#endif  // AREAL
			// 2011:161
			if (COURT_UCLP != BUILDIN_UCLP)
				DeleteMenu(hMenu, CM_ADMIN_INFORMEDS, MF_BYCOMMAND);
			// 2012:180
			if (!COURT_LOCK)
				ModifyMenu(hMenu, CM_PERIOD_SESSION, MF_BYCOMMAND, CM_PERIOD_SESSION, "За дела/заседания по страна");
			// 2015:260 LRQ: disable only
		//#if COURT_TYPE == COURT_ADMIN
		//	if (!COURT_CSVE)
		//		DeleteMenu(hMenu, CM_ADMIN_EXPORT_CSV, MF_BYCOMMAND);
		//#endif  // COURT_ADMIN
		#if TESTVER
		#else  // TESTVER
			if (COURT_CODE != COURT_SOFGS)
			{
				DeleteMenu(hMenu, CM_JUDGE_REGISTER_ACT, MF_BYCOMMAND);
				DeleteMenu(hMenu, CM_FLOAT_REGISTER_ACT, MF_BYCOMMAND);
				DeleteMenu(hMenu, CM_QUERY_REGISTER_ACT, MF_BYCOMMAND);
			}
		#endif  // TESTVER
		#if COURT_TYPE == COURT_AREAL
		#if TESTVER
			if (COURT_CODE != COURT_SOFGS)
		#else  // TESTVER
			if (COURT_CODE != COURT_SOFGS || COURT_EXAR)
		#endif  // TESTVER
			{
				DeleteMenu(hMenu, CM_CLERK_EXTERN_SESSIONS, MF_BYCOMMAND);
				DeleteMenu(hMenu, CM_SECR_EXTERN_SESSIONS, MF_BYCOMMAND);
				DeleteMenu(hMenu, CM_ARCH_IMPORT_ARCHIVES, MF_BYCOMMAND);
				DeleteMenu(hMenu, CM_ARCH_EXPORT_ARCHIVED, MF_BYCOMMAND);
			}

		#if TESTVER
			if (COURT_CODE != COURT_SOFGS)
		#else  // TESTVER
			if (COURT_CODE != COURT_SOFGS || !COURT_EXAR)
		#endif  // TESTVER
			{
				DeleteMenu(hMenu, CM_CLERK_EXPORT_ARCHIVES, MF_BYCOMMAND);
				DeleteMenu(hMenu, CM_SECR_EXPORT_ARCHIVES, MF_BYCOMMAND);
				DeleteMenu(hMenu, CM_CLERK_IMPORT_ARCHIVED, MF_BYCOMMAND);
				DeleteMenu(hMenu, CM_SECR_IMPORT_ARCHIVED, MF_BYCOMMAND);
			}
		#endif  // AREAL
		#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
			if (!can_surrouendoc())
				DeleteMenu(hMenu, CM_REG_IN_SURROUENDOCS, MF_BYCOMMAND);
		#endif  // APPEAL || AREAL
			// 2017:136
			if (COURT_CODE != COURT_VARAS)
				DeleteMenu(hMenu, CM_ADMIN_MOVE_JUDICIAL_ACT, MF_BYCOMMAND);
		}
	}

	TRect windowRect;
	TRect clientRect;

	parent->GetWindowRect(windowRect);
	parent->GetClientRect(clientRect);
	// 2007:127 LPR: calculate rect for SHOW*MAXIMIZED = 2
	AdjustedRect.left = (windowRect.left + clientRect.left) / 2;
	AdjustedRect.right = (windowRect.right + clientRect.right) / 2;
	AdjustedRect.bottom = windowRect.bottom - (clientRect.top - windowRect.top);
	// extend to avoid a XP (only?) bug where drawing
	// a main menu option damages the window frame
	AdjustedRect.top = (windowRect.bottom -= clientRect.bottom);
	parent->ShowWindow(SW_RESTORE);
	AppMainBottom = windowRect.bottom;

	if (MainShowWindow == SW_MAXIMIZE)
	{
		parent->MoveWindow(windowRect, false);
		parent->ShowWindow(SW_MAXIMIZE);
	}
	else
		parent->MoveWindow(windowRect, true);

	// 2015:296 SHOWUP-s moved here from CacMain(), main window is required for EV_COMMAND_ENABLE

	if (SCHED_SHOWUP)
	{
		TExpireSchedGroup group(SCHED_SHOWUP == 2 ? Default->colleges : QUERY_COLLEGES);

		if (strchr(POST_JUDGES, Default->post))
		{
			// 2010:274 FIX: current judge; 2012:059 LRQ: expire sched dialog like
			UserAliasGroup->LoadData(&group.judges, POST_VISIBLE_JUDGES, USER_INTER);
		}

		query_sched(parent, &group);
	}

	if (SHOWUP_IMPORTANT)
	{
		TFloatImportantGroup group;
		strcpy(group.types, Default->colleges);
		query_important(parent, &group);
	}

	if (SHOWUP_REGISTER_ACT && command_enabled(CM_JUDGE_REGISTER_ACT, Default->menus))
		register_acts(parent);
}

bool TAppNameApp::IdleAction(long idleCount)
{
	if (PING_DATABASE && AppIdleCount >= PING_DATABASE)
	{
		CDateTime::SyncServer(SYNC_EXACT);
		AppIdleCount = 0;
	}

	DWORD ticks = GetTickCount();

	if (POSTERM_REINITIALIZE && ticks - TPosTerm::initicks >= POSTERM_REINITIALIZE * 60000)
	{
	#if TESTVER
		log(LOG_POSTERM, CM_REG_PAYMENTS, "Преинициализация на портовете");
	#endif  // TESTVER
		TPosTerm::Shutdown();
		TPosTerm::Initialize();
	}

	return TApplication::IdleAction(idleCount);
}

// Menu Help About AppName command
//

#if COURT_TYPE == COURT_APPEAL
void TAppNameApp::CmRegNewPunishment() { new_request(parent, KIND_PUNISHMENT_INDOCS, TYPE_PUNISHMENT_LAW); }
void TAppNameApp::CmRegNewCitizen() { new_request(parent, KIND_CITIZEN_INDOCS, TYPE_CITIZEN_LAW); }
void TAppNameApp::CmRegNewReCitizen() { new_request(parent, KIND_RECITIZEN_INDOCS, TYPE_RECITIZEN_LAW); }
void TAppNameApp::CmRegNewTrade() { new_request(parent, KIND_TRADE_INDOCS, TYPE_TRADE_LAW); }
void TAppNameApp::CmRegNewReTrade() { new_request(parent, KIND_RETRADE_INDOCS, TYPE_RETRADE_LAW); }
#endif  // APPEAL

#if COURT_TYPE == COURT_ADMIN
void TAppNameApp::CmRegNewAdmin() { new_request(parent, KIND_ADMIN_INDOCS, TYPE_ADMIN_LAW); }
void TAppNameApp::CmRegNewTricky() { new_request(parent, KIND_TRICKY_INDOCS, TYPE_TRICKY_LAW); }
void TAppNameApp::CmRegNewCitizen() { new_request(parent, KIND_CITIZEN_INDOCS, TYPE_CITIZEN_LAW); }
#endif  // ADMIN

#if COURT_TYPE == COURT_AREAL
void TAppNameApp::CmRegNewPunishment() { new_request(parent, KIND_PUNISHMENT_INDOCS, TYPE_PUNISHMENT_LAW); }
void TAppNameApp::CmRegNewCitizen() { new_request(parent, KIND_CITIZEN_INDOCS, TYPE_CITIZEN_LAW); }
void TAppNameApp::CmRegNewTrade() { new_request(parent, KIND_TRADE_INDOCS, TYPE_TRADE_LAW); }
void TAppNameApp::CmRegNewAdmin() { new_request(parent, KIND_ADMIN_INDOCS, TYPE_ADMIN_LAW); }
void TAppNameApp::CmRegNewFirm() { new_request(parent, KIND_FIRM_INDOCS, TYPE_FIRM_LAW); }
#endif  // AREAL

#if COURT_TYPE == COURT_MILITARY
void TAppNameApp::CmRegNewPunishment()
{
	new_request(parent, CourtType == COURT_MILITARY ? KIND_1ST_PUNISHMENT_INDOCS : KIND_2ND_PUNISHMENT_INDOCS,
		TYPE_PUNISHMENT_LAW);
}
#endif  // MILITARY

#if COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmRegNewMarriage() { new_request(parent, KIND_MARRIAGE_INDOCS, TYPE_MARRIAGE_LAW); }
void TAppNameApp::CmRegNewCitizen() { new_request(parent, KIND_CITIZEN_INDOCS, TYPE_CITIZEN_LAW); }
void TAppNameApp::CmRegNewPunishment() { new_request(parent, KIND_PUNISHMENT_INDOCS, TYPE_PUNISHMENT_LAW); }
void TAppNameApp::CmRegNewAdmin() { new_request(parent, KIND_ADMIN_INDOCS, TYPE_ADMIN_LAW); }
#endif  // REGIONAL

void TAppNameApp::CmRegEdit()
{
	TFoundGroup ucnGroup;

	if (get_ucn(parent, &ucnGroup, UCN_SIDES))
	{
		TRequest request;

		// 2003:308 BRW: removed INVOLVEMENT_STANDARD
		if (select_container(parent, &request, &ucnGroup, TYPE_REQUESTS, NULL, NULL, request_editable))
			edit_request(parent, &request);
	}
}

void TAppNameApp::CmRegEditRequest()
{
	TRequest request;

	if (get_group(parent, &request, TYPE_REQUESTS, NULL, request_editable))
		edit_request(parent, &request);
}

void TAppNameApp::CmRegNewSurround()
{
	// 2009:134 LPR/LRQ: brand-new surrounding
	TRCDKeyGroup *group;

	SET_REPEATER(CM_REG_NEW_SURROUND, REPEAT_SURROUNDS);
	while ((group = get_group(parent, NULL, TYPE_SURROUNDABLES, KIND_VISIBLE_SESSIONS_ENDOCS, surroundable,
		GET_GROUP_BASIC)) != NULL)
	{
		TVirtPtr VP(group);
		new_surround(parent, TYPECHECK_PTR_CAST(TRCDKeyContainer, group));
		CHECK_REPEATER();
	}
}

void TAppNameApp::CmRegEditSurround()
{
	TSurround surround;

	SET_REPEATER(CM_REG_EDIT_SURROUND, REPEAT_SURROUNDS1);
	while (get_group(parent, &surround, TYPE_SELECT_SURROUNDS, NULL, match_null))
	{
		edit_surround(parent, &surround, NULL);
		CHECK_REPEATER();
	}
}

void TAppNameApp::CmRegInNew()
{
	TInReg inReg;
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_LAWSUITS, NULL, match_null, GET_GROUP_EMPTY);

	if (group)
	{
		TVirtPtr VP(group);

		if (group->key.type != TYPE_EMPTY)
			new_inreg(parent, &inReg, NULL, (TLawsuit *) group);
		else
		{
			TLawsuit lawsuit;
			lawsuit.key.type = '\0';
			new_inreg(parent, &inReg, NULL, &lawsuit);
		}
	}
}

void TAppNameApp::CmRegInNewX()
{
	TInReg inReg;
	new_inreg_x(parent, &inReg);
}

void TAppNameApp::CmRegInNewEPortal()
{
	TInReg inReg;

	inReg.kind = KIND_EPORTAL_REQUEST;
	new_inreg_eportal(parent, &inReg);
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
void TAppNameApp::CmRegInSurrouendocs()
{
	TSession session;
	char sesKinds[SIZE_OF_KINDS];

	session.key.type = TYPE_TRADE_LAW;
	strcpy(sesKinds, KIND_CLOSED_SESSIONS);

	for (const char *type = TYPE_TRADEALIKE_LAWS; *type; type++)
		strcat_nx(sesKinds, type2ses_kinds(*type));

	while (get_group(parent, &session, TYPE_TRADEALIKE_LAWS, sesKinds, RESULT_SURROUENDOCS))
	{
		if (appeal_surrouendocs(parent, &session, PrimaryGroup(TLawsuit)))
			break;
		// considered rare
		error("Към заседанието няма вписани актове по възражения.");
	}
}
#endif  // APPEAL || AREAL

void TAppNameApp::CmRegInEdit()
{
	TInReg inReg;

	// 2007:123 LRQ: edit all inregs: UNDEPARTEDS -> NULL
	if (get_group(parent, &inReg, TYPE_INREGS, NULL, inreg_editable))
		edit_inreg(parent, &inReg);
}

void TAppNameApp::CmRegNewInRegEvent()
{
	TInReg inReg;

	// 2009:231 TRQ: UNDEPARTEDS -> EVENTABLES
	if (get_group(parent, &inReg, TYPE_INREGS, NULL, RESULT_EVENTABLES))
	{
		// 2001:318 KIND_IN_RESOLUTABLES -> allow resolutions for all
		TInRegEvent inRegEvent;
		new_inreg_event(parent, &inReg);
	}
}

void TAppNameApp::CmRegEditInRegEvent()
{
	TInRegEvent inRegEvent;

	// 2010:260 LPR/LRQ: FIX: UNDEPARTEDS -> INREG_RESOLUTIONS
	// 2016:251 LPR: INREG_RESOLUTIONS -> event_editable
	if (get_group(parent, &inRegEvent, TYPE_INREGS, KIND_VISIBLE_EVENTS, event_editable))
		edit_inreg_event(parent, &inRegEvent);
}

void TAppNameApp::CmRegOutAppeal()
{
	TInReg inReg;
	TOutReg outReg;

	// 2005:049 LPR: BUGFIX: reject letters and other documents
	if (get_group(parent, &inReg, TYPE_INREGS, KIND_APPEAL_IN_DOCS, RESULT_SENDABLES))
		send_appeal(parent, &outReg, &inReg);
}

void TAppNameApp::CmRegOutJuriConf()
{
	send_juriconf(parent);
}

void TAppNameApp::CmRegOutConfHigh()
{
	send_confhigh(parent);
}

static const char *sendable(TRCDKeyGroup *group, char status)
{
	if (group->key.type == TYPE_INREG)
	{
		if (!strchr(RESULT_SENDABLES, status))
			return wrong_status;
		if (!strchr(KIND_TEXTUAL_IN_DOCS, ((TRCDKeyContainer *) group)->kind))
			return "Документът от входящ регистър трябва да бъде писмо или друг документ.";
	}

	return NULL;
}

void TAppNameApp::CmRegOutSendOther()
{
	TOutReg outReg;
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_SEND_OTHERS, NULL, sendable, GET_GROUP_EMPTY);

	if (group)
	{
		TVirtPtr VP(group);
		send_other(parent, &outReg, group);
	}
}

static const char *returnable(TRCDKeyGroup *group)
{
	char result;
	const char *results;
	const char *wrong = wrong_status;

	if (group->key.type == TYPE_REQUEST)
	{
		result = ((TRequestResolution *) group)->resolution;
		results = C2S[RESOLUTION_REJECTED];
	}
	else
	{
		TDecision *decision = (TDecision *) group;
		TSession session;

		session.key = decision->key;
		session.kind = decision->sessionKind;
		session.date = decision->date;

		if (!session.Try("F_RESULT"))
			return "T_SESSION: липсващи в базата, непопълнени или блокирани от друг потребител данни";

		result = session.result;
		results = RESULT_COMPLETIZEDS;
		wrong = "Документът е от заседание което не е с финализиращ резултат";
	}

	return strchr(results, result) ? NULL : wrong;
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
static const char *returnable_lower(TRCDKeyGroup *group, char)
{
	const char *s = NULL;

	if ((XFER_ACTIVE & XFER_REQLAWS) || EISPP_ACTIVE)
	{
		TConnect connect;

		if (!connect.TryBy(group, CONNECT_TYPE_LOWER_INSTANCE))
			s = "Няма вписани данни за първоинстанционен съд";
	#if COURT_TYPE == COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
		else
			s = connect.Returnable(RETURNABLE_FULL);
	#endif  // ADMIN || AREAL || MILITARY
	}

	return s ? s : returnable(group);
}

void TAppNameApp::CmRegOutReturnLower()
{
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_ORIGINS, KIND_VISIBLE_RETURNABLES, returnable_lower);

	if (group)
	{
		TVirtPtr VP(group);
		return_out(parent, (TRCDKeyContainer *) group, PrimaryGroup(TOriginContainer), true);
	}
}
#endif  // APPEAL || ADMIN || AREAL || MILITARY

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
static const char *returnable_prosec(TRCDKeyGroup *group, char)
{
	if (EISPP_ACTIVE)
	{
		TConnect connect;

		if (!connect.TryBy(group, CONNECT_TYPE_PROSECS))
			return "Към документа/делото няма вписани данни за изпращаща прокуратура";

		TSender sender;

		sender.ucnType = connect.ucnType;
		strcpy(sender.ucn, connect.ucn);
		if (!sender.Get())
			return "Липсващ адресант";

		if (!sender.eispp)
			return "Изпращатата прокуратура няма код по ЕИСПП";
	}

	return returnable(group);
}

void TAppNameApp::CmRegOutReturnProsec()
{
	const char prosecables[3] = { TYPE_PUNISHMENT_LAW, TYPE_REQUEST, '\0' };
	TRCDKeyGroup *const group = get_group(parent, NULL, prosecables, KIND_VISIBLE_RETURNABLES, returnable_prosec);

	if (group)
	{
		TVirtPtr VP(group);
		return_out(parent, (TRCDKeyContainer *) group, PrimaryGroup(TOriginContainer), false);
	}
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

void TAppNameApp::CmRegOutEdit()
{
	TOutReg outReg;

	if (get_group(parent, &outReg, TYPE_OUTREGS, NULL))
		edit_outreg(parent, &outReg);
}

void TAppNameApp::CmRegOutReceive()
{
	TOutReg outReg;

	// 2009:183 LPR/LRQ: OUT_REG_APPS, receivable
	// 2009:286 LPR/LRQ: restored receive anything
	if (get_group(parent, &outReg, TYPE_OUTREGS, NULL))
	{
		char status = outReg.Status();

		Result->Seek(status);
		if (status == RESULT_IOREG_SEND ||
			ask("Документът е със статус %s.\nЖелаете ли да редактирате данните?", Result->Seek(status)))
		{
			receive_out(parent, &outReg);
		}
	}
}

void TAppNameApp::CmRegPayments()
{
	payments(parent);
}

static void new_lawsuit(TWindow *parent, const char *kinds)
{
	TFoundGroup ucnGroup;
	TRequest request;

	SET_REPEATER(CM_CLERK_NEW_LAWSUIT, REPEAT_LAWSUITS);
	while (get_ucn(parent, &ucnGroup, UCN_SIDES) &&
		select_container(parent, &request, &ucnGroup, TYPE_REQUESTS, kinds, NULL, request_lawable))
	{
		new_lawsuit(parent, &request);
		CHECK_REPEATER();
	}
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmClerkNew()
{
	new_lawsuit(parent, KIND_INDOCS);
}
#endif  // APPEAL || REGIONAL

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_AREAL
void TAppNameApp::CmClerkNew1st()
{
	new_lawsuit(parent, KIND_1ST_INDOCS);
}

void TAppNameApp::CmClerkNew2nd()
{
	new_lawsuit(parent, KIND_2ND_INDOCS);
}
#endif  // ADMIN || MILITARY || AREAL

void TAppNameApp::CmClerkNewLawsuit()
{
	TRequest request;

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
	if (get_group(parent, &request, TYPE_REQUESTS, NULL, request_lawable))
		new_lawsuit(parent, &request);
#endif  // APPEAL || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_ADMIN
	if (get_create_request(parent, &request))
		new_lawsuit(parent, &request);
#endif  // ADMIN
}

#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmClerkNewJurisd()
{
	new_virtual(parent, CONNECT_TYPE_JURISDICTIONS);
}
#endif  // ADMIN || AREAL || MILITARY || REGIONAL

void TAppNameApp::CmClerkNewReturned()
{
	new_virtual(parent, CONNECT_TYPE_RETURNEDS);
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmClerkNewReReturned()
{
	new_virtual(parent, CONNECT_TYPE_RERETURNEDS);
}

void TAppNameApp::CmClerkNewReNewed()
{
	new_virtual(parent, CONNECT_TYPE_RENEWEDS);
}
#endif  // AREAL || MILITARY || REGIONAL

void TAppNameApp::CmClerkEditLawsuit()
{
	TLawsuit lawsuit;

	SET_REPEATER(CM_CLERK_EDIT_LAWSUIT, REPEAT_LAWSUITS1);
	while (get_group(parent, &lawsuit, TYPE_LAWSUITS, NULL, lawsuit_editable))
	{
		edit_lawsuit(parent, &lawsuit);
		CHECK_REPEATER();
	}
}

#if COURT_TYPE == COURT_AREAL
void TAppNameApp::CmClerkEnterReregisterDate()
{
	TLawsuit lawsuit;
	TDamn damn;

	if (get_group(parent, &lawsuit, TYPE_FIRM_LAWS, NULL))
		if (find_damn(&lawsuit, &damn))
			enter_reregister_date(parent, &damn, &lawsuit);
}
#endif  // AREAL

void TAppNameApp::CmClerkNewResolutionUCN()
{
	TFoundGroup ucnGroup;

	if (get_ucn(parent, &ucnGroup, UCN_SIDES))
	{
		TRequest request;

		// 2003:308 BRW: removed INVOLVEMENT_STANDARD
		if (select_container(parent, &request, &ucnGroup, TYPE_REQUESTS, NULL, RESOLUTION_RESOLUTABLES, match_null))
			new_request_resolution(parent, &request);
	}
}

void TAppNameApp::CmClerkNewRequestResolution()
{
	TRequest request;

	if (get_group(parent, &request, TYPE_REQUESTS, NULL, RESOLUTION_RESOLUTABLES))
		new_request_resolution(parent, &request);
}

void TAppNameApp::CmClerkEditResolutionUCN()
{
	TFoundGroup ucnGroup;

	if (get_ucn(parent, &ucnGroup, UCN_SIDES))
	{
		TRequestResolution resolution;

		// 2003:308 BRW: removed INVOLVEMENT_STANDARD
		if (select_container(parent, &resolution, &ucnGroup, TYPE_REQUESTS, KIND_REQUEST_RESOLUTIONS, RESOLUTION_RESOLUTIONS, match_null))
			edit_request_resolution(parent, &resolution);
	}
}

void TAppNameApp::CmClerkEditRequestResolution()
{
	TRequestResolution resolution;

	if (get_group(parent, &resolution, TYPE_REQUESTS, KIND_REQUEST_RESOLUTIONS, RESOLUTION_RESOLUTIONS))
		edit_request_resolution(parent, &resolution);
}

void TAppNameApp::CmClerkClosedResult()
{
	if (COURT_CODE == COURT_SOFGS)
	{
		CDate sessionDate;
		TLawsuit *const lawsuit = TYPECHECK_PTR_CAST(TLawsuit, get_closed_lawsuit(parent, &sessionDate, NULL, false));
		TVirtPtr VP(lawsuit);

		if (lawsuit)
			closed_result(parent, lawsuit, &sessionDate);
	}
	else
	{
		TLawsuit lawsuit;

		if (get_group(parent, &lawsuit, TYPE_LAWSUITS, NULL, match_null))
			closed_result(parent, &lawsuit, NULL);
	}
}

void TAppNameApp::CmClerkEditClosed()
{
	TSession session;

	if (get_group(parent, &session, TYPE_LAWSUITS, KIND_VISIBLE_CLOSEDS))
		edit_closed(parent, &session, PrimaryGroup(TLawsuit));
}

void TAppNameApp::CmClerkSet()
{
	TLawsuit lawsuit;

	SET_REPEATER(CM_CLERK_SET, REPEAT_FIXES);
	while (get_group(parent, &lawsuit, TYPE_LAWSUITS, NULL, lawsuit_settable))
	{
		TSession session;
		set_session(parent, &session, &lawsuit, NULL, NULL);
		CHECK_REPEATER();
	}
}

void TAppNameApp::CmClerkSetLoad()
{
	CmJudgeSetLoad();
}

void TAppNameApp::CmClerkEditSet()
{
	TSession session;

	if (get_group(parent, &session, TYPE_LAWSUITS, KIND_OPEN_SESSIONS, RESULT_FIXEDS))
		edit_set_session(parent, &session, PrimaryGroup(TLawsuit));
}

void TAppNameApp::CmClerkReset()
{
	TSession source;
	reset_session(parent, &source, NULL);
}

void TAppNameApp::CmClerkUnset()
{
	TSession session;
	unset_session(parent, &session, NULL);
}

void TAppNameApp::CmClerkFixedList()
{
	// 2009:027 new style
	session_list(parent);
}

void TAppNameApp::CmClerkPrintMessages()
{
	TRCDKeyGroup *group;

	SET_REPEATER(CM_CLERK_PRINT_MESSAGES, REPEAT_MESSAGES);
	while ((group = get_group(parent, NULL, TYPE_LAWSUITS, KIND_VISIBLE_SESSIONS_ENDOCS, match_null)) != NULL)
	{
		TVirtPtr VP(group);
		TIndexContainer *container = TYPECHECK_PTR_CAST(TIndexContainer, group);

		if (strchr(KIND_SESSIONS, container->kind))
			edit_sides(parent, (TSession *) container, PrimaryGroup(TLawsuit));
		else
			edit_sides(parent, (TDecision *) container, PrimaryGroup(TLawsuit), SIDE_MODE_EDIT);

		CHECK_REPEATER();
	}
}

void TAppNameApp::CmClerkNewJudicialAct()
{
	CmSecrNewJudicialAct();
}

void TAppNameApp::CmClerkEditJudicialAct()
{
	CmSecrEditJudicialAct();
}

void TAppNameApp::CmClerkForceDate()
{
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_FORCABLE_LAWS, KIND_FORCABLES, forcable);

	if (group)
	{
		TVirtPtr VP(group);
		TRCDKeyContainer *container = (TRCDKeyContainer *) group;

		if (strchr(KIND_ENDOCS, container->kind))
			force_decision(parent, (TDecision *) group, PrimaryGroup(TLawsuit));
		else
			force_session(parent, (TSession *) group, PrimaryGroup(TLawsuit));
	}
}

void TAppNameApp::CmClerkInOutMovement()
{
	TRCDKeyGroup *group;

	SET_REPEATER(CM_CLERK_IN_OUT_MOVEMENT, REPEAT_MOVEMENTS);
	while ((group = get_group(parent, NULL, TYPE_MOVABLES, NULL, match_null, GET_GROUP_BASIC)) != NULL)
	{
		TVirtPtr VP(group);
		movement(parent, group);
		CHECK_REPEATER();
	}
}

void TAppNameApp::CmClerkEditScheds()
{
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_SCHEDULABLES, KIND_VISIBLE_SCHEDULABLES, schedulable);

	if (group)
	{
		TVirtPtr VP(group);
		edit_scheds(parent, (TRCDKeyContainer *) group);
	}
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmClerkNewDistribJurors()
{
	TSession *const session = (TSession *) get_juror_distrib(parent, false, NULL, false);
	TVirtPtr VP(session);

	if (session)
		new_distrib_jurors(parent, session, PrimaryGroup(TLawsuit));
}

void TAppNameApp::CmClerkEditDistribJurors()
{
	TSession *const session = (TSession *) get_juror_distrib(parent, true, NULL, false);
	TVirtPtr VP(session);

	if (session)
		edit_distrib_jurors(parent, session, PrimaryGroup(TLawsuit));
}
#endif  // AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_AREAL
void TAppNameApp::CmClerkExternSessions()
{
	extern_sessions(parent);
}

void TAppNameApp::CmClerkExportArchives()
{
	export_archives(parent);
}

void TAppNameApp::CmClerkImportArchived()
{
	import_archived(parent);
}
#endif  // AREAL

void TAppNameApp::CmClerkLegalAid()
{
	legalaid_import(parent);
}

void TAppNameApp::CmClerkNIAProtocol()
{
	nia_prepare(parent);
}

void TAppNameApp::CmClerkNIACollect()
{
	nia_collect(parent);
}

void TAppNameApp::CmClerkBarcodeScan()
{
	barcode_scan(parent);
}

void TAppNameApp::CmClerkOutMail()
{
	print_mail_list(parent);
}

void TAppNameApp::CmClerkUnfilteredActs()
{
	unfiltered_acts(parent);
}

void TAppNameApp::CmSecrOpenResult()
{
	TSession session;

	SET_REPEATER(CM_SECR_OPEN_RESULT, REPEAT_RESULTS);
	while (get_group(parent, &session, TYPE_LAWSUITS, KIND_OPEN_SESSIONS, not_aborted_present))
	{
		open_result(parent, &session, PrimaryGroup(TLawsuit));
		CHECK_REPEATER();
	}
}

void TAppNameApp::CmSecrClosedResult()
{
	CmClerkClosedResult();
}

void TAppNameApp::CmSecrEditClosed()
{
	CmClerkEditClosed();
}

void TAppNameApp::CmSecrSet()
{
	CmClerkSet();
}

void TAppNameApp::CmSecrSetLoad()
{
	CmJudgeSetLoad();
}

void TAppNameApp::CmSecrEditSet()
{
	CmClerkEditSet();
}

void TAppNameApp::CmSecrReset()
{
	CmClerkReset();
}

void TAppNameApp::CmSecrUnset()
{
	CmClerkUnset();	// 2005:140 cp CmClerkUnset() -> call it
}

void TAppNameApp::CmSecrFixedList()
{
	CmClerkFixedList();
}

void TAppNameApp::CmSecrPrintMessages()
{
	CmClerkPrintMessages();
}

void TAppNameApp::CmSecrNewJudicialAct()
{
	TSession session;

	if (get_group(parent, &session, TYPE_LAWSUITS, KIND_SESSIONS, RESULT_SURROUENTERABLES))
		enter_act(parent, &session, PrimaryGroup(TLawsuit));
}

void TAppNameApp::CmSecrEditJudicialAct()
{
	TDecision decision;

	if (get_group(parent, &decision, TYPE_LAWSUITS, KIND_ENDOCS))
		edit_act(parent, &decision, PrimaryGroup(TLawsuit));
}

void TAppNameApp::CmSecrForceDate()
{
	CmClerkForceDate();
}

void TAppNameApp::CmSecrInOutMovement()
{
	CmClerkInOutMovement();
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmSecrNewDistribJurors()
{
	CmClerkNewDistribJurors();
}

void TAppNameApp::CmSecrEditDistribJurors()
{
	CmClerkEditDistribJurors();
}
#endif  // AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_AREAL
void TAppNameApp::CmSecrExternSessions()
{
	CmClerkExternSessions();
}

void TAppNameApp::CmSecrExportArchives()
{
	CmClerkExportArchives();
}

void TAppNameApp::CmSecrImportArchived()
{
	CmClerkImportArchived();
}
#endif  // AREAL

void TAppNameApp::CmSecrLegalAid()
{
	CmClerkLegalAid();
}

void TAppNameApp::CmSecrNIAProtocol()
{
	CmClerkNIAProtocol();
}

void TAppNameApp::CmSecrNIACollect()
{
	CmClerkNIACollect();
}

void TAppNameApp::CmSecrUnfilteredActs()
{
	CmClerkUnfilteredActs();
}

void TAppNameApp::CmJudgeNewJudicialAct()
{
	CmSecrNewJudicialAct();
}

void TAppNameApp::CmJudgeEditJudicialAct()
{
	CmSecrEditJudicialAct();
}

void TAppNameApp::CmJudgeClosedResult()
{
	CmClerkClosedResult();
}

void TAppNameApp::CmJudgeEditClosed()
{
	CmClerkEditClosed();
}

void TAppNameApp::CmJudgeSetLoad()
{
	load_sessions(parent);
}

void TAppNameApp::CmJudgeFixedList()
{
	CmClerkFixedList();
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmJudgeNewDistribJurors()
{
	CmClerkNewDistribJurors();
}

void TAppNameApp::CmJudgeEditDistribJurors()
{
	CmClerkEditDistribJurors();
}
#endif  // AREAL || MILITARY || REGIONAL

void TAppNameApp::CmJudgeUnfilteredActs()
{
	unfiltered_acts(parent);
}

void TAppNameApp::CmJudgeRegisterAct()
{
	register_acts(parent);
}

void TAppNameApp::CmArchNewArchive()
{
	TExternContainer *lawsuit;

	SET_REPEATER(CM_ARCH_NEW_ARCHIVE, REPEAT_ARCHIVES);
	while ((lawsuit = get_extern_container(parent, extern_archable, NULL, false, GET_GROUP_ARCHFN)) != NULL)
	{
		TVirtPtr VP(lawsuit);
		new_archive(parent, lawsuit);
		CHECK_REPEATER();
	}
}

static const char *archived(TRCDKeyGroup *group, char)
{
	TArchive archive;
	return archive.TryBy((TExternContainer *) group, "F_NO") ? NULL : "Делото не е архивирано";
}

void TAppNameApp::CmArchEditArchive()
{
	TExternContainer *const lawsuit = get_extern_container(parent, archived, NULL, false, GET_GROUP_ARCHFN);
	TVirtPtr VP(lawsuit);

	if (lawsuit)
	{
		TArchive archive;
		archive << lawsuit;

		if (archive.Get())
			edit_archive(parent, &archive, lawsuit);
	}
}

#if COURT_TYPE == COURT_AREAL
void TAppNameApp::CmArchImportArchives()
{
	import_archives(parent);
}

void TAppNameApp::CmArchExportArchived()
{
	export_archived(parent);
}
#endif  // AREAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmArchNewProve()
{
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_PROVABLES, NULL, origin_provable);

	if (group)
	{
		TVirtPtr VP(group);
		new_prove(parent, (TRCDKeyContainer *) group);
	}
}

void TAppNameApp::CmArchEditProve()
{
	TProve prove;

	if (get_group(parent, &prove, TYPE_PROVES, NULL))
		edit_prove(parent, &prove);
}

void TAppNameApp::CmArchNewProveAction()
{
	TProve prove;

	if (get_group(parent, &prove, TYPE_PROVES, NULL))
		new_prove_action(parent, &prove);
}

void TAppNameApp::CmArchEditProveAction()
{
	TProveAction proveAction;

	if (get_group(parent, &proveAction, TYPE_PROVES, KIND_PROVE_ACTIONS))
		edit_prove_action(parent, &proveAction);
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

void TAppNameApp::CmArchInOutMovement()
{
	CmClerkInOutMovement();
}

static const char *lawsuit_killable(TRCDKeyGroup *group, char status)
{
	if (status != RESULT_ARCHIVED)
		return wrong_status;

	if (Today < TYPECHECK_PTR_CAST(TArchive, group)->archiveDate)
		return "Текущата дата е по-малка от датата на архивиране на делото";

	return NULL;
}

void TAppNameApp::CmArchNewDestroy()
{
	TArchive archive;
	long killerNo = archive.killerNo;
	CDate killerDate = archive.killerDate;

	SET_REPEATER(CM_ARCH_NEW_DESTROY, REPEAT_DESTROYS);
	while (get_group(parent, &archive, TYPE_LAWSUITS, KIND_V_ARCHIVES, lawsuit_killable))
	{
		bool expired = (Today - archive.archiveDate) / 12 >= archive.keep;

		if (expired || ask("Внимание! Срокът за съхранение на делото не е изтекъл. %s", Q_CONTINUE))
		{
			archive.killerNo = killerNo;
			archive.killerDate = killerDate;
			new_destroy(parent, &archive, PrimaryGroup(TLawsuit));
			killerNo = archive.killerNo;
			killerDate = archive.killerDate;
		}

		CHECK_REPEATER();
	}
}

void TAppNameApp::CmArchEditDestroy()
{
	TArchive archive;

	if (get_group(parent, &archive, TYPE_LAWSUITS, KIND_V_ARCHIVES, C2S[RESULT_DESTROYED]))
		edit_destroy(parent, &archive, PrimaryGroup(TLawsuit));
}

void TAppNameApp::CmDelivExternal()
{
	announce_external(parent);
}

void TAppNameApp::CmDelivInternal()
{
	announce_internal(parent);
}

void TAppNameApp::CmDelivBarcodeScan()
{
	CmClerkBarcodeScan();
}

static const char *not_lawed(TRCDKeyGroup *group, char status)
{
	return group->key.type == TYPE_REQUEST && status == RESOLUTION_LAWED ? "" : NULL;
}

// 2009:153 LPR: one-liner; 2017:167 action.cpp -> static
static void query_ucn(TWindow *parent, TFoundGroup *ucnGroup)
{
	select_container(parent, NULL, ucnGroup, TYPE_UCNQUERABLES, NULL, NULL, not_lawed);
}

void TAppNameApp::CmQueryUCN()
{
	// 2003:302 BRW: brand-new, used as a model/standard
	// 2004:163 LPR: ucn search changes, used as a model/standard/example
	TFoundGroup ucnGroup;
	// 2008:022 LPR: using repeater
	repeat_ucn(parent, &ucnGroup, UCN_SIDES, query_ucn);
}

void TAppNameApp::CmQueryGroup()
{
	TRCDKeyGroup *group;

	SET_REPEATER_TRUE(CM_QUERY_GROUP);
	while ((group = get_group(parent, NULL, TYPE_LAWQUERABLES, NULL, match_null, GET_GROUP_QUERY)) != NULL)
	{
		TVirtPtr VP(group);
		query_group(parent, group);
	}
}

void TAppNameApp::CmElectricBlue()
{
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_ELEQUERABLES, NULL, match_null, GET_GROUP_QUERY);

	if (group)
	{
		TVirtPtr VP(group);
		electric_blue(parent, TYPECHECK_PTR_CAST(TRCDKeyContainer, group));
	}
}

#if COURT_TYPE == COURT_AREAL
void TAppNameApp::CmQueryFirm()
{
	TDamn damn;

	if (get_ucn(parent, &damn, C2S[UCN_DAMN_CODE]))
		query_key(parent, damn.key);
}
#endif  // AREAL

void TAppNameApp::CmQueryConnect()
{
	TConnect connect;

	while (get_connect(parent, &connect))
		query_connect(parent, &connect);
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmQueryJurorDistrib()
{
	query_juror_distrib(parent, NULL, false);
}
#endif  // AREAL || MILITARY || REGIONAL

void TAppNameApp::CmQueryArchive()
{
	query_archive(parent);
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmQueryEisppLaws()
{
	eispp_laws_list(parent);
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

void TAppNameApp::CmFloatCombo()
{
	float_combo(parent);
}

void TAppNameApp::CmFloatRequest()
{
	float_request(parent);
}

void TAppNameApp::CmFloatConnect()
{
	float_connect(parent);
}

void TAppNameApp::CmFloatIndex()
{
	float_index(parent);
}

void TAppNameApp::CmFloatSubpoena()
{
	float_subpoena(parent);
}

void TAppNameApp::CmFloatAnnounce()
{
	float_announce(parent);
}

void TAppNameApp::CmFloatSurround()
{
	float_surround(parent);
}

void TAppNameApp::CmFloatSenders()
{
	float_senders(parent);
}

void TAppNameApp::CmFloatOutReg()
{
	float_outreg(parent);
}

void TAppNameApp::CmFloatUnappealed()
{
	float_unappealed(parent);
}

void TAppNameApp::CmFloatMovement()
{
	float_movement(parent);
}

void TAppNameApp::CmFloatSched()
{
	float_sched(parent);
}

void TAppNameApp::CmFloatPenality()
{
	float_expense(parent, SUM_TYPE_PENALITY);
}

void TAppNameApp::CmFloatReward()
{
	float_expense(parent, SUM_TYPE_REWARD);
}

void TAppNameApp::CmFloatTax()
{
	float_expense(parent, SUM_TYPE_TAX);
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmFloatJuror()
{
	float_juror(parent);
}
#endif  // AREAL || MILITARY || REGIONAL

void TAppNameApp::CmFloatExecList()
{
	float_exec_list(parent);
}

#if COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmFloatHeritage()
{
	float_heritage(parent);
}
#endif  // REGIONAL

void TAppNameApp::CmFloatPayments()
{
	float_payments(parent);
}

void TAppNameApp::CmFloatRegisterAct()
{
	float_register_act(parent);
}

void TAppNameApp::CmQueryRegisterAct()
{
	query_register_act(parent);
}

void TAppNameApp::CmQueryMovement()
{
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_MOVABLES, NULL, match_null, GET_GROUP_QUERY);

	if (group)
	{
		TVirtPtr VP(group);
		query_movement(parent, group);
	}

}

void TAppNameApp::CmExpireSched()
{
	expire_sched(parent);
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmQueryExchange()
{
	first_set(parent);
}
#endif  // AREAL || REGIONAL

void TAppNameApp::CmTimedSession()
{
	timed_session(parent);
}

void TAppNameApp::CmTimedLawsuit()
{
	timed_lawsuit(parent);
}

void TAppNameApp::CmUnfinishedLaw()
{
	unfinished_law(parent);
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmQueryJurorLoad()
{
	query_juror_load(parent);
}
#endif  // AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_AREAL
void TAppNameApp::CmQueryBankruptcy()
{
#if TESTVER
	if (ask("Новия вариант?"))
		query_bank_html(parent, false);
	else
		print_bankruptcy_wide_checkup(parent, false);
#else  // TESTVER
	// 2018-09-14: LPR: switching to html
	query_bank_html(parent, false);
#endif  // TESTVER
}

void TAppNameApp::CmQueryInconSide()
{
	query_inconside(parent);
}
#endif  // AREAL

void TAppNameApp::CmQueryImportant()
{
	float_important(parent);
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmQueryCorrupt()
{
	query_corrupt(parent);
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

void TAppNameApp::CmPeriodSession()
{
	if (COURT_LOCK)
		period_session(parent);
	else
		select_ucns(parent, UCN_SIDES, false, NULL, float_ucns_period);
}

void TAppNameApp::CmYoungPeriod()
{
	float_young_period(parent);
}

void TAppNameApp::CmUnwrittenAct()
{
	unwritten_act(parent);
}

void TAppNameApp::CmUnwrittenSes()
{
	unwritten_ses(parent);
}

void TAppNameApp::CmPeriodOutReg()
{
	TFoundGroup ucnGroup;

	if (get_ucn(parent, &ucnGroup, UCN_DATED_SIDES))
	{
		TPeriodGroup period("T_OUTREG");

		period.minDate = CDate(1, Today.month, Today.year);
		if (get_period(parent, &period))
			period_outreg(parent, &ucnGroup, &period);
	}
}

void TAppNameApp::CmFloatArchive()
{
	float_archive(parent);
}

void TAppNameApp::CmFloatDestroy()
{
	float_destroy(parent);
}

void TAppNameApp::CmQuerySubpoenas()
{
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_SUBPOENABLES, KIND_VISIBLE_SUBPOENABLES, not_departed);

	if (group)
	{
		TVirtPtr VP(group);
		TDatedContainer *container = TYPECHECK_PTR_CAST(TDatedContainer, group);
		TGArray<TSideWind> sideWinds;

		load_sidewinds(container, &sideWinds);
		print_subpoena_list(parent, container, &sideWinds);
	}
}

void TAppNameApp::CmQueryPayments()
{
	query_payments(parent);
}

void TAppNameApp::CmQueryNIAProtocols()
{
	query_nia_protocols(parent);
}

void TAppNameApp::CmQueryJudges()
{
	print_judge_checkup(parent);
}

void TAppNameApp::CmQueryActivity()
{
	query_activity(parent);
}

#if COURT_TYPE == COURT_AREAL
void TAppNameApp::CmQueryP24a1()
{
	query_p24a1(parent);
}

void TAppNameApp::CmQueryBanConSist()
{
	select_firms(parent, query_banconside);
}
#endif  // AREAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
void TAppNameApp::CmQueryStatFirst()
{
	stat_query_first(parent);
}
#endif  // APPEAL || ADMIN || AREAL || MILITARY
#if COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmQueryStatSecond()
{
	stat_query_second(parent);
}
#endif  // ADMIN || AREAL || MILITARY || REGIONAL
#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY
void TAppNameApp::CmQueryStatThird()
{
	stat_query_third(parent);
}
#endif  // APPEAL || ADMIN || AREAL || MILITARY

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
void  TAppNameApp::CmCombineCorrupt()
{
	combine_corrupts(parent);
}
#endif  // APPEAL || AREAL

void TAppNameApp::CmAdminObjectCount()
{
	query_object_count(parent);
}

void TAppNameApp::CmBookDesc()
{
	print_desc_book(parent);
}

void TAppNameApp::CmBookDescHtml()
{
	desc_book(parent);
}

void TAppNameApp::CmBookDescDeliv()
{
	invDeliver_book(parent);
}

#if COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmBookZZDN()
{
	print_book_ZZDN(parent);
}

void TAppNameApp::CmBookSuccess()
{
	heritage_book(parent);
}
#endif  // REGIONAL

void TAppNameApp::CmBookAbc()
{
	print_abc_book(parent);
}

void TAppNameApp::CmBookAbcHtml()
{
	abc_book(parent);
}

void TAppNameApp::CmBookTime()
{
	print_time_book(parent);
}

void TAppNameApp::CmBookTimeHtml()
{
	time_book(parent);
}

void TAppNameApp::CmBookClosed()
{
	print_closed_book(parent);
}

void TAppNameApp::CmBookClosedHtml()
{
	closed_session_book(parent);
}

void TAppNameApp::CmBookInDoc()
{
	indoc_book(parent);
}

void TAppNameApp::CmBookRequest()
{
	request_book(parent);
}

void TAppNameApp::CmBookInReg()
{
	if (COURT_LOCK)
		print_appealed_input_diary(parent);
	else
		inreg_book(parent);
}

void TAppNameApp::CmBookOutReg()
{
	book_outreg(parent);
}

void TAppNameApp::CmBookDecision()
{
	decision_register(parent);
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_ADMIN || COURT_TYPE == COURT_AREAL
void TAppNameApp::CmBook39()
{
	book_39(parent);
}
#endif  // APPEAL || ADMIN || AREAL

void TAppNameApp::CmBookRecusal()
{
	recusal_book(parent);
}

#if COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmBookDivorce()
{
	divorce_book(parent);
}
#endif  // REGIONAL

#if COURT_TYPE == COURT_ADMIN
void TAppNameApp::CmBook251()
{
	book_251(parent);
}
#endif  // ADMIN

void TAppNameApp::CmBookExecList()
{
	exec_list_book(parent);
}

void TAppNameApp::CmBookAppeal()
{
	appealed_book(parent);
}

#if COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmBookEuher()
{
	book_euher(parent);
}
#endif  // REGIONAL

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmBookExecution()
{
	execute_book(parent);
}
#endif  // AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_AREAL
void TAppNameApp::CmBook634()
{
	print_634_trade_book(parent);
}

void TAppNameApp::CmBookSlowdown()
{
	slow_book(parent);
}
#endif  // AREAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmBookProve()
{
	prove_book(parent);
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

void TAppNameApp::CmBookPenalities()
{
	// 2016:113 IRQ/FIX? leave expense_book only
	expense_book(parent);
}

void TAppNameApp::CmBookArchive()
{
	archive_book(parent);
}

void TAppNameApp::CmBookExternal()
{
	invAnno_book(parent);
}

void TAppNameApp::CmBookInternal()
{
	invSubpoena_book(parent);
}

void TAppNameApp::CmBookPrint()
{
	TPrinterGroup group;

	printer_setup(parent, &group);
}

void TAppNameApp::CmAdminUsers()
{
	edit_users(parent);
}

void TAppNameApp::CmAdminKnownPersons()
{
	edit_knowns(parent);
}

void TAppNameApp::CmAdminSubjects()
{
	edit_subjects(parent);
}

void TAppNameApp::CmAdminCompositions()
{
	edit_compositions(parent);
}

void TAppNameApp::CmAdminPenalities()
{
	log(LOG_NOM, CM_ADMIN_PENALITIES, "основания - глоби");
	edit_penalities(parent);
}

void TAppNameApp::CmAdminReasons()
{
	log(LOG_NOM, CM_ADMIN_REASONS, "основания - служебни представители");
	edit_reasons(parent);
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmAdminDangers()
{
	edit_dangers(parent);
}

void TAppNameApp::CmAdminArticles()
{
	edit_articles(parent);
}
#endif  // AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_AREAL
void TAppNameApp::CmAdminBanConSists()
{
	edit_banconsists(parent);
}
#endif  // AREAL

void TAppNameApp::CmAdminSlcReasons()
{
	edit_slc_reasons(parent);
}

void TAppNameApp::CmAdminArchiveIndexes()
{
	edit_archive_indexes(parent);
}

void TAppNameApp::CmAdminBankAccounts()
{
	edit_accounts(parent);
}

void TAppNameApp::CmAdminCurrences()
{
	edit_currences(parent);
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmAdminSessionPrices()
{
	edit_session_prices(parent);
}
#endif  // AREAL || MILITARY || REGIONAL

void TAppNameApp::CmAdminSenders()
{
	edit_senders(parent);
}

void TAppNameApp::CmAdminCalendar()
{
	edit_calendar(parent);
}

void TAppNameApp::CmAdminHalls()
{
	edit_halls(parent);
}

// 2017:088 preserve if same court, leave InstanceIII cache to offields
static void create_sender_court(long court, char ucnType, const char *ucn)
{
	TSender sender;
	bool found;

	sender.ucnType = ucnType;
	strcpy(sender.ucn, ucn);
	found = sender.Try("F_COURT");

	if (sender.court != court)
	{
		strzcpy(sender.name, Court->Seek(court), sizeof sender.name);
		sender.court = court;
		sender.eispp = Court->eispp;
		sender.Save(found);
	}
}

static void delete_sender_court(char ucnType, const char *ucn)
{
	TSender sender;

	sender.ucnType = ucnType;
	strcpy(sender.ucn, ucn);
	sender.Delete(true);
}

static void create_sender_courts()
{
	create_sender_court(COURT_CODE, UCN_INSTANCE_SELF, INSTANCE_SELF);
	create_sender_court(COURT_CODE, UCN_INSTANCE_HIGHER, INSTANCE_SELF);	// 2012:079
	// 2006:321 former inter-adjust
#if COURT_TYPE == COURT_AREAL
	long courtIII = COURT_AREA - COURT_AREA % 100;

	create_sender_court(courtIII, UCN_INSTANCE_HIGHER, INSTANCE_III);

	// 2009:268 LPR: create/remove SOFAP depending on appeal region
	TSender sender;
	sender.ucnType = UCN_INSTANCE_HIGHER;
	strcpy(sender.ucn, INSTANCE_SOFAP);

	if (courtIII != COURT_SOFAP)
		create_sender_court(COURT_SOFAP, UCN_INSTANCE_HIGHER, INSTANCE_SOFAP);
	else if (sender.Try())
		sender.Delete(true);
#endif  // AREAL
#if COURT_TYPE == COURT_REGIONAL
	create_sender_court(COURT_AREA, UCN_INSTANCE_HIGHER, INSTANCE_III);
	// 2007:044; 2008:267 LPR: broken admin hyerarhy, find by type + area
	//create_sender_court(COURT_AREA / 10 + 900, UCN_INSTANCE_HIGHER, INSTANCE_ADMIN);
	msql m(Court->Name, "F_COURT");

	m.AddChar("F_COURT_TYPE", COURT_ADMIN + '0');
	m.AddLong("F_COURT_AREA", COURT_AREA);

	TQuery q(str(m));
	// 2008:316 FIX: q.Read(), check
	if (q.Read())
	{
		*Court << q;
		create_sender_court(Court->court, UCN_INSTANCE_HIGHER, INSTANCE_ADMIN);
	}

	create_sender_court(COURT_AREA - COURT_AREA % 100, UCN_INSTANCE_HIGHER, INSTANCE_APP);
#endif  // REGIONAL

	char courtUCN[SIZE_OF_UCN];
	sprintf(courtUCN, "%010d", COURT_CODE);
	//create_sender_court(COURT_CODE, UCN_INSTANCE_ANNOUNCE, courtUCN);
	delete_sender_court(UCN_INSTANCE_EXTERNAL, courtUCN);
}

static void serial_uclp_changed()
{
	UCLP->Seek(COURT_UCLP);
	// 2007:307 LPR: fix Area and Region fields
	DEFAULT_AREA = Area->area = Region->area = UCLP->area;
	DEFAULT_REGION = Region->region = UCLP->region;
	Region->ReloadCache();	// 2010:274
}

struct TDupExt
{
	int type;
	const char *name;

	TDupExt(int tType, const char *tName) : type(tType), name(tName) { }
};

void TAppNameApp::CmAdminConfig()
{
	char BACKUP_NO[25 + 4 + 2 + 1];

	strcpy(BACKUP_NO, SERIAL_NO);
	configure(parent);
	reload_config();

	if (!SERIAL_VALID)
		error("Грешка %d при обработка на серийния номер. Желателно е да се въведе валиден сериен номер.", SERIAL_ERROR);
	else if (strcmp(SERIAL_NO, BACKUP_NO))
	{
		create_sender_courts();
		serial_uclp_changed();		// 2010:274 FIX: was missing
		//message("%ld %ld %ld\n", CONFIG_MAX - 1, COURT_CODE, COURT_FLAT + COURT_WRIT + COURT_N10K +
		//	(COURT_MINI ? 8 : 0) + COURT_LOCK);
	}

	// 2014:056 LPR/IRQ: check for duplicate exts
	// 2014:182 LPR/IRQ/TRQ: moved here, warn only
	TIArray<TDupExt> exts;

	for (int type = 0; type < TYPE_COUNT; type++)
	{
		TDoubleExt *ext = Electrics[type].ext;

		for (int i = 0; i < ELECTRIC_EXTS_MAX && any(ext->type); i++, ext++)
		{
			exts.Add(new TDupExt(type, ext->type));

			if (any(ext->ltype))
				exts.Add(new TDupExt(type, ext->ltype));
		}
	}

	for (int i = 0; i < exts.Count() - 1; i++)
	{
		for (int n = i + 1; n < exts.Count(); n++)
		{
			if (!strcmp(exts[i]->name, exts[n]->name))
			{
				mstr m;

				m.printf("Тип %s от електронна папка се дублира в \"%s\"", exts[i]->name,
					Electrics[exts[i]->type].whats);
				if (exts[i]->type != exts[n]->type)
					m.printf(" и \"%s\"", Electrics[exts[n]->type].whats);
				message(MB_ICONWARNING | MB_OK, "%s.", str(m));
			}
		}
	}

	reload_title(parent);
}

void TAppNameApp::CmAdminInformeds()
{
	edit_informeds(parent);
}

void TAppNameApp::CmAdminEPortQuery()
{
	eportal_query(parent);
}

void TAppNameApp::CmAdminEPortLawsuitAssign()
{
	TRCDKeyGroup *group;

	SET_REPEATER_TRUE(CM_QUERY_GROUP);
	while ((group = get_group(parent, NULL, TYPE_LAWSUITS, NULL, match_null, GET_GROUP_QUERY)) != NULL)
	{
		TVirtPtr VP(group);
		eportal_query_access(parent, (TLawsuit *) group);
	}
}

void TAppNameApp::CmAdminEPortPersonAssign()
{
	TTransGroup ucnGroup;

	if (get_ucn(parent, &ucnGroup, UCN_CITIZENS, INVOLVEMENT_EPORTALS, false, false, false))
		eportal_query_access(parent, &ucnGroup);

}

void TAppNameApp::CmAdminEPortSend()
{
	TLawsuit lawsuit;

	if (get_group(parent, &lawsuit, TYPE_LAWSUITS, NULL, not_in_progress))
		eportal_send_unlock(parent, &lawsuit);
}

void TAppNameApp::CmAdminEPortDelete()
{
	TLawsuit lawsuit;

	if (get_group(parent, &lawsuit, TYPE_LAWSUITS, NULL, not_in_progress))
		eportal_delete_lock(parent, &lawsuit);
}

void TAppNameApp::CmAdminCountries()
{
	edit_countries(parent);
}

void TAppNameApp::CmAdminAreas()
{
	edit_areas(parent);
}

void TAppNameApp::CmAdminRegions()
{
	edit_regions(parent);
}

void TAppNameApp::CmAdminUCLPS()
{
	edit_uclps(parent);
}

void TAppNameApp::CmAdminStreets()
{
	edit_streets(parent);
}

void TAppNameApp::CmAdminLog()
{
	float_log(parent);
}

void TAppNameApp::CmAdminDeleteInDoc()
{
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_INDOCS, NULL, indoc_deletable, GET_GROUP_EXACT);
	TVirtPtr VP(group);

	if (group)
		delete_indoc(parent, (TRCDKeyContainer *) group);
}

void TAppNameApp::CmAdminDeleteRequestResolution()
{
	TRequestResolution resolution;
	// 2017:212 RESOLUTION_STANDALONES -> resolution_deletable
	if (get_group(parent, &resolution, TYPE_REQUESTS, KIND_REQUEST_RESOLUTIONS, resolution_deletable))
		delete_resolution(parent, &resolution);
}

void TAppNameApp::CmAdminDeleteLawsuit()
{
	TLawsuit lawsuit;

	if (get_group(parent, &lawsuit, TYPE_LAWSUITS, NULL, RESULT_CLEANS))
		delete_lawsuit(parent, &lawsuit);
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmAdminDeleteEisppNmr()
{
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_PROVABLES, NULL, eispp_nmr_deletable);

	if (group)
	{
		TVirtPtr VP(group);
		delete_eispp_nmr(parent, (TOriginContainer *) group);
	}
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

void TAppNameApp::CmAdminDeleteLawsuitMovement()
{
	TLawsuit lawsuit;

	if (get_group(parent, &lawsuit, TYPE_LAWSUITS, NULL, not_archived_killed, GET_GROUP_ARCHFN))
		delete_movement(parent, &lawsuit);
}

void TAppNameApp::CmAdminDeleteLawsuitEvent()
{
	TRCDKeyGroup *const group = get_group(parent, NULL, TYPE_LAWSUITS, KIND_VISIBLE_SESSIONS_ENDOCS, match_null);
	TVirtPtr VP(group);

	if (group)
		delete_lawsuit_event(parent, PrimaryGroup(TLawsuit), (TIndexContainer *) group);
}

void TAppNameApp::CmAdminDeleteInRegEvent()
{
	TInRegEvent inRegEvent;

	// 2010:260 LPR/LRQ: FIX: UNDEPARTEDS -> INREG_RESOLUTIONS
	// 2018-09-21: FIX: INREG_RESOLUTIONS -> event_editable
	if (get_group(parent, &inRegEvent, TYPE_INREGS, KIND_VISIBLE_EVENTS, event_editable))
		delete_inreg_event(parent, &inRegEvent);
}

void TAppNameApp::CmAdminDeleteOutreg()
{
	TOutReg outReg;

	if (get_group(parent, &outReg, TYPE_OUTREGS, NULL))
		delete_outreg(parent, &outReg);
}

void TAppNameApp::CmAdminDeleteOutregReturn()
{
	TOutReg outReg;

	if (get_group(parent, &outReg, TYPE_OUTREGS, NULL, RESULT_IOREG_RETURNEDS))
		delete_outreg_return(parent, &outReg);
}

static const char *nia_deletable(TNIAProtocol *protocol)
{
	msql m("T_EXEC_LIST");

	m.Add(protocol, "F_NIA_NO, F_NIA_YEAR, F_NIA_TYPE");
	m.AddIsNull("F_TDEP_IN_NO", false);

	TQuery q(str(m));
	return q.Read() ? "По протокола има един или повече изпълнителни листи с попълнени данни за получаване в НАП." : NULL;
}

void TAppNameApp::CmAdminDeleteNIAProtocol()
{
	TNIAProtocol protocol;

	if (get_nia_protocol(parent, &protocol, nia_deletable))
		delete_nia_protocol(parent, &protocol);
}

void TAppNameApp::CmAdminDeleteForeignAddress()
{
	TFoundGroup ucnGroup;

	if (get_ucn(parent, &ucnGroup, UCN_SIDES))
	{
		TAddress address;
		TForeignAddress foreignAddress;

		address << ucnGroup;
		foreignAddress << ucnGroup;

		if (address.Get() && foreignAddress.Get())
			delete_foreign(parent, &foreignAddress);
	}
}

void TAppNameApp::CmAdminDeleteOldCitizen()
{
	TFoundGroup ucnGroup;

	if (get_ucn(parent, &ucnGroup, UCN_REAL_SIDES))
	{
		TNamedGroup *const oldGroup = strchr(UCN_CITIZENS, ucnGroup.ucnType) ? (TNamedGroup *) new TOldCitizen :
			(TNamedGroup *) new TOldFirm;
		TVirtPtr VP(oldGroup);

		*oldGroup << ucnGroup;

		if (oldGroup->Get())
			delete_old_names(parent, oldGroup);
	}
}

void TAppNameApp::CmAdminDeleteMovement()
{
	TMovement movement;

	if (get_group(parent, &movement, TYPE_MOVABLES, KIND_MOVEMENTS, match_null))
		delete_movement(parent, &movement);
}

static const char *archived_not_killed(TRCDKeyGroup *group, char)
{
	TArchive archive;
	const char *status = NULL;

	if (!archive.TryBy((TExternContainer *) group, "F_KILL_DATE"))
		status = "Делото не е архивирано";
	else if (!archive.killDate.Empty())
		status = "Делото е унищожено";

	return status;
}

void TAppNameApp::CmAdminDeleteArchive()
{
	TExternContainer *const lawsuit = get_extern_container(parent, archived_not_killed, NULL, false, GET_GROUP_ARCHFN);
	TVirtPtr VP(lawsuit);

	if (lawsuit)
	{
		TArchive archive;
		archive << lawsuit;
		delete_archive(parent, &archive, lawsuit);
	}
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmAdminDeleteProve()
{
	TProve prove;

	if (get_group(parent, &prove, TYPE_PROVES, NULL))
		delete_prove(parent, &prove);
}

void TAppNameApp::CmAdminDeleteProveAction()
{
	TProveAction proveAction;

	if (get_group(parent, &proveAction, TYPE_PROVES, KIND_PROVE_ACTIONS))
		delete_prove_action(parent, &proveAction);
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

void TAppNameApp::CmAdminDeleteXfer()
{
	xfer_delete(parent);
}

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmAdminDeleteJurorDistrib()
{
	TBaseSession *const session = get_juror_distrib(parent, -1, NULL, false);
	TVirtPtr VP(session);

	if (session)
		delete_distrib(parent, session);
}
#endif  // AREAL || MILITARY || REGIONAL

void TAppNameApp::CmAdminDiscardCitizens()
{
	if (warn("Тази функция изтрива от базата данни всички лица, въведени с ЕГН, които реално не се използват, "
		"и техните адреси.\n\n\%s", Q_CONTINUE))
	{
		discard_citizens(parent);
	}
}

void TAppNameApp::CmAdminRenumberRequest()
{
	TRequest request;

	if (get_group(parent, &request, TYPE_REQUESTS, NULL, RESULT_CLEANS))
		renumber_request(parent, &request);
}

void TAppNameApp::CmAdminRenumberLawsuit()
{
	TLawsuit lawsuit;

	if (get_group(parent, &lawsuit, TYPE_LAWSUITS, NULL, RESULT_CLEANS))
		renumber_lawsuit(parent, &lawsuit);
}

void TAppNameApp::CmAdminRenumberUXN()
{
	TFoundGroup source, target;

	if (TRenumberUXNDialog(parent, &source, &target).Execute() == IDOK && ask_renumber(&source, &target))
		if (renumber(parent, &source, &target))
			message("Лицето е преномерирано.");
}

void TAppNameApp::CmAdminRenumberSurround()
{
	TSurround surround;

	if (get_group(parent, &surround, TYPE_SURROUNDS, NULL, RESULT_CLEANS, match_null, GET_GROUP_EXACT))
		renumber_surround(parent, &surround);
}

void TAppNameApp::CmAdminRenumberInreg()
{
	TInReg inReg;

	if (get_group(parent, &inReg, TYPE_INREGS, NULL, RESULT_CLEANS))
		renumber_inreg(parent, &inReg);
}

void TAppNameApp::CmAdminRenumberStreet()
{
	TStreet source;
	TStreet target;

	while (TRenumberStreetDialog(parent, &source, &target).Execute() == IDOK)
		renumber(&source, &target);
}

void TAppNameApp::CmAdminRenumberSubjects()
{
	update_subjects(parent);
}

void TAppNameApp::CmAdminJoinUXN()
{
	TFoundGroup source, target;

	while (TJoinUXNDialog(parent, &source, &target).Execute() == IDOK && ask_join(&source, &target))
		if (join(parent, &source, &target))
			message("Лицето е присъединено.");
}

void TAppNameApp::CmAdminJoinStreet()
{
	TStreet source;
	TStreet target;

	while (TJoinStreetDialog(parent, &source, &target).Execute() == IDOK)
		join(&source, &target);
}

void TAppNameApp::CmAdminJoinEqualStreets()
{
	join_equal_streets(parent);
}

void TAppNameApp::CmAdminCheckoutCitizens()
{
	checkout_citizens(parent);
}

void TAppNameApp::CmAdminCheckoutFirms()
{
	checkout_firms(parent);
}

void TAppNameApp::CmAdminCheckoutStreetMaps()
{
	checkout_street_maps(parent);
}

void TAppNameApp::CmAdminResetSession()
{
	TSession source;

	// 2011:257 LRQ: RESETABLES -> FIXEDS
	if (get_group(parent, &source, TYPE_LAWSUITS, KIND_OPEN_SESSIONS, RESULT_FIXEDS))
		reset_session(parent, &source, NULL, PrimaryGroup(TLawsuit));
}

void TAppNameApp::CmAdminRekindLawsuit()
{
	rekind_lawsuit(parent);
}

void TAppNameApp::CmAdminRecomposeLawsuit()
{
	TLawsuit source;
	TLawsuit target;

	if (get_group(parent, &source, TYPE_LAWSUITS, NULL))
	{
		target << source;

		if (TGetRecomposeDialog(parent, &target).Execute() == IDOK)
			recompose_law(&source, &target);
	}
}

void TAppNameApp::CmAdminCopySideWinds()
{
	TDummyContainer source;
	TDummyContainer target;
	mstr types(Default->colleges);

	types.cat(TYPE_REQUESTS);
	types.cat(TYPE_INREGS);

	char sourceKind;
	// 2012:142 FIX: lawsuits Involvements() need source kind
	target.Add(new TCharAlias("F_SOURCE_KIND", &sourceKind));

	if (TCopySideWindsDialog(parent, &source, &target, str(types)).Execute() == IDOK)
	{
		mstr m;

		if (strchr(KIND_XFERABLES, source.kind))
			m.printf("%s %Dг. по ", Kind->Seek(source.kind), &source.date);
		m.printf("%s %ld/%ld към ", Type->Seek(source.key.type), source.key.no, source.key.year);
		if (strchr(KIND_XFERABLES, target.kind))
			m.printf("%s %Dг. по ", Kind->Seek(target.kind), &target.date);
		m.printf("%s %ld/%ld?", Type->Seek(target.key.type), target.key.no, target.key.year);

		if (ask("Желаете ли да копирате страните от %s", str(m)))
		{
			TSideWind sideWind;
			TLinkWind linkWind;
			// 2012:142 FIX: Involvements() require the exact target type
			TRCDKeyContainer *const tTarget = kind2RCDKeyContainer(target.kind);
			TVirtPtr VP(tTarget);

			*tTarget << target;
			copy_winds(&source, tTarget, sideWind);
			copy_winds(&source, tTarget, linkWind);

			mstr m;
			m.printf("%D <- %s %D", &target.date, KindAlias->Seek(source.kind), &source.date);
			log(LOG_TRANSFER, CM_ADMIN_COPY_SIDE_WINDS, &target, str(m));
		}
	}
}

void TAppNameApp::CmAdminRenumberSideWind()
{
	renumber_sideWind(parent);
}

void TAppNameApp::CmAdminMoveJudicialAct()
{
	if (electric_rekind_access_check())
	{
		TDecision decision;
		TSession target;

		if (get_move_judicial_act(parent, decision, target) && ask_move(decision, target))
			move_act(decision, target);
	}
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmAdminEisppEditLawsuit()
{
	TLawsuit lawsuit;

	if (get_group(parent, &lawsuit, C2S[TYPE_PUNISHMENT_LAW], NULL, has_eispp_nmr))
		eispp_edit_lawsuit(&lawsuit);
}

void TAppNameApp::CmAdminEisppEditFzlPne()
{
	TLawsuit lawsuit;

	if (get_group(parent, &lawsuit, C2S[TYPE_PUNISHMENT_LAW], NULL, has_eispp_nmr))
		eispp_edit_fzlpne(parent, &lawsuit);
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

void TAppNameApp::CmAdminUpdateRequests()
{
	update_requests(parent);
}

void TAppNameApp::CmAdminUpdateLongTexts()
{
	update_long_texts(parent);
}

void TAppNameApp::CmAdminUpdateLogKinds()
{
	update_log_kinds(parent);
}

void TAppNameApp::CmAdminUpdateEPortals()
{
	update_eportals(parent);
}

#if COURT_TYPE == COURT_AREAL
void TAppNameApp::CmAdminImportReRegistered()
{
	import_reregistered(parent);
}
#endif  // AREAL

void TAppNameApp::CmAdminImportPersonalNo()
{
	import_personal_no(parent);
}

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL
static const char *bankrupt(TRCDKeyGroup *group, char)
{
	return strchr(KIND_INCONSISTENCE_INDOCS, ((TLawsuit *) group)->sourceKind) ? NULL : "Делото не е по несъстоятелност";
}

void TAppNameApp::CmAdminExportBankrupt()
{
	if (!any(XFER_BANKRUPT_PATH))
		error("Не е зададена директория за трансфер на данни към ИСПН.");
	else if (!exist(XFER_BANKRUPT_PATH, false))
		error("Директорията за трансфер на данни към ИСПН липсва или е невалидна.");
	else
	{
		TLawsuit lawsuit;
		long xfer_active = XFER_ACTIVE;	// 2011:304 temporarily ON for initial xfer

		XFER_ACTIVE |= XFER_WEBANKS;
		while (get_group(parent, &lawsuit, TYPE_TRADEALIKE_LAWS, KIND_TRADEALIKE_LAWS, bankrupt))
			export_bankrupt(parent, &lawsuit);
		XFER_ACTIVE = xfer_active;
	}
}
#endif  // APPEAL || AREAL

#if COURT_TYPE == COURT_ADMIN
void TAppNameApp::CmAdminExportCSV()
{
	if (any(EXPORT_CSV_PATH))
		export_csv(parent);
	else
		error("Не е зададена директория за експорт на данни в CSV файлове.");
}
#endif  // ADMIN

void TAppNameApp::CmAdminQueryWeb()
{
	query_web_html(parent);
}

#if COURT_TYPE == COURT_AREAL
void TAppNameApp::CmAdminQueryBankrupt()
{
#if TESTVER
	if (ask("Новия вариант?"))
		query_bank_html(parent, true);
	else
		print_bankruptcy_wide_checkup(parent, true);
#else  // TESTVER
	// 2018-09-14: LPR: switching to html
	query_bank_html(parent, true);
#endif  // TESTVER
}

void TAppNameApp::CmAdminBankrupts()
{
	edit_bankrupts(parent);
}
#endif  // AREAL

#if COURT_TYPE == COURT_APPEAL || COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_MILITARY || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmAdminQueryPolice()
{
	query_police(parent);
}
#endif  // APPEAL || AREAL || MILITARY || REGIONAL

#if COURT_TYPE == COURT_AREAL || COURT_TYPE == COURT_REGIONAL
void TAppNameApp::CmAdminQueryProsec()
{
	query_prosec(parent);
}
#endif  // AREAL || REGIONAL

void TAppNameApp::CmAdminFloatPublication()
{
	float_publication(parent);
}

void TAppNameApp::CmAdminFloatElectric()
{
	float_electric(parent);
}

void TAppNameApp::CmAdminQueryXferWeb()
{
	xfer_web_query(parent);
}

void TAppNameApp::CmAdminQueryYoung()
{
	period_side(parent);
}

void TAppNameApp::CmAdminRandomAct()
{
	random_act(parent);
}

void TAppNameApp::CmHelpAbout()
{
	TAboutDialog(parent).Execute();
}

void TAppNameApp::CmHelpRefresh()
{
	if (ask("Желаете ли презареждане на конфигурацията?"))
	{
		reload_config();
		reload_title(parent);
	}
}

void TAppNameApp::CmHelpLogin()
{
	long layout = get_layout();

	if (get_login(parent))
	{
		initialize_extra();
		reset_user_names();
		electric_signal(ELECTRIC_ACCESS, NULL);
	}

	set_layout(layout);
}

void TAppNameApp::CmHelpTesting()
{
	testfunc(parent);
}

// ----- MAIN ----------------------------------------------------------------
static bool VersionOK()
{
	OSVERSIONINFO osVersionInfo;
	osVersionInfo.dwOSVersionInfoSize = sizeof osVersionInfo;

	if (!GetVersionEx(&osVersionInfo))
	{
		return error("Unable to get the operation system version.\n"
			"Грешка при вземане на версията на операционната система.");
	}

	if (osVersionInfo.dwPlatformId != VER_PLATFORM_WIN32_NT || osVersionInfo.dwMajorVersion < 5)
		return error("Windows 2000 or later required.\nИзисква се Windows 2000 или следваща версия.");

	DWORD version = osVersionInfo.dwMajorVersion * 10 + osVersionInfo.dwMinorVersion;

	WinXPOrLater = version >= 51;
	Win7OrLater = version >= 61;
	return true;
}

#if COURT_TYPE == COURT_APPEAL
#define DBTARGET "APPEAL"
#define APTARGET "Appealative"
#define APSCREEN "апелативния"
#endif  // APPEAL
#if COURT_TYPE == COURT_ADMIN
#define DBTARGET "ADMIN"
#define APTARGET "Administrative"
#define APSCREEN "административния"
#endif  // ADMIN
#if COURT_TYPE == COURT_AREAL
#define DBTARGET "AREA"
#define APTARGET "Areal/District"
#define APSCREEN "окръжния"
#endif  // AREAL
#if COURT_TYPE == COURT_MILITARY
#if SPECIAL
#define DBTARGET "SPECIAL"
#define APTARGET "Special"
#define APSCREEN "специализирания"
#else  // SPECIAL
#define DBTARGET "MILITARY"
#define APTARGET "Military"
#define APSCREEN "военния"
#endif  // SPECIAL
#endif  // MILITARY
#if COURT_TYPE == COURT_REGIONAL
#define DBTARGET "REGION"
#define APTARGET "Regional"
#define APSCREEN "районния"
#endif  // REGIONAL

static bool copy_file(const char *source, const char *target)
{
	TUserFile uf("изпълним");
	long size;
	char *buffer;
	bool result = false;

	if ((buffer = uf.ReadAll(source, size)) != NULL)
	{
		FILE *f;

		if ((f = uf.Open(target, "wb")) != NULL)
		{
			result = fwrite(buffer, size, 1, f) == 1;

			if (!result)
			{
				if (ferror(f))
					uf.Error("четене на");
				else
					error("%s: грешка при запис в изпълним файл.", target);
			}

			if (!uf.Close())
				result = false;
		}
	}

	return result;
}


static bool check_database(const char *my_name)
{
	TSender sender;
	int db_base, db_test;
	int my_base = atoi(VERSION + 2), my_extra = atoi(EXTRAVER);

	UINT len = strlen(DBTARGET);
	sender.ucnType = UCN_INSTANCE_VERSION;
	strcpy(sender.ucn, "0000000001");
	if (!sender.Try() || memcmp(sender.name, DBTARGET, len) || memcmp(sender.name + len, " 1.", 3) || (db_base = atoi(sender.name + len + 3)) <= 0)
		return error("ВНИМАНИЕ! Версията на %s сървър на %s липсва или е невалидна!", APSCREEN, AppNameName);
	sender.ucnType = UCN_INSTANCE_VERSION;
	strcpy(sender.ucn, "0000000002");
	if (!sender.Try() || memcmp(sender.name, "COMMON", 6) || memcmp(sender.name + 6, " 1.", 3) || (db_test = atoi(sender.name + 9)) <= 0)
		return error("ВНИМАНИЕ! Версията на сървъра на %s липсва или е невалидна!", AppNameName);
	if (db_base != db_test)
		return error("ВНИМАНИЕ! Версиите на сървъра на %s не съвпадат!", AppNameName);

	char appname[SIZE_OF_PATH];
	DWORD ver_buff[0x200];
	DWORD size, zero;
	char *file = "", *product = "", *type = "";
	int lm_base = 0, lm_extra;
	// 2008:078 LPR: URQ: message why not performing update
	char *reason = NULL;

	build_fn(UPDATE_DIRECTORY, "appname.exe", appname);

	if (!UPDATE_ACTIVE)
		reason = "Автоматичната актуализация е изключена или не е указана валидна актуализационна директория.";
	else if (strcmpi(basename(my_name), "appname.exe"))
		reason = "Изпълнимият файл на САС не е със стандартното име.";
	else if (!exist(my_name))
		reason = "Изпълнимият файл на САС в текущата директория липсва или е недостъпен.";
	else if (!exist(appname))
		reason = "Изпълнимият файл на САС в актуализационната директория липсва или е недостъпен.";
	else if ((size = GetFileVersionInfoSize((char *) appname, &zero)) <= 0 ||
		size > sizeof ver_buff ||
		!GetFileVersionInfo((char *) appname, zero, sizeof ver_buff, ver_buff) ||
		!VerQueryValue(ver_buff, "\\StringFileInfo\\040904E4\\FileVersion", (void **) &file, &len) || len <= 0 ||
		!VerQueryValue(ver_buff, "\\StringFileInfo\\040904E4\\ProductVersion", (void **) &product, &len) || len <= 0 ||
		!VerQueryValue(ver_buff, "\\StringFileInfo\\040904E4\\SpecialBuild", (void **) &type, &len) || len <= 0
	)
		reason = "Информацията за версията САС в актуализационната директория липсва или е недостъпна.";
	else if (strncmp(product, "1.", 2) || strncmp(file, "1.", 2) ||
		(lm_base = atoi(product + 2)) <= 0)
		reason = "Информацията за версията САС в актуализационната директория е невалидна.";

	if (reason)
	{
		if (UPDATE_VERBOSE)
			error(reason);
		lm_base = 0;
	}
	else
	{
		// 2007:207 LPR/LRQ: warn about different court type update
		if (strcmp(APTARGET, type))
			lm_base = error("В директория %s има версия на САС за друг вид съд!", UPDATE_DIRECTORY);
		// so far, so good; check extra and clear base if bad
		else if ((file = strchr(file, '+')) == NULL)
			lm_extra = 0;
		else if ((lm_extra = atoi(file + 1)) < 0 || (lm_extra == 0 && file[1] != '0'))
		{
			if (UPDATE_VERBOSE)
				error("Информацията за точната версия на САС в актуализационната директория е непълна.");
			lm_base = 0;
		}
	}

	char my_dir[SIZE_OF_PATH];

	if (lm_base)
	{
		if (lm_base != db_base)
		{
			if (UPDATE_VERBOSE)
				error("Версията на САС в актуализационната директория не съвпада с версията на сървъра.");
			lm_base = 0;
		}
		else if (lm_base < my_base || (lm_base == my_base && lm_extra <= my_extra))
		{
			if (UPDATE_VERBOSE)
				error("Версията на САС в актуализационната директория не е по-висока от текущата версия.");
			lm_base = 0;
		}
		else
		{
			char cwd[SIZE_OF_DIR];

			if (getcwd(cwd, sizeof cwd) == NULL)
				lm_base = error("Грешка %d при вземане името на текущата директория.", errno);
			else
			{
				char *filePart;
				DWORD nCount = GetFullPathName(cwd, SIZE_OF_DIR, my_dir, &filePart);

				if (nCount >= SIZE_OF_DIR)	// not sizeof_mydir, since we need space for database name
				{
					lm_base = error("Текущата директория на САС е с прекалено дълго име. Преместете "
						"програмата в директория с име до %d символа.", LENGTH_OF_DIR);
				}
				else if (!nCount)
					lm_base = error("Грешка %lu при вземане пълното име на текущата директория.", GETLASTERROR);
			}
		}
	}

	if (lm_base)
	{
		if (chdir(UPDATE_DIRECTORY))
			fatal("%s: грешка %d при смяна на текущата директория.", UPDATE_DIRECTORY, errno);

		shutdown_config();
		global_shutdown();

		char tMyDir[SIZE_OF_PATH + 2];
		my_dir[0] = toupper(my_dir[0]);

		if (*my_dir >= 'A' && *my_dir <= 'Z' && !strcmp(my_dir + 1, ":\\Program Files\\IS - Varna\\Lawsuit Management"))
			strzcpy(tMyDir, my_dir, 3);
		else if (strchr(my_dir, ' '))
			sprintf(tMyDir, "\"%s\"", my_dir);
		else
			strcpy(tMyDir, my_dir);

		mstr start;
		bool use_powershell = (WinXPOrLater && UPDATE_POWERSHELL == 2) || (Win7OrLater && UPDATE_POWERSHELL);
		bool local_copy = UPDATE_LOCAL_COPY & (use_powershell ? 0x02 : 0x01);
		constant LMUPDATE = use_powershell ? "lmupdate.psh" : "lmupdate.exe";
		char lmupdate[SIZE_OF_PATH];

		if (!local_copy)
			strcpy(lmupdate, LMUPDATE);
		else
		{
			struct stat my_stat, lm_stat;

			build_fn(my_dir, LMUPDATE, lmupdate);

			if ((stat(lmupdate, &my_stat) || stat(LMUPDATE, &lm_stat) || my_stat.st_mtime < lm_stat.st_mtime) &&
				!copy_file(LMUPDATE, lmupdate))
			{
				error("%s: грешка при копиране в %s\n\nЩе бъде извършен опит за актуализация без локално копиране.",
					LMUPDATE, lmupdate);
				strcpy(lmupdate, LMUPDATE);
			}
		}

		if (!use_powershell)
		{
			execl(lmupdate, "lmupdate", tMyDir, str(start), NULL);
			fatal("%s: грешка %d при стартиране.", LMUPDATE, errno);
		}
		else if (!check_pshell_md5sum(NULL, lmupdate))
			fatal("%s: некоректна контролна сума.", lmupdate);
		else
		{
			if (strchr(my_dir, '\''))
				fatal("При актуализация с PowerShell, директорията на САС не може да съдържа апострофи.");

			static constant POWERSHELL = "powershell.exe";
			mstr m;

			m.printf("$ErrorActionPreference = 'Stop' ; $path = '%s' ; $wait = '%ld' ; ", my_dir, UPDATE_WAIT);
			m.cat("trap { if ($logfile) { $logfile.WriteLine($_.exception) ; $logfile.Close() } "
				"Write-Host $_.exception -foregroundcolor 'Red' ; Write-Host -NoNewline "
				"'Press any key to continue...' ; $x = $host.UI.RawUI.ReadKey('NoEcho,IncludeKeyUp') ; "
				"Write-Host '' ; Exit 1 } ");
			m.printf("Get-Content '%s' | Invoke-Expression", lmupdate);
			execlp(POWERSHELL, "PowerShell", "-Command", str(m), NULL);
			fatal("%s: грешка %d при стартиране.", POWERSHELL, errno);
		}
	}

	if (my_base != db_base)
	{
	#if TESTVER || SPEEDTESTING
		error("Версиите на клиента и сървъра на %s не съвпадат!\n"
			"\n"
			"Тестова версия - можете да продължите.", AppNameName);
	#else  // TESTVER || SPEEDTESTING
		return error("ВНИМАНИЕ! Версиите на клиента и сървъра на %s не съвпадат!\n"
			"\n"
			"За да продължите работа е необходимо да се извърши актуализация.", AppNameName);
	#endif  // TESTVER || SPEEDTESTING
	}

	return true;
}

static bool check_serials()
{
	bool entered = false;

	while (!SERIAL_VALID)
	{
		if (is_sysadmin())
		{
			TConfigData configData(CONFIG_NAME_GLOBAL);

			error("Грешка %d при обработка на серийния номер. Въведете валиден сериен номер.", SERIAL_ERROR);
			strcpy(configData.section, "serial");
			strcpy(configData.name, "SERIAL_NO");
			strcpy(configData.value, "");
			if (!configData.Try() && !configData.Insert())
				return false;

			entered = edit_config_data(NULL, &configData, NULL);
			if (!entered)
				return false;
		}
		else if (!ask("Грешка %d при обработка на серийния номер. Изчакайте администратора на системата да "
			"отстрани проблема.\n\nЖелаете ли да опитате отново?", SERIAL_ERROR))
		{
			return false;
		}

		Sleep(250);
		reload_config();
	}

	if (entered)
		create_sender_courts();

	serial_uclp_changed();
	return true;
}

static const UINT CP_REQUIRED = 1251;

class DtorHinst
{
public:
	DtorHinst(const char *const tName, HINSTANCE tHandle) : name(tName), handle(tHandle) { }
	~DtorHinst();

private:
	const char *const name;
	HINSTANCE const handle;
};

DtorHinst::~DtorHinst()
{
	if (handle && !FreeLibrary(handle))
		error("Грешка %lu при освобождаване на %s.", GETLASTERROR, name);
}

class CDtorHrOle
{
public:
	CDtorHrOle();
	~CDtorHrOle();

private:
	HRESULT hrOleInitialize;
};

CDtorHrOle::CDtorHrOle()
	: hrOleInitialize(OleInitialize(NULL))
{
	if (hrOleInitialize != S_OK)
		error("Грешка %lx при инициализация на OLE.", (unsigned long) hrOleInitialize);
}

CDtorHrOle::~CDtorHrOle()
{
	if (hrOleInitialize == S_OK)
		OleUninitialize();
}

static int CacMain(const char *my_name, bool change_dir)
{
	STARTUPINFO startup;
	int result = EXIT_SUCCESS;

	set_new_handler(out_of_memory);

	GetStartupInfo(&startup);
	MainShowWindow = startup.wShowWindow;

	static constant ThemeDllName = "uxtheme.dll";
	HINSTANCE const ThemeDllHandle = LoadLibrary(ThemeDllName);
	DtorHinst ThemeDht(ThemeDllName, ThemeDllHandle);

	if (ThemeDllHandle)
	{
		SetThemeAppProperties = (void WINAPI (*)(DWORD)) GetProcAddress(ThemeDllHandle, "SetThemeAppProperties");
		// 2010:102 LPR: using short tab controls instead
		EnableThemeDialogTexture = (HRESULT WINAPI (*)(HWND, DWORD)) GetProcAddress(ThemeDllHandle,
			"EnableThemeDialogTexture");
	}

	if (GetACP() != CP_REQUIRED)
		fatal("Code page %u required.\nИзисква се кодова страница %u.", CP_REQUIRED, CP_REQUIRED);

	static constant KernelDllName = "kernel32.dll";
	HINSTANCE KernelDllHandle = GetModuleHandle(KernelDllName);

	if (KernelDllHandle)
	{
		BOOL WINAPI (*IsWow64Process)(HANDLE, PBOOL);
		BOOL Wow64Process;

		IsWow64Process = (BOOL WINAPI (*)(HANDLE, PBOOL)) GetProcAddress(KernelDllHandle, "IsWow64Process");
		Windows64Bit = IsWow64Process && IsWow64Process(GetCurrentProcess(), &Wow64Process) && Wow64Process;
		GetProcessId = (DWORD WINAPI (*)(HANDLE)) GetProcAddress(KernelDllHandle, "GetProcessId");
	}

	// 2015:030
	static constant NetDllName = "netapi32.dll";
	HINSTANCE NetDllHandle = GetModuleHandle(NetDllName);

	if (NetDllHandle)
	{
		NetValidateName = (NET_API_STATUS WINAPI (*)(LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, NETSETUP_NAME_TYPE))
			GetProcAddress(NetDllHandle, "NetValidateName");
		DsGetDcNameA = (DWORD WINAPI (*)(LPCSTR, LPCSTR, GUID *, LPCSTR, ULONG, PDOMAIN_CONTROLLER_INFOA *))
			GetProcAddress(NetDllHandle, "DsGetDcNameA");
	}

	// 2017:292
	static constant ShlApiDllName = "shlwapi.dll";
	HINSTANCE const ShlApiDllHandle = LoadLibrary(ShlApiDllName);
	DtorHinst ShlApiDht(ShlApiDllName, ShlApiDllHandle);

	if (ShlApiDllHandle)
		PathIsRelativeA = (BOOL WINAPI (*)(LPCTSTR)) GetProcAddress(ShlApiDllHandle, "PathIsRelativeA");

	CDtorHrOle cdOle;
	CDate::InitDates();

	if (VersionOK())
	{
		TAppNameApp AppNameApp(AppNameName);

		{
			TWaitWindow wait(NULL, "Връзка към базата данни...");
			initialize_tmp();
			initialize_base();
			initialize_dates();

			if (!strcmp(tempdir, TEMPDIR))
				message(MB_ICONWARNING | MB_OK, "ВНИМАНИЕ! Използва се локалната директория за временни файлове.");
			initialize_config();
			randomize();
			_COLLEGE = DEPARTMENT ? "отделение" : "колегия";
		}

		if (change_dir)
		{
			mstr m(my_name);
			const char *my_dir = str(m);	// m never shrinks

			m.cut(basename(my_dir));

			if (chdir(my_dir))
				error("%s: грешка %d при смяна на текущата директория.", my_dir, errno);
		}

		if (check_database(my_name))
		{
			initialize_const();
			initialize_eispp();
		#if TESTVER
			// 2018-03-12: LPR: moved to update_mini()
		#else  // TESTVER
			check_config_dates();
		#endif  // TESTVER

			{
				TWaitWindow wait(NULL, "Зареждане на номенклатури...");
				initialize_noms();
			}

			long layout = get_layout();

			if (layout == LAYOUT_EN_US)
				layout = HKL_NEXT;

			if (get_login(NULL) && check_serials())
			{
				TPosTerm::Initialize();
				set_layout(layout);
				reset_cache_data();
				AppNameApp.SetName(AppNameTitle);
				initialize_extra();

				result = AppNameApp.Run();
				log_inout("Край", Default->ucn);
				TPosTerm::Shutdown();
			}

			shutdown_noms();
			shutdown_const();
		}

		shutdown_config();
		shutdown_dates();
		shutdown_base();
		shutdown_tmp();
	}

	SetPrimaryGroup(NULL);
	return result;
}

int OwlMain(int argc, char *argv[])
{
	test_rtl();

	// no argv error messages: cp1251 is not guaranteed

	int result;

	try
	{
		result = CacMain(argc >= 1 ? argv[0] : "", argc >= 2 && !strcmpi(argv[1], "/chdir"));
	}
	catch (...)
	{
		result = EXIT_FAILURE;
	}

	AppMainWindow = NULL;

	if (!global_shutdown())
		result = EXIT_FAILURE;

	return result;
}
