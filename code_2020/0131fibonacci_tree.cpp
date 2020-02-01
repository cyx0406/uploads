#include <bits/stdc++.h>

typedef long long s64; 

template <class T>
inline void read(T &x)
{
	static char ch; 
	static bool opt; 
	while (!isdigit(ch = getchar()) && ch != '-'); 
	x = (opt = ch == '-') ? 0 : ch - '0'; 
	while (isdigit(ch = getchar()))
		x = x * 10 + ch - '0'; 
	if (opt) x = ~x + 1; 
}

inline bool getopt()
{
	static char ch; 
	while ((ch = getchar()) != 'U' && ch != 'Q'); 
	return ch == 'U'; 
}

const int MaxNV = 1e5 + 5; 
const int MaxNE = MaxNV << 1; 
const int MaxNode = MaxNV << 2; 
const int mod = 1e9 + 7; 

inline void add(int &x, const int &y)
{
	x += y; 
	if (x >= mod)
		x -= mod; 
	if (x < 0)
		x += mod; 
}

struct matrix
{
	int r, c; 
	int a[3][3]; 
	
	matrix(){}
	matrix(const int &x, const int &y):
		r(x), c(y)
	{
		memset(a, 0, sizeof(a)); 
	}
	inline void clear()
	{
		memset(a, 0, sizeof(a)); 
	}
	inline void init()
	{
		memset(a, 0, sizeof(a)); 
		for (int i = 1; i <= r; ++i)
			a[i][i] = 1; 
	}
	inline matrix operator * (const matrix &rhs) const
	{
		matrix res(r, rhs.c); 
		for (int i = 1; i <= r; ++i)
			for (int j = 1; j <= rhs.c; ++j)
				for (int k = 1; k <= c; ++k)
					add(res.a[i][j], 1LL * a[i][k] * rhs.a[k][j] % mod); 
		return res; 
	}
	inline matrix operator ^ (s64 p) const
	{
		matrix x = *this, res(r, c); 
		res.init(); 
		
		for (; p; p >>= 1, x = x * x)
			if (p & 1)
				res = res * x; 
		return res; 
	}
}A(1, 2), T(2, 2); 

int ect, adj[MaxNV], to[MaxNE], nxt[MaxNE]; 
int n, m, f_neg[MaxNV], f_pos[MaxNV], dis1[MaxNV], dis2[MaxNV]; 
int fa[MaxNV], son[MaxNV], sze[MaxNV], dep[MaxNV], pos[MaxNV], idx[MaxNV], top[MaxNV], totpos; 

int tag1[MaxNode], tag2[MaxNode]; 
int sum1[MaxNode], sum2[MaxNode]; 

#define lc (x << 1)
#define rc (x << 1 | 1)
#define trav(u) for (int e = adj[u], v; v = to[e], e; e = nxt[e])

inline void addEdge(const int &u, const int &v)
{
	nxt[++ect] = adj[u], adj[u] = ect, to[ect] = v; 
}

inline int getfib(const s64 &x)
{
	if (x <= 0)
		return f_neg[-x]; 
	if (x < MaxNV)
		return f_pos[x]; 
	return (A * (T ^ (x - 1))).a[1][1]; 
}

inline void dfs1(const int &u)
{
	dep[u] = dep[fa[u]] + 1; 
	sze[u] = 1; 
//	son[u] = top[u] = 0; 
	
	trav(u) if (v != fa[u])
	{
		fa[v] = u; 
		dfs1(v); 
		sze[u] += sze[v]; 
		if (sze[v] > sze[son[u]])
			son[u] = v; 
	}
}

inline void dfs2(const int &u)
{
	if (son[u])
	{
		idx[pos[son[u]] = ++totpos] = son[u]; 
		top[son[u]] = top[u]; 
		dfs2(son[u]); 
	}
	trav(u)
		if (v != son[u] && v != fa[u])
		{
			idx[pos[v] = ++totpos] = v; 
			top[v] = v; 
			dfs2(v); 
		}
}

