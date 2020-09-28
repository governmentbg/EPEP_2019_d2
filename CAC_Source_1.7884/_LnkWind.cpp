#include "cac.h"

TLinkWind::TLinkWind()
	: TBaseWind("T_LINK_WIND", TLinkWind::Clone)
{
	Init();
}

TLinkWind::TLinkWind(const char *name, TGroup *(*const tClone)())
	: TBaseWind(name, tClone)
{
	Init();
}

void TLinkWind::Init()
{
	Add(
		new TCharAlias("F_PROXY_UCN_TYPE", &proxyUcnType),
		new TString("F_PROXY_UCN", proxyUcn, sizeof proxyUcn),
		new TCharAlias("F_PROXY_INVOLVEMENT", &proxyInvolvement),
		new TChar("F_PROXY_KIND", &proxyKind),
		new TCharAlias("F_REDIRECT_UCN_TYPE", &redirectUcnType),
		new TString("F_REDIRECT_UCN", redirectUcn, sizeof redirectUcn),
		new TCharAlias("F_REDIRECT_INVOLVEMENT", &redirectInvolvement),
		new TChar("F_REDIRECT_KIND", &redirectKind),
		new TChar("F_STATE", &state),
		NULL
	);

	// descs assigned in TBaseWind
	proxyKind = redirectKind = SIDE_MARK_NO_INDIRECT;
	state = STATE_NONE;
}

TGroup *TLinkWind::Clone()
{
	return new TLinkWind;
}

void TLinkWind::ExactCriteria(msql &m, const char *fields)
{
	TBaseWind::ExactCriteria(m, fields);
	m.Add(this, "F_PROXY_UCN, F_PROXY_UCN_TYPE, F_PROXY_INVOLVEMENT");
}

void TLinkWind::FloatCriteriaByOrdinary(msql &m, const char *fields)
{
	TBaseWind::ExactCriteria(m, fields);
}

void TLinkWind::SetOrdinary(const TSideWind& side)
{
  key = side.key;
  kind = side.kind;
  date = side.date;
  ucnType = side.ucnType;
  strCopy(ucn, side.ucn, sizeof(ucn));
  involvement = side.involvement;
}  // TLinkWind::SetOrdinary

void TLinkWind::SetProxy(const TSideWind& side)
{
  proxyUcnType = side.ucnType;
  strCopy(proxyUcn, side.ucn, sizeof(proxyUcn));
  proxyInvolvement = side.involvement;
}  // TLinkWind::SetProxy

void TLinkWind::SetProxyKind(const bool useOrdinarySideAddress, const int which_any) // 20070730
{
  METHOD_NAME("SetProxyKind"); // Избран "Начин на упълномощаване"

  if(proxyInvolvement == '\0')
  {
    proxyKind = SIDE_MARK_NO_INDIRECT;
    error("%s::%s\n proxyInvolvement is zero",
	typeid(*this).name(), methodName);
    return;
  }
  // 2014:216
  if (strchr(SUMMONS_FOR_SIDE_WITH_INVS, proxyInvolvement) && which_any == 7)
  {
	if (which_any == 7)
	  proxyKind = SIDE_MARK_AS_WITH_INVOLVEMENT;
	else
	  proxyKind = SIDE_MARK_LIKE_LAWYER;
  }
  else if(strchr(SUMMONS_FOR_SIDE_LEGAL_SPOK_INVS, proxyInvolvement) == NULL)
    proxyKind = SIDE_MARK_LIKE_LAWYER;
  else
    if(useOrdinarySideAddress == false)
	if (which_any == 1)
	   proxyKind = SIDE_MARK_INDIRECT_AS_ANY;
	else if (which_any == 2)
	   proxyKind = SIDE_MARK_INDIRECT_AS_ANY_1;
	else if (which_any == 3)
	   proxyKind = SIDE_MARK_INDIRECT_AS_ANY_2;
	else if (which_any == 4)
	   proxyKind = SIDE_MARK_INDIRECT_AS_ANY_3;
	else
	   proxyKind = SIDE_MARK_INDIRECT_AS_ANY;
    else
      proxyKind = SIDE_MARK_CONSENT_OF_INDIRECT;
}  // TLinkWind::SetProxyKind

