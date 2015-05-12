#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "PlaneHero.h"
#include "PlaneEnemy.h"
#include "GameOverScene.h"

using namespace CocosDenshion;

GameScene* GameScene::m_gamelayer = nullptr;

const float GameScene::refresh_delay[] = {2.0, 1.5, 1.0, 0.5}; //ս��ˢ�¼��

Scene* GameScene::createScene()
{
	//����һ��û����������������
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vect(0, 0));

	//������Ի�ͼ
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

	m_gamelayer = GameScene::create();
	scene->addChild(m_gamelayer);

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

	//�������֣����ű�������
	auto audioengine = SimpleAudioEngine::getInstance();
	audioengine->preloadBackgroundMusic("sound/bgmusic.mp3");
	audioengine->preloadEffect("sound/shoot.wav");
	audioengine->preloadEffect("sound/hero_down.mp3");
	audioengine->preloadEffect("sound/enemy3_down.wav");
	audioengine->preloadEffect("sound/enemy1_down.wav");
	audioengine->preloadEffect("sound/enemy2_down.wav");
	audioengine->preloadEffect("sound/big_spaceship_flying.mp3");

	audioengine->playBackgroundMusic("sound/bgmusic.mp3", true);

	//���ű���ͼ
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
	addChild(menu, 10, PAUSE_MENU);

	//����һ��������
	m_score = 0;
	auto scorelabel = Label::createWithSystemFont("Score: 0", "Arial", 36);
	scorelabel->setAnchorPoint(Vec2(0, 0));
	scorelabel->setPosition(Vec2(0, winSize.height - scorelabel->getContentSize().height)); //λ�����Ͻ�
	addChild(scorelabel, 10, SCORE_LABEL);

	//�����ҷ�ս������ʼλ���趨���·��е�
	auto hero = PlaneHero::create();
	addChild(hero, 0, HERO_TAG);
	hero->setPosition(Vec2(winSize.width / 2, hero->getContentSize().height / 2 + 10));
	//auto herobody = PhysicsBody::createBox(hero->getContentSize()); //�������ò�̫��׼
	auto herobody = PhysicsBody::create();
	Vec2 verts[] = {Vec2(0, 55), Vec2(50, -30), Vec2(-50, -30)};  //���ݵ����һ�������
	herobody->addShape(PhysicsShapeEdgePolygon::create(verts, 3));
	herobody->setCollisionBitmask(0x0); //��������ײģ��
	herobody->setContactTestBitmask(HERO_CONTACTMASKBIT);
	hero->setPhysicsBody(herobody);

	//����һ�����������������ƶ��ҷ�ս��
	auto touchlistener = EventListenerTouchOneByOne::create();
	touchlistener->setSwallowTouches(true);
	touchlistener->onTouchBegan = [this](Touch *pTouch, Event*) {
		//�����ŷɻ��ƶ�
		auto hero = (PlaneHero*)getChildByTag(HERO_TAG);
		if (hero->boundingBox().containsPoint(pTouch->getLocation()))
		{
			//log("choose hero!");
			return true;
		}
		else return false;
	};
	touchlistener->onTouchMoved = [this](Touch* pTouch, Event*) {
		auto delta = pTouch->getDelta();
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

	//�����ӵ���Ⱦ����
	m_bulletBox = SpriteBatchNode::createWithTexture(hero->getTexture()); //�����õ���һ�������ͼ������png��
	addChild(m_bulletBox);

	//������ײ�¼�����
	auto contactlistener = EventListenerPhysicsContact::create();
	contactlistener->onContactBegin = CC_CALLBACK_1(GameScene::dealWithContact, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactlistener, this);	

	//����Level��⣬���ݼ�����Level����ս����ˢ���ٶ�
	m_level = LEVEL1;
	schedule(schedule_selector(GameScene::testLevel), 1.0f);

	//���ó�ʼ�л�ˢ�¼������ʼˢ��
	schedule(schedule_selector(GameScene::refreshAnEnemy), refresh_delay[LEVEL1]);
	
	//ÿ��5�����ˢ��һ��bossս��
	m_canBossRefresh = true;
	schedule(schedule_selector(GameScene::resetBoss), 5.0f); 

	return true;
}

