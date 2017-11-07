//--------------------------------------------------------------------------------------
// File: GameState.cpp
//
//--------------------------------------------------------------------------------------
#include "..\..\Global\Global.h"
#include "..\FadeState\FadeState.h"
#include "..\GameState\GameState.h"
#include "..\ResultState\ResultState.h"

#include "..\..\NeuralNetwork\MyNeuralNetwork.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

int GetRandomNumber(int min, int max, bool seed);

//	ニューラルネットワーク---------------------
NeuralNetwork	TheBrain;

//	学習するデータ行数(縦の数)
const int DataNumber = 56;

//	TrainingSet[DataNumber][項目数(横の数)] =
double TrainingSet[DataNumber][18] =
{
	//	出来れば技硬直中を取得できたら硬直に技を降ることができるようになる。
	//	もしくは攻撃なのか防御なのかのステートをCPUが読み込んで状況に応じて有利な技をふるように

	//	距離　0.0は投げ   0.1は5A or 昇龍   0.2は5B or 2A or 2C   0.3は5C  0.4が差し合いの距離　0.5が突進技やジャンプを仕掛ける距離　それ以降は接近する距離で飛び道具持ち以外は単純に空ダなりダッシュ
	//	状態  0.5は立ち、0.2はしゃがんでいる状態
	//	ゲージ 今のところコマンド入力がCPUにできてないので使うか未定

//	距離, 体力, 状態, 時間, (4列)	  5A,　 2A, 　5B, 　6B,   5C, 蹴り, 前歩, 後歩, ダッ, バッ, ガー, 垂ジ, 前ジ, 後ジ, (14列)

	//　プレイヤーの体力が多い、タイムが約7秒以上(7行)
	0.05f, 0.5f, 0.5f, 0.7f,		0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.5f, 0.5f, 0.7f,			0.3f, 0.1f, 0.0f, 0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.1f, 0.2f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.5f, 0.5f, 0.7f,			0.2f, 0.2f, 0.3f, 0.0f, 0.0f, 0.1f, 0.1f, 0.1f, 0.0f, 0.2f, 0.2f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.5f, 0.5f, 0.7f,			0.1f, 0.1f, 0.2f, 0.0f, 0.3f, 0.1f, 0.2f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.5f, 0.5f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.5f, 0.5f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	1.0f, 0.5f, 0.5f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 


	//　プレイヤーの体力が多い、タイムが約7秒以下(7行)
	0.05f, 0.5f, 0.5f, 0.1f,		0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.5f, 0.5f, 0.1f,			0.3f, 0.1f, 0.0f, 0.2f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.1f, 0.2f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.5f, 0.5f, 0.1f,			0.2f, 0.3f, 0.3f, 0.0f, 0.1f, 0.1f, 0.2f, 0.1f, 0.0f, 0.2f, 0.2f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.5f, 0.5f, 0.1f,			0.1f, 0.1f, 0.2f, 0.0f, 0.3f, 0.1f, 0.2f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.5f, 0.5f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.5f, 0.5f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	1.0f, 0.5f, 0.5f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 


	//	プレイヤーの体力がミリ、タイムが約7秒以上(7行)
	0.05f, 0.1f, 0.5f, 0.7f,		0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.1f, 0.5f, 0.7f,			0.3f, 0.1f, 0.0f, 0.2f, 0.0f, 0.1f, 0.0f, 0.1f, 0.1f, 0.1f, 0.3f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.1f, 0.5f, 0.7f,			0.2f, 0.2f, 0.3f, 0.0f, 0.1f, 0.2f, 0.2f, 0.1f, 0.1f, 0.2f, 0.2f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.1f, 0.5f, 0.7f,			0.1f, 0.1f, 0.2f, 0.0f, 0.3f, 0.1f, 0.2f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.1f, 0.5f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.1f, 0.5f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	1.0f, 0.1f, 0.5f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 


	//	プレイヤーの体力がミリ、タイムが約7秒以下(7行)
	0.05f, 0.1f, 0.5f, 0.1f,		0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.1f, 0.5f, 0.1f,			0.2f, 0.1f, 0.0f, 0.2f, 0.0f, 0.4f, 0.0f, 0.1f, 0.0f, 0.0f, 0.3f, 0.2f, 0.0f, 0.1f, 
	0.2f, 0.1f, 0.5f, 0.1f,			0.2f, 0.2f, 0.3f, 0.0f, 0.1f, 0.3f, 0.2f, 0.0f, 0.0f, 0.2f, 0.2f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.1f, 0.5f, 0.1f,			0.1f, 0.1f, 0.2f, 0.0f, 0.3f, 0.2f, 0.1f, 0.2f, 0.0f, 0.2f, 0.1f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.1f, 0.5f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.1f, 0.5f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.4f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.3f, 
	1.0f, 0.1f, 0.5f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.6f, 0.0f, 0.0f, 0.0f, 0.7f, 


	//	------------------------------------------------ ↑立ち　↓しゃがみ

	//	距離　0.0は投げ   0.1は5A or 昇龍   0.2は5B or 2A or 2C   0.3は5C  0.4が差し合いの距離　0.5が突進技やジャンプを仕掛ける距離　それ以降は接近する距離で飛び道具持ち以外は単純に空ダなりダッシュ


//	距離, 体力, 状態, 時間, (4列)	  5A,　 2A, 　5B, 　6B,   5C, 蹴り, 前歩, 後歩, ダッ, バッ, ガー, 垂ジ, 前ジ, 後ジ, (14列)

	//	プレイヤーがしゃがんで体力が多い、タイムが約7秒以上(7行)
	0.05f, 0.5f, 0.2f, 0.7f,		0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.5f, 0.2f, 0.7f,			0.1f, 0.2f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.1f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.5f, 0.2f, 0.7f,			0.1f, 0.3f, 0.3f, 0.1f, 0.1f, 0.0f, 0.1f, 0.1f, 0.0f, 0.2f, 0.2f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.5f, 0.2f, 0.7f,			0.1f, 0.0f, 0.2f, 0.0f, 0.3f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.5f, 0.2f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.5f, 0.2f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	1.0f, 0.5f, 0.2f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 


	//	プレイヤーがしゃがんで体力が多い、タイムが約7秒以下(7行)
	0.05f, 0.5f, 0.2f, 0.1f,		0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.5f, 0.2f, 0.1f,			0.1f, 0.2f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.1f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.5f, 0.2f, 0.1f,			0.1f, 0.3f, 0.3f, 0.1f, 0.1f, 0.0f, 0.1f, 0.1f, 0.0f, 0.2f, 0.2f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.5f, 0.2f, 0.1f,			0.1f, 0.0f, 0.2f, 0.0f, 0.3f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.5f, 0.2f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.5f, 0.2f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	1.0f, 0.5f, 0.2f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 


	//　プレイヤーがしゃがんで体力ミリ、タイムが約7秒以上(7行)
	0.05f, 0.1f, 0.2f, 0.7f,		0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.1f, 0.2f, 0.7f,			0.1f, 0.2f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.1f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.1f, 0.2f, 0.7f,			0.1f, 0.3f, 0.3f, 0.1f, 0.1f, 0.0f, 0.1f, 0.1f, 0.0f, 0.2f, 0.0f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.1f, 0.2f, 0.7f,			0.1f, 0.0f, 0.2f, 0.0f, 0.3f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.1f, 0.2f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.1f, 0.2f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	1.0f, 0.1f, 0.2f, 0.7f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 


	//　プレイヤーがしゃがんで体力ミリ、タイムが約7秒以下(7行)
	0.05f, 0.1f, 0.2f, 0.1f,		0.1f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.1f, 0.1f, 0.2f, 0.1f,			0.1f, 0.2f, 0.0f, 0.3f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.2f, 0.2f, 0.0f, 0.0f, 
	0.2f, 0.1f, 0.2f, 0.1f,			0.1f, 0.3f, 0.3f, 0.1f, 0.1f, 0.0f, 0.1f, 0.2f, 0.0f, 0.2f, 0.0f, 0.0f, 0.2f, 0.0f, 
	0.3f, 0.1f, 0.2f, 0.1f,			0.1f, 0.0f, 0.2f, 0.0f, 0.3f, 0.0f, 0.2f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.0f, 
	0.4f, 0.1f, 0.2f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.2f, 0.0f, 0.2f, 0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	0.5f, 0.1f, 0.2f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.3f, 0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 0.2f, 
	1.0f, 0.1f, 0.2f, 0.1f,			0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, 0.6f, 0.0f, 0.0f, 0.0f, 0.7f, 

//	距離, 体力, 状態, 時間, (4列)	  5A,　 2A, 　5B, 　6B,   5C, 蹴り, 前歩, 後歩, ダッ, バッ, ガー, 垂ジ, 前ジ, 後ジ, (14列)

	//	距離　0.0～0.1までは投げ　0.1は5A　0.2は5B　0.3は5Cが届く距離　0.4が差し合いの距離　0.5が突進技やジャンプを仕掛ける距離　それ以降は接近する距離で飛び道具持ち以外は単純に空ダなりダッシュ
};

