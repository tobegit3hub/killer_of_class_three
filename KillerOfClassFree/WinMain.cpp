//WinMain.cpp


#define TIMEROFGAME 1
#define SPEEDOFGAME 300
#define TIMEROFCARD 2
#define SPEEDOFCARD 5000

#pragma comment (lib, "winmm.lib")


#include<Windows.h>
#include<MMSystem.h>                  //should be declare after  #include<Windows.h>
#include<CommDlg.h>
#include<stdio.h>

#include"resource.h"
#include"CBattleField.h"


ATOM MyRegisterClass(HINSTANCE hInstance);
bool InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID CALLBACK TimerProcOfGame(HWND hWnd,
						UINT uMsg,
						UINT_PTR idEvent,
						DWORD dwTime);
VOID CALLBACK TimerProcOfCard(HWND hWnd,
						UINT uMsg,
						UINT_PTR idEvent,
						DWORD dwTime);
INT_PTR CALLBACK DialogProc(HWND hwndDlg,
							UINT uMsg,
							WPARAM wParam,
							LPARAM lParam);

void PaintAll(HDC hDC);
void PaintPlayer1(HDC hDC);
void PaintPlayer2(HDC hDC);
void InitAll();
void ReleaseAll();
void ChoseRole();
void GameOver(CPlayer Player);		//Judge if the game is over and show the winner

void SkillOfEmpty(CPlayer & Player);
void SkillOfChenYinan(CPlayer & Player);		//make the function of skill
void SkillOfFuZhineng(CPlayer & Player);
void SkillOfChenDihao(CPlayer & Player);
void SkillOfDengChao(CPlayer & Player);
void SkillOfHeWuxun(CPlayer & Player);
void SkillOfHuangJianbin(CPlayer & Player);


LPCWSTR lpAppName = TEXT("!!! Killer Of Class Free !!!");
HINSTANCE hInst;
HWND hWnd;
HWND hDlg;
HDC hDC;
PAINTSTRUCT ps;
RECT rect;
bool StatusOfGame = true;
bool StatusOfAI = false;		//set the status of AI 
HBITMAP hBmp1, hBmp2, hBmpAttack1, hBmpJink1, hBmpAttack2, hBmpJink2, hBmpPeach, hBmpDuel, hBmpBlood, hBmpBlood_bk;		//hBmp1 is the picture of player1, hBmpAttack is the picture of Attack of player1
HBITMAP hBmp11, hBmp12, hBmp21, hBmp22;		//add double two legatus, which means the second general
HBITMAP hBmpAll, hBmpBackground;		//paint all the picture here firstly
HDC hdcMem1, hdcMem2, hdcMemAttack1, hdcMemJink1, hdcMemAttack2, hdcMemJink2, hdcMemPeach, hdcMemDuel, hdcMemBlood, hdcMemBlood_bk;
HDC hdcMem11, hdcMem12, hdcMem21, hdcMem22;		//add double two legatus, which means the second general
HDC hdcMemAll, hdcMemBackground;		//all bmp is painted here firstly


int cxOfWindow, cyOfWindow;		//record the length and width of client rectangle
int cxOfChecker, cyOfChecker;		//record the length and width of each checker
int MouseX, MouseY;		//record the x, y in the coordinate
TCHAR buffer[128];
int MoveOfBackground = 0;

CBattleField BattleField;
CPlayer Player1, Player2;



int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine, 
					 int nCmdShow)
{
	MyRegisterClass(hInstance);

	InitInstance(hInstance, nCmdShow);

	MSG msg;
	while(1)
	{
		if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			if( WM_QUIT==msg.message )
			{
				return 1;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{



			
		}
	}

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = lpAppName;
	wc.lpszMenuName  = MAKEINTRESOURCE(IDM_MAIN);
	wc.style = CS_HREDRAW | CS_VREDRAW;

	return RegisterClass(&wc);
}


bool InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hWnd = CreateWindow( lpAppName, lpAppName,
						 WS_OVERLAPPEDWINDOW,
						 CW_USEDEFAULT, CW_USEDEFAULT,
						 CW_USEDEFAULT, CW_USEDEFAULT,
						 NULL, 
						 NULL,
						 hInstance,
						 NULL);

	ShowWindow(hWnd, SW_SHOWMAXIMIZED);

	UpdateWindow(hWnd);

	
	ChoseRole();		//built a dialog and let player1 choose the roles

	
	return true;
}







LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		{
			hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
			GetClientRect(hWnd, &rect);

			if(true==StatusOfGame)
			{
				SetTimer(hWnd, TIMEROFGAME, SPEEDOFGAME, TimerProcOfGame);
			}
			SetTimer(hWnd, TIMEROFCARD, SPEEDOFCARD, TimerProcOfCard);
			PlaySound(TEXT("Threer.wav"), NULL, SND_ASYNC | SND_LOOP);		

		}break;


	case WM_COMMAND:		//the message from menu
		{
			switch(wParam)
			{
			case IDM_START:
				{
					SetTimer(hWnd, TIMEROFGAME, SPEEDOFGAME, TimerProcOfGame);
					StatusOfGame = true;
				}break;

			case IDM_PAUSE:
				{
					KillTimer(hWnd, TIMEROFGAME);
					StatusOfGame = false;
				}break;

			case IDM_HOWTOPLAY:
				{
					MessageBox(hWnd, TEXT("Pressing 'D''S''A' and 'J''K''L' can buy some cards."), TEXT("About How To Play"), MB_OK | MB_ICONINFORMATION);
				}break;

			case IDM_EXIT:
				{
					int res = MessageBox(hWnd, TEXT("Do you really want to quit..."), TEXT("Hope You Were"), MB_YESNO | MB_ICONQUESTION);
					if(IDYES==res)
					{
						PostQuitMessage(0);
					}
				}break;



			case IDM_OPEN:		//choose the song by yourself
				{
					OPENFILENAME ofn;						
					TCHAR szFile[MAX_PATH];
					ZeroMemory(&ofn,sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.lpstrFile = szFile;
					ofn.lpstrFile[0] = TEXT('\0');
					ofn.nMaxFile = sizeof(szFile);
					ofn.lpstrFilter = TEXT("mp3\0*.mp3\0ALL\0*.*\0Text\0*.TXT\0C++\0*.cpp\0");
					ofn.nFilterIndex = 1;
					ofn.lpstrFileTitle = NULL;
					ofn.nMaxFileTitle = 0;						
					ofn.lpstrInitialDir = NULL;
					ofn.Flags = OFN_EXPLORER |OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
					GetOpenFileName(&ofn);

					TCHAR lpShortPath[256];
					GetShortPathName(szFile, lpShortPath, sizeof(lpShortPath));
						
					wsprintf(buffer, TEXT("play %s"), lpShortPath);
						
					PlaySound(NULL,0,SND_PURGE);		//make the PlaySound's song pause
					mciSendString(buffer, NULL, 0, NULL);		//play the song in fact
				}break;

			case IDM_RUANJIANRENDECHUANGQI:
				{
					PlaySound(TEXT("Threer.wav"), NULL, SND_ASYNC | SND_LOOP);
				}break;

			case IDM_SIXTEENTHNIGHT:
				{
					PlaySound(TEXT("SixteenthNight.wav"), NULL, SND_ASYNC | SND_LOOP);
				}break;

			case IDM_STOP:		//stop the music
				{
					PlaySound(NULL,0,SND_PURGE);
				}break;



			case IDM_KILLEROFCLASSFREE:
				{
					MessageBox(hWnd, TEXT("Copyleft: You can do anything about the SOURCE CODE."), TEXT("General Public License"), MB_OKCANCEL| MB_ICONASTERISK);
				}







			case IDM_ACTIVE:
				{
					StatusOfAI = true;
				}break;

			case IDM_DEACTIVE:
				{
					StatusOfAI = false;
					
				}break;
			}
		}break;





	case WM_SIZE:
		{
			GetClientRect(hWnd, &rect);		//get the size of client window
			cxOfWindow = rect.right - rect.left;		//compute the length of window
			cyOfWindow = rect.bottom - rect.top;		//compute the height of window

			cxOfChecker = cxOfWindow /LENGTHOFPIPE;		//compute the length of each checker
			cyOfChecker = cyOfWindow /LENGTHOFPIPE*2;		//compute the height of each checker

			InitAll();		
		}break;







	case WM_LBUTTONDOWN:		//you can switch the role or let out some cards by pressing left button
		{
			MouseX = LOWORD(lParam);		//get the x, y right now
			MouseY = HIWORD(lParam);


			if(MouseX>=cxOfWindow/12 && MouseX<=cxOfWindow/4 && MouseY>=cyOfWindow/12 && MouseY<=cyOfWindow/2 && Player1.m_Power>=300)		//make the skill of player1's first general
			{
				if(ChenYinan==Player1.m_MyRole[0])		//set the skill
				{
					Player1.m_Skill = SkillOfChenYinan;
				}
				else if(FuZhineng==Player1.m_MyRole[0])
				{
					Player1.m_Skill = SkillOfFuZhineng;
				}
				else if(ChenDihao==Player1.m_MyRole[0])
				{
					Player1.m_Skill = SkillOfChenDihao;
				}
				else if(DengChao==Player1.m_MyRole[0])
				{
					Player1.m_Skill = SkillOfDengChao;
				}
				else if(HeWuxun==Player1.m_MyRole[0])
				{
					Player1.m_Skill = SkillOfHeWuxun;
				}
				else if(HuangJianbin==Player1.m_MyRole[0])
				{
					Player1.m_Skill = SkillOfHuangJianbin;				
				}
				else if(EMPTY==Player1.m_MyRole[0])
				{
					Player1.m_Skill = SkillOfEmpty;
					Player1.m_Power += 300;		//give back 300 power
				}
				Player1.m_Skill(Player1);		//use the skill
				Player1.m_MinusPower(300);		//minus the power while using skill
				
			}

			if(MouseX>=cxOfWindow*7/12 && MouseX<=cxOfWindow*9/12 && MouseY>=cyOfWindow/12 && MouseY<=cyOfWindow/2 && Player1.m_Power>=300)		//make the skill of player2's first general
			{
				if(ChenYinan==Player2.m_MyRole[0])		//set the skill
				{
					Player2.m_Skill = SkillOfChenYinan;
				}
				else if(FuZhineng==Player2.m_MyRole[0])
				{
					Player2.m_Skill = SkillOfFuZhineng;
				}
				else if(ChenDihao==Player2.m_MyRole[0])
				{
					Player2.m_Skill = SkillOfChenDihao;
				}
				else if(DengChao==Player2.m_MyRole[0])
				{
					Player2.m_Skill = SkillOfDengChao;
				}
				else if(HeWuxun==Player2.m_MyRole[0])
				{
					Player2.m_Skill = SkillOfHeWuxun;
				}
				else if(HuangJianbin==Player2.m_MyRole[0])
				{
					Player2.m_Skill = SkillOfHuangJianbin;				
				}
				else if(EMPTY==Player2.m_MyRole[0])
				{
					Player2.m_Skill = SkillOfEmpty;
					Player2.m_Power += 300;
				}
				Player2.m_Skill(Player2);		//use the skill
				Player2.m_MinusPower(300);		//minus the power while using skill
				
			}





			if(MouseX>=cxOfWindow/3 && MouseX<=cxOfWindow/3+cxOfChecker && MouseY>=cyOfWindow/12 && MouseY<=cyOfWindow/12+cyOfChecker)		//change the first and second general of player1
			{
				Player1.m_SwapRole(Player1.m_MyRole[0], Player1.m_MyRole[1]);

				ReleaseAll();		//release all the bitmap firstly because it's not really frequent
				InitAll();		//reset the role's place
			}
			else if(MouseX>=cxOfWindow/3 && MouseX<=cxOfWindow/3+cxOfChecker && MouseY>=cyOfWindow/3 && MouseY<=cyOfWindow/3+cyOfChecker)		//change the first and third general of player1
			{
				Player1.m_SwapRole(Player1.m_MyRole[0], Player1.m_MyRole[2]);

				ReleaseAll();
				InitAll();
			}


			if(MouseX>=cxOfWindow*5/6 && MouseX<=cxOfWindow*5/6+cxOfChecker && MouseY>=cyOfWindow/12 && MouseY<=cyOfWindow/12+cyOfChecker)		//change the first and second general of player2
			{
				Player2.m_SwapRole(Player2.m_MyRole[0], Player2.m_MyRole[1]);		//almost the same as player1

				ReleaseAll();
				InitAll();
			}
			else if(MouseX>=cxOfWindow*5/6 && MouseX<=cxOfWindow*5/6+cxOfChecker && MouseY>=cyOfWindow/3 && MouseY<=cyOfWindow/3+cyOfChecker)		//change the first and third general of player2
			{
				Player2.m_SwapRole(Player2.m_MyRole[0], Player2.m_MyRole[2]);		//almost the same as player1

				ReleaseAll();
				InitAll();	
			}



			if(MouseY>=cyOfWindow*5/6 && MouseY<=cyOfWindow)		//the premise of responding card message
			{
				for(int i=0;i<4;++i)		
				{
					if(MouseX>=(i+1)*cxOfWindow/12 && MouseX<=(i+1)*cxOfWindow/12+cxOfChecker)		//respond the message from cards of player1
					{
						if( PEACH==Player1.m_Cards[i]  && Player1.m_Blood<6 && Player1.m_Peach() ) 
							Player1.m_Cards[i] = EMPTY;		//and clear the status of cards
						else if( ATTACK==Player1.m_Cards[i] &&Player1.m_Attack() )
							Player1.m_Cards[i] = EMPTY; 
						else if( JINK==Player1.m_Cards[i] && Player1.m_Jink() )
							Player1.m_Cards[i] = EMPTY; 	
						else if( DUEL==Player1.m_Cards[i]) 
						    Player1.m_Cards[i] = EMPTY; 		//do nothing at this time
					}
					else if(MouseX>=(i+1+6)*cxOfWindow/12 && MouseX<=(i+1+6)*cxOfWindow/12+cxOfChecker)		//respond the message from cards of player2
					{
						if( PEACH==Player2.m_Cards[i]  && Player2.m_Blood<6 && Player2.m_Peach() ) 
							Player2.m_Cards[i] = EMPTY; 
						else if( ATTACK==Player2.m_Cards[i] && Player2.m_Attack() )
							Player2.m_Cards[i] = EMPTY;  //and clear the status of cards
						else if( JINK==Player2.m_Cards[i] && Player2.m_Jink() )
							Player2.m_Cards[i] = EMPTY; 
						else if( DUEL==Player2.m_Cards[i]) 
							Player2.m_Cards[i] = EMPTY;  		//do mothing
					}
				}


			}

			InvalidateRect(hWnd, &rect, true);		//no matter which card is chosen, display it immediately
			UpdateWindow(hWnd);
		}break;



	case WM_RBUTTONDOWN:		//you can discard the cards by pressing right button
		{
			MouseX = LOWORD(lParam);		//get the x, y right now
			MouseY = HIWORD(lParam);

			if(MouseY>=cyOfWindow*5/6 && MouseY<=cyOfWindow)		//the base of responding card message
			{
				for(int i=0;i<4;++i)		
				{
					if(MouseX>=(i+1)*cxOfWindow/12 && MouseX<=(i+1)*cxOfWindow/12+cxOfChecker)		//just clear the card
					{
						Player1.m_Cards[i] = EMPTY;		//clear the status of cards
					}

					if(MouseX>=(i+1+6)*cxOfWindow/12 && MouseX<=(i+1+6)*cxOfWindow/12+cxOfChecker)		//almost the same as player1
					{
						Player2.m_Cards[i] = EMPTY;		//clear the status of cards
					}
				}
			}
		}break;


	case WM_KEYDOWN:		//you can buy some cards by pressing different key on keyboard
		{
			switch(wParam)
			{
			case 'D':
				{
					if(Player1.m_Power>=90 && EMPTY==Player1.m_Pipe[0])			//you should have enough power and space
					{
						Player1.m_Attack();
						Player1.m_MinusPower(90);		//when it works, just minus the power
					}
					InvalidateRect(hWnd, &rect, true);		//make sure the card appear right now
					UpdateWindow(hWnd);
				}break;

			case 'S':
				{
					if(Player1.m_Power>=100 && EMPTY==Player1.m_Pipe[0]) 
					{
						Player1.m_Jink();
						Player1.m_MinusPower(100);
					}
					InvalidateRect(hWnd, &rect, true);
					UpdateWindow(hWnd);
				}break;

			case 'A':
				{
					if(Player1.m_Power>=150) 
					{
						Player1.m_Peach();
						Player1.m_MinusPower(150);
					}
					InvalidateRect(hWnd, &rect, true);
					UpdateWindow(hWnd);
				}break;


			case 'J':		//the following is for player2
				{
					if(Player2.m_Power>=90 && EMPTY==Player1.m_Pipe[0])
					{
						Player2.m_Attack();
						Player2.m_MinusPower(90);
					}
					InvalidateRect(hWnd, &rect, true);
					UpdateWindow(hWnd);
				}break;

			case 'K':
				{
					if(Player2.m_Power>=100 && EMPTY==Player1.m_Pipe[0])
					{
						Player2.m_Jink();
						Player2.m_MinusPower(100);
					}
					InvalidateRect(hWnd, &rect, true);
					UpdateWindow(hWnd);
				}break;

			case 'L':
				{
					if(Player2.m_Power>=150)
					{
						Player2.m_Peach();
						Player2.m_MinusPower(150);
					}
					InvalidateRect(hWnd, &rect, true);
					UpdateWindow(hWnd);
				}break;




			case VK_SPACE:		//make a pause or restart of the game
				{
					if(true==StatusOfGame)
					{
						KillTimer(hWnd, TIMEROFGAME);
						StatusOfGame = false;
					}
					else
					{
						SetTimer(hWnd, TIMEROFGAME, SPEEDOFGAME, TimerProcOfGame);
						StatusOfGame = true;
					}
				}break;
			}
		}



	case WM_PAINT:		//just paint all the picture in a function
		{
			hDC = BeginPaint(hWnd, &ps);	


			PaintAll(hDC);
			

			EndPaint(hWnd, &ps);
		}break;


	case WM_ERASEBKGND:		
		{


			return 0;		//avoid painting blackground in white brush
		}break;


	case WM_CLOSE:
		{		
			ReleaseAll();		//release all the DC which is used
			PostQuitMessage(0);
		}break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}







