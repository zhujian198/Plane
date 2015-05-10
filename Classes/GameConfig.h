#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include "cocos2d.h"

USING_NS_CC;

//���������Ϸ��������Ϣ

//�ҷ�ս������
const char* HERO_PLANE_IMG = "hero2.png"; //����plist�ļ�������hero1.png hero333.png��ѡ
const char* HERO_BULLET_IMG = "bullet1.png";
const float HERO_BULLET_RATIO = 1.0f; //�ӵ��������ʣ������뷢��һ�Σ�
//#define  HERO_BULLET_VEC Vect(0, 500)//�ҷ��ɻ��ӵ���������

//�з�ս������
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