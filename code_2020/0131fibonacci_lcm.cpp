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
inline void relax(T &x, const T &y)
{
	if (x < y)
		x = y; 
}

const int mod = 1e9 + 7; 
const int MaxN = 5e4 + 5; 
const int MaxM = 1e6 + 5; 

int n, m; 
int a[MaxN]; 
bool vis[MaxM]; 
int f[MaxM], g[MaxM], t[MaxM]; 

inline void add(int &x, const int &y)
{
	x += y; 
	if (x >= mod)
		x -= mod; 
}

inline int qpow(int x, int y)
{
	if (x == 1)
		return 1; 

	int res = 1; 
	for (; y; y >>= 1, x = 1LL * x * x % mod)
		if (y & 1)
			res = 1LL * res * x % mod; 
	return res; 
}

int main()
{
	read(n); 
	for (int i = 1; i <= n; ++i)
	{
		read(a[i]); 
		relax(m, a[i]); 
		vis[a[i]] = true; 
	}
	f[1] = 1; 
	for (int i = 2; i <= m; ++i)
		add(f[i] = f[i - 1], f[i - 2]); 
	for (int i = 1; i <= m; ++i)
		t[i] = 1; 

	int ans = 1; 
	for (int i = 1; i <= n; ++i)
	{
		g[i] = 1LL * f[i] * qpow(t[i], mod - 2); 
		
		bool flg = false; 
		if (vis[i])
			flg = true; 
		for (int j = i + i; j <= m; j += i)
		{
			t[j] = 1LL * t[j] * g[i] % mod; 
			if (vis[j])
				flg = true; 
		}
		if (flg)
			ans = 1LL * ans * g[i] % mod; 
	}

	std::cout << ans << std::endl; 

	return 0; 
}