#include <3ds.h>
#include <citro2d.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 10
#define CELL 21
#define GRID_X 88
#define GRID_Y 68

#define BG       C2D_Color32(7, 10, 28, 255)
#define PANEL    C2D_Color32(18, 23, 51, 255)
#define WHITE    C2D_Color32(242, 248, 255, 255)
#define CYAN     C2D_Color32(48, 245, 255, 255)
#define PINK     C2D_Color32(255, 65, 205, 255)
#define PURPLE   C2D_Color32(139, 91, 255, 255)
#define GREEN    C2D_Color32(91, 255, 174, 255)
#define YELLOW   C2D_Color32(255, 226, 99, 255)

typedef struct {
    int count;
    int xy[5][2];
    u32 color;
} Piece;

static u8 board[N][N];
static u32 boardColor[N][N];
static Piece pieces[3];
static int score = 0, bestScore = 0, selected = -1, gameOver = 0;
static C2D_TextBuf textBuf;
static C2D_Text text;

static const u32 palette[] = {CYAN, PINK, PURPLE, GREEN, YELLOW};
static const Piece templates[] = {
    {3, {{0,0},{1,0},{2,0}}, CYAN},
    {4, {{0,0},{1,0},{0,1},{1,1}}, PINK},
    {3, {{0,0},{0,1},{1,1}}, PURPLE},
    {4, {{0,0},{1,0},{2,0},{2,1}}, GREEN},
    {3, {{0,0},{1,0},{1,1}}, YELLOW},
    {3, {{0,0},{0,1},{0,2}}, PINK},
    {4, {{0,0},{1,0},{2,0},{1,1}}, CYAN}
};

static void label(const char *s, float x, float y, float scale, u32 color) {
    C2D_TextParse(&text, textBuf, s);
    C2D_TextOptimize(&text);
    C2D_DrawText(&text, C2D_WithColor, x, y, 0.5f, scale, scale, color);
}

static void drawBlock(float x, float y, float size, u32 color) {
    C2D_DrawRectSolid(x, y, size-1, size-1, C2D_Color32(3, 6, 19, 255));
    C2D_DrawRectSolid(x+2, y+2, size-5, size-5, color);
    C2D_DrawRectSolid(x+3, y+3, size-7, 2, C2D_Color32(255,255,255,95));
}

static void newPiece(Piece *p) {
    *p = templates[rand() % (sizeof(templates) / sizeof(templates[0]))];
    p->color = palette[rand() % (sizeof(palette) / sizeof(palette[0]))];
}

static void refillPieces(void) {
    for (int i = 0; i < 3; i++) newPiece(&pieces[i]);
    selected = -1;
}

static int canPlace(const Piece *p, int x, int y) {
    for (int i = 0; i < p->count; i++) {
        int bx = x + p->xy[i][0], by = y + p->xy[i][1];
        if (bx < 0 || bx >= N || by < 0 || by >= N || board[by][bx]) return 0;
    }
    return 1;
}

static void clearFullLines(void) {
    int rows[N] = {0}, cols[N] = {0}, cleared = 0;
    for (int y = 0; y < N; y++)
        for (int x = 0; x < N; x++)
            if (board[y][x]) { rows[y]++; cols[x]++; }

    for (int y = 0; y < N; y++) if (rows[y] == N) {
        for (int x = 0; x < N; x++) board[y][x] = 0;
        cleared++;
    }
    for (int x = 0; x < N; x++) if (cols[x] == N) {
        for (int y = 0; y < N; y++) board[y][x] = 0;
        cleared++;
    }

    if (cleared) score += cleared * 100 + (cleared > 1 ? cleared * cleared * 75 : 0);
    if (score > bestScore) bestScore = score;
}

static void updateGameOver(void) {
    gameOver = 1;
    for (int i = 0; i < 3; i++) if (pieces[i].count)
        for (int y = 0; y < N; y++)
            for (int x = 0; x < N; x++)
                if (canPlace(&pieces[i], x, y)) gameOver = 0;
}

static void placePiece(int idx, int x, int y) {
    Piece *p = &pieces[idx];
    if (!canPlace(p, x, y)) return;

    for (int i = 0; i < p->count; i++) {
        int bx = x + p->xy[i][0], by = y + p->xy[i][1];
        board[by][bx] = 1;
        boardColor[by][bx] = p->color;
    }
    score += p->count * 10;
    p->count = 0;
    clearFullLines();

    int any = 0;
    for (int i = 0; i < 3; i++) if (pieces[i].count) any = 1;
    if (!any) refillPieces();
    updateGameOver();
}

