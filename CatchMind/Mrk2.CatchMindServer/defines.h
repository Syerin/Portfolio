#pragma once

#include <string>
#include <vector>
#include <map>
using namespace std;

#define MAXPLAYER 8
#define BUFSIZE 10240
#define QUESSTION_MAX 5
#define MAXTIME 120  
enum PLAYER_TYPE
{
	PLAYER_TYPE_HOST, 
	PLAYER_TYPE_NOMAL,
	PLAYER_TYPE_GUEST,
	PLAYER_TYPE_END,
};