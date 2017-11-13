//
//                                          接龍 v1.1
//                                                                  by KKKmine 

#include<iostream>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<string>
#include<vector>
#include<algorithm>
#include<windows.h>
using namespace std;

#define VER "v1.1"
#define LABEL_X 72
#define LABEL_Y -3
#define TOP_X -3
const int GIVE_CARDS[7] = { 1, 2, 3, 4, 5, 6, 7 };
const char NUM2CHAR[][3] = { "A ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10", "J ", "Q ", "K " };

int _cho_mode, _pick_head, _count_move, _count_point;
bool isOp = false, isCanAutoFinish = false, isAutoMode = false, isWin = false, isLabalPrint = false, isMenuPrint = false;
HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
vector<vector<int> > card;
vector<struct moving_data> move_record;
int open_card[14];
void printAcard(int, int, int);
void print_Card(int, int, int);
void print_Arrow(int, int, int);
void moving_Card(int, int, int, bool);
int page_menu();

struct moving_data
{
	short pick;
	short num;
	short cho;
	bool isOpen;
}now;

inline void gotoxy(int xpos, int ypos)
{
	SetConsoleCursorPosition(hOuput, { (SHORT)xpos, (SHORT)ypos - TOP_X });
}

void Init()
{
	_count_move = 0;
	_count_point = 0;
	_cho_mode = 1;
	now.cho = 0;
	now.num = 1;
	now.pick = 0;
	now.isOpen = false;
	isAutoMode = false;
	isCanAutoFinish = false;
	isWin = false;

	gotoxy(LABEL_X, LABEL_Y);
	printf("移動 : %3d", _count_move);
	gotoxy(LABEL_X + 14, LABEL_Y);
	printf("分數 : %4d", _count_point);
	gotoxy(0, -1);
	cout << "──────────────────────────────────────────────────";
	vector<int> n(52);
	int dir = 0;
	for (int i = 0; i < 52; i++)
		n[i] = i;
	random_shuffle(n.begin(), n.end());

	for (int i = 10; i < 14; i++) //row編號10~14:右上集牌區
	{
		card[i].clear();
		printAcard(2 + 14 * (i % 7), 1, -1);
		open_card[i] = 0;
	}
	for (int i = 0; i < 7; i++) //row編號0~6:下面七排
	{
		open_card[i] = 1;
		card[i].resize(GIVE_CARDS[i]);
		copy(n.begin() + dir, n.begin() + dir + GIVE_CARDS[i], card[i].begin());
		dir += GIVE_CARDS[i];
		for (int j = 0; j < card[i].size(); j++)
		{
			if (j >= card[i].size() - open_card[i])
				printAcard(2 + 14 * i, 11 + 2 * j, card[i][j]);
			else
				printAcard(2 + 14 * i, 11 + 2 * j, -2);
		}
	}
	card[8].resize(n.size() - dir); //row編號7:左上蓋牌區 8:左上翻牌區
	open_card[8] = 0;
	if (dir < 52)
	{
		copy(n.begin() + dir, n.end(), card[8].begin());
		printAcard(2, 1, -2);
	}
	else
		printAcard(2, 1, -1);
	move_record.clear();
	print_Arrow(0, 0, 1);
}

