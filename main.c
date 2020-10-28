/**************************************************************************************\
 *** Snake 1.3
 *** Desenvolvedor: Abner Cordeiro ( Hellhound )
 *** 2014 - Todos os Direitos reservados.
 ** Arquivo: main.c
 ** data: 21/09/2014 - Inicio do projeto
 **       22/09/2014 - continuação
 **       23/09/2014 - continuação
 **       24/09/2014 - termino.
 ** Contém o principal do jogo,carrega a biblioteca FreeLibrary,trata mensagens do sistema
 ** e chama funções dos outros arquivos.
 ** Programando mesmo após a morte.
\**************************************************************************************/
// TODO (abner#1#09/25/14): Implementar comida bonus na proxima versão.
// TODO (abner#1#09/25/14): Implementar sistema de pontos original do jogo Snake
// TODO (abner#1#09/25/14): Implementar quantos pontos o jogador ganhou ao comer a comida Bonus ou normal

#define _WIN32_WINNT 0x0500
#define WINVER       0x0500

#include <windows.h >
#include <Windowsx.h>
#include <commctrl.h>
#include <stdio.h   >
#include <time.h    >
#include "FreeImage.h"
#include "main.h"
#include "game.h"
#include "init.h"
#include "resource.h"

GAME_INF GameInf;

/**********************----------------------------------------
** Retorna a tecla oposta,ex: baixo = cima,dã...
*********/
int Oposicao(WPARAM d)
{
    switch(d)
    {
    case VK_LEFT :
        return DIREITO;
        break;// break só pra ficar bunitim
    case VK_RIGHT:
        return ESQUERDO;
        break;
    case VK_UP   :
        return BAIXO;
        break;
    case VK_DOWN :
        return CIMA;
        break;
    default:
        return 0;
    }
}

/**********************----------------------------------------
** Função que pinta uma imagem aumentada ou diminuida
*********/
void BlendStretch(HDC hDC_Dest,
                  HBITMAP hBitmap,
                  int nXOriginDest,int nYOriginDest,
                  int nWidthDest  ,int nHeightDest,
                  int nXOriginSrc ,int nYOriginSrc,
                  int nWidthSrc   ,int nHeightSrc)
{
    HDC hBitmapDC   = CreateCompatibleDC   (hDC_Dest);

    SelectObject (hBitmapDC,hBitmap);

    int iAntes = SetStretchBltMode(hDC_Dest, STRETCH_HALFTONE);

    StretchBlt(hDC_Dest,
               nXOriginDest,nYOriginDest,
               nWidthDest,nHeightDest,
               hBitmapDC,
               nXOriginSrc,nYOriginSrc,
               nWidthSrc,nHeightSrc,
               SRCCOPY);

    SetStretchBltMode(hDC_Dest, iAntes);

    DeleteDC    (hBitmapDC);
}

/**********************----------------------------------------
** Função que pinta a imagem deixando  transparente.
*********/
void BlendAlpha(HDC hDC_Dest,
                HBITMAP hBitmap,
                BYTE ConstantAlpha,
                int xoriginDest,int yoriginDest,
                int wDest,int hDest,
                int xoriginSrc,int yoriginSrc,
                int wSrc,int hSrc)
{
    HDC hBitmapDC   = CreateCompatibleDC (hDC_Dest);

    SelectObject (hBitmapDC,hBitmap);

    BLENDFUNCTION BlendFunc;
    BlendFunc.BlendOp             = AC_SRC_OVER;
    BlendFunc.BlendFlags          = 0;
    BlendFunc.SourceConstantAlpha = ConstantAlpha;
    BlendFunc.AlphaFormat         = AC_SRC_ALPHA;

    AlphaBlend(hDC_Dest,
               xoriginDest,yoriginDest,
               wDest,hDest,
               hBitmapDC,
               xoriginSrc,yoriginSrc,
               wSrc,hSrc,
               BlendFunc);

    DeleteDC(hBitmapDC);
}

