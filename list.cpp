#include <cstdio>
#include <iostream>

using namespace std;

char s[][10] = {
    "ÎÞ", 
    "Ï¸",
    "´Ö",
    "Ðé",
    "ºÚ"
};

int main() {
    freopen("a.csv", "w", stdout);
    for (int i = 0; i < 5; ++i) 
    {
        for (int j = 0; j < 5; ++j)
        {
            for (int k = 0; k < 5; ++k)
            {
                for (int l = 0; l < 5; ++l)
                {
                    if (!i + !j + !k + !l == 3) continue;
                    printf("%s%s%s%s,", s[i], s[j], s[k], s[l]);
                }
            }
        }
    }
}
