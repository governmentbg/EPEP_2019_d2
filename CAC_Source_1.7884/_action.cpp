#include "cac.h"

// --------------------- TChooseLinkDialog - end -------------------

class TChooseLinkDialog: public TScaleDialog
{
public:  // Construction / destruction
  TChooseLinkDialog(TWindow* parent, const TGArray<TLinkWind>* pSideLinksArr,
    int& choosenInd, const char* const prefix);
public:  // Methods
  virtual void SetupWindow();
protected:  // Methods
  void OKBNClicked();
protected:  // Data
  const TGArray<TLinkWind>* pSideLinksArray;
  int& choosenIndex;
  const char* const promptPrefix;
  TStringListBox* pLinks;
  TStatic* pPrompt;
DECLARE_RESPONSE_TABLE(TChooseLinkDialog);
};  // class TChooseLinkDialog


DEFINE_RESPONSE_TABLE1(TChooseLinkDialog, TScaleDialog)
  EV_BN_CLICKED(IDOK, OKBNClicked),
END_RESPONSE_TABLE;

TChooseLinkDialog::TChooseLinkDialog(TWindow* parent,
  const TGArray<TLinkWind>* pSideLinksArr, int& choosenInd,
  const char* const prefix):
  TScaleDialog(parent, IDD_CHOOSE_LINK),
  pSideLinksArray(pSideLinksArr), choosenIndex(choosenInd), promptPrefix(prefix)
{
  pLinks = new TStringListBox(this, IDC_LINK);
  pPrompt = new TStatic(this, IDC_LINK_PROMPT, 128);
  if(pSideLinksArray == NULL)
  {
    METHOD_NAME(typeid(*this).name());
    REPORT_M_INVALID_ARGS;
  }
}  // TChooseLinkDialog::TChooseLinkDialog


void TChooseLinkDialog::SetupWindow()
{
  TScaleDialog::SetupWindow();

  char ucnT = '\0';
  const char* ucn = "";

  {
    TSideWind s;

    if(pSideLinksArray)
    {
	if(pSideLinksArray->Count())
	{
	  for (int i = 0; i < pSideLinksArray->Count(); i++)
	  {
	    char s[0x200];
	    side_link_to_string(s, sizeof s, *(*pSideLinksArray)[i], false, false);
	    pLinks->AddString(s);
	  }
	  //links_to_list_box(* pSideLinksArray, * pLinks);

        (* pSideLinksArray) [0]->GetOrdinary(s);
        ucnT = s.ucnType;
        ucn = s.ucn;
      }
    }
  }
  if(ucnT && ucnPresent(ucn))
  {
    CharPtr promptTxt(pPrompt->GetTextLimit());

    if(promptPrefix)
      scSprintf(promptTxt.Size(), promptTxt, "%s ", promptPrefix);
    {
      size_t l = strlen(promptTxt);

      scSprintf(promptTxt.Size() - l, promptTxt + l, "%s ", ucn);
      l = strlen(promptTxt);
      ucnAndUcnTypeToName(ucn, ucnT, promptTxt + l, promptTxt.Size() - l);
    }
    pPrompt->SetText(promptTxt);
    pLinks->SetHorizExtent();
  }
}  // TChooseLinkDialog::SetupWindow


void TChooseLinkDialog::OKBNClicked()
{
  if(pLinks->GetCount() > 0)
  {
    int chsInd = pLinks->GetSelIndex();

    if(chsInd >= 0)
    {
      choosenIndex = chsInd;
      CmOk();
    }
    else
      say_no_selection();
  }
  else
    CmCancel();
}  // TChooseLinkDialog::OKBNClicked

// --------------------- TChooseLinkDialog - begin -------------------

// --------------------- choose_link_wind - begin -------------------

bool choose_link_wind(TWindow* parent, TGArray<TLinkWind>& walnLinks,
  int& choosenInd, const char* const prefix)
{
  const int items = walnLinks.Count();
  bool res = false;

  if(items > 1)
    res = TChooseLinkDialog(parent, &walnLinks, choosenInd, prefix).
      Execute() == IDOK;
  else
    if(items == 1)
    {
      choosenInd = 0;
      res = true;
    }
  return res;
}  // choose_link_wind

bool inreg_side(TWindow* parent, TInReg* pInReg, TSideWind* pSide)
{
  FUNC_NAME("inreg_side");
  bool res = false;

  if(pInReg && pSide)
  {  // -- 0 --
    if(pSide->state == STATE_NONE ||
       pSide->state == STATE_HOLD ||  // 21.feb.2001  // We lose the
          // STATE_HOLD information for this dated side
       pSide->state == STATE_APPEALER || pSide->state == STATE_INDIRECT)
    {  // -- 0.5 --
      if(pSide->state == STATE_APPEALER)
      {  // Direct appealer --> non-appealer.
        pSide->state = STATE_NONE;
        res = true;
      }
      else
      {  // -- 1 --
        TGArray<TLinkWind> links;

        if(inreg_to_link_winds(pInReg, links))
        {  // -- 2 --
          TGArray<TLinkWind> thisSideLinks;

          if(pSide->state == STATE_INDIRECT)
          {  // -- 3 -- Indirect appealer --> non-appealer.
            t_to_T_links(*pSide, links, thisSideLinks, false);
		const int cnt = thisSideLinks.Count();

            for(int i = 0; i < cnt; i++)
            {
              thisSideLinks[i]->state = STATE_NONE;
            }
            pSide->state = STATE_NONE;
            res = true;
          }  // -- 3 --
          else
          {  // -- 4 -- Non-appealer --> Direct/indirect appealer.
            int choosenInd = 0;

            t_to_T_links(*pSide, links, thisSideLinks, true);
            {  // -- 5 --
		  const int cnt = thisSideLinks.Count();

              if(cnt > 0)
              {  // -- 6 --
                if(choose_link_wind(parent, thisSideLinks, choosenInd,
                  "Подаване на жалба от") == true)
                {  // -- 7 --
                  if(choosenInd == 0)
                  {  // Non-appealer --> Direct appealer.
                    pSide->state = STATE_APPEALER;
                    res = true;
			}
                  else
                  {  // -- 8 -- Non-appealer --> Indirect appealer.
                    if(1 <= choosenInd && choosenInd < cnt)
                    {
                      thisSideLinks[choosenInd]->state = STATE_INDIRECT;
			    if(thisSideLinks[choosenInd]->Update("F_STATE") == true)
			    {
                        pSide->state = STATE_INDIRECT;
                        res = true;
                      }
                    }
                  }  // -- 8 --
                }  // -- 7 --
              }  // -- 6 --
            }  // -- 5 --
          }  // -- 4 --
        }  // -- 2 --
      }  // -- 1 --
    }  // -- 0.5 --
    else
      error(WRONG_DATED_SIDE_STATE);
  }  // -- 0 --
  else
    REPORT_F_INVALID_ARGS;
  return res;
}  // inreg_side
