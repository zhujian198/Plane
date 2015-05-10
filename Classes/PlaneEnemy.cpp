#include "PlaneEnemy.h" 
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

PlaneEnemy* PlaneEnemy::createWithEnemyType(int planetype)
{
	//�Զ���һ����������������һ������֡�����֣���֡һ��������Ϸ����ʱ�������ϵ�
	auto plane = new PlaneEnemy();
	if (plane && plane->init(planetype))
	{
		plane->autorelease();
		return plane;
	}
	else
	{
		CC_SAFE_DELETE(plane);
		return nullptr;
	}
}

bool PlaneEnemy::init(int planetype)
{
	Sprite::init();

	m_planeType = planetype;
	m_live = true;
	
	//��ʼ������ͼ��ս���ٶ�
	String framename;
	switch (planetype)
	{
	case EnemyPlaneType::Enemy1:
		framename = "enemy1.png";
		m_velocity = random(0.5, 1.5) * Enemy1_Vec; //����һ������ٶ�
		m_life = Enemy1_Life;
		m_points = Enemy1_Points;
		break;
	case EnemyPlaneType::Enemy2:
		framename = "enemy2.png";
		m_velocity = random(0.5, 1.5) * Enemy2_Vec;
		m_life = Enemy2_Life;
		m_points = Enemy2_Points;
		break;
	case EnemyPlaneType::Enemy3:
		framename = "enemy3_n1.png";
		m_velocity =  random(0.5, 1.5) * Enemy3_Vec;
		m_life = Enemy3_Life;
		m_points = Enemy3_Points;
		break;
	case EnemyPlaneType::Enemy4:
		framename = "enemy3_n2.png";
		m_velocity = random(0.5, 1.5) * Enemy4_Vec;
		m_life = Enemy4_Life;
		m_points = Enemy4_Points;
		break;
	}

	initWithSpriteFrameName(framename.getCString());

	//���ص��˱�ը����֡����
	initEnemyBlowUpFrames(planetype);

	/*
	 *��ʱ�����ӵ����ܹ���
	//��ʼ���ӵ�������������һ��SpriteBatchNode
	bulletBox = SpriteBatchNode::createWithTexture(getTexture()); //�����õ���һ�������ͼ������png��
	//�����������������˷�������λ�ã�λ�ڷɻ��ײ��м�
	bulletBox->setPosition(Vec2(getContentSize().width / 2, 0));
	bulletBox->setAnchorPoint(Vec2(0, 0));
	addChild(bulletBox);
	*/

	return true;
}

void PlaneEnemy::onEnter()
{
	Sprite::onEnter();

	/*
	 *��ʱ�����л����书��

	//����󣬿�ʼ�����ӵ���ÿHERO_BULLET_RATIO�뷢��һ��
	schedule(schedule_selector(PlaneEnemy::beginShooting), HERO_BULLET_RATIO);
	//����ӵ��Ƿ���磬�����������bulletBox
	schedule(schedule_selector(PlaneEnemy::removeBullet), 0.1);
	
	*/

	schedule(schedule_selector(PlaneEnemy::moveOn));
}

void PlaneEnemy::initEnemyBlowUpFrames(int planetype)
{
	if (planetype == EnemyPlaneType::Enemy1)
	{
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down1.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down2.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy1_down3.png"));
	}
	
	else if (planetype == EnemyPlaneType::Enemy2)
	{
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down1.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down2.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down3.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy2_down4.png"));
	}

	else
	{
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down1.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down2.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down3.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down4.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down5.png"));
		m_blowframes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("enemy3_down6.png"));
	}
}

/* 
 *��ʱ�����ӵ����书��

void PlaneEnemy::beginShooting(float dt)
{
	//log("a bullet shoot!");
	auto bullet = Sprite::createWithSpriteFrameName(HERO_BULLET_IMG);
	bulletBox->addChild(bullet);

	//���ӵ���һ��body����һ����ʼ�ٶȣ������䵽����������
	auto body = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(0, 0, 0));
	body->setm_velocity(HERO_BULLET_VEC); 

	bullet->setPhysicsBody(body);
}

void PlaneEnemy::removeBullet(float dt)
{
	//����ÿһ���ӵ����ѳ����ɾ��
	auto vec = bulletBox->getChildren();
	for (auto &bullet : vec)
	{
		auto posInNode = bullet->getPosition();
		//����bullet����bulletBox��ģ������ϵ�����Ҫת������������ϵ
		auto posInWorld = bulletBox->convertToWorldSpace(posInNode);
		if (posInWorld.y > Director::getInstance()->getVisibleSize().height)
		{
			bulletBox->removeChild(bullet, true);
			//log("a bullet remove!");
		}
	}
}

void PlaneEnemy::stopShooting()
{
	unschedule(schedule_selector(PlaneEnemy::beginShooting));
}
*/

bool PlaneEnemy::isLive()
{
	return m_live;
}

int PlaneEnemy::getPoints()
{
	return m_points;
}

void PlaneEnemy::getHurt()
{
	m_life--;
	if (m_life == 0)
	{
		m_live = false;
		return;
	}
}

FiniteTimeAction* PlaneEnemy::getBlowUpAction()
{
	switch (m_planeType)
	{
	case Enemy1:
		SimpleAudioEngine::getInstance()->playEffect("sound/enemy1_down.wav");
		break;
	case Enemy2:
		SimpleAudioEngine::getInstance()->playEffect("sound/enemy2_down.wav");
		break;
	case Enemy3:
		SimpleAudioEngine::getInstance()->playEffect("sound/enemy3_down.wav");
		break;
	case Enemy4:
		SimpleAudioEngine::getInstance()->playEffect("sound/enemy3_down.wav");
		break;
	}

	//������������
	auto animation = Animation::createWithSpriteFrames(m_blowframes);
	animation->setDelayPerUnit(0.2);
	animation->setRestoreOriginalFrame(true);
	auto blowUp = Animate::create(animation);

	return blowUp;
}

void PlaneEnemy::moveOn(float dt)
{
	//log("enemy moveon!");

	//�����н��ٶȣ������ƶ���һ�����磬�����ȥ
	this->setPositionY(getPositionY() - dt * m_velocity);
	if (getPositionY() < -this->getContentSize().height / 2)
	{
		removeFromParent();
		//log("enemy out!");
	}
}