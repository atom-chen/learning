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
	//�첽���㡣����һ�α��������������ֵ��ֹͣ��������һ�δ�
	//�ϴ���ͣ����ʼ��������		
	ANode *node = this->startNode;
	while (node!=this->endNode)
	{
		/**���������Χ4�������X��Y�ķ�Χ������������ڣ���
       ���Ǽ���Ҫ�������ĵ�**/
		int startX = max(0 , node->x - 1);
		int endX = min(grid->getNumCols() - 1, node->x + 1);

		int startY = max(0, node->y - 1);
		int endY = min(grid->getNumRows() - 1, node->y + 1);

		for (int i = startX; i <= endX; i++)
		{
			for (int j = startY; j <= endY; j++)
			{  
				/////////////////////////////////////////////////////////////////////
				//4������ 8������ȥ���������
				if (count==4 && i!=node->x && j!=node->y)
				{
					continue;
				}
				/**�������ڵ��ǵ�ǰ�ڵ㡢����ͨ���ĵ㡢���ų�ˮƽ�ʹ�ֱ����
			       �����ϰ���ڵ�ʱ�ܴ�б��λ�ô��������������**/
				ANode *test = grid->getNode(i, j);
				if (test == node || !test->walkable || !isDiagonalWalkable(node, test))
				{
					continue;
				}
				float cost = straightCost;
				//�����ǰ��Ͳ��Ե��x��y����ȵĻ�����ô�϶��ߵĲ��Ǻ���������򣬶���б��
				if (!((node->x == test->x) || (node->y==test->y)))
				{
					//���Ըı�cost����
					cost = diagCost;
				}
				//����Gֵ = ��ǰ���Gֵ + ��һ��Ĵ���ֵ * �������ӣ�
				float g = node->g + cost*test->costMultiplier;
				//������Ե����Hֵ
				float h = diagonal(test);
				float f = g + h;
				//������Ե��ڿ����б�����ѿ����б���
				bool isInOpen = open->indexOf(test) != -1;
				bool isInClosed = closed->indexOf(test) != -1;
				//���� ���� �ر���
				if (isInOpen || isInClosed)
				{
					//������μ����Fֵ��С�����Ա��ε�FֵΪ׼
					if (test->f >f)
					{
						test->f = f;
						test->g = g;
						test->h = h;
						//����ָ���ò��Ե�Ϊ���ּ������ĵ�
						test->setParent(node);
						//��
						if (isInOpen)
						{
							open->updateNode(test);
						}
					}
				}
				/**������ڴ������б���ѿ����б��У�
				����˸��´����Լ����ø��ڵ��⣬
				�� Ҫ���ò��Ե�������������*/
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
		//�Ѿ�����δ�������ĵ���뵽�ѿ����б���
		closed->push(node);
		//����������б�ĳ���Ϊ0����û�ҵ�·��
		if (open->getLength() == 0)
		{
			return;
		}
		/**�Ӵ������б���ȡ�����յ����������С�Ľڵ㣬���¸�ֵ����ǰ�ڵ㣬
		��Ϊ�´ε����ĵ㣬����ѭ����ֱ����ǰ����յ�ڵ���ͬһ���ڵ�ʱ��
		˵���Ѿ��ҵ�·����*/
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
		//����·��������ȫ��·���ڵ㣬�ϲ���ͬһֱ���ϵ�·���ڵ�
		//������1,2,3,���㣬��2��1�ĺᡢ�������ֵ�ֱ���3��2�ĺᡢ�������ֵ�����
		//�жϴ����㹲�ߣ���ʱ����ɾ���м��2
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
	//�ϲ����߽ڵ����еڶ����������յ�������㷨�������£�
	//���һ��·����1-10ʮ���ڵ���ɣ���ô�ɽڵ�10��1��ʼ���
	//�ڵ���Ƿ�����ϰ��������֮�䲻�����ϰ����ֱ�Ӻϲ�
	//����·���ڵ�����нڵ㡣
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