void TLinkWind::SetRedirect(const TSideWind& side)
{
  redirectUcnType = side.ucnType;
  strCopy(redirectUcn, side.ucn, sizeof(redirectUcn));
  redirectInvolvement = side.involvement;
}  // TLinkWind::SetRedirect

void TLinkWind::SetRedirectKind (const bool useOrdinarySideAddress)
{
  METHOD_NAME("SetRedirectKind");

  if (redirectInvolvement == '\0')
  {
    redirectKind = SIDE_MARK_NO_INDIRECT;
    error("%s::%s\n redirectInvolvement is zero",
      typeid(*this).name(), methodName);
    return;
  }
  if(strchr(SUMMONS_FOR_SIDE_LEGAL_SPOK_INVS, redirectInvolvement) == NULL)
    redirectKind = SIDE_MARK_LIKE_LAWYER;
  else
  {
    char tmp[256];
    const char* const unsupported = " has an unsupported value";

    ostrstream(tmp, sizeof(tmp))
      << typeid(*this).name() << "::" << methodName << "\r\n"
      << "the redirectInvolvement" << unsupported << "." << ends;
    ENDING_ZERO(tmp);
    if(useOrdinarySideAddress == true)
    {
      strConcat(tmp, "\r\nThe useOrdinarySideAddress argument", sizeof(tmp));
      strConcat(tmp, unsupported, sizeof(tmp));
      strConcat(tmp, ", too.", sizeof(tmp));
    }
    fatal(tmp);
  }
}  // TLinkWind::SetRedirectKind

void TLinkWind::ClearRedirection()
{
  redirectKind = SIDE_MARK_NO_INDIRECT;
  redirectUcnType = redirectUcn [0] = redirectInvolvement = '\0';
}  // TLinkWind::ClearRedirection

void TLinkWind::GetOrdinary(TSideWind& side) const
{
  side.key = key;
  side.kind = kind;
  side.date = date;
  side.ucnType = ucnType;
  strCopy(side.ucn, ucn, sizeof(side.ucn));
  side.involvement = involvement;
}  // TLinkWind::GetOrdinary

void TLinkWind::GetProxy(TSideWind& side) const
{
  side.key = key;
  side.kind = kind;
  side.date = date;
  side.ucnType = proxyUcnType;
  strCopy(side.ucn, proxyUcn, sizeof(side.ucn));
  side.involvement = proxyInvolvement;
}  // TLinkWind::GetProxy

void TLinkWind::GetRedirect(TSideWind& side) const
{
  side.key = key;
  side.kind = kind;
  side.date = date;
  side.ucnType = redirectUcnType;
  strCopy(side.ucn, redirectUcn, sizeof(side.ucn));
  side.involvement = redirectInvolvement;
}  // TLinkWind::GetRedirect

bool TLinkWind::GetAddressSide(TSideWind& side) const
{
  METHOD_NAME("GetAddressSide");
  bool res = false;

  clearGroupData(side);
  side.state = STATE_NONE;	// 2016:203 LPR: FIX 

  if(Redirected())
  {
    if(redirectKind == SIDE_MARK_LIKE_LAWYER)
    {
      GetRedirect(side);
      res = true;
    }
    else
    {
      const char* const unsupported = " has an unsupported value";

      error("%s::%s\n The redirectKind%s.",
        typeid(*this).name(), methodName, unsupported);
    }
  }
  else
  {
    if(Proxied())
    {
      if(proxyKind == SIDE_MARK_CONSENT_OF_INDIRECT)
      {
        GetOrdinary(side);
        res = true;
      }
      else
      {
        GetProxy(side);
        res = true;
      }
    }
    else
    {
      GetOrdinary(side);
      res = true;
    }
  }
  if(res == false)
  {
    error("%s::%s\n%s",
      typeid(*this).name(), methodName, UNSUCCESSFUL_OPERATION);
  }
  return res;
}  // TLinkWind::GetAddressSide

void TLinkWind::Print(mstr &m)
{
	char s[0x400];
	TLinkWind::Print(s, sizeof s);
	m.cat(s);
}