/**********************----------------------------------------
** Função que pinta a imagem deixando a cor crTransparent transparente.
*********/
void BlendTransparent(HDC hDC_Dest,
                      HBITMAP hBitmap,
                      int xoriginDest,int yoriginDest,
                      int wDest,int hDest,
                      int xoriginSrc,int yoriginSrc,
                      int wSrc,int hSrc,UINT crTransparent)
{
    HDC memDC = CreateCompatibleDC ( hDC_Dest );
    SelectObject(memDC,hBitmap);

    TransparentBlt(hDC_Dest,
                   xoriginDest,
                   yoriginDest,
                   wDest,
                   hDest,memDC,
                   xoriginSrc,yoriginSrc,
                   wSrc,hSrc,crTransparent);

    DeleteDC(memDC);
}


/**********************----------------------------------------
** Zera as variaveis do jogo,e inicializa...
*********/
void NewGame(GAME_INF *pGameInf)
{
    int x = 0,y = 0;

    for(x = 0; x < TOTAL_TILES_LINHAS; x++)
        for(y = 0; y < TOTAL_TILES_COLUNAS; y++)
            pGameInf->iMatriz1[x][y] = 0;

    x = 0;
    y = 10;
    /**********************----------------------------------------
    ** Colocando a cabeça da cobra
    *********/
    pGameInf->iDirecaoCorrent = BAIXO;
    pGameInf->bPegou = TRUE;
    pGameInf->Snake[x].iLinha    = 11;
    pGameInf->Snake[x].iColuna   = 10;
    pGameInf->Snake[x].iTipoPart = CABECA_3;
    pGameInf->Snake[x].iDirecao  = BAIXO;
    x++;
    /**********************----------------------------------------
    ** Laço que coloca o corpo da cobra
    *********/
    while(x < 10 )
    {

        pGameInf->Snake[x].iLinha  = y;
        pGameInf->Snake[x].iColuna = 10;
        pGameInf->Snake[x].iTipoPart = CORPO_2;
        pGameInf->Snake[x].iDirecao  = BAIXO;
        y--;
        x++;
    }

    /**********************----------------------------------------
    ** Colocando o rabo da cobra
    *********/
    pGameInf->Snake[x].iLinha  = y;
    pGameInf->Snake[x].iColuna = 10;
    pGameInf->Snake[x].iTipoPart = RABO_1;
    pGameInf->Snake[x].iDirecao  = BAIXO;

    pGameInf->iNumSegmentos = x;

    pGameInf->iNumTotalPoints = 0;

    pGameInf->iSnakeVelocit = 6;

    pGameInf->iCountBigFood = 0;
    GeraMaca(pGameInf,&pGameInf->Food.x,&pGameInf->Food.y );
}

/**********************----------------------------------------
** Verifica o que é para mostrar e mostra!.
*********/
void showgame()
{

    switch(GameInf.iGameStatus)
    {
    case GAME_MENU:
        ShowMenu(&GameInf);
        break;
    case GAME_RUNNING:
        ShowGame(&GameInf);
        break;
    }
}