static void resetGame(void) {
    memset(board, 0, sizeof(board));
    memset(boardColor, 0, sizeof(boardColor));
    score = 0;
    gameOver = 0;
    refillPieces();
}

static void drawTop(void) {
    C2D_TargetClear(C2D_GetScreenTarget(GFX_TOP), BG);
    C2D_SceneBegin(C2D_GetScreenTarget(GFX_TOP));
    C2D_DrawRectSolid(7, 7, 386, 226, PANEL);
    C2D_DrawRectSolid(8, 8, 384, 2, CYAN);
    label("NEON BLOCK", 18, 15, 0.65f, CYAN);

    char line[96];
    snprintf(line, sizeof(line), "SCORE %06d   BEST %06d", score, bestScore);
    label(line, 18, 39, 0.40f, WHITE);
    label("CLEAR ROWS + COLUMNS", 18, 51, 0.32f, PURPLE);

    for (int y = 0; y < N; y++) for (int x = 0; x < N; x++) {
        float px = GRID_X + x * CELL, py = GRID_Y + y * CELL;
        C2D_DrawRectSolid(px, py, CELL-1, CELL-1, C2D_Color32(29, 36, 66, 255));
        if (board[y][x]) drawBlock(px, py, CELL, boardColor[y][x]);
    }
    if (gameOver) label("NO MOVES LEFT!", 125, 221, 0.42f, PINK);
}

static void drawBottom(void) {
    C2D_TargetClear(C2D_GetScreenTarget(GFX_BOTTOM), BG);
    C2D_SceneBegin(C2D_GetScreenTarget(GFX_BOTTOM));
    C2D_DrawRectSolid(7, 7, 306, 226, PANEL);
    C2D_DrawRectSolid(8, 8, 304, 2, PINK);
    label("CHOOSE YOUR PIECE", 18, 17, 0.52f, WHITE);
    label("Tap a piece, then tap the board", 18, 42, 0.32f, CYAN);

    for (int i = 0; i < 3; i++) {
        float bx = 16 + i * 100;
        C2D_DrawRectSolid(bx, 73, 88, 92, selected == i ? CYAN : C2D_Color32(68,78,120,255));
        C2D_DrawRectSolid(bx+2, 75, 84, 88, C2D_Color32(11,16,38,255));
        if (!pieces[i].count) {
            label("USED", bx+24, 110, 0.35f, PURPLE);
            continue;
        }
        for (int j = 0; j < pieces[i].count; j++)
            drawBlock(bx+12+pieces[i].xy[j][0]*17, 88+pieces[i].xy[j][1]*17, 17, pieces[i].color);
    }

    C2D_DrawRectSolid(18, 181, 284, 35, C2D_Color32(30,35,67,255));
    label("L + R: NEW GAME     START: EXIT", 23, 191, 0.31f, YELLOW);
    label("NEON EDITION  /  PROTOTYPE", 49, 220, 0.31f, PURPLE);
}

int main(void) {
    gfxInitDefault();
    consoleInit(GFX_BOTTOM, NULL);
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();
    textBuf = C2D_TextBufNew(4096);
    srand((unsigned)time(NULL));
    refillPieces();

    while (aptMainLoop()) {
        hidScanInput();
        u32 down = hidKeysDown();
        touchPosition touch;

        if (down & KEY_START) break;
        if ((hidKeysDown() & KEY_L) && (hidKeysDown() & KEY_R)) resetGame();

        if (down & KEY_TOUCH) {
            hidTouchRead(&touch);
            if (touch.py >= 73 && touch.py <= 165) {
                int idx = touch.px / 100;
                if (idx >= 0 && idx < 3 && pieces[idx].count) selected = idx;
            } else if (selected >= 0 && !gameOver &&
                       touch.px >= GRID_X && touch.px < GRID_X + N*CELL &&
                       touch.py >= GRID_Y && touch.py < GRID_Y + N*CELL) {
                placePiece(selected, (touch.px-GRID_X)/CELL, (touch.py-GRID_Y)/CELL);
            } else if (gameOver && touch.py > 180) resetGame();
        }

        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        drawTop();
        drawBottom();
        C3D_FrameEnd(0);
    }

    C2D_TextBufDelete(textBuf);
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    return 0;
}
