#include "Binary.h"

/**
* @param compareValue	�����ֶΣ���Ϊ���ַ�����ֱ�ӱȽϱ����Ԫ�ر����ֵ
*
*/
bool Binary::init()
{
	data = CCArray::create();
	return true;
}

/** �����������Ԫ��
* @param node			����ӵ�Ԫ�ض���
*/
void Binary::push(ANode *node)
{

	//���½ڵ�����ĩβ��
	data->addObject(node);
	int len = data->count();

	//��������ֻ��һ��Ԫ����ʡ��������̣��������Ԫ��ִ���ϸ�����

	if (len>1)
	{
		/** ������ڵ㵱ǰ�������� */
		int index = len;
		/** �½ڵ㵱ǰ���ڵ��������� */
		int parentIndex = index / 2 - 1;
		ANode *temp;

		//�����ĸ��ڵ㣨λ��Ϊ��ǰλ�ó���2ȡ���������4��Ԫ�صĸ��ڵ�λ����2����7��Ԫ�صĸ��ڵ�λ����3���Ƚϣ�
		//�����Ԫ�رȸ��ڵ�Ԫ��С�򽻻�������Ԫ�أ�Ȼ���ٺ���λ�õĸ��ڵ�Ƚϣ�ֱ�����ĸ��ڵ㲻�ٱ�����
		//�����Ѿ����ﶥ�ˣ�����1��λ��
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


/** ���������б��е�һ��Ԫ�� */

ANode *Binary::shift()
{
	//�ȵ�������Ԫ��
	ANode *result = NULL;
	if (data->count()>0)
	{
		result = dynamic_cast<ANode*>(data->objectAtIndex(0));
		data->removeObjectAtIndex(0);
	}
	int len = data->count();
	//����������Ԫ�غ�������˻�������ֻ��һ��Ԫ������ʡ��������̣��������βԪ��ִ���³�����
	if (len>1)
	{
		/** ��β�ڵ� */
		ANode *lastNode = dynamic_cast<ANode*>(data->lastObject());
		data->removeLastObject(true);
		//����βԪ��������λ
		data->insertObject(lastNode, 0);
		/** ĩβ�ڵ㵱ǰ�������� */
		int index = 0;

		/** ĩβ�ڵ㵱ǰ��һ�ӽڵ��������� */
		int childIndex = (index + 1) * 2 - 1;

		/** ĩβ�ڵ㵱ǰ�����ӽڵ��н�С��һ�������� */
		int compareIndex = 0;
		ANode *temp = NULL;

		//�����������ӽڵ�Ƚϣ������С���ӽڵ����С�ͽ����ǽ�����ֱ�������ӽڵ㶼������
		while (childIndex<len)
		{
			//ֻ��һ���ӽڵ�����
			if (childIndex+1==len)
			{
				compareIndex = childIndex;
			}
			//�������ӽڵ���ȡ���н�С���Ǹ�
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


/** ����ĳһ���ڵ��ֵ������ı��˶������ĳһ�ڵ��ֵ�Ժ����Ѳ����Զ�����������������Ҫ�ֶ�
*  ���ô˷������ж��������� */
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