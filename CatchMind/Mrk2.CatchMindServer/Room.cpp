#include <Windows.h>	
#include "Room.h"
#include "Packet.h"
#include <time.h>


Room::Room()
{
	m_iPlayerCount = 0;
	for(int i = 0; i < MAXPLAYER; i++)
	m_bPlayerNumber[i] = false;
	m_eCurGameState = GAME_STATE_WAIT;
	m_fEffect = 0.0f;
	m_fTimer = 0.0f;
	m_iQuestionCount = 1;
}


Room::~Room()
{
}


void Room::Init()
{
	printf("------Question Load------\n");
	FILE* pFile = fopen("Question.txt", "r");
	char szbuf[BUFSIZE];
	int Questioncount = 0;
	if (pFile == NULL)
		fclose(pFile);
	else
	{
		while (!feof(pFile))
		{
			QUESTION_INFO* pQuestionInfo = new QUESTION_INFO;
			pQuestionInfo->prevSend = false;
			fscanf(pFile, "%s\n", pQuestionInfo->strQuestion.c_str());
			m_mapQuestion.insert(make_pair(Questioncount, pQuestionInfo));
			Questioncount++;
		}
		fclose(pFile);
	}


	for (auto iter = m_mapQuestion.begin(); iter != m_mapQuestion.end(); iter++)
	{
		printf("%s\n", (*iter).second->strQuestion.c_str());
	}
	fclose(pFile);
	printf("-----Question Load Complete------\n");

}
void Room::EnterPlayer(SOCKET sock,char* buf)
{
	bool isNull=false;
	PACKET_ROOM_IN packet;
	memcpy(&packet, buf, sizeof(packet));
	USER_INFO* pUserInfo = new USER_INFO;		
	pUserInfo->sock = sock;
	pUserInfo->strName = packet.name;
	pUserInfo->PlayerType = PLAYER_TYPE_NOMAL;
	pUserInfo->m_bReady = false;
	if (m_eCurGameState != GAME_STATE_WAIT)
	{
		pUserInfo->PlayerState = PLAYER_STATE_TRESPASS;
	}
	else
	{
		pUserInfo->PlayerState = PLAYER_STATE_WAIT;
	}
	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter) == NULL)
		{
			(*iter) = pUserInfo;
			isNull = true;
			break;
		}
	}
	if(isNull==false)
	m_vecUser.push_back(pUserInfo);
	if (m_eCurGameState != GAME_STATE_WAIT)
	{
		USERRANK* pUserRank = NULL;
		pUserRank = new USERRANK;
		pUserRank->Score = 0;
		pUserRank->sock = sock;
		pUserRank->strName = packet.name;
		m_vecUserRank.push_back(pUserRank);
	}
	m_iPlayerCount++;
}

void Room::JoinPlayer(SOCKET sock)
{
	bool isHost = false; //호스트존재여부
	if (m_iPlayerCount > 1)
	{
		for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
		{
			if ((*iter)->PlayerType == PLAYER_TYPE_HOST)
				isHost = true;
		}
	}
	if (m_iPlayerCount<= MAXPLAYER)
	{
		PACKET_ROOM_IN  packet_room_in;
		packet_room_in.header.wIndex = PACKET_INDEX_ROOM_IN;
	
		for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
		{
			if ((*iter) == NULL)
				continue;
			if ((*iter)->sock == sock)
			{
				for (int i = 0; i < MAXPLAYER; i++)
				{
					if (m_bPlayerNumber[i] == false)
					{
						//빈자리
						m_bPlayerNumber[i] = true;
						(*iter)->PlayerNumber = i;
						packet_room_in.index = i;
						break;
					}
				}
				if (!isHost)
				{
					(*iter)->PlayerType = PLAYER_TYPE_HOST;
					packet_room_in.type = (int)PLAYER_TYPE_HOST;
				}
				else
				{
					(*iter)->PlayerType = PLAYER_TYPE_NOMAL;
					packet_room_in.type = (int)PLAYER_TYPE_NOMAL;
				}
				packet_room_in.state = (*iter)->PlayerState;
				strcpy(packet_room_in.name, (*iter)->strName.c_str());
				packet_room_in.header.wLen = sizeof(PACKET_HEADER) + 20 + strlen((*iter)->strName.c_str()) + 1;
				packet_room_in.curRound = m_iQuestionCount;
				packet_room_in.RoundEnd = QUESSTION_MAX;
				send(sock, (char*)&packet_room_in, packet_room_in.header.wLen, 0);
				break;
			}
		}
	}
}

