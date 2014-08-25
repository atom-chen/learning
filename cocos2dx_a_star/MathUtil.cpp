#include "MathUtil.h"


/**
* ��������ȷ�����������ߵĶ�Ԫһ�η��� y = ax + b���� x = ay + b
* @param ponit1
* @param point2
* @param type		ָ�����غ�������ʽ��Ϊ0�����xֵ�õ�y��Ϊ1�����y�õ�x
*
* @return �ɲ���������ȷ����ֱ�ߵĶ�Ԫһ�κ���
*/
sel_callfuncx MathUtil::getLineFunc(CCPoint p1, CCPoint p2, int type)
{
	sel_callfuncx resultFuc;
	// �ȿ���������һ����ֱ��������ֱ�ߵ��������ʱֱ�߷���Ϊ y = a ���� x = a ����ʽ
	if (p1.x == p2.x)
	{
		if (type == 1)
		{
			resultFuc = MathUtil::getLineFunc1;
		}
		return resultFuc;
	}
	else if (p1.y == p2.y)
	{
		if (type==0)
		{
			resultFuc = MathUtil::getLineFunc2;
		}
		return resultFuc;
	}

	if (type==0)
	{
		resultFuc = MathUtil::getLineFunc3;
	}
	else if (type==1)
	{
		resultFuc = MathUtil::getLineFunc4;
	}
	return resultFuc;
}

float MathUtil::getLineFunc1(CCPoint p1, CCPoint p2,float y)
{
	return p1.x;
}
float MathUtil::getLineFunc2(CCPoint p1, CCPoint p2, float x)
{
	return p1.y;
}
float MathUtil::getLineFunc3(CCPoint p1, CCPoint p2, float x)
{
	float a = (p1.y - p2.y) / (p1.x - p2.x);
	float b = (p1.y - a * p1.x);
	return a*x + b;
}
float MathUtil::getLineFunc4(CCPoint p1, CCPoint p2, float y)
{
	float a = (p1.y - p2.y) / (p1.x - p2.x);
	float b = (p1.y - a * p1.x);
	return (y-b)/a;
}
/**
* �õ���������ߵ�б��
* @param ponit1
* @param point2
* @return 			��������ߵ�б��
*
*/
float MathUtil::getSlope(CCPoint p1, CCPoint p2)
{
	return (p2.y - p1.y) / (p2.x - p1.x);
}