VOID CALLBACK TimerProcOfGame(HWND hWnd,		//make the game keeps going on
						UINT uMsg,
						UINT_PTR idEvent,
						DWORD dwTime)
{
	BattleField.Counteract(Player1, Player2);		//check and erase if Attach and Jink meets


	Player1.m_Move();		//make the pipe of player1 move
	if( ATTACK==Player1.m_Pipe[LENGTHOFPIPE-1] )		//if the lastest attach meet another's base
	{
		if(Player2.m_Blood>0)
		{
			Player2.m_Blood--;
		}
		if(0>=Player2.m_Blood)		//if another player's blood runs out
		{
			GameOver(Player1);
		}
	}
	Player1.m_AddPower(10);		//add power randomly


	Player2.m_Move();		//almost the same as player1
	if( ATTACK==Player2.m_Pipe[LENGTHOFPIPE-1] )
	{
		if(Player1.m_Blood>0)
		{
			Player1.m_Blood--;
		}
		InvalidateRect(hWnd, &rect, true);
		UpdateWindow(hWnd);
		if(0>=Player1.m_Blood)
		{
			GameOver(Player2);
		}
	}
	Player2.m_AddPower(10);




	if(true==StatusOfAI)		//if active the AI, then do that automaticly
	{
		int NumberOfAttack1=0;
		int NumberOfJink1=0;
		int NumberOfJink2=0;

		for(int i=0;i<LENGTHOFPIPE;++i)
		{
			if(ATTACK==Player1.m_Pipe[i])  NumberOfAttack1++;		//record the number of attack
			if(JINK==Player1.m_Pipe[i])  NumberOfJink1++;
			if(JINK==Player2.m_Pipe[i])  NumberOfJink2++;
		}
		for(int j=0;j<4;++j)
		{			
			if(NumberOfJink2<NumberOfAttack1 && JINK==Player2.m_Cards[j])		//if attack, then jink
			{
				Player2.m_Jink();
				Player2.m_Cards[j] = EMPTY;
				NumberOfJink2++;
			}
			if(0>=NumberOfJink1 && ATTACK==Player2.m_Cards[j])
			{
				Player2.m_Attack();
				Player2.m_Cards[j] = EMPTY;
				NumberOfJink1++;
			}
			if(Player2.m_Blood<6 && PEACH==Player2.m_Cards[j])
			{
				Player2.m_Peach();
				Player2.m_Cards[j] = EMPTY;
				//PlaySound(TEXT("Peach.wav"), NULL, SND_ASYNC);		
			}
		}
	}






	InvalidateRect(hWnd, &rect, true);		//force to redraw immediately
	UpdateWindow(hWnd);
}



