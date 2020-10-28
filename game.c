/**************************************************************************************\
 *** Snake 1.3
 *** Desenvolvedor: Abner Cordeiro ( Hellhound )
 *** 2014 - Todos os Direitos reservados.
 ** Arquivo: game.c
 ** Contém o principal do jogo,logica e plotagem.
 ** Programando mesmo após a morte.
\**************************************************************************************/
#define _WIN32_WINNT 0x0500
#define WINVER       0x0500
#include <windows.h>
#include <stdio.h>
#include "FreeImage.h"
#include "main.h"
#include "resource.h"
#include "game.h"

char g_szVer [] = "Snake Versão 1.3 - Desenvolvido por Hellhound.";
char g_szText[] = "Paixão por Softwares - 2014.";


#define GAME_POS_X  5
#define GAME_POS_Y 10

#define TILE_WIDTH  20
#define TILE_HEIGHT 20
/**********************----------------------------------------
** Pinta um retangulo sem BRUSH ( TRANSPARENTE )
*********/
void Retangulo(HDC hdc,int iPosX,int iPosY,int iTamX,int iTamY)
{
    MoveToEx(hdc, (int) iPosX, (int) iPosY, (LPPOINT) NULL);
    LineTo(hdc, (int) iPosX + iTamX, (int) iPosY);

    MoveToEx(hdc, (int) iPosX, (int) iPosY + iTamY, (LPPOINT) NULL);
    LineTo(hdc, (int) iPosX + iTamX, (int) iPosY + iTamY);
    //
    MoveToEx(hdc, (int) iPosX, (int) iPosY, (LPPOINT) NULL);
    LineTo(hdc, (int) iPosX, (int) iPosY + iTamY);

    MoveToEx(hdc, (int) iPosX  + iTamX, (int) iPosY, (LPPOINT) NULL);
    LineTo(hdc, (int) iPosX  + iTamX, (int) iPosY + iTamY);
}

/**********************----------------------------------------
** Mostra o MENU principal
*********/
void ShowMenu(GAME_INF *pGameInf)
{
    BITMAP bmp1;
    BITMAP bmp2;

    GetObject(pGameInf->hBitmapMenuBack, sizeof(BITMAP), (LPVOID) &bmp1 );
    BlendStretch(pGameInf->hDC_Screen,pGameInf->hBitmapMenuBack,0,0,pGameInf->rc.right,pGameInf->rc.bottom,0,0,bmp1.bmWidth,bmp1.bmHeight);

    int iMenuSelectX_pos;
    int iMenuSelectY_pos;

    if(!pGameInf->iMenuSelected)
    {
        iMenuSelectX_pos = 35;
        iMenuSelectY_pos = 340;
    }
    else
    {
        iMenuSelectX_pos = 35;
        iMenuSelectY_pos = 390;
    }

    GetObject(pGameInf->hBitmapMenuSelect, sizeof(BITMAP), (LPVOID) &bmp1 );
    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapMenuSelect,255,iMenuSelectX_pos,iMenuSelectY_pos,
               bmp1.bmWidth - 70,bmp1.bmHeight - 70 ,0,0,
               bmp1.bmWidth ,bmp1.bmHeight);

    GetObject(pGameInf->hBitmapMenuNewGame, sizeof(BITMAP), (LPVOID) &bmp1 );
    GetObject(pGameInf->hBitmapMenuQuit, sizeof(BITMAP), (LPVOID) &bmp2 );

    int iAntes = SetStretchBltMode(pGameInf->hDC_Screen, COLORONCOLOR);

    bmp1.bmWidth -= 60;
    bmp2.bmWidth -= 60;
    if(!pGameInf->iMenuSelected)
    {
        BlendTransparent(pGameInf->hDC_Screen,
                         pGameInf->hBitmapMenuNewGame,
                         ((pGameInf->rc.right - bmp1.bmWidth ) /2),350,
                         bmp1.bmWidth ,(bmp1.bmHeight / 2) - 30 ,
                         0,bmp1.bmHeight / 2,
                         bmp1.bmWidth+60,bmp1.bmHeight / 2,
                         RGB(0,0,0));

        BlendTransparent(pGameInf->hDC_Screen,
                         pGameInf->hBitmapMenuQuit,
                         (pGameInf->rc.right - bmp2.bmWidth ) /2,400,
                         bmp2.bmWidth,(bmp2.bmHeight / 2) - 30 ,
                         0,0,
                         bmp2.bmWidth+60,bmp2.bmHeight / 2,
                         RGB(0,0,0));
    }

    else
    {
        BlendTransparent(pGameInf->hDC_Screen,
                         pGameInf->hBitmapMenuNewGame,
                         ((pGameInf->rc.right - bmp1.bmWidth ) /2),350,
                         bmp1.bmWidth ,(bmp1.bmHeight / 2) - 30 ,
                         0,0,
                         bmp1.bmWidth+60,bmp1.bmHeight / 2,
                         RGB(0,0,0));

        BlendTransparent(pGameInf->hDC_Screen,
                         pGameInf->hBitmapMenuQuit,
                         (pGameInf->rc.right - bmp2.bmWidth ) /2,400,
                         bmp2.bmWidth,(bmp2.bmHeight / 2) - 30 ,
                         0,bmp2.bmHeight / 2,
                         bmp2.bmWidth+60,bmp2.bmHeight / 2,
                         RGB(0,0,0));
    }

    SetStretchBltMode(pGameInf->hDC_Screen, iAntes);

    SetTextColor(pGameInf->hDC_Screen,RGB(0,0,0));
    SetBkMode(pGameInf->hDC_Screen, TRANSPARENT);

    HGDIOBJ hGDIObject = SelectObject(pGameInf->hDC_Screen,pGameInf->hFonte1);
    SIZE Size;
    GetTextExtentPoint32(pGameInf->hDC_Screen,g_szVer,strlen(g_szVer),&Size);
    TextOut(pGameInf->hDC_Screen,(pGameInf->rc.right - Size.cx) / 2,280,g_szVer,strlen(g_szVer));
    GetTextExtentPoint32(pGameInf->hDC_Screen,g_szText,strlen(g_szText),&Size);
    TextOut(pGameInf->hDC_Screen,(pGameInf->rc.right - Size.cx) / 2,300,g_szText,strlen(g_szText));

    SelectObject(pGameInf->hDC_Screen,hGDIObject);
}

