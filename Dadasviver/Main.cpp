# include <Siv3D.hpp> // OpenSiv3D v0.6.11d

using App = SceneManager<String>;

struct Unit
{
	double x;
	double y;
	int HP;
	int co;//当たり判定の大きさ(円)
	int Eattack;//敵の当たった時の攻撃力
	bool enable;//有効フラグ
	int inv;	//無敵時間
	int exp;	//経験値
	int level;	//レベル

	double vx;	//x軸の移動量
	double vy;	//y軸の移動量
};
struct Weapon
{
	double x;
	double y;
	double vx;
	double vy;
	int co;		//当たり判定の大きさ(円)
	int atk;	//武器の攻撃力
	bool enable;//有効フラグ
	int level;	//武器レベル
	int wm;		//武器の向き
};

const int slimenum = 100;	//スライムの最大数
const int knifenum = 100;	//ナイフの最大数


Unit abe;
Unit slime[slimenum];
Unit dia[slimenum];
Weapon knife[knifenum];
Weapon katana;
Weapon syuriken;

void init();	//初期化設定
void player();	//自機
void weapon();	//武器
void Knife();	//ナイフ
void Katana();	//刀
void Syuriken();//手裏剣
void enemy();	//敵機
void Levelup();	//レベルアップ時の演出
//void draw();	//描画
void direction();//自機の向き


bool collision(Unit a, Unit b);	//当たり判定関数
bool weaponcollsion(Weapon a, Unit b);	//武器の当たり判定関数

int muki = 0;	//自機の向き
int wct = 0;	//武器のクールタイム
int delay = 0;	//斜めに向いたときディレイをかける
int wno = 0;	//武器のナンバー
int Destroy = 0;  //倒した数


bool pause = false;		//ポーズ機能
bool levelup = false;	//レベルアップ演出
int select = 0;			//レベルアップボーナス選択用

int rolling = 0;

double t;

double theta[4];


class Title : public App::Scene // タイトルシーン
{
public:
	Title(const InitData& init)
		:IScene{ init }
	{}
	~Title()
	{}
	void update()  override
	{
		// 左クリックで
		if (MouseL.down() || KeyEnter.down())
		{
			// ゲームシーンに遷移
			changeScene(U"Game");
		}
	}
	void draw()const override
	{
		TextureAsset(U"OP1").draw(0, 0);

	}
};
class Game : public App::Scene
{
public:

	Game(const InitData& init)
		: IScene{ init }

	{}

	~Game()
	{}


