#include "Binary.h"

/**
* @param compareValue	排序字段，若为空字符串则直接比较被添加元素本身的值
*
*/
bool Binary::init()
{
	data = CCArray::create();
	return true;
}

/** 向二叉堆中添加元素
* @param node			欲添加的元素对象
*/
void Binary::push(ANode *node)
{

	//将新节点添至末尾先
	data->addObject(node);
	int len = data->count();

	//若数组中只有一个元素则省略排序过程，否则对新元素执行上浮过程

	if (len>1)
	{
		/** 新添入节点当前所在索引 */
		int index = len;
		/** 新节点当前父节点所在索引 */
		int parentIndex = index / 2 - 1;
		ANode *temp;

		//和它的父节点（位置为当前位置除以2取整，比如第4个元素的父节点位置是2，第7个元素的父节点位置是3）比较，
		//如果新元素比父节点元素小则交换这两个元素，然后再和新位置的父节点比较，直到它的父节点不再比它大，
		//或者已经到达顶端，及第1的位置
		while (index>=0 && parentIndex>=0 && compareTwoNodes(node, dynamic_cast<ANode*>(data->objectAtIndex(parentIndex))))
		{
			temp = dynamic_cast<ANode*>(data->objectAtIndex(parentIndex));
			data->replaceObjectAtIndex(parentIndex, node, false);
			data->replaceObjectAtIndex(index - 1, temp, false);
			index /= 2;
			parentIndex = index / 2 - 1;
		}

	}
}


/** 弹出开启列表中第一个元素 */

ANode *Binary::shift()
{
	//先弹出列首元素
	ANode *result = NULL;
	if (data->count()>0)
	{
		result = dynamic_cast<ANode*>(data->objectAtIndex(0));
		data->removeObjectAtIndex(0);
	}
	int len = data->count();
	//若弹出列首元素后数组空了或者其中只有一个元素了则省略排序过程，否则对列尾元素执行下沉过程
	if (len>1)
	{
		/** 列尾节点 */
		ANode *lastNode = dynamic_cast<ANode*>(data->lastObject());
		data->removeLastObject(true);
		//将列尾元素排至首位
		data->insertObject(lastNode, 0);
		/** 末尾节点当前所在索引 */
		int index = 0;

		/** 末尾节点当前第一子节点所在索引 */
		int childIndex = (index + 1) * 2 - 1;

		/** 末尾节点当前两个子节点中较小的一个的索引 */
		int compareIndex = 0;
		ANode *temp = NULL;

		//和它的两个子节点比较，如果较小的子节点比它小就将它们交换，直到两个子节点都比它大
		while (childIndex<len)
		{
			//只有一个子节点的情况
			if (childIndex+1==len)
			{
				compareIndex = childIndex;
			}
			//有两个子节点则取其中较小的那个
			else
			{
				compareIndex = compareTwoNodes(dynamic_cast<ANode*>(data->objectAtIndex(childIndex)), dynamic_cast<ANode*>(data->objectAtIndex(
					childIndex + 1))) ? childIndex : childIndex + 1;
			}
			if (compareTwoNodes(dynamic_cast<ANode*>(data->objectAtIndex(compareIndex)), lastNode))
			{
				temp = dynamic_cast<ANode*>(data->objectAtIndex(compareIndex));
				data->replaceObjectAtIndex(compareIndex, lastNode, true);
				data->replaceObjectAtIndex(index, temp, true);
				index = compareIndex;
				childIndex = (index + 1) * 2 - 1;
			}
			else
			{
				break;
			}
		}

	}
	return result;
}


/** 更新某一个节点的值。在你改变了二叉堆中某一节点的值以后二叉堆不会自动进行排序，所以你需要手动
*  调用此方法进行二叉树更新 */
void Binary::updateNode(ANode *node)
{
	int index = data->indexOfObject(node) + 1;
	if (index ==0)
	{
		return;
	}
	else
	{
		int parentIndex = index / 2 - 1;
		ANode *temp = NULL;
		while (index>-1 && parentIndex>-1 && compareTwoNodes(node, dynamic_cast<ANode*>(data->objectAtIndex(parentIndex))))
		{
			temp = dynamic_cast<ANode*>(data->objectAtIndex(parentIndex));
			data->replaceObjectAtIndex(parentIndex, node, false);
			data->replaceObjectAtIndex(index - 1, temp, false);
			index /= 2;
			parentIndex = index / 2 - 1;
		}
	}
}

int Binary::indexOf(ANode *node)
{
	return data->indexOfObject(node);
}

int Binary::getLength()
{
	return data->count();
}

bool Binary::compareTwoNodes(ANode *node1, ANode *node2)
{
	return node1->f < node2->f;
}

const char* Binary::toString()
{
	const char* result = "";

	int len = data->count();
	for (int i = 0; i < len; i++)
	{
		ANode *node = dynamic_cast<ANode*>(data->objectAtIndex(i));

		result = CCString::createWithFormat("%s%f", result, node->f)->getCString();
		if (i < len - 1)
		{
			result = CCString::createWithFormat("%s,", result)->getCString();
		}
	}
	return result;
}