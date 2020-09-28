bool operator == (const TSideWind & s0, const TSideWind & s1) ;  // 02.jul.2001

class TRegisteredMail: public TUCNGroup  // TGroup  15.jan.2004
{
public:  // Construction / Destruction
  TRegisteredMail();
public:  // Methods
  virtual bool KeyOk() const;

	virtual void ExactCriteria(msql &m, const char *fields = NULL);
	virtual void FloatCriteria(msql &m, const char *fields = NULL);
	void FloatCriteria(char *s, const char *fields = NULL) { TGroup::FloatCriteria(s, fields); }

  virtual bool Insert();
  virtual void Print(mstr &m);  // emulation
  void Print(char* const dest, const size_t destSz) const;
  void PrintDoc(char* const dest, const size_t destSz) const;
  void PrintUclp(char* const dest, const size_t destSz) const;
  void PrintReceiver(char* const dest, const size_t destSz) const;
  void PrintFlags(char* const dest, const size_t destSz) const;

  bool operator==(const TRegisteredMail &other) const;
  bool operator<(const TRegisteredMail &other) const;
  bool operator<=(const TRegisteredMail &that) const { return operator<(that) || operator==(that); }

public:
  virtual void EnvelopeWhereClause(ostream& os,
    const TRegisteredMail& pattern) const;
  virtual long NextEnvelopeNo(const TRegisteredMail& pattern) const;  // re-
    // turns 0 on error.
  virtual bool SameEnvelope(const TRegisteredMail& other) const;
  virtual bool MayJoinEnvelope(const TRegisteredMail& envlpMember,
    const bool errMsg = true) const;  // Determines if '*this'
    // object may join an envelope with the traits given.
  virtual bool CanEnvelope(const bool errMsg = true) const;

public:  // Data
  // Key fields
  TRCDKey doc;
  CDate date;
  // char ucn[SIZE_OF_UCN];  // From TUCNGroup
  // char ucnType;           // From TUCNGroup
  long letterNo;
  // Address fields (operator<< will work with TAddress)
  long uclp;
  long street;
  char adrNo[SIZE_OF_ADR_NO];
  char subNo[SIZE_OF_SUB_NO];
  char entrance[SIZE_OF_ENTRANCE];
  char floor[SIZE_OF_FLOOR];
  char apartment[SIZE_OF_APARTMENT];
  char blockNo[SIZE_OF_BLOCK_NO];
  // Other fields
  char lawsuitType;  // Must be filled!
  char text[SIZE_OF_REG_MAIL_TEXT];  // For additional
  // information
  long flags;
  long envelopeNo;  // 08.sep.2003
  // Status fields
  char status;  // Must be filled!  // 08.sep.2003
  CDate send;  // 08.sep.2003
  char barcode[SIZE_OF_BARCODE_TEXT];  // 2019:094

protected:  // Methods
  virtual bool GenerateLetterNo();
};  // class TRegisteredMail
