#pragma once

//基盤シーン遷移クラス
//シーン遷移に必要な画面を開閉したり等の処理を行う
//シーン遷移を色々できるように継承前提の設計で作る

class GameSceneManager;

enum class TransitionPhase {
	Close,	//閉じる
	Change,	//変更途中
	Open,	//開く
};


class BaseSceneTransition
{
public:

	//初期化
	virtual void Initialize() = 0;


	//終了
	virtual void Finalize() = 0;

	//更新
	virtual void Update() = 0;

	//描画
	virtual void Draw() = 0;

	//シーン閉じる
	virtual void Close() = 0;

	//シーン開ける
	virtual void Open() = 0;

	//仮想デストラクタ
	virtual ~BaseSceneTransition() = default;

	//遷移フラグ
	bool IsTransition()const { return isTransition; }

	//getter
	TransitionPhase GetPhase()const { return transitionPhase; }

protected:
	
	//シーン遷移フェーズ
	TransitionPhase transitionPhase = TransitionPhase::Close;

	//シーン遷移管理フラグ
	bool isTransition = false;
};

