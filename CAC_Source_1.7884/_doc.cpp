#include "cac.h"

bool operator==(const TSideWind &s0, const TSideWind &s1)
{
  return (compare_side_winds(s0, s1) == 0);
}

// ------------------------- TRegisteredMail - begin -------------------------

TRegisteredMail::TRegisteredMail():
  TUCNGroup("T_REGISTERED_MAIL", NULL)
{
	Add(
		NEW_RCDKEY("F_DOC_", &doc),
		new TDate("F_DATE", &date, true),
		new TLong("F_LETTER_NO", &letterNo),

		new TLongAlias("F_UCLP", &uclp),
		new TLongAlias("F_STREET", &street),
		new TString("F_ADR_NO", adrNo, sizeof adrNo),
		new TString("F_SUB_NO", subNo, sizeof subNo),
		new TString("F_ENTRANCE", entrance, sizeof entrance),
		new TString("F_FLOOR", floor, sizeof floor),
		new TString("F_APARTMENT", apartment, sizeof apartment),
		new TString("F_BLOCK_NO", blockNo, sizeof blockNo),

		new TCharAlias("F_LAWSUIT_TYPE", &lawsuitType),
		new TString("F_TEXT", text, sizeof(text)),
		new TLong("F_FLAGS", &flags),

		new TLong("F_ENVELOPE_NO", &envelopeNo),
		new TChar("F_STATUS", &status),
		new TDate("F_SEND", &send, false),
		new TString("F_BARCODE", barcode, sizeof barcode),
		NULL
	);

	descs = Descs + TABLE_REGISTERED_MAIL;
	status = REGISTERED_MAIL_PREPARED;
}

bool TRegisteredMail::KeyOk() const
{
  const char* const methodName = "KeyOk";
  const bool docOk = TRCDKey_(doc).FilledOK();
  const bool dateOk = date.Valid();
  const bool letterNoOk = letterNo > 0;
  const bool ucnTypeOk = (ucnType != '\0');
  const bool ucnOk =
    ucn[sizeof(ucn) - 1] == '\0' && strlen(ucn) == (sizeof(ucn) - 1);
  const bool res = (docOk && dateOk && letterNoOk && ucnTypeOk && ucnOk);

  if(res == false)
  {
    error("%s::%s failed:\n"
      "doc: %s,\n"
      "date: %s,\n"
      "letterNo: %s,\n"
      "ucnType: %s,\n"
      "ucn: %s",
      typeid(*this).name(), methodName,
      docOk ? "Ok" : "Invalid",
      dateOk ? "Ok" : "Invalid",
      letterNoOk ? "Ok" : "Invalid",
      ucnTypeOk ? "Ok" : "Invalid",
      ucnOk ? "Ok" : "Invalid"
      );
  }
  return res;
}

bool TRegisteredMail::GenerateLetterNo()
{
  const char* const methodName = "GenerateLetterNo";
  bool res = false;

  if(letterNo <= 0)
  {
    letterNo = 1;
    if(KeyOk())
    {  // The desired case - only letterNo was missing
      char crit[576];

	letterNo = -1;
      ostrstream(crit, sizeof(crit))
        << "SELECT MAX(F_LETTER_NO) FROM " << Name << " WHERE "
        << "F_DOC_TYPE = '" << doc.type << "' AND F_DOC_YEAR = " << doc.year
	  << " AND F_DOC_NO = " << doc.no << " AND F_DATE = " << date.Value()
        << " AND F_UCN = '" << ucn << "' AND F_UCN_TYPE = '" << ucnType
        << "'" << ";" << ends;
      ENDING_ZERO(crit);
      if(criteria_finished_ok(crit, typeid(*this).name(), methodName))
      {
        TQuery q(crit);

        if(q.Read())
        {
	    {
		TDummyGroup g;

		g.Add(new TLong("MAX", &letterNo));
		g << q;
	    }
	    letterNo++;
	    if(0 < letterNo && letterNo <= NO_MAX)
          {  // Ok! Here it is.
            res = true;
          }
          else
          {
            letterNo = -1;
            error("%s::%s\n%s",
              typeid(*this).name(), methodName,
              "unable to generate the next letter number");
          }
        }
      }
    }
    else
      error("%s\n%s::%s",
        WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
  if(res == false)
    error("%s::%s\n%s",
      typeid(*this).name(), methodName, UNSUCCESSFUL_OPERATION);
  return res;
}  // TRegisteredMail::GenerateLetterNo

bool TRegisteredMail::Insert()
{
  const char* const methodName = "Insert";
  bool res = false;

  if(letterNo <= 0)
  {
	if(GenerateLetterNo())
	{
	  res = TGroup::Insert();
	}
  }
  else
    res = TGroup::Insert();
  if(res == false)
    error("%s::%s\n%s",
	typeid(*this).name(), methodName, UNSUCCESSFUL_OPERATION);
  return res;
}  // TRegisteredMail::Insert

void TRegisteredMail::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_DOC_TYPE, F_DOC_YEAR, F_DOC_NO, F_DATE, F_UCN, F_UCN_TYPE, F_LETTER_NO");
}

void TRegisteredMail::FloatCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_DATE");
}

