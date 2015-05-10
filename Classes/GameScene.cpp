#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "PlaneHero.h"
#include "PlaneEnemy.h"
#include "GameOverScene.h"

using namespace CocosDenshion;

GameScene* GameScene::m_gamelayer = nullptr;

const float GameScene::refresh_delay[] = {2.0, 1.5, 1.0, 0.5};

Scene* GameScene::createScene()
{
	//����һ��û����������������
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vect(0, 0));

	auto layer = GameScene::create();
	scene->addChild(layer);

	m_gamelayer = layer;  //����������ֵ

	return scene;
}

bool GameScene::init()
{
	Layer::init(); 
	log("Game init!");

	auto winSize = Director::getInstance()->getWinSize();

	//����plist�ļ�
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("shoot_background.plist");

	//��������
	auto audioengine = SimpleAudioEngine::getInstance();
	audioengine->preloadBackgroundMusic("sound/bgmusic.mp3");
	audioengine->preloadEffect("sound/shoot.wav");
	audioengine->preloadEffect("sound/hero_down.mp3");
	audioengine->preloadEffect("sound/enemy3_down.wav");
	audioengine->preloadEffect("sound/enemy1_down.wav");
	audioengine->preloadEffect("sound/enemy2_down.wav");
	audioengine->preloadEffect("sound/big_spaceship_flying.mp3");

	audioengine->playBackgroundMusic("sound/bgmusic.mp3", true);

	//���ű���
	playBackground();

	//������ͣ��ť
	auto pause_sp1 = Sprite::createWithSpriteFrameName("game_pause_nor.png");
	auto pause_sp2 = Sprite::createWithSpriteFrameName("game_pause_pressed.png");
	auto resume_sp1 = Sprite::createWithSpriteFrameName("game_resume_nor.png");
	auto resume_sp2 = Sprite::createWithSpriteFrameName("game_resume_pressed.png");

	auto pauseitem = MenuItemSprite::create(pause_sp1, pause_sp2);
	auto resumeitem = MenuItemSprite::create(resume_sp1, resume_sp2);
	auto pauseButton = MenuItemToggle::createWithTarget(this, menu_selector(GameScene::pauseButtonCallBack), pauseitem, resumeitem, nullptr);
	pauseButton->setAnchorPoint(Vec2(0, 0));
	pauseButton->setPosition(Vec2(winSize.width / 2 - pauseButton->getContentSize().width, winSize.height / 2 - pauseButton->getContentSize().height)); //λ�����Ͻ�
	
	auto menu = Menu::create(pauseButton, nullptr);
	addChild(menu);

	//����һ��������
	m_score = 0;
	m_scorelabel = Label::createWithSystemFont("Score: 0", "Arial", 36);
	m_scorelabel->setAnchorPoint(Vec2(0, 0));
	m_scorelabel->setPosition(Vec2(0, winSize.height - m_scorelabel->getContentSize().height)); //λ�����Ͻ�
	addChild(m_scorelabel, 10);

	//�����ҷ�ս������ʼλ���趨���·��е�
	auto hero = PlaneHero::create();
	addChild(hero, 0, HERO_TAG);
	hero->setPosition(Vec2(winSize.width / 2, hero->getContentSize().height / 2 + 10));
	auto herobody = PhysicsBody::createBox(hero->getContentSize());
	herobody->setCollisionBitmask(0x0); //��������ײģ��
	herobody->setContactTestBitmask(HERO_CONTACTMASKBIT);
	hero->setPhysicsBody(herobody);

	//�����ӵ���Ⱦ����
	m_bulletBox = SpriteBatchNode::createWithTexture(hero->getTexture()); //�����õ���һ�������ͼ������png��
	addChild(m_bulletBox);

	//����һ�����������������ƶ��ҷ�ս��
	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->setSwallowTouches(true);
	touchlistener->onTouchBegan = [this](Touch *pTouch, Event*) {
		/*
		auto hero = (PlaneHero*)getChildByTag(HERO_TAG);
		if (hero->boundingBox().containsPoint(pTouch->getLocation()))
		{
			//log("choose hero!");
			return true;
		}
		else return false;
		*/
		return true;
	};
	touchlistener->onTouchMoved = [this](Touch* pTouch, Event*) {
		auto delta = pTouch->getDelta() / 5;
		auto hero = (PlaneHero*)getChildByTag(HERO_TAG);
		auto oldpos = hero->getPosition(); //�������ó�����λ��

		hero->setPosition(Vec2(hero->getPosition().x + delta.x, hero->getPosition().y + delta.y));
		
		//���ܳ���
		auto winSize = Director::getInstance()->getWinSize();
		if (hero->getPositionY() < hero->getContentSize().height / 2 || hero->getPositionY() > winSize.height - hero->getContentSize().height / 2
			|| hero->getPositionX() < hero->getContentSize().width / 2 || hero->getPositionX() > winSize.width - hero->getContentSize().width / 2)
		{
			hero->setPosition(oldpos);
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);	

	//������ײ�¼�����
	auto contactlistener = EventListenerPhysicsContact::create();
	contactlistener->onContactBegin = CC_CALLBACK_1(GameScene::dealWithContact, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactlistener, this);	

	//����Level��⣬���ݼ�����Level����ս����ˢ���ٶȺ�ս����ˢ������
	m_level = LEVEL1;
	schedule(schedule_selector(GameScene::testLevel), 1.0f);

	//��ʼˢ�¼��1��
	schedule(schedule_selector(GameScene::refreshAnEnemy), refresh_delay[0]);
	m_bossnum = 0;
	schedule(schedule_selector(GameScene::resetBossNum), 5.0f); //5������һ��bossս����Ŀ

	return true;
}

void GameScene::pauseButtonCallBack(Ref* pSender)
{
	//log("press pause button!");
	static int i = 0;
	if (0 == i)
	{
		i = 1;
		//��ͣ��Ϸ���ر��¼�������Director����ͣ��û��Ϊ���ǹر��¼���������˻���Ҫ�ֶ��رգ�
		_eventDispatcher->pauseEventListenersForTarget(this);
		Director::getInstance()->pause();
	}
	else
	{
		i = 0;
		_eventDispatcher->resumeEventListenersForTarget(this);
		Director::getInstance()->resume();
	}
}

void GameScene::playBackground()
{
	auto winSize = Director::getInstance()->getWinSize();

	int delta = 10; //�����õģ����������������ƶ����ܳ��ַ�϶

	//���ű���
	auto bg1 = Sprite::createWithSpriteFrameName("background.png");
	bg1->setScaleX(winSize.width / bg1->getContentSize().width);
	bg1->setScaleY(winSize.height / bg1->getContentSize().height);
	bg1->setAnchorPoint(Vec2(0, 0));
	addChild(bg1);

	auto move1 = MoveBy::create(30, Vec2(0, -winSize.height));
	auto callfunc1 = CallFunc::create([bg1](){
		bg1->setPosition(Vec2(0, 0));
	});
	auto action1 = Sequence::create(move1, callfunc1, nullptr);
	bg1->runAction(RepeatForever::create(action1));

	auto bg2 = Sprite::createWithSpriteFrameName("background.png");
	bg2->setScaleX(winSize.width / bg2->getContentSize().width);
	bg2->setScaleY(winSize.height / bg2->getContentSize().height);
	bg2->setAnchorPoint(Vec2(0, 0));
	bg2->setPosition(Vec2(0, bg1->getPositionY() + winSize.height - delta));
	addChild(bg2);
	
	auto move2 = MoveTo::create(30, Vec2(0, 0));
	auto callfunc2 = CallFunc::create([bg2, bg1, winSize, delta](){
		bg2->setPosition(Vec2(0, bg1->getPositionY() + winSize.height - delta));
	});
	auto action2 = Sequence::create(move2, callfunc2, nullptr);
	bg2->runAction(RepeatForever::create(action2));
}

bool GameScene::dealWithContact(PhysicsContact& contact)
{
	//log("contact begin!");

	auto node1 = contact.getShapeA()->getBody()->getNode();
	auto node2 = contact.getShapeB()->getBody()->getNode();

	if (!node1 || !node2)
		return false;

	auto tag1 = node1->getTag();
	auto tag2 = node2->getTag();

	//�����ײ�����ӵ��͵з��ɻ�
	if ((tag1 == HERO_BULLET_TAG && tag2 ==ENEMY_TAG) || (tag2 == HERO_BULLET_TAG && tag1 ==ENEMY_TAG))
	{
		PlaneEnemy* enemy = nullptr;
		if (tag1 == HERO_BULLET_TAG)
		{
			node1->removeFromParent();
			enemy =((PlaneEnemy*)node2);
		}
		else
		{
			node2->removeFromParent();
			enemy =((PlaneEnemy*)node1);
		}

		enemy->getHurt();
		
		//����з��ɻ�ûѪ�ˣ����䱬ը
		if (!enemy->isLive())
		{
			//ˢ�·���
			m_score += enemy->getPoints();
			char buf[100] = {0};
			sprintf(buf, "Score: %d", m_score);
			m_scorelabel->setString(buf);

			enemy->getPhysicsBody()->setContactTestBitmask(0x0); //������ײ��־λ�����ٷ�����ײ�¼�
			//�õл���ը��Ȼ���Ƴ�����Ⱦ������Ϊ��������������ִ��
			auto callfunc = CallFunc::create([enemy]()
			{
				enemy->removeFromParent();
			});
			auto blowUpAction = enemy->getBlowUpAction();
			enemy->runAction(Sequence::create(blowUpAction, callfunc, nullptr));
		}
	}

	//�����ײ�����ҷ�ս���͵л����ҷ�ֱ�ӹҵ�
	if ((tag1 == HERO_TAG && tag2 == ENEMY_TAG) || (tag2 == HERO_TAG && tag1 == ENEMY_TAG))
	{
		_eventDispatcher->removeEventListenersForType(EventListener::Type::TOUCH_ONE_BY_ONE); //���ٽ��ܴ����¼�

		PlaneHero *hero = nullptr;
		PlaneEnemy *enemy = nullptr;
		if (tag1 == HERO_TAG)
		{
			hero = ((PlaneHero*)node1);
			enemy = ((PlaneEnemy*)node2);
		}
		else
		{
			hero = ((PlaneHero*)node2);
			enemy = ((PlaneEnemy*)node1);
		}

		hero->getPhysicsBody()->setContactTestBitmask(0x0); //������ײ��־λ�����ٷ�����ײ�¼�

		//�һ���ը��Ȼ���Ƴ�����Ⱦ������Ϊ��������������ִ��
		auto callfunc = CallFunc::create([hero, this]()
		{
			hero->removeFromParent();
			gameover();
		});
		auto blowUpAction = ((PlaneHero*)hero)->getBlowUpAction();
		hero->runAction(Sequence::create(blowUpAction, callfunc, nullptr));

		enemy->getHurt();
		
		//����з��ɻ�ûѪ�ˣ����䱬ը
		if (!enemy->isLive())
		{
			//ˢ�·���
			m_score += enemy->getPoints();
			char buf[100] = {0};
			sprintf(buf, "Score: %d", m_score);
			m_scorelabel->setString(buf);

			enemy->getPhysicsBody()->setContactTestBitmask(0x0); //������ײ��־λ�����ٷ�����ײ�¼�
			//�õл���ը��Ȼ���Ƴ�����Ⱦ������Ϊ��������������ִ��
			auto callfunc = CallFunc::create([enemy]()
			{
				enemy->removeFromParent();
			});
			auto blowUpAction = enemy->getBlowUpAction();
			enemy->runAction(Sequence::create(blowUpAction, callfunc, nullptr));
		}
	}

	return true;
}

void GameScene::gameover()
{
	publishScore(); //�洢������Ϸ����
	auto scene = GameOverScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

void GameScene::testLevel(float dt)
{
	//���ݵ���ķ���������LEVEL
	if (m_score > Level1Up_Score && m_score < Level2Up_Score)
		m_level = LEVEL2;
	else if (m_score > Level2Up_Score && m_score < Level3Up_Score)
		m_level = LEVEL3;
	else if (m_score > Level3Up_Score)
		m_level = LEVEL4;

	//����level����ս��ˢ������
	switch (m_level)
	{
	case Level1Up_Score: 
		m_level = LEVEL2;
		//log("level up!");
		schedule(schedule_selector(GameScene::refreshAnEnemy), refresh_delay[1]); //����ս��ˢ���ٶ�
		break;
	case Level2Up_Score: 
		m_level = LEVEL3;
		//log("level3!");
		schedule(schedule_selector(GameScene::refreshAnEnemy), refresh_delay[2]);
		break;
	case Level3Up_Score: 
		m_level = LEVEL4;
		//log("level4!");
		schedule(schedule_selector(GameScene::refreshAnEnemy), refresh_delay[3]); 
		break;
	}
	
}

void GameScene::refreshAnEnemy(float dt)
{
	//log("refresh an enemy");

	auto winSize = Director::getInstance()->getWinSize();

	int enemy_type = PlaneEnemy::Enemy1;

	switch (m_level)
	{
	case LEVEL1:
		break;
	case LEVEL2:
		enemy_type = random(0, 1);
		break;
	case LEVEL3:
		enemy_type = random(0, 2);
		if (m_bossnum < 1)
			m_bossnum++;
		else enemy_type = random(0, 1);
		break;
	case LEVEL4:
		enemy_type = random(0, 3);
		if (m_bossnum < 1)
			m_bossnum++;
		else enemy_type = random(0, 1);
		break;
	}

	//����ս�����ͼ���ս��
	auto enemy = PlaneEnemy::createWithEnemyType(enemy_type);
	addChild(enemy, 0, ENEMY_TAG);

	//�趨ս����ʼλ�õ�X���ȡֵ��Χ�����������Χ�������ս����ʼX��λ��
	int min = enemy->getContentSize().width / 2;
	int max = winSize.width - enemy->getContentSize().width / 2;
	enemy->setPosition(Vec2(random(min, max), winSize.height + enemy->getContentSize().height / 2));

	//���л�һ��body
	auto enemybody = PhysicsBody::createBox(enemy->getContentSize());
	enemybody->setCollisionBitmask(0x0); //��������ײģ�⣬��Ϊ����Ҫ��
	enemybody->setContactTestBitmask(ContactMaskBit::ENEMY_CONTACTMASKBIT);
	enemy->setPhysicsBody(enemybody);
}

void GameScene::publishScore()
{
	auto userdefault = UserDefault::getInstance();
	
	//�鿴·����������
	//log(userdefault->getXMLFilePath().c_str()); 
	
	//�洢������Ϸ����
	char score_str[100] = {0};
	sprintf(score_str, "%d", m_score);
	userdefault->setStringForKey("LastScore", score_str);

	//�洢�����Ϸ����
	auto bestscore = userdefault->getStringForKey("BestScore");
	if (m_score > atoi(bestscore.c_str()))
		userdefault->setStringForKey("BestScore", score_str);
}