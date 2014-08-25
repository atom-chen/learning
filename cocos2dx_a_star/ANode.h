#pragma once
#include "cocos2d.h"
#include "math.h"
using namespace cocos2d;
class ANode : public CCObject
{
	/** һ���ڵ�ĳߴ� */
public:
	const static int NODE_SIZE = 34;

	/** �ڵ��к� */
	int x;

	/** �ڵ��к� */
	int y;

	float f;
	float g;
	float h;
	bool walkable;
	float costMultiplier;

	/** ������� */
	int buriedDepth;

	/** ���� */
	float distance;

	static ANode *create(int x, int y);
	bool init();
	CREATE_FUNC(ANode);
	/**
	* �ж������ڵ���Ļλ���Ƿ�һ��
	* @param otherNode
	* @return
	*
	*/
	bool posEquals(ANode *otherNode);

	/** �õ��˽ڵ㵽��һ�ڵ��������� */
	float getDistanceTo(ANode *targetNode);
	ANode* getParent();
	void setParent(ANode* node);
private :
	ANode *parent;
};