void TRegisteredMail::Print(char* const dest, const size_t destSz) const
{
  if(dest && destSz)
  {
    PrintDoc(dest, destSz);
    strConcat(dest, " ", destSz);
    {
      size_t l = strlen(dest);

      PrintUclp(dest + l, destSz - l);
      strConcat(dest, " ", destSz);
      l = strlen(dest);
      PrintReceiver(dest + l, destSz - l);
    }
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), "Print");
}  // TRegisteredMail::Print

void TRegisteredMail::Print(mstr &m)
{
	char s[0x400];
	TRegisteredMail::Print(s, sizeof s);
	m.cat(s);
}

void TRegisteredMail::PrintDoc(char* const dest, const size_t destSz) const
{
  const char* const methodName = "PrintDoc";
  if(dest && destSz)
  {
    if(doc.type == TYPE_OUTREG)
    {
	ostrstream(dest, destSz)
        << "Изх. No " << doc.no << "/" << doc.year << "г." << ends;
      dest[destSz - 1] = '\0';
      if(TRCDKey_(doc).FilledOK())
      {  // Look for related stuff...
        TOutReg outReg;
        char docTxt[64];

	  TRCDKey_(doc).PrintAlias(docTxt, sizeof(docTxt));
        outReg.key = doc;
        if(outReg.Get())
        {
          SessPtr pSess;  // I'll use &(pSess.Ref().key) also as pLawKey below.
          TRCDKey reqKey;
          TRCDKeyContainer_ surrKeyData;
          const char realLawType = outreg_to_law_type(outReg,
            &(pSess.Ref().key), pSess.Ptr(), &surrKeyData, &reqKey, false);

          if(realLawType)
          {
            const char* const errFmt =
              "%s::%s\n"
              "Видът дело, за който се отнася %s "
              "не съответствува на обявения в %s.";

            if(TRCDKey_(pSess.Ref().key).FilledOK())
            {  // Lawsuit or session
              if(lawsuitType == pSess.Ref().key.type)
              {
                strConcatCh(dest, BONEVA_SIDES_DELIMITER, destSz);
                {
                  const size_t l = strlen(dest);

                  TRCDKey_(pSess.Ref().key).PrintAlias(dest + l, destSz - l);
                }
              }
              else
                error(errFmt, typeid(*this).name(), methodName, docTxt, Name);
            }
            else
              if(TRCDKey_(reqKey).FilledOK())
              {  // Request
                if(lawsuitType == realLawType)
                {
                  strConcatCh(dest, BONEVA_SIDES_DELIMITER, destSz);
                  {
                    const size_t l = strlen(dest);

                    TRCDKey_(reqKey).PrintAlias(dest + l, destSz - l);
                  }
                }
                else
                  error(errFmt, typeid(*this).name(), methodName, docTxt,
                    Name);
              }
              else
                if(surrKeyData.FilledOK())
                {
                  if(lawsuitType == realLawType)
                  {  // Surround
                    strConcatCh(dest, BONEVA_SIDES_DELIMITER, destSz);
                    {
                      const size_t l = strlen(dest);

                      surrKeyData.PrintAlias(dest + l, destSz - l);
                    }
                  }
                  else
			  error(errFmt, typeid(*this).name(), methodName, docTxt,
			    Name);
                }
          }
        }
      }
    }
    else
      TRCDKey_(doc).PrintAlias(dest, destSz);
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), "PrintDoc");
}  // TRegisteredMail::PrintDoc

void TRegisteredMail::PrintUclp(char* const dest, const size_t destSz) const
{
  if(dest && destSz)
  {
    if(uclp)
    {
	uclpToName(uclp, dest, destSz);
    }
    else
    {
	ZERO_LEN(dest);
    }
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), "PrintUclp");
}  // TRegisteredMail::PrintUclp

