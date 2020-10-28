#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

HBITMAP FiBitmapToWin32Bitmap(GAME_INF *pGameInf,FIBITMAP* src);
HBITMAP LoadPNG( GAME_INF *pGameInf,char *szNameFile );
void LoadResources(GAME_INF *pGameInf);
void UnloadResources(GAME_INF *pGameInf);


#endif // INIT_H_INCLUDED