	void update()
	{

		if (pause == false && levelup == false)
		{
			//時間
			t = Scene::Time();
			direction();
			player();
			enemy();
			weapon();
			if (Destroy >= 10)
			{
				changeScene(U"WResult");
			}
			if (abe.enable == false)
			{
				// ゲームシーンに遷移
				changeScene(U"LResult");
			}
		}
		//draw();

		if (levelup == true && pause == false)
		{
			Levelup();
		}

		if (KeyEnter.down())
		{
			if (pause == true)
			{
				pause = false;
			}
			else if (pause == false)
			{
				pause = true;
			}
		}

	}
	void draw()const override
	{
		TextureAsset(U"map").draw(0, 0);
		if (abe.enable == true)
		{
			//Circle{ abe.x,abe.y,abe.co }.draw(ColorF{ 1.0,1.0,1.0 });
			TextureAsset(U"abeimg").draw(abe.x - 15, abe.y - 20);
		}

		for (int i = 0; i < slimenum; i++)
		{
			if (slime[i].enable == true)
			{
				//Circle{ slime[i].x,slime[i].y,slime[i].co }.draw(ColorF{ 1.0,0.0,0.0 });

				if (abe.x <= slime[i].x)
				{
					TextureAsset(U"slimeimg").draw(slime[i].x - 20, slime[i].y - 30);
				}
				if (abe.x > slime[i].x)
				{
					TextureAsset(U"slimeimg").mirrored().draw(slime[i].x - 20, slime[i].y - 30);
				}
				for (int j = 0; j < knifenum; j++)
				{

					if (weaponcollsion(knife[j], slime[i]))
					{
						FontAsset(U"Reggae One")(U"10!!"_fmt(knife[j].atk)).
							drawAt({ slime[i].x,slime[i].y - 30 }, ColorF{ 100.0,0.0,0.0 });
					}
				}

			}

			if (dia[i].enable == true)
			{
				//Circle{ dia[i].x,dia[i].y,dia[i].co }.draw(ColorF{ 0.0,1.0,0.0 });
				TextureAsset(U"diaimg").draw(dia[i].x - 13, dia[i].y - 13);
			}
		}

		for (int i = 0; i < knifenum; i++)
		{
			if (knife[i].enable == true)
			{
				//Circle{ knife[i].x,knife[i].y,knife[i].co }.draw(ColorF{ 0.0,1.0,0.0 });

				if (knife[i].wm == 0)
				{
					TextureAsset(U"knifeimg").rotated(135_deg).drawAt(knife[i].x - 10, knife[i].y);
				}
				if (knife[i].wm == 1)
				{
					TextureAsset(U"knifeimg").rotated(315_deg).draw(knife[i].x, knife[i].y - 12);
				}
				if (knife[i].wm == 2)
				{
					TextureAsset(U"knifeimg").rotated(225_deg).draw(knife[i].x - 12, knife[i].y - 20);
				}
				if (knife[i].wm == 3)
				{
					TextureAsset(U"knifeimg").rotated(45_deg).draw(knife[i].x - 12, knife[i].y);
				}
				if (knife[i].wm == 4)
				{
					TextureAsset(U"knifeimg").rotated(90_deg).draw(knife[i].x - 20, knife[i].y - 4);
				}
				if (knife[i].wm == 5)
				{
					TextureAsset(U"knifeimg").rotated(180_deg).draw(knife[i].x - 20, knife[i].y - 20);
				}
				if (knife[i].wm == 6)
				{
					TextureAsset(U"knifeimg").rotated(270_deg).draw(knife[i].x - 3, knife[i].y - 20);
				}
				if (knife[i].wm == 7)
				{
					TextureAsset(U"knifeimg").draw(knife[i].x - 3, knife[i].y - 3);
				}

			}
		}

		if (katana.enable == true)
		{
			//Circle{ katana.x,katana.y,katana.co }.draw(ColorF{ 0.0,1.0,0.0 });
			TextureAsset(U"katanaimg").rotated(rolling * 8_deg).draw(katana.x - 37, katana.y - 25);
		}

		for (int i = 0; i < syuriken.level; i++)
		{
			//手裏剣の描画
			Circle{ OffsetCircular{ Vec2{syuriken.x,syuriken.y}, 100, theta[i]}, syuriken.co }.draw(ColorF{ 0.25 });
		}

		//自機のHPゲージ
		RectF{ abe.x - 20,abe.y + 20,abe.HP / 25,10 }.draw(ColorF{ 0.0,0.0,1.0 });

		//expゲージ
		Rect{ 100, 30, 600, 30 }.shearedX(10).draw(ColorF{ 0.0,0.0,0.0 });
		Rect{ 100, 30, abe.exp, 30 }.shearedX(10).draw(ColorF{ 0.3,1.0,0.3 });
		Rect{ 100, 30, 600, 30 }.shearedX(10).drawFrame(2);
		FontAsset(U"Reggae One")(U"Level.{}"_fmt(abe.level)).drawAt({ 150, 15 }, ColorF{ 0.0,0.0,0.0 });
		if (wno == 0)
		{
			TextureAsset(U"knifeicon").draw(10, 0);
		}
		if (wno == 1)
		{
			TextureAsset(U"katanaicon").draw(10, 0);
		}

		if (levelup == true)
		{
			if (select != 0)
			{
				RoundRect{ 100, 200, 180, 225, 20 }.drawShadow(Vec2{ 2, 2 }, 8, 0).draw(ColorF{ 0.0,0.7,0.0 });
			}
			else if (select == 0)
			{
				RoundRect{ 100, 200, 180, 225, 20 }.drawShadow(Vec2{ 2, 2 }, 8, 0).draw(ColorF{ 0.8,0.9,1.0 });
			}
			if (select != 1)
			{
				RoundRect{ 310, 200, 180, 225, 20 }.drawShadow(Vec2{ 2, 2 }, 8, 0).draw(ColorF{ 0.0,0.7,0.0 });
			}
			else if (select == 1)
			{
				RoundRect{ 310, 200, 180, 225, 20 }.drawShadow(Vec2{ 2, 2 }, 8, 0).draw(ColorF{ 0.8,0.9,1.0 });
			}
			if (select != 2)
			{
				RoundRect{ 520, 200, 180, 225, 20 }.drawShadow(Vec2{ 2, 2 }, 8, 0).draw(ColorF{ 0.0,0.7,0.0 });
			}
			else if (select == 2)
			{
				RoundRect{ 520, 200, 180, 225, 20 }.drawShadow(Vec2{ 2, 2 }, 8, 0).draw(ColorF{ 0.8,0.9,1.0 });
			}

		}

		if (pause == true)
		{
			Rect{ 0, 0, 800, 600 }.draw(ColorF{ 0.0,0.0,0.0,0.3 });
		}


	}


};
class LResult :public App::Scene//負けた時のエンディング
{
public:
	LResult(const InitData& init)
		:IScene{ init }
	{}
	~LResult() {}

