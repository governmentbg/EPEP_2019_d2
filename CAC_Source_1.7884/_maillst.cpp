#include "cac.h"

# define WM_LWN_VSCROLLED (WM_USER+130)

# define WM_BN_RIGHTCLICKED (WM_USER+150)

// ------------------------- TMailType - begin -------------------------

class TMailType : public TGroup
{
  public:  // Construction / Destruction
	  TMailType();

  public:  // Methods
	virtual void ExactCriteria(msql &m, const char *fields = NULL);

    static bool LoadAll(char* const dest, const size_t destSz,
	const bool lawTypesByPights = true);

  public:  // Data
    char mailType;
    char name[SIZE_OF_NAME];
    char alias[SIZE_OF_ALIAS];
};

TMailType::TMailType():
  TGroup("T_MAIL_TYPE", NULL)
{
	Add(
		new TChar("F_MAIL_TYPE", &mailType),
		new TString("F_NAME", name, sizeof name),
		new TString("F_ALIAS", alias, sizeof alias),
		NULL
	);

	descs = Descs + TABLE_MAIL_TYPE;
}

void TMailType::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_MAIL_TYPE");
	m.cat(" ");
}

bool TMailType::LoadAll(char* const dest, const size_t destSz,
  const bool lawTypesByPights)
{
  const char* const methodName = "TMailType::LoadAll";
  bool rT = true;

  if(dest && destSz)
  {
    char crit[128];

    dest[0] = '\0';
    ostrstream(crit, sizeof(crit))
      << "SELECT F_MAIL_TYPE FROM " << "T_MAIL_TYPE" << ";" << ends;
    if(criteria_finished_ok(crit, methodName))
    {
      TCountRecsQuery q(crit);
      TMailType mt;
      int elements = 0;

      while(q.Read())
      {
        mt << q;
        if(mt.mailType)
        {
          if(lawTypesByPights == false ||
            strchr(TYPE_LAWSUITS, mt.mailType) == NULL ||
            strchr(Default->colleges, mt.mailType))
          {
            strConcatCh(dest, mt.mailType, destSz);
            elements++;
          }
        }
      }
      if(strlen(dest) != elements)  // q.getRecsCount()  13.aug.2003
        rT = false;
      if(dest[0] == '\0')
        rT = false;
    }
    else
      rT = false;
    dest[destSz - 1] = '\0';
  }
  else
    rT = error("%s\n%s",
      WRONG_ARGS_PASSED_TO, methodName);
  return rT;
}  // TMailType::LoadAll

// ------------------------- TMailType - end -------------------------

// ------------------------- MailListDlgData - begin -------------------------

struct MailListDlgData
{
  MailListDlgData();
  void Clear();
  void CopyTo(begNEndNYearTypesAC& target) const;
  void CopyFrom(const begNEndNYearTypesAC& source);
  char mailTypes[SIZE_OF_EXT_TYPES];
  CDate begDate;
  CDate endDate;
  CDate sent;
  bool wantCheckup;
};  // struct MailListDlgData

MailListDlgData::MailListDlgData()
{
  Clear();
};  // MailListDlgData::MailListDlgData

void MailListDlgData::Clear()
{
  mailTypes[0] = '\0';
  clear_CDate(begDate);
  clear_CDate(endDate);
  clear_CDate(sent);
  wantCheckup = false;
}  // MailListDlgData::Clear

void MailListDlgData::CopyTo(begNEndNYearTypesAC& target) const
{
  target.clear();
  target.begDate = begDate;
  target.endDate = endDate;

  // Messy - begin
  target.begN = sent.day;
  target.endN = sent.month;
  target.year = sent.year;
  // Messy - end

  target.wantCheckup = wantCheckup;
  strCopy(target.types, mailTypes, sizeof(target.types));
  if(strlen(target.types) < strlen(mailTypes))
    error("MailListDlgData::CopyTo\n%s", UNSUCCESSFUL_OPERATION);
}  // MailListDlgData::CopyTo

void MailListDlgData::CopyFrom(const begNEndNYearTypesAC& source)
{
  Clear();
  begDate = source.begDate;
  endDate = source.endDate;

  // Messy - begin
  sent.day = (char)(source.begN);
  sent.month = (char)(source.endN);
  sent.year = source.year;
  // Messy - end

  wantCheckup = source.wantCheckup;
  strCopy(mailTypes, source.types, sizeof(mailTypes));
  if(strlen(mailTypes) < strlen(source.types) ||
    (sent.Empty() == false && sent.Valid() == false))
    error("MailListDlgData::CopyFrom\n%s", UNSUCCESSFUL_OPERATION);
}  // MailListDlgData::CopyFrom

// ------------------------- MailListDlgData - end -------------------------

// ------------------------- TOrdinaryMail - begin -------------------------

class TOrdinaryMail: public TDummyGroup
{
  public:  // Construction / Destruction
    TOrdinaryMail();
  public:  // Methods
	virtual void ExactCriteria(msql &m, const char *fields = NULL);
  public:  // Data
    CDate date;
    char mailType;
    long quantity;
};  // class TOrdinaryLetter

TOrdinaryMail::TOrdinaryMail()
{
	Add(
		new TDate("F_DATE", &date, false),
		new TChar("F_MAIL_TYPE", &mailType),
		new TLong("F_QUANTITY", &quantity),
		NULL
	);
}

void TOrdinaryMail::ExactCriteria(msql &m, const char *fields)
{
	m.Begin(Name, fields);
	m.Add(this, "F_DATE, F_MAIL_TYPE");
	m.cat(" ");
}

// ------------------------- TOrdinaryMail - end -------------------------

// ----------------------- TCheckRegisteredMail - end -----------------------

class TCheckRegisteredMail: public TRegisteredMail
{
public:  // Construction / Destruction
  TCheckRegisteredMail();
public:  // Data
  bool checked;
};  // class TCheckRegisteredMail

TCheckRegisteredMail::TCheckRegisteredMail()
{
  checked = false;
}

// ----------------------- TCheckRegisteredMail - end -----------------------

// ----------------------- reg_mail_really_sent - begin -----------------------

static bool reg_mail_really_sent(const TRegisteredMail& rm)
{
  return (rm.status == REGISTERED_MAIL_SENT && rm.send.Empty() == false);
}

// ------------------------- reg_mail_really_sent - end -------------------------

// ----------------------- RegisteredMailListCriteria - begin -----------------------

class RegisteredMailListCriteria: public SQLCriteria
{
  public:  // Construction / Destruction
    RegisteredMailListCriteria(const MailListDlgData& dlgDataRef);
  protected:  // Methods
    virtual bool ArgsOk();
    virtual void What(ostream& os);
    virtual void From(ostream& os);
    virtual void Where(ostream& os);
    virtual void OrderBy(ostream& os);
  protected:  // Data
    const MailListDlgData& rDlgData;
};  // class RegisteredMailListCriteria

RegisteredMailListCriteria::
RegisteredMailListCriteria(const MailListDlgData& dlgDataRef):
  SQLCriteria(512),
  rDlgData(dlgDataRef)
{
}

bool RegisteredMailListCriteria::ArgsOk()
{
  bool rT = SQLCriteria::ArgsOk();

  if(rT)
    if(rDlgData.mailTypes[0] == '\0')
      rT = false;
  return rT;
}

void RegisteredMailListCriteria::What(ostream& os)
{
  os << SQL_SELECT
    << "*";
}

void RegisteredMailListCriteria::From(ostream& os)
{
  os << " FROM " << "T_REGISTERED_MAIL";
}

void RegisteredMailListCriteria::Where(ostream& os)
{
  os << SQL_WHERE;
  if(rDlgData.wantCheckup == true)
  {
    if(rDlgData.begDate.Valid() || rDlgData.endDate.Valid())
    {
      if(rDlgData.begDate.Valid())
        os << "F_SEND >= " << rDlgData.begDate.Value() << " ";
      if(rDlgData.begDate.Valid() && rDlgData.endDate.Valid())
        os << " AND ";
      if(rDlgData.endDate.Valid())
        os << "F_SEND <= " << rDlgData.endDate.Value() << " ";
	os << " AND ";
    }
    os << "F_STATUS = \'" << REGISTERED_MAIL_SENT << "'";
    os << " AND ";
  }
  else
  {
    if(rDlgData.begDate.Valid() || rDlgData.endDate.Valid())
    {
      if(rDlgData.begDate.Valid())
        os << "F_DATE >= " << rDlgData.begDate.Value() << " ";
      if(rDlgData.begDate.Valid() && rDlgData.endDate.Valid())
	  os << " AND ";
      if(rDlgData.endDate.Valid())
        os << "F_DATE <= " << rDlgData.endDate.Value() << " ";
	os << " AND ";
    }
    if(is_default_post_admin() == false)
    {
      os << "F_STATUS != '" << REGISTERED_MAIL_SENT << "'";
	os << " AND ";
    }
  }
  os << "F_LAWSUIT_TYPE IN(" << SQLCharSet(rDlgData.mailTypes) << ")";
}

void RegisteredMailListCriteria::OrderBy(ostream&
//  os  // 12.sep.2003
  )
{
}

// --------------------- RegisteredMailListCriteria - end ---------------------

// --------------------- MailListColumns - begin ---------------------

class MailListColumnsEnum
{
  protected :
    enum
    {
      iC_RegSerialNo = 0,          //  1
      iC_RegOutNo,                 //  2
      iC_RegFlags,                 //  3
      iC_RegUclp,                  //  4
      iC_RegReceiver,              //  5
      iC_OrdMailType,              //  6
      iC_OrdQuantity,              //  7
      iC_RegReceiverAndRemark,     //  8
      iC_RegRemark,                //  9
      iC_RegNumRowspan,            // 10
      iC_RegFlagsRowspan,          // 11
      iC_RegUclpRowspan,           // 12
	iC_RegReceiverRowspan,       // 13
	iC_RegBarcode,               // 14 2017:095
	i_columns
    };
};  // class MailListColumnsEnum

class MailListColumns: public MailListColumnsEnum, public columnsArr
{
  public:  // Construction / Destruction
    MailListColumns(const char * formatFN, TWindow * parent,
      const TOrdinaryMail* const pOrd, TRegisteredMail* const pReg,
      const int regSerialNo, int rowspan);
  protected :  // Methods
    void MakeColumns(const TOrdinaryMail* const pOrd,
      TRegisteredMail* const pReg, const int regSerialNo, const int rowspan);
    void AddText(column* pColumn, char* const text);
    void Rowspan(const int ciph, const int rowspan, const char* const nbsp);
};  // class MailListColumns

MailListColumns::MailListColumns(const char * formatFN, TWindow * parent,
  const TOrdinaryMail* const pOrd, TRegisteredMail* const pReg,
  const int regSerialNo, int rowspan):
  columnsArr (formatFN, parent, i_columns, 1 + OFFSET_MAIL_LIST)
{
  MakeColumns(pOrd, pReg, regSerialNo, rowspan);
  defineMaxRows();
}

void MailListColumns::AddText(column* pColumn, char* const text)
{
  if(pColumn && text)
  {
    char* const bonevaDelim = strchr(text, BONEVA_SIDES_DELIMITER);

    if(bonevaDelim)
    {
      *bonevaDelim = '\0';
      pColumn->addText(text);
      pColumn->addText(bonevaDelim + 1);
    }
    else
      pColumn->addText(text);
  }
  else
    error("%s\n%s::%s", WRONG_ARGS_PASSED_TO, typeid(*this).name(), "AddText");
}  // MailListColumns::AddText

void MailListColumns::MakeColumns(const TOrdinaryMail* const pOrd,
  TRegisteredMail* const pReg, const int regSerialNo, const int rowspan)
{
  column* pColumn;
  char nbsp[100];

  ostrstream(nbsp, sizeof(nbsp))
    << HTML_CODE_ON << html_CER_blank() << HTML_CODE_OFF << ends;
  ENDING_ZERO(nbsp);
  if(strlen(nbsp) <
    (strlen(HTML_CODE_ON) + strlen(html_CER_blank()) + strlen(HTML_CODE_OFF)))
  error("nbsp buffer is too short");
  if(pOrd)
  {
    if((pColumn = columns[iC_OrdMailType]) != NULL)
    {
      if(pOrd->mailType)
      {
        TMailType ml;

        ml.mailType = pOrd->mailType;
        pColumn->addText(ml.Get() ? ml.name : nbsp);
      }
      else
        pColumn->addText("ОБЩО");
    }
    if((pColumn = columns[iC_OrdQuantity]) != NULL)
    {
      addLongToColumn(pColumn, pOrd->quantity, true);
    }
  }
  if(pReg)
  {
    if((pColumn = columns[iC_RegSerialNo]) != NULL)
    {
      addLongToColumn(pColumn, regSerialNo, 0);
    }
    if((pColumn = columns[iC_RegOutNo]) != NULL)
    {
      char tmp[128];

      tmp[0] = '\0';
      pReg->PrintDoc(tmp, sizeof(tmp));
      if(tmp[0])
	  AddText(pColumn, tmp);
      else
        AddText(pColumn, nbsp);
    }
    if((pColumn = columns[iC_RegFlags]) != NULL)
    {
	char tmp[32];

      tmp[0] = '\0';
      pReg->PrintFlags(tmp, sizeof(tmp));
      if(tmp[0])
        pColumn->addText(tmp);
      else
        AddText(pColumn, nbsp);
    }
    if((pColumn = columns[iC_RegUclp]) != NULL)
    {
      char tmp[2 * SIZE_OF_UCLP_NAME + 32];

      pReg->PrintUclp(tmp, sizeof(tmp));
      if(tmp[0])
        AddText(pColumn, tmp);
      else
        AddText(pColumn, nbsp);
    }
    if((pColumn = columns[iC_RegReceiver]) != NULL)
    {
      char tmp[SIZE_OF_NAME_FOR_BOOK + SIZE_OF_ADDRESS_FOR_BOOK + 16];

      pReg->PrintReceiver(tmp, sizeof(tmp));
      if(tmp[0])
        AddText(pColumn, tmp);
      else
        AddText(pColumn, nbsp);
    }
    if((pColumn = columns[iC_RegReceiverAndRemark]) != NULL)
    {
      char tmp[SIZE_OF_NAME_FOR_BOOK + SIZE_OF_ADDRESS_FOR_BOOK + 16];

      pReg->PrintReceiver(tmp, sizeof(tmp));
      AddText(pColumn, tmp);
      if(pReg->text[0])
        AddText(pColumn, pReg->text);
      if(tmp[0] == '\0' && pReg->text[0] == '\0')
        AddText(pColumn, nbsp);
    }
    if((pColumn = columns[iC_RegRemark]) != NULL)
    {
	if(pReg->text[0])
	  AddText(pColumn, pReg->text);
	else
	  AddText(pColumn, nbsp);
    }
    // 2017:095
    if((pColumn = columns[iC_RegBarcode]) != NULL)
    {
	if(pReg->barcode[0])
	  AddText(pColumn, pReg->barcode);
	else
	  AddText(pColumn, nbsp);
    }
    Rowspan(iC_RegNumRowspan, rowspan, nbsp);
    Rowspan(iC_RegFlagsRowspan, rowspan, nbsp);
    Rowspan(iC_RegUclpRowspan, rowspan, nbsp);
    Rowspan(iC_RegReceiverRowspan, rowspan, nbsp);
  }
}  // MailListColumns::MakeColumns

void MailListColumns::Rowspan(const int ciph, const int rowspan,
  const char* const nbsp)
{
  if(0 <= ciph && ciph < i_columns && columns && columns[ciph])
  {
    if(rowspan > 0)
    {
      char tmp[64];

      ostrstream(tmp, sizeof(tmp)) << "rowspan=\"" << rowspan << "\"" << ends;
      ENDING_ZERO(tmp);
	columns[ciph]->addText(tmp);
    }
    else
	if(nbsp && nbsp[0])
	  columns[ciph]->addText(nbsp);
  }
}  // MailListColumns::Rowspan

// --------------------- MailListColumns - end ---------------------

// --------------------- MailListOutFile - begin ---------------------

class TMailListOutFile: public frmHeadOutText
{
  public:  // Construction / destruction
    TMailListOutFile(TWindow* parent,
	const char* fmtFName);
  public:  // Methods
    int FormattedHead(const begNEndNYearTypesAC& dlgData, bool write = true);
    int FormattedText(const begNEndNYearTypesAC& dlgData,
	const char* const section, bool write = true);
    virtual bool SupportsAnyFormattedText() const ;
  protected:  // Methods
    virtual void fHOutput(const void* p, char* s, size_t len, int algn,
	size_t skip, int dCipher);
};  // class TMailListOutFile

TMailListOutFile::TMailListOutFile(TWindow* parent,
  const char* fmtFName):
  frmHeadOutText(parent, fmtFName)
{
}

int TMailListOutFile::FormattedHead(const begNEndNYearTypesAC& dlgData,
  bool write)
{
  return formHead(&dlgData, write);
}

bool TMailListOutFile::SupportsAnyFormattedText() const
{
  return true;
}

int TMailListOutFile::FormattedText(const begNEndNYearTypesAC& dlgData,
  const char* const section, bool write)
{
  int res = 0;

  if(SupportsAnyFormattedText() && section && section[0])
    res = formHead(&dlgData, write, section);
  return res;
}

void TMailListOutFile::fHOutput(const void* p, char* s, size_t len,
  int algn, size_t skip, int dCipher)
{
  const begNEndNYearTypesAC* ptrAC =
    reinterpret_cast<const begNEndNYearTypesAC*>(p);

  if (ptrAC)
  {
    if(!begNEndNYearTypesACOutput(*ptrAC, *this, s, len, algn, skip, dCipher))
	miscOutput(*this, s, len, algn, skip, dCipher);
  }
}  // TMailListOutFile::fHOutput

