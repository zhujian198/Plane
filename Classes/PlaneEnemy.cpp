#include "PlaneEnemy.h" 
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

PlaneEnemy* PlaneEnemy::createWithEnemyType(int planetype)
{
	//自定义一个工厂方法，传入一个精灵帧的名字，该帧一定是在游戏进入时候加载完毕的
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
	
	//初始化纹理图，战机速度
	String framename;
	switch (planetype)
	{
	case EnemyPlaneType::Enemy1:
		framename = "enemy1.png";
		m_velocity = random(0.5, 1.5) * Enemy1_Vec; //设置一个随机速度
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

	//加载敌人爆炸精灵帧集合
	initEnemyBlowUpFrames(planetype);

	/*
	 *暂时不做子弹功能功能
	//初始化子弹发射器，它是一个SpriteBatchNode
	bulletBox = SpriteBatchNode::createWithTexture(getTexture()); //这里获得的是一张纹理大图（整张png）
	//下面两个代码设置了发射器的位置，位于飞机底部中间
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
	 *暂时不做敌机发射功能

	//进入后，开始发射子弹，每HERO_BULLET_RATIO秒发射一发
	schedule(schedule_selector(PlaneEnemy::beginShooting), HERO_BULLET_RATIO);
	//检查子弹是否出界，出界则清除出bulletBox
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
 *暂时不做子弹发射功能

void PlaneEnemy::beginShooting(float dt)
{
	//log("a bullet shoot!");
	auto bullet = Sprite::createWithSpriteFrameName(HERO_BULLET_IMG);
	bulletBox->addChild(bullet);

	//给子弹套一个body，加一个初始速度，让其射到物理世界中
	auto body = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(0, 0, 0));
	body->setm_velocity(HERO_BULLET_VEC); 

	bullet->setPhysicsBody(body);
}

void PlaneEnemy::removeBullet(float dt)
{
	//遍历每一颗子弹，把出界的删除
	auto vec = bulletBox->getChildren();
	for (auto &bullet : vec)
	{
		auto posInNode = bullet->getPosition();
		//由于bullet处于bulletBox的模型坐标系，因此要转化成世界坐标系
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

	//加载死亡动画
	auto animation = Animation::createWithSpriteFrames(m_blowframes);
	animation->setDelayPerUnit(0.2);
	animation->setRestoreOriginalFrame(true);
	auto blowUp = Animate::create(animation);

	return blowUp;
}

void PlaneEnemy::moveOn(float dt)
{
	//log("enemy moveon!");

	//根据行进速度，往下移动，一旦出界，清除出去
	this->setPositionY(getPositionY() - dt * m_velocity);
	if (getPositionY() < -this->getContentSize().height / 2)
	{
		removeFromParent();
		//log("enemy out!");
	}
}