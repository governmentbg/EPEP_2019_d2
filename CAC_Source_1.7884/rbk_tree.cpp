#include "cac.h"

// CNode and CTree are based on Red-Black tree implementation by Emin
// Martinian, http://www.mit.edu/~emin/source_code/index.html, under
// the folowing license:

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that neither the name of Emin
// Martinian nor the names of any contributors are be used to endorse or
// promote products derived from this software without specific prior
// written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// ----- CNode ---------------------------------------------------------------
void CNode::Reset(CNode *a)
{
	parent = left = right = a;
}

#define root (&root)
#define nil (&nil)

CTree::CTree(bool (*tLessEql)(const CNode *a, const CNode *b))
	: destroy(Destroy), lesseql(tLessEql)
{
	Reset(nil);
	Reset(root);
}

CTree::~CTree()
{
	DestroyHelper(root->left);
}

void CTree::Add(CNode *x)
{
	CNode *y;

#if RANGECHECK
	if (x->left != x || x->right != x || x->parent != x)
		mbk_fatal("CTree::Add: re-add", EINVAL);
#endif  // RANGECHECK
	InsertHelper(x);
	x->red = true;

	while (x->parent->red)
	{
		if (x->parent == x->parent->parent->left)
		{
			y = x->parent->parent->right;

			if (y->red)
			{
				x->parent->red = false;
				y->red = false;
				x->parent->parent->red = true;
				x = x->parent->parent;
			}
			else
			{
				if (x == x->parent->right)
				{
					x = x->parent;
					LeftRotate(x);
				}

				x->parent->red = false;
				x->parent->parent->red = true;
				RightRotate(x->parent->parent);
			}
		}
		else
		{
			y = x->parent->parent->left;

			if (y->red)
			{
				x->parent->red = false;
				y->red = false;
				x->parent->parent->red = true;
				x = x->parent->parent;
			}
			else
			{
				if (x == x->parent->left)
				{
					x = x->parent;
					RightRotate(x);
				}

				x->parent->red = false;
				x->parent->parent->red = true;
				LeftRotate(x->parent->parent);
			}
		}
	}

	root->left->red = false;
}

int CTree::Count() const
{
	int count = 0;

	for (const CNode *t = GetFirst(); t; t = GetNext(t))
		count++;

	return count;
}

CNode *CTree::Detach(CNode *z, int n)
{
	CNode *g = n ? n > 0 ? GetNext(z) : GetPrev(z) : NULL;
	CNode *y;
	CNode *x;

	y = ((z->left == nil) || (z->right == nil)) ? z : Successor(z);
	x = (y->left == nil) ? y->right : y->left;

	if (root == (x->parent = y->parent))
		root->left = x;
	else
	{
		if (y == y->parent->left)
			y->parent->left = x;
		else
			y->parent->right = x;
	}

	if (y != z)
	{
		if (!(y->red))
			DeleteFixUp(x);

		y->left = z->left;
		y->right = z->right;
		y->parent = z->parent;
		y->red = z->red;
		z->left->parent = z->right->parent = y;

		if (z == z->parent->left)
			z->parent->left = y;
		else
			z->parent->right = y;
	}
	else
	{

		if (!(y->red))
			DeleteFixUp(x);
	}

#if RANGECHECK
	z->Reset(z);
#endif  // RANGECHECK
	return g;
}

bool CTree::Empty()
{
	return root->left == nil;
}

CNode *CTree::Find(const CNode *q) const
{
	CNode *x = root->left;

#if RANGECHECK
	if (!lesseql)
		mbk_fatal("CTree::Find: NULL lesseql", EINVAL);
#endif  // RANGECHECK

	while (x != nil)
	{
		if (lesseql(x, q))
		{
			if (lesseql(q, x))
				return x;

			x = x->right;
		}
		else
			x = x->left;
	}

	return NULL;
}

void CTree::Flush()
{
	DestroyHelper(root->left);
	Reset(nil);	
	Reset(root);
}

void CTree::ForEach(void (*action)(CNode *x, void *data), void *data) const
{
	ForEachHelper(root->left, action, data);
}

CNode *CTree::GetFirst() const
{
	CNode *x = root->left;

	if (x == nil)
		return NULL;

	while (x->left != nil)
		x = x->left;

	return x;
}

CNode *CTree::GetLast() const
{
	CNode *x = root->left;

	if (x == nil)
		return NULL;

	while (x->right != nil)
		x = x->right;

	return x;
}