int GetRandomNumber(int min, int max, bool seed)
{
	int	number = (((abs(rand()) % (max - min + 1)) + min));
	if (number > max)
		number = max;
	if (number < min)
		number = min;
	return number;
}

void TrainTheBrain(void)
{
	double	error = 1.0;
	int	count = 0;

	//TheBrain.DumpData("PreTraining.txt");

	//	ニューラルネットワークの初期化

	//	誤差、入力値と理想値の差が小さい程、制度が高い証拠

	//	学習の回数　↓
	//	1万回程		テストや実行用
	//	4万回程		割と理想な値にそって動いてくれる。対戦するならこれが一番練習にもなるし対戦になるかも(　推奨値　)
	//	8万回程		プログラム側が最強の値ならこれが一番強くなるがロード(読み込み時間)が非常に長い。あと格ゲーは明確な一つの答えは中々存在しないため決まった動きに完璧に近づける必要がそもそもない。最適解が最も面白い訳ではない。
	while (error > 0.0001 && count < 40000)
	{
		error = 0.0;
		count++;
		for (int i = 0; i < DataNumber + 1; i++)
		{
			//	入力データ
			TheBrain.SetInput(0, TrainingSet[i][0]);
			TheBrain.SetInput(1, TrainingSet[i][1]);
			TheBrain.SetInput(2, TrainingSet[i][2]);
			TheBrain.SetInput(3, TrainingSet[i][3]);
			

			//	出力データ
			TheBrain.SetDesiredOutput(0, TrainingSet[i][4]);
			TheBrain.SetDesiredOutput(1, TrainingSet[i][5]);
			TheBrain.SetDesiredOutput(2, TrainingSet[i][6]);
			TheBrain.SetDesiredOutput(3, TrainingSet[i][7]);
			TheBrain.SetDesiredOutput(4, TrainingSet[i][8]);
			TheBrain.SetDesiredOutput(5, TrainingSet[i][9]);
			TheBrain.SetDesiredOutput(6, TrainingSet[i][10]);
			TheBrain.SetDesiredOutput(7, TrainingSet[i][11]);
			TheBrain.SetDesiredOutput(8, TrainingSet[i][12]);
			TheBrain.SetDesiredOutput(9, TrainingSet[i][13]);
			TheBrain.SetDesiredOutput(10, TrainingSet[i][14]);
			TheBrain.SetDesiredOutput(11, TrainingSet[i][15]);
			TheBrain.SetDesiredOutput(12, TrainingSet[i][16]);
			TheBrain.SetDesiredOutput(13, TrainingSet[i][17]);

			TheBrain.FeedForward();
			error += TheBrain.CalculateError();
			TheBrain.BackPropagate();
		}
		error = error / DataNumber;
	}

}

