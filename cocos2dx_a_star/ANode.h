#pragma once
#include "cocos2d.h"
#include "math.h"
using namespace cocos2d;
class ANode : public CCObject
{
	/** 一个节点的尺寸 */
public:
	const static int NODE_SIZE = 34;

	/** 节点列号 */
	int x;

	/** 节点行号 */
	int y;

	float f;
	float g;
	float h;
	bool walkable;
	float costMultiplier;

	/** 埋葬深度 */
	int buriedDepth;

	/** 距离 */
	float distance;

	static ANode *create(int x, int y);
	bool init();
	CREATE_FUNC(ANode);
	/**
	* 判断两个节点屏幕位置是否一样
	* @param otherNode
	* @return
	*
	*/
	bool posEquals(ANode *otherNode);

	/** 得到此节点到另一节点的网格距离 */
	float getDistanceTo(ANode *targetNode);
	ANode* getParent();
	void setParent(ANode* node);
private :
	ANode *parent;
};