void GameScene::playBackground()
{
	auto winSize = Director::getInstance()->getWinSize();

	int delta = 10; //�����õģ����������������ƶ����ܳ��ַ�϶

	//���ű���������������ͼƬ��һ���ģ�������ѭ������
	//ע��㣺1.�ֲ�ͼ�����ظ�Ӧ�ñ��豸�Ĵ�һ�� 2.�����ͼӦ����΢��ǰ��һ�� �����Ͳ����з�϶��
	auto bg1 = Sprite::createWithSpriteFrameName("background.png");
	bg1->setScaleX(winSize.width / bg1->getContentSize().width);
	bg1->setScaleY(winSize.height / bg1->getContentSize().height + 0.01);
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
	bg2->setScaleY(winSize.height / bg2->getContentSize().height + 0.01);
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

void GameScene::gameover()
{
	publishScore(); //�洢��Ϸ����
	auto scene = TransitionFade::create(2.0, GameOverScene::createScene());
	Director::getInstance()->replaceScene(scene);
}

void GameScene::pauseButtonCallBack(Ref* pSender)
{
	//log("press pause button!");

	/*
	 *  ��ͣ/���� ���裺
	 *1.�ر�/���� ��������
	 *2.�����ӽڵ㣬�ѳ�����ͣ��ť�� �ر�/���� ��Ⱦ(onExit)
	 *3.�ر�/���� �л���ˢ��schedule
	 *4.�ر�/���� ���������ģ��
	*/
	static int i = 0;
	if (0 == i)
	{
		i = 1;

		_eventDispatcher->pauseEventListenersForTarget(this);
		
		auto vec = this->getChildren();
		for (auto &child : vec)
		{
			if (child->getTag() != PAUSE_MENU)
				child->onExit();
		}

		unschedule(schedule_selector(GameScene::refreshAnEnemy));

		((Scene*)this->getParent())->getPhysicsWorld()->setAutoStep(false);
		
	}
	else
	{
		i = 0;

		_eventDispatcher->resumeEventListenersForTarget(this);
		
		auto vec = this->getChildren();
		for (auto &child : vec)
		{
			if (child->getTag() != PAUSE_MENU)
				child->onEnter();
		}

		schedule(schedule_selector(GameScene::refreshAnEnemy), refresh_delay[m_level]);

		((Scene*)this->getParent())->getPhysicsWorld()->setAutoStep(true);
	}
}

bool GameScene::dealWithContact(PhysicsContact& contact)
{
	//log("contact begin!");
	//��ȡ������ײ�Ľڵ�
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

		hitEnemy(enemy);
	}

	//�����ײ�����ҷ�ս���͵л�
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

		hitEnemy(enemy);
		hitHero(hero);
	}

	return true;
}

void GameScene::hitEnemy(PlaneEnemy* enemy)
{
	enemy->getHurt();
		
	//����з��ɻ����ˣ����ӷ����������Ƴ�����Ⱦ��
	if (!enemy->isLive())
	{
		m_score += enemy->getPoints();
		char buf[100] = {0};
		sprintf(buf, "Score: %d", m_score);
		auto scorelabel = (Label*)this->getChildByTag(SCORE_LABEL);
		scorelabel->setString(buf);
		
		//���ڵз��ɻ���ը��һ��ʱ�䣬��������ɾ���з��ɻ������ִ��һ���ӳٶ������ȴ�1.2����ɾ���ɻ�
		auto node = Node::create();
		addChild(node);

		auto waitBlowUp = DelayTime::create(1.2);
		auto clearEnemy = CallFunc::create([enemy, node]() {
			enemy->removeFromParent();
			node->removeFromParent();
			//log("enemy cleared!");
		});
			
		node->runAction(Sequence::create(waitBlowUp, clearEnemy, nullptr));
	}
}