/**********************----------------------------------------
** Gera 2 numeros aleatórios,vê se o snake nao ocupa esta posição
** se sim,ele chama a si proprio para gerar outros numeros,senão
** ele deixa o numero gerado.para a comida
*********/
void GeraMaca(GAME_INF *pGameInf,long *piFoodX,long *piFoodY)
{
    int x;

    *piFoodX = (rand() % (TOTAL_TILES_LINHAS  - 1) );
    *piFoodY = (rand() % (TOTAL_TILES_COLUNAS - 1) );

    for(x = 0; x <= pGameInf->iNumSegmentos; x++)
    {
        if((pGameInf->Snake[x].iLinha  == *piFoodY) &&
                (pGameInf->Snake[x].iColuna == *piFoodX))
        {
            GeraMaca(pGameInf,piFoodX,piFoodY);
            break;
        }
    }
}

/**********************----------------------------------------
** Função que atualiza todas as informações sobre o jogo
** chamar ela mais vezes deixa o jogo mais rapido.
*********/
void GameUpdate(GAME_INF *pGameInf)
{
    register int x;
    register int y;

    /**********************----------------------------------------
    ** Zera a Matriz bidimensional com as posições das imagens
    *********/
    for(x = 0; x < TOTAL_TILES_LINHAS; x++)
        for(y = 0; y < TOTAL_TILES_COLUNAS; y++)
            pGameInf->iMatriz1[x][y] = 0;

    /**********************----------------------------------------
    ** Copia as posições do segmento da frente para o de traz
    ** e atualiza a posição da cabeça
    *********/
    for(x = pGameInf->iNumSegmentos,y = (pGameInf->iNumSegmentos - 1); x > 0; x--,y--)
    {
        //             10                             9
        //              9                             8
        //              1                             0
        pGameInf->Snake[x].iLinha   = pGameInf->Snake[y].iLinha;
        pGameInf->Snake[x].iColuna  = pGameInf->Snake[y].iColuna;
        pGameInf->Snake[x].iDirecao = pGameInf->Snake[y].iDirecao;
        pGameInf->Snake[x].iVertice = pGameInf->Snake[y].iVertice;

        // certifica de que no ultimo elemento sempre
        // seja o RABO da cobra,mesmo erdando
        // do elemento anterior como sendo CORPO
        if(x == (pGameInf->iNumSegmentos - 1))
        {
            pGameInf->Snake[x].iVertice  = 0;
            pGameInf->Snake[x].iDirecao = pGameInf->Snake[x - 2].iDirecao;
        }

        if(y != 0) pGameInf->Snake[x].iTipoPart = pGameInf->Snake[y].iTipoPart;

        if( (pGameInf->Snake[y].iColuna >= 0 && pGameInf->Snake[y].iColuna < TOTAL_TILES_COLUNAS) &&
                (pGameInf->Snake[y].iLinha  >= 0 && pGameInf->Snake[y].iLinha  < TOTAL_TILES_LINHAS) )
        {
            // setamos o fim do array como sendo SEMPRE o rabo.
            if(y == (pGameInf->iNumSegmentos - 1))
            {
                if(pGameInf->Snake[y].iDirecao == CIMA    ) pGameInf->Snake[y].iTipoPart = RABO_1;
                if(pGameInf->Snake[y].iDirecao == BAIXO   ) pGameInf->Snake[y].iTipoPart = RABO_3;
                if(pGameInf->Snake[y].iDirecao == DIREITO ) pGameInf->Snake[y].iTipoPart = RABO_2;
                if(pGameInf->Snake[y].iDirecao == ESQUERDO) pGameInf->Snake[y].iTipoPart = RABO_4;
            }
            // copiamos a info do array Snake[] para a Matriz1[][]
            pGameInf->iMatriz1[pGameInf->Snake[y].iColuna][pGameInf->Snake[y].iLinha] = pGameInf->Snake[y].iTipoPart;

            // se for alguma parte do snake quando ele vira,então setamos na Matriz1 tambem.
            if(pGameInf->Snake[y].iVertice > 0)
            {
                pGameInf->iMatriz1[pGameInf->Snake[y].iColuna][pGameInf->Snake[y].iLinha] = pGameInf->Snake[y].iVertice;
            }
        }
    }
    /**********************----------------------------------------
    ** atualiza a posição da cabeça
    *********/

    switch(pGameInf->iDirecaoCorrent)
    {
    case BAIXO   :
    {
        pGameInf->Snake[1].iLinha  = pGameInf->Snake[0].iLinha++;
    }
    break;
    case CIMA    :
    {
        pGameInf->Snake[1].iLinha  = pGameInf->Snake[0].iLinha--;
    }
    break;
    case DIREITO :
    {
        pGameInf->Snake[1].iColuna = pGameInf->Snake[0].iColuna++;
    }
    break;
    case ESQUERDO:
    {
        pGameInf->Snake[1].iColuna = pGameInf->Snake[0].iColuna--;
    }
    break;
    }

    /**********************----------------------------------------
    ** Transporta o segmento atual para o outro lado do jogo,se nescessário.
    *********/
    if(pGameInf->Snake[0].iLinha  > TOTAL_TILES_LINHAS  )  pGameInf->Snake[0].iLinha  = 0;
    if(pGameInf->Snake[0].iLinha  <       0             )   pGameInf->Snake[0].iLinha  = TOTAL_TILES_LINHAS - 1 ;
    if(pGameInf->Snake[0].iColuna <       0             )   pGameInf->Snake[0].iColuna = TOTAL_TILES_COLUNAS - 1;
    if(pGameInf->Snake[0].iColuna >= TOTAL_TILES_COLUNAS) pGameInf->Snake[0].iColuna = 0;
    // previne que o usuario aperte ao mesmo tempo duas teclas,
    //o que iria confundir o jogo e fazer ele colidir com o proprio corpo...
    if(pGameInf->bPegou == FALSE) pGameInf->bPegou = TRUE;

    /**********************----------------------------------------
    ** verifica a colisão da cabeça com a comida
    *********/
    if(((pGameInf->Snake[0].iLinha == pGameInf->Food.y ) && (pGameInf->Snake[0].iColuna == pGameInf->Food.x)) ||
            ((pGameInf->Snake[1].iLinha == pGameInf->Food.y ) && (pGameInf->Snake[1].iColuna == pGameInf->Food.x)))
    {
        // Posiciona o texto informando quantos pontos
        // o jogador ganhou.
        pGameInf->iTempoText = 50;
        pGameInf->iTextX = (pGameInf->Snake[1].iColuna * TILE_WIDTH ) + GAME_POS_X;
        pGameInf->iTextY = (pGameInf->Snake[1].iLinha  * TILE_HEIGHT) + GAME_POS_Y;

        pGameInf->iTextY += 15;
        if(pGameInf->Snake[1].iColuna < (TOTAL_TILES_COLUNAS - 4) )
        {
            if(pGameInf->iDirecaoCorrent == CIMA    || pGameInf->iDirecaoCorrent == BAIXO   ) pGameInf->iTextX += 25;
            if(pGameInf->iDirecaoCorrent == DIREITO || pGameInf->iDirecaoCorrent == ESQUERDO) pGameInf->iTextY += 25;
        }
        else
        {
            if(pGameInf->iDirecaoCorrent == CIMA    || pGameInf->iDirecaoCorrent == BAIXO   ) pGameInf->iTextX -= 70;
            if(pGameInf->iDirecaoCorrent == DIREITO || pGameInf->iDirecaoCorrent == ESQUERDO) pGameInf->iTextX -= 70;
        }
        if(pGameInf->Snake[1].iLinha > (TOTAL_TILES_LINHAS - 4) )
        {
            pGameInf->iTextY -= 40;
        }

        // como ele comeu a maçã atual,então gera outra em outras posições.
        GeraMaca(pGameInf,&pGameInf->Food.x,&pGameInf->Food.y );

        if(pGameInf->iCountBigFood == 2)
        {
            GeraMaca(pGameInf,&pGameInf->BigFood[0].x,&pGameInf->BigFood[0].y );
            pGameInf->iCountBigFood = 0;
        }
        else
        {
            pGameInf->BigFood[0].x = 0;
            pGameInf->BigFood[0].y = 0;
        }
        /**********************----------------------------------------
        ** para aumentar o corpo dele,como no final ele tem um rabo
        ** eu copio as info do rabo dele para pGameInf->iNumSegmentos + 1,
        ** e em pGameInf->iNumSegmentos eu coloco um novo segmento do corpo.
        *********/


        pGameInf->Snake[pGameInf->iNumSegmentos + 1].iLinha   = pGameInf->Snake[pGameInf->iNumSegmentos].iLinha;
        pGameInf->Snake[pGameInf->iNumSegmentos + 1].iColuna  = pGameInf->Snake[pGameInf->iNumSegmentos].iColuna;
        pGameInf->Snake[pGameInf->iNumSegmentos + 1].iDirecao = pGameInf->Snake[pGameInf->iNumSegmentos].iDirecao;
        pGameInf->Snake[pGameInf->iNumSegmentos + 1].iVertice = pGameInf->Snake[pGameInf->iNumSegmentos].iVertice;

        pGameInf->Snake[pGameInf->iNumSegmentos].iTipoPart = pGameInf->Snake[pGameInf->iNumSegmentos - 1].iTipoPart;
        pGameInf->Snake[pGameInf->iNumSegmentos].iVertice = 0;
        /* CABEÇA CORPO -> CORPO -> CORPO -> RABO -> AAAA -> AAAA -> AAAA
        copia as info do rabo para NumSegmentos + 3
        */
        pGameInf->iNumTotalPoints += PLUS_POINTS;
        pGameInf->iNumSegmentos += 1;
        pGameInf->iCountBigFood ++;
    }

    /**********************----------------------------------------
    ** verifica a colisão da cabeça com o resto do corpo.
    *********/
    register int iSegmts;
    for(iSegmts = 2; iSegmts <= pGameInf->iNumSegmentos; iSegmts++)
    {
        if( ((pGameInf->Snake[1].iLinha  == pGameInf->Snake[iSegmts].iLinha) &&
             (pGameInf->Snake[1].iColuna == pGameInf->Snake[iSegmts].iColuna)))
        {
            pGameInf->iGameStatus = GAME_GAMEOVER;
            pGameInf->iTempoText = 0;
            if(MessageBox(pGameInf->hwnd,"Você colidiu com o corpo da cobra!!!\r\nNovo Jogo???","GameOver...",MB_YESNO ) == IDYES)
            {/*
                if( pGameInf->iNumTotalPoints > (pGameInf->pRecords[0]).iRecord )
                {
                    ShowDialogAddRecord(pGameInf);

                    (pGameInf->pRecords[0]).iRecord = pGameInf->iNumTotalPoints;

                    //strcpy((pGameInf->pRecords[0]).szNome,pGameInf-);
                    SaveRecord(pGameInf);

                    MessageBox(NULL,"Entrou para a lista de Records!!!","",MB_OK);
                }*/

                NewGame(pGameInf);
                pGameInf->iGameStatus = GAME_RUNNING;
            }
        }
    }

    /**********************----------------------------------------
    ** um debugzinho
    *********/

    char szDebug[MAX_PATH];
    sprintf(szDebug,
            "linha:%d coluna:%d Segmentos:%d Alimentos:%d",
            pGameInf->Snake[0].iLinha,
            pGameInf->Snake[0].iColuna,
            pGameInf->iNumSegmentos,
            pGameInf->iNumTotalPoints );
    SetWindowText(pGameInf->hwnd,szDebug );

}