int main()
{
	//設定視窗大小
	COORD buf_size = { 100, 200 };
	SMALL_RECT win_rect = { 0, 0, buf_size.X - 1, 40 - 1 };
	CONSOLE_SCREEN_BUFFER_INFOEX info = { sizeof(info) };
	GetConsoleScreenBufferInfoEx(hOuput, &info);
	info.dwSize = buf_size;
	info.srWindow = win_rect;
	SetConsoleScreenBufferInfoEx(hOuput, &info);
	SetConsoleWindowInfo(hOuput, true, &win_rect);

	CONSOLE_CURSOR_INFO cursor_info;
	cursor_info = { 100, false };
	SetConsoleCursorInfo(hOuput, &cursor_info);
	SetConsoleTitle("接龍");

	//初始化變數
	int kb;
	for (int i = 0; i<14; i++)
	{
		vector<int> c;
		card.push_back(c);
	}
	cout << endl << endl;
	cout << "                         _____ _____ __    __ _____ _____ __ _____ _____ " << endl;
	cout << "                        |  ___|     |  |  |  |_   _|  _  |  | __  |   __|" << endl;
	cout << "                        |___  |  |  |  |__|  | | | |  _  |  |    -|   __|" << endl;
	cout << "                        |_____|_____|_____|__| |_| |_| |_|__|__|__|_____|" << endl;
	cout << endl << endl << endl;
	cout << "          ┌──────────────────────────────────────┐" << endl;
	cout << "          │                                                                            │" << endl;
	cout << "          │                                    規則                                    │" << endl;
	cout << "          │                                                                            │" << endl;
	cout << "          │  遊戲開始：將一副撲克牌 52 張分成七組，每組張數依照數字由一至七張從左排至  │" << endl;
	cout << "          │          右，每組中最上方那張紙牌翻開顯示，其餘以覆蓋的方式堆疊，剩下未分  │" << endl;
	cout << "          │          組的紙牌同樣以覆蓋的方式堆疊在左上角。                            │" << endl;
	cout << "          │  移動：只能移動已翻開的紙牌，並且從大到小，以紅黑花色交錯的方式將紙牌移動  │" << endl;
	cout << "          │          上的覆蓋到另一已翻開的紙牌上，而原本翻開顯示的紙牌移走後，就可將  │" << endl;
	cout << "          │          同堆的最上的覆蓋紙牌掀開。                                        │" << endl;
	cout << "          │  收集：A 紙牌可先移動到右上角當成收集堆的基礎，並依照花色收集成四堆，由小  │" << endl;
	cout << "          │          排到大的方式收集。如果某個牌堆是空的，則可以將 K（以及其牌堆中所  │" << endl;
	cout << "          │          有的紙牌）移到該牌堆。                                            │" << endl;
	cout << "          │  左上角的牌堆：一次發一張牌，若有可移動的紙牌可移動至該牌堆，若沒有可移動  │" << endl;
	cout << "          │          的紙牌則可繼續發下一張紙牌，發完時可重新由第一張發牌。            │" << endl;
	cout << "          │  勝利 : 將所有撲克牌完全收集完畢時遊戲結束。                               │" << endl;
	cout << "          │                                                                            │" << endl;
	cout << "          │──────────────────────────────────────│" << endl;
	cout << "          │                                                                            │" << endl;
	cout << "          │                                    控制                                    │" << endl;
	cout << "          │                                                                            │" << endl;
	cout << "          │  方向鍵    選擇                      Space    確定                         │" << endl;
	cout << "          │  z鍵       返回上一個選擇 不扣分數   c鍵      返回上一步 並扣除前次得分    │" << endl;
	cout << "          │  Esc       選單                                                            │" << endl;
	cout << "          │                                                                     " << VER << "   │" << endl;
	cout << "          └──────────────────────────────────────┘" << endl;
	cout << endl;
	system("pause");
	system("cls");
	srand(time(NULL));
	Init();

	while (1)
	{
		//是否勝利
		if (!isWin)
		{
			if (card[10].size() + card[11].size() + card[12].size() + card[13].size() == 52)
			{
				isWin = true;
				gotoxy(0, LABEL_Y);
				cout << "[遊戲]勝利!!!!                         ";
			}
		}
		//是否符合AutoFinish條件
		if (card[8].empty() && !isAutoMode && !isWin)
		{
			bool flag = true;
			for (int i = 0; i < 8 && flag; i++)
			if (open_card[i] != card[i].size())
				flag = false;
			if (flag && !isCanAutoFinish)
			{
				isCanAutoFinish = true;
				gotoxy(0, LABEL_Y);
				cout << "按A可自動完成牌局                      ";
			}
			else if (!flag && isCanAutoFinish)
			{
				isCanAutoFinish = false;
				gotoxy(0, LABEL_Y);
				cout << "                                       ";
			}
		}
		else if (isCanAutoFinish && !isWin)
		{
			isCanAutoFinish = false;
			gotoxy(0, LABEL_Y);
			cout << "                                       ";
		}
		//自動完成or接收指令
		gotoxy(0, TOP_X);
		if (!isAutoMode)
			kb = getch();
		else
		{
			bool flag = true;
			print_Arrow(now.cho, now.cho / 7, 0);
			for (int i = 0; i < 7 && flag; i++)
			{
				if (!card[i].empty())
				{
					for (int j = 10; j < 14 && flag; j++){
						if (card[j].size() != card[i].back() % 13) //數字正確 
							continue;
						else if (!card[j].empty()) //有牌 要同色
						if (card[j][0] / 13 != card[i].back() / 13)
							continue;
						_cho_mode = 3;
						now.pick = i;
						now.num = 1;
						now.cho = j;
						_pick_head = card[i].back();
						kb = 32;
						flag = false;
						break;
					}
				}
			}
			if (flag)
			{
				isAutoMode = false;
				isCanAutoFinish = false;
				kb = 0;
				_cho_mode = 1;
				now.cho = 0;
				print_Arrow(now.cho, now.cho / 7, 1);
			}
		}
		//清空提示欄
		if (isLabalPrint && !isWin)
		{
			gotoxy(0, LABEL_Y);
			if (isCanAutoFinish)
				cout << "按A可自動完成牌局                      ";
			else
				cout << "                                       ";
			isLabalPrint = false;
		}

		//執行指令
		switch (kb)
		{
		case 72: case 80: //上下
			if (_cho_mode != 2)
			{
				print_Arrow(now.cho, now.cho / 7, 0);
				now.cho = (now.cho + 7) % 14;
				if (now.cho == 9 && open_card[8] != 0)
					now.cho = 8;
				else if ((now.cho == 8 || now.cho == 9) && open_card[8] == 0)
					now.cho = 10;
				print_Arrow(now.cho, now.cho / 7, 1);
			}
			else if (_cho_mode == 2 && open_card[now.pick] > now.num && kb == 72) //選幾張上
			{
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 4);
				now.num++;
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 5);
			}
			else if (_cho_mode == 2 && now.num > 1 && kb == 80)  //選幾張下
			{
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 4);
				now.num--;
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 5);
			}
			break;
		case 75: //左
			if (_cho_mode != 2)
			{
				print_Arrow(now.cho, now.cho / 7, 0);
				now.cho--;
				if (now.cho == 9 && open_card[8] != 0)
					now.cho = 8;
				else if (now.cho == 9 && open_card[8] == 0)
					now.cho = 7;
				else if (now.cho == -1)
					now.cho = 6;
				else if (now.cho == 6)
					now.cho = 13;
				print_Arrow(now.cho, now.cho / 7, 1);
			}
			break;
		case 77: //右
			if (_cho_mode != 2)
			{
				print_Arrow(now.cho, now.cho / 7, 0);
				now.cho++;
				if (now.cho == 9)
					now.cho = 10;
				else if (now.cho == 8 && open_card[8] == 0)
					now.cho = 10;
				else if (now.cho == 7)
					now.cho = 0;
				else if (now.cho == 14)
					now.cho = 7;
				print_Arrow(now.cho, now.cho / 7, 1);
			}
			break;
		case 32: //空白
			if (isWin) continue;
			if (_cho_mode == 1) //選完第幾排
			{
				if ((card[now.cho].size() == 0 && now.cho != 7) || (card[8].size() == 0 && now.cho == 7)) //拿取排有牌
				{
					gotoxy(0, LABEL_Y);
					cout << "[移動違規]選取牌沒有牌";
					isLabalPrint = true;
					continue;
				}
				if (now.cho == 7)
				{
					if (open_card[8] == card[8].size())
					{
						printAcard(2, 1, -2);
						open_card[8] = 0;
						for (int i = 1; i < 9; i++)
						{
							gotoxy(16, i);
							cout << "                         ";
						}
						_count_point -= 100;
						if (_count_point < 0)
							_count_point = 0;
						gotoxy(LABEL_X + 14, LABEL_Y);
						printf("分數 : %4d", _count_point);
					}
					else
					{
						open_card[8]++;
						if (open_card[8] > 3)
						{
							gotoxy(18, 2);
							for (int i = 3; i > 0; i--)
							{
								cout << NUM2CHAR[card[8][open_card[8] - i] % 13];
								if (card[8][open_card[8] - i] / 13 < 2) SetConsoleTextAttribute(hOuput, 4);
								cout << (char)(card[8][open_card[8] - i] / 13 + 3);
								if (card[8][open_card[8] - i] / 13 < 2) SetConsoleTextAttribute(hOuput, 7);
								if (i > 1)
									cout << " │";
							}
						}
						else
							printAcard(16 + 6 * (open_card[8] - 1), 1, card[8][open_card[8] - 1]);
					}
					now.num = 1;
					if (open_card[8] == card[8].size())
						printAcard(2, 1, -1);

					move_record.push_back(now);
					_count_move++;
					gotoxy(LABEL_X, LABEL_Y);
					printf("移動 : %3d", _count_move);
					continue;
				}
				now.pick = now.cho;
				now.num = 1;
				print_Arrow(now.cho, now.cho / 7, 0);
				print_Arrow(now.cho, now.cho / 7, 3);
				if (now.pick / 7 == 0 && open_card[now.pick] != 1)
				{
					print_Arrow(now.pick, card[now.pick].size(), 5);
					_cho_mode = 2;
				}
				else if (now.pick / 7 == 0 && open_card[now.pick] == 1)
				{
					print_Arrow(now.cho, now.cho / 7, 1);
					print_Arrow(now.pick, card[now.pick].size(), 6);
					_pick_head = card[now.pick][card[now.pick].size() - 1];
					_cho_mode = 3;
				}
				else
				{
					print_Arrow(now.cho, now.cho / 7, 1);
					if (now.pick != 8)
						_pick_head = card[now.pick][card[now.pick].size() - 1];
					else
						_pick_head = card[8][open_card[8] - 1];
					_cho_mode = 3;
				}
			}
			else if (_cho_mode == 2) //選完拿幾張
			{
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 6);
				print_Arrow(now.cho, now.cho / 7, 1);
				_pick_head = card[now.pick][card[now.pick].size() - now.num];
				_cho_mode = 3;
			}
			else  if (_cho_mode == 3) //選完移到第幾排
			{
				//確認移動條件
				gotoxy(0, LABEL_Y);
				if (now.cho == now.pick)
				{
					cout << "[移動違規]移動排與放置排不能相同";
					isLabalPrint = true;
					continue;
				}
				else if (card[now.pick].size() == 0)
				{
					cout << "[移動違規]選取排需有牌";
					isLabalPrint = true;
					continue;
				}
				if (now.cho / 7 == 1 && !isOp) //給上排條件
				{
					if (now.num > 1)
					{
						cout << "[移動違規]無法移動多張牌至收集區";
						isLabalPrint = true;
						continue;
					}
					else if (now.cho == 7 || now.cho == 8)
					{
						cout << "[移動違規]無法移動牌至翻牌區";
						isLabalPrint = true;
						continue;
					}
					else if (card[now.cho].size() != _pick_head % 13)
					{
						cout << "[移動違規]移動排與放置排數字需連續";
						isLabalPrint = true;
						continue;
					}
					else if (!card[now.cho].empty()) //有牌 要同色
					if (card[now.cho][0] / 13 != _pick_head / 13)
					{
						cout << "[移動違規]移動排與放置排花色需相同";
						isLabalPrint = true;
						continue;
					}
				}
				else if (now.cho / 7 == 0 && !isOp) //給下排條件
				{
					if (!card[now.cho].empty())
					{
						if (_pick_head % 13 + 1 != card[now.cho].back() % 13)
						{
							cout << "[移動違規]移動排與放置排數字需連續";
							isLabalPrint = true;
							continue;
						}
						else if ((_pick_head >= 26 && card[now.cho].back() >= 26) || (_pick_head < 26 && card[now.cho].back() < 26))
						{
							cout << "[移動違規]移動排與放置排花色不能相同";
							isLabalPrint = true;
							continue;
						}
					}
					else if (_pick_head % 13 != 12)
					{
						cout << "[移動違規]移動排與放置排數字需連續";
						isLabalPrint = true;
						continue;
					}
				}
				moving_Card(now.pick, now.num, now.cho, false);
				//加分數
				if (now.pick >= 10 && now.cho < 7) //上移到下
					_count_point -= 15;
				else if (now.cho >= 10 && now.pick <= 8) //移動到上
					_count_point += 10;
				else if (now.pick == 8 || now.isOpen) //下面移動有翻牌or移動到下面
					_count_point += 5;
				if (_count_point < 0)
					_count_point = 0;
				gotoxy(LABEL_X + 14, LABEL_Y);
				printf("分數 : %4d", _count_point);
				_count_move++;
				gotoxy(LABEL_X, LABEL_Y);
				printf("移動 : %3d", _count_move);

				move_record.push_back(now);
				now.isOpen = false;
				print_Arrow(now.cho, now.cho / 7, 1);
				_cho_mode = 1;
			}
			break;
		case 99: //c
			if (isWin) continue;
			if (move_record.size() == 0)
			{
				gotoxy(0, LABEL_Y);
				cout << "[移動違規]已回到第一步";
				isLabalPrint = true;
				continue;
			}

			//擦掉舊箭頭
			print_Arrow(now.cho, now.cho / 7, 0);
			print_Arrow(now.pick, now.pick / 7, 2);
			if (now.pick < 7)
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 4);

			now.cho = move_record.back().pick; //讀取和消除紀錄
			now.num = move_record.back().num;
			now.pick = move_record.back().cho;
			now.isOpen = move_record.back().isOpen;
			move_record.pop_back();

			if (now.pick == 7)
			{
				if (open_card[8] == 0)
				{
					printAcard(2, 1, -1);
					open_card[8] = card[8].size();
					int j = 3;
					if (card[8].size() < j)
						j = card[8].size();
					for (int i = 0; i<j; i++)
						printAcard(16 + 6 * i, 1, card[8][open_card[8] + i - j]);
				}
				else if (open_card[8] > 3)
				{
					if (open_card[8] == card[8].size())
						printAcard(2, 1, -2);
					open_card[8]--;
					gotoxy(18, 2);
					for (int i = 3; i > 0; i--)
					{
						cout << NUM2CHAR[card[8][open_card[8] - i] % 13];
						if (card[8][open_card[8] - i] / 13 < 2) SetConsoleTextAttribute(hOuput, 4);
						cout << (char)(card[8][open_card[8] - i] / 13 + 3);
						if (card[8][open_card[8] - i] / 13 < 2) SetConsoleTextAttribute(hOuput, 7);
						if (i > 1)
							cout << " │";
					}
					if (open_card[8] == card[8].size() - 1)
						printAcard(2, 1, -2);
				}
				else
				{
					open_card[8]--;
					for (int i = 1; i < 9; i++)
					{
						gotoxy(16, i);
						cout << "                         ";
					}
					for (int i = 0; i < open_card[8]; i++)
					{
						printAcard(16 + 6 * i, 1, card[8][i]);
					}
				}
				if (open_card[8] == card[8].size() - 1)
					printAcard(2, 1, -2);
			}
			else
			{
				//改分數
				if (now.cho >= 10 && now.pick < 7) //上移到下 還原
					_count_point += 10;
				else if (now.pick >= 10 && now.cho <= 8) //移動到上 還原
					_count_point -= 15;
				else if (now.cho == 8 || now.isOpen) //下面移動有翻牌or移動到下面 還原
					_count_point -= 5;
				moving_Card(now.pick, now.num, now.cho, now.isOpen);
				now.isOpen = false;
			}
			_count_point -= 2;
			if (_count_point < 0)
				_count_point = 0;
			gotoxy(LABEL_X + 14, LABEL_Y);
			printf("分數 : %4d", _count_point);
			_count_move++;
			gotoxy(LABEL_X, LABEL_Y);
			printf("移動 : %3d", _count_move);

			now.cho = now.pick;
			print_Arrow(now.cho, now.cho / 7, 1);
			_cho_mode = 1;
			break;
		case 122: //z
			if (_cho_mode == 2)
			{
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 4);
				print_Arrow(now.pick, now.pick / 7, 2);
				print_Arrow(now.pick, now.pick / 7, 1);
				now.cho = now.pick;
				_cho_mode = 1;
			}
			else if (_cho_mode == 3)
			{
				print_Arrow(now.cho, now.cho / 7, 0);
				if (now.pick / 7 == 0 && open_card[now.pick] != 1)
				{
					print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 5);
					_cho_mode = 2;
				}
				else
				{
					print_Arrow(now.pick, now.pick / 7, 2);
					print_Arrow(now.pick, now.pick / 7, 1);
					if (now.pick / 7 == 0 && open_card[now.pick] == 1)
						print_Arrow(now.pick, card[now.pick].size(), 4);
					now.cho = now.pick;
					_cho_mode = 1;
				}
			}
			break;
		case 97: //a
			if (isCanAutoFinish)
			{
				isAutoMode = true;
				print_Arrow(now.cho, now.cho / 7, 0);
				print_Arrow(now.pick, now.pick / 7, 2);
				if (now.pick < 7)
					print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 4);
			}
			break;
		case 27: //"Esc"
			switch (page_menu())
			{
			case 0:
				break;
			case 1:
				system("cls");
				isMenuPrint = false;
				gotoxy(0, LABEL_Y);
				cout << "[遊戲]成功開啟新局";
				isLabalPrint = true;
				Init();
				break;
			case -1:
				return 0;
				break;
			}
			break;
		case 47: //"/"
			string str;
			isLabalPrint = true;
			gotoxy(0, LABEL_Y);
			cout << "/";
			cin >> str;
			if (str == "op")
			{
				gotoxy(0, LABEL_Y);
				if (isOp)
				{
					cout << "[通知]移動限制還原";
					isOp = false;
				}
				else
				{
					cout << "[通知]移動限制解除";
					isOp = true;
				}
			}
			else if (str == "debug")
			{
				gotoxy(0, 36);
				for (int i = 0; i <= 20; i++)
				{
					for (int j = 0; j <= 100; j++)
					{
						cout << " ";
					}
				}
				gotoxy(0, 36);
				for (int i = 0; i < 14; i++)
				{
					cout << "第" << i << "排組 :";
					for (int j = 0; j < card[i].size(); j++)
					{
						if (j == card[i].size() - open_card[i] && i != 8)
							cout << "|";
						else if (j == open_card[i] && i == 8)
							cout << "|";
						else
							cout << " ";
						printf("%2d", card[i][j]);
					}
					cout << endl;
				}
				cout << "選取模式 : " << _cho_mode << endl;
				cout << "cho_row : " << now.cho << endl;
				cout << "pick_row : " << now.pick << " " << " pick_num : " << now.num << " pick_head : " << _pick_head;
			}
			break;
		}
	}
	return 0;
}

