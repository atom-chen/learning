#pragma once
#include "ANode.h"

class NodeGrid : public CCObject
{
public:
	int top;
	int bottom;
	static NodeGrid *create(int numCols, int numRows);
	bool init();
	CREATE_FUNC(NodeGrid);
	void setSize(int numCols, int numRows);
	ANode* getNode(int x, int y);
	void setEndNode(int x, int y);
	void setStartNode(int x, int y);
	void setWalkable(int x, int y, bool value);
	bool hasBarrier(int staratX, int starttY, int endX, int endY);
	CCArray *getNodesUnderPoint(float xPos, float yPos, CCArray *exception=NULL);
	ANode *findReplacer(ANode *fromNode, ANode *toNode);
	void calculateBuriedDepth();
	ANode* getEndNode();
	int getNumCols();
	int getNumRows();
	ANode* getStartNode();
private:
	CCArray* bornPos;
	CCArray* nodes;
	ANode* startNode;
	ANode* endNode;
	int numCols;
	int numRows;
	int getNodeBuriedDepth(ANode *node, int loopCount = 10);
	CCArray *concatArrays(CCArray *array1, CCArray *array2);

};