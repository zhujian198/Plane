#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"

USING_NS_CC;

class PlaneEnemy;
class PlaneHero;

//��Ϸ��������
class GameScene : public Layer
{
//��������Ϸ������Ϣ
public:
	enum NodeTag
	{
		HERO_TAG = 100,
		ENEMY_TAG = 101,
		HERO_BULLET_TAG = 102,
		SCORE_LABEL = 103,
		PAUSE_MENU = 104
	};

	//��ײ�¼�bitλ
	enum ContactMaskBit
	{
		ENEMY_CONTACTMASKBIT = 0x01 || 0x02,
		HERO_BULLET_CONTACTMASKBIT = 0x01,
		HERO_CONTACTMASKBIT = 0x02
	};

	//��Ϸlevel������Խ��levelԽ��
	enum Level
	{
		LEVEL1,
		LEVEL2,
		LEVEL3,
		LEVEL4
	};

	//level��������ķ���
	enum LevelUp_Score
	{
		Level1Up_Score = 50,
		Level2Up_Score = 200,
		Level3Up_Score = 500
	};

	static const float refresh_delay[]; //ս��ˢ�¼��

public:
	static Scene* createScene();
	CREATE_FUNC(GameScene);
	bool init();
	
public:
	static GameScene* sharedGameLayer() { return m_gamelayer; } //��ȡ����Ϸ�㵥������

private:
	static GameScene* m_gamelayer; //��Ϸ������ĵ�������

public:
	SpriteBatchNode* getBulletBox() { return m_bulletBox; }	 //��ȡ�ӵ���Ⱦ��

private:
	SpriteBatchNode *m_bulletBox; //����ӵ�����Ⱦ��

private:
	int m_level; //��ǰ��Ϸlevel
	int m_score; //��ǰ��Ϸ����

private:
	void playBackground(); //�������ƶ���Ϸ����
	void publishScore(); //�洢��Ϸ����
	void gameover();

	void pauseButtonCallBack(Ref* pSender); //�����ͣ��ť�ص�����

	bool dealWithContact(PhysicsContact&); //������ײ�¼��ص�����
	void hitEnemy(PlaneEnemy* enemy);
	void hitHero(PlaneHero* hero);

	//��Ϸ��schedule�Ļص�����
	void testLevel(float dt); //ÿ�����һ�Σ����ݵ�ǰ����ķ���������Ϸlevel
	void refreshAnEnemy(float dt); //ˢ��һ�ܵз�ս��

	//m_canBossRefresh��ʾ�ɷ�ˢ��boss��ÿ��dt�����ˢ��һ��
	bool m_canBossRefresh; 
	void resetBoss(float dt) { if (!m_canBossRefresh) m_canBossRefresh = true; }
};

#endif

