struct CNode
{
#if RANGECHECK
	CNode() { Reset(this); }
#endif  // RANGECHECK
private:
	void Reset(CNode *a);

	bool red;
	CNode *left;
	CNode *right;
	CNode *parent;
	friend class CTree;
};

class CTree
{
public:
	CTree(bool (*tLessEql)(const CNode *a, const CNode *b) = NULL);
	~CTree();

	void Add(CNode *x);
	int Count() const;
	CNode *Detach(CNode *z, int n = 0);		// n = Next/Prev
	bool Empty();
	CNode *Find(const CNode *q) const;		// by LessEql
	CNode *FirstThat(bool (*matches)(CNode *x, void *data), void *data) const;
	void Flush();
	void ForEach(void (*action)(CNode *x, void *data), void *data) const;
	CNode *GetFirst() const;
	CNode *GetLast() const;
	CNode *GetNext(const CNode *x) const;	// must exist
	CNode *GetPrev(const CNode *x) const;	// must exist
	int IndexOf(const CNode *x) const;		// must exist
	CNode *ItemAt(int index) const;		// must exist
	CNode *Remove(CNode *z, int n = 0);		// n = Next/Prev

protected:
	void (*destroy)(CNode *a);
	bool (*lesseql)(const CNode *a, const CNode *b);
	CNode nil;
	CNode root;

	static void Destroy(CNode *a) { delete a; }

	void DeleteFixUp(CNode *x);
	void DestroyHelper(CNode *x);
	void ForEachHelper(CNode *x, void (*action)(CNode *x, void *data), void *data) const;
	void InsertHelper(CNode *z);
	void LeftRotate(CNode *x);
	void Reset(CNode *a);
	void RightRotate(CNode *x);
	CNode *Predecessor(const CNode *x) const;
	CNode *Successor(const CNode *x) const;
};

typedef CNode TNode;

template <class T> class TTree : protected CTree
{
public:
	TTree(bool (*lesseql)(const TNode *a, const TNode *b) = NULL) : CTree(lesseql) { destroy = Destroy; }

	void Add(T *x) { CTree::Add(x); }
	int Count() const { return CTree::Count(); }
	T *Detach(T *z, int n = 0) { return (T *) CTree::Detach(z, n); }
	bool Empty() { return CTree::Empty(); }
	T *Find(const T *q) const { return (T *) CTree::Find(q); }
	T *FirstThat(bool (*matches)(TNode *x, void *data), void *data) const { return (T *) CTree::FirstThat(matches, data); }
	void Flush() { CTree::Flush(); }
	void ForEach(void (*action)(TNode *x, void *data), void *data) const { CTree::ForEach(action, data); }
	T *GetFirst() const { return (T *) CTree::GetFirst(); }
	T *GetLast() const { return (T *) CTree::GetLast(); }
	T *GetNext(const T *x) const { return (T *) CTree::GetNext(x); }
	T *GetPrev(const T *x) const { return (T *) CTree::GetPrev(x); }
	int IndexOf(const T *x) const { return CTree::IndexOf(x); }
	T *ItemAt(int index) const { return (T *) CTree::ItemAt(index); }
	T *Remove(T *z, int n = 0) { return (T *) CTree::Remove(z, n); }

protected:
	static void Destroy(TNode *a) { delete (T *) a; }
};

#define foreach_t(T, t, tree) for (T *t = tree.GetFirst(); t; t = tree.GetNext(t))
#define foreach_pt(T, t, tree) for (T *t = tree->GetFirst(); t; t = tree->GetNext(t))

template <class T> class TSTree : public TTree<T>
{
public:
	TSTree() : TTree<T>(LessEql) { }

protected:
	static bool LessEql(const TNode *a, const TNode *b) { return *(const T *) a <= *(const T *) b; }
};