void GameScene::hitHero(PlaneHero* hero)
{
	//�һ�ֱ���������ȴ���ըʱ�����������ҷ�ս����������Ϸ
	hero->dead();

	auto node = Node::create();
	addChild(node);

	auto clearHero = CallFunc::create([this]() {
		this->removeChildByTag(HERO_TAG);
	});

	auto gameoverCall = CallFunc::create([this]() {
		//log("call gameover!");
		this->gameover();
	});

	node->runAction(Sequence::create(DelayTime::create(1.5), clearHero, gameoverCall, nullptr));
}

void GameScene::testLevel(float dt)
{
	int oldlevel = m_level;

	//���ݵ���ķ���������LEVEL
	if (m_score > Level1Up_Score && m_score < Level2Up_Score)
		m_level = LEVEL2;
	else if (m_score > Level2Up_Score && m_score < Level3Up_Score)
		m_level = LEVEL3;
	else if (m_score > Level3Up_Score)
		m_level = LEVEL4;

	//���levelû�仯������Ҫ����ˢ���ٶ�
	if (oldlevel == m_level)
		return;

	//����level����ս��ˢ������
	switch (m_level)
	{
	case Level1Up_Score: 
		m_level = LEVEL2;
		//log("level up!");
		break;
	case Level2Up_Score: 
		m_level = LEVEL3;
		//log("level3!");
		break;
	case Level3Up_Score: 
		m_level = LEVEL4;
		//log("level4!");
		break;
	}
	
	schedule(schedule_selector(GameScene::refreshAnEnemy), refresh_delay[m_level]); //����ս��ˢ���ٶ�
}

void GameScene::refreshAnEnemy(float dt)
{
	//log("refresh an enemy");

	auto winSize = Director::getInstance()->getWinSize();

	int enemy_type = PlaneEnemy::Enemy1;

	//������ϷLevelѡ�������ˢ�²�ͬ�����ս��
	switch (m_level)
	{
	case LEVEL1:
		break;
	case LEVEL2:
		enemy_type = random(0, 1);
		break;
	case LEVEL3:
		enemy_type = random(0, 2);
		if (m_canBossRefresh && enemy_type == 2) //���ˢ�³�bossս������ô��ֹ�����ˢ�£�ֱ���ý�ֹ��־������
			m_canBossRefresh = false;
		else enemy_type = random(0, 1);
		break;
	case LEVEL4:
		enemy_type = random(0, 3);
		if (m_canBossRefresh && enemy_type >= 2)
			m_canBossRefresh = false;
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
	Vec2 vec[10]; //��ŵз�ս���Ķ���ε�
	memset(vec, 0, sizeof(vec));
	int vec_count = 0;
	switch (enemy_type)
	{
	case PlaneEnemy::Enemy1:
		vec[0] = Vec2(0, -17);
		vec[1] = Vec2(-22, 5);
		vec[2] = Vec2(22, 5);
		vec_count = 3;
		break;
	case PlaneEnemy::Enemy2:
		vec[0] = Vec2(0, -40);
		vec[1] = Vec2(-40, 0);
		vec[2] = Vec2(0, 45);
		vec[3] = Vec2(40, 0);
		vec_count = 4;
		break;
	default:
		vec[0] = Vec2(50, -100);
		vec[1] = Vec2(-50, -100);
		vec[2] = Vec2(-54, -85);
		vec[3] = Vec2(-54, 90);
		vec[4] = Vec2(54, 90);
		vec[5] = Vec2(67, -88);
		vec_count = 6;
		break;
	}

	//auto enemybody = PhysicsBody::createBox(enemy->getContentSize());
	auto enemybody = PhysicsBody::create();
	enemybody->addShape(PhysicsShapePolygon::create(vec, vec_count));
	enemybody->setCollisionBitmask(0x0); //��������ײģ�⣬��Ϊ����Ҫ
	enemybody->setContactTestBitmask(ContactMaskBit::ENEMY_CONTACTMASKBIT);
	enemy->setPhysicsBody(enemybody);
}