void TLinkWind::Print(char* s, const size_t sSz) const
{
  METHOD_NAME("Print");

  if(s && sSz > 1)
  {
    side_link_to_string (s, sSz, * this, false, false) ;
  }
  else
    error("%s\n%s::%s",
	WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
}  // TLinkWind::Print

bool TLinkWind::Proxied() const
{
  return proxyUcnType && ucnPresent(proxyUcn) && proxyInvolvement;
}

bool TLinkWind::Redirected() const
{
  return redirectUcnType && ucnPresent(redirectUcn) && redirectInvolvement;
}  // TLinkWind::Redirected

bool TLinkWind::FilledOk () const
{
  return(Name && Name[0] && DatedContainer_(key, kind, date).FilledOK() &&
    ucn[0] && ucnType && involvement && Proxied()) ;
}  // TLinkWind::FilledOK

bool TLinkWind::operator==(const TLinkWind &that) const
{
	return key == that.key && kind == that.kind && date == that.date && involvement == that.involvement &&
		ucnType == that.ucnType && strcmp(ucn, that.ucn) == 0 && proxyInvolvement == that.proxyInvolvement &&
		proxyUcnType == that.proxyUcnType && strcmp(proxyUcn, that.proxyUcn) == 0;
}

#define compret(field) if (field != that.field) return field < that.field
#define compstr(field) if (strcmp(field, that.field)) return strcmp(field, that.field) < 0

bool TLinkWind::operator<(const TLinkWind& that) const
{
	compret(key);
	compret(kind);
	compret(date);
	compret(involvement);
	compret(ucnType);
	compstr(ucn);
	compret(proxyInvolvement);
	compret(proxyUcnType);
	return strcmp(proxyUcn, that.proxyUcn) < 0;
}

bool TLinkWind::SameLink(const TLinkWind *that) const
{
	return SameWind(that) && proxyUcnType == that->proxyUcnType && proxyInvolvement == that->proxyInvolvement &&
		!strcmp(proxyUcn, that->proxyUcn);
}

bool TLinkWind::Contains(const TSideWind *sideWind) const
{
	TSideWind proxy, redirect;

	GetProxy(proxy);
	GetRedirect(redirect);
	return sideWind->SameWind(this) || sideWind->SameWind(proxy) || sideWind->SameWind(redirect);
}

// ----- TLinkWindArray ------------------------------------------------------
TLinkWindArray::TLinkWindArray(TSideWindArray *tSideWinds)
	: sideWinds(tSideWinds)
{
	lesseql = LessEqlT;
}

int TLinkWindArray::Add(TLinkWind *linkWind)
{
	FixKeys(linkWind);
	return TGroupArray::Add(linkWind);
}

TLinkWind *TLinkWindArray::FindLink(const TLinkWind *linkWind) const
{
	for (int i = 0; i < Count(); i++)
		if ((*this)[i]->SameLink(linkWind))
			return (*this)[i];

	return NULL;
}

TSideWind *TLinkWindArray::FindSide(const char *ucn, char ucnType, char involvement) const
{
	for (int i = 0; i < sideWinds->Count(); i++)
	{
		TSideWind *sideWind = (*sideWinds)[i];

		if (sideWind->ucnType == ucnType && sideWind->involvement == involvement && !strcmp(sideWind->ucn, ucn))
			return sideWind;
	}

	return NULL;
}

void TLinkWindArray::FixKeys(TLinkWind *linkWind)
{
	TSideWind *sideWind = FindSide(linkWind->ucn, linkWind->ucnType, linkWind->involvement);

	linkWind->ordinaryKey = sideWind ? sideWind->Key() : 0;
	sideWind = FindSide(linkWind->proxyUcn, linkWind->proxyUcnType, linkWind->proxyInvolvement);
	linkWind->proxyKey = sideWind ? sideWind->Key() : 0;
}

void TLinkWindArray::Insert(TLinkWind *linkWind, int index)
{
	FixKeys(linkWind);
	TGroupArray::Insert(linkWind, index);
}

bool TLinkWindArray::LessEqlT(const long t1, const long t2)
{
	TLinkWind *l1 = (TLinkWind *) t1;
	TLinkWind *l2 = (TLinkWind *) t2;
	return l1->ordinaryKey != l2->ordinaryKey ? l1->ordinaryKey < l2->ordinaryKey : l1->proxyKey <= l2->proxyKey;
}
