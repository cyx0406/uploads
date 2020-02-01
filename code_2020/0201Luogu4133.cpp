#include <cmath>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>

typedef long long ll; 

const int MaxN = 110; 

ll n, f[MaxN]; 
int m, cnt, pos[MaxN]; 
int g[MaxN][2]; 

int main()
{
    std::cin >> n; 
    f[1] = 1, f[2] = 2; 
    for (m = 3; f[m - 1] <= n; ++m)
        f[m] = f[m - 1] + f[m - 2]; 
    --m; 
    for (int i = m; i >= 1; --i)
        if (n >= f[i])
        {
            n -= f[i]; 
            pos[++cnt] = i; 
        }
    std::sort(pos + 1, pos + cnt + 1); 
    g[1][1] = 1, g[1][0] = pos[1] - 1 >> 1; 
    for (int i = 2; i <= cnt; ++i)
    {
        g[i][1] = g[i - 1][0] + g[i - 1][1]; 
        g[i][0] = g[i - 1][1] * (pos[i] - pos[i - 1] - 1 >> 1) + g[i - 1][0] * (pos[i] - pos[i - 1] >> 1);  
    }

    printf("%d\n", g[cnt][0] + g[cnt][1]);  

    return 0; 
}