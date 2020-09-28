#define LP_SIZE sizeof(void *)

class TIntegerArray : protected memblock_t
{
public:
	TIntegerArray(bool sorted = false);
	~TIntegerArray() { Clear(); }

	long operator[](int index) const { return ItemAt(index); }
	int Add(const long value);
	void Add(const long value1, const long value2, ...);
	void Clear() { mbk_clear(this); }
	bool Contains(const long value) const { return Find(value) != INT_MAX; }
	int Count() const { return size / LP_SIZE; }
	long Detach(int index);
	int Find(const long value) const;
	void Insert(const long value, int index);
	bool Sorted() { return lesseql; }

protected:
	bool (*lesseql)(const long value1, const long value2);

#if RANGECHECK
	long ItemAt(int index) const;
#else  // RANGECHECK
	long ItemAt(int index) const { return ((long *) data)[index]; }
#endif  // RANGECHECK
};

class TPointerArray : protected TIntegerArray
{
public:
	~TPointerArray();

	TIntegerArray::Clear;
	TIntegerArray::Count;
	void Flush();
	void Remove(int index);
	TIntegerArray::Sorted;
	
protected:
	TPointerArray(void (*tDelete)(void *tData));

	void (*destroy)(void *tData);

	int Add(const void *tData) { return TIntegerArray::Add((long) tData); }
	void Add(const void *data1, const void *data2, va_list ap);
	bool Contains(const void *tData) const { return TIntegerArray::Contains((long) tData); }
	void *Detach(int index) { return (void *) TIntegerArray::Detach(index); }
	int Find(const void *tData) const { return TIntegerArray::Find((long) tData); }
	void *ItemAt(int index) const { return (void *) TIntegerArray::ItemAt(index); }
	void Insert(const void *tData, int index) { TIntegerArray::Insert((long) tData, index); }
};

// ----- TIArray -------------------------------------------------------------
template <class T> class TIArray : public TPointerArray
{
public:
	TIArray() : TPointerArray(DeleteT) { }

	T *operator[](int index) const { return (T *) ItemAt(index); }
	int Add(const T *t) { return TPointerArray::Add(t); }
	void Add(const T *t1, const T *t2, ...);
	void Insert(const T *t, int index) { TPointerArray::Insert(t, index); }
	T *Detach(int index) { return (T *) TPointerArray::Detach(index); }

protected:
	T *ItemAt(int index) const { return (T *) TPointerArray::ItemAt(index); }
	static void DeleteT(void *t) { delete (T *) t; }
};

template <class T> void TIArray<T>::Add(const T *t1, const T *t2, ...)
{
	va_list ap;

	va_start(ap, t2);
	TPointerArray::Add(t1, t2, ap);
	va_end(ap);
}

template <class T> class TISArray : public TPointerArray
{
public:
	TISArray() : TPointerArray(DeleteT) { lesseql = LessEqlT; }

	T *operator[](int index) const { return (T *) ItemAt(index); }
	void Add(const T *t) { TPointerArray::Add(t); }
	bool Contains(const T *t) const { return TPointerArray::Contains(t); }
	T *Detach(int index) { return (T *) TPointerArray::Detach(index); }
	int Find(const T *t) const { return TPointerArray::Find(t); }

protected:
	static void DeleteT(void *t) { delete (T *) t; }
	static bool LessEqlT(const long t1, const long t2) { return *(T *) t1 <= *(T *) t2; }
};

// ----- TStringArray --------------------------------------------------------
#define TStringArray TNTStringArray
class TStringArray : public TPointerArray
{
public:
	TStringArray(bool sorted = false);	// true = case-insensitive sort

	char *operator[](int index) const { return (char *) ItemAt(index); }
	void Add(const char *s) { TPointerArray::Add(s); }
	void Insert(const char *s, int index) { TPointerArray::Insert(s, index); }
	int Find(const char *s) { return TPointerArray::Find(s); }

protected:
	static void DeleteString(void *s);
};
