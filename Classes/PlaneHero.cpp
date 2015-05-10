#include "PlaneHero.h"'
#include "GameScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

bool PlaneHero::init()
{
	Sprite::init();

	m_live = true;
	
	//��ʼ������ͼ
	initWithSpriteFrameName("hero2.png");

	return true;
}

void PlaneHero::onEnter()
{
	Sprite::onEnter();

	//����󣬿�ʼ�����ӵ�
	schedule(schedule_selector(PlaneHero::beginShooting), .5f);
	//����ӵ��Ƿ���磬�����������bulletBox
	schedule(schedule_selector(PlaneHero::removeBullet), 0.1);
}

void PlaneHero::beginShooting(float dt)
{
	SimpleAudioEngine::getInstance()->playEffect("sound/shoot.wav");

	//log("a bullet shoot!");
	//����һ���ӵ������뵽GameLayer���ӵ�������
	auto bullet = Sprite::createWithSpriteFrameName("bullet1.png");
	bullet->setPosition(Vec2(getPositionX(), getPositionY() + getContentSize().height / 2));
	auto bulletBox = GameScene::sharedGameLayer()->getBulletBox();
	
	bulletBox->addChild(bullet, 0, GameScene::HERO_BULLET_TAG);

	//���ӵ���һ��body����һ����ʼ�ٶȣ������䵽����������
	auto body = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(0, 0, 0));
	body->setVelocity(Vect(0, 500)); 
	body->setContactTestBitmask(GameScene::ContactMaskBit::HERO_BULLET_CONTACTMASKBIT);
	body->setCollisionBitmask(0x0); //��������ײģ�⣬��Ϊ����Ҫ��

	bullet->setPhysicsBody(body);
}

void PlaneHero::removeBullet(float dt)
{
	//����ÿһ���ӵ����ѳ����ɾ��
	auto bulletBox = GameScene::sharedGameLayer()->getBulletBox();
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

void PlaneHero::stopShooting()
{
	unschedule(schedule_selector(PlaneHero::beginShooting));
}

bool PlaneHero::isLive()
{
	return m_live;
}

FiniteTimeAction* PlaneHero::getBlowUpAction()
{
	this->stopShooting();
	SimpleAudioEngine::getInstance()->playEffect("sound/hero_down.mp3");

	//������������
	auto animation = Animation::create();
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n1.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n2.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n3.png"));
	animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("hero_blowup_n4.png"));
	animation->setDelayPerUnit(0.2);
	animation->setRestoreOriginalFrame(true);

	auto animate = Animate::create(animation);
	
	//��ը�����������
	auto callfunc = CallFunc::create([this](){
		this->m_live = false;
	});
	
	auto blowUp = Sequence::create(animate, callfunc, nullptr);
	return blowUp;
}