// --------------------- MailListOutFile - end ---------------------

// --------------------- TMailListHTMLWriter - begin ---------------------

class TMailListHTMLWriter : public HTMLOutput
{
  public:
    TMailListHTMLWriter(TMailListOutFile& outFileRef);
};

TMailListHTMLWriter::TMailListHTMLWriter(TMailListOutFile& outFileRef):
  HTMLOutput(outFileRef)
{
}

// --------------------- TMailListHTMLWriter - end ---------------------

// --------------------- MailListWriter - begin ---------------------

class MailListWriter
{
  public:  // Construction / Destruction
    MailListWriter(TWindow* const parent, const TGArray<TOrdinaryMail>& ord,
      const TISArray<TRegisteredMail>& reg, const MailListDlgData& dlgDataRef);
  public:  // Methods
    void Print();
  protected:  // Methods
    bool MakeFormatFN();
  protected:  // Data
    const TGArray<TOrdinaryMail>& ordMail;
    const TISArray<TRegisteredMail>& regMail;
    const MailListDlgData& rDlgData;
    char formatFN[128];
    TWindow* const pParent;
};  // class MailListWriter

MailListWriter::MailListWriter(TWindow* const parent,
  const TGArray<TOrdinaryMail>& ord, const TISArray<TRegisteredMail>& reg,
  const MailListDlgData& dlgDataRef):
  ordMail(ord), regMail(reg), rDlgData(dlgDataRef), pParent(parent)
{
}

bool MailListWriter::MakeFormatFN()
{
  strCopy(formatFN, "text\\omaillst", sizeof(formatFN));
  strConcat(formatFN, DEFAULT_FORMAT_FILE_EXTENSION, sizeof(formatFN));
  return(strlen(formatFN) + 1 < sizeof(formatFN));
}

void MailListWriter::Print()
{
  if(MakeFormatFN())
  {
    {
	TMailListOutFile of(pParent, formatFN);
	TMailListHTMLWriter writer(of);
	begNEndNYearTypesAC moreCommonDlgData;
	bool writeOrdinary = false;
	const int regMailItems = regMail.Count();

	rDlgData.CopyTo(moreCommonDlgData);
	of.title();
	of.FormattedHead(moreCommonDlgData);
	of.head();
	{
	  for(int i = 0;
	    writeOrdinary == false && i < ordMail.Count();
	    i++)
	    if(ordMail[i]->quantity > 0)
	    writeOrdinary = true;
	}
	if(writeOrdinary)
	{  // -- Ordinary mail --
	  TOrdinaryMail oHeap;

	  clearGroupData(oHeap);
	  of.FormattedText(moreCommonDlgData, "***_START_OF_ORDINARY");
	  for(int i = 0; i < ordMail.Count(); i++)
	  {
	    if(ordMail[i]->quantity > 0)
	    {
		MailListColumns ordC(formatFN, pParent, ordMail[i], NULL, 0, 0);

		writer.WriteToTableRow(ordC, OFFSET_MAIL_LIST, "***_ORDINARY_ROW");
		oHeap.quantity += ordMail[i]->quantity;
	    }
	  }
	  {
	    MailListColumns ordCT(formatFN, pParent, &oHeap, NULL, 0, 0);

	    writer.WriteToTableRow(ordCT, OFFSET_MAIL_LIST,
		"***_ORDINARY_TOTAL_ROW");
	  }
	  of.FormattedText(moreCommonDlgData, "***_END_OF_ORDINARY");
	}  // -- Ordinary mail --
	if(regMailItems > 0)
	{  // -- Registered mail --
	  int envelopeNo = 1;

	  of.FormattedText(moreCommonDlgData, "***_START_OF_REGISTERED");
	  for(int i = 0; i < regMailItems; i++)
	  {
	    // Determine if a member of a joint envelope is being printed
	    bool binded = false;
	    bool bindedNo1 = false;
	    int envlpMembersCnt = 0;

	    if(regMail[i]->envelopeNo != 0)
	    {
		// Determine if this is really a joint envelope
		for(int j = 0; j < regMailItems; j++)
		  if(regMail[i]->SameEnvelope(*(regMail[j])))
		    envlpMembersCnt++;

		if(envlpMembersCnt > 0)
		{  // It is.
		  binded = true;
		  if(i == 0)
		    bindedNo1 = true;
		  else
		    if(regMail[i]->SameEnvelope(*(regMail[i - 1])) == false)
			bindedNo1 = true;
		}
	    }

	    // Dependent on the joint envelope relation there are several
	    // varianths of rows.
	    if(binded)
	    {  // A joint envelope member
		if(bindedNo1)
		{  // First of a group
		  MailListColumns regC(formatFN, pParent, NULL, regMail[i],
		    envelopeNo, envlpMembersCnt);

		  writer.WriteToTableRow(regC, OFFSET_MAIL_LIST,
		    "***_REGISTERED_ROW_START_OF_GROUP");
		  envelopeNo++;
		}
		else
		{  // Next of a group
		  MailListColumns regC(formatFN, pParent, NULL, regMail[i],
		    envelopeNo, 0);

		  writer.WriteToTableRow(regC, OFFSET_MAIL_LIST,
		    "***_REGISTERED_ROW_REST_OF_GROUP");
		  // There is no need to do "envelopeNo++;" here.
		}
	    }
	    else
	    {  // Single doc
		MailListColumns regC(formatFN, pParent, NULL, regMail[i],
		  envelopeNo, 0);

		writer.WriteToTableRow(regC, OFFSET_MAIL_LIST,
		  "***_REGISTERED_ROW");
		envelopeNo++;
	    }
	  }
	  of.FormattedText(moreCommonDlgData, "***_END_OF_REGISTERED");
	}  // -- Registered mail --
	of.tail();
	//show_html_stream(pParent, of);
	show_office_stream(of);
    }
  }
}  // MailListWriter::Print

// --------------------- MailListWriter - end ---------------------

// ------------------------ TMailDocDialog - begin ------------------------
class TMailDocDialog: public TScaleDialog
{
  public:  // Construction / Destruction
    TMailDocDialog(TWindow* const parent, TRegisteredMail& data,
	const bool editExisting);
  public:  // Methods
    virtual void SetupWindow();
  protected:  // Methods
    void OkBnClicked();
    void AddBnClicked();
    void EditBnClicked();
    void DeleteBnClicked();
    void OutRgCnvRcvBnClicked();
    bool ReceiverWork(const bool fromEditButton);
    //int32 AnyFocusKilled(TParam1 hwndGetFocus, TParam2);
    //bool DocExists(TRCDKey* const pKey = NULL, const bool askAllowed = true);
    bool InsertReceiverString();
    void EnableDisableButtons();
    HBRUSH EvCtlColor(HDC hDC, HWND hWndChild, uint ctlType);
    int OutregReceiverConversionPossible();
    void NameOutRgCnvRcvButton();
    void DisablePostal();
  protected:  // Data
    TRegisteredMail& rRm;
    TRegisteredMail localRm;  // 19.feb.2004 BUGFIX
    const bool edit;
    //const MailListDlgData& rDlgData;
    //bool badWork;	// OMFG... OK -> Cancel 
    char types[SIZE_OF_TYPES];
  private:
    // 2009:065 LPR: eliminating GetDlgItemPtr
    TCharAliasFace *md_type;
    TLongFace *md_no;
    TYearFace *md_year;
    TCheckBox *md_sent;
    TEdit *md_remark;
    TStatic *md_redstatic1;
    TStringListBox *md_receiver;
    TButton *md_outrg_cnv_rcv;
    TGroupBox *md_post_grp;
    TRadioButton *md_post_r;
    TRadioButton *md_post_ar;
    TRadioButton *md_parcel;
    TPushButton *md_add;
    TPushButton *md_edit;
    TPushButton *md_delete;
    TPushButton *idok;
    TBarCodeFace *barcode;
DECLARE_RESPONSE_TABLE(TMailDocDialog);
};  // class TMailDocDialog

DEFINE_RESPONSE_TABLE1(TMailDocDialog, TScaleDialog)
  EV_BN_CLICKED(IDOK, OkBnClicked),
  EV_BN_CLICKED(IDADD, AddBnClicked),
  EV_BN_CLICKED(IDEDIT, EditBnClicked),
  EV_BN_CLICKED(IDDELETE, DeleteBnClicked),
  EV_BN_CLICKED(IDC_EDIT_MAIL_DOC_OUTRG_CNV_RCV, OutRgCnvRcvBnClicked),
//  EV_MESSAGE(WM_KILLFOCUS, AnyFocusKilled),
  EV_WM_CTLCOLOR,
END_RESPONSE_TABLE;

TMailDocDialog::TMailDocDialog(TWindow* const parent, TRegisteredMail& data,
  const bool editExisting):
  TScaleDialog(parent, IDD_EDIT_MAIL_DOC),
  rRm(data), edit(editExisting)
{
  localRm << rRm;  // 19.feb.2004 BUGFIX
  //badWork = false;
  strCopy(types, TYPE_LAWSUITS, sizeof(types));
  strConcatCh(types, TYPE_REQUEST, sizeof(types));
  strConcatCh(types, TYPE_OUTREG, sizeof(types));
  md_type = new TCharAliasFace(this, IDC_EDIT_MAIL_DOC_TYPE, "T", Type, types);
  md_no = new TLongFace(this, IDC_EDIT_MAIL_DOC_NO, "N", SIZE_OF_NO);
  md_year = new TYearFace(this, IDC_EDIT_MAIL_DOC_YEAR, "Y");
  {
    md_post_grp = new TGroupBox(this, IDC_EDIT_MAIL_DOC_POST_GRP);
    md_post_r = new TRadioButton(this, IDC_EDIT_MAIL_DOC_R, md_post_grp);
    md_post_ar = new TRadioButton(this, IDC_EDIT_MAIL_DOC_AR, md_post_grp);
    md_parcel = new TRadioButton(this, IDC_EDIT_MAIL_DOC_PARCEL, md_post_grp);
  }
  md_sent = new TCheckBox(this, IDC_EDIT_MAIL_DOC_SENT, NULL);
  md_remark = new TEdit(this, IDC_EDIT_MAIL_DOC_REMARK, sizeof(localRm.text));
  md_receiver = new TStringListBox(this, IDC_EDIT_MAIL_DOC_RECEIVER);
  idok = new TPushButton(this, IDOK);
  md_add = new TPushButton(this, IDADD);
  md_edit = new TPushButton(this, IDEDIT);
  md_delete = new TPushButton(this, IDDELETE);
  md_outrg_cnv_rcv = new TButton(this, IDC_EDIT_MAIL_DOC_OUTRG_CNV_RCV);
  new TPushButton(this, IDCANCEL);
  md_redstatic1 = new TStatic(this, IDC_EDIT_MAIL_DOC_REDSTATIC1, 0);
  barcode = new TBarCodeFace(this, IDC_EDIT_MAIL_DOC_BARCODE, "F_BARCODE");
}  // TMailDocDialog::TMailDocDialog

void TMailDocDialog::DisablePostal()
{
  md_post_r->SetCheck(BF_UNCHECKED);
  md_post_ar->SetCheck(BF_UNCHECKED);
  md_parcel->SetCheck(BF_UNCHECKED);
}

void TMailDocDialog::SetupWindow()
{
  TScaleDialog::SetupWindow();
  {
    TCharAliasFace* pType = md_type;
    TLongFace* pNo = md_no;
    TYearFace* pYear = md_year;

    if(pType)
	pType->SetC(localRm.doc.type);
    if(pNo)
	pNo->SetL(localRm.doc.no);
    if(pYear)
	pYear->SetL(localRm.doc.year);
    pType->EnableWindow(false);
    pNo->EnableWindow(false);
    pYear->EnableWindow(false);
  }
  md_sent->SetCheck(reg_mail_really_sent(localRm) ? BF_CHECKED : BF_UNCHECKED);  // 09.feb.2004 change
  if(edit == true)
  {
    InsertReceiverString();
    md_remark->Transfer(localRm.text, tdSetData);
    if(localRm.envelopeNo != 0)
    {
	char tmp[128];

	strCopy(tmp, "Документът е част от общ ", sizeof(tmp));
	strConcat(tmp, localRm.flags == OUTREG_PACKAGE ? "колет" : "плик",
	  sizeof(tmp));
	strConcat(tmp, ".", sizeof(tmp));
	md_redstatic1->SetWindowText(tmp);
    }
    if(is_default_post_admin() == true)
    {
	if(
	  // rRm.status != REGISTERED_MAIL_SENT ||  // 09.feb.2004
	  reg_mail_really_sent(localRm) == false ||  // 09.feb.2004
	  localRm.envelopeNo != 0)
	  md_sent->EnableWindow(false);
    }
    else
	md_sent->ShowWindow(SW_HIDE);
  }
  else
  {
    md_sent->ShowWindow(SW_HIDE);
  }

  DisablePostal();
  if(edit == true || localRm.doc.type == TYPE_OUTREG)
  {
    // 2010:063 FIX: OUTREG_POSTAL support
    switch(localRm.flags & OUTREG_POSTAL)
    {
	case OUTREG_R:
	  md_post_r->SetCheck(BF_CHECKED);
	  break;
	case OUTREG_AR:
	  md_post_ar->SetCheck(BF_CHECKED);
	  break;
	case OUTREG_PACKAGE:
	  md_parcel->SetCheck(BF_CHECKED);
	  break;
	default:
	  md_post_r->SetCheck(BF_CHECKED);
	  error("Unknown way of sending the letter!");
	  //badWork = true;
	  break;
    }
  }
  else
  {
    md_post_r->SetCheck(BF_CHECKED);
  }
  if(localRm.doc.type == TYPE_OUTREG ||  // Not allowed (Lucy)
     (edit == true && localRm.envelopeNo != 0) // Keep the joint envlp integrity
    )
  {
    md_receiver->EnableWindow(false);
    if (!edit)  // 2017:118 PRQ/LRQ
	DisablePostal();
  }
  if(localRm.envelopeNo == 0)
    md_redstatic1->Show(SW_HIDE);
  NameOutRgCnvRcvButton();  // 16.feb.2004 - begin
  if(localRm.doc.type != TYPE_OUTREG)
    md_outrg_cnv_rcv->Show(SW_HIDE);  // 16.feb.2004 - end
  EnableDisableButtons();
  barcode->SetS(localRm.barcode);
}  //  TMailDocDialog::SetupWindow

void TMailDocDialog::OkBnClicked()
{
  //if(badWork == false)
  {
    //if(DocExists(&(rRm.doc), false))
    //{
	bool statusOk = true;

	if(edit == true)
	{
        const char status =
	    md_sent->GetCheck() == BF_CHECKED ?
            REGISTERED_MAIL_SENT : REGISTERED_MAIL_PREPARED;

        if(localRm.status != REGISTERED_MAIL_SENT &&
          status == REGISTERED_MAIL_SENT)
        {
          statusOk = false;
          error("Изпращане се регистрира чрез бутон Потвърждение / Запис от "
            "главния диалог.");
        }
        else
          if(localRm.status == REGISTERED_MAIL_SENT &&
            status != REGISTERED_MAIL_SENT)
          {
            localRm.status = status;
		clear_CDate(localRm.send);
	    }
	}
	if(statusOk == true)
	{
	  md_remark->Transfer(localRm.text, tdGetData);
	  {
	    // 2010:063 LPR: OUTREG_POSTAL support
	    int flags = 0;

	    if (md_post_r->GetCheck() == BF_CHECKED)
		flags = OUTREG_R;
	    else if (md_post_ar->GetCheck() == BF_CHECKED)
		flags = OUTREG_AR;
	    else if (md_parcel->GetCheck() == BF_CHECKED)
		flags = OUTREG_PACKAGE;

	    localRm.flags = (localRm.flags & ~OUTREG_POSTAL) | flags;
	  }
	  strcpy(localRm.barcode, barcode->S());
	  rRm << localRm;  // 19.feb.2004 BUGFIX
	  CmOk();
	}
    //}
  }
  //else
  //  CmCancel();
}  // TMailDocDialog::OkBnClicked

bool TMailDocDialog::InsertReceiverString()
{
  bool res = false;
  TStringListBox& lstBox = *md_receiver;

  if(lstBox.GetCount() == 0)
  {
//    if(rRm.KeyOk())
    {
      char tmp[256];

      tmp[0] = '\0';
      localRm.PrintUclp(tmp, sizeof(tmp));
      strConcat(tmp, " ", sizeof(tmp));
      localRm.PrintReceiver(tmp + strlen(tmp), sizeof(tmp) - strlen(tmp));
	lstBox.AddString(tmp);
	res = true;
	lstBox.SetHorizExtent();
    }
  }
  else
    error("Не може да има повече от един получател.");
  EnableDisableButtons();
  return res;
}  // TMailDocDialog::InsertReceiverString