	int r = Random(2);
	void update() override
	{


		if (r == 0)
		{
			TextureAsset(U"ED1").draw(0, 0);

			if (MouseL.down())
			{
				// ゲームシーンに遷移
				changeScene(U"Title");
			}
		}
		else if (r == 1) {
			TextureAsset(U"ED2").draw(0, 0);
			if (MouseL.down())
			{
				// ゲームシーンに遷移
				changeScene(U"Title");
			}
		}
		else if (r == 2) {
			TextureAsset(U"ED3").draw(0, 0);
			if (MouseL.down())
			{
				// ゲームシーンに遷移
				changeScene(U"Title");
			}
		}
	}
	void draw() const override
	{
		TextureAsset::Register(U"ED1", U"Ed_Lose1.jpg");
		TextureAsset::Register(U"ED2", U"Ed_Lose2.jpg");
		TextureAsset::Register(U"ED3", U"Ed_Lose3.jpg");
	}
};
class WResult :public App::Scene//負けた時のエンディング
{
public:
	WResult(const InitData& init)
		:IScene{ init }
	{}
	~WResult() {}

	int r = Random(2);
	void update() override
	{


		if (r == 0)
		{
			TextureAsset(U"ED4").draw(0, 0);

			if (MouseL.down())
			{
				// ゲームシーンに遷移
				changeScene(U"Title");
			}
		}
		else if (r == 1) {
			TextureAsset(U"ED5").draw(0, 0);
			if (MouseL.down())
			{
				// ゲームシーンに遷移
				changeScene(U"Title");
			}
		}
		else if (r == 2) {
			TextureAsset(U"ED6").draw(0, 0);
			if (MouseL.down())
			{
				// ゲームシーンに遷移
				changeScene(U"Title");
			}
		}
	}
	void draw() const override
	{
		TextureAsset::Register(U"ED4", U"Ed_Win1.jpg");
		TextureAsset::Register(U"ED5", U"Ed_Win2.jpg");
		TextureAsset::Register(U"ED6", U"Ed_Win3.jpg");
	}
};

void Main()
{

	App manager;
	manager.add<Title>(U"Title");
	manager.add<Game>(U"Game");
	manager.add<LResult>(U"LResult");
	manager.add<WResult>(U"WResult");


	init();
	//Scene::SetBackground(ColorF{ 0.3, 1.0, 0.3 });

	//アセットを登録する
	TextureAsset::Register(U"map", U"map.png");
	TextureAsset::Register(U"abeimg", U"abe.png");
	TextureAsset::Register(U"knifeimg", U"ナイフ.png");
	TextureAsset::Register(U"knifeicon", U"knifeicon.png");
	TextureAsset::Register(U"katanaimg", U"刀.png");
	TextureAsset::Register(U"katanaicon", U"刀アイコン.jpg");
	TextureAsset::Register(U"slimeimg", U"slime.png");
	TextureAsset::Register(U"diaimg", U"diamond.png");
	TextureAsset::Register(U"OP1", U"OP1.jpg");
	TextureAsset::Register(U"ED1", U"ed_lose1.jpg");
	TextureAsset::Register(U"ED2", U"ed_lose2.jpg");
	TextureAsset::Register(U"ED3", U"ed_lose3.jpg");
	TextureAsset::Register(U"ED4", U"ed_win1.jpg");
	TextureAsset::Register(U"ED5", U"ed_win2.jpg");
	TextureAsset::Register(U"ED6", U"ed_win3.jpg");

	FontAsset::Register(U"Reggae One", 20, U"Reggae-master/fonts/ttf/ReggaeOne-Regular.ttf");

	manager.init(U"Scene");

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}

	}
}

