//CBattleField.h

#ifndef _H_H_CBattleField
#define _H_H_CBattleField

#include"CPlayer.h"

class CBattleField
{
private:

public:
	CBattleField(){};		//do nothing
	~CBattleField(){};


	void Counteract(CPlayer & Player1, CPlayer & Player2);		//check and clear if Attack and Jink meet

};

















#endif