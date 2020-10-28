/**************************************************************************************\
 *** Snake 1.3
 *** Desenvolvedor: Abner Cordeiro ( Hellhound )
 *** 2014 - Todos os Direitos reservados.
\**************************************************************************************/
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#define TIMER_GAME 1

#define WIN_WIDTH_RELEASE  500
#define WIN_HEIGHT_RELEASE 650
#define WIN_WIDTH_DEBUG    500
#define WIN_HEIGHT_DEBUG   530

#define TOTAL_TILES_LINHAS 24
#define TOTAL_TILES_COLUNAS 24

#define GAME_MENU     0
#define GAME_RUNNING  1
#define GAME_PAUSED   2
#define GAME_GAMEOVER 3

#define CIMA     1
#define BAIXO    2
#define DIREITO  3
#define ESQUERDO 4

#define CABECA_1 1
#define CABECA_2 2
#define CABECA_3 3
#define CABECA_4 4
#define CORPO_1  5
#define CORPO_2  6
#define RABO_1   7
#define RABO_2   8
#define RABO_3   9
#define RABO_4   10
#define VERTICE_1 11
#define VERTICE_2 12
#define VERTICE_3 13
#define VERTICE_4 14

#define WINCLASS_STR "Snake_1.3_Class"
#define WINTITLE "Snake 1.3 - TESTANDO 1,2,3... - Hellhound"
#define SHIFTED 0x8000
typedef struct tagOBJETO
{
    int iLinha;
    int iColuna;
    int iTipoPart; // cabeça corpo ou rabo
    int iDirecao;  // para cima,para baixo,direita ,esquerda
    int iVertice;
}OBJETO;

typedef struct tagRECORDS
{
    char szNome[MAX_PATH];
    int iRecord;
}*PRECORDS,RECORDS;

typedef struct tagGAME
{
    HINSTANCE hInst;
    HWND hwnd;
    RECT rc;

    int iGameStatus;
    int iMenuSelected;

    int iNumSegmentos;
    int iDirecaoCorrent;
    BOOL bPegou;
    int iNumTotalPoints;
    int iCountBigFood;
    int iMatriz1[TOTAL_TILES_LINHAS + 10][TOTAL_TILES_COLUNAS + 10];

    OBJETO Snake[TOTAL_TILES_LINHAS * TOTAL_TILES_COLUNAS];

    POINT Food;
    POINT BigFood[2];

    int iSnakeVelocit;

    int iTempoText;
    int iTextX;
    int iTextY;

    PRECORDS pRecords;

    int TeclaPressionada;
    HDC hDC_Screen;
    HBITMAP hBitmapScreen;

    HBITMAP hBitmapMenuBack;
    HBITMAP hBitmapGameBack;
    HBITMAP hBitmapMenuNewGame;
    HBITMAP hBitmapMenuQuit;
    HBITMAP hBitmapMenuSelect;

    HBITMAP hBitmapSnakeHead;
    HBITMAP hBitmapSnakeBody;
    HBITMAP hBitmapSnakeTail;

    HBITMAP hBitmapFood;
    HBITMAP hBitmapBigFood;
    HBITMAP hBitmapNumbers;

    HFONT hFonte1;
    HFONT hFonte2;
}GAME_INF;

void BlendStretch(HDC hDC_Dest,
                  HBITMAP hBitmap,
                  int nXOriginDest,int nYOriginDest,
                  int nWidthDest  ,int nHeightDest,
                  int nXOriginSrc ,int nYOriginSrc,
                  int nWidthSrc   ,int nHeightSrc);



void BlendAlpha(HDC hDC_Dest,
                HBITMAP hBitmap,
                BYTE ConstantAlpha,
                int xoriginDest,int yoriginDest,
                int wDest,int hDest,
                int xoriginSrc,int yoriginSrc,
                int wSrc,int hSrc);


void BlendTransparent(HDC hDC_Dest,
                      HBITMAP hBitmap,
                      int xoriginDest,int yoriginDest,
                      int wDest,int hDest,
                      int xoriginSrc,int yoriginSrc,
                      int wSrc,int hSrc,UINT crTransparent);

void NewGame(GAME_INF *pGameInf);


PRECORDS LoadRecord(GAME_INF *pGameInf);
PRECORDS SaveRecord(GAME_INF *pGameInf);

BOOL ShowDialogAddRecord(GAME_INF *pGameInf);
INT_PTR CALLBACK DialogAddRecordProc( HWND ,UINT ,WPARAM ,LPARAM );

#endif // MAIN_H_INCLUDED