void TMailDocDialog::EnableDisableButtons()
{
  md_outrg_cnv_rcv->EnableWindow(false);
  if(localRm.doc.type != TYPE_OUTREG || edit == true)
  {
    int cnt = md_receiver->GetCount();

    if(cnt < 1)
    {
      idok->EnableWindow(false);
      md_add->EnableWindow(true);
      md_edit->EnableWindow(false);
      md_delete->EnableWindow(false);
    }
    else
      if(cnt == 1)
      {
        idok->EnableWindow(true);
        md_add->EnableWindow(false);
        md_edit->EnableWindow(true);
        md_delete->EnableWindow(true);
      }
      else
      {  // > 1  hypothetic...
        idok->EnableWindow(false);
        md_add->EnableWindow(false);
        md_edit->EnableWindow(false);
        md_delete->EnableWindow(true);
      }
    if(localRm.doc.type == TYPE_OUTREG)
    {
      md_add->EnableWindow(false);
      md_edit->EnableWindow(false);
      md_delete->EnableWindow(true);
      md_outrg_cnv_rcv->EnableWindow(cnt == 1 &&
        OutregReceiverConversionPossible() != 0 &&
        localRm.envelopeNo == 0);
    }
  }
  else
  {
    md_add->EnableWindow(false);
    md_edit->EnableWindow(false);
    md_delete->EnableWindow(false);
  }
  if(edit == true && localRm.envelopeNo != 0)
  {  // To keep the integrity of the joint envelope
    md_add->EnableWindow(false);
    md_edit->EnableWindow(false);
    md_delete->EnableWindow(false);
  }
  if(localRm.doc.type == TYPE_OUTREG)
  {  // Disallowed for outregs...
    md_add->EnableWindow(false);
    md_edit->EnableWindow(false);
    md_delete->EnableWindow(false);
  }
  if(reg_mail_really_sent(localRm) == true)
  {
    md_add->EnableWindow(false);
    md_edit->EnableWindow(false);
    md_delete->EnableWindow(false);
    md_remark->EnableWindow(false);
    md_receiver->EnableWindow(false);
    DisablePostal();
  }
}  // TMailDocDialog::EnableDisableButtons

bool TMailDocDialog::ReceiverWork(const bool fromEditButton)
{
  bool res = false;

  if(TRCDKey_(localRm.doc).FilledOK())
  {
    // Get the receiver
    TTransGroup ucnGroup;
    CharPtr tmp(strlen(UCN_REAL_SIDES) + 1
#if INSTANCE  // 30.mar.2004 - begin
#  if APPEAL
#  else  // of APPEAL
      + strlen(UCN_DAMNS)
#  endif  // of APPEAL
#endif  // of INSTANCE  // 30.mar.2004 - end
    );
//    char tc[2];  // 15.jun.2004

    strCopy(tmp, UCN_REAL_SIDES, tmp.Size());
#if INSTANCE  // 30.mar.2004 - begin
#  if APPEAL
#  else  // of APPEAL
    strConcat(tmp, UCN_DAMNS, tmp.Size());
#  endif  // of APPEAL
#endif  // of INSTANCE  // 30.mar.2004 - end
//    tc[1] = '\0';  // 15.jun.2004 - begin
//    tc[0] = UCN_ANONYMOUS_CODE;
//    difference(tmp, tc);
//    tc[0] = UCN_UNKNOWN_CODE;
//    difference(tmp, tc);  // 15.jun.2004 - end
    if(fromEditButton)
    {
      strCopy(ucnGroup.ucn, localRm.ucn, sizeof(ucnGroup.ucn));
      ucnGroup.ucnType = localRm.ucnType;
    }
    if(fromEditButton || (tmp[0] && get_ucn(this, &ucnGroup, tmp, true)))
    {
      TAnyAddress *address;

      // 2008:018 LPR: address handling here was crap; is foreign allowed?
	if ((address = get_address(this, &ucnGroup, ADDRESS_BULGARIAN_ONLY)) != NULL)
	{
	localRm << address;
	res = true;
	delete address;
      }
    }
  }
  return res;
}  // TMailDocDialog::ReceiverWork

void TMailDocDialog::AddBnClicked()
{
  const bool res = ReceiverWork(false);

  if(res == true)
    if(InsertReceiverString() == false)
      CmCancel();
    else
    {
      md_receiver->SetFocus();
      EnableDisableButtons();
    }
}  // TMailDocDialog::AddBnClicked

void TMailDocDialog::EditBnClicked()
{
  TStringListBox& lstBox = *md_receiver;
  const int selInd = lstBox.GetSelIndex();

  if(selInd == 0)  // No more than 1 item in the list box is allowed...
  {
    const bool res = ReceiverWork(true);

    if(res == true)
    {
	lstBox.DeleteString(selInd);
	if(InsertReceiverString() == false)
	  CmCancel();
      lstBox.SetHorizExtent();
    }
  }
}  // TMailDocDialog::EditBnClicked

void TMailDocDialog::DeleteBnClicked()
{
  TStringListBox& lstBox = *md_receiver;
  const int selInd = lstBox.GetSelIndex();

  if(selInd == 0)  // No more than 1 item in the list box is allowed...
  {
    lstBox.DeleteString(selInd);
    {
      TAddress addr;

      clearGroupData(addr);
      localRm << addr;
      lstBox.SetHorizExtent();
      EnableDisableButtons();
    }
  }
  EnableDisableButtons();  // Because of the situation, when 0(zero) items
  // are remaining in the list.
}  // TMailDocDialog::DeleteBnClicked

int TMailDocDialog::OutregReceiverConversionPossible()
{
  int res = 0;

  if(localRm.doc.type == TYPE_OUTREG &&
    TRCDKey_(localRm.doc).FilledOK() == true)
  {
    TOutReg outRg;

    outRg.key = localRm.doc;
    if(outRg.Get() == true && ucnPresent(outRg.receiver))
    {
      if(md_receiver->GetCount() == 1)
      {
        if(strcmp(localRm.ucn, outRg.receiver) == 0 &&
          // 20061220 localRm.ucnType == UCN_INSTANCE_THIS)
                      localRm.ucnType == UCN_INSTANCE_HIGHER)
        {
          res = 1;
        }
        else
        {
          res = -1;  // To be possible to return back to outRg.receiver with
            // no regard to rRm.ucnType
        }
      }
    }
  }
  return res;
}  // TMailDocDialog::OutregReceiverConversionPossible

void TMailDocDialog::OutRgCnvRcvBnClicked()
{
  const int conversion = OutregReceiverConversionPossible();

  if(conversion == 1)
  {
    TSender sender;

    sender.ucnType = localRm.ucnType;
    strCopy(sender.ucn, localRm.ucn, sizeof(sender.ucn));
    if(sender.Get())
    {
	TGArray<TFirm> firms;

	// Collect all possible firms to convert to
	{
		// 2017:111 LPR: rewrite
		TFirm firm;
		msql m(firm.Name);

		strupr(sender.name);		// IRQ: FIX for mixed-court names
		m.Add(&sender, "F_NAME");
		m.AddOrder("F_UCN_TYPE DESC, F_UCN");  // "DESC will bring 'f' forwards"
		firm.LoadArray(firms, str(m));
		// WaitWindow not reproduced: loses focus, and search by name is fast
	}

	if(firms.Count() > 0)
	{
	int choosen;
	const int fCnt = firms.Count();

	// Choose one of the possible firms
	if(fCnt > 1)
	{
	  // Here is the better...
	  TStringArray strArr;
	  TUCNType ucnT;
	  char tmp[sizeof(firms[0]->name) + 1 + sizeof(firms[0]->ucn) + 1 +
	    sizeof(ucnT.name)];

	  for(int i = 0; i < fCnt; i++)
	  {
	    TFirm& f = (*(firms[i]));

	    ucnT.ucnType = f.ucnType;
	    if(ucnT.Get() == false)
	    {
	      ZERO_LEN(ucnT.name);
	    }
	    ostrstream(tmp, sizeof(tmp))
		<< ucnT.name << " " << f.ucn << " " << f.name << ends;
	    ENDING_ZERO(tmp);
	    strArr.Add(strnew(tmp));
	  }
	  choosen = select_string(this, &strArr, "Юридически лица с еднакви имена", "Изберете нужното юридическо лице");
	}
	if(0 <= choosen && choosen < fCnt)
	{
	  TFirm& firm = *(firms[choosen]);
	  TFoundGroup ucnGroup;

	    strCopy(ucnGroup.ucn, firm.ucn, sizeof(ucnGroup.ucn));
	    ucnGroup.ucnType = firm.ucnType;
	    {  // Edit the address of the choosen firm
		TAddress addr;

		// Create an addres if none exists.
		strCopy(addr.ucn, firm.ucn, sizeof(addr.ucn));
		addr.ucnType = firm.ucnType;
		if(addr.Try() == false)
		{
		  strCopy(addr.ucn, firm.ucn, sizeof(addr.ucn));
		  addr.ucnType = firm.ucnType;
		  addr.Insert();
		}
		// Edit any address
	    if(get_address(this, ucnGroup.ucn, ucnGroup.ucnType, &addr))
	    {  // Refresh data
		localRm << addr;
		  md_receiver->DeleteString(0);
		    // There is EXACTLY one element
		    // in the list box (see
		    // OutregReceiverConversionPossible)
		  if(InsertReceiverString() == false)
		    CmCancel();
		  md_receiver->SetHorizExtent();
		}
	    }
	}
	}
	else
	{
	  error("Юридическо лице с име \"%s\" не е открито.", sender.name);
	}
    }
  }
  else
    if(conversion == -1)
    {
	TOutReg outRg;

      outRg.key = localRm.doc;  // It's Ok - see OutregReceiverConversionPossible
      if(outRg.Get() == true && ucnPresent(outRg.receiver))
      {
        TAddress addr;

        strCopy(addr.ucn, outRg.receiver, sizeof(addr.ucn));
        // 20061220 addr.ucnType = UCN_INSTANCE_THIS;
                    addr.ucnType = UCN_INSTANCE_HIGHER ;
        if(addr.Get() == false)
        {
          strCopy(addr.ucn, outRg.receiver, sizeof(addr.ucn));
          // 20061220 addr.ucnType = UCN_INSTANCE_THIS;
                      addr.ucnType = UCN_INSTANCE_HIGHER ;  
        }
        localRm << addr;
	  md_receiver->DeleteString(0);
	    // There is EXACTLY one element
	    // in the list box (see
	    // OutregReceiverConversionPossible)
	  if(InsertReceiverString() == false)
          CmCancel();
        md_receiver->SetHorizExtent();
      }
    }
  NameOutRgCnvRcvButton();
}  // TMailDocDialog::OutRgCnvRcvBnClicked

void TMailDocDialog::NameOutRgCnvRcvButton()
{
  const char* const n = (OutregReceiverConversionPossible() == -1) ?
    "ЮЛ -> Съд" : "Съд -> ЮЛ";

  md_outrg_cnv_rcv->SetWindowText(n);
}

HBRUSH TMailDocDialog::EvCtlColor(HDC hDC, HWND hWndChild, uint ctlType)
{
  if(ctlType == CTLCOLOR_STATIC)
  {
    if(md_redstatic1->HWindow == hWndChild)
    {
	if(GetBkColor(hDC) != TColor::LtRed)
	{
	  SetTextColor(hDC, TColor::LtRed);
	  SetBkMode(hDC, TRANSPARENT);
	  return(HBRUSH)GetStockObject(NULL_BRUSH);
	}
    }
  }
  return TScaleDialog::EvCtlColor(hDC, hWndChild, ctlType);
} // TMailDocDialog::EvCtlColor

// ------------------------ TMailDocDialog - end ------------------------

// ----------------------- TOrdMailListWindow - begin -----------------------
class TOrdMailListWindow: public TNTListWindow
{
  public:  // Construction / Destruction
    TOrdMailListWindow(TWindow* parent, int resourceId,
      const MailListDlgData& dlgDataRef);
    virtual ~TOrdMailListWindow();
  public:  // Methods
    virtual void SetupWindow();
    const TGArray<TOrdinaryMail>& Data() const;
    virtual bool CountsOk() const;
    bool SetQuantity(const int item, const char* const quantityStr);
    bool NonZeroData() const;
  protected:  // Methods
    virtual bool LoadData();
    virtual bool InsertColumns();
    virtual bool InsertRows();
    virtual bool Put(const int item);
    void EvKeyDown(uint key, uint repeatCount, uint flags);
  protected:  // Data
    TGArray<TOrdinaryMail> data;
    const MailListDlgData& rDlgData;
DECLARE_RESPONSE_TABLE(TOrdMailListWindow);
};  // class TOrdMailListWindow

DEFINE_RESPONSE_TABLE1(TOrdMailListWindow, TNTListWindow)
  EV_WM_KEYDOWN,
END_RESPONSE_TABLE;

TOrdMailListWindow::TOrdMailListWindow(TWindow* parent, int resourceId,
  const MailListDlgData& dlgDataRef):
  TNTListWindow(parent, resourceId),
  rDlgData(dlgDataRef)
{
}

TOrdMailListWindow::~TOrdMailListWindow()
{
}

void TOrdMailListWindow::SetupWindow()
{
  TNTListWindow::SetupWindow();
  LoadData();
  if(InsertColumns())
    InsertRows();
  if(GetItemCount() > 0)
    SetSel(0, true);
}  // TOrdMailListWindow::SetupWindow

bool TOrdMailListWindow::LoadData()
{
  const char* const methodName = "LoadData";
  bool rT = true;
  char mailTypes[sizeof(rDlgData.mailTypes)];

  if(TMailType::LoadAll(mailTypes, sizeof(mailTypes)))
  {
    const size_t mtl = strlen(mailTypes);

    for(int i = 0; rT && i < mtl; i++)
    {
      const char t = mailTypes[i];
      const bool typeLaw = strchr(TYPE_LAWSUITS, t);

      if(typeLaw == false || strchr(rDlgData.mailTypes, t))
      {  // Only user-selected types
	TOrdinaryMail* pNewOM = new TOrdinaryMail;

	pNewOM->mailType = t;
	data.Add(pNewOM);
      }
    }
  }
  else
    rT = false;
  if(rT == false)
    error("%s::%s\n%s",
      typeid(*this).name(), methodName, UNSUCCESSFUL_OPERATION);
  return rT;
}  // TOrdMailListWindow::LoadData

bool TOrdMailListWindow::InsertColumns()
{
    TListWindColumn lwc;

    lwc.SetText("Брой");
    lwc.SetWidth(50);
    InsertColumn(0, lwc);

    lwc.SetText("Изпращане");
    lwc.SetWidth(130);
    InsertColumn(1, lwc);
    
    return true;
}  // TOrdMailListWindow::InsertColumns

bool TOrdMailListWindow::CountsOk() const
{
  const bool res = GetItemCount() == data.Count();

  if(res == false)
    error("%s::%s\n%s",
      typeid(*this).name(), "CountsOk", UNSUCCESSFUL_OPERATION);
  return res;
}

bool TOrdMailListWindow::InsertRows()
{
  const int dItems = data.Count();
  bool rT = true;
  TWaitWindow ww(this, PLEASE_WAIT);

  for(int i = 0; rT == true && i < dItems; i++)
  {  // -- 0 --
    if(Put(i) == false)
	rT = false;
  }  // -- 0 --
  if(CountsOk() == false)
    rT = false;
  if(rT == false)
    error("%s::%s\n%s", typeid(*this).name(), "InsertRows",
	UNSUCCESSFUL_OPERATION);
  return rT;
}  // TOrdMailListWindow::InsertRows

bool TOrdMailListWindow::Put(const int item)
{
  const char* const methodName = "Put";
  const int itemsCnt = GetItemCount();

  if(0 <= item && item <= itemsCnt &&
    0 <= item && item < data.Count())
  {
    int subItem = 0;
    char tmp[256];
    const TOrdinaryMail& om = *(data[item]);

    tmp[0] = '\0';
    {  // The item  (doc identification)
      TListWindItem lwi;

	lwi.iItem = item;
      lwi.iSubItem = subItem;  // Not necessary, but let it be.
      strCopy(tmp, "0", sizeof(tmp));
      lwi.SetText(tmp);
	InsertItem(lwi);
    }
    {  // The subitems
	LV_ITEM lvi;

      lvi.mask = 0;
      lvi.mask |= LVIF_TEXT;
      lvi.iItem = item;
	{  // The mail type name
	  lvi.iSubItem = ++subItem;
        {
          TMailType mt;

          mt.mailType = om.mailType;
          if(mt.mailType && mt.Get())
            strCopy(tmp, mt.name, sizeof(tmp));
          else
            tmp[0] = '\0';
        }
        lvi.pszText = tmp;
	  SetItem(TListWindItem(lvi));
	}
    }

    return true;
  }

  return error("%s\n%s::%s", WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
}  // TOrdMailListWindow::Put

bool TOrdMailListWindow::SetQuantity(const int item,
  const char* const quantityStr)
{
  const char* const methodName = "SetQuantity";
  bool rT = true;

  if(quantityStr && 0 <= item && item < GetItemCount())
  {
    if(CountsOk())
    {
      const size_t l = strlen(quantityStr);

      for(size_t i = 0; rT && i < l; i++)
      {
        if(quantityStr[i] < '0' || '9' < quantityStr[i])
          rT = error("Невалиден символ '%c'", quantityStr[i]);
      }
      if(rT)
        rT = SetItemText(item, 0, quantityStr);
      if(rT)
        data[item]->quantity = atoi(quantityStr);
    }
    else
      rT = false;
  }
  else
    rT = error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
  return rT;
}  // TOrdMailListWindow::SetQuantity

bool TOrdMailListWindow::NonZeroData() const
{
  bool dataFound = false;

  if(CountsOk())
  {
    for(int i = 0; dataFound == false && i < data.Count(); i++)
    {
      dataFound = (data[i]->quantity > 0);
    }
  }
  return dataFound;
}  // TOrdMailListWindow::NonZeroData

void TOrdMailListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  TNTListWindow::EvKeyDown(key, repeatCount, flags);
  if(key == VK_F2 && GetSelCount() == 1)
  {
    bool found = false;

    for(int i = 0; found == false && i < GetItemCount(); i++)
    {
      if(IsSelected(i))
      {
        EditLabel(i);
        found = true;
      }
    }
  }
}  // TOrdMailListWindow::EvKeyDown

