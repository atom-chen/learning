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
* 判断两节点之间是否存在障碍物
* @param point1
* @param point2
* @return
*
*/
bool NodeGrid::hasBarrier(int startX, int startY, int endX, int endY)
{
	//如果起点终点是同一个点那傻子都知道它们间是没有障碍物的
	if (startX==endX && startY==endY)
	{
		return false;
	}

	//两节点中心位置
	if (getNode(endX,endY)->walkable == false)
	{
		return true;
	}

	/**遍历方向，为true则为横向遍历，否则为纵向遍历*/
	CCPoint p1 = ccp(startX + 0.5, startY + 0.5);
	CCPoint p2 = ccp(endX + 0.5, endY + 0.5);
	float distX = abs(endX - startX);
	float distY = abs(endY - startY);
	bool loopDirection = distX > distY ? true : false;

	/**起始点与终点的连线方程*/
	sel_callfuncx lineFunc;
	/** 循环递增量 */
	float i = 0;
	/** 循环起始值 */
	float loopStart = 0;
	/** 循环终结值 */
	float loopEnd = 0;

	/** 起终点连线所经过的节点 */
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

		//开始纵向遍历起点与终点间的节点看是否存在障碍(不可移动点)
		for (i = loopStart; i <= loopEnd; i++)
		{
			if (i == loopStart)
			{
				i += .5;
			}
			//根据y得到直线上的x值
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
* 得到一个点下的所有节点
* @param xPos		点的横向位置
* @param yPos		点的纵向位置
* @param grid		所在网格
* @param exception	例外格，若其值不为空，则在得到一个点下的所有节点后会排除这些例外格
* @return 			共享此点的所有节点
*
*/
CCArray* NodeGrid::getNodesUnderPoint(float xPos, float yPos, CCArray *exception/* =NULL */)
{
	CCArray *result = CCArray::create();
	bool xIsInt = xPos - (int)xPos==0;
	bool yIsInt = yPos - (int)yPos==0;
	//点由四节点共享情况
	if (xIsInt && yIsInt)
	{
		result->addObject(getNode(xPos - 1, yPos - 1));
		result->addObject(getNode(xPos, yPos - 1));
		result->addObject(getNode(xPos - 1, yPos));
		result->addObject(getNode(xPos, yPos));
	}
	//点由2节点共享情况
	//点落在两节点左右临边上
	else if (xIsInt && !yIsInt)
	{
		result->addObject(getNode(xPos - 1, yPos));
		result->addObject(getNode(xPos, yPos));
	}
	//点落在两节点上下临边上
	else if (!xIsInt && yIsInt)
	{
		result->addObject(getNode(xPos, yPos - 1));
		result->addObject(getNode(xPos, yPos));
	}
	//点由一节点独享情况
	else
	{
		result->addObject(getNode(xPos, yPos));
	}

	//在返回结果前检查结果中是否包含例外点，若包含则排除掉
	if (exception)
	{
		result->removeObjectsInArray(exception);
	}
	return result;
}

/**当终点不可移动时寻找一个离原终点最近的可移动点来替代之 */
ANode* NodeGrid::findReplacer(ANode *fromNode, ANode *toNode)
{
	ANode *result = NULL;

	//若终点可移动则根本无需寻找替代点
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
		//当前遍历节点
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
					//计算此候选节点到起点的距离，记录离起点最近的候选点为替代点
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

/** 计算全部路径点的埋葬深度 */
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


/** 计算一个节点的埋葬深度
* @param node		欲计算深度的节点
* @param loopCount	计算深度时遍历此节点外围圈数。默认值为10*/
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
* 将两个数组合体，元素不重复
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