//変数の初期化
void init()
{
	abe.x = 400;
	abe.y = 300;
	abe.HP = 1000;
	abe.co = 20;
	abe.level = 1;
	abe.exp = 0;
	abe.enable = true;


	for (int i = 0; i < slimenum; i++)
	{
		slime[i].x = 0;
		slime[i].y = 0;
		slime[i].vx = 0;
		slime[i].vy = 0;
		slime[i].Eattack = 10;
		slime[i].HP = 30;
		slime[i].co = 10;
		slime[i].enable = false;
	}

	for (int i = 0; i < knifenum; i++)
	{
		knife[i].level = 1;
		knife[i].enable = false;
		knife[i].co = 5;
		knife[i].atk = 10;
	}

	katana.level = 0;
	katana.enable = false;
	katana.co = 40;
	katana.atk = 30;

	syuriken.level = 0;
	syuriken.enable = false;
	syuriken.co = 10;
	syuriken.atk = 10;

	for (int i = 0; i < slimenum; i++)
	{
		dia[i].co = 10;
		dia[i].enable = false;
	}
}

//自機のアップデート関数
void player()
{

	if (KeyD.pressed() && abe.x <= 790)
	{
		++abe.x;
	}
	if (KeyA.pressed() && abe.x > 10)
	{
		--abe.x;
	}
	if (KeyS.pressed() && abe.y <= 590)
	{
		++abe.y;
	}
	if (KeyW.pressed() && abe.y > 100)
	{
		--abe.y;
	}

	if (abe.HP <= 0)
	{
		abe.enable = false;
	}

	for (int i = 0; i < slimenum; i++)
	{

		if (collision(abe, dia[i]) && dia[i].enable == true)
		{
			abe.exp += 200;
			dia[i].enable = false;
		}

	}
	if (abe.exp >= 600)
	{
		levelup = true;
		++abe.level;
		abe.exp = 0;
	}

	if (abe.inv > 0)
	{
		--abe.inv;
	}

	if (KeyK.pressed())
	{
		--abe.HP;
	}

}

