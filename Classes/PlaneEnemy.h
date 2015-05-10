#ifndef PLANEENEMY_H
#define PLANEENEMY_H

#include "cocos2d.h"

USING_NS_CC;

//�з��ɻ���
class PlaneEnemy : public Sprite
{
public:
	enum EnemyPlaneType
	{
		Enemy1,
		Enemy2,
		Enemy3,
		Enemy4
	}; //ս������

	enum EnemyPlaneVec
	{
		Enemy1_Vec = 100,
		Enemy2_Vec = 80,
		Enemy3_Vec = 50,
		Enemy4_Vec = 30
	}; //ս���ٶ�

	enum EnemyLife
	{
		Enemy1_Life = 1,
		Enemy2_Life = 2,
		Enemy3_Life = 5,
		Enemy4_Life = 8
	}; //ս������

	enum Enemy_Points
	{
		Enemy1_Points = 10,
		Enemy2_Points = 20,
		Enemy3_Points = 50,
		Enemy4_Points = 80
	}; //ս������

public:
	//�Զ��幤������������enemy�кܶ��֣�����enemy�����ͳ�ʼ������init������г�ʼ��
	static PlaneEnemy* createWithEnemyType(int planetype);
	bool init(int planetype);

	void onEnter();

public:
	void initEnemyBlowUpFrames(int planetype); //���ر�ը����ͼ��

	//��ʱ�����л����ӵ����书��
	/*
	void beginShooting(float dt); //��ʼ�����ӵ������dt�뷢��һ��
	void removeBullet(float dt); //�Ƴ��䵽��Ļ������ӵ�
	void stopShooting(); //ֹͣ�����ӵ�
	*/
	FiniteTimeAction* getBlowUpAction(); //�ɻ���ը
	void getHurt(); //�ɻ�������һ��
	bool isLive(); //���طɻ��Ƿ񻹻���
	int getPoints();
	int getType() { return m_planeType; }

	void moveOn(float dt); //ÿdt���ƶ�һ�ξ���

private:
	int m_velocity; //ս���ٶ�
	int m_life; //ս������
	int m_points;
	int m_planeType; //�л�����
	bool m_live; //�ɻ��Ƿ���ŵı�־
	Vector<SpriteFrame*> m_blowframes; //��ű�ը������֡
};

#endif