void ShowGame(GAME_INF *pGameInf)
{
    BITMAP bmp;
    // register para tentar utilizar algum eax da vida...
    register int x;
    register int y;
    // static para permanecer o valor mesmo ao termino da função.
    static int iTime1 = 0;
    /**********************----------------------------------------
    ** Determina a velocidade do jogo
    ** mude aonde compara iTime1 para mudar velocidade do Snake.
    *********/

    iTime1++;
    if((pGameInf->iGameStatus != GAME_PAUSED) || (pGameInf->iGameStatus != GAME_GAMEOVER))
    {
        if(iTime1 >= pGameInf->iSnakeVelocit )
        {
            GameUpdate(pGameInf);
            iTime1 = 0;
        }
    }
    /**********************----------------------------------------
    ** Como a grama ficará em baixo ela é pintada primeiro
    *********/
    GetObject(pGameInf->hBitmapGameBack, sizeof(BITMAP), (LPVOID) &bmp );
    BlendStretch(pGameInf->hDC_Screen,pGameInf->hBitmapGameBack,0,0,pGameInf->rc.right,pGameInf->rc.bottom,0,0,bmp.bmWidth,bmp.bmHeight);

    // PINTA A COMIDA
    GetObject(pGameInf->hBitmapFood, sizeof(BITMAP), (LPVOID) &bmp );

    BlendTransparent(pGameInf->hDC_Screen,
                     pGameInf->hBitmapFood,
                     (pGameInf->Food.x * TILE_WIDTH ) + GAME_POS_X,
                     (pGameInf->Food.y * TILE_HEIGHT) + GAME_POS_Y,
                     bmp.bmWidth,bmp.bmHeight,
                     0,0,
                     bmp.bmWidth,bmp.bmHeight,RGB(0,0,0));

    /*
        if((pGameInf->BigFood[0].x > 0) && (pGameInf->BigFood[0].y > 0) )
        {
            ZeroMemory(&bmp,sizeof(BITMAP));

            GetObject(pGameInf->hBitmapBigFood, sizeof(BITMAP), (LPVOID) &bmp );

            //BlendTransparent
            BlendTransparent(pGameInf->hDC_Screen,
                             pGameInf->hBitmapBigFood,
                             (pGameInf->BigFood[0].x * TILE_WIDTH ) + GAME_POS_X,
                             (pGameInf->BigFood[0].y * TILE_HEIGHT) + GAME_POS_Y,
                             bmp.bmWidth,bmp.bmHeight,
                             0,0,
                             bmp.bmWidth,bmp.bmHeight,RGB(0,0,0));
        }
    */

    /**********************----------------------------------------
    ** Procura pela matriz aonde está marcado que existe imagem ( da cobra )
    ** primeiro pinta o corpo e o rabo..
    *********/
    for(x = 0; x  < TOTAL_TILES_LINHAS; x++)
    {
        for(y = 0; y < TOTAL_TILES_COLUNAS; y++)
        {
            if(pGameInf->iMatriz1[x][y] > 0)
            {
                register int iTempx = (x * TILE_WIDTH ) + GAME_POS_X;
                register int iTempy = (y * TILE_HEIGHT) + GAME_POS_Y;

                GetObject(pGameInf->hBitmapSnakeBody, sizeof(BITMAP), (LPVOID) &bmp );
                if(pGameInf->iMatriz1[x][y] == CORPO_1)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeBody,255,
                               iTempx,iTempy,
                               TILE_WIDTH,TILE_HEIGHT,
                               0,bmp.bmHeight / 3,
                               TILE_WIDTH,TILE_HEIGHT);
                }

                else if(pGameInf->iMatriz1[x][y] == CORPO_2)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeBody,255,
                               iTempx,iTempy,
                               TILE_WIDTH,TILE_HEIGHT,
                               bmp.bmWidth / 2,bmp.bmHeight / 3,
                               TILE_WIDTH,TILE_HEIGHT);
                }

                //*************************************************************************************
                GetObject(pGameInf->hBitmapSnakeTail, sizeof(BITMAP), (LPVOID) &bmp );
                if(pGameInf->iMatriz1[x][y] == RABO_1)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeTail,255,
                               iTempx ,iTempy,
                               bmp.bmWidth / 2,bmp.bmHeight / 2 ,
                               0,0,
                               TILE_WIDTH,TILE_HEIGHT);
                }

                else if(pGameInf->iMatriz1[x][y] == RABO_2)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeTail,255,
                               iTempx ,iTempy,
                               bmp.bmWidth / 2,bmp.bmHeight / 2 ,
                               bmp.bmWidth / 2,0,
                               TILE_WIDTH,TILE_HEIGHT);
                }
                else if(pGameInf->iMatriz1[x][y] == RABO_3)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeTail,255,
                               iTempx ,iTempy,
                               bmp.bmWidth / 2,bmp.bmHeight / 2 ,
                               0,bmp.bmHeight / 2,
                               TILE_WIDTH,TILE_HEIGHT);
                }
                else if(pGameInf->iMatriz1[x][y] == RABO_4)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeTail,255,
                               iTempx ,iTempy,
                               bmp.bmWidth / 2,bmp.bmHeight / 2 ,
                               bmp.bmWidth / 2,bmp.bmHeight / 2,
                               TILE_WIDTH,TILE_HEIGHT);
                }
                //**********************************************************************************
                GetObject(pGameInf->hBitmapSnakeBody, sizeof(BITMAP), (LPVOID) &bmp );
                if(pGameInf->iMatriz1[x][y] == VERTICE_1)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeBody,255,
                               iTempx ,iTempy,
                               bmp.bmWidth / 2,bmp.bmHeight / 3 ,
                               0,0,
                               TILE_WIDTH,TILE_HEIGHT);
                }
                else if(pGameInf->iMatriz1[x][y] == VERTICE_2)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeBody,255,
                               iTempx ,iTempy,
                               bmp.bmWidth / 2,bmp.bmHeight / 3 ,
                               bmp.bmWidth / 2,0,
                               TILE_WIDTH,TILE_HEIGHT);
                }
                else if(pGameInf->iMatriz1[x][y] == VERTICE_3)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeBody,255,
                               iTempx ,iTempy,
                               bmp.bmWidth / 2,bmp.bmHeight / 3 ,
                               0,(bmp.bmHeight / 3) * 2,
                               TILE_WIDTH,TILE_HEIGHT);
                }
                else if(pGameInf->iMatriz1[x][y] == VERTICE_4)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeBody,255,
                               iTempx ,iTempy,
                               bmp.bmWidth / 2,bmp.bmHeight / 3 ,
                               bmp.bmWidth / 2,(bmp.bmHeight / 3) * 2,
                               TILE_WIDTH,TILE_HEIGHT);
                }
            }

        }
    }

    /**********************----------------------------------------
    ** Agora procura pela matriz aonde está a cabeça da cobra
    ** então pinta,fiz isso para a imagem da cabeça ( POR SER MAIOR )
    ** ficar por cima ( SOBREPOR ) o resto do corpo e rabo.
    *********/
    for(x = 0; x  < TOTAL_TILES_LINHAS; x++)
    {
        for(y = 0; y < TOTAL_TILES_COLUNAS; y++)
        {
            if(pGameInf->iMatriz1[x][y] > 0)
            {
                register int iTempx = (x * TILE_WIDTH ) + GAME_POS_X;
                register int iTempy = (y * TILE_HEIGHT) + GAME_POS_Y;

                GetObject(pGameInf->hBitmapSnakeHead, sizeof(BITMAP), (LPVOID) &bmp );

                if(pGameInf->iMatriz1[x][y] == CABECA_1)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeHead,255,
                               iTempx - 10,iTempy - 20,
                               bmp.bmWidth / 4,bmp.bmHeight ,0,0,
                               bmp.bmWidth / 4,bmp.bmHeight);
                }
                else if(pGameInf->iMatriz1[x][y] == CABECA_2)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeHead,255,
                               iTempx,iTempy - 10,
                               bmp.bmWidth / 4,bmp.bmHeight ,bmp.bmWidth / 4,0,
                               bmp.bmWidth / 4,bmp.bmHeight);
                }

                else if(pGameInf->iMatriz1[x][y] == CABECA_3)
                {
                    BlendAlpha(pGameInf->hDC_Screen,pGameInf->hBitmapSnakeHead,255,
                               iTempx - 10,iTempy,
                               bmp.bmWidth / 4,bmp.bmHeight ,2 *(bmp.bmWidth / 4),0,
                               bmp.bmWidth / 4,bmp.bmHeight);
                }
                else if(pGameInf->iMatriz1[x][y] == CABECA_4)
                {
                    BlendAlpha(pGameInf->hDC_Screen,
                               pGameInf->hBitmapSnakeHead,255,
                               iTempx - 20,iTempy - 10,
                               bmp.bmWidth / 4,bmp.bmHeight ,3 *(bmp.bmWidth / 4),0,
                               bmp.bmWidth / 4,bmp.bmHeight);
                }
            }
        }
    }
    /*
            for(x = 0; x  < TOTAL_TILES_LINHAS; x++)
            {
                for(y = 0; y < TOTAL_TILES_COLUNAS; y++)
                {
                    Retangulo(pGameInf->hDC_Screen,GAME_POS_X,GAME_POS_Y,(y * TILE_WIDTH) + GAME_POS_X,(x * TILE_HEIGHT) + GAME_POS_Y);
                }
            }
    */
    if(pGameInf->iTempoText >= 0)
    {
        ShowPointsText(pGameInf);
    }

    ShowTotalPoints(pGameInf,340,10);

    Retangulo(pGameInf->hDC_Screen,GAME_POS_X,GAME_POS_Y,(TOTAL_TILES_COLUNAS * TILE_WIDTH),(TOTAL_TILES_LINHAS * TILE_HEIGHT));
}
void ShowPointsText(GAME_INF *pGameInf)
{
    char szBuff[100];
    HGDIOBJ hGDIObject = SelectObject(pGameInf->hDC_Screen,pGameInf->hFonte2);
    sprintf(szBuff,POINTS_STR,PLUS_POINTS);
    TextOut(pGameInf->hDC_Screen,pGameInf->iTextX,pGameInf->iTextY--,szBuff,strlen(szBuff));

    SelectObject(pGameInf->hDC_Screen,hGDIObject);
    pGameInf->iTempoText--;
}

