#include "NodeGrid.h"
#include "MathUtil.h"
#include <math.h>
NodeGrid *NodeGrid::create(int numCols, int numRows)
{
	NodeGrid *grid =NodeGrid::create();
	grid->setSize(numCols, numRows);
	return grid;
}

bool NodeGrid::init()
{
	return true;
}

void NodeGrid::setSize(int numCols, int numRows)
{
	this->numCols = numCols;
	this->numRows = numRows;
	this->nodes = CCArray::create();
	for (int i = 0; i < this->numCols;i++)
	{
		CCArray *nodesN = CCArray::create();
		for (int j = 0; j < this->numRows;j++)
		{
			nodesN->addObject(ANode::create(i, j));
		}

		this->nodes->addObject(nodesN);
	}
}

ANode* NodeGrid::getNode(int x, int y)
{
	if (x>=this->nodes->count())
	{
		return NULL;
	}
	CCArray *cols = dynamic_cast<CCArray*>(this->nodes->objectAtIndex(x));
	if (y>=cols->count())
	{
		return NULL;
	}
	ANode *result = dynamic_cast<ANode*>(cols->objectAtIndex(y));
	return result;
}

void NodeGrid::setEndNode(int x, int y)
{
	this->endNode = getNode(x, y);
}

void NodeGrid::setStartNode(int x, int y)
{
	this->startNode = getNode(x, y);
}

void NodeGrid::setWalkable(int x, int y, bool value)
{
	ANode *n = this->getNode(x, y);
	if (n==NULL)
	{
		return;
	}
	n->walkable = value;
}

/**
* �ж����ڵ�֮���Ƿ�����ϰ���
* @param point1
* @param point2
* @return
*
*/
bool NodeGrid::hasBarrier(int startX, int startY, int endX, int endY)
{
	//�������յ���ͬһ������ɵ�Ӷ�֪�����Ǽ���û���ϰ����
	if (startX==endX && startY==endY)
	{
		return false;
	}

	//���ڵ�����λ��
	if (getNode(endX,endY)->walkable == false)
	{
		return true;
	}

	/**��������Ϊtrue��Ϊ�������������Ϊ�������*/
	CCPoint p1 = ccp(startX + 0.5, startY + 0.5);
	CCPoint p2 = ccp(endX + 0.5, endY + 0.5);
	float distX = abs(endX - startX);
	float distY = abs(endY - startY);
	bool loopDirection = distX > distY ? true : false;

	/**��ʼ�����յ�����߷���*/
	sel_callfuncx lineFunc;
	/** ѭ�������� */
	float i = 0;
	/** ѭ����ʼֵ */
	float loopStart = 0;
	/** ѭ���ս�ֵ */
	float loopEnd = 0;

	/** ���յ������������Ľڵ� */
	CCArray *nodesPassed;
	ANode *elem;

	if (loopDirection)
	{
		lineFunc = MathUtil::getLineFunc(p1, p2, 0);
		loopStart = min(startX, endX);
		loopEnd = max(startX, endX);
		for (i = loopStart; i <= loopEnd;i++)
		{
			if (i==loopStart)
			{
				i += 0.5;
			}
			float yPos = (*lineFunc)(p1, p2, i);
			nodesPassed = getNodesUnderPoint(i, yPos);
			CCObject *eacheO = NULL;
			CCARRAY_FOREACH(nodesPassed, eacheO)
			{
				ANode *each = dynamic_cast<ANode*>(eacheO);
				if (each->walkable==false)
				{
					return true;
				}
			}
			if (i == loopStart + .5)
			{
				i -= .5;
			}
		}
	}
	else
	{
		lineFunc = MathUtil::getLineFunc(p1, p2, 1);

		loopStart = min(startY, endY);
		loopEnd = max(startY, endY);

		//��ʼ�������������յ��Ľڵ㿴�Ƿ�����ϰ�(�����ƶ���)
		for (i = loopStart; i <= loopEnd; i++)
		{
			if (i == loopStart)
			{
				i += .5;
			}
			//����y�õ�ֱ���ϵ�xֵ
			float xPos = (*lineFunc)(p1, p2, i);

			nodesPassed = getNodesUnderPoint(xPos, i);
			CCObject *eacheO = NULL;
			CCARRAY_FOREACH(nodesPassed, eacheO)
			{
				ANode *each = dynamic_cast<ANode*>(eacheO); 
				if (each->walkable == false)
				{
					return true;
				}
			}

			if (i == loopStart + .5){
				i -= .5;
			}
		}

	}
	return false;
}