//--------------------------------------------------------------------------------------
// コンストラクタ
//--------------------------------------------------------------------------------------
GameState::GameState()
	: State(STATE_GAME)		// ステート番号
	, time(GAME_TIME)
	, timespeed(GAME_TIME_SPEED)
	, timecount(0)

	, stop_cnt(0)
	, stop_cnt_max(0)
	, stop_flg(false)

	, game_round(1)
	, round_faze(0)

	, upd_cnt_max(UPDATE_TIME)
	, upd_cnt(0)

	, game_finish_flg(false)
{
	// 絵のロード----------------------------

	// タイム
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/GameTime.png", nullptr, &p_time);

	// 体力ゲージ
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/lifegauge.png", nullptr, &p_gauge_life);

	// パワーゲージ
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/powergauge.png", nullptr, &p_gauge_power);

	// パワーゲージ数値
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/power_number.png", nullptr, &p_gauge_power_num);

	// CBアイコン
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/CBicon.png", nullptr, &p_cb_icon);

	// パワーゲージ上地
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/gauge_02.png", nullptr, &p_gauge_upper);
	// パワーゲージ下部
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/gauge_03.png", nullptr, &p_gauge_under);

	// ライフゲージ上地
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/gauge_01.png", nullptr, &p_life_gauge_upper);
	// ライフゲージ下部
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/gauge_00.png", nullptr, &p_life_gauge_under);

	// コンボ数字
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/combo_number.png", nullptr, &p_combo_num);
	// コンボレイド
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/raid.png", nullptr, &p_combo_raid);
	// コンボバー
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/combo_bar.png", nullptr, &p_combo_bar);
	// コンボダメージ
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/damage.png", nullptr, &p_combo_damage);
	// コンボダメージナンバー
	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/Gauge/damage_number.png", nullptr, &p_combo_damage_num);

	CreateWICTextureFromFile(g_pd3dDevice, g_pImmediateContext, L"Data/Image/CRACK ARENA/Game/RoundCall/Gamecall//string_debug.png", nullptr, &p_ko_debug);



	// タスク登録
	p_camera = new Camera();

	p_battlecall = new BattleCall( AIN_CLOMA , AIN_CLOMA );

	p_roundcall = new RoundCall( 1 );

	p_winnercall = new WinnerCall();

	p_effectsystem = new EffectSystem();

	p_cutin = new CutInSystem();

	// 背景のエントリー
	p_background = new BackGround( AIN_CLOMA , p_camera );

	// BGMロード
	bgm.Load(L"Data/Sound/Character/Ain_Cloma/mo.mp3");
	//bgm.Play();

	// キャラクターの登録
	p_Player1P = new Ain_Cloma_Base( p_gauge_life , p_gauge_power , p_gauge_power_num , p_cb_icon , p_gauge_upper , p_gauge_under , p_combo_num , p_combo_raid , p_combo_bar , p_combo_damage , p_combo_damage_num , PLAYER_1P , CHARACTER_COLOR_1 , p_camera , p_background , p_effectsystem, p_cutin, p_winnercall );
	p_Player2P = new Ain_Cloma_Base( p_gauge_life , p_gauge_power , p_gauge_power_num, p_cb_icon, p_gauge_upper, p_gauge_under, p_combo_num, p_combo_raid, p_combo_bar, p_combo_damage, p_combo_damage_num, PLAYER_2P, CHARACTER_COLOR_2, p_camera, p_background , p_effectsystem, p_cutin, p_winnercall);

	p_Player1P->EnemyPointerGetter( p_Player2P );
	p_Player2P->EnemyPointerGetter( p_Player1P );

	// キャラクターのラウンドコール用タスクを登録
	p_Player1P_rw = new Ain_RW( CHARACTER_COLOR_1 , PLAYER_1P , RIGHT , p_Player1P->PositionGetterX() , p_Player1P->PositionGetterY() , 1 , 2 );
	p_Player2P_rw = new Ain_RW( CHARACTER_COLOR_1 , PLAYER_2P , LEFT , p_Player2P->PositionGetterX() , p_Player2P->PositionGetterY(), 1 , 2 );
	
}


