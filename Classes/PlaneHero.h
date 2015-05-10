#ifndef PLANEHERO_H
#define PLANEHERO_H

#include "cocos2d.h"

USING_NS_CC;

//我方飞机类
class PlaneHero : public Sprite
{
public:
	//工厂方法，在init里面进行初始化
	CREATE_FUNC(PlaneHero);
	bool init();

	void onEnter();

public:
	void beginShooting(float dt); //开始发射子弹，间隔dt秒发射一发
	void removeBullet(float dt); //移除射到屏幕外面的子弹
	void stopShooting(); //停止发射子弹
	
	FiniteTimeAction* getBlowUpAction(); //飞机爆炸
	bool isLive(); //返回飞机是否还活着

private:
	bool m_live; //飞机是否活着的标志
};

#endif

