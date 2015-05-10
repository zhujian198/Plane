#ifndef PLANEHERO_H
#define PLANEHERO_H

#include "cocos2d.h"

USING_NS_CC;

//�ҷ��ɻ���
class PlaneHero : public Sprite
{
public:
	//������������init������г�ʼ��
	CREATE_FUNC(PlaneHero);
	bool init();

	void onEnter();

public:
	void beginShooting(float dt); //��ʼ�����ӵ������dt�뷢��һ��
	void removeBullet(float dt); //�Ƴ��䵽��Ļ������ӵ�
	void stopShooting(); //ֹͣ�����ӵ�
	
	FiniteTimeAction* getBlowUpAction(); //�ɻ���ը
	bool isLive(); //���طɻ��Ƿ񻹻���

private:
	bool m_live; //�ɻ��Ƿ���ŵı�־
};

#endif
