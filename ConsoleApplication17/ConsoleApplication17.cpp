#include "header.h"
// repository link: https://github.com/Wow10d/ConsoleApplication17
// очищати з 0
int main()
{
	setlocale(LC_ALL, "ukr");

	system("chcp 65001 >nul");


	srand(time(NULL));
	showCursor(false);

	COORD dx[3] = { { 0, 0 },{ 0, 0 },{ 0, 0 } };
	COORD car = { MAX_X / 2 - 5 , CAR_Y };
	COORD road[2][ROAD_LEN];
	COORD trouble[ROAD_LEN];
	COORD gifts[ROAD_LEN];
	lbEntry easyLB[LB_SIZE], hardLB[LB_SIZE];
	int key{};
	int score = 0;
	bool work = true;
	float waitmSecs = 1.f / 10.f;
	clock_t start = 0;
	bool hardMode = false;
	int mode{};
	char name[16];
	loadLeaderboard(easyLB, "easy.txt");
	loadLeaderboard(hardLB, "hard.txt");
	printCarGame();
	system("pause");
	system("cls");
	cout << "Register: "; cin >> name;
	printLeaderboard(easyLB, "EASY LEADERBOARD", 5, 5);
	printLeaderboard(hardLB, "HARD LEADERBOARD", 40, 5);
	cout << endl << endl << endl << endl;
	cout << "Choose mode: \n Easy - 1 \n Hard - 2" << endl; cin >> mode;

	if (mode == 2) hardMode = true;

	system("cls");


	for (short i = 0; i < ROAD_LEN; i++)
	{
		road[0][i] = { 5, SHORT(i + 1) };
		road[1][i] = { 45, SHORT(i + 1) };
		trouble[i] = { 0, SHORT(i + 1) };
		gifts[i] = { 0, SHORT(i + 1) };
	}
	move_road(dx, road, trouble, gifts, hardMode);
	clock_t gameStart = clock();
	while (work)
	{
		setCursorPosition({ 65, 1 });	// clock pos
		cout << "Time: " << clock() / CLK_TCK;
		setCursorPosition({ 65, 0 });
		cout << "Score: " << score << "   ";
		setCursorPosition(car);
		cout << "^";
		setCursorPosition({ car.X, SHORT(car.Y + 1 )});
		cout << "#";
		Sleep(30);
		setCursorPosition(car);
		cout << " ";
		setCursorPosition({ car.X, SHORT(car.Y + 1 )});
		cout << " ";
		if (clock() - start > CLOCKS_PER_SEC * waitmSecs)	// CLK_TCK
		{
			start = clock();
			move_road(dx, road, trouble, gifts, hardMode);
		}
		work = check_r(car, road, trouble, gifts, score);		// check crush
		if (_kbhit())
		{
			key = (int)_getch();
			switch (key)
			{
			case (75):
				car.X -= 1;	 // <-
				break;
			case (77):
				car.X += 1;	 // ->
				break;
			case (72):	  // ^
				car.Y--;
				break;
			case (80):	 // v
				car.Y++;
				break;
			case (27):		// ESC
				work = false;
				break;
			case KEY_SPACE:
				setCursorPosition({ 30, 24 });
				cout << "PAUSE. Press any key to continue...";
				_getch();
				setCursorPosition({ 30, 24 });
				cout << "                                   ";
				break;
			}
			if (car.X < MIN_X) car.X = MIN_X;
			if (car.X > MAX_X) car.X = MAX_X;
			if (car.Y < CAR_Y - 5) car.Y = CAR_Y - 5;
			if (car.Y > CAR_Y) car.Y = CAR_Y;
		}
	}
	int time = (clock() - gameStart) / CLOCKS_PER_SEC;
	if (hardMode) {
		addToLeaderboard(hardLB, name, score, time);
	}
	else {
		addToLeaderboard(easyLB, name, score, time);
	}

	saveLeaderboard(easyLB, "easy.txt");
	saveLeaderboard(hardLB, "hard.txt");
	setCursorPosition({ 24, 24 });
	cout << "BOOM!" << endl;
	system("pause");
	showCursor(true);
	return 0;
}
