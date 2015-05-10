#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"

USING_NS_CC;

//游戏主场景层
class GameScene : public Layer
{
//以下是游戏配置信息
public:
	enum NodeTag
	{
		HERO_TAG = 100,
		ENEMY_TAG = 101,
		HERO_BULLET_TAG = 102
	};

	//碰撞事件bit位
	enum ContactMaskBit
	{
		ENEMY_CONTACTMASKBIT = 0x01 || 0x02,
		HERO_BULLET_CONTACTMASKBIT = 0x01,
		HERO_CONTACTMASKBIT = 0x02
	};

	//游戏level，分数越高level越高
	enum Level
	{
		LEVEL1,
		LEVEL2,
		LEVEL3,
		LEVEL4
	};

	//level提升所需的分数
	enum LevelUp_Score
	{
		Level1Up_Score = 10,
		Level2Up_Score = 50,
		Level3Up_Score = 100
	};

	static const float refresh_delay[]; //战机刷新间隔

public:
	static Scene* createScene();
	CREATE_FUNC(GameScene);
	bool init();

	static GameScene* sharedGameLayer() { return m_gamelayer; }
	
public:
	SpriteBatchNode* getBulletBox() { return m_bulletBox; }	

private:
	int m_score; //存放获得的分数
	Label* m_scorelabel; //分数栏

	int m_bossnum; //表示场景里面大型战机的数目，由于太大，一个场景就放一个。关于大型战机数目的清除，采用的是一种不智能的方式，即每隔多少秒重置一次数目，而不是在击落或出界后重置。
	void resetBossNum(float dt) { if (m_bossnum > 0) m_bossnum = 0; }

	int m_level; //当前游戏level
	SpriteBatchNode *m_bulletBox; //存放子弹的一个集合
	static GameScene* m_gamelayer; //游戏场景层的单例对象

	void playBackground();
	void pauseButtonCallBack(Ref* pSender);
	bool dealWithContact(PhysicsContact&); //处理碰撞事件
	void gameover();
	void testLevel(float dt); //每秒调用一次，根据分数设置游戏level
	void refreshAnEnemy(float dt); 
	void publishScore(); //存储最后一次分数
};

#endif