//--------------------------------------------------------------------------------------
// デストラクタ
//--------------------------------------------------------------------------------------
GameState::~GameState()
{
	if (p_time) p_time->Release();

	if (p_gauge_life) p_gauge_life->Release();
	if (p_gauge_power) p_gauge_power->Release();
	if (p_gauge_power_num) p_gauge_power_num->Release();
	if (p_cb_icon) p_cb_icon->Release();

	if (p_gauge_upper) p_gauge_upper->Release();
	if (p_gauge_under) p_gauge_under->Release();

	if (p_life_gauge_upper) p_life_gauge_upper->Release();
	if (p_life_gauge_under) p_life_gauge_under->Release();

	if (p_combo_bar) p_combo_bar->Release();
	if (p_combo_damage) p_combo_damage->Release();
	if (p_combo_damage_num) p_combo_damage_num->Release();
	if (p_combo_num) p_combo_num->Release();
	if (p_combo_raid) p_combo_raid->Release();
	
	if (p_ko_debug) p_ko_debug->Release();


	delete p_Player1P_rw;
	delete p_Player2P_rw;

	delete p_cutin;

	delete p_camera;
	delete p_background;
	delete p_effectsystem;

	delete p_battlecall;
	delete p_roundcall;
	delete p_winnercall;

	delete p_Player1P;
	delete p_Player2P;

	bgm.Unload();
}


//--------------------------------------------------------------------------------------
// 初期化
//--------------------------------------------------------------------------------------
void GameState::Initialize()
{
	//	ニューラルネットの学習する関数
	TheBrain.Initialize(4, 14, 14);		// 入力数, 調整数, 出力数
	TheBrain.SetLearningRate(0.9);		// 学習率 0～1
	TheBrain.SetMomentum(true, 0.9);	// 0～1;
	TrainTheBrain();
}


