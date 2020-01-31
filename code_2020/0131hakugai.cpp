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
	static char buf[25], *tail = buf; 
	if (!x)
		putchar('0');
	else
	{
		for (; x; x /= 10) *++tail = x % 10 + '0'; 
		for (; tail != buf; --tail) putchar(*tail); 
	}
}

typedef long long s64; 

s64 mod; 
int a, b, n, K, p; 

inline s64 plus(s64 x, s64 y)
{
	x += y; 
	return x >= mod ? x - mod : x; 
}

typedef long double ld; 

inline s64 qmul(s64 a, s64 b)
{
    s64 res = a * b - (s64)((ld)a / mod * b + 1e-8) * mod; 
    return res < 0 ? res + mod : res;  
}

struct mat
{
	int r, c; 
	s64 a[2][2];
	mat(){}
	mat(int _r, int _c):
		r(_r), c(_c) {memset(a, 0, sizeof(a));}
	inline mat operator * (const mat &rhs) const
	{
		mat res(r, rhs.c);
		for (int i = 0; i < r; ++i)
			for (int k = 0; k < c; ++k)
				for (int j = 0; j < rhs.c; ++j)
					res.a[i][j] = plus(res.a[i][j], qmul(a[i][k], rhs.a[k][j])); 
		return res; 
	}
	inline mat operator ^ (s64 p) const
	{
		mat res(r, c), x = *this; 
		res.init(); 
		for (; p; p >>= 1, x = x * x)
			if (p & 1)
				res = res * x; 
		return res; 
	}
	inline void init()
	{
		for (int i = 0; i < r; ++i)
			a[i][i] = 1; 
	}
}T(2, 2), F0(1, 2); 

inline s64 calc_G(s64 n, s64 p)
{
	if (n == 0)
		return a % p; 
	else if (n == 1)
		return b % p; 
	
	mod = p; 
	T.a[0][0] = 3 % mod, T.a[0][1] = 1 % mod; 
	T.a[1][0] = mod - 1, T.a[1][1] = 0; 
	F0.a[0][0] = b % mod, F0.a[0][1] = a % mod; 
	return (F0 * (T ^ (n - 1))).a[0][0]; 
}

const int MaxN = 1e6 + 5; 
int pri[MaxN], n_pri; 

inline s64 lcm(s64 a, s64 b)
{
	return a / std::__gcd(a, b) * b; 
}

inline void sieve_init(int n = 1000000)
{
	static bool sie[MaxN]; 

	for (int i = 2; i <= n; ++i)
	{
		if (!sie[i])
			pri[++n_pri] = i; 
		for (int j = 1; j <= n_pri && pri[j] * i <= n; ++j)
		{
			sie[i * pri[j]] = true; 
			if (i % pri[j] == 0)
				break; 
		}
	}
}

inline s64 getp(s64 p)
{
	if (p == 2)
		return 3; 
	else if (p == 3)
		return 8; 
	else if (p == 5)
		return 20; 
	else if (p % 5 == 1 || p % 5 == 4)
		return p - 1; 
	else
		return 2 * p + 2; 
} 

inline s64 find_period(s64 p)
{
	s64 x = p; 
	s64 res = 1; 
	for (int i = 1; x > 1 && i <= n_pri && 1LL * pri[i] * pri[i] <= p && pri[i] <= x; ++i)
		if (x % pri[i] == 0)
		{
			x /= pri[i]; 
			s64 cur = getp(pri[i]); 
			while (x % pri[i] == 0)
				x /= pri[i], cur *= pri[i]; 
			res = lcm(res, cur); 
		}
	if (x > 1)
		res = lcm(res, getp(x)); 
	return res; 
}

inline s64 calc(s64 n, int k, s64 p)
{
	if (k == 0)
		return n % p; 
	return calc_G(calc(n, k - 1, find_period(p)), p); 
}

int main()
{
	sieve_init(); 
	
	int orzcx; 
	read(orzcx); 
	while (orzcx--)
	{
		read(a), read(b), read(n), read(K), read(p);
		
		if (p == 1)
		{
			puts("0"); 
			continue; 
		}
		putint(calc(n, K, p)); 
		putchar('\n'); 
	}
		
	return 0; 	
}