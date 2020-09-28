struct HeapingData
{
	HeapingData() { Clear(); }
	void Clear();
	virtual void Copy(const HeapingData& other) = 0;
	virtual bool LessThan(const HeapingData& other) const = 0;
	virtual bool Equal(const HeapingData& other) const = 0;
	virtual HeapingData& Accumulate(const HeapingData& other) = 0;
	HeapingData& operator = (const HeapingData& other);
	bool operator<=(const HeapingData &that) const { return LessThan(that) || Equal(that); }
};  // struct HeapingData

class HeapingDataArray : public TISArray<HeapingData>
{
protected:
	virtual HeapingData* NewItem() = 0;
public:
	HeapingDataArray() { }
	bool StoreUp(const HeapingData& piece);
};  // class HeapingDataArray

enum WhatPartOfDay;

struct SessJudgeDistrData : public HeapingData
{
	CDate date;
  int instance;
  WhatPartOfDay partOfDay;
  char judgeUcn[SIZE_OF_UCN];
  char judgePost;
  char judgeInitials[SIZE_OF_CITIZEN_INITIALS];
  int numberOfSessions;
  char secretaryUcn[SIZE_OF_UCN];

  SessJudgeDistrData();
  void Clear();
  virtual void Copy(const HeapingData& other);
	virtual bool LessThan(const HeapingData& other) const;
	virtual bool Equal(const HeapingData& other) const;
	virtual HeapingData& Accumulate(const HeapingData& other);
};  // struct SessJudgeDistrData

class SessJudgeDistrArray : public HeapingDataArray
{
public:
	SessJudgeDistrArray() { }
	SessJudgeDistrData* operator [] (int index);
protected:
	virtual HeapingData* NewItem();
};  // class SessJudgeDistrArray