VOID CALLBACK TimerProcOfCard(HWND hWnd,		//just make some to refresh the empty cards
						UINT uMsg,
						UINT_PTR idEvent,
						DWORD dwTime)
{
	Player1.m_RefreshCards();
	Player2.m_RefreshCards();
}




void InitAll()		//init the role of both player1 and player2
{
	hDC = GetDC(hWnd);

	wsprintf(buffer, TEXT("%i.bmp"), Player1.m_MyRole[0]);		//get the role of first general
	hBmp1 = (HBITMAP)LoadImage(hInst, buffer, IMAGE_BITMAP, cxOfWindow/6, cyOfWindow*5.0/12, LR_LOADFROMFILE);		//load the picture of first general
	hdcMem1 = CreateCompatibleDC(hDC);
	SelectObject(hdcMem1, hBmp1);

	wsprintf(buffer, TEXT("%i.bmp"), Player1.m_MyRole[1]);
	hBmp11 = (HBITMAP)LoadImage(hInst, buffer, IMAGE_BITMAP, cxOfWindow/12, cyOfWindow/6, LR_LOADFROMFILE);		//load the picture of second general
	hdcMem11 = CreateCompatibleDC(hDC);
	SelectObject(hdcMem11, hBmp11);

	wsprintf(buffer, TEXT("%i.bmp"), Player1.m_MyRole[2]);
	hBmp12 = (HBITMAP)LoadImage(hInst, buffer, IMAGE_BITMAP, cxOfWindow/12, cyOfWindow/6, LR_LOADFROMFILE);		//load the picture of third general
	hdcMem12 = CreateCompatibleDC(hDC);
	SelectObject(hdcMem12, hBmp12);


	wsprintf(buffer, TEXT("%i.bmp"), Player2.m_MyRole[0]);		//load the picture of the first general of player2
	hBmp2 = (HBITMAP)LoadImage(hInst, buffer, IMAGE_BITMAP, cxOfWindow/6, cyOfWindow*5.0/12, LR_LOADFROMFILE);
	hdcMem2 = CreateCompatibleDC(hDC);
	SelectObject(hdcMem2, hBmp2);

	wsprintf(buffer, TEXT("%i.bmp"), Player2.m_MyRole[1]);
	hBmp21 = (HBITMAP)LoadImage(hInst, buffer, IMAGE_BITMAP, cxOfWindow/12, cyOfWindow/6, LR_LOADFROMFILE);		//load the picture of second general
	hdcMem21 = CreateCompatibleDC(hDC);
	SelectObject(hdcMem21, hBmp21);

	wsprintf(buffer, TEXT("%i.bmp"), Player2.m_MyRole[2]);
	hBmp22 = (HBITMAP)LoadImage(hInst, buffer, IMAGE_BITMAP, cxOfWindow/12, cyOfWindow/6, LR_LOADFROMFILE);		//load the picture of third general
	hdcMem22 = CreateCompatibleDC(hDC);
	SelectObject(hdcMem22, hBmp22);



	hBmpAttack1 = (HBITMAP)LoadImage(hInst, TEXT("Attack1.bmp"), IMAGE_BITMAP, cxOfChecker, cyOfChecker, LR_LOADFROMFILE);	//load the attack card of player1
	hdcMemAttack1 = CreateCompatibleDC(hDC);
	SelectObject(hdcMemAttack1, hBmpAttack1);
	
	hBmpJink1 = (HBITMAP)LoadImage(hInst, TEXT("Jink1.bmp"), IMAGE_BITMAP, cxOfChecker, cyOfChecker, LR_LOADFROMFILE);		//load the jink card of player 1
	hdcMemJink1 = CreateCompatibleDC(hDC);
	SelectObject(hdcMemJink1, hBmpJink1);

	hBmpAttack2 = (HBITMAP)LoadImage(hInst, TEXT("Attack3.bmp"), IMAGE_BITMAP, cxOfChecker, cyOfChecker, LR_LOADFROMFILE);		//almost the smae as player1
	hdcMemAttack2 = CreateCompatibleDC(hDC);
	SelectObject(hdcMemAttack2, hBmpAttack2);
	
	hBmpJink2 = (HBITMAP)LoadImage(hInst, TEXT("Jink2.bmp"), IMAGE_BITMAP, cxOfChecker, cyOfChecker, LR_LOADFROMFILE);
	hdcMemJink2 = CreateCompatibleDC(hDC);
	SelectObject(hdcMemJink2, hBmpJink2);

	hBmpPeach = (HBITMAP)LoadImage(hInst, TEXT("Peach.bmp"), IMAGE_BITMAP, cxOfChecker, cyOfChecker, LR_LOADFROMFILE);	//load the peach card of both players
	hdcMemPeach = CreateCompatibleDC(hDC);
	SelectObject(hdcMemPeach, hBmpPeach);

	hBmpDuel = (HBITMAP)LoadImage(hInst, TEXT("Duel.bmp"), IMAGE_BITMAP, cxOfChecker, cyOfChecker, LR_LOADFROMFILE);	//load the duel card of both players
	hdcMemDuel = CreateCompatibleDC(hDC);
	SelectObject(hdcMemDuel, hBmpDuel);


	hBmpBlood = (HBITMAP)LoadImage(hInst, TEXT("Blood.bmp"), IMAGE_BITMAP, cxOfWindow/24, cyOfWindow*5.0/12, LR_LOADFROMFILE);
	hBmpBlood_bk = (HBITMAP)LoadImage(hInst, TEXT("Blood_bk.bmp"), IMAGE_BITMAP, cxOfWindow/24, cyOfWindow*5.0/12, LR_LOADFROMFILE);
	hdcMemBlood = CreateCompatibleDC(hDC);
	hdcMemBlood_bk = CreateCompatibleDC(hDC);
	SelectObject(hdcMemBlood, hBmpBlood);
	SelectObject(hdcMemBlood_bk, hBmpBlood_bk);


	hBmpBackground = (HBITMAP)LoadImage(hInst, TEXT("Background.bmp"), IMAGE_BITMAP, 0, cyOfWindow, LR_LOADFROMFILE);		//load the image of background
	hdcMemBackground = CreateCompatibleDC(hDC);
	SelectObject(hdcMemBackground, hBmpBackground);



	hdcMemAll = CreateCompatibleDC(hDC);		//here to initial the memory DC 
	hBmpAll = CreateCompatibleBitmap(hDC, cxOfWindow, cyOfWindow);
	SelectObject(hdcMemAll, &hBmpAll);


	ReleaseDC(hWnd, hDC);
}



