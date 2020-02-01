#include <bits/stdc++.h>

template <class T>
inline void read(T &x)
{
	static char ch; 
	while (!isdigit(ch = getchar())); 
	x = ch - '0'; 
	while (isdigit(ch = getchar()))
		x = x * 10 + ch - '0'; 
}

template <class T>
inline void putint(T x)
{
	static char buf[15], *tail = buf; 
	if (!x)
		putchar('0'); 
	else
	{
		for (; x; x /= 10) *++tail = x % 10 + '0'; 
		for (; tail != buf; --tail) putchar(*tail); 
	}
}

const int mod = 1e9 + 7; 
const int MaxN = 2e5 + 5; 
const int INF = 0x3f3f3f3f; 

struct mat
{
	int a, b, c, d; 
	mat(){}
	mat(int _a, int _b, int _c, int _d):
		a(_a), b(_b), c(_c), d(_d) {}
	inline mat operator * (const mat &rhs) const
	{
		int resa = (1LL * a * rhs.a + 1LL * b * rhs.c) % mod; 
		int resb = (1LL * a * rhs.b + 1LL * b * rhs.d) % mod; 
		int resc = (1LL * c * rhs.a + 1LL * d * rhs.c) % mod; 
		int resd = (1LL * c * rhs.b + 1LL * d * rhs.d) % mod; 
		return mat(resa, resb, resc, resd); 
	}
	inline int get_ans() const
	{
		int res = a + b; 
		return res >= mod ? res - mod : res;
	}
}I(1, 0, 1, 0); 

namespace Splay
{
	int bin[MaxN], top; 
	mat val[MaxN], prod[MaxN]; 

	int nT, rt, dval[MaxN], dsum[MaxN]; 
	int lc[MaxN], rc[MaxN], fa[MaxN]; 

	inline int new_node()
	{
		if (top)
			return bin[top--]; 
		else
			return ++nT; 
	}

	inline void upt(int x)
	{
		dsum[x] = dval[x] + dsum[lc[x]] + dsum[rc[x]]; 
		prod[x] = lc[x] ? prod[lc[x]] * val[x] : val[x]; 
		if (rc[x])
			prod[x] = prod[x] * prod[rc[x]]; 
	}
	inline bool which(int x)
	{
		return rc[fa[x]] == x; 
	}
	inline void Rotate(int x)
	{
		int y = fa[x], z = fa[y]; 
		int b = lc[y] == x ? rc[x] : lc[x]; 
		if (b) fa[b] = y; 
		fa[x] = z, fa[y] = x; 

		if (z)
			(lc[z] == y ? lc[z] : rc[z]) = x; 
		if (lc[y] == x)
			rc[x] = y, lc[y] = b; 
		else
			lc[x] = y, rc[y] = b; 

		upt(y); 
	}
	inline void Splay(int x, int tar = 0)
	{
		while (fa[x] != tar)
		{
			if (fa[fa[x]] != tar)
				Rotate(which(x) == which(fa[x]) ? fa[x] : x); 
			Rotate(x); 
		}
		if (!tar) rt = x; 
		upt(x); 
	}
	inline void insert(int pos, int d, const mat &T, int pre = 0, int &x = rt)
	{
		if (!x)
		{
			x = new_node(); 
			dval[x] = dsum[x] = d; 
			lc[x] = rc[x] = 0, fa[x] = pre; 
			val[x] = prod[x] = T, Splay(x); 
			return; 
		}
		int lsze = dval[x] + dsum[lc[x]]; 
		if (pos - d >= lsze)
			insert(pos - lsze, d, T, x, rc[x]); 
		else
			insert(pos, d, T, x, lc[x]); 
	}
	inline int find(int k)
	{
		int x = rt; 
		while (x)
		{
			int lsze = dsum[lc[x]] + dval[x]; 
			if (k <= dsum[lc[x]])
				x = lc[x]; 
			else if (k <= lsze)
				return Splay(x), x; 
			else
				k -= lsze, x = rc[x]; 
		}
		return 0; 
	}
	inline int find_min(int x)
	{
		while (lc[x]) x = lc[x]; 
		return x; 
	}
	inline int find_max(int x)
	{
		while (rc[x]) x = rc[x]; 
		return x; 
	}
	inline void erase(int pos)
	{
		if (pos == INF) return; 

		int x = find(pos); 
		bin[++top] = x; 

		int pre = find_max(lc[x]); Splay(pre); 
		int suf = find_min(rc[x]); Splay(suf, pre); 
		fa[x] = 0, lc[suf] = 0; 
		upt(suf), upt(pre); 
	}
	inline int query()
	{
		return prod[rt].get_ans(); 
	}
	inline void init()
	{
		insert(0, 0, I); 
		insert(+INF, +INF, I); 
	}
}

