//CBattleField.cpp

#include"CBattleField.h"
#include<Windows.h>


void CBattleField::Counteract(CPlayer & Player1, CPlayer & Player2)
{
	for(int i=0;i<LENGTHOFPIPE-1;++i)		//only need to check one pipe but two place in one time
	{
		if(ATTACK==Player1.m_Pipe[i])
		{
			if(JINK==Player2.m_Pipe[LENGTHOFPIPE-i-1])
			{
				Player1.m_Pipe[i] = EMPTY;
				Player2.m_Pipe[LENGTHOFPIPE-i-1] = EMPTY;
			}
			else if(JINK==Player2.m_Pipe[LENGTHOFPIPE-i-2])
			{
				Player1.m_Pipe[i] = EMPTY;
				Player2.m_Pipe[LENGTHOFPIPE-i-2] = EMPTY;
			}
		}
		
		if(JINK==Player1.m_Pipe[i])
		{
			if(ATTACK==Player2.m_Pipe[LENGTHOFPIPE-i-1])
			{
				Player1.m_Pipe[i] = EMPTY;
				Player2.m_Pipe[LENGTHOFPIPE-i-1] = EMPTY;
			}
			else if(ATTACK==Player2.m_Pipe[LENGTHOFPIPE-i-2])
			{
				Player1.m_Pipe[i] = EMPTY;
				Player2.m_Pipe[LENGTHOFPIPE-i-2] = EMPTY;
			}
		}
	}


	if((ATTACK==Player1.m_Pipe[LENGTHOFPIPE-1] && JINK==Player2.m_Pipe[0]) || (JINK==Player1.m_Pipe[LENGTHOFPIPE-1] && ATTACK==Player2.m_Pipe[0]))		//the special circumstance
	{
		Player1.m_Pipe[LENGTHOFPIPE-1] = EMPTY;
		Player2.m_Pipe[0] = EMPTY;
	}
}