const TGArray<TOrdinaryMail>& TOrdMailListWindow::Data() const
{
  CountsOk();  // Provocate the error message (if there is something wrong)
  return data;
}
// ----------------------- TOrdMailListWindow - end -----------------------

// ----------------------- TRegMailListWindow - begin -----------------------

//class TRegMailListWindow: public TColumnRowListWindow
class TRegMailListWindow: public TResizeListWindow
{
  public:  // Construction / Destruction
    TRegMailListWindow(TWindow* parent, int resourceId,
      const MailListDlgData& dlgDataRef);
    virtual ~TRegMailListWindow();
  public:  // Methods
    virtual void SetupWindow();

    // Do not remember for further use these values!
    // They may be invalid in any further moment
    const TISArray<TCheckRegisteredMail>& DataTmp() const;
    TISArray<TCheckRegisteredMail>& NonConstDataTmp();

    bool DeleteSelected();
    virtual bool InsertLetter();
    virtual bool InsertLetterEx();
    virtual bool EditSelected();
    virtual bool CountsOk() const;
    bool DeselectAll();
    bool EnvelopeJob();
    bool SaveSelection(const CDate sent);
    bool CorrectEnvelopeSelection(const bool errMsg = true) const;
    bool CorrectEnvelopeCheck(const bool errMsg = true) const;
    bool EnvelopeMemberSelected() const;
    virtual bool ReDisplay();
    bool IsChecked(const int ind) const;
    bool SetItemCheck(const int ind, const bool checked, const bool affectData,
      bool* pAdminSlapped = NULL);
    bool SetEnvelopeCheck(const int ind, const bool checked,
      const bool affectData, bool* pAdminSlapped = NULL);
    bool ToggleItem(const int ind, const bool affectData);
    int GetCheckCount(bool nonSentOnly = false) const;
    int GetNonSentCheckCount() const;
    int GetFocusIndex() const;  // Returns -1 if there is no focused element
    int FindSubstring(int startingItem, const char* const text) const; //Returns
      // -1 if not found
    bool DisabledItem(const TRegisteredMail& rm) const;
    bool SentItemSelected() const;
  protected:  // Methods
    virtual bool LoadData();
    virtual bool InsertColumns();
    virtual bool InsertRows();
    virtual bool Put(const int item, const int imgInd = -1);
    virtual bool ReArrange();
    bool CorrectEnvelopeSelectionOrCheck(const bool check,
      const bool errMsg = true) const;
    char GetDoc(TRCDKeyGroup& aKey);
    bool GetFlags(TRegisteredMail& rm);
    bool GetReceiver(TRegisteredMail& rm);
    bool GetOutregReceivers(const TRCDKey key,
      TGArray<TUCNGroup>& receivers);
    bool GetAddress(TRegisteredMail& rm);
    bool InsertOutreg(const char lawType, const TRCDKey key);
    bool InsertNonOutreg(const char lawType, const TRCDKey key);
    bool EditOutreg(const int ind, bool& reArranged);
    bool EditNonOutreg(const int ind, bool& reArranged);
    bool EditAny(const int ind, bool& reArranged);
    bool InitImageList();
    bool EnvelopeSelection(const TRegisteredMail& commonTraits, const bool set,
      const bool newly);
    void EvLButtonDown(uint modKeys, TPoint& point);
    void EvVScroll(uint scrollCode, uint thumbPos, THandle hWndCtl);
  protected:  // Data
    TISArray<TCheckRegisteredMail>* pData;  // It could be simply
      // TISArray<TRegisteredMail> data, but this way may gain
      // performance improvement in ReArrange().
    const MailListDlgData& rDlgData;
    TImageList* pImageList;  // 08.sep.2003
    enum ImageListIndex
    {
      Ili_Blank = 0,
      Ili_BigEnvelope,
      Ili_BigEnvelopeA,
      Ili_SmallEnvelope,
      Ili_SmallEnvelopeA,
      Ili_MiddleContent,
      Ili_LastContent,
      Ili_Parcel,
      Ili_BigEnvelopeDis,
      Ili_BigEnvelopeADis,
      Ili_SmallEnvelopeDis,
      Ili_SmallEnvelopeADis,
      Ili_MiddleContentDis,
      Ili_LastContentDis,
      Ili_ParcelDis,

      Ili_images
    };
    TImageList* pStateImageList;
    enum StateImageIndex
    {
      Si_Unchecked = 1,
      Si_Checked,
    };
DECLARE_RESPONSE_TABLE(TRegMailListWindow);
    // 2009:127 LPR: new resize
    int AdjustedWidth(int value) { return value * initialWidth / 770; }
};  // class TRegMailListWindow


DEFINE_RESPONSE_TABLE1(TRegMailListWindow, TResizeListWindow)
  EV_WM_LBUTTONDOWN,
  EV_WM_VSCROLL,
END_RESPONSE_TABLE;

TRegMailListWindow::TRegMailListWindow(TWindow* parent, int resourceId,
  const MailListDlgData& dlgDataRef):
  TResizeListWindow(parent, resourceId),
  rDlgData(dlgDataRef)
{
  pData = new TISArray<TCheckRegisteredMail>;
  pImageList = NULL;
  pStateImageList = NULL;
}

TRegMailListWindow::~TRegMailListWindow()
{
  delete pImageList;
  delete pStateImageList;
  delete pData;
}

void TRegMailListWindow::SetupWindow()
{
  TResizeListWindow::SetupWindow();
  InitImageList();
  LoadData();
  if(InsertColumns())
    InsertRows();
}  // TRegMailListWindow::SetupWindow

bool TRegMailListWindow::InitImageList()
{
  bool rT = true;

  pImageList = new TImageList(TSize(16, 16), ILC_COLOR4, 16, 1);

  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONBLANK)));

  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONBIGENVELOPE)));
  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONBIGENVELOPEA)));
  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONSMALLENVELOPE)));
  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONSMALLENVELOPEA)));
  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONMIDDLECONTENT)));
  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONLASTCONTENT)));
  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONPARCEL)));

  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONBIGENVELOPEDIS)));
  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONBIGENVELOPEADIS)));
  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONSMALLENVELOPEDIS)));
  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONSMALLENVELOPEADIS)));
  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONMIDDLECONTENTDIS)));
  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONLASTCONTENTDIS)));
  pImageList->Add(TIcon(GetApplication()->GetInstance(),
    TResId(IDI_ICONPARCELDIS)));

  if(pImageList->GetImageCount() != Ili_images)
    rT = false;
  SetImageList(*pImageList, TListWindow::Small);
  if(GetImageList(TListWindow::Small) == NULL)
    rT = false;
  if(rT)
  {
    int icons = 0;

    pStateImageList = new TImageList(TSize(16, 16), ILC_COLOR4, 2, 1);
    pStateImageList->Add(TIcon(GetApplication()->GetInstance(),
	TResId(IDI_ICONUNCHECK)
	));
    icons++;
    pStateImageList->Add(TIcon(GetApplication()->GetInstance(),
	TResId(IDI_ICONCHECK)
	));
    icons++;
    if(pStateImageList->GetImageCount() != icons)
	rT = false;
    SetImageList(*pStateImageList, TListWindow::State);
    if(GetImageList(TListWindow::State) == NULL)
      rT = false;
  }
  if(rT != true)
    error("%s::%s\n%s",
      typeid(*this).name(), "InitImageList", UNSUCCESSFUL_OPERATION);
  return rT;
}  // TRegMailListWindow::InitImageList

bool TRegMailListWindow::LoadData()
{
  const char* const methodName = "LoadData";
  bool rT = true;
  RegisteredMailListCriteria crit(rDlgData);

  if(pData && crit.Compose())
  {
    TWaitWindow ww(this, LOADING);

    {
      TQuery q(crit.Criteria());

      while(q.Read())
      {
        TCheckRegisteredMail* pNewRM = new TCheckRegisteredMail;

        (*pNewRM) << q;
	pData->Add(pNewRM);
      }
    }
  }
  else
    rT = false;
  if(rT == false)
    error("%s::%s\n%s",
	typeid(*this).name(), methodName, UNSUCCESSFUL_OPERATION);
  return rT;
}  // TRegMailListWindow::LoadData

bool TRegMailListWindow::CountsOk() const
{
  const bool res =
    pData && GetItemCount() == pData->Count();

  if(res == false)
    error("%s::%s\n%s",
	typeid(*this).name(), "CountsOk", UNSUCCESSFUL_OPERATION);
  return res;
}

bool TRegMailListWindow::InsertColumns()
{
	TListWindColumn lwc;

	lwc.SetText("Документ");
	lwc.SetWidth(AdjustedWidth(180));	// 185
	InsertColumn(0, lwc);

	lwc.SetText("Вид");
	lwc.SetWidth(AdjustedWidth(45));
	InsertColumn(1, lwc);

	lwc.SetText("Населено място");
	lwc.SetWidth(AdjustedWidth(130));	// 135
	InsertColumn(2, lwc);

	lwc.SetText("Получател");
	lwc.SetWidth(AdjustedWidth(235));	// 240
	InsertColumn(3, lwc);

	lwc.SetText("Забележка");
	lwc.SetWidth(AdjustedWidth(150));
	InsertColumn(4, lwc);

	return true;
}  // TRegMailListWindow::InsertColumns

bool TRegMailListWindow::InsertRows()
{
  bool rT = true;

  if(pData)
  {
    const int dItems = pData->Count();

    for(int i = 0; rT == true && i < dItems; i++)
    {  // -- 0 --
      int img = -1;
      const TRegisteredMail& rm = *((*pData)[i]);

      if(rm.envelopeNo)
      {  // Determine the appropriate icon
        bool firstInEnvelope = false;
        bool middleInEnvelope = false;
        bool lastInEnvelope = false;
        const bool canEnvelope = rm.CanEnvelope(false);
        const bool disabled = DisabledItem(rm);

        if(canEnvelope)
        {
          if(i == 0)
            firstInEnvelope = true;
          else
            if(rm.SameEnvelope(*((*pData)[i - 1])) == false)
              firstInEnvelope = true;
            else
		  if(i == pData->Count() - 1)
                lastInEnvelope = true;
              else
                if(rm.SameEnvelope(*((*pData)[i + 1])) == false)
                  lastInEnvelope = true;
                else
                  middleInEnvelope = true;
        }
        if(firstInEnvelope)
        {
          if(rm.flags == OUTREG_R)
            img = disabled ? Ili_BigEnvelopeDis : Ili_BigEnvelope;
          else
            if(rm.flags == OUTREG_AR)
              img = disabled ? Ili_BigEnvelopeADis : Ili_BigEnvelopeA;
        }
        else
          if(middleInEnvelope)
          {
            img = disabled ? Ili_MiddleContentDis : Ili_MiddleContent;
          }
          else
            if(lastInEnvelope)
            {
              img = disabled ? Ili_LastContentDis : Ili_LastContent;
            }
      }

      // At last - put it
      if(Put(i, img) == false)
        rT = false;
    }  // -- 0 --
  }
  else
    rT = false;
  if(rT)
    if(CountsOk() == false)
      rT = false;
  if(rT == false)
    error("%s::%s\n%s", typeid(*this).name(), "InsertRows",
	UNSUCCESSFUL_OPERATION);
  return rT;
}  // TRegMailListWindow::InsertRows