void ReleaseAll()
{
	DeleteDC(hdcMem1);		//release the DC of player1's role
	DeleteDC(hdcMem11);
	DeleteDC(hdcMem12);

	DeleteDC(hdcMem2);		//release the DC of player2's role
	DeleteDC(hdcMem21);
	DeleteDC(hdcMem22);


	DeleteDC(hdcMemAttack1);		//release the DC of cards
	DeleteDC(hdcMemJink1);
	DeleteDC(hdcMemAttack2);
	DeleteDC(hdcMemJink2);
	DeleteDC(hdcMemPeach);
	DeleteDC(hdcMemDuel);
	DeleteDC(hdcMemBlood);
	DeleteDC(hdcMemBlood_bk);


	SelectObject(hdcMemAll, NULL);		//release the bitmap and DC of all
	DeleteObject(hBmpAll);
	DeleteDC(hdcMemAll);
}







void PaintPlayer1(HDC hDC)
{
	wsprintf(buffer, TEXT("%i"), Player1.m_Power);
	TextOut(hDC, cxOfWindow*9/24, cyOfWindow*7/24, buffer, 4);		//output power of player


	for(int i=0;i<LENGTHOFPIPE;++i)			//draw the pipe of player1
	{
		if(ATTACK == Player1.m_Pipe[i])		//draw the Attach of player1
		{	
			BitBlt(hDC, i*cxOfChecker, cyOfWindow *4.0/6, cxOfChecker, cyOfChecker, hdcMemAttack1, 0, 0, SRCCOPY);			
		
		}
		else if(JINK == Player1.m_Pipe[i])			//draw the Jink of player1
		{
			BitBlt(hDC, i*cxOfChecker, cyOfWindow *4.0/6, cxOfChecker, cyOfChecker, hdcMemJink1, 0, 0, SRCCOPY);	
		}
	}


	for(int i=0;i<4;++i)			//draw the cards of player1
	{
		if(ATTACK == Player1.m_Cards[i])		//draw the Attach card of player1
		{	
			BitBlt(hDC, (i+1)*cxOfChecker, cyOfWindow *5.0/6, cxOfChecker, cyOfChecker, hdcMemAttack1, 0, 0, SRCCOPY);			
		}
		else if(JINK == Player1.m_Cards[i])			//draw the Jink card of player1
		{
			BitBlt(hDC, (i+1)*cxOfChecker, cyOfWindow *5.0/6, cxOfChecker, cyOfChecker, hdcMemJink1, 0, 0, SRCCOPY);	
		}
		else if(PEACH == Player1.m_Cards[i])		//draw the Peach card of player1
		{
			BitBlt(hDC, (i+1)*cxOfChecker, cyOfWindow *5.0/6, cxOfChecker, cyOfChecker, hdcMemPeach, 0, 0, SRCCOPY);	
		}
		else if(DUEL == Player1.m_Cards[i])		//draw the Duel card of player1
		{
			BitBlt(hDC, (i+1)*cxOfChecker, cyOfWindow *5.0/6, cxOfChecker, cyOfChecker, hdcMemDuel, 0, 0, SRCCOPY);	
		}
	}




	BitBlt(hDC, cxOfWindow/12, cyOfWindow/12, cxOfWindow/6, cyOfWindow*5.0/12, hdcMem1, 0, 0, SRCCOPY);		//draw the role
	BitBlt(hDC, cxOfWindow/3, cyOfWindow/12, cxOfWindow/12, cyOfWindow/6, hdcMem11, 0, 0, SRCCOPY);		
	BitBlt(hDC, cxOfWindow/3, cyOfWindow/3, cxOfWindow/12, cyOfWindow/6, hdcMem12, 0, 0, SRCCOPY);		

	BitBlt(hDC, cxOfWindow/4, cyOfWindow/12, cxOfWindow/24, cyOfWindow*5.0/12* Player1.m_Blood/6, hdcMemBlood_bk, 0, 0, SRCAND);		//make a mask to draw the blood of player1
	BitBlt(hDC, cxOfWindow/4, cyOfWindow/12, cxOfWindow/24, cyOfWindow*5.0/12* Player1.m_Blood/6, hdcMemBlood, 0, 0, SRCPAINT);

}