void printAcard(int x, int y, int id)
{
	if (id == -1)
	{
		gotoxy(x, y);
		cout << "┌────┐ ";
		gotoxy(x, y + 1);
		cout << "│┌    ┐│";
		for (int i = 2; i <= 5; i++)
		{
			gotoxy(x, y + i);
			cout << "│        │ ";
		}
		gotoxy(x, y + 6);
		cout << "│└    ┘│";
		gotoxy(x, y + 7);
		cout << "└────┘ ";
	}
	else if (id == -2)
	{
		gotoxy(x, y);
		cout << "┌────┐ ";
		for (int i = 0; i<3; i++)
		{
			gotoxy(x, y + 2 * i + 1);
			cout << "│\\/\\/\\/\\/│ ";
			gotoxy(x, y + 2 * i + 2);
			cout << "│/\\/\\/\\/\\│ ";
		}
		gotoxy(x, y + 7);
		cout << "└────┘ ";
	}
	else
	{
		int num = id % 13, suit = id / 13;
		gotoxy(x, y);
		cout << "┌────┐ ";
		gotoxy(x, y + 1);
		cout << "│" << NUM2CHAR[num];
		if (suit < 2) SetConsoleTextAttribute(hOuput, 4);
		cout << (char)(suit + 3);
		if (suit < 2) SetConsoleTextAttribute(hOuput, 7);
		cout << "     " << "│ ";
		for (int i = 0; i<5; i++)
		{
			gotoxy(x, y + 2 + i);
			cout << "│        │ ";
		}
		gotoxy(x, y + 7);
		cout << "└────┘ ";
	}
}

