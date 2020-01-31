#include <bits/stdc++.h>

const int mod = 1e9 + 7; 

int n, K; 

struct mat
{
	int r, c; 
	int a[5][5]; 
	
	mat(){}
	mat(int n, int m):
		r(n), c(m) {memset(a, 0, sizeof(a));}
	
	inline void init() //初始化单位矩阵 
	{
		for (int i = 1; i <= r; ++i)
			a[i][i] = 1; 
	}
	
	inline mat operator * (const mat &rhs) const //矩阵乘法 
	{
		mat res(r, rhs.c); 
		for (int i = 1; i <= r; ++i)
			for (int k = 1; k <= c; ++k)
				for (int j = 1; j <= rhs.c; ++j)
					res.a[i][j] = (res.a[i][j] + 1LL * a[i][k] * rhs.a[k][j]) % mod; 
		return res; 
	}
	
	inline mat operator ^ (int p) const //矩阵快速幂 
	{
		mat res(r, c), x = *this; 
		res.init(); 
		for (; p; p >>= 1, x = x * x)
			if (p & 1)
				res = res * x; 
		return res; 
	}
}; 

int main()
{
	int TAT; 
	std::cin >> TAT; 
	while (TAT--)
	{
		scanf("%d%d", &n, &K); 
		
		mat T(4, 4); 
		T.a[1][1] = T.a[1][3] = T.a[2][2] = 1; 
		T.a[2][4] = T.a[3][1] = T.a[4][2] = 1; 
		mat F1 = mat(1, 4); 
		F1.a[1][2] = 1, F1.a[1][3] = 1; 
		mat F2 = F1; 
		
		F1 = F1 * (T ^ n); 
		F2 = F2 * (T ^ (n + 1)); 
		int a1 = F1.a[1][1], b1 = F1.a[1][2]; 
		int a2 = F2.a[1][1], b2 = F2.a[1][2]; 
		//预处理类斐波拉契的n+1,n+2项对应的系数 
		
		mat F(1, 2); 
		F.a[1][2] = 1; 
		
		T = mat(2, 2); 
		T.a[1][1] = a1, T.a[1][2] = a2 ? a2 - 1 : mod - 1; 
		T.a[2][1] = b1 ? b1 - 1 : b1, T.a[2][2] = b2 ? b2 - 1 : b2; 
		//通过得到的系数得出转移矩阵 
		
		F = F * (T ^ K); 
		printf("%d\n", F.a[1][2]); 
	}
	
	fclose(stdin); 
	fclose(stdout); 
	return 0; 
}
