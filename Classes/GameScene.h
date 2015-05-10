#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "cocos2d.h"

USING_NS_CC;

//��Ϸ��������
class GameScene : public Layer
{
//��������Ϸ������Ϣ
public:
	enum NodeTag
	{
		HERO_TAG = 100,
		ENEMY_TAG = 101,
		HERO_BULLET_TAG = 102
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
		Level1Up_Score = 10,
		Level2Up_Score = 50,
		Level3Up_Score = 100
	};

	static const float refresh_delay[]; //ս��ˢ�¼��

public:
	static Scene* createScene();
	CREATE_FUNC(GameScene);
	bool init();

	static GameScene* sharedGameLayer() { return m_gamelayer; }
	
public:
	SpriteBatchNode* getBulletBox() { return m_bulletBox; }	

private:
	int m_score; //��Ż�õķ���
	Label* m_scorelabel; //������

	int m_bossnum; //��ʾ�����������ս������Ŀ������̫��һ�������ͷ�һ�������ڴ���ս����Ŀ����������õ���һ�ֲ����ܵķ�ʽ����ÿ������������һ����Ŀ���������ڻ�����������á�
	void resetBossNum(float dt) { if (m_bossnum > 0) m_bossnum = 0; }

	int m_level; //��ǰ��Ϸlevel
	SpriteBatchNode *m_bulletBox; //����ӵ���һ������
	static GameScene* m_gamelayer; //��Ϸ������ĵ�������

	void playBackground();
	void pauseButtonCallBack(Ref* pSender);
	bool dealWithContact(PhysicsContact&); //������ײ�¼�
	void gameover();
	void testLevel(float dt); //ÿ�����һ�Σ����ݷ���������Ϸlevel
	void refreshAnEnemy(float dt); 
	void publishScore(); //�洢���һ�η���
};

#endif

