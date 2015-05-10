#ifndef PLANEENEMY_H
#define PLANEENEMY_H

#include "cocos2d.h"

USING_NS_CC;

//敌方飞机类
class PlaneEnemy : public Sprite
{
public:
	enum EnemyPlaneType
	{
		Enemy1,
		Enemy2,
		Enemy3,
		Enemy4
	}; //战机类型

	enum EnemyPlaneVec
	{
		Enemy1_Vec = 100,
		Enemy2_Vec = 80,
		Enemy3_Vec = 50,
		Enemy4_Vec = 30
	}; //战机速度

	enum EnemyLife
	{
		Enemy1_Life = 1,
		Enemy2_Life = 2,
		Enemy3_Life = 5,
		Enemy4_Life = 8
	}; //战机生命

	enum Enemy_Points
	{
		Enemy1_Points = 10,
		Enemy2_Points = 20,
		Enemy3_Points = 50,
		Enemy4_Points = 80
	}; //战机分数

public:
	//自定义工厂方法，由于enemy有很多种，根据enemy的类型初始化，在init里面进行初始化
	static PlaneEnemy* createWithEnemyType(int planetype);
	bool init(int planetype);

	void onEnter();

public:
	void initEnemyBlowUpFrames(int planetype); //加载爆炸纹理图集

	//暂时不做敌机的子弹发射功能
	/*
	void beginShooting(float dt); //开始发射子弹，间隔dt秒发射一发
	void removeBullet(float dt); //移除射到屏幕外面的子弹
	void stopShooting(); //停止发射子弹
	*/
	FiniteTimeAction* getBlowUpAction(); //飞机爆炸
	void getHurt(); //飞机被击中一次
	bool isLive(); //返回飞机是否还活着
	int getPoints();
	int getType() { return m_planeType; }

	void moveOn(float dt); //每dt秒移动一段距离

private:
	int m_velocity; //战机速度
	int m_life; //战机生命
	int m_points;
	int m_planeType; //敌机机型
	bool m_live; //飞机是否活着的标志
	Vector<SpriteFrame*> m_blowframes; //存放爆炸纹理精灵帧
};

#endif

