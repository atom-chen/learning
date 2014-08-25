#pragma once
#include "ANode.h"
class Binary:public CCObject
{
public:
	/**
	* @param compareValue	�����ֶΣ���Ϊ���ַ�����ֱ�ӱȽϱ����Ԫ�ر����ֵ
	*
	*/
	CREATE_FUNC(Binary);
	bool init();

	/** �����������Ԫ��
	* @param node			����ӵ�Ԫ�ض���
	*/
	void push(ANode *node);
	/** ���������б��е�һ��Ԫ�� */
	ANode *shift();
	/** ����ĳһ���ڵ��ֵ������ı��˶������ĳһ�ڵ��ֵ�Ժ����Ѳ����Զ�����������������Ҫ�ֶ�
	*  ���ô˷������ж��������� */
	void updateNode(ANode *node);
	/** ����ĳ�ڵ���������λ�� */
	int indexOf(ANode *node);
	int getLength();
	/**�Ƚ������ڵ㣬����true���ʾ��һ���ڵ�С�ڵڶ���*/
	bool compareTwoNodes(ANode *node1, ANode *node2);

	/** д�˷�����Ŀ�����ڿ���trace������Ҫ�鿴�Ľ����ֱ��traceһ��Binary���󼴿ɵõ�����ȫ��Ԫ�ص�ֵ��
	* �����ֶε�ֵ */
	const char* toString();
private:
	CCArray *data;

};