//敵機のアップデート関数
void enemy()
{
	for (int i = 0; i < slimenum; i++)
	{
		if (slime[i].enable == true)
		{
			double speed = 0.3;//速度
			double dx = abe.x - slime[i].x;//プレイヤーと敵のx方向の距離
			double dy = abe.y - slime[i].y;//プレイヤーと敵のy方向の距離
			double d = sqrt(dx * dx + dy * dy);//敵とプレイヤーとの距離
			slime[i].vx = speed * (dx / d);//xの移動量
			slime[i].vy = speed * (dy / d);//yの移動量

			slime[i].x += slime[i].vx;
			slime[i].y += slime[i].vy;

			if (collision(abe, slime[i]) && abe.inv == 0)
			{
				--abe.HP;
				abe.inv = 30;
			}

			for (int j = 0; j < knifenum; j++)
			{
				if (weaponcollsion(knife[j], slime[i]) && knife[j].enable == true)
				{
					knife[j].vy = 0;
					knife[j].enable = false;
					slime[i].HP -= knife[j].atk;

					FontAsset(U"Reggae One")(U"{}!!"_fmt(knife[j].atk)).
						drawAt({ slime[i].x,slime[i].y - 30 }, ColorF{ 100.0,0.0,0.0 });

					knife[j].vx = 0;


				}
			}

			//手裏剣の当たり判定
			for (int j = 0; j < syuriken.level; j++)
			{
				const Vec2 pos = OffsetCircular{ Vec2{syuriken.x,syuriken.y},100, theta[j] };
				if (Circle{ pos, syuriken.co }.intersects(Circle{ slime[i].x, slime[i].y, slime[i].co }))
				{
					slime[i].HP -= syuriken.atk;

					FontAsset(U"Reggae One")(U"{}!!"_fmt(syuriken.atk)).
						drawAt({ slime[i].x,slime[i].y - 30 }, ColorF{ 100.0,0.0,0.0 });
				}
			}

			if (weaponcollsion(katana, slime[i]) && katana.enable == true)
			{
				slime[i].HP -= katana.atk;

				FontAsset(U"Reggae One")(U"{}!!"_fmt(katana.atk)).
					drawAt({ slime[i].x,slime[i].y - 30 }, ColorF{ 100.0,0.0,0.0 });
			}

			if (weaponcollsion(syuriken, slime[i]))
			{
				slime[i].HP -= syuriken.atk;

				FontAsset(U"Reggae One")(U"{}!!"_fmt(syuriken.atk)).
					drawAt({ slime[i].x,slime[i].y - 30 }, ColorF{ 100.0,0.0,0.0 });
			}

			if (slime[i].HP <= 0)
			{
				for (int j = 0; j < slimenum; j++)
				{
					if (dia[j].enable == false)
					{
						dia[j].x = slime[i].x;
						dia[j].y = slime[i].y;
						dia[j].enable = true;
						Destroy++;
						break;
					}
				}

				slime[i].enable = false;
			}
		}
		if (RandomBool(0.0001) == true && slime[i].enable == false)
		{
			slime[i].x = Random(-100, 900);
			slime[i].y = Random(-100, 700);
			slime[i].HP = 30;
			slime[i].enable = true;
		}

	}
}

//武器のアップデート関数
void weapon()
{
	if (wno == 2)
	{
		wno = 0;
	}

	if (KeySpace.down())
	{
		if (wno == 0)
		{
			for (int i = 0; i < slimenum; i++)
			{
				knife[i].enable = false;
			}
		}
		wno++;
	}

	if (wno == 0)
	{
		Knife();
	}

	if (wno == 1)
	{
		Katana();
	}


	Syuriken();

	if (wct > 0)
	{
		--wct;
	}

}