void print_Card(int row, int col, int nth)
{
	col = (col + 1) % 2;
	if (col == 0)
		printAcard(2 + 14 * (row % 7), 1 + 10 * col, card[row][nth]);
	else if (col == 1)
		printAcard(2 + 14 * (row % 7), 1 + 10 * col + 2 * nth, card[row][nth]);
}

void print_Arrow(int row, int col, int type)
{
	row %= 7;
	int swift_x = 14 * row, swift_y = 0;

	if (row == 1 && col == 1 && open_card[8] < 3)
		swift_x = 8 + 6 * open_card[8];
	else if (row == 1 && col == 1 && open_card[8] >= 3)
		swift_x = 26;
	if (col == 0 && card[row].size() > 0)
		swift_y = 2 * card[row].size() + 9;
	else if (col == 0 && card[row].size() == 0)
		swift_y = 11;

	switch (type)
	{
	case 0:
		gotoxy(7 + swift_x, 10 + swift_y);
		cout << " ";
		break;
	case 1:
		gotoxy(7 + swift_x, 10 + swift_y);
		cout << "△";
		break;
	case 2:
		gotoxy(7 + swift_x, 9 + swift_y);
		cout << " ";
		break;
	case 3:
		gotoxy(7 + swift_x, 9 + swift_y);
		cout << "▲";
		break;
	case 4:
		gotoxy(1 + 14 * row, 10 + 2 * col);
		cout << " ";
		gotoxy(14 + 14 * row, 10 + 2 * col);
		cout << " ";
		break;
	case 5:
		gotoxy(1 + 14 * row, 10 + 2 * col);
		cout << ">";
		gotoxy(14 + 14 * row, 10 + 2 * col);
		cout << "<";
		break;
	case 6:
		gotoxy(1 + 14 * row, 10 + 2 * col);
		cout << (char)16;
		gotoxy(14 + 14 * row, 10 + 2 * col);
		cout << (char)17;
		break;
	}
}

