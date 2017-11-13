//
//                                          ���s v1.1
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
	printf("���� : %3d", _count_move);
	gotoxy(LABEL_X + 14, LABEL_Y);
	printf("���� : %4d", _count_point);
	gotoxy(0, -1);
	cout << "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w";
	vector<int> n(52);
	int dir = 0;
	for (int i = 0; i < 52; i++)
		n[i] = i;
	random_shuffle(n.begin(), n.end());

	for (int i = 10; i < 14; i++) //row�s��10~14:�k�W���P��
	{
		card[i].clear();
		printAcard(2 + 14 * (i % 7), 1, -1);
		open_card[i] = 0;
	}
	for (int i = 0; i < 7; i++) //row�s��0~6:�U���C��
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
	card[8].resize(n.size() - dir); //row�s��7:���W�\�P�� 8:���W½�P��
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
	//�]�w�����j�p
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
	SetConsoleTitle("���s");

	//��l���ܼ�
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
	cout << "          �z�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{" << endl;
	cout << "          �x                                                                            �x" << endl;
	cout << "          �x                                    �W�h                                    �x" << endl;
	cout << "          �x                                                                            �x" << endl;
	cout << "          �x  �C���}�l�G�N�@�Ƽ��J�P 52 �i�����C�աA�C�ձi�ƨ̷ӼƦr�Ѥ@�ܤC�i�q���Ʀ�  �x" << endl;
	cout << "          �x          �k�A�C�դ��̤W�診�i�ȵP½�}��ܡA��l�H�л\���覡���|�A�ѤU����  �x" << endl;
	cout << "          �x          �ժ��ȵP�P�˥H�л\���覡���|�b���W���C                            �x" << endl;
	cout << "          �x  ���ʡG�u�ಾ�ʤw½�}���ȵP�A�åB�q�j��p�A�H���ª�������覡�N�ȵP����  �x" << endl;
	cout << "          �x          �W���л\��t�@�w½�}���ȵP�W�A�ӭ쥻½�}��ܪ��ȵP������A�N�i�N  �x" << endl;
	cout << "          �x          �P�諸�̤W���л\�ȵP�ȶ}�C                                        �x" << endl;
	cout << "          �x  �����GA �ȵP�i�����ʨ�k�W���������諸��¦�A�è̷Ӫ�⦬�����|��A�Ѥp  �x" << endl;
	cout << "          �x          �ƨ�j���覡�����C�p�G�Y�ӵP��O�Ū��A�h�i�H�N K�]�H�Ψ�P�襤��  �x" << endl;
	cout << "          �x          �����ȵP�^����ӵP��C                                            �x" << endl;
	cout << "          �x  ���W�����P��G�@���o�@�i�P�A�Y���i���ʪ��ȵP�i���ʦܸӵP��A�Y�S���i����  �x" << endl;
	cout << "          �x          ���ȵP�h�i�~��o�U�@�i�ȵP�A�o���ɥi���s�ѲĤ@�i�o�P�C            �x" << endl;
	cout << "          �x  �ӧQ : �N�Ҧ����J�P�������������ɹC�������C                               �x" << endl;
	cout << "          �x                                                                            �x" << endl;
	cout << "          �x�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�x" << endl;
	cout << "          �x                                                                            �x" << endl;
	cout << "          �x                                    ����                                    �x" << endl;
	cout << "          �x                                                                            �x" << endl;
	cout << "          �x  ��V��    ���                      Space    �T�w                         �x" << endl;
	cout << "          �x  z��       ��^�W�@�ӿ�� ��������   c��      ��^�W�@�B �æ����e���o��    �x" << endl;
	cout << "          �x  Esc       ���                                                            �x" << endl;
	cout << "          �x                                                                     " << VER << "   �x" << endl;
	cout << "          �|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}" << endl;
	cout << endl;
	system("pause");
	system("cls");
	srand(time(NULL));
	Init();

	while (1)
	{
		//�O�_�ӧQ
		if (!isWin)
		{
			if (card[10].size() + card[11].size() + card[12].size() + card[13].size() == 52)
			{
				isWin = true;
				gotoxy(0, LABEL_Y);
				cout << "[�C��]�ӧQ!!!!                         ";
			}
		}
		//�O�_�ŦXAutoFinish����
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
				cout << "��A�i�۰ʧ����P��                      ";
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
		//�۰ʧ���or�������O
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
						if (card[j].size() != card[i].back() % 13) //�Ʀr���T 
							continue;
						else if (!card[j].empty()) //���P �n�P��
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
		//�M�Ŵ�����
		if (isLabalPrint && !isWin)
		{
			gotoxy(0, LABEL_Y);
			if (isCanAutoFinish)
				cout << "��A�i�۰ʧ����P��                      ";
			else
				cout << "                                       ";
			isLabalPrint = false;
		}

		//������O
		switch (kb)
		{
		case 72: case 80: //�W�U
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
			else if (_cho_mode == 2 && open_card[now.pick] > now.num && kb == 72) //��X�i�W
			{
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 4);
				now.num++;
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 5);
			}
			else if (_cho_mode == 2 && now.num > 1 && kb == 80)  //��X�i�U
			{
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 4);
				now.num--;
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 5);
			}
			break;
		case 75: //��
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
		case 77: //�k
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
		case 32: //�ť�
			if (isWin) continue;
			if (_cho_mode == 1) //�粒�ĴX��
			{
				if ((card[now.cho].size() == 0 && now.cho != 7) || (card[8].size() == 0 && now.cho == 7)) //�����Ʀ��P
				{
					gotoxy(0, LABEL_Y);
					cout << "[���ʹH�W]����P�S���P";
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
						printf("���� : %4d", _count_point);
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
									cout << " �x";
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
					printf("���� : %3d", _count_move);
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
			else if (_cho_mode == 2) //�粒���X�i
			{
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 6);
				print_Arrow(now.cho, now.cho / 7, 1);
				_pick_head = card[now.pick][card[now.pick].size() - now.num];
				_cho_mode = 3;
			}
			else  if (_cho_mode == 3) //�粒����ĴX��
			{
				//�T�{���ʱ���
				gotoxy(0, LABEL_Y);
				if (now.cho == now.pick)
				{
					cout << "[���ʹH�W]���ʱƻP��m�Ƥ���ۦP";
					isLabalPrint = true;
					continue;
				}
				else if (card[now.pick].size() == 0)
				{
					cout << "[���ʹH�W]����ƻݦ��P";
					isLabalPrint = true;
					continue;
				}
				if (now.cho / 7 == 1 && !isOp) //���W�Ʊ���
				{
					if (now.num > 1)
					{
						cout << "[���ʹH�W]�L�k���ʦh�i�P�ܦ�����";
						isLabalPrint = true;
						continue;
					}
					else if (now.cho == 7 || now.cho == 8)
					{
						cout << "[���ʹH�W]�L�k���ʵP��½�P��";
						isLabalPrint = true;
						continue;
					}
					else if (card[now.cho].size() != _pick_head % 13)
					{
						cout << "[���ʹH�W]���ʱƻP��m�ƼƦr�ݳs��";
						isLabalPrint = true;
						continue;
					}
					else if (!card[now.cho].empty()) //���P �n�P��
					if (card[now.cho][0] / 13 != _pick_head / 13)
					{
						cout << "[���ʹH�W]���ʱƻP��m�ƪ��ݬۦP";
						isLabalPrint = true;
						continue;
					}
				}
				else if (now.cho / 7 == 0 && !isOp) //���U�Ʊ���
				{
					if (!card[now.cho].empty())
					{
						if (_pick_head % 13 + 1 != card[now.cho].back() % 13)
						{
							cout << "[���ʹH�W]���ʱƻP��m�ƼƦr�ݳs��";
							isLabalPrint = true;
							continue;
						}
						else if ((_pick_head >= 26 && card[now.cho].back() >= 26) || (_pick_head < 26 && card[now.cho].back() < 26))
						{
							cout << "[���ʹH�W]���ʱƻP��m�ƪ�⤣��ۦP";
							isLabalPrint = true;
							continue;
						}
					}
					else if (_pick_head % 13 != 12)
					{
						cout << "[���ʹH�W]���ʱƻP��m�ƼƦr�ݳs��";
						isLabalPrint = true;
						continue;
					}
				}
				moving_Card(now.pick, now.num, now.cho, false);
				//�[����
				if (now.pick >= 10 && now.cho < 7) //�W����U
					_count_point -= 15;
				else if (now.cho >= 10 && now.pick <= 8) //���ʨ�W
					_count_point += 10;
				else if (now.pick == 8 || now.isOpen) //�U�����ʦ�½�Por���ʨ�U��
					_count_point += 5;
				if (_count_point < 0)
					_count_point = 0;
				gotoxy(LABEL_X + 14, LABEL_Y);
				printf("���� : %4d", _count_point);
				_count_move++;
				gotoxy(LABEL_X, LABEL_Y);
				printf("���� : %3d", _count_move);

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
				cout << "[���ʹH�W]�w�^��Ĥ@�B";
				isLabalPrint = true;
				continue;
			}

			//�����½b�Y
			print_Arrow(now.cho, now.cho / 7, 0);
			print_Arrow(now.pick, now.pick / 7, 2);
			if (now.pick < 7)
				print_Arrow(now.pick, card[now.pick].size() - now.num + 1, 4);

			now.cho = move_record.back().pick; //Ū���M��������
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
							cout << " �x";
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
				//�����
				if (now.cho >= 10 && now.pick < 7) //�W����U �٭�
					_count_point += 10;
				else if (now.pick >= 10 && now.cho <= 8) //���ʨ�W �٭�
					_count_point -= 15;
				else if (now.cho == 8 || now.isOpen) //�U�����ʦ�½�Por���ʨ�U�� �٭�
					_count_point -= 5;
				moving_Card(now.pick, now.num, now.cho, now.isOpen);
				now.isOpen = false;
			}
			_count_point -= 2;
			if (_count_point < 0)
				_count_point = 0;
			gotoxy(LABEL_X + 14, LABEL_Y);
			printf("���� : %4d", _count_point);
			_count_move++;
			gotoxy(LABEL_X, LABEL_Y);
			printf("���� : %3d", _count_move);

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
				cout << "[�C��]���\�}�ҷs��";
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
					cout << "[�q��]���ʭ����٭�";
					isOp = false;
				}
				else
				{
					cout << "[�q��]���ʭ���Ѱ�";
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
					cout << "��" << i << "�Ʋ� :";
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
				cout << "����Ҧ� : " << _cho_mode << endl;
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
		cout << "�z�w�w�w�w�{ ";
		gotoxy(x, y + 1);
		cout << "�x�z    �{�x";
		for (int i = 2; i <= 5; i++)
		{
			gotoxy(x, y + i);
			cout << "�x        �x ";
		}
		gotoxy(x, y + 6);
		cout << "�x�|    �}�x";
		gotoxy(x, y + 7);
		cout << "�|�w�w�w�w�} ";
	}
	else if (id == -2)
	{
		gotoxy(x, y);
		cout << "�z�w�w�w�w�{ ";
		for (int i = 0; i<3; i++)
		{
			gotoxy(x, y + 2 * i + 1);
			cout << "�x\\/\\/\\/\\/�x ";
			gotoxy(x, y + 2 * i + 2);
			cout << "�x/\\/\\/\\/\\�x ";
		}
		gotoxy(x, y + 7);
		cout << "�|�w�w�w�w�} ";
	}
	else
	{
		int num = id % 13, suit = id / 13;
		gotoxy(x, y);
		cout << "�z�w�w�w�w�{ ";
		gotoxy(x, y + 1);
		cout << "�x" << NUM2CHAR[num];
		if (suit < 2) SetConsoleTextAttribute(hOuput, 4);
		cout << (char)(suit + 3);
		if (suit < 2) SetConsoleTextAttribute(hOuput, 7);
		cout << "     " << "�x ";
		for (int i = 0; i<5; i++)
		{
			gotoxy(x, y + 2 + i);
			cout << "�x        �x ";
		}
		gotoxy(x, y + 7);
		cout << "�|�w�w�w�w�} ";
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
		cout << "��";
		break;
	case 2:
		gotoxy(7 + swift_x, 9 + swift_y);
		cout << " ";
		break;
	case 3:
		gotoxy(7 + swift_x, 9 + swift_y);
		cout << "��";
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
	//�����½b�Y
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
		card[pick].erase(card[pick].end() - num, card[pick].end()); //�R�����J�P
		if (isOpen)
			open_card[cho] --;
		open_card[cho] += num;
		open_card[pick] -= num;
	}
	else if (pick == 8)
	{
		card[cho].push_back(_pick_head); //�s�W
		print_Card(cho, cho / 7, card[cho].size() - 1); //���
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
					cout << " �x";
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
		card[8].erase(card[8].begin() + open_card[8]); //�R�����J�P
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
					cout << " �x";
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

	//�O�_½�P����ܨS�P
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
		cout << "          �z�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�{" << endl;
		cout << "          �x                                                                            �x" << endl;
		cout << "          �x                                    �~��                                    �x" << endl;
		cout << "          �x                                                                            �x" << endl;
		cout << "          �x                                  ���s�}�l                                  �x" << endl;
		cout << "          �x                                                                            �x" << endl;
		cout << "          �x                                    ���}                                    �x" << endl;
		cout << "          �x                                                                            �x" << endl;
		cout << "          �x�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�x" << endl;
		cout << "          �x                                                                            �x" << endl;
		cout << "          �x                                    �W�h                                    �x" << endl;
		cout << "          �x                                                                            �x" << endl;
		cout << "          �x  �C���}�l�G�N�@�Ƽ��J�P 52 �i�����C�աA�C�ձi�ƨ̷ӼƦr�Ѥ@�ܤC�i�q���Ʀ�  �x" << endl;
		cout << "          �x          �k�A�C�դ��̤W�診�i�ȵP½�}��ܡA��l�H�л\���覡���|�A�ѤU����  �x" << endl;
		cout << "          �x          �ժ��ȵP�P�˥H�л\���覡���|�b���W���C                            �x" << endl;
		cout << "          �x  ���ʡG�u�ಾ�ʤw½�}���ȵP�A�åB�q�j��p�A�H���ª�������覡�N�ȵP����  �x" << endl;
		cout << "          �x          �W���л\��t�@�w½�}���ȵP�W�A�ӭ쥻½�}��ܪ��ȵP������A�N�i�N  �x" << endl;
		cout << "          �x          �P�諸�̤W���л\�ȵP�ȶ}�C                                        �x" << endl;
		cout << "          �x  �����GA �ȵP�i�����ʨ�k�W���������諸��¦�A�è̷Ӫ�⦬�����|��A�Ѥp  �x" << endl;
		cout << "          �x          �ƨ�j���覡�����C�p�G�Y�ӵP��O�Ū��A�h�i�H�N K�]�H�Ψ�P�襤��  �x" << endl;
		cout << "          �x          �����ȵP�^����ӵP��C                                            �x" << endl;
		cout << "          �x  ���W�����P��G�@���o�@�i�P�A�Y���i���ʪ��ȵP�i���ʦܸӵP��A�Y�S���i����  �x" << endl;
		cout << "          �x          ���ȵP�h�i�~��o�U�@�i�ȵP�A�o���ɥi���s�ѲĤ@�i�o�P�C            �x" << endl;
		cout << "          �x  �ӧQ : �N�Ҧ����J�P�������������ɹC�������C                               �x" << endl;
		cout << "          �x                                                                            �x" << endl;
		cout << "          �x�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�x" << endl;
		cout << "          �x                                                                            �x" << endl;
		cout << "          �x                                    ����                                    �x" << endl;
		cout << "          �x                                                                            �x" << endl;
		cout << "          �x  ��V��    ���                      Space    �T�w                         �x" << endl;
		cout << "          �x  z��       ��^�W�@�ӿ�� ��������   c��      ��^�W�@�B �æ����e���o��    �x" << endl;
		cout << "          �x  Esc       ���                                                            �x" << endl;
		cout << "          �x                                                                     " << VER << "   �x" << endl;
		cout << "          �|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�}" << endl;
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
		case 72: //�W
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
		case 80: //�U
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
		case 32: //�ť�
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