/**************************************************************************************\
 *** Snake 1.3
 *** Desenvolvedor: Abner Cordeiro ( Hellhound )
 *** 2014 - Todos os Direitos reservados.
 ** Arquivo: init.c
 ** Utiliza a biblioteca FreeImage para carregar as imagens png e converte-a
 ** utilizando a WinAPI para bitmap,então libera os indentificadores da FreeLibrary.
 ** Programando mesmo após a morte.
\**************************************************************************************/
#define _WIN32_WINNT 0x0500
#define WINVER       0x0500

#include <windows.h>
#include "FreeImage.h"
#include "main.h"

/**********************----------------------------------------
** Função que converte de FIBITMAP* para HBITMAP.
*********/
HBITMAP FiBitmapToWin32Bitmap(GAME_INF *pGameInf,FIBITMAP* src)
{
    HBITMAP hbm = CreateDIBitmap(pGameInf->hDC_Screen,
                                 FreeImage_GetInfoHeader(src),
                                 CBM_INIT,
                                 FreeImage_GetBits(src),
                                 FreeImage_GetInfo(src),
                                 DIB_RGB_COLORS);
    return hbm;
}

/**********************----------------------------------------
** Função que carrega uma imagem png e retorna um identificador HBITMAP.
*********/
HBITMAP LoadPNG( GAME_INF *pGameInf,char *szNameFile )
{
    FIBITMAP *pFiBitmap = NULL;
    HBITMAP hBitmap     = NULL;
    pFiBitmap = FreeImage_Load(FIF_PNG,szNameFile, PNG_DEFAULT);
    hBitmap = FiBitmapToWin32Bitmap(pGameInf,pFiBitmap);
    FreeImage_Unload(pFiBitmap);
    return hBitmap;
}


/**********************----------------------------------------
** Função que carrega uma imagem png e retorna um identificador HBITMAP.
*********/
HBITMAP LoadPNG_Ex(GAME_INF *pGameInf,char *szNameFile)
{
    HBITMAP hBitmap     = NULL;
    FIBITMAP *pFiBitmap = FreeImage_Load(FIF_PNG,szNameFile, PNG_DEFAULT);
    RGBQUAD appColor = { 0, 0, 0, 255 };
    FIBITMAP *display_dib_c = FreeImage_Composite(pFiBitmap, TRUE, &appColor,NULL);
    hBitmap = FiBitmapToWin32Bitmap(pGameInf,display_dib_c);
    FreeImage_Unload(pFiBitmap);
    FreeImage_Unload(display_dib_c);
    return hBitmap;
}


/**********************----------------------------------------
** Função que carrega todas as imagens do jogo.
*********/
void LoadResources(GAME_INF *pGameInf)
{
    pGameInf->hBitmapMenuBack    = LoadPNG(pGameInf,"gfx\\menu_back.png"       );
    pGameInf->hBitmapGameBack    = LoadPNG(pGameInf,"gfx\\game_back.png"       );
    pGameInf->hBitmapMenuNewGame = LoadPNG_Ex(pGameInf,"gfx\\BttnNewGame.png"  );
    pGameInf->hBitmapMenuQuit    = LoadPNG_Ex(pGameInf,"gfx\\BttnQuit.png"     );
    pGameInf->hBitmapMenuSelect  = LoadPNG(pGameInf,"gfx\\select.png"          );
    pGameInf->hBitmapSnakeHead   = LoadPNG(pGameInf,"gfx\\head.png"            );
    pGameInf->hBitmapSnakeBody   = LoadPNG(pGameInf,"gfx\\body.png"            );
    pGameInf->hBitmapSnakeTail   = LoadPNG(pGameInf,"gfx\\tail.png"            );
    pGameInf->hBitmapFood        = LoadPNG(pGameInf,"gfx\\drop.png"            );
    pGameInf->hBitmapBigFood     = LoadPNG(pGameInf,"gfx\\game_drop.png");
    pGameInf->hBitmapNumbers     = LoadPNG_Ex(pGameInf,"gfx\\numbers.png"      );
}


/**********************----------------------------------------
** Função que descarrega todas as imagens do jogo.
*********/
void UnloadResources(GAME_INF *pGameInf)
{
    DeleteObject(pGameInf->hBitmapMenuBack   );
    DeleteObject(pGameInf->hBitmapGameBack   );
    DeleteObject(pGameInf->hBitmapMenuNewGame);
    DeleteObject(pGameInf->hBitmapMenuQuit   );
    DeleteObject(pGameInf->hBitmapMenuSelect );
    DeleteObject(pGameInf->hBitmapSnakeHead  );
    DeleteObject(pGameInf->hBitmapSnakeBody  );
    DeleteObject(pGameInf->hBitmapSnakeTail  );
    DeleteObject(pGameInf->hBitmapFood       );
    DeleteObject(pGameInf->hBitmapBigFood    );
    DeleteObject(pGameInf->hBitmapNumbers    );
}