/*//描画関数
void draw()
{

	//画像の描画
	TextureAsset(U"map").draw(0, 0);
	if (abe.enable == true)
	{
		//Circle{ abe.x,abe.y,abe.co }.draw(ColorF{ 1.0,1.0,1.0 });
		TextureAsset(U"abeimg").draw(abe.x - 15, abe.y - 20);
	}

	for (int i = 0; i < slimenum; i++)
	{
		if (slime[i].enable == true)
		{
			//Circle{ slime[i].x,slime[i].y,slime[i].co }.draw(ColorF{ 1.0,0.0,0.0 });

			if (abe.x <= slime[i].x)
			{
				TextureAsset(U"slimeimg").draw(slime[i].x - 20, slime[i].y - 30);
			}
			if (abe.x > slime[i].x)
			{
				TextureAsset(U"slimeimg").mirrored().draw(slime[i].x - 20, slime[i].y - 30);
			}
			for (int j = 0; j < knifenum; j++)
			{

				if (weaponcollsion(knife[j], slime[i]))
				{


					FontAsset(U"Reggae One")(U"10!!"_fmt(knife[j].atk)).
						drawAt({ slime[i].x,slime[i].y - 30 }, ColorF{ 100.0,0.0,0.0 });



				}
			}

		}

		if (dia[i].enable == true)
		{
			//Circle{ dia[i].x,dia[i].y,dia[i].co }.draw(ColorF{ 0.0,1.0,0.0 });
			TextureAsset(U"diaimg").draw(dia[i].x - 13, dia[i].y - 13);
		}
	}

	for (int i = 0; i < knifenum; i++)
	{
		if (knife[i].enable == true)
		{
			//Circle{ knife[i].x,knife[i].y,knife[i].co }.draw(ColorF{ 0.0,1.0,0.0 });

			if (knife[i].wm == 0)
			{
				TextureAsset(U"knifeimg").rotated(135_deg).drawAt(knife[i].x - 10, knife[i].y);
			}
			if (knife[i].wm == 1)
			{
				TextureAsset(U"knifeimg").rotated(315_deg).draw(knife[i].x, knife[i].y - 12);
			}
			if (knife[i].wm == 2)
			{
				TextureAsset(U"knifeimg").rotated(225_deg).draw(knife[i].x - 12, knife[i].y - 20);
			}
			if (knife[i].wm == 3)
			{
				TextureAsset(U"knifeimg").rotated(45_deg).draw(knife[i].x - 12, knife[i].y);
			}
			if (knife[i].wm == 4)
			{
				TextureAsset(U"knifeimg").rotated(90_deg).draw(knife[i].x - 20, knife[i].y - 4);
			}
			if (knife[i].wm == 5)
			{
				TextureAsset(U"knifeimg").rotated(180_deg).draw(knife[i].x - 20, knife[i].y - 20);
			}
			if (knife[i].wm == 6)
			{
				TextureAsset(U"knifeimg").rotated(270_deg).draw(knife[i].x - 3, knife[i].y - 20);
			}
			if (knife[i].wm == 7)
			{
				TextureAsset(U"knifeimg").draw(knife[i].x - 3, knife[i].y - 3);
			}

		}
	}

	//自機のHPゲージ
	RectF{ abe.x - 20,abe.y + 20,abe.HP / 25,10 }.draw(ColorF{ 0.0,0.0,1.0 });

	//expゲージ
	Rect{ 100, 30, 600, 30 }.shearedX(10).draw(ColorF{ 0.0,0.0,0.0 });
	Rect{ 100, 30, abe.exp, 30 }.shearedX(10).draw(ColorF{ 0.3,1.0,0.3 });
	Rect{ 100, 30, 600, 30 }.shearedX(10).drawFrame(2);
	FontAsset(U"Reggae One")(U"Level.{}"_fmt(abe.level)).drawAt({ 150, 15 }, ColorF{ 0.0,0.0,0.0 });
	if (wno == 0)
	{
		TextureAsset(U"knifeicon").draw(10, 0);
	}

	if (levelup == true)
	{
		if (select != 0)
		{
			RoundRect{ 100, 200, 180, 225, 20 }.drawShadow(Vec2{ 2, 2 }, 8, 0).draw(ColorF{ 0.0,0.7,0.0 });
		}
		else if (select == 0)
		{
			RoundRect{ 100, 200, 180, 225, 20 }.drawShadow(Vec2{ 2, 2 }, 8, 0).draw(ColorF{ 0.0,0.0,0.7 });
		}
		if (select != 1)
		{
			RoundRect{ 310, 200, 180, 225, 20 }.drawShadow(Vec2{ 2, 2 }, 8, 0).draw(ColorF{ 0.0,0.7,0.0 });
		}
		else if (select == 1)
		{
			RoundRect{ 310, 200, 180, 225, 20 }.drawShadow(Vec2{ 2, 2 }, 8, 0).draw(ColorF{ 0.0,0.0,0.7 });
		}
		if (select != 2)
		{
			RoundRect{ 520, 200, 180, 225, 20 }.drawShadow(Vec2{ 2, 2 }, 8, 0).draw(ColorF{ 0.0,0.7,0.0 });
		}
		else if (select == 2)
		{
			RoundRect{ 520, 200, 180, 225, 20 }.drawShadow(Vec2{ 2, 2 }, 8, 0).draw(ColorF{ 0.0,0.0,0.7 });
		}

	}

	if (pause == true)
	{
		Rect{ 0, 0, 800, 600 }.draw(ColorF{ 0.0,0.0,0.0,0.3 });
	}

}*/

