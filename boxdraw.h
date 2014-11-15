#ifndef XZ_BOX_DRAW
#define XZ_BOX_DRAW

#define LINESTYLE_NONE   0
#define LINESTYLE_NORMAL 1
#define LINESTYLE_BOLD   2
#define LINESTYLE_DASH   3
#define LINESTYLE_BDASH  4

#define UP      8
#define RIGHT   4
#define DOWN    2
#define LEFT    1

#define SHOW_NULL   0
#define SHOW_LV     1
#define SHOW_LVCHAR 2

typedef unsigned char grid_t;

const int MAX_LV = 10;
const int INNER_CHAR_SIZE = 3;

class BoxDraw {
private:
    grid_t **map;
    int **border;
    
    char ***content;

    int height;
    int width;
    int *cwidth;
    int *rheight;
    char *lvchar[MAX_LV];
    int show_mode;

    static void _setLine(int &charDesc, int dir, int linestyle);
public:
    BoxDraw(int height, int width);
    void setGridLv(int x, int y, grid_t lv);
    void setLvChar(grid_t lv, const char *s);
    void setSquare(int x1, int y1, int x2, int y2, grid_t lv);
    void setBorder(grid_t lv1, grid_t lv2, int lineStyle);
    void setColWidth(int idx, int width);
    void setRowHeight(int idx, int height);
    void setGridContent(int x, int y, const char *con, int len = -1 );
    
    int **render(FILE* f);
    void finalize();
};

#endif