//--------------------------------------------------------------------------------------
// 更新
//--------------------------------------------------------------------------------------
void GameState::Update()
{
	// ラウンドフェイズ0 ( キャラクターアピール ) ---------------
	if (round_faze == 0){
		// ラウンドコール時のキャラクター更新
		p_Player1P_rw->Update();
		p_Player2P_rw->Update();
			
		// 1P2P がアピールを終了したら 
		if ( p_Player1P_rw->NextFlg() == true && p_Player2P_rw->NextFlg() == true )
		round_faze = 1;

		// ボタンが入力されたら、処理を飛ばす
		if (g_InputManager->IsKeyDown('D') || g_InputManager->IsButtonDown(XINPUT_GAMEPAD_X))
		{
			// 終了状態
			round_faze = 1;
		}
	}

	// ラウンドフェイズ1 ( バトルコール ) -----------------------
	else if (round_faze == 1){
		//　キャラクター更新
		p_Player1P_rw->Update();
		p_Player2P_rw->Update();

		// バトルコールの更新
		p_battlecall->Update();

		// バトルコールが終了していれば次へ
		if (p_battlecall->NextFlg() == true)
		{
			round_faze = 2;
		}

		// ボタンが入力されたら、処理を飛ばす
		if (g_InputManager->IsKeyDown('D') || g_InputManager->IsButtonDown(XINPUT_GAMEPAD_X))
		{
			// 終了状態
			round_faze = 2;
		}
	}

	// ラウンドフェイズ2 および 3 ---------------------
	else if (round_faze >= 2 && round_faze <= 4 ){

		// ラウンドフェイズ2 ( ラウンドコール ) ---------------------
		if (round_faze == 2){

			// ラウンドコールの更新
			p_roundcall->Update();

			// ラウンドコールが終了していれば次へ
			if (p_roundcall->NextFlg() == true)
			{
				round_faze = 3;
			}
		}

		// ラウンドフェイズ3 ( 対戦 ) -------------------------------
		else if (round_faze >= 3 && round_faze <= 4){

			// キャラクターの更新
			CharacterUpdate();

			// タイムの更新
			TimeUpDate();

			// どちらかの体力が0になっていたら
			if (p_Player1P->LifeGetter() <= 0 || p_Player2P->LifeGetter() <= 0)
			{
				// フェイズが3ならば
				if (round_faze == 3){
					// ヒットストップさせる
					stop_flg = true;
					stop_cnt = 0;
					stop_cnt_max = GAME_STOP_CNT_MAX;
					// 背景を振動させる
					p_background->BackVibration(50, 10);
					round_faze = 4;
				}

				// フェイズが4ならば
				else if (round_faze == 4) {

					if (stop_cnt == 0){
						// 超必殺技演出終了
						p_background->EndSuperMove();

						// キャラクターをそれぞれ決着状態にする
						// ダブルダウン
						if (p_Player1P->LifeGetter() <= 0 && p_Player2P->LifeGetter() <= 0){
							p_Player1P->StateChanger(AIN_GAME_LOSE);
							p_Player2P->StateChanger(AIN_GAME_LOSE);

							// ドローの時はゲーム勝利にならないようにウィンカウントを加算する
							if (p_Player1P->GetWinCount() <= GAME_ROUND_NUM - 2){
								p_Player1P->SetWinCount();
							}
							if (p_Player2P->GetWinCount() <= GAME_ROUND_NUM - 2){
								p_Player2P->SetWinCount();
							}
						}
						else if (p_Player1P->LifeGetter() <= 0 && p_Player2P->LifeGetter() >= 1){
							p_Player1P->StateChanger(AIN_GAME_LOSE);
							p_Player2P->StateChanger(AIN_GAME_WIN);
						}
						else if (p_Player2P->LifeGetter() <= 0 && p_Player1P->LifeGetter() >= 1){
							p_Player2P->StateChanger(AIN_GAME_LOSE);
							p_Player1P->StateChanger(AIN_GAME_WIN);
						}
						// フェイズを進める
						round_faze = 5;
					}
				}
			}
		}
	}
	// ラウンドフェイズ4 ( 決着 ) -----------------------
	else if (round_faze == 5){
		// ウイナーコール更新
		p_winnercall->UpDate();

		// キャラクターの更新
		CharacterUpdate();

		// タイムの更新
		TimeUpDate();
		// ウイナーコールが終わっていれば次のラウンドへ
		if ((p_winnercall->GetNextFlg() == true)
			|| (p_Player1P->GetRoundDownFlg() == true && p_Player2P->GetRoundDownFlg() == true)){

			// どちらかが先行先取していたら
			if (p_Player1P->GetWinCount() >= GAME_ROUND_NUM || p_Player2P->GetWinCount() >= GAME_ROUND_NUM){
				round_faze = 6;
			}
			else{
				//キャラクターのステータス初期化
				p_Player1P->RoundInitialize();
				p_Player2P->RoundInitialize();

				// 次ラウンドへ
				round_faze = 2;

				// 背景の初期化
				p_background->Initialize();

				// ファイナルラウンドになるかどうかの判断
				if ((p_Player1P->GetWinCount() >= GAME_ROUND_NUM - 1 && p_Player2P->GetWinCount() >= GAME_ROUND_NUM - 1)
					|| (game_round == GAME_ROUND_NUM + 1)){
					game_round = 4;
				}
				game_round++;
				// ラウンドコール初期化
				p_roundcall->Initialize(game_round);
			}
		}
	}
	// ラウンドフェイズ4 ( ゲーム勝利 ) -----------------------
	else if (round_faze == 6){
		// リザルトへ遷移
		g_pStateManager->PushState(new FadeInState(new ResultState()));
	}

	//カメラ更新
	p_camera->Update();
	
	// エフェクト更新
	p_effectsystem->Update();

	// 背景更新
	p_background->Update();
	p_background->BVUpdate();


	//	ニューラルネットワーク//////////////////////////////////////////////////////////////////////////////////////////////

	//	実際に出てくるデータ
	cout << fixed << setprecision(3) << TheBrain.GetOutput(0) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(1) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(2) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(3) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(4) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(5) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(6) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(7) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(8) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(9) << std::endl;

	cout << fixed << setprecision(3) << TheBrain.GetOutput(10) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(11) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(12) << std::endl;
	cout << fixed << setprecision(3) << TheBrain.GetOutput(13) << std::endl;


	//	---------------------------------------------------------	↓	AIの行動条件を判断する為に各値を取得しています。

	//	入力値等を検証する時によく使ってた
	/*if (time <= 73)
	{
	time--;
	}*/


	//	decision は　条件って意味です。

	//	プレイヤー1と2の距離
	float distance_decision;
	//	1Pの位置から2Pの位置を引いて、距離を出す。
	distance_decision = fabs(p_Player1P->PositionGetterX() - p_Player2P->PositionGetterX());
	//	判断を細かくさせる為に、距離(数値)がそれ以上離れているときは数値を代入する。700ドット以上の間合いは全て同じ判断をするように
	if (distance_decision >= 700)
	{
		distance_decision = 1000.0f;
	}
	distance_decision = distance_decision / 1000.0f;		//	1000で割って0～1までの値を割り出す。
	//	距離が0.7を越えたら距離が1.0離れている時と同じ処理をするように
	if (distance_decision >= 0.6)
	{
		distance_decision = 1.0f;
	}


	//	プレイヤーのライフを取得
	float life_decision;
	life_decision = p_Player1P->LifeGetter();				//	数値を取得
	life_decision = life_decision / 600.0f;					//	体力MAX400 初期400で割ってたが、精度を高める為に上限数字を変更。
	if (life_decision >= 0.4)
	{
		life_decision = 1.0f;
	}


	//	プレイヤーの状態の取得(立ち、しゃがみ等)			standなら0.5などにする必要がある	行動状態　MOVE_STAND Common.h
	float playerstate_decision;
	//playerstate_decision = p_Player1P->StateGetter();		//	状態の取得
	//	しゃがみ状態
	if (p_Player1P->StateGetter() == MOVE_SQUAT || p_Player1P->StateGetter() == MOVE_ATTACK_LOW)
	{
		//float data_a = p_Player1P->StateGetter();
		playerstate_decision = 0.2f;
	}
	//	立ち状態
	if (p_Player1P->StateGetter() == MOVE_STAND)
	{
		playerstate_decision = 0.5f;
	}
	//	ジャンプ中
	if (p_Player1P->StateGetter() == MOVE_AIR)
	{
		playerstate_decision = 0.8f;
	}
	//	それ以外の時
	else
	{
		playerstate_decision = 0.5f;
	}


	//	ゲーム中の時間の取得
	float gametimecount_decision;
	gametimecount_decision = time;							//	タイムの数値を取得
	gametimecount_decision = gametimecount_decision / 77;	//	? 10で割った方がいいのかな？	77秒がマックス

	//	ゲージの量の取得
	//float gaugeamount_decision;
	//gaugeamount_decision = p_Player1P->GetPlayerGauge();	//	取得してます！
	//gaugeamount_decision = gaugeamount_decision / 100;		//	ゲージの数値をニューラルネットワークで使う0.1やら0.5とかの数値として使えるように割ってます。
	//if (gaugeamount_decision > 0.5)
	//{
	//	gaugeamount_decision = 1.0;
	//}



	//	硬直中の時間を取得 したかった


	//	行動をニューラルネットワークに入力	(入力番号,調べたい値)
	TheBrain.SetInput(0, distance_decision);				//	距離入力
	TheBrain.SetInput(1, life_decision);					//	体力入力
	TheBrain.SetInput(2, playerstate_decision);				//	1Pのキャラ状態を入力
	TheBrain.SetInput(3, gametimecount_decision);			//	試合中のタイムを入力
	//TheBrain.SetInput(4, gaugeamount_decision);			//	ゲージの入力

	//	FeedForward()を実行し、CPUの状態を決定する
	TheBrain.FeedForward();									//	学習させた値


	//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	// AI_の行動条件分岐
	//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	//	移動や攻撃の分岐　//
	//	いくつかの出力の値で一番大きい値が、これらの状態を選ぶ
	switch (TheBrain.GetMaxOutputID())
	{

		//	・・・A技・・・  //
	case 0:	//	5A
		// キー入力状態を"A"にする
		p_Player2P->SetAiKeyInputNum(KEY_A);
		// キーの入力状態をアクティブにする
		p_Player2P->SetAiKeyInputActive();
		break;

	case 1:	//	2A
		//	下入力
		p_Player2P->SetAiLeverInputNum(AI_LEVER_DOWN);
		// キー入力状態を"A"にする
		p_Player2P->SetAiKeyInputNum(KEY_A);
		// キーの入力状態をアクティブにする
		p_Player2P->SetAiKeyInputActive();
		break;


		//	・・・B技・・・  //
	case 2:	//	5B
		// キー入力状態を"B"にする
		p_Player2P->SetAiKeyInputNum(KEY_B);
		// キーの入力状態をアクティブにする
		p_Player2P->SetAiKeyInputActive();
		break;

	case 3:	//	6B
		//	右向きの時
		if (p_Player2P->GetPlayerDirection() == RIGHT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_RIGHT);
		}

		//	左向きの時
		if (p_Player2P->GetPlayerDirection() == LEFT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_LEFT);
		}
		// キー入力状態を"B"にする
		p_Player2P->SetAiKeyInputNum(KEY_B);
		// キーの入力状態をアクティブにする
		p_Player2P->SetAiKeyInputActive();
		break;



		//	・・・C技・・・  //
	case 4:	//	5C
		// キー入力状態を"C"にする
		p_Player2P->SetAiKeyInputNum(KEY_C);
		// キーの入力状態をアクティブにする
		p_Player2P->SetAiKeyInputActive();
		break;

		//	・・・コマンド技・・・  //
	case 5:	//	蹴り飛ばす じゃないです
		// キー入力状態を"C"にする
		p_Player2P->SetAiKeyInputNum(KEY_C);
		// キーの入力状態をアクティブにする
		p_Player2P->SetAiKeyInputActive();
		break;

		//	・・・行動・・・  //
	case 6:	//	前歩き
		//	右向きの時
		if (p_Player2P->GetPlayerDirection() == RIGHT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_RIGHT);
		}

		//	左向きの時
		if (p_Player2P->GetPlayerDirection() == LEFT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_LEFT);
		}
		break;


	case 7:	//	後ろ歩き
		//	右向きの時
		if (p_Player2P->GetPlayerDirection() == RIGHT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_LEFT);
		}

		//	左向きの時
		if (p_Player2P->GetPlayerDirection() == LEFT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_RIGHT);
		}
		break;


	case 8:	//	ダッシュ
		p_Player2P->SetAiLeverInputNum(AI_LEVER_DASH);
		break;


	case 9:	//	バックステップ
		p_Player2P->SetAiLeverInputNum(AI_LEVER_BACKSTEP);
		break;

	case 10:	//	ガード
		//p_Player2P->SetAiLeverInputNum(AI_LEVER_GUARD);

		//	右向きの時
		if (p_Player2P->GetPlayerDirection() == RIGHT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_LEFT);
		}

		//	左向きの時
		if (p_Player2P->GetPlayerDirection() == LEFT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_RIGHT);
		}
		break;

	case 11:	//	垂直ジャンプ
		p_Player2P->SetAiLeverInputNum(AI_LEVER_UP);
		break;

	case 12:	//	前ジャンプ
		//	右向きの時
		if (p_Player2P->GetPlayerDirection() == RIGHT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_UPRIGHT);
		}
		//	左向きの時
		if (p_Player2P->GetPlayerDirection() == LEFT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_UPLEFT);
		}
		break;

	case 13:	//	後ろジャンプ
		//	右向きの時
		if (p_Player2P->GetPlayerDirection() == RIGHT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_UPLEFT);
		}
		//	左向きの時
		if (p_Player2P->GetPlayerDirection() == LEFT)
		{
			p_Player2P->SetAiLeverInputNum(AI_LEVER_UPRIGHT);
		}
		break;

	default:
		break;
	}
}