void ShowTotalPoints(GAME_INF *pGameInf,int iPosX,int iPosY)
{
    BITMAP bmp;
    GetObject(pGameInf->hBitmapNumbers, sizeof(BITMAP), (LPVOID) &bmp );

    int iArrayNumX[10] = {0,(bmp.bmWidth / 11),((bmp.bmWidth / 11) * 2) ,((bmp.bmWidth / 11) * 3),((bmp.bmWidth / 11) * 4)};
    int iArrayNumY[10] = {iPosY,iPosY,iPosY,iPosY,iPosY};
    int iTotalDigits = 0,iCount = 0;
    char szPoints[MAX_PATH],szBuff[10];
    sprintf(szPoints,"%.5d",pGameInf->iNumTotalPoints);
    iTotalDigits = strlen(szPoints);

    while(iCount < iTotalDigits)
    {
        szBuff[0] = szPoints[iCount];
        szBuff[1] = '\0';
        int iNum = atoi(szBuff);

        BlendTransparent(pGameInf->hDC_Screen,
                         pGameInf->hBitmapNumbers,
                         iArrayNumX[iCount] + iPosX,iArrayNumY[iCount],
                         (bmp.bmWidth / 11),bmp.bmHeight,
                         (bmp.bmWidth / 11) * iNum,0,
                         (bmp.bmWidth / 11),bmp.bmHeight,
                         RGB(0,0,0));

        iCount++;
    }
}