/**
* �õ�һ�����µ����нڵ�
* @param xPos		��ĺ���λ��
* @param yPos		�������λ��
* @param grid		��������
* @param exception	���������ֵ��Ϊ�գ����ڵõ�һ�����µ����нڵ����ų���Щ�����
* @return 			����˵�����нڵ�
*
*/
CCArray* NodeGrid::getNodesUnderPoint(float xPos, float yPos, CCArray *exception/* =NULL */)
{
	CCArray *result = CCArray::create();
	bool xIsInt = xPos - (int)xPos==0;
	bool yIsInt = yPos - (int)yPos==0;
	//�����Ľڵ㹲�����
	if (xIsInt && yIsInt)
	{
		result->addObject(getNode(xPos - 1, yPos - 1));
		result->addObject(getNode(xPos, yPos - 1));
		result->addObject(getNode(xPos - 1, yPos));
		result->addObject(getNode(xPos, yPos));
	}
	//����2�ڵ㹲�����
	//���������ڵ������ٱ���
	else if (xIsInt && !yIsInt)
	{
		result->addObject(getNode(xPos - 1, yPos));
		result->addObject(getNode(xPos, yPos));
	}
	//���������ڵ������ٱ���
	else if (!xIsInt && yIsInt)
	{
		result->addObject(getNode(xPos, yPos - 1));
		result->addObject(getNode(xPos, yPos));
	}
	//����һ�ڵ�������
	else
	{
		result->addObject(getNode(xPos, yPos));
	}

	//�ڷ��ؽ��ǰ��������Ƿ��������㣬���������ų���
	if (exception)
	{
		result->removeObjectsInArray(exception);
	}
	return result;
}

/**���յ㲻���ƶ�ʱѰ��һ����ԭ�յ�����Ŀ��ƶ��������֮ */
ANode* NodeGrid::findReplacer(ANode *fromNode, ANode *toNode)
{
	ANode *result = NULL;

	//���յ���ƶ����������Ѱ�������
	if (toNode->walkable)
	{
		result = toNode;
	}
	else
	{
		if (toNode->buriedDepth == -1)
		{
			toNode->buriedDepth = getNodeBuriedDepth(toNode, max(this->numCols, numRows));
		}
		int xFrom = toNode->x - toNode->buriedDepth < 0 ? 0 : toNode->x - toNode->buriedDepth;
		int xTo = toNode->x + toNode->buriedDepth > numCols - 1 ? numCols - 1 : toNode->x + toNode->buriedDepth;
		int yFrom = toNode->y - toNode->buriedDepth < 0 ? 0 : toNode->y - toNode->buriedDepth;
		int yTo = toNode->y + toNode->buriedDepth > numRows - 1 ? numRows - 1 : toNode->y + toNode->buriedDepth;

		ANode *n;
		//��ǰ�����ڵ�
		for (int i = xFrom; i < xTo; i++)
		{
			for (int j = yFrom; j < yTo; j++)
			{
				if ((i>xFrom&&i<xTo) && (j>yFrom&& j<yTo))
				{
					continue;
				}
				n = getNode(i, j);
				if (n->walkable)
				{
					//����˺�ѡ�ڵ㵽���ľ��룬��¼���������ĺ�ѡ��Ϊ�����
					n->getDistanceTo(fromNode);
					if (!result)
					{
						result = n;
					}
					else if (n->distance < result->distance)
					{
						result = n;
					}
				}
			}
		}
	}
	return result;
}

/** ����ȫ��·������������ */
void NodeGrid::calculateBuriedDepth()
{
	ANode *node;
	for (int i = 0; i < numCols;i++)
	{
		for (int j = 0; j < numRows;j++)
		{
			CCArray *nodea = dynamic_cast<CCArray*>(nodes->objectAtIndex(i));
			node = dynamic_cast<ANode*>(nodea->objectAtIndex(j));
			if (node->walkable)
			{
				node->buriedDepth = 0;
			}
			else
			{
				node->buriedDepth = getNodeBuriedDepth(node, max(numCols, numRows));
			}
		}
		
	}
}


/** ����һ���ڵ���������
* @param node		��������ȵĽڵ�
* @param loopCount	�������ʱ�����˽ڵ���ΧȦ����Ĭ��ֵΪ10*/
int NodeGrid::getNodeBuriedDepth(ANode *node, int loopCount /* = 10 */)
{
	int result = node->walkable ? 0 : 1;
	int l = 1;
	while (l<=loopCount)
	{
		int startX = node->x - 1 < 0 ? 0 : node->x - 1;
		int endX = node->x + 1 > numCols - 1 ? numCols - 1 : node->x + 1;
		int startY = node->y - 1 < 0 ? 0 : node->y - 1;
		int endY = node->y + 1 > numRows - 1 ? numRows - 1 : node->y + 1;

		ANode *n;
		for (int i = startX; i <= endX; i++)
		{
			for (int j = startY; j <= endY; j++)
			{
				n = getNode(i, j);
				if (n != node&&n->walkable)
				{
					return result;
				}
			}
		}
		result++;
		l++;
	}
	return result;
}
/**
* ������������壬Ԫ�ز��ظ�
* @return
*
*/
CCArray *NodeGrid::concatArrays(CCArray *array1, CCArray *array2)
{
	CCArray * result = CCArray::create();
	result->addObjectsFromArray(array1);
	result->removeObjectsInArray(array2);
	result->addObjectsFromArray(array2);
	return result;
}

int NodeGrid::getNumCols()
{
	return numCols;
}

int NodeGrid::getNumRows()
{
	return numRows;
}


ANode* NodeGrid::getEndNode()
{
	return this->endNode;
}

ANode* NodeGrid::getStartNode()
{
	return startNode;
}
