
#include "../../ceerte.lib/ceerte.h"
#include <iostream>
#include <cmath>
#include <conio.h>
#include "SFML/Graphics.hpp"

using namespace std;
using namespace sf;

class Schtabellir
{
protected:
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
		double width, height;
	};

	vector <vector<unsigned int>> frame;
	position robotPosition{ 0, 0 , 0 };
	size robotSize{ 0 ,0 };
	size frameSize{ 0, 0 };
	double safePadding = 0;
	double safeRadius = 0;
	color colorSensetivity = { 0,0,0 };

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

	bool isSafePosition(position pos)
	{
		return pos.x > safePadding + safeRadius
			&& pos.x < frameSize.width - (safeRadius + safePadding)
			&& pos.y > safePadding + safeRadius
			&& pos.y < frameSize.height - (safeRadius + safePadding);
	}
	
	void setFrameSize()
	{
		frameSize = { (double)frame.size(), (double)frame[0].size() };
	}

	void setSafeRadius()
	{
		safeRadius = sqrt(pow(robotSize.width, 2) + pow(robotSize.width, 2)) / 2;
	}


public:
	Schtabellir()
	{

	}

	Schtabellir(vector <vector<unsigned int>>& frame, double padding, color sensetivity) 
		: frame(frame), colorSensetivity(sensetivity), safePadding(padding / 20)
	{
		locate(frame);
		setFrameSize();
		setSafeRadius();
	}

	void locate(vector <vector<unsigned int>>& frame)
	{
		position left{ 0,0 };
		position right{ 0,0 };

		for (int i = 0; i < frame.size(); i++)
		{
			for (int j = 0; j < frame[i].size(); j++)
			{
				color c = getColor(frame[i][j]);
				if (c.g >= colorSensetivity.g) 
					left = { (double)i, (double)j };
				if (c.b >= colorSensetivity.g) 
					right = { (double)i, (double)j };
			}
		}
		double proectionX = left.x - right.x;
		double proectionY = left.y - right.y;
		double tangens = sqrt(pow(proectionX, 2) + pow(proectionY, 2));
		robotSize = { tangens , tangens};
		robotPosition = { left.x - (proectionX) / 2 , 
			left.y - (proectionY) / 2, asin(proectionX / tangens) * 180 / 3.1415 * (-1)};
	}

	void rotate(double relativeAngle)
	{
		robotPosition.angle += relativeAngle;
	}

	void rotation(double absoluteAngle)
	{
		robotPosition.angle = absoluteAngle;
	}

	bool move(double milimeters)
	{
		position newPos = { robotPosition.x + milimeters / 20 * getCos(robotPosition.angle),
			robotPosition.y + milimeters / 20 * getSin(robotPosition.angle), robotPosition.angle };
		if (isSafePosition(newPos))
		{
			robotPosition = newPos;
			return true;
		}
		return false;
	}

	position getPosition()
	{
		return robotPosition;
	}

	virtual void render()
	{
		printPoint(robotPosition.x * 2, robotPosition.y, 4);

		printPoint(robotPosition.x + getCos(robotPosition.angle - 90) * robotSize.width, 
			robotPosition.y + getSin(robotPosition.angle - 90) * (robotSize.width / 2), 1);

		printPoint(robotPosition.x + getCos(robotPosition.angle + 90) * robotSize.width, 
			robotPosition.y + getSin(robotPosition.angle + 90) * (robotSize.width / 2), 2);
	}

	~Schtabellir()
	{

	}
};


class GraphicSchtabellir : public Schtabellir
{
	RenderWindow window;
	CircleShape robotPoint;
	RectangleShape robotRectangle;
	CircleShape leftPoint;
	CircleShape rightPoint;
	RectangleShape safeZone;
	CircleShape robotSafeCircle;

public:
	GraphicSchtabellir(vector <vector<unsigned int>>& frame, double padding, color sensetivity)
	{
		this->frame = frame;
		this->colorSensetivity = sensetivity;
		this->safePadding = padding / 20;

		setFrameSize();
		

		window.create(VideoMode(frameSize.width, frameSize.height), "My window");
		window.clear(Color(0, 255, 0));
		
		locate(frame);
		setSafeRadius();

		robotPoint.setRadius(3.f);
		robotPoint.setFillColor(Color::Red);

		leftPoint.setRadius(3.f);
		leftPoint.setFillColor(Color::Green);

		rightPoint.setRadius(3.f);
		rightPoint.setFillColor(Color::Blue);

		robotRectangle.setSize(Vector2f(robotSize.width + 6, robotSize.width + 6));
		robotRectangle.setOrigin((robotSize.width + 6) / 2, (robotSize.width + 6) / 2);
		robotRectangle.setFillColor(Color(70, 70, 70));
			
		safeZone.setSize(Vector2f(frameSize.width - robotSize.width - safePadding * 2, 
			frameSize.height - robotSize.height - safePadding * 2));
		safeZone.setPosition(safePadding + robotSize.width / 2, safePadding + robotSize.height / 2);
		safeZone.setFillColor(Color(150, 150, 150));

		robotSafeCircle.setRadius(safeRadius + 3);
		robotSafeCircle.setOrigin(safeRadius, safeRadius);
		robotSafeCircle.setFillColor(Color(200, 200, 50));
	}

	virtual void render()
	{
		window.clear(Color(150, 0, 0));
		window.draw(safeZone);

		robotRectangle.setPosition(robotPosition.x + 3, robotPosition.y + 3);
		robotRectangle.setRotation(-robotPosition.angle - 90);
		robotPoint.setPosition(robotPosition.x, robotPosition.y);
		robotSafeCircle.setPosition(robotPosition.x, robotPosition.y);
		rightPoint.setPosition((robotPosition.x + getCos(robotPosition.angle - 90) * (robotSize.width / 2)), 
			robotPosition.y + getSin(robotPosition.angle - 90) * (robotSize.width / 2));
		leftPoint.setPosition((robotPosition.x + getCos(robotPosition.angle + 90) * (robotSize.width / 2)), 
			robotPosition.y + getSin(robotPosition.angle + 90) * (robotSize.width / 2));

		window.draw(robotSafeCircle);
		window.draw(robotRectangle);
		window.draw(robotPoint);
		window.draw(leftPoint);
		window.draw(rightPoint);

		window.display();
	}

	~GraphicSchtabellir()
	{
		window.close();
	}
};


int main() {
	vector <vector<unsigned int>> frame;
	for (int i = 0; i < 800; i++)
	{
		vector<unsigned int> row;
		for (int j = 0; j < 600; j++)
		{
			row.push_back(0);
		}
		frame.push_back(row);
	}

	frame[140][100] = 0x00FF0000;
	frame[100][100] = 0xFF000000;

	GraphicSchtabellir schtabellir(frame, 200, { 200, 200, 200});

	schtabellir.render();
	char key;
	while (true)
	{
		if (_kbhit())
		{
			key = _getch();

			if (key == 27)	break;

			switch (key)
			{
			case 77:
				schtabellir.rotate(-10);
				break;
			case 75:
				schtabellir.rotate(10);
				break;
			case 72:
				schtabellir.move(200);
				break;
			case 80:
				schtabellir.move(-200);
				break;
			}
			system("cls");
			schtabellir.render();
		}
	}
}