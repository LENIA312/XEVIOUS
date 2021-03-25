#include <DxLib.h>
#include "NPCManager.h"
#include "Define.h"

// 各敵のヘッダーファイル-------------------------------------------------------
// 小さい敵------
#include "Terrazi.h"
#include "ZolBak.h"
#include "Toroid.h"
#include "Torken.h"
#include "Barra.h"
#include "Kapi.h"
#include "Derota.h"
#include "Zoshi.h"
#include "Logram.h"
#include "Sheonite.h"
#include "Bozalogram.h"
#include "Domogram.h"
#include "Grobda.h"
#include "Jara.h"
// 大きい敵------
#include "GaruDerota.h"
#include "GaruBarra.h"
#include "Sol.h"
#include "Bacura.h"
// ボス--------
#include "AddorGuileness.h"
// アイテム----
#include "SpecialFlag.h"
// 敵の弾------
#include "Spario.h"
#include "Zakato.h"
#include "GidoSpario.h"
#include "BragZakato.h"
#include "GaruZakato.h"
#include "BragSpario.h"

//------------------------------------------------------------------------------

using namespace std;

/*------------------------------------------------------------------------------
コンストラクタ
------------------------------------------------------------------------------*/
NPCManager::NPCManager(Player* aPlayer)
{
	// 変数
	mDifficulty = 2;
	mNowArea = 0;
	mTortalScore = 0;
	mAddLeft = 0;
	mColor = 4;
	mColorTimer = 0;
	mIsTestPlay = false;

	// オブジェクト
	mPlayer = aPlayer;
	mZapper = aPlayer->getZapper();
	mBlaster = aPlayer->getBlaster();

	// ファイル読み込み
	readNPCFile();
}

/*------------------------------------------------------------------------------
デストラクタ
------------------------------------------------------------------------------*/
NPCManager::~NPCManager()
{
}

/*------------------------------------------------------------------------------
更新
------------------------------------------------------------------------------*/
void NPCManager::update()
{
	// 生成
	generateNPC();
	// 色の変化アニメ
	colorAnime();
	// プレイヤーの座標
	int px = mPlayer->getCollisionPoint(0);
	int py = mPlayer->getCollisionPoint(1);
	int pw = mPlayer->getCollisionPoint(2);
	int ph = mPlayer->getCollisionPoint(3);
	// 照準チカチカリセット
	mPlayer->setTikaTikaFlg(false);
	// NPC更新
	for (int i = 0; i < 2; i++) {
		for (const auto npc : mNPCList[i]) {
			// カメラセット
			npc->setCameraY(mCameraY);
			// エリアの変わり目にy座標の位置を移動
			int ey = npc->getCollisionPoint(1) - 16;
			if (ey <= Define::GameScreenHeight && mCameraY > Define::MapHeight - Define::GameScreenHeight) {
				npc->areaChange();
			}
			// カラーセット
			npc->setColor(mColor);
			// 更新
			npc->update();
		}
		// アクティブフラグが折られていたらリストから敵データを消す
		for (auto npc = mNPCList[i].begin(); npc != mNPCList[i].end();) {
			if ((*npc)->checkActiveFlag() == false) {
				npc = mNPCList[i].erase(npc);
			}
			else {
				npc++;
			}
		}
	}
	// プレイヤーとの当たり判定処理
	if (!mIsTestPlay) {
		collisionPlayer();
	}
}

/*------------------------------------------------------------------------------
描画
------------------------------------------------------------------------------*/
void NPCManager::draw() const
{
	for (int i = 0; i < 2; i++) {
		for (const auto npc : mNPCList[i]) {
			npc->draw();
		}
	}
	// デバッグ	
	if (mIsTestPlay) {
		drawCollision();
	}
	//DrawFormatString(0, 0, GetColor(255, 255, 255), "Camera:%d Sky:%d Ground:%d", (int)mCameraY, mNPCList[1].size(), mNPCList[0].size());
}

/*------------------------------------------------------------------------------
当たり判定の描画
------------------------------------------------------------------------------*/
void NPCManager::drawCollision() const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);

	// NPC
	for (int i = 0; i < 2; i++) {
		for (const auto npc : mNPCList[i]) {
			npc->drawCollision(255,0,0);
		}
	}
	// プレイヤー
	mPlayer->drawCollision(0,255,255);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/*------------------------------------------------------------------------------
プレイヤーとの当たり判定処理
------------------------------------------------------------------------------*/
void NPCManager::collisionPlayer()
{
	// プレイヤーの座標
	int px = mPlayer->getCollisionPoint(0);
	int py = mPlayer->getCollisionPoint(1);
	int pw = mPlayer->getCollisionPoint(2);
	int ph = mPlayer->getCollisionPoint(3);
	// リスト
	for (auto npc : mNPCList[1]) {
		if (!npc->getIsStriken()) {
			int ex = npc->getCollisionPoint(0);
			int ey = npc->getCollisionPoint(1);
			int ew = npc->getCollisionPoint(2);
			int eh = npc->getCollisionPoint(3);
			// プレイヤーが敵に当たった時
			if (px + pw >= ex && px <= ex + ew &&
				py + ph >= ey && py <= ey + eh && mPlayer->checkActiveFlag()) {
				mPlayer->collisionGameObject(npc->getTag());
			}
		}
	}
}

