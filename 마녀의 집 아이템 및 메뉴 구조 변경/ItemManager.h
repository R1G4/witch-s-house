#pragma once
#include "singletonBase.h"
#include "image.h"

class ItemManager:
	public singletonBase<ItemManager>
{

protected:
	//������ ���� �� Ű��(� ���������� �̹��� �̸����� �����Ѵ�.)
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
		ITEMKINDS itemKinds;	//������ ����
		wstring contents;		//������ ����?
		wstring name;			//������ ��
		bool isTry;				//�������� Ŭ���Ͽ� ������ ��� �õ��� �Ͽ�����?
	};

	typedef vector<Item> vItem;
	typedef vector<Item>::iterator viItem; 
	Item _itemSelected;
	FloatRect _rcSelected;	//���õ� ������(������)�� ��Ʈ�� ������
	INDEX _index;
	float _rcAlpha;			//��Ʈ ���İ�(����)
	float _rcAlphaChange;	//��Ʈ ���İ�(����) ������
	MENUSTATE _isOpenCheck;	//����
private:
	vItem _vItem;
	viItem _viItem;
public:
	ItemManager();
	~ItemManager();

	//�������� ����������(ADD)
	bool addItem(string itemKey);

	//�̹��� Ű�� �������� �������� ���������(REMOVE)
	bool useItem(string itemKey);

	//�̹��� �̸� �������� �������� ���������
	bool useItem(int itemNum);

	void render();

	//�׽�Ʈ������ ��� �����(�ӽ�)
	MENUSTATE itemOpen();

	//�ش� �������� �����ϴ��� �Ұ��� ��ȯ�Ѵ�.
	bool KeyCheck(string itemKey)
	{
		for (int i = 0; i < _vItem.size(); i++)
		{

			if (_vItem[i].itemKinds != (ITEMKINDS)conversion(itemKey)) continue;

			return true;
		}
		return false;
	};

	//ITEMKINDS�� ����ȭ�ϸ� �̳��� �ʹ� ���⿡ ���ͷ� �����´�.
	ITEMKINDS getItemKinds(string itemKey) 
	{ 
		for (int i = 0; i < _vItem.size(); i++)
		{
			
			if (_vItem[i].itemKinds != (ITEMKINDS)conversion(itemKey)) continue;
			return (ITEMKINDS)conversion(itemKey);
		}
	};

	//�� �Լ��� �������� ����ϴ°� �ƴ� �׳� ������ ����(������ ���ڸ��� ������ �����)
	bool removeItem(string itemKey);
};