//ナイフのアップデート関数
void Knife()
{
	if (wct == 0)
	{
		//弾が無効なときのみ初期値をセットし有効にする
		for (int i = 0; i < knifenum; i++)
		{
			//撃てる弾をみつける
			if (knife[i].enable == false) {
				//弾を撃つ

				knife[i].wm = muki;

				if (knife[i].wm == 0)
				{
					knife[i].vx = 2;
				}
				if (knife[i].wm == 1)
				{
					knife[i].vx = -2;
				}
				if (knife[i].wm == 2)
				{
					knife[i].vy = 2;
				}
				if (knife[i].wm == 3)
				{
					knife[i].vy = -2;
				}
				if (knife[i].wm == 4)
				{
					knife[i].vx = 1.4;
					knife[i].vy = -1.4;
				}
				if (knife[i].wm == 5)
				{
					knife[i].vx = 1.4;
					knife[i].vy = 1.4;
				}
				if (knife[i].wm == 6)
				{
					knife[i].vx = -1.4;
					knife[i].vy = 1.4;
				}
				if (knife[i].wm == 7)
				{
					knife[i].vx = -1.4;
					knife[i].vy = -1.4;
				}

				knife[i].x = abe.x;
				knife[i].y = abe.y;

				knife[i].enable = true;
				wct = 50;//連射速度　小さいほど連射できる
				break;
			}
		}
	}
	//弾が有効なときに弾を動かす
	for (int i = 0; i < knifenum; i++)
	{
		if (knife[i].enable == true) {
			knife[i].x = knife[i].x + knife[i].vx;
			knife[i].y = knife[i].y + knife[i].vy;
			if (knife[i].x >= 800 || knife[i].y >= 600 || knife[i].x <= 0 || knife[i].y <= 0)
			{
				knife[i].vx = 0;
				knife[i].vy = 0;
				knife[i].enable = false;
			}
		}
	}
}

void Katana()
{
	if (wct == 0)
	{
		katana.enable = true;
	}

	if (katana.enable == true)
	{
		katana.x = abe.x;
		katana.y = abe.y;
		rolling++;
		if (rolling == 40)
		{
			katana.enable = false;
			rolling = 0;
			wct = 200;
		}
	}
}

void Syuriken()
{
	syuriken.x = abe.x;
	syuriken.y = abe.y;

	if (syuriken.level >= 1)
	{
		// 円座標系における角度座標
		theta[0] = (90_deg + t * 180_deg);

	}
	if (syuriken.level >= 2)
	{
		theta[1] = (270_deg + t * 180_deg);
	}
	if (syuriken.level >= 3)
	{
		theta[2] = (180_deg + t * 180_deg);
	}
	if (syuriken.level >= 4)
	{
		theta[3] = (0_deg + t * 180_deg);
	}
}

//レベルアップ時の関数
void Levelup()
{
	if (KeyD.down() && select != 2)
	{
		++select;
	}
	if (KeyA.down() && select != 0)
	{
		--select;
	}
	if (KeyShift.down())
	{
		levelup = false;
	}
}

//当たり判定関数
bool collision(Unit a, Unit b)
{
	const Circle A{ a.x,a.y,a.co };
	const Circle B{ b.x,b.y,b.co };

	if (A.intersects(B) && a.enable == true && b.enable == true)
	{
		//当たっている
		return true;
	}
	return false;
}
//武器の当たり判定関数
bool weaponcollsion(Weapon a, Unit b)
{
	const Circle A{ a.x,a.y,a.co };
	const Circle B{ b.x,b.y,b.co };

	if (A.intersects(B) && a.enable == true && b.enable == true)
	{
		//当たっている
		return true;
	}
	return false;
}

//向きの関数
void direction()
{
	//自機の向きを変える
	if (KeyD.pressed() && delay == 0)
	{
		muki = 0;
	}
	if (KeyA.pressed() && delay == 0)
	{
		muki = 1;
	}
	if (KeyS.pressed() && delay == 0)
	{
		muki = 2;
	}
	if (KeyW.pressed() && delay == 0)
	{
		muki = 3;
	}
	if (KeyW.pressed() && KeyD.pressed())
	{
		muki = 4;
		delay = 10;
	}
	if (KeyS.pressed() && KeyD.pressed())
	{
		muki = 5;
		delay = 10;
	}
	if (KeyS.pressed() && KeyA.pressed())
	{
		muki = 6;
		delay = 10;
	}
	if (KeyA.pressed() && KeyW.pressed())
	{
		muki = 7;
		delay = 10;
	}

	if (delay > 0)
	{
		--delay;
	}
}