void Room::NoticeOtherPlayer(SOCKET sock)
{
	PACKET_ROOM_PLAYER_INFO packet;
	packet.header.wIndex = PACKET_INDEX_ROOM_PLAYER_INFO;

	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter) == NULL)
			continue;
		if ((*iter)->sock == sock)
			continue;
		strcpy(packet.name, (*iter)->strName.c_str());
		packet.index = (*iter)->PlayerNumber;
		packet.type = (*iter)->PlayerType;
		packet.State = (*iter)->PlayerState;
		packet.header.wLen = sizeof(PACKET_HEADER) + 12 + strlen((*iter)->strName.c_str()) + 1;
		send(sock, (char*)&packet, packet.header.wLen, 0);
	}


	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter)->sock == sock)
		{
			strcpy(packet.name, (*iter)->strName.c_str());
			packet.index = (*iter)->PlayerNumber;
			packet.type = (*iter)->PlayerType;
			packet.State = (*iter)->PlayerState;
			packet.header.wLen = sizeof(PACKET_HEADER) + 12 + strlen((*iter)->strName.c_str()) + 1;
			break;
		}
	}
	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter) == NULL)
		continue;
		if ((*iter)->sock == sock)
			continue;
		send((*iter)->sock, (char*)&packet, packet.header.wLen, 0);
	}
}
void Room::ReadyPlayer(SOCKET sock,char* buf)
{
	PACKET_ROOM_PLAYER_READY Packet_ready;
	memcpy(&Packet_ready, buf, sizeof(Packet_ready));
	for (int i = 0; i < m_iPlayerCount; i++)
	{
		if(m_vecUser[i]->PlayerNumber == Packet_ready.PlayerNumber)
		m_vecUser[i]->m_bReady = Packet_ready.m_bReady;
		if (m_vecUser[i]->PlayerType != PLAYER_TYPE_HOST)
		{
			if (Packet_ready.m_bReady)
			{
				m_vecUser[i]->PlayerState = PLAYER_STATE_READY;
			}
			else
			{
				m_vecUser[i]->PlayerState = PLAYER_STATE_WAIT;

			}
		}
	}
	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter) == NULL)
		continue;
		if ((*iter)->sock == sock)
			continue;
		send((*iter)->sock, (char*)&Packet_ready,sizeof(Packet_ready), 0);
	}
}
void Room::StartGame(SOCKET sock,char* buf)
{
	PACKET_ROOM_START Packet_start;
	memcpy(&Packet_start, buf, sizeof(Packet_start));
	Packet_start.curRound = m_iQuestionCount;


	USERRANK* pUserRank = NULL;

	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		pUserRank = new USERRANK;
		pUserRank->Score = 0;
		pUserRank->sock = (*iter)->sock;
		pUserRank->strName = (*iter)->strName;
		m_vecUserRank.push_back(pUserRank);
	}


	for (int i = 0; i < m_vecUser.size(); i++)
	{
		if (m_vecUser[i] == NULL)
			continue;

		if (m_vecUser[i]->PlayerType == PLAYER_TYPE_HOST)
		{
			m_vecUser[i]->PlayerState = PLAYER_STATE_DRAW;
			Packet_start.CurDrawPlayer = m_vecUser[i]->PlayerNumber;
			CurDrawPlayer = m_vecUser[i]->PlayerNumber;
			Packet_start.NextDrawPlayer = GetNextDrawing();
			NextDrawPlayer = Packet_start.NextDrawPlayer;
			break;
		}
	}
	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter)->sock == sock)
			continue;
		(*iter)->PlayerState = PLAYER_STATE_GAME;
	}
	m_eCurGameState = GAME_STATE_START;
	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter) == NULL)
			continue;
		send((*iter)->sock, (char*)&Packet_start, sizeof(Packet_start), 0);
		(*iter)->m_bReady = false;
	}
}
bool Room::GameOver()
{
	if (m_iQuestionCount-1 == QUESSTION_MAX || m_iPlayerCount == 1)
	{
		PACKET_RANKING pakcet_ranking;
		pakcet_ranking.header.wIndex = PACKET_INDEX_ROOM_RANK;
		
		for (auto Useriter = m_vecUser.begin(); Useriter != m_vecUser.end(); Useriter++)
		{
			for (auto iter = m_vecUserRank.begin(); iter != m_vecUserRank.end(); iter++)
			{
				pakcet_ranking.Socre = (*iter)->Score;
				strcpy(pakcet_ranking.PlayerName, (*iter)->strName.c_str());
				pakcet_ranking.header.wLen = sizeof(PACKET_HEADER) + 4 + strlen(pakcet_ranking.PlayerName) + 1;
				printf("\n[GameOver : Rank ] %d 에게 %d 만큼 보냈다.\n", (*Useriter)->sock,pakcet_ranking.header.wLen);
				send((*Useriter)->sock, (char*)&pakcet_ranking, pakcet_ranking.header.wLen, 0);
			}
		}

		PACKET_ROOM_GAME_OVER Packet_Gameover;
		Packet_Gameover.header.wIndex = PACKET_INDEX_ROOM_GAMEOVER;
		Packet_Gameover.header.wLen = sizeof(Packet_Gameover);


		for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
		{
			if ((*iter) == NULL)
				continue;
			printf("[GameOver] %d 에게  %d 만큼 보냈다.\n", (*iter)->sock, sizeof(Packet_Gameover));

			send((*iter)->sock, (char*)&Packet_Gameover, sizeof(Packet_Gameover), 0);
		}

		for (auto iter = m_mapQuestion.begin(); iter != m_mapQuestion.end(); iter++)
		{
			(*iter).second->prevSend = false;
		}
		m_vecUserRank.clear();
		m_iQuestionCount = 1;
			return true;
	}
	return false;
}
void Room::SendQuestion(SOCKET sock, char* buf)
{
	int random;
	m_eCurGameState = GAME_STATE_GAME;
	while (true)
	{
		random = rand() % m_mapQuestion.size();
		if (m_mapQuestion[random]->prevSend == false)
		{
			break;
		}
		else
			continue;
	}

	PACKET_REQUEST_QUESTION Packet_Question;
	Packet_Question.header.wIndex = PACKET_INDEX_REQUEST_QUESTION;
	for (auto iter = m_mapQuestion.begin(); iter != m_mapQuestion.end(); iter++)
	{
		if ((*iter).first == random)
		{
			strcpy(Packet_Question.Question, (char*)(*iter).second->strQuestion.c_str());
			m_curQuestion = (*iter).second->strQuestion.c_str();
			(*iter).second->prevSend = true;
		}
	}
	Packet_Question.header.wLen = sizeof(PACKET_HEADER) + strlen(Packet_Question.Question) + 1;
	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter) == NULL)
			continue;
		if ((*iter)->PlayerState == PLAYER_STATE_DRAW)
		{
			m_iQuestionCount++;
			send(sock, (char*)&Packet_Question, Packet_Question.header.wLen, 0);
			break;
		}
	}

}
void Room::ChatPlayer(SOCKET sock, char* buf,int len)
{
	PACKET_ROOM_CHAT Packet_chat;
	memcpy(&Packet_chat, buf, len);

	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter) == NULL)
			continue;
		send((*iter)->sock, (char*)&Packet_chat, Packet_chat.header.wLen,0);
	}
}
void Room::AnswerPlayer(SOCKET sock, char* buf,int len)
{
	PACKET_ROOM_ANSWER Packet_Answer;
	memcpy(&Packet_Answer, buf, len);

	if (!strcmp(m_curQuestion.c_str(), Packet_Answer.buf))
	{
		Packet_Answer.header.wIndex = PAKCET_INDEX_ROOM_ANSWER;
		Packet_Answer.header.wLen = sizeof(PACKET_HEADER) + 4;

		for (auto iter = m_vecUserRank.begin(); iter != m_vecUserRank.end(); iter++)
		{
			if ((*iter)->sock == sock)
			{
				(*iter)->Score++;
				break;
			}
		}

		for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
		{
			if ((*iter) == NULL)
				continue;
			if ((*iter)->sock == sock)
				(*iter)->PlayerState = PLAYER_STATE_ANSWER;

			send((*iter)->sock, (char*)&Packet_Answer, Packet_Answer.header.wLen, 0);
		}
	}
}
void Room::NextTurn(SOCKET sock)
{
	if (GameOver())
	{
		return;
	}
	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter)->PlayerState == PLAYER_STATE_TRESPASS)
			(*iter)->PlayerState = PLAYER_STATE_GAME;
	}
	NextDrawingPlayer();
}
void Room::DrawMouseDown(SOCKET sock, char* buf)
{
	PACKET_ROOM_DRAWDOWN Packet_Mouse_Down;
	memcpy(&Packet_Mouse_Down, buf, sizeof(Packet_Mouse_Down));
	
	if (m_eCurGameState != GAME_STATE_GAME)
	{
		return;
	}
	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter) == NULL)
			continue;
		if ((*iter)->sock == sock)
			continue;
		if ((*iter)->PlayerState == PLAYER_STATE_TRESPASS)
			continue;
		send((*iter)->sock, (char*)&Packet_Mouse_Down, sizeof(Packet_Mouse_Down), 0);
	}
}
void Room::DrawMouseMove(SOCKET sock, char* buf)
{
	PACKET_ROOM_DRAWMOVE Packet_Mouse_Move;
	memcpy(&Packet_Mouse_Move, buf, sizeof(Packet_Mouse_Move));
	

	if (m_eCurGameState != GAME_STATE_GAME)
	{
		return;
	}

	printf("OriginX : %d , OriginY : %d \n", Packet_Mouse_Move.OriginX, Packet_Mouse_Move.OriginY);
	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter) == NULL)
			continue;

		if ((*iter)->sock == sock)
			continue;
		if ((*iter)->PlayerState == PLAYER_STATE_TRESPASS)
			continue;

		send((*iter)->sock, (char*)&Packet_Mouse_Move, sizeof(Packet_Mouse_Move), 0);
	}
}

