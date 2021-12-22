#include <iostream>
#include <vector>
#include <cmath>
#include <Windows.h>
#include <conio.h>
#include "../../../ceerte.lib/ceerte.h"

using namespace std;

class Schtabellir
{
	struct position
	{
		double x, y;
		double angle;
	};

	struct color
	{
		int r, g, b;
	};

	struct size
	{
		int width, height;
	};

	vector <vector<unsigned int>> frame;
	position pos{ 0, 0 , 0};
	size robotSize{ 0 ,0 };
	size safeSize{ 0, 0 };
	int safePadding = 2;


	color getColor(unsigned int value) {
		union X
		{
			unsigned long number;
			unsigned char bytes[4];
		};
		X x{ value };
		return { x.bytes[1], x.bytes[2], x.bytes[3] };
	}

	double getSin(double angle) 
	{
		return sin(angle * 3.1415 / 180) * (-1);
	}

	double getCos(double angle)
	{
		return cos(angle * 3.1415 / 180);
	}

	void printPoint(int x, int y, int color) 
	{
		GotoXY(x, y);
		printColor(color);
	}

public:
	Schtabellir(vector <vector<unsigned int>> &frame) : frame(frame)
	{
		update(frame);
		safeSize = { (int)frame[0].size() - safePadding - robotSize.width, (int)frame.size() - safePadding - robotSize.width };
	}

	void update(vector <vector<unsigned int>> &frame)
	{
		position left{ 0,0 };
		position right{ 0,0 };

		for (int i = 0; i < frame.size(); i++)
		{
			for (int j = 0; j < frame[i].size(); j++)
			{
				color c = getColor(frame[i][j]);
				if (c.g >= 200) left = { (double) i, (double) j };
				if (c.b >= 200) right = { (double) i, (double) j };
			}
		}
		double proX = left.x - right.x;
		double proY = left.y - right.y;
		double size = sqrt(pow(proX, 2) + pow(proY, 2));
		robotSize = { (int) size };
		pos = { left.x - (proX) / 2 , left.y - (proY) / 2, asin(proX / size) * 180 / 3.1415 * (-1) };
	}

	void rotate(int relativeAngle) 
	{
		pos.angle += relativeAngle;
	}

	void rotation(int absoluteAngle)
	{
		pos.angle = absoluteAngle;
	}

	void move(int milimeters) 
	{
		pos.x += floor(milimeters * getCos(pos.angle));
		pos.y += floor(milimeters * getSin(pos.angle));
	}
	
	void render()
	{
		printPoint(pos.x * 2, pos.y, 4);
		printPoint((pos.x + getCos(pos.angle - 90) * (robotSize.width / 2)) * 2, pos.y + getSin(pos.angle - 90) * (robotSize.width / 2), 1);
		printPoint((pos.x + getCos(pos.angle + 90) * (robotSize.width / 2)) * 2, pos.y + getSin(pos.angle + 90) * (robotSize.width / 2), 2);
	}

	~Schtabellir()
	{

	}
};

int main() {
	vector <vector<unsigned int>> frame;
	for (int i = 0; i < 30; i++)
	{
		vector<unsigned int> row;
		for (int j = 0; j < 30; j++)
		{
			row.push_back(0);
		}
		frame.push_back(row);
	}

	//system("cls");
	SetTextColor(15);
	for (int i = 0; i < frame.size(); i++)
	{
		for (int j = 0; j < frame[i].size(); j++)
		{
			//cout << (char)219 << (char)219;
		}
		cout << endl;
	}

	frame[18][1] = 0x00FF0000;
	frame[1][1] = 0xFF000000;
	
	Schtabellir schtabellir(frame);
	//schtabellir.render();
	////schtabellir.rotate(180);
	////schtabellir.rotation(0);
	//schtabellir.move(20);
	///*schtabellir.render();
	//schtabellir.rotate(180);
	//schtabellir.move(7);
	//schtabellir.render();
	//schtabellir.rotation(0);
	//schtabellir.move(7);*/
	//schtabellir.render();
	//schtabellir.rotation(0);
	//schtabellir.move(10);
	schtabellir.render();
	char key;
	while (true)
	{
		if (_kbhit())				//отслеживает факт нажатия клавиши
		{
			key = _getch();			// принимает значения клавиш

			if (key == 27)	break;			// Escape -> выходит из цикла


			switch (key)
			{
			case 77:
				schtabellir.rotate(-15);
				break;
			case 75:
				schtabellir.rotate(15);
				break;
			case 72:
				schtabellir.move(2);
				break;
			case 80:
				schtabellir.move(-2);
				break;
			}
			system("cls");
			schtabellir.render();
		}
	}

	//shtabellir.getPosition();
	//system("pause");
	//_getch();
}