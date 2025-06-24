#include "header.h"

void showCursor(bool flag)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(hOut, &info);
	info.bVisible = flag;
	SetConsoleCursorInfo(hOut, &info);
}

void setCursorPosition(COORD coord)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	// COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void SetColor(int text, int background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void move_road(COORD* dx, COORD road[2][ROAD_LEN], COORD trouble[ROAD_LEN], COORD gifts[ROAD_LEN], bool hardMode)
{
	short const dMax = 5;
	short i;

	short minValY = 1;
	short minValX = -1;
	short maxValX = 1;

	for (i = ROAD_LEN - 1; i > 0; i--)
	{
		setCursorPosition(road[0][i]);		// left road border
		cout << " ";
		road[0][i] = road[0][i - 1];
		road[0][i].Y++;
		setCursorPosition(road[0][i]);
		cout << "|";
		//
		setCursorPosition(road[1][i]);		// right road border
		cout << " ";
		road[1][i] = road[1][i - 1];
		road[1][i].Y++;
		setCursorPosition(road[1][i]);
		cout << "|";
		//
		setCursorPosition(trouble[i]);		// trouble
		cout << " ";
		trouble[i] = trouble[i - 1];
		trouble[i].Y++;
		setCursorPosition(trouble[i]);
		if (trouble[i].X > 0) {
			SetColor(4, 0);
			cout << "&";
			SetColor(15, 0);
		}
		setCursorPosition(gifts[i]);        // gifts
		cout << " ";
		gifts[i] = gifts[i - 1];
		gifts[i].Y++;
		setCursorPosition(gifts[i]);
		if (gifts[i].X > 0) {
			SetColor(2, 0);
			cout << "$";
			SetColor(15, 0);
		}
	}

	setCursorPosition({ trouble[0].X, 1 });
	if (trouble[0].X > 0) cout << " ";
	setCursorPosition({ gifts[0].X, 1 });
	if (gifts[0].X > 0) cout << " ";

	short newX{};
	if (hardMode && rand() % 4 == 0) {
		newX = road[0][0].X + rand() % (road[1][0].X - road[0][0].X - 1);
	}

	trouble[0] = {newX, 1};
	setCursorPosition(trouble[0]);
	if (trouble[0].X > 0) cout << "&";


	short giftX{};
	if (rand() % 10 == 0) {
		giftX = road[0][0].X + 1 + rand() % (road[1][0].X - road[0][0].X - 2);
	}

	gifts[0] = { giftX, 1 };
	setCursorPosition(gifts[0]);
	if (gifts[0].X > 0) cout << "$";

	if (dx[0].Y <= 0)
	{
		dx[0].Y = rand() % dMax;
		dx[0].X = minValX + rand() % (maxValX - minValX + 1);
	}
	if (dx[1].Y <= 0)
	{
		dx[1].Y = rand() % dMax;
		dx[1].X = minValX + rand() % (maxValX - minValX + 1);
	}

	if (dx[2].Y <= 0)
	{
		dx[2].Y = rand() % dMax;
		dx[2].X = minValX + rand() % (maxValX - minValX + 1);
	}

	i = 0;

	setCursorPosition(road[0][i]);
	cout << " ";
	road[0][i].X += dx[0].X;
	if (dx[0].X < 0)
		dx[0].X--;
	else
		dx[0].X++;
	if (road[0][i].X < MIN_X) road[0][i].X = MIN_X;
	if (road[0][i].X > MAX_X / 2 - 4) road[0][i].X = MAX_X / 2 - 4;
	dx[0].Y--;
	setCursorPosition(road[0][i]);
	cout << "|";

	setCursorPosition(road[1][i]);
	cout << " ";
	road[1][i].X += dx[1].X;
	if (dx[1].X < 0)
		dx[1].X--;
	else
		dx[1].X++;
	if (road[1][i].X < MAX_X / 2 + 4) road[1][i].X = MAX_X / 2 + 4;
	if (road[1][i].X > MAX_X) road[1][i].X = MAX_X;
	dx[1].Y--;
	setCursorPosition(road[1][i]);
	cout << "|";
}

