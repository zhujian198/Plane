#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include "cocos2d.h"

USING_NS_CC;

//这里包含游戏的配置信息

//我方战机配置
const char* HERO_PLANE_IMG = "hero2.png"; //根据plist文件，还有hero1.png hero333.png可选
const char* HERO_BULLET_IMG = "bullet1.png";
const float HERO_BULLET_RATIO = 1.0f; //子弹发射速率（多少秒发射一次）
//#define  HERO_BULLET_VEC Vect(0, 500)//我方飞机子弹飞行速率

//敌方战机配置
enum EnemyPlaneType
{
	Enemy1,
	Enemy2,
	Enemy3,
	Enemy4
};

const char* ENEMY1_PLANE_IMG = "enemy1.png";
const char* ENEMY2_PLANE_IMG = "enemy2.png";
const char* ENEMY3_PLANE_IMG = "enemy3_n1.png";
const char* ENEMY4_PLANE_IMG = "enemy3_n2.png";
const char* ENEMY_BULLET_IMG = "bullet2.png";

#endif