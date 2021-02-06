#pragma once
#include "singletonBase.h"
#include "image.h"

class ItemManager:
	public singletonBase<ItemManager>
{

protected:
	//아이템 종류 및 키값(어떤 아이템인지 이미지 이름으로 구분한다.)
	enum ITEMKINDS
	{
		ITEM_TEDDYBEAR = 58,
		ITEM_TEDDYBEARBODY = 59,
		ITEM_TEDDYBEARHAND = 11,
		ITEM_FROH = 60
	};

	enum INDEX
	{
		FIRST = 0,
		SECOND,
		THIRD,
		FOURTH
	};

	int conversion(string itemKey); 
	void rcAlphaChange();
private:
	struct Item
	{
		ITEMKINDS itemKinds;	//아이템 종류
		wstring contents;		//아이템 설명?
		wstring name;			//아이템 명
		bool isTry;				//아이템을 클릭하여 아이템 사용 시도를 하였는지?
	};

	typedef vector<Item> vItem;
	typedef vector<Item>::iterator viItem; 
	Item _itemSelected;
	FloatRect _rcSelected;	//선택된 아이템(컨텐츠)를 렉트로 보여줌
	INDEX _index;
	float _rcAlpha;			//렉트 알파값(투명도)
	float _rcAlphaChange;	//렉트 알파값(투명도) 조절용
	MENUSTATE _isOpenCheck;	//상태
private:
	vItem _vItem;
	viItem _viItem;
public:
	ItemManager();
	~ItemManager();

	//아이템을 가져갔을때(ADD)
	bool addItem(string itemKey);

	//이미지 키값 기준으로 아이템을 사용했을때(REMOVE)
	bool useItem(string itemKey);

	//이미지 이름 기준으로 아이템을 사용했을때
	bool useItem(int itemNum);

	void render();

	//테스트용으로 잠시 만든거(임시)
	MENUSTATE itemOpen();

	//해당 아이템이 존재하는지 불값을 반환한다.
	bool KeyCheck(string itemKey)
	{
		for (int i = 0; i < _vItem.size(); i++)
		{

			if (_vItem[i].itemKinds != (ITEMKINDS)conversion(itemKey)) continue;

			return true;
		}
		return false;
	};

	//ITEMKINDS을 전역화하면 이넘이 너무 많기에 겟터로 가져온다.
	ITEMKINDS getItemKinds(string itemKey) 
	{ 
		for (int i = 0; i < _vItem.size(); i++)
		{
			
			if (_vItem[i].itemKinds != (ITEMKINDS)conversion(itemKey)) continue;
			return (ITEMKINDS)conversion(itemKey);
		}
	};

	//이 함수는 아이템을 사용하는게 아닌 그냥 아이템 제거(곰돌이 손자를때 쓰려고 만든거)
	bool removeItem(string itemKey);
};