//--------------------------------------------------------------------------------------
// 描画
//--------------------------------------------------------------------------------------
void GameState::Draw()
{

	// ラウンドフェイズ0 ( キャラクターアピール ) ---------------
	if (round_faze == 0){
		// ラウンドコール時のキャラクター表示
		p_Player1P_rw->Draw();
		p_Player2P_rw->Draw();
	}

	// ラウンドフェイズ1 ( バトルコール ) -----------------------
	else if (round_faze == 1){
		// ラウンドコール時のキャラクター表示
		p_Player1P_rw->Draw();
		p_Player2P_rw->Draw();

		// バトルコール描画
		p_battlecall->Draw();
	}

	// ラウンドフェイズ2 ( ラウンドコール ) ---------------------
	else if (round_faze == 2){
		// ラウンドコール時のキャラクター表示
		p_Player1P_rw->Draw();
		p_Player2P_rw->Draw();

		// ラウンドコール描画
		p_roundcall->Draw();
	}

	// ラウンドフェイズ3 ( 対戦 ) -------------------------------
	else if (round_faze >= 3 && round_faze <= 4){

		// 表示
		p_Player1P->Draw();
		p_Player2P->Draw();

		// タイムの表示
		TimeDraw();
	}
	// ラウンドフェイズ4 ( KO ) -----------------------
	else if (round_faze == 5){
		// ウイナーコール描画
		p_winnercall->Draw();

		// 表示
		p_Player1P->Draw();
		p_Player2P->Draw();

		// タイムの表示
		TimeDraw();

		//// レクト型更新(切り出す大きさや位置)
		//RECT rect = { (LONG)(0), (LONG)(0), (LONG)(1120), (LONG)(174) };

		//// 表示
		//g_Sprites->Draw(
		//	p_ko_debug,														// テクスチャハンドル
		//	Vector2(0, 228.0f),					// 表示座標
		//	&rect,																// 切り出す矩形
		//	Vector4(1.0f, 1.0f, 1.0f, 1.0f),														// 頂点カラー
		//	0.0f,																// 回転角(ラジアン)
		//	Vector2(0.0f, 0.0f),								// 絵の頂点
		//	1.0f,																// スケール
		//	SpriteEffects_None,													// フリップ(反転効果)
		//	0.33f);														// 深度( 0.0f ～ 1.0f )
	}
	

	// エフェクト表示
	p_effectsystem->Draw();

	// カットイン描画
	if (p_cutin->GetLifeFlg() == true){
		p_cutin->Draw();
	}

	// 背景表示
	p_background->Draw();

	
}