inline void upt(const int &x)
{
	add(sum1[x] = sum1[lc], sum1[rc]); 
	add(sum2[x] = sum2[lc], sum2[rc]); 
}

inline void add_node(const int &x, const int &l, const int &r, const int &val1, const int &val2)
{
//	printf(":%d %d %d %d %d:%d\n", x, l, r, val1, val2, dis1[r] - dis1[l - 1]); 
	add(sum1[x], 1LL * (dis1[r] - dis1[l - 1]) * val1 % mod); 
	add(tag1[x], val1); 
	add(sum2[x], 1LL * (dis2[r] - dis2[l - 1]) * val2 % mod); 
	add(tag2[x], val2); 	
}

inline void dnt(const int &x, const int &l, const int &r)
{
	if (tag1[x] || tag2[x])
	{
		int mid = l + r >> 1; 
		add_node(lc, l, mid, tag1[x], tag2[x]); 
		add_node(rc, mid + 1, r, tag1[x], tag2[x]); 
		tag1[x] = tag2[x] = 0; 
	}
}

inline void modify(const int &x, const int &l, const int &r, const int &u, const int &v, const int &val1, const int &val2)
{
	if (u <= l && r <= v)
	{
		add_node(x, l, r, val1, val2); 
		return; 
	}
	dnt(x, l, r); 
	int mid = l + r >> 1; 
	if (u <= mid)
		modify(lc, l, mid, u, v, val1, val2); 
	if (v > mid)
		modify(rc, mid + 1, r, u, v, val1, val2); 
	upt(x); 
}

inline int query(const int &x, const int &l, const int &r, const int &u, const int &v)
{
	if (u <= l && r <= v)
	{
		int res = sum1[x] + sum2[x]; 
		if (res >= mod)
			res -= mod; 
		return res; 
	}
	dnt(x, l, r); 
	int mid = l + r >> 1, res = 0; 
	if (u <= mid)
		add(res, query(lc, l, mid, u, v)); 
	if (v > mid)
		add(res, query(rc, mid + 1, r, u, v)); 
	return res; 
}

inline int path_query(int u, int v)
{
	int res = 0; 
	while (top[u] != top[v])
	{
		if (dep[top[u]] < dep[top[v]])
			std::swap(u, v); 
		add(res, query(1, 1, n, pos[top[u]], pos[u])); 
		u = fa[top[u]]; 
	}
	if (dep[u] > dep[v])
		std::swap(u, v); 
	add(res, query(1, 1, n, pos[u], pos[v])); 
	return res; 
}

int main()
{
	A.a[1][1] = T.a[1][1] = T.a[1][2] = T.a[2][1] = 1; 
		
	read(n), read(m); 
	for (int i = 2; i <= n; ++i)
	{
		int u, v; 
        read(u), read(v); 
        addEdge(u, v); 
        addEdge(v, u); 
	}
	
	dfs1(1); 
	pos[1] = idx[1] = top[1] = totpos = 1; 
	dfs2(1); 
	
	f_neg[0] = 0, f_neg[1] = 1; 
	for (int i = 2; i <= n; ++i)
		add(f_neg[i] = f_neg[i - 2], -f_neg[i - 1]); 
	f_pos[1] = f_pos[2] = 1; 
	for (int i = 3; i < MaxNV; ++i)
		add(f_pos[i] = f_pos[i - 1], f_pos[i - 2]); 
	for (int i = 1; i <= n; ++i)
	{
		add(dis1[i] = dis1[i - 1], getfib(dep[idx[i]])); 
		add(dis2[i] = dis2[i - 1], getfib(dep[idx[i]] - 1)); 
	}
	
	bool opt; 
	int x; 
	s64 y; 
	while (m--)
	{
		opt = getopt(), read(x), read(y); 
		if (opt)
			modify(1, 1, n, pos[x], pos[x] + sze[x] - 1, getfib(y - dep[x] + 1), getfib(y - dep[x]));
		else
			printf("%d\n", path_query(x, y));  
	}
	
	return 0; 
}