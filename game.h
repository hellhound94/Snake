/**************************************************************************************\
 *** Snake 1.3
 *** Desenvolvedor: Abner Cordeiro ( Hellhound )
 *** 2014 - Todos os Direitos reservados.
\**************************************************************************************/
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#define POINTS_STR "+%d Pontos"

#define PLUS_POINTS 10
void Retangulo(HDC hdc,int iPosX,int iPosY,int iTamX,int iTamY);
void GeraMaca(GAME_INF *pGameInf,long *piFoodX,long *piFoodY);
void GameUpdate(GAME_INF *pGameInf);

void ShowMenu(GAME_INF *pGameInf);
void ShowGame(GAME_INF *pGameInf);
void ShowPointsText(GAME_INF *pGameInf);

void ShowTotalPoints(GAME_INF *pGameInf,int iPosX,int iPosY);

#endif // GAME_H_INCLUDED