bool check_r(COORD car, COORD road[2][ROAD_LEN], COORD trouble[ROAD_LEN], COORD gifts[ROAD_LEN], int& score)
{

	// road == car
	for (short i = 0; i < ROAD_LEN; i++) {
		if ((car.X == road[0][i].X && car.Y == road[0][i].Y) || (car.X == road[1][i].X && car.Y == road[1][i].Y))
		{
			return false;
		}
	}
	// trouble == car
	for (short i = 0; i < ROAD_LEN; i++) {
		if (car.X == trouble[i].X && car.Y == trouble[i].Y && trouble[i].X > 0) {
			return false;
		}
	}
	// gifts == car
	for (short i = 0; i < ROAD_LEN; i++) {
		if (car.X == gifts[i].X && car.Y == gifts[i].Y && gifts[i].X > 0) {
			score += 1;
			gifts[i].X = 0;
			setCursorPosition({ car.X, car.Y });
			cout << " ";
		}
	}
	
	return true;
}
void printCarGame()
{

	short startX = 38;
	short startY = 3;
	const char* art[] = {
		"     ____      _       ____  ",
		"    / ___|    / \\     |  _ \\ ",
		"   | |       / _ \\    | |_) |",
		"   | |___   / ___ \\   |  _ < ",
		"    \\____| /_/   \\_\\  |_| \\_\\",
		"",
		"  ____      _      __  __   _____ ",
		" / ___|    / \\    |  \\/  | | ____|",
		"| |  _    / _ \\   | |\\/| | |  _|  ",
		"| |_| |  / ___ \\  | |  | | | |___ ",
		" \\____| /_/   \\_\\ |_|  |_| |_____|"
	};

	for (int i = 0; i < 11; ++i) {
		setCursorPosition({ startX, SHORT(startY + i) });
		cout << art[i] << endl;
	}
}

void loadLeaderboard(lbEntry* arr, const char* filename) {
	ifstream fin(filename, ios::binary);
	if (!fin) {
		cout << "Error opening file for reading!" << endl;
		return;
		}
	for (int i = 0; i < LB_SIZE; i++) {
		fin.read((char*)&arr[i], sizeof(lbEntry));
	}
	fin.close();
}
void saveLeaderboard(lbEntry* arr, const char* filename) {
	ofstream fout(filename, ios::binary);
	if (!fout) {
		cout << "Error opening file for writing!" << endl;
		return;
	}
	for (int i = 0; i < LB_SIZE; i++) {
		fout.write((const char*)&arr[i], sizeof(lbEntry));
	}
	fout.close();
}
void addToLeaderboard(lbEntry* arr, const char* name, int score, int time) {
	int existing = -1;
	for (int i = 0; i < LB_SIZE; ++i) {
		if (strncmp(arr[i].name, name, 15) == 0 && arr[i].score > 0) {
			existing = i;
			break;
		}
	}
	if (existing != -1) {
		if (score < arr[existing].score) return;
		if (score == arr[existing].score && time >= arr[existing].time) return;
		// якщо новий результат кращий Ч видал€Їмо старий
		for (int i = existing; i < LB_SIZE - 1; ++i) {
			arr[i] = arr[i + 1];
		}
		arr[LB_SIZE - 1].score = 0;
		arr[LB_SIZE - 1].name[0] = 0;
		arr[LB_SIZE - 1].time = 0;
	}
	int pos = -1;
	for (int i = 0; i < LB_SIZE; ++i) {
		if (score > arr[i].score || (score == arr[i].score && (arr[i].score == 0 || time < arr[i].time))) {
			pos = i;
			break;
		}
	}
	if (pos == -1) return;
	for (int i = LB_SIZE - 1; i > pos; --i) {
		arr[i] = arr[i - 1];
	}
	strncpy(arr[pos].name, name, 15);
	arr[pos].name[15] = 0;
	arr[pos].score = score;
	arr[pos].time = time;
}
void printLeaderboard(lbEntry* arr, const char* title, short x, short y){
	setCursorPosition({ x,y });
	cout << title << endl;
	for (int i = 0; i < LB_SIZE; ++i) {
		setCursorPosition({ x, (short)(y + i + 1) });
		if (arr[i].score > 0) {
			cout << (i + 1) << ". " << arr[i].name << " - " << arr[i].score << " (" << arr[i].time << "s)" << endl;
		}
		else {
			cout << (i + 1) << ". ---" << endl;
		}
			
	}
}