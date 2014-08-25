#include "AStar.h"

bool AStar::init()
{
	this->straightCost = 1.0f;
	this->diagCost = sqrt(2);
	this->floydPath = NULL;
	return true;
}

CCArray *AStar::findPath(NodeGrid *grid, int count,bool f)
{
	this->grid = grid;
	this->open = Binary::create();
	this->closed = Binary::create();

	this->startNode = grid->getStartNode();
	this->endNode = grid->getEndNode();
	this->startNode->g = 0;
	this->startNode->h = diagonal(this->startNode);
	this->startNode->f = this->startNode->g + this->startNode->h;
	search(count,f);
	return floydPath;
}

void AStar::search(int count,bool f)
{
	//异步运算。当上一次遍历超出最大允许值后停止遍历，下一次从
	//上次暂停处开始继续遍历		
	ANode *node = this->startNode;
	while (node!=this->endNode)
	{
		/**计算起点周围4个方向的X、Y的范围（包括起点在内），
       就是即将要加入计算的点**/
		int startX = max(0 , node->x - 1);
		int endX = min(grid->getNumCols() - 1, node->x + 1);

		int startY = max(0, node->y - 1);
		int endY = min(grid->getNumRows() - 1, node->y + 1);

		for (int i = startX; i <= endX; i++)
		{
			for (int j = startY; j <= endY; j++)
			{  
				/////////////////////////////////////////////////////////////////////
				//4个方向 8个方向去掉这个限制
				if (count==4 && i!=node->x && j!=node->y)
				{
					continue;
				}
				/**如果这个节点是当前节点、不能通过的点、且排除水平和垂直方向
			       都是障碍物节点时能从斜角位置传过的特殊情况，**/
				ANode *test = grid->getNode(i, j);
				if (test == node || !test->walkable || !isDiagonalWalkable(node, test))
				{
					continue;
				}
				float cost = straightCost;
				//如果当前点和测试点的x、y不相等的话，那么肯定走的不是横向或者纵向，而是斜线
				if (!((node->x == test->x) || (node->y==test->y)))
				{
					//所以改变cost代价
					cost = diagCost;
				}
				//计算G值 = 当前点的G值 + 走一格的代价值 * 代价因子；
				float g = node->g + cost*test->costMultiplier;
				//传入测试点估计H值
				float h = diagonal(test);
				float f = g + h;
				//如果测试点在考察列表或者已考察列表中
				bool isInOpen = open->indexOf(test) != -1;
				bool isInClosed = closed->indexOf(test) != -1;
				//打开中 或者 关闭中
				if (isInOpen || isInClosed)
				{
					//如果本次计算的F值更小，则以本次的F值为准
					if (test->f >f)
					{
						test->f = f;
						test->g = g;
						test->h = h;
						//重新指定该测试点为本轮计算中心点
						test->setParent(node);
						//打开
						if (isInOpen)
						{
							open->updateNode(test);
						}
					}
				}
				/**如果不在待考察列表和已考察列表中，
				则除了更新代价以及设置父节点外，
				还 要将该测试点加入待考察数组*/
				else
				{
					test->f = f;
					test->g = g;
					test->h = h;
					test->setParent(node);
					open->push(test);
				}
			}
		}
		//把经过这次处理的中心点加入到已考察列表中
		closed->push(node);
		//如果待考察列表的长度为0，则没找到路径
		if (open->getLength() == 0)
		{
			return;
		}
		/**从待考察列表中取出到终点所需代价最小的节点，重新赋值给当前节点，
		做为下次的中心点，不断循环，直到当前点和终点节点是同一个节点时，
		说明已经找到路径了*/
		node = open->shift();
	}
	buildPath(f);
	return;
}

void AStar::floyd()
{
	if (floydPath == NULL)
	{
		return;
	}
	int len = floydPath->count();
	if (len>2)
	{
		ANode *vector = ANode::create(0, 0);
		ANode *tempVector =  ANode::create(0, 0);
		//遍历路径数组中全部路径节点，合并在同一直线上的路径节点
		//假设有1,2,3,三点，若2与1的横、纵坐标差值分别与3与2的横、纵坐标差值相等则
		//判断此三点共线，此时可以删除中间点2
		ANode *nodeD1 = dynamic_cast<ANode*>(floydPath->objectAtIndex(len - 1));
		ANode *nodeD2 = dynamic_cast<ANode*>(floydPath->objectAtIndex(len - 2));
		floydVector(vector, nodeD1, nodeD2);
		for (int i = floydPath->count() - 3; i >= 0;i--)
		{
			ANode *nodeA1 = dynamic_cast<ANode*>(floydPath->objectAtIndex(i + 1));
			ANode *nodeA = dynamic_cast<ANode*>(floydPath->objectAtIndex(i));
			floydVector(tempVector, nodeA1, nodeA);
			if (vector->x==tempVector->x&&vector->y==tempVector->y)
			{
				floydPath->removeObject(nodeA1);
			}
			else
			{
				vector->x = tempVector->x;
				vector->y = tempVector->y;
			}
		}
	}
	//合并共线节点后进行第二步，消除拐点操作。算法流程如下：
	//如果一个路径由1-10十个节点组成，那么由节点10从1开始检查
	//节点间是否存在障碍物，若它们之间不存在障碍物，则直接合并
	//此两路径节点间所有节点。
	len = floydPath->count();
	for (int i = len - 1; i >= 0;i--)
	{
		for (int j = 0; j < i - 1;j++)
		{
			ANode *ni = dynamic_cast<ANode*>(floydPath->objectAtIndex(i));
			ANode *nj = dynamic_cast<ANode*>(floydPath->objectAtIndex(j));
			if (!grid->hasBarrier(ni->x, ni->y,nj->x,nj->y))
			{
				for (int k = i - 1; k>j; k--)
				{
					floydPath->removeObjectAtIndex(k);
				}
				i = j;
				len = floydPath->count();
				break;
			}
		}
	}
}

void AStar::buildPath(bool f)
{
	this->floydPath = CCArray::create();
	ANode *node = endNode;
	this->floydPath->addObject(node);

	while (node!=startNode)
	{
		node = node->getParent();
		if (node==NULL)
		{
			return;
		}
		if (floydPath->containsObject(node))
		{
			break;
		}
		floydPath->insertObject(node, 0);
	}
	if (f)
	{
		floyd();
	}
}

void AStar::floydVector(ANode *target, ANode *n1, ANode *n2)
{
	target->x = n1->x - n2->x;
	target->y = n1->y - n2->y;
}

bool AStar::isDiagonalWalkable(ANode *node1, ANode *node2)
{
	ANode *nearByNode1 = grid->getNode(node1->x, node2->y);
	ANode *nearByNode2 = grid->getNode(node2->x, node1->y);
	if (nearByNode1->walkable || nearByNode2->walkable)
	{
		return true;
	}
	return false;
}

float AStar::manhattan(ANode *node)
{
	float a = abs(node->x - endNode->x)*straightCost;
	float b = abs(node->y + endNode->y)*straightCost;
	return a+b;
}

float AStar::euclidian(ANode *node)
{
	float dx = node->x - endNode->x;
	float dy = node->y - endNode->y;
	return sqrt(dx*dx + dy*dy)*straightCost;

}

float AStar::diagonal(ANode *node)
{
	float dx = abs((node->x - endNode->x));
	float dy = abs((node->y - endNode->y));
	float diag = min(dx,dy);
	float staight = dx + dy;
	return diagCost * diag + straightCost*(staight - 2 * diag);
}