/*------------------------------------------------------------------------------
生成
------------------------------------------------------------------------------*/
void NPCManager::generateNPC()
{
	int d = mDifficulty;
	int a = mNowArea;
	// ファイルの最大数分ループ
	for (int i = 0; i < mNPCDataMax[d][a]; i++) {
		// まだ生成されていなくてカメラの中に入ったら
		if (!mNPCData[d][a][i].flag && mNPCData[d][a][i].y >= mCameraY) {
			// NPCを生成する
			switch (mNPCData[d][a][i].type) {
			case  0: // テラジ
				mNPCList[1].emplace_back(make_shared<Terrazi>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  1: // ゾルバク
				mNPCList[0].emplace_back(make_shared<Zolbak>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  2: // トーロイド
				mNPCList[1].emplace_back(make_shared<Toroid>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  3: // タルケン
				mNPCList[1].emplace_back(make_shared<Torken>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  4: // バーラ
				mNPCList[0].emplace_back(make_shared<Barra>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  5: //	カピ
				mNPCList[1].emplace_back(make_shared<Kapi>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  6: // デロータ
				mNPCList[0].emplace_back(make_shared<Derota>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  7: // ゾシー
				mNPCList[1].emplace_back(make_shared<Zoshi>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  8: // ログラム
				mNPCList[0].emplace_back(make_shared<Logram>(this, mNPCData[d][a][i], mPlayer));
				break;
			case  9: // シオナイト
				mNPCList[1].emplace_back(make_shared<Sheonite>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 10: // ボザログラム
				mNPCList[0].emplace_back(make_shared<Bozalogram>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 11: // ドモグラム
				mNPCList[0].emplace_back(make_shared<Domogram>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 12: // グロブダー
				mNPCList[0].emplace_back(make_shared<Grobda>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 13: // ジアラ
				mNPCList[1].emplace_back(make_shared<Jara>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 14: // ガルデロータ
				mNPCList[0].emplace_back(make_shared<GaruDerota>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 15: // ガルバーラ
				mNPCList[0].emplace_back(make_shared<GaruBarra>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 16: // ソル
				mNPCList[0].emplace_back(make_shared<Sol>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 17: // バキュラ
				mNPCList[1].emplace_back(make_shared<Bacura>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 18: // アンドアジェネシス
				mNPCList[0].emplace_back(make_shared<AddorGuileness>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 19: // スペシャルフラッグ
				mNPCList[0].emplace_back(make_shared<SpecialFlag>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 20: // ザカート
				mNPCList[1].emplace_back(make_shared<Zakato>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 21: // ギドスパリオ
				mNPCList[1].emplace_back(make_shared<GidoSpario>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 22: // ブラグザカート
				mNPCList[1].emplace_back(make_shared<BragZakato>(this, mNPCData[d][a][i], mPlayer));
				break;
			case 23: // ガルザカート
				mNPCList[1].emplace_back(make_shared<GaruZakato>(this, mNPCData[d][a][i], mPlayer));
				break;
			}
			// 次からは生成されないようにフラグを立てる
			mNPCData[d][a][i].flag = true;
		}
	}
}

/*------------------------------------------------------------------------------
敵の弾を生成
------------------------------------------------------------------------------*/
void NPCManager::generateEnemyBullet(int aType, int aPtn, int aX, int aY)
{
	NPCDATA data;
	data.ptn = aPtn;
	data.x = aX;
	data.y = aY;
	switch (aType) {
	case 0: // スパリオ
		mNPCList[1].emplace_back(make_shared<Spario>(this, data, mPlayer));
		break;
	case 1: // ブラグザカートスパリオ
		for (int i = 0; i < 5; i++) {
			data.ptn = i + 1;
			mNPCList[1].emplace_back(make_shared<Spario>(this, data, mPlayer));
		}
		break;
	case 2: // ガルザカート
		for (int i = 0; i < 16; i++) {
			// スパリオ
			data.ptn = i + 6;
			mNPCList[1].emplace_back(make_shared<Spario>(this, data, mPlayer));
			if (i < 4) {
				data.ptn = i;
				mNPCList[1].emplace_back(make_shared<BragSpario>(this, data, mPlayer));
			}
		}
		break;
	}
}

/*------------------------------------------------------------------------------
NPCのファイルを読み込む
------------------------------------------------------------------------------*/
void NPCManager::readNPCFile()
{
	// 変数宣言
	int num;
	int area, areaN;
	int input[64];
	char inputc[64];
	int fp[5];
	fp[0] = FileRead_open("res/file/NPCData_0.csv");
	fp[1] = FileRead_open("res/file/NPCData_1.csv");
	fp[2] = FileRead_open("res/file/NPCData_2.csv");
	fp[3] = FileRead_open("res/file/NPCData_3.csv");
	fp[4] = FileRead_open("res/file/NPCData_4.csv");
	// 5回ファイル読み込み
	for (int i = 0; i < 5; i++) {
		num = 0;
		area = 0;
		areaN = 0;
		while (1) {
			for (int j = 0; j < 64; j++) {
				// 1文字取得する
				inputc[j] = input[j] = FileRead_getc(fp[i]);
				// カンマか改行ならそこまでを文字列とする
				if (input[j] == ',' || input[j] == '\n') {
					inputc[j] = '\0';
					break;
				}
				// ファイルの終わりなら終了
				if (input[j] == EOF) {
					mNPCDataMax[i][area] = areaN;
					goto EXFILE;
				}
				// スラッシュがあれば次のエリアへ
				if (inputc[j] == '/') {
					mNPCDataMax[i][area] = areaN;
					area++;
					while (FileRead_getc(fp[i]) != '\n');
					//カウンタを最初に戻す
					j = -1;
					areaN = 0;
					continue;
				}
			}
			switch (num) {
			case 0: // 敵の種類
				mNPCData[i][area][areaN].type = atoi(inputc);
				break;
			case 1: // 行動パターン
				mNPCData[i][area][areaN].ptn = atoi(inputc);
				break;
			case 2: // x座標
				mNPCData[i][area][areaN].x = atoi(inputc);
				break;
			case 3: // y座標
				mNPCData[i][area][areaN].y = atoi(inputc);
				break;
			}
			// 出現したかどうか確認するフラグ
			mNPCData[i][area][areaN].flag = false;
			num++;
			if (num == 4) {
				num = 0;
				areaN++;
			}
		}
		EXFILE:
		FileRead_close(fp[i]);
	}
}

/*------------------------------------------------------------------------------
色の変化アニメ
------------------------------------------------------------------------------*/
void NPCManager::colorAnime()
{
	mColorTimer++;
	if (mColorTimer <= 20 && mColorTimer % 5 == 0) {
		mColor--;
	}
	if (mColorTimer > 20 && mColorTimer <= 40 && mColorTimer % 5 == 0) {
		mColor++;
	}
	if (mColorTimer > 70) {
		mColorTimer = 0;
	}
}

/*------------------------------------------------------------------------------
リストの中身を空にする
------------------------------------------------------------------------------*/
void NPCManager::clearList()
{
	for (int i = 0; i < 2; i++) {
		for (auto npc = mNPCList[i].begin(); npc != mNPCList[i].end();) {
			if ((*npc)->checkActiveFlag() == true) {
				npc = mNPCList[i].erase(npc);
			}
			else {
				npc++;
			}
		}
	}
	// すべての敵の出現フラグをオフに
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 16; j++) {
			for (int k = 0; k < mNPCDataMax[i][j]; k++) {
				mNPCData[i][j][k].flag = false;
			}
		}
	}
}

/*------------------------------------------------------------------------------
カメラの座標をセット
------------------------------------------------------------------------------*/
void NPCManager::setCamera(float aY)
{
	mCameraY = aY;
}

/*------------------------------------------------------------------------------
現在のエリアをセット
------------------------------------------------------------------------------*/
void NPCManager::setNowArea(int aNowArea)
{
	mNowArea = aNowArea;
}

/*------------------------------------------------------------------------------
現在の難易度をセット
------------------------------------------------------------------------------*/
void NPCManager::setDifficulty(int aDifficulty)
{
	mDifficulty = aDifficulty;
}

/*------------------------------------------------------------------------------
スコアを加算
------------------------------------------------------------------------------*/
void NPCManager::addScore(int aScore)
{
	mTortalScore += aScore;
	if (mTortalScore > 9999990) {
		mTortalScore = 9999990;
	}
}

/*------------------------------------------------------------------------------
残基を加算
------------------------------------------------------------------------------*/
void NPCManager::addLeft(int aLeft)
{
	if (!mIsTestPlay) {
		mAddLeft += aLeft;
	}
	// エディター中ならスコア加算
	else {
		mTortalScore += aLeft;
	}
}

/*------------------------------------------------------------------------------
合計スコアを返す
------------------------------------------------------------------------------*/
int NPCManager::getScore() const
{
	return mTortalScore;
}

/*------------------------------------------------------------------------------
テストプレイモード
------------------------------------------------------------------------------*/
void NPCManager::testPlayModeOn()
{
	mIsTestPlay = true;
}

/*------------------------------------------------------------------------------
NPCリストの数を返す
------------------------------------------------------------------------------*/
int NPCManager::getNPCListSize(int a) const
{
	return mNPCList[a].size();
}

/*------------------------------------------------------------------------------
スコアリセット
------------------------------------------------------------------------------*/
void NPCManager::resetScore()
{
	mTortalScore = 0;
}

/*------------------------------------------------------------------------------
加算する残基を返す
------------------------------------------------------------------------------*/
int NPCManager::getAddLeft() const
{
	return mAddLeft;
}

/*------------------------------------------------------------------------------
残基加算数をリセット
------------------------------------------------------------------------------*/
void NPCManager::resetAddLeft()
{
	mAddLeft = 0;
}