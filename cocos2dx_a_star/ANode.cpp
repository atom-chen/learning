#include "ANode.h"

bool ANode::init()
{
	this->costMultiplier = 1.0f;
	this->walkable = true;
	this->buriedDepth = -1;
	this->walkable = true;
	this->parent = NULL;
	return true;
}
ANode *ANode::create(int x, int y)
{
	ANode *node = ANode::create();
	node->x = x;
	node->y = y;
	return node;
}

/**
* �ж������ڵ���Ļλ���Ƿ�һ��
* @param otherNode
* @return
*
*/
bool ANode::posEquals(ANode *otherNode)
{
	if (this->x == otherNode->x && this->y == otherNode->y)
	{
		return true;
	}
	return false;
}

/** �õ��˽ڵ㵽��һ�ڵ��������� */
float ANode::getDistanceTo(ANode *targetNode)
{
	float disX = targetNode->x - this->x;
	float disY = targetNode->y - this->y;
	this->distance = sqrtf(disX*disX + disY*disY);
	return this->distance;
}

ANode* ANode::getParent()
{
	return parent;
}

void ANode::setParent(ANode* node)
{
	this->parent = node;
}