void moving_Card(int pick, int num, int cho, bool isOpen)
{
	//擦掉舊箭頭
	print_Arrow(cho, cho / 7, 0);
	print_Arrow(pick, pick / 7, 2);
	if (pick < 7)
		print_Arrow(pick, card[pick].size() - num + 1, 4);

	if (pick != 8 && cho != 8)
	{
		if (isOpen)
			printAcard(2 + 14 * cho, 11 + 2 * card[cho].size() - 2, -2);
		for (int i = card[pick].size() - num; i < card[pick].size(); i++)
		{
			card[cho].push_back(card[pick][i]);
			print_Card(cho, cho / 7, card[cho].size() - 1);
			if (pick / 7 == 0)
			{
				gotoxy(2 + 14 * (pick % 7), 11 + 2 * i + 6);
				cout << "             ";
				gotoxy(2 + 14 * (pick % 7), 11 + 2 * i + 7);
				cout << "             ";
			}
		}
		card[pick].erase(card[pick].end() - num, card[pick].end()); //刪除撲克牌
		if (isOpen)
			open_card[cho] --;
		open_card[cho] += num;
		open_card[pick] -= num;
	}
	else if (pick == 8)
	{
		card[cho].push_back(_pick_head); //新增
		print_Card(cho, cho / 7, card[cho].size() - 1); //顯示
		open_card[pick] -= 1;
		open_card[cho] += 1;
		if (open_card[8] > 2)
		{
			gotoxy(18, 2);
			for (int i = 3; i > 0; i--)
			{
				cout << NUM2CHAR[card[8][open_card[8] - i] % 13];
				if (card[8][open_card[8] - i] / 13 < 2) SetConsoleTextAttribute(hOuput, 4);
				cout << (char)(card[8][open_card[8] - i] / 13 + 3);
				if (card[8][open_card[8] - i] / 13 < 2) SetConsoleTextAttribute(hOuput, 7);
				if (i > 1)
					cout << " │";
			}
		}
		else
		{
			for (int i = 1; i < 9; i++)
			{
				gotoxy(16, i);
				cout << "                         ";
			}
			for (int i = 0; i < open_card[8]; i++)
			{
				printAcard(16 + 6 * i, 1, card[8][i]);
			}
		}
		card[8].erase(card[8].begin() + open_card[8]); //刪除撲克牌
	}
	else if (cho == 8)
	{
		card[8].insert(card[8].begin() + open_card[8], card[pick].back());
		open_card[pick] -= 1;
		open_card[cho] += 1;
		if (open_card[8] > 3)
		{
			gotoxy(18, 2);
			for (int i = 3; i > 0; i--)
			{
				cout << NUM2CHAR[card[8][open_card[8] - i] % 13];
				if (card[8][open_card[8] - i] / 13 < 2) SetConsoleTextAttribute(hOuput, 4);
				cout << (char)(card[8][open_card[8] - i] / 13 + 3);
				if (card[8][open_card[8] - i] / 13 < 2) SetConsoleTextAttribute(hOuput, 7);
				if (i > 1)
					cout << " │";
			}
		}
		else
		{
			for (int i = 1; i < 9; i++)
			{
				gotoxy(16, i);
				cout << "                         ";
			}
			for (int i = 0; i < open_card[8]; i++)
			{
				printAcard(16 + 6 * i, 1, card[8][i]);
			}
		}
		card[pick].pop_back();
		if (pick < 7)
		{
			gotoxy(2 + 14 * (pick % 7), 11 + 2 * card[pick].size() + 6);
			cout << "             ";
			gotoxy(2 + 14 * (pick % 7), 11 + 2 * card[pick].size() + 7);
			cout << "             ";
		}
	}

	//是否翻牌或顯示沒牌
	if (card[pick].size() != 0 && pick != 8)
	{
		if (open_card[pick] == 0)
		{
			open_card[pick] = 1;
			now.isOpen = true;
		}
		else
			now.isOpen = false;
		print_Card(pick, pick / 7, card[pick].size() - 1);
	}
	else if (card[pick].size() == 0 && pick != 8)
		printAcard(2 + 14 * (pick % 7), 11 - 10 * (pick / 7), -1);
}

