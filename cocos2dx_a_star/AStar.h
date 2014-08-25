#pragma once
#include "Binary.h"
#include "NodeGrid.h"
#include "MathUtil.h"
class AStar : public CCObject
{
private:
	Binary *open;
	Binary *closed;
	NodeGrid *grid;
	ANode *endNode;
	ANode *startNode;
	CCArray *floydPath;
	float straightCost;
	float diagCost;
	void buildPath(bool floy);
	void floydVector(ANode *target, ANode *n1, ANode *n2);
	bool isDiagonalWalkable(ANode *node1, ANode *node2);
	float manhattan(ANode *node);
	float euclidian(ANode *node);
	float diagonal(ANode *node);

	/** 弗洛伊德路径平滑处理
	form http://wonderfl.net/c/aWCe **/
	void floyd();
	void search(int count,bool f);

public:
	CREATE_FUNC(AStar);
	bool init();
	CCArray* findPath(NodeGrid *grid, int count = 8,bool f=true);
};