/**********************----------------------------------------
** É chamada quando o sistema têm algo interessante para nós...
*********/
LRESULT CALLBACK WindowProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    switch(uMsg)
    {
    /**********************----------------------------------------
    ** Ao chamar CreateWindow se sucesso ele manda a mensagem
    ** WM_CREATE para a proc registrada na classe criada,heheh
    *********/
    case WM_CREATE:
    {
        /**********************----------------------------------------
        ** Inicializa a biblioteca FreeImage,carrega imagens,converte,
        ** e desaloca recursos não mais utilizados...
        *********/
        FreeImage_Initialise(FALSE);

        GameInf.hwnd = hwnd;
        GetClientRect(hwnd,&GameInf.rc);

        NewGame(&GameInf);

        HDC hdc = GetDC(hwnd);

        GameInf.hDC_Screen       = CreateCompatibleDC ( hdc );
        GameInf.hBitmapScreen    = CreateCompatibleBitmap ( hdc, GameInf.rc.right, GameInf.rc.bottom );
        SelectObject ( GameInf.hDC_Screen, GameInf.hBitmapScreen );

        ReleaseDC(hwnd,hdc);

        LoadResources(&GameInf);

        GameInf.hFonte1 = CreateFont(17,7,0,0,FW_MEDIUM,
                                     FALSE,FALSE,FALSE,
                                     ANSI_CHARSET,
                                     OUT_DEFAULT_PRECIS,
                                     CLIP_DEFAULT_PRECIS,
                                     PROOF_QUALITY,
                                     DEFAULT_PITCH,
                                     "Comic Sans MS");


        GameInf.hFonte2 = CreateFont(18,8,0,0,FW_EXTRABOLD,
                                     FALSE,FALSE,FALSE,
                                     ANSI_CHARSET,
                                     OUT_DEFAULT_PRECIS,
                                     CLIP_DEFAULT_PRECIS,
                                     PROOF_QUALITY,
                                     DEFAULT_PITCH,
                                     "Comic Sans MS");

        SetTimer(hwnd,TIMER_GAME,2,NULL);

    }
    break;

    /**********************----------------------------------------
    ** Algum botão pressionado!!!!!!
    *********/
    case WM_KEYDOWN:
    {
        if(GameInf.iGameStatus == GAME_MENU)
        {
            if(wParam == VK_UP  ) GameInf.iMenuSelected = 0;
            if(wParam == VK_DOWN) GameInf.iMenuSelected = 1;
            if(wParam == VK_RETURN)
            {
                if(GameInf.iMenuSelected == 0) GameInf.iGameStatus = GAME_RUNNING;
                else SendMessage(hwnd,WM_CLOSE,0,0);
            }
        }
        else if(GameInf.iGameStatus != GAME_MENU)
        {
            /**********************----------------------------------------
            ** Usuario apertou botão F1 dentro do jogo ( PAUSA O JOGO )
            *********/
            if(wParam == VK_F1)
            {
                GameInf.iGameStatus = (GameInf.iGameStatus == GAME_PAUSED) ? GAME_RUNNING : GAME_PAUSED;
            }
            if((GameInf.iGameStatus != GAME_PAUSED) && (GameInf.bPegou == 1) )
            {
                /**********************----------------------------------------
                ** Usuario apertou botão esquerdo dentro do jogo
                *********/
                if(wParam == VK_LEFT && (GameInf.iDirecaoCorrent != Oposicao(wParam)) && (GameInf.iDirecaoCorrent != ESQUERDO) )
                {
                    GameInf.Snake[0].iLinha  = GameInf.Snake[1].iLinha;
                    GameInf.Snake[0].iColuna = GameInf.Snake[1].iColuna;
                    if(GameInf.iDirecaoCorrent == BAIXO)
                    {
                        GameInf.Snake[0].iColuna--;
                        GameInf.Snake[1].iVertice = VERTICE_1;
                    }
                    if(GameInf.iDirecaoCorrent == CIMA )
                    {
                        GameInf.Snake[0].iColuna--;
                        GameInf.Snake[1].iVertice = VERTICE_4;
                    }

                    GameInf.Snake[0].iDirecao  = ESQUERDO;
                    GameInf.Snake[0].iTipoPart = CABECA_4;
                    GameInf.Snake[1].iTipoPart = CORPO_1;
                    GameInf.iDirecaoCorrent    = ESQUERDO;
                    GameInf.bPegou = FALSE;
                }
                /**********************----------------------------------------
                ** Usuario apertou botão para cima dentro do jogo
                *********/
                if(wParam == VK_UP  && (GameInf.iDirecaoCorrent != Oposicao(wParam))&& (GameInf.iDirecaoCorrent != CIMA))
                {
                    GameInf.Snake[0].iLinha  = GameInf.Snake[1].iLinha;
                    GameInf.Snake[0].iColuna = GameInf.Snake[1].iColuna;

                    if(GameInf.iDirecaoCorrent == DIREITO )
                    {
                        GameInf.Snake[0].iLinha--;
                        GameInf.Snake[1].iVertice = VERTICE_1;
                    }
                    if(GameInf.iDirecaoCorrent == ESQUERDO)
                    {
                        GameInf.Snake[0].iLinha--;
                        GameInf.Snake[1].iVertice = VERTICE_2;
                    }

                    GameInf.Snake[0].iDirecao  = CIMA;
                    GameInf.Snake[0].iTipoPart = CABECA_1;
                    GameInf.Snake[1].iTipoPart = CORPO_2;
                    GameInf.iDirecaoCorrent    = CIMA;
                    GameInf.bPegou = FALSE;
                }
                /**********************----------------------------------------
                ** Usuario apertou botão direito dentro do jogo
                *********/
                if(wParam == VK_RIGHT && (GameInf.iDirecaoCorrent != Oposicao(wParam))&& (GameInf.iDirecaoCorrent != DIREITO))
                {

                    GameInf.Snake[0].iLinha  = GameInf.Snake[1].iLinha;
                    GameInf.Snake[0].iColuna = GameInf.Snake[1].iColuna;

                    if(GameInf.iDirecaoCorrent == BAIXO)
                    {
                        GameInf.Snake[0].iColuna++;
                        GameInf.Snake[1].iVertice = VERTICE_2;
                    }
                    if(GameInf.iDirecaoCorrent == CIMA )
                    {
                        GameInf.Snake[0].iColuna++;
                        GameInf.Snake[1].iVertice = VERTICE_3;
                    }
                    GameInf.Snake[0].iDirecao  = DIREITO;
                    GameInf.Snake[0].iTipoPart = CABECA_2;
                    GameInf.Snake[1].iTipoPart = CORPO_1;
                    GameInf.iDirecaoCorrent    = DIREITO;
                    GameInf.bPegou = FALSE;
                }
                /**********************----------------------------------------
                ** Usuario apertou botão para baixo dentro do jogo
                *********/
                if(wParam == VK_DOWN && (GameInf.iDirecaoCorrent != Oposicao(wParam))&& (GameInf.iDirecaoCorrent != BAIXO))
                {
                    GameInf.Snake[0].iLinha  = GameInf.Snake[1].iLinha;
                    GameInf.Snake[0].iColuna = GameInf.Snake[1].iColuna;

                    if(GameInf.iDirecaoCorrent == DIREITO )
                    {
                        GameInf.Snake[1].iVertice = VERTICE_4;
                        GameInf.Snake[0].iLinha++;
                    }
                    if(GameInf.iDirecaoCorrent == ESQUERDO)
                    {
                        GameInf.Snake[1].iVertice = VERTICE_3;
                        GameInf.Snake[0].iLinha++;
                    }

                    GameInf.Snake[0].iDirecao  = BAIXO;
                    GameInf.Snake[0].iTipoPart = CABECA_3;
                    GameInf.Snake[1].iTipoPart = CORPO_2;
                    GameInf.iDirecaoCorrent    = BAIXO;
                    GameInf.bPegou = FALSE;
                }
                if(GameInf.Snake[0].iLinha  >= TOTAL_TILES_LINHAS )GameInf.Snake[0].iLinha  = 0;
                if(GameInf.Snake[0].iColuna >= TOTAL_TILES_COLUNAS)GameInf.Snake[0].iColuna = 0;
            }
        }

    }
    break;
    /**********************----------------------------------------
    ** Timer usado para contar o tempo para pintar,e processar as
    ** informações do jogo.
    *********/
    case WM_TIMER:
    {
        if (wParam == TIMER_GAME)
        {
            HDC hdc = GetDC(hwnd);

            showgame( hwnd,GameInf.hDC_Screen );

            BitBlt(hdc,0,0,GameInf.rc.right, GameInf.rc.bottom,GameInf.hDC_Screen,0, 0,SRCCOPY);

            ReleaseDC(hwnd,hdc);
        }
    }
    break;

    /**********************----------------------------------------
    ** Solicitação de atualização da area cliente da Window.
    *********/
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hwnd,&ps);

        EndPaint(hwnd,&ps);
    }
    break;
    /**********************----------------------------------------
    ** Usuario quer fechar o jogo,desaloca tudo relacionado ao jogo.
    *********/
    case WM_CLOSE:
    {
        KillTimer   (hwnd, TIMER_GAME     );
        DeleteDC    (GameInf.hDC_Screen   );
        DeleteObject(GameInf.hBitmapScreen);
        DeleteObject(GameInf.hFonte1);
        DeleteObject(GameInf.hFonte2);
        free(GameInf.pRecords);
        UnloadResources(&GameInf);
        FreeImage_DeInitialise();
        DestroyWindow(hwnd);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd,uMsg,wParam,lParam);
    }
    return FALSE;
}