int page_menu()
{
	int kb, select = 0;
	if (!isMenuPrint)
	{
		gotoxy(0, 98);
		cout << "          ┌──────────────────────────────────────┐" << endl;
		cout << "          │                                                                            │" << endl;
		cout << "          │                                    繼續                                    │" << endl;
		cout << "          │                                                                            │" << endl;
		cout << "          │                                  重新開始                                  │" << endl;
		cout << "          │                                                                            │" << endl;
		cout << "          │                                    離開                                    │" << endl;
		cout << "          │                                                                            │" << endl;
		cout << "          │──────────────────────────────────────│" << endl;
		cout << "          │                                                                            │" << endl;
		cout << "          │                                    規則                                    │" << endl;
		cout << "          │                                                                            │" << endl;
		cout << "          │  遊戲開始：將一副撲克牌 52 張分成七組，每組張數依照數字由一至七張從左排至  │" << endl;
		cout << "          │          右，每組中最上方那張紙牌翻開顯示，其餘以覆蓋的方式堆疊，剩下未分  │" << endl;
		cout << "          │          組的紙牌同樣以覆蓋的方式堆疊在左上角。                            │" << endl;
		cout << "          │  移動：只能移動已翻開的紙牌，並且從大到小，以紅黑花色交錯的方式將紙牌移動  │" << endl;
		cout << "          │          上的覆蓋到另一已翻開的紙牌上，而原本翻開顯示的紙牌移走後，就可將  │" << endl;
		cout << "          │          同堆的最上的覆蓋紙牌掀開。                                        │" << endl;
		cout << "          │  收集：A 紙牌可先移動到右上角當成收集堆的基礎，並依照花色收集成四堆，由小  │" << endl;
		cout << "          │          排到大的方式收集。如果某個牌堆是空的，則可以將 K（以及其牌堆中所  │" << endl;
		cout << "          │          有的紙牌）移到該牌堆。                                            │" << endl;
		cout << "          │  左上角的牌堆：一次發一張牌，若有可移動的紙牌可移動至該牌堆，若沒有可移動  │" << endl;
		cout << "          │          的紙牌則可繼續發下一張紙牌，發完時可重新由第一張發牌。            │" << endl;
		cout << "          │  勝利 : 將所有撲克牌完全收集完畢時遊戲結束。                               │" << endl;
		cout << "          │                                                                            │" << endl;
		cout << "          │──────────────────────────────────────│" << endl;
		cout << "          │                                                                            │" << endl;
		cout << "          │                                    控制                                    │" << endl;
		cout << "          │                                                                            │" << endl;
		cout << "          │  方向鍵    選擇                      Space    確定                         │" << endl;
		cout << "          │  z鍵       返回上一個選擇 不扣分數   c鍵      返回上一步 並扣除前次得分    │" << endl;
		cout << "          │  Esc       選單                                                            │" << endl;
		cout << "          │                                                                     " << VER << "   │" << endl;
		cout << "          └──────────────────────────────────────┘" << endl;
		isMenuPrint = true;
	}
	gotoxy(40, 100 + 2 * select);
	cout << ">";
	gotoxy(59, 100 + 2 * select);
	cout << "<";
	gotoxy(0, 190);
	gotoxy(0, 95);
	while (1)
	{
		kb = getch();
		switch (kb)
		{
		case 72: //上
			gotoxy(40, 100 + 2 * select);
			cout << " ";
			gotoxy(59, 100 + 2 * select);
			cout << " ";
			select = (select + 2) % 3;
			gotoxy(40, 100 + 2 * select);
			cout << ">";
			gotoxy(59, 100 + 2 * select);
			cout << "<";
			break;
		case 80: //下
			gotoxy(40, 100 + 2 * select);
			cout << " ";
			gotoxy(59, 100 + 2 * select);
			cout << " ";
			select = (select + 1) % 3;
			gotoxy(40, 100 + 2 * select);
			cout << ">";
			gotoxy(59, 100 + 2 * select);
			cout << "<";
			break;
		case 32: //空白
			gotoxy(40, 100 + 2 * select);
			cout << " ";
			gotoxy(59, 100 + 2 * select);
			cout << " ";
			if (select == 0)
				return 0;
			else if (select == 1)
				return 1;
			else if (select == 2)
				return -1;
			break;
		}
	}
}