void TRegisteredMail::PrintReceiver(char* const dest, const size_t destSz) const
{
  if(dest && destSz)
  {
    dest[0] = '\0';
    {
      TAddress addr;

      addr.uclp = uclp;
      addr.street = street;
	strCopy(addr.adrNo, adrNo, sizeof(addr.adrNo));
      strCopy(addr.subNo, subNo, sizeof(addr.subNo));
      strCopy(addr.entrance, entrance, sizeof(addr.entrance));
      strCopy(addr.floor, floor, sizeof(addr.floor));
      strCopy(addr.apartment, apartment, sizeof(addr.apartment));
      strCopy(addr.blockNo, blockNo, sizeof(addr.blockNo));
      addressWithStr_SB_ToString(addr, dest, destSz);
    }
    if(dest[0])
      strConcatCh(dest, BONEVA_SIDES_DELIMITER, destSz);
    {
      const size_t l = strlen(dest);

      ucnAndUcnTypeToName(ucn, ucnType, dest + l, destSz - l);
    }
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), "PrintReceiver");
}  // TRegisteredMail::PrintReceiver

void TRegisteredMail::PrintFlags(char* const dest, const size_t destSz) const
{
  if(dest && destSz)
  {
    dest[0] = '\0';
    switch(flags)
    {
      case OUTREG_R:
        strConcat(dest, "R", destSz);
        break;
      case OUTREG_AR:
        strConcat(dest, "AR", destSz);
        break;
      case OUTREG_PACKAGE:
        strConcat(dest, "Колет", destSz);
        break;
    }
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), "PrintFlags");
}  // TRegisteredMail::PrintFlags

bool TRegisteredMail::operator==(const TRegisteredMail& other) const
{
  return(
    flags == other.flags &&  // 11.aug.2003
    TRCDKey_(doc) == TRCDKey_(other.doc) && date == other.date &&
    ucnType == other.ucnType && strcmp(ucn, other.ucn) == 0 &&
    envelopeNo == other.envelopeNo && letterNo == other.letterNo);
}  // TRegisteredMail::operator==

bool TRegisteredMail::operator<(const TRegisteredMail& other) const
{  // an old varianth - in VersionArchives\1820\Preliminary\_appname.j -->
  // _MailLst.cpp
  bool res;

  if(flags < other.flags)  // 11.aug.2003 - begin
    res = true;
  else
    if(flags > other.flags)
      res = false;
    else  // 11.aug.2003 - end
      if(ucnType < other.ucnType)  // 08.sep.2003 (order changed) - begin
        res = true;
      else
        if(ucnType > other.ucnType)
          res = false;
        else
        {
          const int ucnCmp = strcmp(ucn, other.ucn);

          if(ucnCmp < 0)
            res = true;
          else
            if(ucnCmp > 0)
              res = false;
            else  // 08.sep.2003 (order changed) - end
              if(envelopeNo < other.envelopeNo)  // 08.sep.2003 - begin
                res = true;
              else
                if(envelopeNo > other.envelopeNo)
                  res = false;
                else  // 08.sep.2003 - end
                  if(date < other.date)
                    res = true;
                  else
                    if(date > other.date)
                      res = false;
                    else
                      if(doc.type < other.doc.type)
                        res = true;
                      else
                        if(doc.type > other.doc.type)
                          res = false;
                        else
                          if(doc.year < other.doc.year)
                            res = true;
                          else
                            if(doc.year > other.doc.year)
                              res = false;
                            else
                              if(doc.no < other.doc.no)
                                res = true;
                              else
                                if(doc.no > other.doc.no)
                                  res = false;
                                else
                                  res = (letterNo < other.letterNo);
        }
  return res;
}  // TRegisteredMail::operator<

// - - - - - Keep the logic in these methods in conformity! - begin - - - - - -

bool TRegisteredMail::CanEnvelope(const bool errMsg) const
{
  const bool res =
    (flags == OUTREG_R || flags == OUTREG_AR || flags == OUTREG_PACKAGE);

  if(res == false && errMsg == true)
    error("Начинът на изпращане не позволява обединяване в общ плик.");
  return res;
}

bool TRegisteredMail::SameEnvelope(const TRegisteredMail& other) const
{
  return(MayJoinEnvelope(other, false) &&
    envelopeNo == other.envelopeNo);
}

