#include <cstdio>
#include <cstring>
#include <iostream>
#include "boxdraw.h"
#include "bitfont.cpp"

using namespace std;

int main1() {
    BoxDraw bd(3, 3);
    bd.setSquare(1, 1, 3, 3, 1);
    bd.setGridLv(1, 1, 2);
    bd.setGridLv(1, 3, 2);
    bd.setGridLv(3, 1, 2);
    bd.setGridLv(3, 3, 2);
    
    bd.setLvChar(1, "  ");
    bd.setLvChar(2, "  ");
    bd.setBorder(1, 1, LINESTYLE_NONE);
    bd.setBorder(0, 1, LINESTYLE_BOLD);
    bd.setBorder(1, 2, LINESTYLE_BOLD);
    bd.setBorder(0, 2, LINESTYLE_NONE);
    
    bd.render(stdout);
    bd.finalize();
    return 0;
}

int main() {
    FILE *f = fopen("a.txt", "w");
    char *c = "Ply_py";
    BoxDraw bd(8, 6 * strlen(c));
    for (int k = 0; k < strlen(c); ++k) {
        for (int i = 0; i < 5; ++i) {
            for (int j = 0; j < 8; ++j) {
                if (font[5*c[k] + i] & (1<<j))
                    bd.setGridLv(j + 1, k * 6 + i + 1, 1);
            }
        }
    }
    bd.setBorder(0, 1, LINESTYLE_BOLD);
    bd.setLvChar(0, "  ");
    bd.setLvChar(1, "  ");
    bd.render(f);
    bd.finalize();
}

int main3() {
    //freopen("a.txt", "w", stdout);
    BoxDraw bd(3, 3);
    bd.setSquare(1, 1, 3, 3, 1);

    
    bd.setLvChar(1, "  ");
    bd.setLvChar(2, "  ");
    bd.setBorder(1, 1, LINESTYLE_NORMAL);
    bd.setBorder(0, 1, LINESTYLE_NORMAL);
    
    bd.setGridContent(1, 1, "夏雨民");
    bd.setGridContent(2, 1, "张伟华");
    bd.setGridContent(3, 1, "解铮");
    bd.setGridContent(1, 2, "2120505070");
    bd.setGridContent(2, 2, "2120505098");
    bd.setGridContent(3, 2, "2120505111");
    bd.setGridContent(1, 3, "计算机23");
    bd.setGridContent(2, 3, "计算机24");
    bd.setGridContent(3, 3, "计算机25");
    
    //bd.setRowHeight(1, 2);
    
    bd.render(stdout);
    bd.finalize();
    return 0;
}

