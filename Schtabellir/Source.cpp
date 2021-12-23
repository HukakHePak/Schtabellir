#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class Schtabellir
{
	struct position
	{
		int x, y;
		double angle;
	};

	struct color
	{
		int r, g, b;
	};

	unsigned int main_angle = 0;
	vector <vector<unsigned int>> frame;
	position pos;
	unsigned int size;

	color getColor(unsigned int value) {
		union X
		{
			unsigned long number;
			unsigned char bytes[4];
		};
		X x{value};
		return {x.bytes[1], x.bytes[2], x.bytes[3] };
	}

	double getSin() 
	{
		return sin(pos.angle * 3.1415 / 180) * (-1);
	}

	double getCos()
	{
		return cos(pos.angle * 3.1415 / 180);
	}

public:
	Schtabellir(vector <vector<unsigned int>> frame) : frame(frame)
	{
		position left{ 0,0 };
		position right{ 0,0 };

		for (int i = 0; i < frame.size(); i++)
		{
			for (int j = 0; j < frame[i].size(); j++)
			{
				color c = getColor(frame[i][j]);
				if (c.g >= 200) left = { i, j };
				if (c.b >= 200) right = { i, j };
			}
		}
		int proX = left.x - right.x;
		int proY = left.y - right.y;
		double size = sqrt(pow(proX, 2) + pow(proY, 2));
		pos = { left.x - (proX) / 2 , left.y - (proY) / 2, asin(proX / size) * 180 / 3.1415 * (-1)};
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
		pos.x += floor(milimeters * getCos());
		pos.y += floor(milimeters * getSin());
	}
	
	void render()
	{
		cout << "x = " << pos.x << endl << " y = " << pos.y << endl << "angle = " << pos.angle << endl;
	}

	~Schtabellir()
	{

	}
};

int main() {
	vector <vector<unsigned int>> frame;
	for (int i = 0; i < 6; i++)
	{
		vector<unsigned int> row;
		for (int j = 0; j < 8; j++)
		{
			row.push_back(0);
		}
		frame.push_back(row);
	}




	frame[2][0] = 0x00FF0000;
	frame[0][0] = 0xFF000000;
	
	Schtabellir schtabellir(frame);
	schtabellir.render();
	//schtabellir.rotate(180);
	schtabellir.rotation(0);
	schtabellir.move(7);
	/*schtabellir.render();
	schtabellir.rotate(180);
	schtabellir.move(7);
	schtabellir.render();
	schtabellir.rotation(0);
	schtabellir.move(7);*/
	schtabellir.render();
	//shtabellir.getPosition();
	system("pause");
}