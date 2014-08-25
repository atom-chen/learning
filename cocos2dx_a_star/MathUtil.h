#pragma once

#include "cocos2d.h"
using namespace cocos2d;

typedef float(*sel_callfuncx)(CCPoint p1, CCPoint p2, float y);

#define funcx_selector(_SELECTOR) (sel_callfuncx)(&_SELECTOR)

class MathUtil
{
public:

	/**
	* ��������ȷ�����������ߵĶ�Ԫһ�η��� y = ax + b���� x = ay + b
	* @param ponit1
	* @param point2
	* @param type		ָ�����غ�������ʽ��Ϊ0�����xֵ�õ�y��Ϊ1�����y�õ�x
	*
	* @return �ɲ���������ȷ����ֱ�ߵĶ�Ԫһ�κ���
	*/
	static sel_callfuncx getLineFunc(CCPoint p1, CCPoint p2, int type = 0);
	static float getLineFunc1(CCPoint p1, CCPoint p2, float y);
	static float getLineFunc2(CCPoint p1, CCPoint p2, float x);
	static float getLineFunc3(CCPoint p1, CCPoint p2, float x);
	static float getLineFunc4(CCPoint p1, CCPoint p2, float y);
	/**
	* �õ���������ߵ�б��
	* @param ponit1
	* @param point2
	* @return 			��������ߵ�б��
	*
	*/
	static float getSlope(CCPoint p1, CCPoint p2);
};