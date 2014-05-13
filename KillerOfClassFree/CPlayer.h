//CPlayer.h

#ifndef _H_H_CPlayer
#define _H_H_CPLayer

#define LENGTHOFPIPE 12		//define the number of checkers of each pipe


enum StatusOfCards
{EMPTY=0, ATTACK=1, JINK=2, PEACH=3, DUEL=4};

enum Role		//define all the roles that can be used in this game
{Nobody=0,ChenDihao=41, DengChao=42, ChenYinan=45, HuangJianbin=47, HeWuxun=2, FuZhineng=1};



class CPlayer		//contain all the attributions of player
{
private:
					//because it would be very very hard to access if define the data in private
public:
	CPlayer();		//initial all the data member
	~CPlayer(){};		//do nothing


	StatusOfCards m_Pipe[LENGTHOFPIPE];		//record the battlefield of each player
	StatusOfCards m_Cards[4];		//each player has four cards at one time
	int m_Blood;		//record the HP of player
	int m_Power;		//the power can be used to buy cards
	Role m_MyRole[3];		//each player has three generals
	void (*m_Skill) (CPlayer & Player);		//but only the first general can use its skill

	void m_Move();		//let the card in battlefield move
	void m_AddPower(int increase);		//to add some power
	void m_MinusPower(int decrease);		//to minus some power
	void m_SwapRole(Role & role1, Role & role2);		//exchange first general and  second or third general
	StatusOfCards m_DrawPhase();		//to get one card randomly
	void m_RefreshCards();		//to refresh the m_Cards[]

	bool m_Attack();
	bool m_Jink();
	bool m_Peach();
	
};















#endif