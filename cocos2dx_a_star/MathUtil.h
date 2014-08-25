#pragma once

#include "cocos2d.h"
using namespace cocos2d;

typedef float(*sel_callfuncx)(CCPoint p1, CCPoint p2, float y);

#define funcx_selector(_SELECTOR) (sel_callfuncx)(&_SELECTOR)

class MathUtil
{
public:

	/**
	* 根据两点确定这两点连线的二元一次方程 y = ax + b或者 x = ay + b
	* @param ponit1
	* @param point2
	* @param type		指定返回函数的形式。为0则根据x值得到y，为1则根据y得到x
	*
	* @return 由参数中两点确定的直线的二元一次函数
	*/
	static sel_callfuncx getLineFunc(CCPoint p1, CCPoint p2, int type = 0);
	static float getLineFunc1(CCPoint p1, CCPoint p2, float y);
	static float getLineFunc2(CCPoint p1, CCPoint p2, float x);
	static float getLineFunc3(CCPoint p1, CCPoint p2, float x);
	static float getLineFunc4(CCPoint p1, CCPoint p2, float y);
	/**
	* 得到两点间连线的斜率
	* @param ponit1
	* @param point2
	* @return 			两点间连线的斜率
	*
	*/
	static float getSlope(CCPoint p1, CCPoint p2);
};