void Room::ExitPlayer(SOCKET sock, char* buf)
{

}
void Room::ShutDownPlayer(SOCKET sock, char* buf)
{
	printf("ShutDownPlayer 진입.");
	int NextHost = MAXPLAYER;
	int OutPlayerIndex = MAXPLAYER;
	for (int i = 0; i < m_vecUser.size(); i++)
	{
		if (m_vecUser[i] == NULL)
			continue;
		if (m_vecUser[i]->sock == sock)
		{
			m_iPlayerCount--;
			m_bPlayerNumber[m_vecUser[i]->PlayerNumber] = false;
			OutPlayerIndex = m_vecUser[i]->PlayerNumber;
			if (m_iPlayerCount == 0)
			{
				m_eCurGameState = GAME_STATE_WAIT;
				for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
				{
					if ((*iter)->sock == sock)
					{
						iter = m_vecUser.erase(iter);
						return;
					}
				}
			}
			if (m_vecUser[i]->PlayerType == PLAYER_TYPE_HOST)
			{
				printf("ShutDownPlayer가 HOST 였다.");

				for (int j = 0; j < MAXPLAYER; j++)
				{
					if (m_vecUser[i]->PlayerNumber == j)
						continue;

					if (m_bPlayerNumber[j] == true)
					{
						for (int k = 0; k < m_vecUser.size(); k++)
						{
							if (m_vecUser[k]->PlayerNumber == j)
							{
								m_vecUser[k]->PlayerType = PLAYER_TYPE_HOST;
								NextHost = m_vecUser[k]->PlayerNumber;
							}
						}
						break;
					}
				}
			}


			if (m_eCurGameState == GAME_STATE_GAME || m_eCurGameState == GAME_STATE_START || m_eCurGameState == GAME_STATE_NEXT)
			{
				printf("게임중 나갔다. \n");

				if (m_vecUser[i]->PlayerState == PLAYER_STATE_DRAW)
				{
					printf("ShutDownPlayer가 DRAW 상태였다.");

					CurDrawPlayer = NextDrawPlayer;
					NextDrawPlayer = CurDrawPlayer;
					while (true)
					{
						if (m_iPlayerCount == 1)
						{
							m_eCurGameState = GAME_STATE_RESULTS;
							break;
						}
						if (CurDrawPlayer == NextDrawPlayer)
						{
							NextDrawPlayer++;
							continue;
						}
						if (NextDrawPlayer == MAXPLAYER)
						{
							NextDrawPlayer = 0;
						}
						if (m_bPlayerNumber[NextDrawPlayer] == true)
						{
							break;
						}

						if (m_bPlayerNumber[NextDrawPlayer] == false)
							NextDrawPlayer++;
					}

					PACKET_ROOM_SHUTDOWN_DRAW_PLYAER Packet_Shutdown_draw;
					Packet_Shutdown_draw.header.wIndex = PACKET_INDEX_ROOM_SHUTDOWN_DRAW_PLYAER;
					Packet_Shutdown_draw.header.wLen = sizeof(Packet_Shutdown_draw);
					Packet_Shutdown_draw.index = OutPlayerIndex;
					Packet_Shutdown_draw.NextHost = NextHost;
					Packet_Shutdown_draw.GameOver = GameOver();
					for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
					{
						if ((*iter)->sock == sock)
						{
							iter = m_vecUser.erase(iter);
							break;
						}
					}
					for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
					{
						if ((*iter) == NULL)
							continue;
						printf(("%d 에게 %d만큼 보냈습니다\n"), (*iter)->sock, sizeof(Packet_Shutdown_draw));

						send((*iter)->sock, (char*)&Packet_Shutdown_draw, sizeof(Packet_Shutdown_draw), 0);
						printf("PACKET_ROOM_SHUTDOWN_DRAW_PLYAER를 send하였습니다.\n");
					}
					break;
				}
				else if (m_vecUser[i]->PlayerState != PLAYER_STATE_DRAW&& m_vecUser[i]->PlayerState != PLAYER_STATE_TRESPASS)
				{
					printf("ShutDownPlayer가 일반인이었다.");
					if (m_vecUser[i]->PlayerNumber == NextDrawPlayer)
					{
						NextDrawPlayer = GetNextDrawing();
					}


					PACKET_ROOM_SHUTDOWN_NOMAL_PLAYER Packet_Shutdown_nomal;

					Packet_Shutdown_nomal.header.wIndex = PACKET_INDEX_ROOOM_SHUTDOWN_NOMAL_PLAYER;
					Packet_Shutdown_nomal.header.wLen = sizeof(Packet_Shutdown_nomal);
					Packet_Shutdown_nomal.index = OutPlayerIndex;
					Packet_Shutdown_nomal.NextHost = NextHost;
					Packet_Shutdown_nomal.actTrespass = actTressPass();

					
					for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++) 
					{
						if ((*iter)->sock == sock)
						{
							iter = m_vecUser.erase(iter);
							break;
						}
					}
					for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
					{
						if ((*iter) == NULL)
							continue;

						send((*iter)->sock, (char*)&Packet_Shutdown_nomal, sizeof(Packet_Shutdown_nomal), 0);
						printf(("%d 에게 %d만큼 보냈습니다\n"), (*iter)->sock, sizeof(Packet_Shutdown_nomal));
						printf("PACKET_ROOM_SHUTDOWN_PLAYER 를 send하였습니다.\n");
					}
					GameOver();
			
					break;
				}
				else if (m_vecUser[i]->PlayerState == PLAYER_STATE_TRESPASS)
				{
					PACKET_ROOM_SHUTDOWN_PLAYER Packet_Shutdown;
					Packet_Shutdown.header.wIndex = PACKET_INDEX_ROOM_SHUTDOWN_PLAYER;
					Packet_Shutdown.header.wLen = sizeof(Packet_Shutdown);
					Packet_Shutdown.index = OutPlayerIndex;
					Packet_Shutdown.NextHost = NextHost;

					for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); ) 
					{
						if ((*iter)->sock == sock)
						{
							iter = m_vecUser.erase(iter);
							break;
						}
					}
					for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
					{
						if ((*iter) == NULL)
							continue;

						send((*iter)->sock, (char*)&Packet_Shutdown, sizeof(Packet_Shutdown), 0);
						printf(("%d 에게 %d만큼 보냈습니다\n"), (*iter)->sock, sizeof(Packet_Shutdown));
						printf("PACKET_ROOM_SHUTDOWN_PLAYER 를 send하였습니다.");
					}

				}
			}
			else 
			{
				PACKET_ROOM_SHUTDOWN_PLAYER Packet_Shutdown;
				Packet_Shutdown.header.wIndex = PACKET_INDEX_ROOM_SHUTDOWN_PLAYER;
				Packet_Shutdown.header.wLen = sizeof(Packet_Shutdown);
				Packet_Shutdown.index = OutPlayerIndex;
				Packet_Shutdown.NextHost = NextHost;


				for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++) {
					if ((*iter)->sock == sock)
					{
						iter = m_vecUser.erase(iter);
						break;
					}
				}
				for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
				{
					if ((*iter) == NULL)
						continue;

					send((*iter)->sock, (char*)&Packet_Shutdown, sizeof(Packet_Shutdown), 0);
					printf(("%d 에게 %d만큼 보냈습니다\n"), (*iter)->sock, sizeof(Packet_Shutdown));
					printf("PACKET_ROOM_SHUTDOWN_PLAYER 를 send하였습니다.");
				}
				break;
			}
		}
	}
}
void Room::ShutdonwDrawPlayer()
{
	CurDrawPlayer = NextDrawPlayer;
	NextDrawPlayer = GetNextDrawing();

}