void PaintPlayer2(HDC hDC)		//almost the same as player1
{
	wsprintf(buffer, TEXT("%i"), Player2.m_Power);
	TextOut(hDC, cxOfWindow*21/24, cyOfWindow*7/24, buffer, 4);

	for(int i=0;i<LENGTHOFPIPE;++i)
	{
		if(ATTACK == Player2.m_Pipe[i])
		{			
//			HBITMAP hBmpAttackBk2 = (HBITMAP)LoadImage(hInst, TEXT("Attack2_bk.bmp"), IMAGE_BITMAP, cxOfChecker, cyOfChecker, LR_LOADFROMFILE);
//			HDC hdcMemAttackBk2 = CreateCompatibleDC(hDC);
//			SelectObject(hdcMemAttackBk2, hBmpAttackBk2);
//			BitBlt(hDC, (LENGTHOFPIPE-1-i) * cxOfChecker, cyOfWindow *4.0/6, cxOfChecker, cyOfChecker, hdcMemAttackBk2, 0, 0, SRCAND);			//make the invisible blackgroud
			BitBlt(hDC, (LENGTHOFPIPE-1-i) * cxOfChecker, cyOfWindow *4.0/6, cxOfChecker, cyOfChecker, hdcMemAttack2, 0, 0, SRCCOPY);		
		}
		else if( JINK== Player2.m_Pipe[i])
		{			
//			HBITMAP hBmpJinkBk2 = (HBITMAP)LoadImage(hInst, TEXT("Jink2_bk.bmp"), IMAGE_BITMAP, cxOfChecker, cyOfChecker, LR_LOADFROMFILE);
//			HDC hdcMemJinkBk2 = CreateCompatibleDC(hDC);
//			SelectObject(hdcMemJinkBk2, hBmpJinkBk2);
//			BitBlt(hDC, (LENGTHOFPIPE-1-i) * cxOfChecker, cyOfWindow *4.0/6, cxOfChecker, cyOfChecker, hdcMemJinkBk2, 0, 0, SRCAND);			//make the invisible blackgroud			
			BitBlt(hDC, (LENGTHOFPIPE-1-i) * cxOfChecker, cyOfWindow *4.0/6, cxOfChecker, cyOfChecker, hdcMemJink2, 0, 0, SRCCOPY);	
		}
	}



	for(int i=0;i<4;++i)			//draw the cards of player2
	{
		if(ATTACK == Player2.m_Cards[i])		//draw the Attach card of player2
		{	
			BitBlt(hDC, (i+1+6)*cxOfChecker, cyOfWindow *5.0/6, cxOfChecker, cyOfChecker, hdcMemAttack2, 0, 0, SRCCOPY);			
		}
		else if(JINK == Player2.m_Cards[i])			//draw the Jink card of player2
		{
			BitBlt(hDC, (i+1+6)*cxOfChecker, cyOfWindow *5.0/6, cxOfChecker, cyOfChecker, hdcMemJink2, 0, 0, SRCCOPY);	
		}
		else if(PEACH == Player2.m_Cards[i])		//draw the Peach card of player2
		{
			BitBlt(hDC, (i+1+6)*cxOfChecker, cyOfWindow *5.0/6, cxOfChecker, cyOfChecker, hdcMemPeach, 0, 0, SRCCOPY);	
		}
		else if(DUEL == Player2.m_Cards[i])		//draw the Duel card of player2
		{
			BitBlt(hDC, (i+1+6)*cxOfChecker, cyOfWindow *5.0/6, cxOfChecker, cyOfChecker, hdcMemDuel, 0, 0, SRCCOPY);	
		}
	}




	BitBlt(hDC, cxOfWindow* 7.0/12, cyOfWindow/12, cxOfWindow/4, cyOfWindow*5.0/12, hdcMem2, 0, 0, SRCCOPY);
	BitBlt(hDC, cxOfWindow*10.0/12, cyOfWindow/12, cxOfWindow/12, cyOfWindow/6, hdcMem21, 0, 0, SRCCOPY);		
	BitBlt(hDC, cxOfWindow*10.0/12, cyOfWindow/3, cxOfWindow/12, cyOfWindow/6, hdcMem22, 0, 0, SRCCOPY);	


	BitBlt(hDC, cxOfWindow*9.0/12, cyOfWindow/12, cxOfWindow/24, cyOfWindow*5.0/12* Player2.m_Blood/6, hdcMemBlood_bk, 0, 0, SRCAND);
	BitBlt(hDC, cxOfWindow*9.0/12, cyOfWindow/12, cxOfWindow/24, cyOfWindow*5.0/12* Player2.m_Blood/6, hdcMemBlood, 0, 0, SRCPAINT);
}


