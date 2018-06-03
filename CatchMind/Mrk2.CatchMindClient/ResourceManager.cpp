#include "ResourceManager.h"
#include "Bitmap.h"
#include "defines.h"
ResourceManager* ResourceManager::m_pThis = NULL;

ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
	for (int i = 0; i < RES_TYPE_END; i++)
		SAFE_DELETE(m_pBitmap[i]);

	for (int i = 0; i < CHARACTER_TYPE_END; i++)
		SAFE_DELETE(m_pBitmap_Character[i]);
}


void ResourceManager::Init(HDC hdc)
{
	for (int i = 0; i < RES_TYPE_END; i++)
		m_pBitmap[i] = new Bitmap();

	for (int i = 0; i < CHARACTER_TYPE_END; i++)
		m_pBitmap_Character[i] = new Bitmap();

	m_pBitmap[RES_TYPE_BACKGROUND]->Init(hdc, "Res\\BackGround.bmp");
	m_pBitmap[RES_TYPE_NAMEBLOCK]->Init(hdc, "Res\\NameBlock.bmp");
	m_pBitmap[RES_TYPE_NAMEBLOCK_HOST]->Init(hdc, "Res\\NameBlock_host.bmp");

	m_pBitmap[RES_TYPE_NAMEBLOCK_FRAME_DRAW]->Init(hdc, "Res\\NameBlockDraw.bmp");
	m_pBitmap[RES_TYPE_NAMEBLOCK_FRAME_READY]->Init(hdc, "Res\\NameBlockReady.bmp");

	m_pBitmap[RES_TYPE_LOGIN]->Init(hdc, "Res\\Login.bmp");
	m_pBitmap[RES_TYPE_LOGIN_ID]->Init(hdc, "Res\\Login_Id.bmp");
	m_pBitmap[RES_TYPE_SELECTCOLOR]->Init(hdc, "Res\\PaintSelect.bmp");
	m_pBitmap[RES_TYPE_GAME_READY]->Init(hdc, "Res\\Ready.bmp");
	m_pBitmap[RES_TYPE_GAME_START]->Init(hdc, "Res\\Start.bmp");

	m_pBitmap[RES_TYPE_BUTTON_READY_WAIT]->Init(hdc, "Res\\ReadyButton_00.bmp");
	m_pBitmap[RES_TYPE_BUTTON_READY]->Init(hdc, "Res\\ReadyButton_01.bmp");

	m_pBitmap[RES_TYPE_BUTTON_START]->Init(hdc, "Res\\StartButton.bmp");;

	m_pBitmap[RES_TYPE_CHAT]->Init(hdc, "Res\\Chat.bmp");
	m_pBitmap[RES_TYPE_QUESTION]->Init(hdc, "Res\\Question.bmp");

	m_pBitmap[RES_TYPE_NEXT_TURN]->Init(hdc, "Res\\NextTurn.bmp");//юс╫ц

	m_pBitmap[RES_TYPE_STAR]->Init(hdc, "Res\\Star.bmp");

	m_pBitmap[RES_TYPE_RANK_LINE]->Init(hdc, "Res\\Rank_Line.bmp");
	m_pBitmap[RES_TYPE_CLEAN]->Init(hdc, "Res\\Clean.bmp");
	m_pBitmap[RES_TYPE_EXPLAIN]->Init(hdc, "Res\\Explain.bmp");
	m_pBitmap[RES_TYPE_ALLREADY]->Init(hdc, "Res\\Game_Ready.bmp");
	m_pBitmap[RES_TYPE_TREPASS]->Init(hdc, "Res\\Trepass.bmp");

	char sztemp[128];
	for (int i = 0; i < CHARACTER_TYPE_END; i++)
	{
		sprintf(sztemp, "Res\\Character\\Character_type_%.2d.bmp", i);
		m_pBitmap_Character[i]->Init(hdc, sztemp);
	}
}

Bitmap* ResourceManager::GetBitmap(int ResourceType)
{
	return m_pBitmap[ResourceType];
}
Bitmap* ResourceManager::GetBitmapCharacter(int ResourceType)
{
	return m_pBitmap_Character[ResourceType];
}