void Room::TransTresPassToGame()
{
	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter)->PlayerState == PLAYER_STATE_TRESPASS)
			(*iter)->PlayerState = PLAYER_STATE_GAME;
	}
}
int Room::GetNextRound()
{
	return m_iQuestionCount;
}

void Room::InitRoom()
{
	m_eCurGameState = GAME_STATE_WAIT;
	m_iQuestionCount = 1;
	PACKET_ROOM_INITIALIZED packet_room_init;
	packet_room_init.header.wIndex = PACKET_INDEX_ROOM_INITIALIZED;
	packet_room_init.header.wLen = sizeof(packet_room_init);
	for (auto iter = m_vecUser.begin();iter != m_vecUser.end(); iter++)
	{
		if ((*iter) == NULL)
			continue;
		(*iter)->PlayerState = PLAYER_STATE_WAIT;
		(*iter)->m_bReady = false;
		packet_room_init.PlayerState = PLAYER_STATE_WAIT;
		send((*iter)->sock, (char*)&packet_room_init, sizeof(packet_room_init), 0);
	}
	for (auto iter = m_mapQuestion.begin(); iter != m_mapQuestion.end(); iter++)
	{
		(*iter).second->prevSend = false;
	}
	m_vecUserRank.clear();
}

void Room::NextDrawingPlayer()
{
	CurDrawPlayer = NextDrawPlayer;
	NextDrawPlayer = GetNextDrawing();

	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter) == NULL)
			continue;
		(*iter)->PlayerState = PLAYER_STATE_GAME;
	}

	PACKET_ROOM_NEXTTURN Packet_Next;
	Packet_Next.header.wIndex = PACKET_INDEX_ROOM_NEXTTURN;
	Packet_Next.header.wLen = sizeof(Packet_Next);
	Packet_Next.curDrawPlayer = CurDrawPlayer;
	Packet_Next.NextDrawPlayer = NextDrawPlayer;
	Packet_Next.curRound = m_iQuestionCount;
	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter) == NULL)
			continue;
		if ((*iter)->PlayerNumber == CurDrawPlayer)
		{
			(*iter)->PlayerState = PLAYER_STATE_DRAW;
		}
		send((*iter)->sock, (char*)&Packet_Next, sizeof(Packet_Next), 0);
	}
}

int Room::GetNextDrawing()
{
	NextDrawPlayer = CurDrawPlayer;
	while (true)
	{
		if (m_iPlayerCount == 1)
		{
			break;
		}
		if (CurDrawPlayer == NextDrawPlayer)
		{
			NextDrawPlayer++;
			continue;
		}
		if (NextDrawPlayer == MAXPLAYER)
		{
			NextDrawPlayer = 0;
		}
		if (m_bPlayerNumber[NextDrawPlayer] == true)
		{
			break;
		}

		if (m_bPlayerNumber[NextDrawPlayer] == false)
			NextDrawPlayer++;
	}
	return NextDrawPlayer;
}

bool Room::actTressPass()
{
	int trespassCount = 0;
	for (auto iter = m_vecUser.begin(); iter != m_vecUser.end(); iter++)
	{
		if ((*iter)->PlayerState == PLAYER_STATE_TRESPASS)
		{
			trespassCount++;
		}
	}
	if (m_iPlayerCount == 2 && trespassCount >0)
	{
		return true;
	}
	return false;
}