//--------------------------------------------------------------------------------------
// ステートが切り替わる時に呼び出される関数
//--------------------------------------------------------------------------------------
void GameState::StateChanged( StateManager *stateManager )
{
	// 現在実行中のステートなら更新と描画をする
	if( stateManager->GetCurrentStateNo() == stateNo )
	{
		SetAttr( TASK_ATTR_UPDATE | TASK_ATTR_DRAW, 0 );
	}
	else {
		SetAttr( 0, TASK_ATTR_UPDATE );
	}
}

//--------------------------------------------------------------------------------------
// タイムアップデート
//--------------------------------------------------------------------------------------
void GameState::TimeUpDate()
{
	
	timecount++;
	if (timecount >= timespeed)
	{
		time--;
		timecount = 0;
	}

	if (time < 0)
	{
		time = GAME_TIME;
	}

}

//--------------------------------------------------------------------------------------
// タイムドロー
//--------------------------------------------------------------------------------------
void GameState::TimeDraw()
{
	int tmp1, tmp2;
	tmp1 = time / 10;
	tmp2 = time - (tmp1 * 10);

	g_AnimationSystem->Animation(p_time, 533.0f, 25.0f, (float)(28), (float)(40), (float)(tmp1), 0.0f, 1.0f, 0.04f, true);
	g_AnimationSystem->Animation(p_time, 559.0f, 25.0f, (float)(28), (float)(40), (float)(tmp2), 0.0f, 1.0f, 0.04f, true);

	g_AnimationSystem->Animation(p_life_gauge_under, 0.0f, 0.0f, 1120.0f, 173.0f, 0.0f, 0.0f, 1.0f, 0.01f, true);
	g_AnimationSystem->Animation(p_life_gauge_upper, 0.0f, 0.0f, 1120.0f, 170.0f, 0.0f, 0.0f, 1.0f, 0.02f, true);

}

