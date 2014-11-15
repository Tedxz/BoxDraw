#include <cstdio>
#include <iostream>
#include <cstring>
#include "boxdraw.h"

using namespace std;

// common functions ------------------------------------------------------------

void swap(int &a, int &b) {
    int t = a;
    a = b;
    b = t;
}
int lowbit(int x) {
    return x & -x;
}

// variables -------------------------------------------------------------------

char _char_code[73][INNER_CHAR_SIZE];
char *_char_map[1<<(5*3)];
bool _loaded_flag = false;

// implements ------------------------------------------------------------------

void load() {
    char sbuf[INNER_CHAR_SIZE];
    int id;
    int desc;

    FILE *fcc = fopen("charcode.dat", "r");
    while (fscanf(fcc, "%d\t%s", &id, sbuf) != EOF) {
        strcpy(_char_code[id], sbuf);

    }
    fclose(fcc);

    FILE *fcm = fopen("charmap.dat", "r");
    // char description code is encode in octonary
    while (fscanf(fcm, "%o %d", &desc, &id) != EOF) {
        _char_map[desc] = _char_code[id];
    }
    _loaded_flag = true;
}


BoxDraw::BoxDraw(int height, int width) {
    show_mode = SHOW_LVCHAR;
    this->height = height;
    this->width = width;

    map = new grid_t* [height + 2];
    for (int i = 0; i < height + 2; ++i) {
        map[i] = new grid_t[width + 2];
        memset(map[i], 0, (width + 2) * (sizeof (grid_t)));
    }
    
    border = new int*[MAX_LV];
    for (int i = 0; i < MAX_LV; ++i) {
        border[i] = new int[MAX_LV];
        memset(border[i], 0, MAX_LV * (sizeof (int)));
    }

    cwidth = new int [width + 2];
    for (int i = 0; i < width + 2; ++i)
        cwidth[i] = 1;

    rheight = new int [height + 2];
    for (int i = 0; i < height + 2; ++i)
        rheight[i] = 1;
        
    memset(lvchar, 0, sizeof lvchar);
    
    content = new char**[height + 2];
    for (int i = 0; i < height + 2; ++i) {
        content[i] = new char*[width + 2];
        memset(content[i], 0, (width + 2) * sizeof (char*));
    }

    if (!_loaded_flag)
        load();
}

void BoxDraw::setGridLv(int x, int y, grid_t lv) {
    map[x][y] = lv;
}

void BoxDraw::setLvChar(grid_t lv, const char *s) {
    lvchar[lv] = strdup(s);
}

void BoxDraw::setSquare(int x1, int y1, int x2, int y2, grid_t lv) {
    if (x1 > x2) swap(x1, x2);
    if (y1 > y2) swap(x1, x2);
    if (x1 < 1 || x2 > height) return;
    if (y1 < 1 || y2 > width) return;
    for (int i = x1; i <= x2; ++i)
        memset(&map[i][y1], lv, (y2 - y1 + 1) * (sizeof (grid_t)));
}

void BoxDraw::setBorder(grid_t lv1, grid_t lv2, int lineStyle) {
    border[lv1][lv2] = border[lv2][lv1] = lineStyle;
}


void BoxDraw::_setLine(int &charDesc, int dir, int linestyle) {
    for (int td = lowbit(dir); dir; td = lowbit(dir)) {
        dir -= lowbit(dir);
        charDesc &= ~(0x07 * td * td * td);
        charDesc |= linestyle * td * td * td;
    }
}

int **BoxDraw::render(FILE *f) {
    int **arr;
    arr = new int*[(height + 2) * 2];
    for (int i = 0; i  <(height + 2) * 2; ++i) {
        arr[i] = new int[(width + 2) * 2];
        memset(arr[i], 0, ((width + 2) * 2) * (sizeof (int)));
    }
    int tls;
    for (int i = 1; i < height + 2; ++i) {
        int ii = i << 1;
        for (int j = 1; j < width + 2; ++j) {
            int jj = j << 1;
            arr[ii][jj] = map[i][j];

            // left
            tls = border[map[i][j]][map[i][j-1]];
            _setLine(arr[ii][jj-1], UP|DOWN, tls);
            _setLine(arr[ii-1][jj-1], DOWN, tls);
            _setLine(arr[ii+1][jj-1], UP, tls);

            // up
            tls = border[map[i][j]][map[i-1][j]];
            _setLine(arr[ii-1][jj], LEFT|RIGHT, tls);
            _setLine(arr[ii-1][jj-1], RIGHT, tls);
            _setLine(arr[ii-1][jj+1], LEFT, tls);

        }
    }

    //test output
    for (int i = 1; i < (height + 1) * 2; ++i) {
        int irep = 1;
        if (!(1 & i)) irep = rheight[i >> 1];
        for (int ii = 0; ii < irep; ++ii) {
            for (int j = 1; j < (width + 1) * 2; ++j) {
                int jrep = 1;
                if (!(1 & j)) jrep = cwidth[j >> 1];
                char *con = content[i >> 1][j >> 1];
                for (int jj = 0; jj < jrep; ++jj) {
                    if ((i & 1) || (j & 1)) {
                        fprintf(f, "%s", _char_map[arr[i][j]]);
                    } else {
                        switch (show_mode) {
                            case SHOW_LV:
                                fprintf(f, "%d ", arr[i][j]);
                                break;
                            case SHOW_LVCHAR:
                                if (con != NULL && !ii) {                                
                                    if (jj * 2 < strlen(con))
                                        fprintf(f, "%c%c", 
                                                con[jj * 2], con[jj * 2 + 1]);
                                    else
                                        fprintf(f, "  ");
                                } else if (lvchar[arr[i][j]]) {
                                    fprintf(f, "%s", lvchar[arr[i][j]]);
                                } else {
                                    fprintf(f, "  ");
                                }
                                break;
                            default:    
                                fprintf(f, "  ");
                                break;
                        }
                    }
                }
            }
            fprintf(f, "\n");
        }
    }
}

void BoxDraw::setColWidth(int idx, int width) {
    cwidth[idx] = width;
}

void BoxDraw::setRowHeight(int idx, int height) {
    rheight[idx] = height;
}

void BoxDraw::setGridContent(int x, int y, const char *con, int len) {
    if (content[x][y] != NULL)
        delete[] content[x][y];
    content[x][y] = strdup(con);
    if (len < 0) len = strlen(con);
    len = (len + 1) >> 1;
    if (len > cwidth[y]) cwidth[y] = len;
        
}


void BoxDraw::finalize() {
    delete[] cwidth;
    delete[] rheight;
    for (int i = 0; i < height + 2; ++i)
        delete[] map[i];
    delete []map;
    for (int i = 0; i < MAX_LV; ++i)
        delete[] border[i];
    delete[] border;
    for (int i = 0; i < height + 2; ++i) {
        for (int j = 0; j < width + 2; ++j)
            if (content[i][j] != NULL)
                delete[] content[i][j];
        delete[] content[i];
    }
    delete[] content;
                
}

