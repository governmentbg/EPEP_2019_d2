#define LP_SIZE sizeof(void *)

class TIntegerArray : protected memblock_t
{
public:
	TIntegerArray(bool sorted = false);
	~TIntegerArray() { Clear(); }

	intptr_t operator[](int index) const { return ItemAt(index); }
	int Add(const intptr_t value);
	void Add(const intptr_t value1, const intptr_t value2, ...);
	void Clear() { mbk_clear(this); }
	bool Contains(const intptr_t value) const { return Find(value) != INT_MAX; }
	int Count() const { return size / LP_SIZE; }
	intptr_t Detach(int index);
	int Find(const intptr_t value) const;
	void Insert(const intptr_t value, int index);
	bool Sorted() { return lesseql; }

protected:
	bool (*lesseql)(const intptr_t value1, const intptr_t value2);

#if RANGECHECK
	intptr_t ItemAt(int index) const;
#else  // RANGECHECK
	intptr_t ItemAt(int index) const { return ((intptr_t *) data)[index]; }
#endif  // RANGECHECK

private:
	TIntegerArray(const TIntegerArray &);
};

class TPointerArray : protected TIntegerArray
{
public:
	~TPointerArray();

	void Clear() { TIntegerArray::Clear(); }
	int Count() const { return TIntegerArray::Count(); }
	void Flush();
	void Remove(int index);
	bool Sorted() { return TIntegerArray::Sorted(); }
	
protected:
	TPointerArray(void (*tDelete)(void *tData));

	void (*destroy)(void *tData);

	int Add(const void *tData) { return TIntegerArray::Add((intptr_t) tData); }
	void Add(const void *data1, const void *data2, va_list ap);
	bool Contains(const void *tData) const { return TIntegerArray::Contains((intptr_t) tData); }
	void *Detach(int index) { return (void *) TIntegerArray::Detach(index); }
	int Find(const void *tData) const { return TIntegerArray::Find((intptr_t) tData); }
	void *ItemAt(int index) const { return (void *) TIntegerArray::ItemAt(index); }
	void Insert(const void *tData, int index) { TIntegerArray::Insert((intptr_t) tData, index); }
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
	static bool LessEqlT(const intptr_t t1, const intptr_t t2) { return *(T *) t1 <= *(T *) t2; }
};