//--------------------------------------------------------------------------------------
// プレイヤーの更新
//--------------------------------------------------------------------------------------
void GameState::CharacterUpdate()
{
	
	// プレイヤーの入力処理を受け付ける。
	p_Player1P->KeyInputUpdate();
	p_Player2P->KeyInputUpdate();

	// プレイヤーのレバーの入力処理の更新
	p_Player1P->LeverInputUpdate();
	p_Player2P->LeverInputUpdate();

	

	// 更新を遅くする
	if (upd_cnt < upd_cnt_max){
		upd_cnt++;
	}
	else{
		// デバッグ変数
		upd_cnt = 0;

		// ヒットストップが起きていなければ更新を行う。
		if (stop_cnt == 0){

			// プレイヤーの更新
			p_Player1P->Update();
			p_Player2P->Update();

			

			// 攻撃があたっているかどうかの判断
			if (stop_flg == false){
				p_Player1P->HitGetUpdate();
				stop_flg = p_Player1P->HitStopFlgGetter();
				if (stop_flg == true){ stop_cnt_max += p_Player1P->HitStopCntGetter(); }
			}
			if (stop_flg == false){
				p_Player2P->HitGetUpdate();
				stop_flg = p_Player2P->HitStopFlgGetter();
				if (stop_flg == true){ stop_cnt_max += p_Player2P->HitStopCntGetter(); }
			}

			// 攻撃があたっていたら、食らい処理を行う
			p_Player1P->HitSetUpdate();
			p_Player2P->HitSetUpdate();

		}

	}

	// ゲージの更新
	p_Player1P->GaugeUpdate();
	p_Player2P->GaugeUpdate();

	// カットインの更新
	if (p_cutin->GetLifeFlg() == true){
		stop_cnt_max = 10;
		stop_cnt = 0;
		stop_flg = true;
		p_cutin->UpDate();
	}

	// ヒットストップの更新を行う
	if (stop_flg == true)
	{
		stop_cnt++;
		if (stop_cnt >= stop_cnt_max)
		{
			stop_flg = false;
			stop_cnt_max = 0;
			stop_cnt = 0;
		}
	}



}
