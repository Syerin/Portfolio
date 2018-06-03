#pragma once
#include "defines.h"
class Bitmap;
class ResourceManager
{
private:
	Bitmap* m_pBitmap[RES_TYPE_END];
	Bitmap* m_pBitmap_Character[CHARACTER_TYPE_END];

	static 	ResourceManager* m_pThis;
	ResourceManager();
public:
	void Init(HDC hdc);

	Bitmap* GetBitmap(int ResourceType);
	Bitmap* GetBitmapCharacter(int ResourceType);

	static 	ResourceManager* GetInstance()
	{
		if (m_pThis == NULL)
		{
			m_pThis = new ResourceManager();
		}
		return m_pThis;
	}

	~ResourceManager();
};