bool TRegisteredMail::MayJoinEnvelope(const TRegisteredMail& envlpMember,
      const bool errMsg) const  // Determines if '*this'
      // object may join an envelope with the traits given.
{
  const bool can_ = CanEnvelope(errMsg);
  const bool single_ =
    (envelopeNo == 0 || envelopeNo == envlpMember.envelopeNo);
  const bool lawType_ = lawsuitType == envlpMember.lawsuitType;
  const bool receiver_ =
    (ucnType == envlpMember.ucnType && strcmp(ucn, envlpMember.ucn) == 0);
  const bool way_ = (flags == envlpMember.flags);
  const bool address_ =
    (uclp == envlpMember.uclp && street == envlpMember.street &&
    strcmp(adrNo, envlpMember.adrNo) == 0 && strcmp(subNo, envlpMember.subNo) == 0 &&
    strcmp(entrance, envlpMember.entrance) == 0 &&
    strcmp(floor, envlpMember.floor) == 0 &&
    strcmp(apartment, envlpMember.apartment) == 0 &&
    strcmp(blockNo, envlpMember.blockNo) == 0
    );
  const bool res =
    (can_ && single_ && lawType_ && receiver_ && way_ && address_);

  if(res == false && errMsg == true &&
    can_ == true  // otherwise an error messsage has already
      // displayed.
    )
  {
    const char* reason = "?";

    if(single_ == false)
      reason = "общи пликове";
    else
      if(lawType_ == false)
        reason = "видове дела";
      else
        if(receiver_ == false)
          reason = "получатели";
        else
          if(way_ == false)
            reason = "начини на изпращане";
          else
            if(address_ == false)
              reason = "адреси";
    error("Обединяването в общ плик е невъзможно "
      "(различни %s).", reason);
  }
  return res;
}

long TRegisteredMail::NextEnvelopeNo(const TRegisteredMail& pattern) const
// returns 0 on error
{
  long res = 0;
  const char* const methodName = "NextEnvelopeNo";

  if(ucnPresent(pattern.ucn) && pattern.ucnType)
  {
    char crit[512 + 256];

    {
      ostrstream oss (crit, sizeof(crit));

      oss << "SELECT MAX(F_ENVELOPE_NO) FROM T_REGISTERED_MAIL ";
      EnvelopeWhereClause(oss, pattern);
      oss << " ;" << ends;
    }
    ENDING_ZERO(crit);
    if(criteria_finished_ok(crit, methodName))
    {
      TQuery q(crit);

      if(q.Read())
      {
	  TMaxLongGroup max;
	  max << q;
        res = max.max + 1;
      }
      if(res == 0)
	  error("%s::%s\n%s",
          typeid(*this).name(), methodName, UNSUCCESSFUL_OPERATION);
    }
  }
  else
    error("%s\n%s::%s", WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
  return res;
}  // TRegisteredMail::NextEnvelopeNo

void TRegisteredMail::EnvelopeWhereClause(ostream& os,
  const TRegisteredMail& pattern) const
{
  const char* const ISNULL = " IS NULL ";
  const char* const EQ = " = ";
  const char* const AND = " AND ";

    // Nothing in the WHERE clause is absolutely necessary, including
    // the clause itself.
    // But keep conformity with MayJoinEnvelope() though.
  os << "WHERE "
    "F_UCN = '" << pattern.ucn << "'" << AND << "F_UCN_TYPE" << EQ << "'"
    << pattern.ucnType << "'";
  os << AND << "F_LAWSUIT_TYPE" << EQ << "'" << pattern.lawsuitType << "'";
  os << AND;
  os << "F_FLAGS";
  if(pattern.flags)
    os << EQ << pattern.flags;
  else
    os << ISNULL;
  os << AND;
  os << "F_UCLP";
  if(pattern.uclp)
    os << EQ << pattern.uclp;
  else
    os << ISNULL;
  os << AND;
  os << "F_STREET";
  if(pattern.street)
    os << EQ << pattern.street;
  else
    os << ISNULL;
  os << AND;
  os << "F_ADR_NO";
  if(pattern.adrNo[0])
    os << EQ << "'" << pattern.adrNo << "'";
  else
    os << ISNULL;
  os << AND;
  os << "F_SUB_NO";
  if(pattern.subNo[0])
    os << EQ << "'" << pattern.subNo << "'";
  else
    os << ISNULL;
  os << AND;
  os << "F_ENTRANCE";
  if(pattern.entrance[0])
    os << EQ << "'" << pattern.entrance << "'";
  else
    os << ISNULL;
  os << AND;
  os << "F_FLOOR";
  if(pattern.floor[0])
    os << EQ << "'" << pattern.floor << "'";
  else
    os << ISNULL;
  os << AND;
  os << "F_APARTMENT";
  if(pattern.apartment[0])
    os << EQ << "'" << pattern.apartment << "'";
  else
    os << ISNULL;
  os << AND;
  os << "F_BLOCK_NO";
  if(pattern.blockNo[0])
    os << EQ << "'" << pattern.blockNo << "'";
  else
    os << ISNULL;
}  // TRegisteredMail::EnvelopeWhereClause

// - - - - - Keep the logic in these methods in conformity! - end - - - - - -

// ------------------------- TRegisteredMail - end -------------------------