CNode *CTree::GetNext(const CNode *x) const
{
	x = Successor(x);
	return x != nil ? (CNode *) x : NULL;
}

CNode *CTree::GetPrev(const CNode *x) const
{
	x = Predecessor(x);
	return x != nil ? (CNode *) x : NULL;
}

int CTree::IndexOf(const CNode *x) const
{
	int index = 0;

	for (const CNode *t = GetFirst(); t != x; t = GetNext(t))
		index++;

	return index;
}

CNode *CTree::ItemAt(int index) const
{
	CNode *x = GetFirst();

	while (index--)
		x = GetNext(x);

	return x;
}

CNode *CTree::Remove(CNode *z, int n)
{
	CNode *g = Detach(z, n);
	destroy(z);
	return g;
}

void CTree::DeleteFixUp(CNode *x)
{
	CNode *left = root->left;
	CNode *w;

	while ((!x->red) && (x != left))
	{
		if (x == x->parent->left)
		{
			w = x->parent->right;

			if (w->red)
			{
				w->red = false;
				x->parent->red = true;
				LeftRotate(x->parent);
				w = x->parent->right;
			}

			if ((!w->right->red) && (!w->left->red))
			{
				w->red = true;
				x = x->parent;
			}
			else
			{
				if (!w->right->red)
				{
					w->left->red = false;
					w->red = true;
					RightRotate(w);
					w = x->parent->right;
				}

				w->red = x->parent->red;
				x->parent->red = false;
				w->right->red = false;
				LeftRotate(x->parent);
				break;
			}
		}
		else
		{
			w = x->parent->left;

			if (w->red)
			{
				w->red = false;
				x->parent->red = true;
				RightRotate(x->parent);
				w = x->parent->left;
			}

			if ((!w->right->red) && (!w->left->red))
			{
				w->red = true;
				x = x->parent;
			}
			else
			{
				if (!w->left->red)
				{
					w->right->red = false;
					w->red = true;
					LeftRotate(w);
					w = x->parent->left;
				}

				w->red = x->parent->red;
				x->parent->red = false;
				w->left->red = false;
				RightRotate(x->parent);
				break;
			}
		}
	}

	x->red = false;
}

void CTree::DestroyHelper(CNode *x)
{
	if (x != nil)
	{
		DestroyHelper(x->left);
		DestroyHelper(x->right);
		destroy(x);
	}
}

void CTree::ForEachHelper(CNode *x, void (*action)(CNode *x, void *data), void *data) const
{
	if (x != nil)
	{
		ForEachHelper(x->left, action, data);
		action(x, data);
		ForEachHelper(x->right, action, data);
	}
}

void CTree::InsertHelper(CNode *z)
{
	CNode *x;
	CNode *y;

	z->left = z->right = nil;
	y = root;
	x = root->left;

	while (x != nil)
	{
		y = x;

		if (lesseql && !lesseql(x, z))
			x = x->left;
		else
			x = x->right;
	}

	z->parent = y;

	if (y == root || (lesseql && !lesseql(y, z)))
		y->left = z;
	else
		y->right = z;
}

void CTree::LeftRotate(CNode *x)
{
	CNode *y;

	y = x->right;
	x->right = y->left;

	if (y->left != nil)
		y->left->parent = x;

	y->parent = x->parent;

	if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;

	y->left = x;
	x->parent = y;
}

void CTree::Reset(CNode *a)
{
	a->Reset(nil);
	a->red = false;
}

void CTree::RightRotate(CNode *y)
{
	CNode *x;

	x = y->left;
	y->left = x->right;

	if (x->right != nil)
		x->right->parent = y;

	x->parent = y->parent;

	if (y == y->parent->left)
		y->parent->left = x;
	else
		y->parent->right = x;

	x->right = y;
	y->parent = x;
}

CNode *CTree::Predecessor(const CNode *x) const
{
	CNode *y;

	if ((y = x->left) != nil)
	{
		while (y->right != nil)
			y = y->right;

		return y;
	}
	else
	{
		y = x->parent;

		while (x == y->left)
		{
			if (y == root)
				return (CNode *) nil;

			x = y;
			y = y->parent;
		}

		return y;
	}
}

CNode *CTree::Successor(const CNode *x) const
{
	CNode *y;

	if ((y = x->right) != nil)
	{
		while (y->left != nil)
			y = y->left;

		return y;
	}
	else
	{
		y = x->parent;

		while (x == y->right)
		{
			x = y;
			y = y->parent;
		}

		if (y == root)
			return (CNode *) nil;

		return y;
	}
}