bool TRegMailListWindow::Put(const int item, const int imgInd)
{
  const char* const methodName = "Put";
  const int itemsCnt = GetItemCount();

  if(pData != NULL && 0 <= item && item <= itemsCnt &&
    0 <= item && item < pData->Count())
  {
    int subItem = 0;
    char tmp[256];
    const TCheckRegisteredMail& rm = *((*pData)[item]);

    {  // The item  (doc identification)
      TListWindItem lwi;
      const bool disabled = DisabledItem(rm);

      lwi.iItem = item;
      lwi.iSubItem = subItem;  // Not necessary, but let it be.
      tmp[0] = '\0';
      rm.PrintDoc(tmp, sizeof(tmp));
      lwi.SetText(tmp);
      if(pImageList &&  // 08.sep.2003 - begin
        GetImageList(TListWindow::Small) != NULL)
      {
        int img = imgInd;

        if(img == -1)
        {
          switch(rm.flags)
          {
            case OUTREG_R:
              img = disabled ? Ili_SmallEnvelopeDis : Ili_SmallEnvelope;
              break;
            case OUTREG_AR:
              img = disabled ? Ili_SmallEnvelopeADis : Ili_SmallEnvelopeA;
              break;
            case OUTREG_PACKAGE:
              img = disabled ? Ili_ParcelDis : Ili_Parcel;
              break;
          }
	  }
	  if(img < 0 || pImageList->GetImageCount() <= img)
          img = 0;
        if(0 <= img && img < pImageList->GetImageCount())
          lwi.SetImageIndex(img);
      }  // 08.sep.2003 - end
      if(pStateImageList && GetImageList(TListWindow::State) != NULL)
      {
	  {
	    lwi.mask |= LVIS_STATEIMAGEMASK;
	    lwi.state |=
		INDEXTOSTATEIMAGEMASK(rm.checked ? Si_Checked : Si_Unchecked);
	  }
	}
	InsertItem(lwi);
    }
    {  // The subitems
	LV_ITEM lvi;

	lvi.mask = 0;
	lvi.mask |= LVIF_TEXT;
	lvi.iItem = item;
	{  // The flags
	  lvi.iSubItem = ++subItem;
	  rm.PrintFlags(tmp, sizeof(tmp));
	  lvi.pszText = tmp;
	  SetItem(TListWindItem(lvi));
	}
	{  // The living place
	  lvi.iSubItem = ++subItem;
	  rm.PrintUclp(tmp, sizeof(tmp));
	  lvi.pszText = tmp;
	  SetItem(TListWindItem(lvi));
      }
	{  // The receiver
	  lvi.iSubItem = ++subItem;
        rm.PrintReceiver(tmp, sizeof(tmp));
        lvi.pszText = tmp;
	  SetItem(TListWindItem(lvi));
	}
	{  // The description
	  lvi.iSubItem = ++subItem;
	  strCopy(tmp, rm.text, sizeof(tmp));
	  lvi.pszText = tmp;
	  SetItem(TListWindItem(lvi));
	}
    }
    return true;
  }

  return error("%s\n%s::%s", WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
}  // TRegMailListWindow::Put

bool TRegMailListWindow::IsChecked(const int ind) const
{
  METHOD_NAME("IsChecked");
  bool checked = false;

  if(0 <= ind && ind < GetItemCount())
  {
    const uint state = GetItemState(ind, LVIS_STATEIMAGEMASK);
    uint desiredState = INDEXTOSTATEIMAGEMASK(Si_Checked);

    if((state & desiredState) == desiredState)
    {
      checked = true;
    }
  }
  else
    REPORT_M_INVALID_ARGS;
  return checked;
}  // TRegMailListWindow::IsChecked

int TRegMailListWindow::GetCheckCount(bool nonSentOnly) const
{
  int count = 0;

  for(int i = 0; i < GetItemCount(); i++)
  {
    if(IsChecked(i))
    {
      if(nonSentOnly == true)
      {
        if(pData != NULL && reg_mail_really_sent(*((*pData)[i])) == false)
        {
          count++;
        }
      }
      else
      {
        count++;
      }
    }
  }
  return count;
}  // TRegMailListWindow::GetCheckCount

int TRegMailListWindow::GetNonSentCheckCount() const
{
  return GetCheckCount(true);
}

bool TRegMailListWindow::DisabledItem(const TRegisteredMail& rm) const
{
  return (is_default_post_admin() &&
    rDlgData.wantCheckup == false &&  // 12.feb.2003
    rm.status == REGISTERED_MAIL_SENT && rm.send.Empty() == false);
}

bool TRegMailListWindow::SetItemCheck(const int ind, const bool checked,
  const bool affectData, bool* pAdminSlapped)
{
  METHOD_NAME("SetItemCheck");
  bool res = false;

  if(pAdminSlapped)
    *pAdminSlapped = false;
  if(0 <= ind && ind < GetItemCount())
  {
    bool adminSlapped = false;

    if(pStateImageList && GetImageList(TListWindow::State) != NULL &&
      pData != NULL && CountsOk() == true)
    {
      const UINT desiredState =
        INDEXTOSTATEIMAGEMASK(checked ? Si_Checked : Si_Unchecked);
      TCheckRegisteredMail& rm = *((*pData)[ind]);

      if(DisabledItem(rm) == false)
      {
        res = SetItemState(ind, desiredState, LVIS_STATEIMAGEMASK);
        if(res == true && affectData == true)
        {
          if(pData != NULL)
          {
            rm.checked = checked;
          }
          else
          {
            res = false;
          }
        }
      }
      else
      {
        adminSlapped = true;
      }
    }
    if(res == false && adminSlapped == false)
      REPORT_M_UNSUCCESSFUL;
    if(pAdminSlapped)
      *pAdminSlapped = adminSlapped;
  }
  else
    REPORT_M_INVALID_ARGS;
  return res;
}  // TRegMailListWindow::SetItemCheck

bool TRegMailListWindow::SetEnvelopeCheck(const int ind, const bool checked,
  const bool affectData, bool* pAdminSlapped)
{
  bool rT = true;
  METHOD_NAME("SetEnvelopeCheck");
  const int cnt = GetItemCount();

  if(0 <= ind && ind < cnt)
  {
    if(CountsOk() == true)
    {
	const int cnt = pData->Count();
      TRegisteredMail& rm = *((*pData)[ind]);

      for(int i = 0; rT == true && i < cnt; i++)
      {
        if(i == ind || rm.SameEnvelope(*((*pData)[i])) == true)
        {
          const bool res = SetItemCheck(i, checked, affectData, pAdminSlapped);

          if(res == false &&
            (pAdminSlapped == NULL || (*pAdminSlapped) == false))
          {
            rT = false;
          }
        }
      }
    }
    else
    {
      rT = false;
    }
    if(rT == false)
      REPORT_M_UNSUCCESSFUL;
  }
  else
  {
    rT = false;
    REPORT_M_INVALID_ARGS;
  }
  return rT;
}  // TRegMailListWindow::SetEnvelopeCheck

bool TRegMailListWindow::ToggleItem(const int ind, const bool affectData)
{
  METHOD_NAME("ToggleItem");
  bool adminSlapped = false;
  bool res = false;

  if(CountsOk() == true && 0 <= ind && ind < GetItemCount())
  {
    if((*pData)[ind]->envelopeNo != 0)
    {
      res = SetEnvelopeCheck(ind, IsChecked(ind) ? false : true, affectData,
        &adminSlapped);
    }
    else
    {
      res = SetItemCheck(ind, IsChecked(ind) ? false : true, affectData,
        &adminSlapped);
    }
    if(res == false && adminSlapped == false)
      REPORT_M_UNSUCCESSFUL;
  }
  else
    REPORT_M_INVALID_ARGS;
  return res;
}  // TRegMailListWindow::ToggleItem

int TRegMailListWindow::GetFocusIndex() const
{
  int ind = -1;

  for(int i = 0; ind == -1 && i < GetItemCount(); i++)
  {
    const int st = GetItemState(i, LVIF_STATE);

    if((st && LVIS_FOCUSED) == LVIS_FOCUSED)
    {
      ind = i;
    }
  }
  return ind;
}  // TRegMailListWindow::GetFocusIndex

int TRegMailListWindow::FindSubstring(int startingItem,
  const char* const text) const
{
  int found = -1;
  const int cnt = GetItemCount();

  if(cnt > 0 && text != NULL)
  {
    CharPtr buff(512);
    const size_t minLen = strlen(text);

    if(startingItem < 0 || cnt <= startingItem)
    {
      startingItem = 0;
    }
    // Search forwards from startingItem
    for(int i = startingItem; found == -1 && i < cnt; i++)
    {
      const int len = GetItemText(i, 0, buff, buff.Size());

      if(minLen <= len)
      {
        if((minLen == 0 && buff[0] == '\0') ||
          (minLen > 0 && strstr(buff, text) != NULL))
        {
          found = i;
        }
      }
    }
    // Search from the beginning torwards startingItem
    for(int i = 0; found == -1 && i < startingItem; i++)
    {
      const int len = GetItemText(i, 0, buff, buff.Size());

      if(minLen <= len)
      {
        if((minLen == 0 && buff[0] == '\0') ||
          (minLen > 0 && strstr(buff, text) != NULL))
        {
          found = i;
        }
      }
    }
  }
  return found;
}  // TRegMailListWindow::FindSubstring

bool TRegMailListWindow::ReDisplay()
{
  const bool res = DeleteAllItems() && InsertRows();

  if(res == false)
    error("%s::%s\n%s", typeid(*this).name(), "ReDisplay",
      UNSUCCESSFUL_OPERATION);
  return res;
}

bool TRegMailListWindow::ReArrange()
{
  bool rT = CountsOk();

  if(rT && pData->Count() > 0)
  {
    TISArray<TCheckRegisteredMail>* pTmp = new TISArray<TCheckRegisteredMail>();

    for(int i = pData->Count() - 1; rT && i >= 0; i--)
	pTmp->Add(pData->Detach(i));
    if(rT)
    {
      delete pData;
      pData = pTmp;
    }
    else
      delete pTmp;
  }
  if(rT)
    rT = ReDisplay();
  if(rT == false)
    error("%s::%s\n%s", typeid(*this).name(), "ReArrange",
      UNSUCCESSFUL_OPERATION);
  return rT;
}  //  TRegMailListWindow::ReArrange

bool TRegMailListWindow::EnvelopeJob()
{
  const char* const methodName = "EnvelopeJob";
  bool rT = CountsOk();
  const int sc = GetSelCount();
  const int itCnt = GetItemCount();

  if(rT && sc > 0)
  {
    TRegisteredMail commonTraits;
    int alreadyInEnvelopeCnt = 0;
    bool mess = false;
    bool reallySentDetected = false;

    {  // Detecting the common traits
      int firstSel = -1;
      int selPassed = 0;
      bool ctDetected = false;

      for(int i = 0; ctDetected == false && selPassed < sc && i < itCnt; i++)
      {
        if(IsSelected(i))
        {
          selPassed++;
          if(((*pData)[i])->envelopeNo != 0)
          {
            commonTraits << *((*pData)[i]);
            ctDetected = true;
          }
          else
          {
            firstSel = i;
          }
        }
      }
      if(ctDetected == false)
      {
        if(0 <= firstSel && firstSel < itCnt)
        {
          commonTraits << *((*pData)[firstSel]);
        }
        else
        {
          error("Cannot define common traits for envelope.");
          rT = false;
        }
      }
    }
    if(rT == true)
    {  // Checking the selection
      for(int i = 0; rT == true && mess == false && i < itCnt; i++)
      {
        if(IsSelected(i))
        {
          if(((*pData)[i])->envelopeNo != 0)
          {
            alreadyInEnvelopeCnt++;
          }
          if(reg_mail_really_sent(*((*pData)[i])) == true)
          {
            reallySentDetected = true;
          }
          if(((*pData)[i])->MayJoinEnvelope(commonTraits))
          {
            if(((*pData)[i])->envelopeNo != 0 &&
              commonTraits.envelopeNo != 0 &&
              ((*pData)[i])->envelopeNo != commonTraits.envelopeNo)
            {
              mess = true;
              error("Избрани са елементи от различни пликове.");
            }
            else
              if(((*pData)[i])->envelopeNo != 0 &&
                commonTraits.envelopeNo == 0)
              {
                commonTraits.envelopeNo = ((*pData)[i])->envelopeNo;
              }
          }
          else
          {
            mess = true;
            // error message in MayJoinEnvelope()
          }
        }
      }
      if(mess == false)
      {  // determine if the whole envelope is selected
        int fullEnvelopeCnt = 0;

        for(int ii = 0; ii < itCnt ; ii++)
        {
          if(commonTraits.SameEnvelope(*((*pData)[ii])))
            fullEnvelopeCnt++;
        }
        if(alreadyInEnvelopeCnt != 0 && fullEnvelopeCnt != alreadyInEnvelopeCnt)
        {
          mess = true;
          error("Не са избрани всички документи от плика.");
        }
      }
    }

    // Do something
    if(rT == true && mess == false)
    {
      if(alreadyInEnvelopeCnt == sc)
      {
        if(ask("Желаете ли разкомплектоване на плика?"))
        {
          rT = EnvelopeSelection(commonTraits, false, false);
        }
        else
        {
          mess = true;
        }
      }
      else
        if(alreadyInEnvelopeCnt == 0)
        {
          if(reallySentDetected == false)
          {
            if(sc > 1)
            {
              const TRegisteredMail tmp;
              long int envlpNo = tmp.NextEnvelopeNo(commonTraits);

              if(envlpNo)
              {
                commonTraits.envelopeNo = envlpNo;
                rT = EnvelopeSelection(commonTraits, true, true);
              }
              else
                rT = false;
            }
            else
            {
              error("За общ плик са нужни поне два елмента.");
              mess = true;
            }
          }
          else
          {
            error("Не може да се обединяват вече изпратени писма.");
            mess = true;
          }
        }
        else
        {
          if(ask("Желаете ли добавяне на допълнително избраните "
            "документи към плика?"))
          {
            rT = EnvelopeSelection(commonTraits, true, false);
          }
          else
          {
            mess = true;
          }
        }
    }

    if(rT == true && mess == false)
    {  // Refresh the list window
      DeselectAll();
      rT = ReArrange();
    }
  }
  if(rT == false)
    error("%s::%s\n%s", typeid(*this).name(), methodName,
      UNSUCCESSFUL_OPERATION);
  return rT;
}  // TRegMailListWindow::EnvelopeJob

bool TRegMailListWindow::EnvelopeSelection(const TRegisteredMail& commonTraits,
  const bool set, const bool newly)
{
  const char* const methodName = "EnvelopeSelection";
  bool rT = true;
  bool userCancel = false;
  bool parcelConfirmed = false;
  bool userInterference = false;
  const int itCnt = GetItemCount();

  for(int i = 0; rT == true && i < itCnt ; i++)
  {  // -- 0 --
    if(IsSelected(i) && ((*pData)[i])->MayJoinEnvelope(commonTraits))
    {  // -- 1 --
      if(set)
      {  // -- 2 --
        if(newly == true && parcelConfirmed == false &&
          ((*pData)[i])->flags == OUTREG_PACKAGE)
        {
          if(ask("Обединяване на колети.\n%s", WANT_CONTINUE_QUESTION) == true)
            parcelConfirmed = true;
          else
          {
            rT = false;
            userCancel = true;
          }
        }
        if(rT == true)
        {  // -- 3 --
          if(((*pData)[i])->envelopeNo == 0)
          {
            ((*pData)[i])->envelopeNo = commonTraits.envelopeNo;
		if(((*pData)[i])->Update("F_ENVELOPE_NO") == false)
		  rT = false;
	    }
	    else
		if(((*pData)[i])->envelopeNo != commonTraits.envelopeNo)
		{
		  rT = false;
		}
	  }  // -- 3 --
	}  // -- 2 --
	else
	{
	  ((*pData)[i])->envelopeNo = 0;
        if(((*pData)[i])->Update("F_ENVELOPE_NO") == false)
          rT = false;
      }
    }  // -- 1 --
  }  // -- 0 --
  if(rT == true)
  {  // -- 4 --  A paranoic check if anoher user (in this very moment)
    // has already used the same envelope number...
    char crit[512 + 256];
    {
      TRegisteredMail tmp;
      ostrstream oss(crit, sizeof(crit));

      oss << "SELECT COUNT(F_DOC_NO) FROM T_REGISTERED_MAIL ";
      tmp.EnvelopeWhereClause(oss, commonTraits);
      oss << " AND F_ENVELOPE_NO = " << (set ? commonTraits.envelopeNo : 0);
      oss << " ;" << ends;
    }
    ENDING_ZERO(crit);
    if(criteria_finished_ok(crit, methodName))
    {
      TQuery q(crit);
	TCountLongGroup count;

      count.count = 0;
      if(q.Read())
	{
        count << q;
        if(set)
        {
          if(count.count != GetSelCount())
            rT = false;
        }
        else
        {
          if(count.count != 0)
            rT = false;
        }
      }
      else
        rT = false;
      if(rT == false)
      {
        userInterference = true;
        error("Този общ плик съдържа %d писма (а не %d, както ще "
          "бъде показано).%s",
          count.count, GetSelCount(),
          count.count != 0 ?
            "\nМоже би друг потребител работи със същия получател." : ""
          );
      }
    }
    else
      rT = false;
  }  // -- 4 --
  if(rT == false && userCancel == false && userInterference == false)
    error("%s::%s\n%s", typeid(*this).name(), methodName,
      UNSUCCESSFUL_OPERATION);
  return rT;
}  // TRegMailListWindow::EnvelopeSelection

bool TRegMailListWindow::SaveSelection(const CDate sent)
{
  const char* const methodName = "SaveSelection";
  bool rT = sent.Valid();

  if(rT)
  {  // -- -1 --
    bool selOk = true;
    const int selCnt =
	GetCheckCount();

    rT = CountsOk();
    if(rT)
    {  // -- 0 --
	const int itCnt = GetItemCount();

	if(selCnt != 0)
	{  // -- 1 --
	  //selOk = CorrectEnvelopeSelection();  // 09.feb.2004
	  selOk = CorrectEnvelopeCheck();  // 09.feb.2004

	  if(selOk == true)
	  {  // -- 1.5 --
	    if(ask("Избрани са %d препоръчани писма за изпращане по пощата.\n"
		"Потвърждавате ли съхраняването им в базата данни?", selCnt))
	    {  // -- 2 --
		for(int i = 0; rT == true && i < itCnt ; i++)
		{  // -- 3 --
		  const bool updateIt =
		    is_default_post_admin() ?
			(IsChecked(i) == true &&
			DisabledItem(*((*pData)[i])) == false &&
			rDlgData.wantCheckup == false  // 12.feb.2003 assurance
			)
			: IsChecked(i);

		  if(updateIt)
		  {  // -- 4 --
		     ((*pData)[i])->status = REGISTERED_MAIL_SENT;
		     ((*pData)[i])->send = sent;
		     if(((*pData)[i])->Update("F_STATUS, F_SEND") == true)
		     {
			((*pData)[i])->checked = false;
		     }
		     else
		     {
			rT = false;
		     }
		  }  // -- 4 --
		}  // -- 3 --
	    }  // -- 2 --
	  }  // -- 1.5 --
	  else
	    rT = false;
	}  // -- 1 --
	else
	{
	  rT = false;
	  say_no_selection();
	}
    }  // -- 0 --
    if(rT == false && selOk == true && selCnt != 0)
	error("%s::%s\n%s", typeid(*this).name(), methodName,
	  UNSUCCESSFUL_OPERATION);
  }  // -- -1 --
  else
    error("%s\n%s::%s", WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
  return rT;
}  // TRegMailListWindow::SaveSelection

bool TRegMailListWindow::CorrectEnvelopeSelectionOrCheck(const bool check,
  const bool errMsg) const
{
  // I assume that tle list window is correctly sorted.
  bool rT = CountsOk();
  const int selCnt = check ? GetCheckCount() : GetSelCount();
  const int itCnt = GetItemCount();
  bool programError = false;

  if(rT)
  {
    TRegisteredMail commonTraits;
    bool ctValid = false;  // common traits valid
    int ctSelCnt = 0;  // common selection count
    int selItemsPassed = 0;
    int i = 0;

    clearGroupData(commonTraits);
    while(rT == true && selItemsPassed < selCnt)
    {
      bool envelopeLeft = false;
      bool envlpLeftAndNextEnvlpReached = false;  // 25.feb.2004

      if(check ? IsChecked(i) : IsSelected(i))
      {
        if(ctValid == true)
        {
          if(commonTraits.SameEnvelope(*((*pData)[i])) == true)
          {
            ctSelCnt++;
          }
          else
          {
            envelopeLeft = true;  // and ctValid == true
            if(((*pData)[i])->envelopeNo != 0)  // 25.feb.2004 - begin
            {
              envlpLeftAndNextEnvlpReached = true;
            }  // 25.feb.2004 - end
          }
        }
        else
        {
          if((*pData)[i]->envelopeNo != 0)
          {
            ctValid = true;  // and envelopeLeft == false
            ctSelCnt = 1;
            commonTraits << *((*pData)[i]);
          }
        }
        selItemsPassed++;
      }
      else
      {
        if(ctValid == true)
          envelopeLeft = true;  // and ctValid == true
      }
      if(ctValid == true && ctSelCnt != 0 && commonTraits.doc.type != '\0' &&
        (i == itCnt - 1 || selItemsPassed == selCnt))
      {
        envelopeLeft = true;  // and ctValid == true
      }
      if(envelopeLeft == true)
      {
        if(ctValid == true && ctSelCnt != 0 && commonTraits.doc.type != '\0')
        {
          int envItems = 0;

          for(int j = 0; j < itCnt; j++)
          {
            if(commonTraits.SameEnvelope(*((*pData)[j])) == true)
              envItems++;
          }
          if(envItems != ctSelCnt)
            rT = false;  // The incorrect selection is registered here.
        }
        else
        {
          programError = true;
          rT = false;
        }
        ctValid = false;  // envelopeLeft == true, Ok here
        ctSelCnt = 0;
        clearGroupData(commonTraits);
        if(envlpLeftAndNextEnvlpReached == true)  // 25.feb.2004 - begin
        {
          ctValid = true;  // envelopeLeft == true, Ok here
          ctSelCnt = 1;
          commonTraits << *((*pData)[i]);
        }  // 25.feb.2004 - end
      }
      i++;
    }
    if(rT == false)
    {
      if(programError == true)
      {
        error("%s::%s\n%s", typeid(*this).name(), "CorrectEnvelopeSelection",
          UNSUCCESSFUL_OPERATION);
      }
      else
        if(errMsg == true)
          error("Има %s и не%s елементи от един и същ плик.",
            check ? "маркирани" : "избрани", check ? "маркирани" : "избрани");

    }
  }
  return rT;
}  // TRegMailListWindow::CorrectEnvelopeSelectionOrCheck

bool TRegMailListWindow::CorrectEnvelopeSelection(const bool errMsg) const
{
  return CorrectEnvelopeSelectionOrCheck(false, errMsg);
}  // TRegMailListWindow::CorrectEnvelopeSelection

bool TRegMailListWindow::CorrectEnvelopeCheck(const bool errMsg) const
{
  return CorrectEnvelopeSelectionOrCheck(true, errMsg);
}  // TRegMailListWindow::CorrectEnvelopeCheck

bool TRegMailListWindow::EnvelopeMemberSelected() const
{
  bool emSelected = false;

  if(CountsOk() == true)
  {
    if(GetSelCount() > 0)
    {
      for(int i = 0; emSelected == false && i < GetItemCount(); i++)
      {
        if(IsSelected(i) && (*pData)[i]->envelopeNo != 0)
        {
          emSelected = true;
        }
      }
    }
  }
  return emSelected;
}  // TRegMailListWindow::EnvelopeMemberSelected

bool TRegMailListWindow::SentItemSelected() const
{
  bool siSelected = false;

  if(CountsOk() == true)
  {
    if(GetSelCount() > 0)
    {
      for(int i = 0; siSelected == false && i < GetItemCount(); i++)
      {
	if(IsSelected(i) && reg_mail_really_sent(*((*pData)[i])) == true)
	{
	  siSelected = true;
	}
      }
    }
  }
  return siSelected;
}  // TRegMailListWindow::SentItemSelected

char TRegMailListWindow::GetDoc(TRCDKeyGroup &aKey)
{
	mstr insertTypes(rDlgData.mailTypes);
	insertTypes.cat(TYPE_REQUESTS);
	insertTypes.cat(TYPE_OUTREGS);

	char college = '\0';
	TRCDKeyGroup *const group = get_group(this, NULL, str(insertTypes), NULL, match_null);
	TVirtPtr VP(group);

	if (group)
	{
	#if TESTVER
		// re-enable "F_TEXT: import" crash, added outregs are stored with lawsuitType='c' (added lawsuits too)
		//TBaseString *text = (TBaseString *) group->Find("F_TEXT");
		//if (text)
		//	text->SetS("");
		// or simply aKey.key = group->key;
	#endif  // TESTVER
		college = find_college((const TRCDKeyContainer *) group);
		aKey << group;
	}

	return college;
}

bool TRegMailListWindow::GetReceiver(TRegisteredMail& rm)
{
  bool res = false;

  if(TRCDKey_(rm.doc).FilledOK())
  {
    TFoundGroup ucnGroup;
    CharPtr tmp(strlen(UCN_DATED_SIDES) + 1);
    char tc[2];

    strCopy(tmp, UCN_DATED_SIDES, tmp.Size());
    tc[1] = '\0';
    tc[0] = UCN_ANONYMOUS_CODE;
    difference(tmp, tc);
    tc[0] = UCN_UNKNOWN_CODE;
    difference(tmp, tc);
    if(get_ucn(this, &ucnGroup, tmp))
    {  // -- 3 --
      strCopy(rm.ucn, ucnGroup.ucn, sizeof(rm.ucn));
      rm.ucnType = ucnGroup.ucnType;
      res = true;
    }
  }
  return res;
}  // TRegMailListWindow::GetReceiver

bool TRegMailListWindow::GetOutregReceivers(const TRCDKey key,
  TGArray<TUCNGroup>& receivers)
{
  const int oldCount = receivers.Count();

  if(TRCDKey_(key).FilledOK() && key.type == TYPE_OUTREG)
  {  // Keep this logic in conformity with
    // AppealedLawsOutRegColumns::AddReceiversToColumn
    TOutReg outReg;

    outReg.key = key;
    if(outReg.Get())
    {
      if(ucnPresent(outReg.receiver))
      {
        TSender* pNewSender = new TSender;

        strCopy(pNewSender->ucn, outReg.receiver, sizeof(pNewSender->ucn));
        // 20061220 pNewSender->ucnType = UCN_INSTANCE_THIS;
        pNewSender->ucnType = outReg.receiverType;
        receivers.Add(pNewSender);
      }
      else
      {
        bool walnsChecked = false ;
        bool walnReceiver = false ;

        if(DatedContainer_(outReg).FilledOK())
        {  // -- 0 --
	    TSideWind waln;
	    CharPtr buff(2 * 1024);

          waln << outReg;
	    waln.FloatCriteria(buff);
          {  // -- 1 --
            TQuery q(buff);

            walnsChecked = true;
            while(q.Read())
            {  // -- 2 --
		  TSideWind* pNewWaln = new TSideWind;
              (*pNewWaln) << q;
		  receivers.Add(pNewWaln);
              walnReceiver = true;
            }  // -- 2 --
          }  // -- 1 --
        }  // -- 0 --
        if(walnReceiver == false)
          if(outrg_retLawToLowerInst_weakCond(outReg, walnsChecked == false))
            if(TRCDKey_(outReg.lawsuit).FilledOK())
            {
              LawPtr pLaw ;

              pLaw.Ref().key = outReg.lawsuit;
              if(pLaw.Ref().Get())
              {
# if INSTANCE
#   if APPEAL
                {
                  TLawsuit * pLawsuit = new TLawsuit ;
                  pLawsuit -> key = pLaw.Ref().key;
                  TConnect *c = main_connect( pLawsuit );
                  if ( c != NULL )
                    if ( c -> connectType == UCN_INSTANCE_LOWER )
                    {
                      TSender* pNewSender = new TSender;

                      strCopy(pNewSender->ucn, c -> ucn,
                              sizeof(pNewSender->ucn));
                      pNewSender->ucnType = c -> ucnType;
                      receivers.Add(pNewSender);
                    }

                  delete c;
                  delete pLawsuit;
                }
#   else  // of APPEAL
                {
                  // 20070111 comment
                  // if(pLaw.Ref().regionKind && pLaw.Ref().regionNo &&
                  //    pLaw.Ref().regionYear)
                  {
			  const char senderUcnType =
                      sourceTypeToSenderUCNType(pLaw.Ref(), pLaw.Ref().kind);

                    if(senderUcnType == UCN_INSTANCE_LOWER)
                    {
                      // 20070111  -->
                      // Взаимствана е логиката на sourceTypeToSenderUCNType (const TLawsuit & rLawsuit)
                      if( strchr (KIND_2ND_LAWSUITS, pLaw.Ref().kind) // "WCUHП" "N" "n" "Я" ""
                                                                      // ВНОХД, ВНЧХД, КНАХД, ВЧНД, ВНАХД,
                                                                      // Гражданско дело (В), Търговско дело (В), Административно дело (К)
				  &&
                         (pLaw.Ref().source.type!=TYPE_JURISD) )      // 'j' - Пристигнал документ
                      { // search connect
                       	TGArray<TConnect> connects;
                        TConnect *connect = new TConnect;
                       	if (load_connects(pLaw, &connects))
                        {
                      		  for (int i = 0; i < connects.Count(); i++)
                      		  {
                       			  *connect << connects[i];
                              TSender* pNewSender = new TSender;
                              strCopy(pNewSender->ucn, connect -> ucn,
                                      sizeof(pNewSender->ucn));
                              pNewSender->ucnType = connect -> ucnType;
                              receivers.Add(pNewSender);
                      			}
                      	}
                      	delete connect;
                      }
                    }
                  }
                }
#   endif  // of APPEAL
# else  // of INSTANCE
# endif  // of INSTANCE
              }
            }
      }
    }
    if(oldCount >= receivers.Count())
    {
      char tmp[128];

      TRCDKey_(key).Print(tmp, sizeof(tmp));
      error("%s:\nЛипсва получател", tmp);
    }
  }
  else
    error("%s\n%s::%s",
	WRONG_ARGS_PASSED_TO, typeid(*this).name(), "GetOutregReceivers");
  return (oldCount < receivers.Count());
}  // TRegMailListWindow::GetOutregReceivers

bool TRegMailListWindow::GetAddress(TRegisteredMail& rm)
{
  bool res = false;

  if(ucnPresent(rm.ucn) && rm.ucnType)
  {
    TAddress addr;

    // Create an addres if none exists.
    strCopy(addr.ucn, rm.ucn, sizeof(addr.ucn));
    addr.ucnType = rm.ucnType;
    if(addr.Try() == false)
    {
	strCopy(addr.ucn, rm.ucn, sizeof(addr.ucn));
	addr.ucnType = rm.ucnType;
	addr.Insert();
    }
    // Edit any address
    if(get_address(this, rm.ucn, rm.ucnType, &addr))
    {
      rm << addr;
      res = true;
    }
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), "GetAddress");
  return res;
}  // TRegMailListWindow::GetAddress

bool TRegMailListWindow::GetFlags(TRegisteredMail& rm)
{
  int choosen = 0;
  bool res = false;

  rm.flags = 0;
  if(T3RadiosDialog(this, choosen, "Начин на изпращане",
    	"Изпращане", "R", "AR", "Колет").Execute() == IDOK)
  {
    res = true;
    switch(choosen)
    {
      case 0:
        rm.flags = OUTREG_R;
        break;
      case 1:
        rm.flags = OUTREG_AR;
        break;
      case 2:
        rm.flags = OUTREG_PACKAGE;
        break;
      default:
        res = false;
        break;
    }
  }
  return res;
}  // TRegMailListWindow::GetFlags

bool TRegMailListWindow::InsertOutreg(const char lawType, const TRCDKey key)
{
  const char* const methodName = "InsertOutreg";
  bool allInserted = true;
  bool rT = true;

  if(pData && lawType && TRCDKey_(key).FilledOK() && key.type == TYPE_OUTREG)
  {
    TOutReg outReg;

    outReg.key = key;
    if(outReg.Get())
    {  // -- outReg.Get() --
      if(outReg.flags == OUTREG_R || outReg.flags == OUTREG_AR ||
        outReg.flags == OUTREG_PACKAGE)
      {  // -- outReg.flags --
        TRegisteredMail common;

        common.doc = key;
        common.lawsuitType = lawType;
        common.flags = outReg.flags;
        if(TMailDocDialog(this, common, false).Execute() == IDOK)
        {
          TGArray<TUCNGroup> receivers;

          if(GetOutregReceivers(key, receivers))
          {
		for(int i = 0; i < receivers.Count(); i++)
            {
              bool currentInserted = false;
              strCopy(common.ucn, receivers[i]->ucn, sizeof(common.ucn));
              common.ucnType = receivers[i]->ucnType;
              {
                TAddress addr;

                strCopy(addr.ucn, common.ucn, sizeof(addr.ucn));
                addr.ucnType = common.ucnType;
                if(addr.Get())
                  common << addr;
                else
                  rT = false;
              }
              {
                TCheckRegisteredMail* pNewRM = new TCheckRegisteredMail;

                (*pNewRM) << common;
                if(pNewRM->Insert())
                {
                  currentInserted = true;
		  pData->Add(pNewRM);
                  {
                    const int addIndex = pData->Find(pNewRM);

                    if(Put(addIndex))
                    {
                      SetSel(addIndex, true);
                      EnsureVisible(addIndex, false);  // 05.mar.2004
                    }
                    else
                      rT = false;
		  }
		}
                else
		{
                  rT = false;
                  allInserted = false;
                }
                if(currentInserted == false)
                  delete pNewRM;
              }
            }
          }
          if(allInserted == false || rT == false)
            error("%s::%s\n%s",
              typeid(*this).name(), methodName, UNSUCCESSFUL_OPERATION);
        }
      }  // -- outReg.flags --
      else
        rT = error("Документът не е изпратен като "
          "\"R\", \"AR\" или \"Колет\".");
    }  // -- outReg.Get() --
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
  return (allInserted && rT);
}  // TRegMailListWindow::InsertOutreg

bool TRegMailListWindow::InsertNonOutreg(const char lawType, const TRCDKey key)
{
  bool inserted = false;

  if(pData && lawType && TRCDKey_(key).FilledOK() && key.type != TYPE_OUTREG)
  {
    TCheckRegisteredMail* pNewRM = new TCheckRegisteredMail();

    //clearGroupData(*pNewRM);
    pNewRM->date = Today;
    pNewRM->doc = key;
    pNewRM->lawsuitType = lawType;
    //pNewRM->status = REGISTERED_MAIL_PREPARED;
    if(TMailDocDialog(this, *pNewRM, false).Execute() == IDOK)
    {
      if(pNewRM->Insert())
      {
	pData->Add(pNewRM);
	{
          inserted = true;
          {
            const int addIndex = pData->Find(pNewRM);

            Put(addIndex);
            SetSel(addIndex, true);
            EnsureVisible(addIndex, false);  // 05.mar.2004
          }
        }
      }
    }
    if(inserted == false)
      delete pNewRM;
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), "InsertNonOutreg");
  return inserted;
}  // TRegMailListWindow::InsertNonOutreg

bool TRegMailListWindow::InsertLetter()
{
  bool inserted = false;

  DeselectAll();
  if(CountsOk())
  {
    char lawType;
    TRCDKey choosenKey;
    {
      LawPtr pLaw;
      TRCDKeyGroup& aKey = pLaw.Ref();  // Whatever
        // inheritor of TRDCKeyGroup...
      lawType = GetDoc(aKey);

      if(lawType)
        choosenKey = aKey.key;
      else
	  choosenKey.year = choosenKey.no = choosenKey.type = '\0';
    }
    if(lawType && choosenKey.type)
    {
      if(choosenKey.type == TYPE_OUTREG)
        inserted = InsertOutreg(lawType, choosenKey);
      else
        inserted = InsertNonOutreg(lawType, choosenKey);
    }
  }
  return inserted;
}  // TRegMailListWindow::InsertLetter

bool TRegMailListWindow::InsertLetterEx()
{
  bool inserted = false;
  bool put = false;

  DeselectAll();
  if(pData && CountsOk())
  {
    TRCDKey choosenKey;
    char lawType;
    {
      LawPtr pLaw;
      TRCDKeyGroup& aKey = pLaw.Ref();  // Whatever
        // inheritor of TRDCKeyGroup...
      lawType = GetDoc(aKey);

      if(lawType)
        choosenKey = aKey.key;
      else
        choosenKey.year = choosenKey.no = choosenKey.type = '\0';
    }
    if(lawType && choosenKey.type)
    {  // -- 2 --
      TCheckRegisteredMail* pNewRM = new TCheckRegisteredMail();

      clearGroupData(*pNewRM);
      pNewRM->date = Today;
      pNewRM->doc = choosenKey;
      pNewRM->lawsuitType = lawType;
      pNewRM->status = REGISTERED_MAIL_PREPARED;
      if(GetReceiver(*pNewRM))
      {  // -- 3 --
	  if(GetAddress(*pNewRM))
        {
          if(GetFlags(*pNewRM))
          {
            TAnyShortTextDialog(this, pNewRM->text, sizeof(pNewRM->text),
              "Пояснителен текст").Execute();
            if(pNewRM->Insert())
            {
	      pData->Add(pNewRM);
	      {
                inserted = true;
                {
                  const int addIndex = pData->Find(pNewRM);

                  put = Put(addIndex);
                  SetSel(addIndex, true);
                  EnsureVisible(addIndex, false);  // 05.mar.2004
                }
              }
            }
          }
        }
      }  // -- 3 --
      if(inserted == false)
        delete pNewRM;
    }  // -- 2 --
  }
  return (inserted && put);
}  // TRegMailListWindow::InsertLetterEx

bool TRegMailListWindow::EditAny(const int ind, bool& reArranged)
{
  const char* const methodName = "EditAny";
  bool res = false;

  reArranged = false;
  if(0 <= ind && ind <= GetItemCount())
  {
    TRegisteredMail& rm = *((*pData)[ind]);

    if(rm.KeyOk())
    {
      TRegisteredMail ctrl;       // 16.feb.2004 - begin

      ctrl << rm;                 // 16.feb.2004 - end
      if(TMailDocDialog(this, rm, true).Execute() == IDOK)
      {
        if(ctrl.ucnType == rm.ucnType &&  // 16.feb.2004 - begin
          strcmp(ctrl.ucn, rm.ucn) == 0)
        {                                // 16.feb.2004 - end
          if(rm.Update())
          {
            res = ReDisplay();
          }
        }                                // 16.feb.2004 - begin
        else
        {
          if(ctrl.Delete(true) && rm.Insert())
          {
            reArranged = res = ReArrange();
          }
        }                                // 16.feb.2004 - end
        if(res == false)
          error("%s::%s\n%s",
            typeid(*this).name(), methodName, UNSUCCESSFUL_OPERATION);
      }
    }
  }
  else
    error("%s\n%s::%s",
      WRONG_ARGS_PASSED_TO, typeid(*this).name(), methodName);
  return res;
}  // TRegMailListWindow::EditAny

bool TRegMailListWindow::EditOutreg(const int ind, bool& reArranged)
{
  return EditAny(ind, reArranged);
}  // TRegMailListWindow::EditOutreg

bool TRegMailListWindow::EditNonOutreg(const int ind, bool& reArranged)
{
  return EditAny(ind, reArranged);
}  // TRegMailListWindow::EditNonOutreg

bool TRegMailListWindow::EditSelected()
{
  bool res = false;

  if(CountsOk())
  {
    int selInd = -1;
    int selCnt = 0;
    const int itCnt = GetItemCount();

    for(int i = 0; selCnt < 1 && i < itCnt; i++)
    {
      if(IsSelected(i))
      {
        selInd = i;
        selCnt++;
      }
    }
    if(selCnt == 1 && 0 <= selInd && selInd < itCnt)
    {
      TRegisteredMail& rm = *((*pData)[selInd]);
      TRegisteredMail ctrl;
      bool reArranged = false;

      ctrl << rm;
      DeselectAll();
      if(rm.doc.type == TYPE_OUTREG)
        res = EditOutreg(selInd, reArranged);
      else
        res = EditNonOutreg(selInd, reArranged);
      if(res == true && (ctrl == rm) == false && reArranged == false)
      {
        res = ReArrange();  // To keep sort proper
      }
    }
  }
  return res;
}  // TRegMailListWindow::EditSelected

bool TRegMailListWindow::DeselectAll()
{
  bool rT = true;

  for(int i = 0; i < GetItemCount(); i++)
    if(IsSelected(i))
      if(SetSel(i, false) == false)
        rT = false;
  if(rT == false)
    error("%s::%s\n%s", typeid(*this).name(), "DeselectAll",
      UNSUCCESSFUL_OPERATION);
  return rT;
}

static void prt_reg_mail_critical_fields(const TRegisteredMail& rmi,
  char* const dest, const size_t destSz)
{  // 04.mar.2004
  if(dest && destSz > 0)
  {
    if(destSz > 0)
    {
      ostrstream oss(dest, destSz);

      // doc, date
      oss << TRCDKey_(rmi.doc) << ", " << rmi.date << ", ";
      {  // ucn ant ucnType
        char tmp[SIZE_OF_NAME_FOR_BOOK];

        if(ucnAndUcnTypeToName(rmi.ucn, rmi.ucnType, tmp, sizeof(tmp)) == false)
        {
          if(rmi.ucnType)
          {
            TUCNType ucnt;

            ZERO_LEN(tmp);
            ucnt.ucnType = rmi.ucnType;
            if(ucnt.Get())
            {
              strConcat(tmp, ucnt.name, sizeof(tmp));
            }
            else
            {
              strConcat(tmp, "'", sizeof(tmp));
              strConcatCh(tmp, rmi.ucnType, sizeof(tmp));
              strConcat(tmp, "'", sizeof(tmp));
            }
          }
          else
          {
            strConcat(tmp, "'\\0'", sizeof(tmp));
          }
          strConcat(tmp, " \"", sizeof(tmp));
          strConcat(tmp, rmi.ucn, sizeof(tmp));
          strConcat(tmp, "\"", sizeof(tmp));
        }
        oss << tmp << ", ";
      }
      // letterNo, lawsuitType
      oss << rmi.letterNo << ", " << Type_(rmi.lawsuitType) << ", ";
      // status
      switch(rmi.status)
      {
        case REGISTERED_MAIL_PREPARED:
          oss << "Подготвено";
          break;
        case REGISTERED_MAIL_SENT:
          oss << "Изпратено";
          break;
        case '\0':
          oss << "'\\0'";
          break;
        default:
          oss << "'" << rmi.status << "'";
          break;
      }
      oss << ends;
    }
    dest[destSz - 1] = '\0';
  }

}  // prt_reg_mail_critical_fields

bool TRegMailListWindow::DeleteSelected()
{
  bool rT = true;
  const char* const methodName = "DeleteSelected";

  if(pData && CountsOk())
  {
    const int selCnt = GetSelCount();

    if(selCnt > 0)
    {
      bool goAhead = false;
      const char* const wantDel = "Желаете ли да изтриете избрани";

      if(SentItemSelected() == false)
      {
        if(EnvelopeMemberSelected() == false)
        {
          if(selCnt == 1)
          {
            if(ask("%s%s елемент?", wantDel, "я"))
              goAhead = true;
          }
          else
          {
            if(ask("Изтриване на множество елементи.\n\n""%s%s %d елемента?",
              wantDel, "те", selCnt))
              goAhead = true;
          }
        }
        else
        {
          error("Не могат да се изтриват елементи от общ плик. "
            "Ако трябва да направите това, първо разкомплектовайте плика.");
        }
      }
      else
      {
        error("Не могат да се изтриват вече изпратени писма.");
      }
      if(goAhead)
      {
        for(int i = GetItemCount() - 1; rT && i >= 0 ; i--)
        {
          if(IsSelected(i))
          {
            if((*pData)[i]->Delete(true))
            {
		  DeleteAnItem(i);
		  pData->Remove(i);
		}
		else
            {
              rT = false;
              // 04.mar.2004 - begin  // on debugging purposes
              //error("%s::%s\n%s::Delete(true) failed.",
		  //  typeid(*this).name(), methodName,
              //  typeid(*((*pData)[i])).name(), i);
              char memDescr[256];
              char dbDescr[256];
              TRegisteredMail& rmi = (*((*pData)[i]));

              prt_reg_mail_critical_fields(rmi, memDescr, sizeof(memDescr));
              {
                TRegisteredMail dbrm;

                dbrm << rmi;
                if(dbrm.KeyOk() && dbrm.Get())
                {
                  prt_reg_mail_critical_fields(dbrm, dbDescr, sizeof(dbDescr));
                }
                else
                {
                  strCopy(dbDescr, "?", sizeof(dbDescr));
                }
              }
              error("%s::%s\n"
                "%s::Delete(true) failed.\n"
                "Memory object: %s\n"
                "Database object: %s",
                typeid(*this).name(), methodName,
                typeid(*((*pData)[i])).name(),
                memDescr,
                dbDescr);
              // 04.mar.2004 - end
            }
          }
        }
      }
    }
    else
      say_no_selection();
  }
  else
    rT = false;
  if(rT == false)
    error("%s::%s\n%s", typeid(*this).name(), methodName,
      UNSUCCESSFUL_OPERATION);
  return rT;
}  // TRegMailListWindow::DeleteSelected

const TISArray<TCheckRegisteredMail>& TRegMailListWindow::DataTmp() const
{
  if(pData == NULL)
    fatal("%s::%s\npData is NULL", typeid(*this).name(), "DataTmp");
  CountsOk();  // Provocate the error message (if there is something wrong)
  return (*pData);
}

TISArray<TCheckRegisteredMail>& TRegMailListWindow::NonConstDataTmp()
{
  if(pData == NULL)
    fatal("%s::%s\npData is NULL", typeid(*this).name(), "NonConstDataTmp");
  CountsOk();  // Provocate the error message (if there is something wrong)
  return (*pData);
}

void TRegMailListWindow::EvLButtonDown(uint modKeys, TPoint& point)
{
  // Port from Visual C++ source
  bool checkMarkHit = false;
  TLwHitTestInfo hitTestInfo(point);
  const int item = HitTest(hitTestInfo);

  {
    if(0 <= item && item < GetItemCount())
    {
	const uint flags = hitTestInfo.GetFlags();

      // we need additional checking in owner-draw mode
      // because we only get LVHT_ONITEM
      if(flags == LVHT_ONITEM && (GetStyle() & LVS_OWNERDRAWFIXED))
      {
        TRect rect;

        if(GetItemRect(item, &rect, Icon))
        {
          // check if hit was on a state icon
          if(point.x < rect.left)
          {
            checkMarkHit = true;
          }
        }
      }
      else
        if(flags & LVHT_ONITEMSTATEICON)
        {
          checkMarkHit = true;
        }
    }
  }
  if(checkMarkHit == true)
  {
    ToggleItem(item, true);
  }
  else
  {
    TListWindow::EvLButtonDown(modKeys, point);
  }
}  // TRegMailListWindow::EvLButtonDown

void TRegMailListWindow::EvVScroll(uint scrollCode, uint thumbPos,
  THandle hWndCtl)
{
  TResizeListWindow::EvVScroll(scrollCode, thumbPos, hWndCtl);
  {
    if(Parent)
      if(Parent->IsWindow())
        Parent->PostMessage(WM_LWN_VSCROLLED, 0,
          reinterpret_cast<long>(HWindow));
  }
}

// ------------------------ TArrowStatic - begin ------------------------

class TArrowStatic: public TStatic
{
public:  // Construction / Destruction
  TArrowStatic(TWindow* parent, int resourceId, uint textLimit = 0);
public:  // Methods
  virtual void ODADrawEntire(DRAWITEMSTRUCT far& drawInfo);
  virtual void ODAFocus(DRAWITEMSTRUCT far& drawInfo);
  virtual void ODASelect(DRAWITEMSTRUCT far& drawInfo);
};  // class TArrowStatic


TArrowStatic::TArrowStatic(TWindow* parent, int resourceId, uint textLimit):
  TStatic(parent, resourceId, textLimit)
{
}

void TArrowStatic::ODADrawEntire(DRAWITEMSTRUCT far& drawInfo)
{
  int n = drawInfo.rcItem.bottom - drawInfo.rcItem.top;
  n -= n % 2;

  if (n >= 2)
  {
    const POINT points[] = { { 0, 0 }, { n / 2, n / 2 }, { 0, n, }, { 0, 0 } };
    HRGN rgn = ::CreatePolygonRgn(points, sizeof points / sizeof points[0],
	ALTERNATE);  // WINDING - the other choice

    if(rgn != NULL)
    {
	TDC dc(drawInfo.hDC);

	if(dc.GetHDC() != NULL)
	{
	  ::FillRgn(dc.GetHDC(), rgn, (HBRUSH) GetStockObject(BLACK_BRUSH));
	  dc.SelectStockObject(WHITE_PEN);
	  ::Polyline(dc.GetHDC(), points, sizeof points / sizeof points[0]);
	}
    }
  }
}  // TArrowStatic::ODADrawEntire

void TArrowStatic::ODAFocus(DRAWITEMSTRUCT far& drawInfo)
{
  ODADrawEntire(drawInfo);
}

void TArrowStatic::ODASelect(DRAWITEMSTRUCT far& drawInfo)
{
  ODADrawEntire(drawInfo);
}

// ------------------------ TArrowStatic - end ------------------------


// ------------------------ TMailListDialog - begin ------------------------

class TMailListDialog: public TResizeDialog
{
  public:  // Construction / Destruction
    TMailListDialog(TWindow* const parent, const MailListDlgData& dlgDataRef);
  public:  // Methods
    virtual void SetupWindow();
  protected:  // Methods
    void AddBnClicked();
    void EditBnClicked();
    void DeleteBnClicked();
    void PrintBnClicked();
    void EnvelopeBnClicked();
    void SearchBnClicked();
    void OkBnClicked();
    void CancelBnClicked();  // 09.mar.2004 - begin
    void EvClose();
    bool MayQuit();  // 09.mar.2004 - end
    int32 ButtonRightClicked(TParam1, TParam2 bnHandle);
    int32 LWndVScrolled(TParam1, TParam2 lwndHandle);
    void OrdLabelEdited(TLwDispInfoNotify& nmHdr);
    void EvSize(uint sizeType, TSize& size);
    void RegLstItemChanged(TLwNotify&);
    void SearchTextChanged();
    void EnableDisableRegButtons();
    void SetDescription();
    void RefreshRegMailLabel();
    void PrintSelection();
    void FollowLastFoundItem();
  protected:  // Data
    const MailListDlgData& rDlgData;
    char txtToSearch[128];
    int lastFoundIndex;
  private:
    // 2009:065 LPR: eliminating GetDlgItemPtr
    TButton *idcancel;
    TStatic *ml_descr_static;
    TButton *ml_add_registered;
    TButton *ml_edit_registered;
    TButton *ml_delete_registered;
    TStatic *ml_registered_label;
    TRegMailListWindow *ml_registered;
    TDateFace *ml_date_sent;
    TButton *ml_print;
    TOrdMailListWindow *ml_ordinary;
    TStringFace *ml_searchtext;
    TArrowStatic *ml_searchmark;
    TButton *idok;
    TButton *ml_envelope;
    virtual void Resize();	// 2009:127 LPR: new resizing
DECLARE_RESPONSE_TABLE(TMailListDialog);
};  // class TMailListDialog

DEFINE_RESPONSE_TABLE1(TMailListDialog, TResizeDialog)
  EV_WM_SIZE,
  EV_BN_CLICKED(IDADD, AddBnClicked),
  EV_BN_CLICKED(IDEDIT, EditBnClicked),
  EV_BN_CLICKED(IDDELETE, DeleteBnClicked),
  EV_BN_CLICKED(IDPRINT, PrintBnClicked),
  EV_BN_CLICKED(IDC_MAIL_LIST_ENVELOPE, EnvelopeBnClicked),
  EV_BN_CLICKED(IDC_MAIL_LIST_SEARCH, SearchBnClicked),
  EV_BN_CLICKED(IDOK, OkBnClicked),
  EV_LVN_ENDLABELEDIT(IDC_MAIL_LIST_ORDINARY, OrdLabelEdited),  // OWL Event Handlers
  EV_LVN_ITEMCHANGED(IDC_MAIL_LIST_REGISTERED, RegLstItemChanged), //OWL Event Handlers
  EV_MESSAGE(WM_BN_RIGHTCLICKED, ButtonRightClicked),
  EV_MESSAGE(WM_LWN_VSCROLLED, LWndVScrolled),
  EV_EN_CHANGE(IDC_MAIL_LIST_SEARCHTEXT, SearchTextChanged),
  EV_WM_CLOSE,  // 09.mar.2004
  EV_BN_CLICKED(IDCANCEL, CancelBnClicked),  // 09.mar.2004
//  EV_WM_KEYDOWN,
END_RESPONSE_TABLE;

TMailListDialog::TMailListDialog(TWindow* const parent,
  const MailListDlgData& dlgDataRef):
  TResizeDialog(parent, SHOW_MAXIMIZED, IDD_MAIL_LIST_DIALOG),
  rDlgData(dlgDataRef)
{
  ZERO_LEN(txtToSearch);
  lastFoundIndex = -1;
  ml_descr_static = new TStatic(this, IDC_MAIL_LIST_DESCR_STATIC, 0);
  new TStatic(this, IDC_MAIL_LIST_ORDINARY_LABEL, 0);
  ml_ordinary = new TOrdMailListWindow(this, IDC_MAIL_LIST_ORDINARY, rDlgData);
  ml_registered_label = new TStatic(this, IDC_MAIL_LIST_REGISTERED_LABEL, 0);
  autoSize = ml_registered = new TRegMailListWindow(this, IDC_MAIL_LIST_REGISTERED, rDlgData);
  ml_add_registered = new TButton(this, IDADD);
  ml_delete_registered = new TButton(this, IDDELETE);
  ml_print = new TButton(this, IDPRINT);
  idok = new TButton(this, IDOK);
  idcancel = new TButton(this, IDCANCEL);
  ml_edit_registered = new TButton(this, IDEDIT);
  ml_envelope = new TButton(this, IDC_MAIL_LIST_ENVELOPE);
  new TButton(this, IDC_MAIL_LIST_SEARCH);
  ml_date_sent = new TDateFace(this, IDC_MAIL_LIST_DATE_SENT, "DS", true);
  ml_searchtext = new TStringFace(this, IDC_MAIL_LIST_SEARCHTEXT, "TXTSRCH", sizeof(txtToSearch));
  ml_searchmark = new TArrowStatic(this, IDC_MAIL_LIST_SEARCHMARK, 0);
}  // TMailListDialog::TMailListDialog

void TMailListDialog::Resize()
{
	TResizeDialog::Resize();

	TResizeDialog::Resize(idcancel, RESIZE_RIGHT);
	TResizeDialog::Resize(ml_descr_static, RESIZE_WIDTH);
	TResizeDialog::Resize(ml_add_registered, RESIZE_RIGHT);
	TResizeDialog::Resize(ml_edit_registered, RESIZE_RIGHT);
	TResizeDialog::Resize(ml_delete_registered, RESIZE_RIGHT);
}

void TMailListDialog::SetupWindow()
{
  TResizeDialog::SetupWindow();
  SetDescription();
  RefreshRegMailLabel();
  EnableDisableRegButtons();
  ml_date_sent->SetDate(Today);
  FollowLastFoundItem();
}  // TMailListDialog::SetupWindow

void TMailListDialog::SetDescription()
{
  char tmp[512];
  size_t l = 0;  // Important!

  // 2009:034 LPR: common colleges support
  strCopy(tmp, "Опис на писмата по ", sizeof(tmp) - l);
  l = strlen(tmp);
  {
    ostrstream oss(tmp + l, sizeof(tmp) - l);
    char types[SIZE_OF_TYPES];

    strcpy(types, rDlgData.mailTypes);
    for (char *s = types; *s; s++)
	if (strchr(COLLEGE_COMMONS, *s))
		memmove(s, s + 1, strlen(s));
    if (*types)
    {
	types_ToPlural(oss, rDlgData.mailTypes) ;
	ENDING_ZERO(tmp);
	strConcat(tmp, " дела, ", sizeof(tmp) - l);
    }
    oss << ends;
    for (const char *s = rDlgData.mailTypes; *s; s++)
	if (strchr(COLLEGE_COMMONS, *s))
	{
	   strConcat(tmp, College->Seek(*s), sizeof(tmp) - l);
	   strConcat(tmp, ", ", sizeof(tmp) - l);
	}
  }
  strConcat(tmp,
    rDlgData.wantCheckup ? "изпратени" : "подготвени за изпращане",
    sizeof(tmp) - l);
  strConcat(tmp, " по пощата ", sizeof(tmp) - l);
  l = strlen(tmp);
  period_to_string(rDlgData.begDate, rDlgData.endDate, tmp + l, sizeof(tmp) - l,
    true, true);
  ml_descr_static->SetWindowText(tmp);
}

void TMailListDialog::RefreshRegMailLabel()
{
  char tmp[128];
  const int regCnt = ml_registered->GetItemCount();


  ostrstream(tmp, sizeof(tmp))
    << "Препоръчани писма (" << regCnt << ")" << ends;
  ENDING_ZERO(tmp);
  ml_registered_label->SetWindowText(tmp);
}

void TMailListDialog::EvSize(uint sizeType, TSize& size)
{
  TResizeDialog::EvSize(sizeType, size);
  ml_registered->Scroll(0, 0);	// clear any unneeded horizontal scrollbar
  FollowLastFoundItem();
}  // TMailListDialog::EvSize

void TMailListDialog::AddBnClicked()
{
  TRegMailListWindow* const pReg = ml_registered;

  if(pReg)
    pReg->InsertLetter();
  RefreshRegMailLabel();
  lastFoundIndex = -1;  // 04.mar.2004
  FollowLastFoundItem();
}  // TMailListDialog::AddBnClicked

void TMailListDialog::EditBnClicked()
{
  TRegMailListWindow* const pReg = ml_registered;

  if(pReg)
    pReg->EditSelected();
  FollowLastFoundItem();
  EnableDisableRegButtons();
}  // TMailListDialog::EditBnClicked

int32 TMailListDialog::ButtonRightClicked(TParam1,
  TParam2 bnHandle)
{
  // Determine if IDC_MAIL_LIST_ADD_REGISTERED had sent the messgae
  if(bnHandle && ml_add_registered->HWindow == reinterpret_cast<HWND>(bnHandle))
  {
    TRegMailListWindow* const pReg = ml_registered;

    if(pReg)
      pReg->InsertLetterEx();
  }
  // Determine if IDC_MAIL_LIST_PRINT had sent the messgae
  if(bnHandle && ml_print->HWindow == reinterpret_cast<HWND>(bnHandle))
  {
    PrintSelection();
  }
  return 0;
}  // TMailListDialog::ButtonRightClicked

int32 TMailListDialog::LWndVScrolled(TParam1, TParam2 lwndHandle)
{
  // Determine wheter IDC_MAIL_LIST_REGISTERED had sent the messgae
  if(lwndHandle && ml_registered->HWindow == reinterpret_cast<HWND>(lwndHandle))
  {
    FollowLastFoundItem();
  }
  return 0;
}  // TMailListDialog::LWndVScrolled

void TMailListDialog::DeleteBnClicked()
{
  TRegMailListWindow* const pReg = ml_registered;

  if(pReg)
    pReg->DeleteSelected();
  RefreshRegMailLabel();
  EnableDisableRegButtons();  // Because of the situation, when 0(zero) items
    // are remaining in the list.
  lastFoundIndex = -1;  // 04.mar.2004
  FollowLastFoundItem();
}  // TMailListDialog::DeleteBnClicked

void TMailListDialog::PrintBnClicked()
{
  PrintSelection();
}  // TMailListDialog::PrintBNClicked

void TMailListDialog::PrintSelection()
{
  TOrdMailListWindow* const pOrd = ml_ordinary;
  TRegMailListWindow* const pReg = ml_registered;

  if(pOrd && pReg && pReg->CountsOk() &&
    //pReg->CorrectEnvelopeSelection()  // 09.feb.2003
    pReg->CorrectEnvelopeCheck()  // 09.feb.2003
    )
  {
    TISArray<TCheckRegisteredMail>& all = pReg->NonConstDataTmp();  // Here is
      // ok, we use it in a single block without giving control to the
	// (*pReg) object
    TISArray<TRegisteredMail> selection;
    bool rT = true;

    try
    {
      for(int i = 0; rT && i < pReg->GetItemCount(); i++)
      {
        const bool getIt =
          is_default_post_admin() ?
          (pReg->IsChecked(i)
            && (reg_mail_really_sent(*(all[i])) == false  // 09.feb.2004
              || rDlgData.wantCheckup == true  // 12.feb.2004
              )
          )
           : pReg->IsChecked(i);

        if(getIt)
        {
          TRegisteredMail* pNewRM = new TRegisteredMail;

          (*pNewRM) << (*(all[i]));
	  selection.Add(pNewRM);
	}
      }
      if(rT)
        if(is_default_post_admin())
	{
	    if(rDlgData.wantCheckup == false)  // 12.feb.2004
          {
		if(pReg->GetNonSentCheckCount() != selection.Count())
              rT = false;
          }
          else  // 12.feb.2004
          {
		if(pReg->GetCheckCount() != selection.Count())
              rT = false;
          }
        }
        else
        {
          if(pReg->GetCheckCount() != selection.Count())
            rT = false;
        }
      if(rT == false)
        error("%s::%s\n%s.",
          typeid(*this).name(), "PrintSelection", UNSUCCESSFUL_OPERATION);
      {
	  MailListDlgData tmp;

        strCopy(tmp.mailTypes, rDlgData.mailTypes, sizeof(tmp.mailTypes));
        tmp.begDate = rDlgData.begDate;
        tmp.endDate = rDlgData.endDate;
        tmp.sent = ml_date_sent->Date();
        tmp.wantCheckup = rDlgData.wantCheckup;
        MailListWriter(this, pOrd->Data(), selection, tmp).Print();
	}
    }
    CATCH_ANYTHING
  }
}  // TMailListDialog::PrintSelection

void TMailListDialog::EnvelopeBnClicked()
{
  TRegMailListWindow* const pReg = ml_registered;

  if(pReg && pReg->CountsOk())
  {
    pReg->EnvelopeJob();
  }
  EnableDisableRegButtons();
}  // TMailListDialog::EnvelopeBnClicked

void TMailListDialog::OkBnClicked()
{
  TRegMailListWindow* const pReg = ml_registered;

  if(pReg && pReg->CountsOk())
  {
    if(pReg->SaveSelection(ml_date_sent->Date()) == true)
    {
      if(is_default_post_admin() == false)
        CmOk();
      else
	  pReg->ReDisplay();
    }
  }
}  // TMailListDialog::OkBnClicked

void TMailListDialog::SearchBnClicked()
{
  TRegMailListWindow* const pReg = ml_registered;
  TStringFace* const pSTxt = ml_searchtext;

  if(pReg && pReg->CountsOk() && pSTxt)
  {
    const int cnt = pReg->GetItemCount();

    if(cnt > 0)
    {
      lastFoundIndex ++;
      if(lastFoundIndex < 0 || cnt <= lastFoundIndex ||
        strcmp(txtToSearch, pSTxt->S()) != 0)
      {
	  lastFoundIndex = 0;
      }
      strCopy(txtToSearch, pSTxt->S(), sizeof(txtToSearch));
      {
        // I could not make TListWindow::FindItem perform search by substring
        const int matching = pReg->FindSubstring(lastFoundIndex, txtToSearch);

        if(0 <= matching && matching < cnt)
	  {
	    pReg->EnsureVisible(matching, false);
          lastFoundIndex = matching;
        }
        else
        {
          // Ii is theoretically possible to find an empty string
          // (practically not, though).
          if(txtToSearch[0])
          {
            info("Търсеният низ \"%s\" не е открит.", txtToSearch);
          }
          else
          {
            info("Въведете критерий за търсене.");
          }
          lastFoundIndex = -1;
        }
      }
    }
    else
    {
      lastFoundIndex = -1;
    }
  }
  else
  {
    lastFoundIndex = -1;
  }
  FollowLastFoundItem();
}  // TMailListDialog::SearchBnClicked

void TMailListDialog::SearchTextChanged()
{
  lastFoundIndex = -1;
  FollowLastFoundItem();
}

void TMailListDialog::FollowLastFoundItem()
{
  TStatic& mark = *ml_searchmark;
  TRect markRect = mark.GetWindowRect();
  TPoint markLeftTop(markRect.left, markRect.top);
  TPoint markRightBottom(markRect.right, markRect.bottom);
  bool hideMark = true;
  TRegMailListWindow* const pReg = ml_registered;

  ScreenToClient(markLeftTop);
  ScreenToClient(markRightBottom);
  if(pReg != NULL && pReg->CountsOk() &&
    0 <= lastFoundIndex && lastFoundIndex < pReg->GetItemCount())
  {
    TRect regLstRect = pReg->GetWindowRect();
    TPoint regLstLeftTop(regLstRect.left, regLstRect.top);
    TPoint itemPos;

    ScreenToClient(regLstLeftTop);
    if(pReg->GetItemPosition(lastFoundIndex, &itemPos) == true)
    {
	if(0 <= itemPos.Y() && itemPos.Y() < regLstRect.Height())
	{
	  int yCorrection = 0;
        TRect itemRect;

        if(pReg->GetItemRect(lastFoundIndex, &itemRect, TListWindow::Bounds))
        {
          yCorrection = itemRect.Height() / 5;
        }
        hideMark = false;
        mark.MoveWindow(markLeftTop.X(),
          regLstLeftTop.Y() + itemPos.Y() + yCorrection,
          markRect.Width(), markRect.Height());
        InvalidateRect(TRect(markLeftTop.X(), markLeftTop.Y(),
          markRightBottom.X(), markRightBottom.Y()));
        mark.ShowWindow(SW_SHOW);
        mark.BringWindowToTop();
	  mark.Invalidate();
      }
    }
  }
  if(hideMark == true)
  {
    mark.MoveWindow(markLeftTop.X(), 0, markRect.Width(), markRect.Height());
    InvalidateRect(TRect(markLeftTop.X(), markLeftTop.Y(),
	markRightBottom.X(), markRightBottom.Y()));
    mark.ShowWindow(SW_HIDE);
  }
}  // TMailListDialog::FollowLastFoundItem

void TMailListDialog::RegLstItemChanged(TLwNotify&)
{
  EnableDisableRegButtons();
}

void TMailListDialog::EnableDisableRegButtons()
{
  TRegMailListWindow* const pReg = ml_registered;

  if(pReg && pReg->CountsOk())
  {
    const int selCnt = pReg->GetSelCount();
    const int checkCnt = pReg->GetCheckCount();
    const int nonSentCheckCnt = pReg->GetNonSentCheckCount();
    TOrdMailListWindow* const pOrd = ml_ordinary;

    if(rDlgData.wantCheckup == false)
    {
      if(is_default_post_admin() == true)
      {
        ml_print->EnableWindow(nonSentCheckCnt != 0 ||
          (pOrd != NULL && pOrd->NonZeroData() == true));
        idok->EnableWindow(nonSentCheckCnt != 0);
	}
      else
      {
        ml_print->EnableWindow(checkCnt != 0 ||
          (pOrd != NULL && pOrd->NonZeroData() == true));
        idok->EnableWindow(checkCnt != 0);
      }
      ml_envelope->EnableWindow(selCnt > 1 ||
        (selCnt == 1 && pReg->CorrectEnvelopeSelection(false)));
      if(is_default_post_admin() == true)
      {
        ml_edit_registered->EnableWindow(selCnt == 1);
        ml_delete_registered->EnableWindow(selCnt != 0 && pReg->SentItemSelected() == false);
      }
      else
      {
        ml_edit_registered->EnableWindow(selCnt == 1);
        ml_delete_registered->EnableWindow(selCnt != 0);
      }
    }
    else
    {
      ml_print->EnableWindow(checkCnt != 0 ||
          (pOrd != NULL && pOrd->NonZeroData() == true));
      idok->EnableWindow(false);
      ml_delete_registered->EnableWindow(false);
      ml_envelope->EnableWindow(false);
	ml_edit_registered->EnableWindow(false);
	ml_add_registered->EnableWindow(false);
    }
  }
}  // TMailListDialog::EnableDisableRegButtons

void TMailListDialog::OrdLabelEdited(TLwDispInfoNotify& nmHdr)
{
  if(nmHdr.item.pszText != NULL)
  {
    TOrdMailListWindow* const pLw = ml_ordinary;

    if(pLw && 0 <= nmHdr.item.iItem && nmHdr.item.iItem < pLw->GetItemCount())
    {
      pLw->SetQuantity(nmHdr.item.iItem, nmHdr.item.pszText);
//      pLw->SetItemText(nmHdr.item.iItem, 0, nmHdr.item.pszText);
      EnableDisableRegButtons();
    }
  }
}  // TMailListDialog::PrintBNClicked

bool TMailListDialog::MayQuit()
{  // 09.mar.2004
  bool mayQuit = true;
  TRegMailListWindow* const pReg = ml_registered;

  if(pReg != NULL && pReg->GetCheckCount() != 0)
  {
    if(ask(
      "Има маркирани елементи.\nНаистина ли желаете да излезете от режима?") ==
      false)
    {
      mayQuit = false;
    }
  }
  return mayQuit;
}  // TMailListDialog::MayQuit

void TMailListDialog::CancelBnClicked()
{  // 09.mar.2004
  if(MayQuit())
    CmCancel();
}

void TMailListDialog::EvClose()
{  // 09.mar.2004
  if(MayQuit())
    TResizeDialog::EvClose();
}

// ----------------------- TMailListDialog - end -----------------------

// -------------------- MailListCriteriaDialog - begin -----------------

class MailListCriteriaDialog : public TScaleDialog
{  // 08.feb.2003; 2009:034 TDlgItemPtrDialog -> TScaleDialog
  public:  // Construction / Destruction
    MailListCriteriaDialog(TWindow* const parent,
	begNEndNYearTypesAC& inpDiaryDlgDataRef);
  public:  // Methods
    virtual void SetupWindow();
  protected:  // Methods
    void OKBNClicked();
    bool UserInputOk();
    void HandleRegimeGrpBoxMsg(uint);
    void AdjustDatesGrpBoxText();
  protected:  // Data
    begNEndNYearTypesAC& rInpDiaryDlgData;
    TTypeListFace* pTypes;
    TDateFace* pBegD;
    TDateFace* pEndD;
    TRadioButton *ml_prepare;
    TRadioButton *ml_checkup;
    TWindow* pDatesGrp;
  DECLARE_RESPONSE_TABLE(MailListCriteriaDialog);
};  // class MailListCriteriaDialog


DEFINE_RESPONSE_TABLE1(MailListCriteriaDialog, TScaleDialog)
  EV_BN_CLICKED(IDOK, OKBNClicked),
  EV_CHILD_NOTIFY_ALL_CODES(IDC_MLC_ACTION_GRP, HandleRegimeGrpBoxMsg),
END_RESPONSE_TABLE;

MailListCriteriaDialog::MailListCriteriaDialog(TWindow* const parent,
  begNEndNYearTypesAC& inpDiaryDlgDataRef):
  TScaleDialog(parent, IDD_MAIL_LIST_CRIT),
  rInpDiaryDlgData(inpDiaryDlgDataRef)
{
  // 2009:034 LPR: NULL -> xColleges
  pTypes =
    new TTypeListFace(this, IDC_MLC_LAW_TYPES, "LT", Default->xColleges, false);
  pDatesGrp = new TGroupBox(this, IDC_MLC_DATE_GRP);
  pBegD = new TDateFace(this, IDC_MLC_BEG_DATE, "FD", true);
  new TDateFaceUpDown(this, IDC_MLC_BEG_DATE_UPDN, pBegD);
  pEndD = new TDateFace(this, IDC_MLC_END_DATE, "TD", true);
  new TDateFaceUpDown(this, IDC_MLC_END_DATE_UPDN, pEndD);
  {
    TGroupBox* pRegimeGrpBox = new TGroupBox(this, IDC_MLC_ACTION_GRP);
    ml_prepare = new TRadioButton(this, IDC_MLC_PREPARE, pRegimeGrpBox);
    ml_checkup = new TRadioButton(this, IDC_MLC_CHECKUP, pRegimeGrpBox);
  }
}  // MailListCriteriaDialog::MailListCriteriaDialog

void MailListCriteriaDialog::SetupWindow()
{
  TScaleDialog::SetupWindow();

  pBegD->SetDate(rInpDiaryDlgData.begDate);
  pEndD->SetDate(rInpDiaryDlgData.endDate);
  pTypes->SetHorizExtent();
  ml_prepare->SetCheck(BF_CHECKED);
  ml_checkup->SetCheck(BF_UNCHECKED);
  AdjustDatesGrpBoxText();
}  // MailListCriteriaDialog::SetupWindow

void MailListCriteriaDialog::OKBNClicked()
{
  if(UserInputOk())
    CmOk();
}

bool MailListCriteriaDialog::UserInputOk()
{
  bool rT = true;

  strCopy(rInpDiaryDlgData.types, pTypes->S(), sizeof(rInpDiaryDlgData.types));
  rInpDiaryDlgData.begDate = pBegD->Date();
  rInpDiaryDlgData.endDate = pEndD->Date();
  rInpDiaryDlgData.wantCheckup = (ml_checkup->GetCheck() == BF_CHECKED);
  if(rT)
    if(rInpDiaryDlgData.begDate > rInpDiaryDlgData.endDate)
      rT = error(INCORRECT_PERIOD_GIVEN);
  if(rT)
    if(rInpDiaryDlgData.types[0] == '\0')
      rT = error(NO_TYPE_CHOOSEN);
  return rT;
}  // MailListCriteriaDialog::UserInputOk

void MailListCriteriaDialog::HandleRegimeGrpBoxMsg(uint)
{
  AdjustDatesGrpBoxText();
}

void MailListCriteriaDialog::AdjustDatesGrpBoxText()
{
  if(ml_prepare->GetCheck() == BF_CHECKED)
    pDatesGrp->SetWindowText("Подготвени");
  else
    pDatesGrp->SetWindowText("Изпратени");
}

// -------------------- MailListCriteriaDialog - end -----------------


void print_mail_list(TWindow* const pParent)
{
  begNEndNYearTypesAC commonDlgData;

  commonDlgData.begDate = commonDlgData.endDate = Today;
  if(MailListCriteriaDialog(pParent, commonDlgData).Execute() == IDOK)
  {
    MailListDlgData dlgData;

    dlgData.CopyFrom(commonDlgData);
    register_usage(REGIME_MAIL_LIST, 1, dlgData.mailTypes,
	"Поща");  // 01.dec.2004
    TMailListDialog(pParent, dlgData).Execute();
  }
}  // print_mail_list

//