void PaintAll(HDC hDC)		//to draw all of the objects
{
		BitBlt(hDC, 0, 0, cxOfWindow, cyOfWindow, hdcMemBackground, MoveOfBackground, 0, SRCCOPY);		//paint the background without making much glitter
			
		MoveOfBackground += 2;		//make the background moves
		if(4486-cxOfWindow<MoveOfBackground)
		{
			MoveOfBackground = 0;
		}



	PaintPlayer1(hDC);
	PaintPlayer2(hDC);

	


	//BitBlt(hDC, cxOfWindow/12, cyOfWindow/12, cxOfWindow/6, cyOfWindow*5.0/12, hdcMem1, 0, 0, SRCCOPY);		//直接hDC就可以了
	//BitBlt(hdcMemAll, cxOfWindow/12, cyOfWindow/12, cxOfWindow/6, cyOfWindow*5.0/12, hdcMem1, 0, 0, SRCCOPY);		//为什么放这里可以，原来的改就不行
	//BitBlt(hDC, 0, 0, cxOfWindow, cyOfWindow, hdcMemAll, 0, 0, SRCCOPY);		//还是闪屏更严重，而且是黑色的, CompatibleBitmap是否用& 
}










void ChoseRole()
{
	hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_CHOSEROLE), hWnd, DialogProc);
	ShowWindow(hDlg, SW_SHOWNORMAL);
	UpdateWindow(hDlg);
}