struct seg
{
	int l, r; 
	seg(){}
	seg(int _l, int _r):
		l(_l), r(_r) {}
	inline bool operator < (const seg &rhs) const
	{
		return l < rhs.l || (l == rhs.l && r < rhs.r); 
	}
}; 

int n; 
std::set<seg> S; 

inline mat calc_T(int d)
{
	return mat(1, (d - 1) >> 1, 1, d >> 1); 
}

inline void insert(int x, int y)
{
	if (y == INF) return; 
	if (x == -INF) x = 0; 
	Splay::insert(y, y - x, calc_T(y - x)); 
}

inline void addFib(int x)
{
	if (x < 0) return;
	if (x == 0) x = 1; 

	auto suf = S.upper_bound(seg(x + 1, INF)); 
	auto it = std::prev(suf); 

	if (it->r < x - 1)
	{
		Splay::erase(suf->l); 
		insert(it->r, x); 
		insert(x, suf->l); 

		seg cur(x, x); 
		if (x == it->r + 2)
			cur.l = it->l, S.erase(it); 
		if (x == suf->l - 2)
			cur.r = suf->r, S.erase(suf); 
		S.insert(cur); 
	}
	else if (x == it->r + 1)
	{
		Splay::erase(suf->l); 
		Splay::erase(it->r); 
		seg cur(it->l, it->r - 2); 
		S.erase(it);

		if (cur.l <= cur.r)
		{
			S.insert(cur);  
			insert(cur.r, suf->l); 
		}
		else
			insert(std::prev(suf)->r, suf->l); 
		addFib(x + 1); 
	}
	else if ((x & 1) != (it->l & 1))
	{
		int l = it->l, r = it->r; 

		S.erase(it); 
		Splay::erase(suf->l); 

		if (x == l - 1)
		{
			for (int i = x + 2; i <= r; i += 2)
				Splay::erase(x + 3); 
			Splay::erase(x + 1); 
		}
		else
		{
			for (int i = x; i <= r; i += 2)
				Splay::erase(x + 1); 
		}

		insert(std::prev(suf)->r, suf->l); 
		if (l <= x - 1)
			S.insert(seg(l, x - 1)); 
		addFib(r + 1); 
	}
	else
	{
		int l = it->l, r = it->r; 

		S.erase(it); 
		Splay::erase(suf->l); 

		if (x == l)
		{
			for (int i = x + 2; i <= r; i += 2)
				Splay::erase(x + 2); 
			Splay::erase(x); 
		}
		else
		{
			for (int i = x; i <= r; i += 2)
				Splay::erase(x); 
			if (l + 1 <= x - 1)
			{
				Splay::erase(l); 
				insert(std::prev(S.insert(seg(l + 1, x - 1)).first)->r, l + 1); 
			}
		}

		insert(std::prev(suf)->r, suf->l); 
		addFib(r + 1), addFib(l - 2); 
	}
}

int main()
{
	Splay::init(); 
	S.insert(seg(-INF, -INF)); 
	S.insert(seg(+INF, +INF)); 

	read(n); 
	for (int i = 1, x; i <= n; ++i)
	{
		read(x); 
		addFib(x); 
		putint(Splay::query()); 
		putchar('\n'); 
	}

	return 0; 
}