INT_PTR CALLBACK DialogAddRecordProc( HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    static HWND hRecord;
    static HWND hName;
    static GAME_INF *pGameInf;

    char szBuff[MAX_PATH];

    switch(uMsg)
    {
    case WM_INITDIALOG:
        {
            hRecord = GetDlgItem(hwnd,TEXT_RECORD);
            hName   = GetDlgItem(hwnd,EDIT_NAME  );

            pGameInf = (GAME_INF*)lParam;

            sprintf(szBuff,"%d",pGameInf->iNumTotalPoints);

            SetWindowText(hRecord,szBuff);
        }
        return TRUE;
        break;
    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
                case BUTTON_OK:
                    {
                        GetWindowText(hName,(pGameInf->pRecords[0]).szNome,MAX_PATH);

                        EndDialog(hwnd,0);
                    }
                break;
            }

        }
        return TRUE;
        break;

case WM_CLOSE:
    {
        EndDialog(hwnd,0);
    }
    break;
    }
    return FALSE;
}

BOOL ShowDialogAddRecord(GAME_INF *pGameInf)
{
    DialogBoxParam(pGameInf->hInst,MAKEINTRESOURCE(IDD_ADD_RECORD),pGameInf->hwnd,DialogAddRecordProc,(LPARAM)pGameInf);
    //FIX
    return TRUE;
}


