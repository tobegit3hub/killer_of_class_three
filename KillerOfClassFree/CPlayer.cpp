//CPlayer.cpp


#include<Windows.h>
#include<stdlib.h>
#include<ctime>

#include"CPlayer.h"

CPlayer::CPlayer()
{
	for(int i=0;i<LENGTHOFPIPE;++i)
	{
		m_Pipe[i] = EMPTY;
	}

	for(int i=0;i<4;++i)
	{
		m_Cards[i] = EMPTY;
	}


	m_Blood = 4;
	m_Power = 1000;
	for(int i=0;i<3;++i)
	{
		m_MyRole[i] = Nobody;
	}

	m_MyRole[0] = FuZhineng;
	m_MyRole[2] = ChenDihao;		//just show the default role of second player

	srand( time(0) );
}



void CPlayer::m_Move()
{

	for(int i=LENGTHOFPIPE-1;i>0;--i)
	{
		m_Pipe[i] = m_Pipe[i-1];
	}
	m_Pipe[0] = EMPTY;
	
}


void CPlayer::m_AddPower(int increase)
{
	m_Power += increase;
}

void CPlayer::m_MinusPower(int decrease)
{
	m_Power -= decrease;
}


void CPlayer::m_SwapRole(Role & role1, Role & role2)
{
	Role temp;
	temp = role1;
	role1 = role2;
	role2 = temp;
}

StatusOfCards CPlayer::m_DrawPhase()
{
	int num = rand()%100;
	if(num<40)
	{
		return ATTACK;
	}
	else if(num<70)
	{
		return JINK;
	}
	else if(num<90)
	{
		return PEACH;
	}
	else
	{
		return DUEL;
	}
}

void CPlayer::m_RefreshCards()
{
	for(int i=0;i<4;++i)
	{
		if( EMPTY==m_Cards[i] )
		{
			m_Cards[i] = m_DrawPhase();
		}
	}
}


bool CPlayer::m_Attack()
{
	m_Pipe[0] = ATTACK;
	return true;
}
bool CPlayer::m_Jink()
{
	m_Pipe[0] = JINK;
	return true;
}
bool CPlayer::m_Peach()
{
	if(m_Blood<6)
	{
		m_Blood++;
		return true;
	}
	else
	{
		return false;
	}
}