INT_PTR CALLBACK DialogProc(HWND hwndDlg,
							UINT uMsg,
							WPARAM wParam,
							LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		{
			switch(wParam)		//choose all the roles but only for player1
			{
			case IDR_CHENYINAN:
				{
					Player1.m_MyRole[0] = ChenYinan;
					InvalidateRect(hWnd, &rect, true);
					UpdateWindow(hWnd);
				}break;

			case IDR_FUZHINENG:
				{
					Player1.m_MyRole[0] = FuZhineng;
					InvalidateRect(hWnd, &rect, true);
					UpdateWindow(hWnd);
				}break;


			case IDC_CHENDIHAO:
				{
					if(Nobody==Player1.m_MyRole[1])
					{
						Player1.m_MyRole[1] = ChenDihao;
					}
					else
					{
						Player1.m_MyRole[2] = ChenDihao;
					}
				}break;

			case IDC_DENGCHAO:
				{
					if(Nobody==Player1.m_MyRole[1])
					{
						Player1.m_MyRole[1] = DengChao;
					}
					else
					{
						Player1.m_MyRole[2] = DengChao;
					}
				}break;

			case IDC_HEWUXUN:
				{
					if(Nobody==Player1.m_MyRole[1])
					{
						Player1.m_MyRole[1] = HeWuxun;
					}
					else
					{
						Player1.m_MyRole[2] = HeWuxun;
					}
				}break;

			case IDC_HUANGJIANBIN:
				{
					if(Nobody==Player1.m_MyRole[1])
					{
						Player1.m_MyRole[1] = HuangJianbin;
					}
					else
					{
						Player1.m_MyRole[2] = HuangJianbin;
					}
				}break;


			case IDB_DONE:
				{
					InitAll();		//if done, initial the player and paint them

					InvalidateRect(hWnd, &rect, true);
					UpdateWindow(hWnd);

					EndDialog(hDlg, 0);
				}
			}
		}break;
	}
	return 0;
}


void GameOver(CPlayer Player)
{
	KillTimer(hWnd, TIMEROFGAME);

	TCHAR  buffer[128];
	wsprintf(buffer, TEXT("!!!Game Over!!! Do you want to play again ?"), Player.m_Blood);
	int res = MessageBox(NULL, buffer, TEXT("I'm The Winner"), MB_YESNOCANCEL);
	if(IDNO==res)
	{
		PostQuitMessage(0);
	}
	else if(IDYES==res)
	{
		ChoseRole();
		Player1.m_Blood = 4;
		Player2.m_Blood = 4;
	}
	SetTimer(hWnd, TIMEROFGAME, SPEEDOFGAME, TimerProcOfGame);
}


void SkillOfEmpty(CPlayer & Player)
{
	;		//do nothing
}

void SkillOfChenYinan(CPlayer & Player)
{
	Player.m_Pipe[0] = ATTACK;
	Player.m_Pipe[1] = ATTACK;
	Player.m_Pipe[2] = ATTACK;

	//PlaySound(TEXT("ChenYinan.wav"), NULL, NULL);	
}

void SkillOfFuZhineng(CPlayer & Player)
{
	Player.m_Pipe[0] = JINK;
	Player.m_Pipe[1] = JINK;
	Player.m_Pipe[2] = JINK;
}

void SkillOfChenDihao(CPlayer & Player)
{
	Player.m_Pipe[0] = ATTACK;
	Player.m_Pipe[1] = ATTACK;
	Player.m_Pipe[2] = JINK;
}

void SkillOfDengChao(CPlayer & Player)
{
	Player.m_Pipe[0] = JINK;
	Player.m_Pipe[1] = JINK;
	Player.m_Pipe[2] = JINK;
}

void SkillOfHeWuxun(CPlayer & Player)
{
	Player.m_Pipe[0] = JINK;
	Player.m_Pipe[1] = JINK;
	Player.m_Pipe[2] = JINK;
}

void SkillOfHuangJianbin(CPlayer & Player)
{
	Player.m_Pipe[0] = JINK;
	Player.m_Pipe[1] = JINK;
	Player.m_Pipe[2] = JINK;
}