PRECORDS LoadRecord(GAME_INF *pGameInf)
{
    pGameInf->pRecords = (PRECORDS)malloc( (sizeof(RECORDS)* 6) );
    FILE *pFile = fopen("snake.txt","rb+" );

    if(pFile == NULL)
    {
        pFile = fopen("snake.txt","wb+" );

        ZeroMemory(pGameInf->pRecords, (sizeof(RECORDS)* 6));

        (pGameInf->pRecords[0]).iRecord = (pGameInf->pRecords[1]).iRecord = (pGameInf->pRecords[3]).iRecord = (pGameInf->pRecords[4]).iRecord = (pGameInf->pRecords[5]).iRecord = 0;

        fwrite( pGameInf->pRecords, sizeof(RECORDS),6, pFile );
    }
    fread( pGameInf->pRecords,sizeof(RECORDS),6,pFile );

    fclose(pFile);

    return pGameInf->pRecords;
}


PRECORDS SaveRecord(GAME_INF *pGameInf)
{
    FILE *pFile = fopen("snake.txt","rb+" );

    if(pFile == NULL)
    {
        pFile = fopen("snake.txt","wb+" );
    }
    fwrite( pGameInf->pRecords, sizeof(RECORDS),6, pFile );

    fclose(pFile);

    return pGameInf->pRecords;
}
/**********************----------------------------------------
** Inicio...do pesadelo.
*********/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    //PRECORDS pRecords =
    //LoadRecord(&GameInf);
    // salvamos isso,vai que precisa neh...
    GameInf.hInst = hInstance;
    // isso tava por padrão caso fosse programar GUI,mas de boa.
    InitCommonControls();

    WNDCLASSEX wincl;
    MSG      msg;
    HWND     hwnd;

    wincl.cbSize      = sizeof (WNDCLASSEX);
    wincl.style       = CS_DBLCLKS;
    wincl.lpfnWndProc = WindowProc;
    wincl.cbClsExtra  = 0;
    wincl.cbWndExtra  = 0;
    wincl.hInstance   = hInstance;
    wincl.hIcon       = LoadIcon     (hInstance, MAKEINTRESOURCE(IDI_ICON1  ));
    wincl.hCursor       = LoadCursor (hInstance, MAKEINTRESOURCE(IDI_CURSOR1));
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    wincl.lpszMenuName  = NULL;
    wincl.lpszClassName = WINCLASS_STR;
    wincl.hIconSm       = LoadIcon (hInstance, MAKEINTRESOURCE(IDI_ICON1));

    /**********************----------------------------------------
    ** Registra a Classe
    *********/

    if(!RegisterClassEx (&wincl)) return 0;

    // inicializa a parada aleatória manolo.
    srand(time(NULL));

    AddFontResource("comicbd.ttf");

    hwnd = CreateWindow(WINCLASS_STR,WINTITLE,
                        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
                        (GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH_DEBUG) / 2,
                        (GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT_DEBUG) / 2,
                        WIN_WIDTH_DEBUG,WIN_HEIGHT_DEBUG, HWND_DESKTOP,NULL,hInstance,NULL);


    ShowWindow(hwnd,nShowCmd);

    /**********************----------------------------------------
    ** Mostra a janela criada,e espera eventos...
    *********/
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage (&msg);
    }
    return (int)msg.wParam;
}
