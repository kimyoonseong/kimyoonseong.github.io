// Screen.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

void draw(char* loc, const char* face)
{
	strncpy(loc, face, strlen(face));
}

class Screen {
	int size;
	char* screen;

public:
	Screen(int sz) : size(sz), screen(new char[sz + 1]) {}
	~Screen() { delete[] screen; }

	void draw(int pos, const char* face) 
	{
		if (face == nullptr) return;
		if (pos < 0 || pos >= size) return;
		strncpy(&screen[pos], face, strlen(face));
	}

	void render() 
	{
		printf("%s\r", screen);
	}

	void clear()
	{
		memset(screen, ' ', size);
		screen[size] = '\0';
	}

	int length()
	{
		return size;
	}

};

class GameObject {
	int pos;
	int hp;
	char face[20];
	Screen* screen;

public:
	GameObject(int pos, const char* face, Screen* screen, int hp)
		: pos(pos), screen(screen)
	{
		strcpy(this->face, face);
	}
	int getPosition()
	{
		return pos;
	}
	void setPosition(int pos)
	{
		this->pos = pos;
	}
	void draw()
	{
		screen->draw(pos, face);
	}
	
	char* getFace()
	{
		return face;
	}

	void Face(const char* face)
	{
		strcpy(this->face, face);
	}
	
};

class Player : public GameObject {
	
	bool turn=true;
public:
	Player(int pos, const char* face, Screen* screen, int hp) 
		: GameObject(pos, face, screen,hp)
	{	
		
	}

	
	void moveLeft()
	{
		setPosition(getPosition() - 1);
		turn = false;
	}

	void moveRight()
	{
		setPosition(getPosition() + 1);
		turn = true;
	}
	
	bool getdirect() {
		return turn;
	}

	void update()
	{

	}

};

class Enemy : public GameObject {
	
public:
	Enemy(int pos, const char* face, Screen* screen,int hp) 
		: GameObject(pos, face, screen,hp)
	{
	}

	void moveRandom()
	{
		setPosition( getPosition() + rand() % 3 - 1);
	}

	void update()
	{
		moveRandom();
	}
};

class Bullet : public GameObject {
	bool isFiring;
	
	bool direct= false;
public:
	Bullet(int pos, const char* face, Screen* screen,int hp) 
		: GameObject(pos, face, screen,hp), isFiring(false)
	{
	}

	void moveLeft()
	{
		setPosition(getPosition() - 1);
	}

	void moveRight()
	{
		setPosition(getPosition() + 1);
	}

	void draw()
	{
		if (isFiring == false) return;
		GameObject::draw();
	}
	
	void fire(int player_pos, bool way)
	{
		isFiring = true;
		
		direct = way;

		if (direct)
		{
			setPosition(player_pos + 1);
		}
		else 
		{
			setPosition(player_pos - 1);
		}
	}

	void update(int enemy_pos)
	{
		if (isFiring == false) return;
		int pos = getPosition();

		if (pos < enemy_pos&&direct) {
			pos = pos + 1;
		}
		else if (pos < enemy_pos&&direct) {
			pos = pos + 1;
		}
		else if (pos !=0 &&direct==false) {
			pos = pos - 1;
		}
		else {
			isFiring = false;
		}
		
		setPosition(pos);
	}
};
class Laser : public GameObject {
	bool direct = false;
	bool isFiring;
public:
	Laser(int pos, const char* face, Screen* screen,int hp) :GameObject(pos, face, screen,hp),isFiring(false)
	{
	}

	void moveLeft()
	{
		setPosition(getPosition() - 1);
	}

	void moveRight()
	{
		setPosition(getPosition() + 1);
	}

	void draw()
	{
		if (isFiring == false) return;
		GameObject::draw();
	}
	
	void fire(int player_pos, bool way)
	{
		isFiring = true;
		direct = way;

		if (direct)
		{
			setPosition(player_pos + 1);
		}
		else
		{
			setPosition(player_pos - 1);
		}
	}

	void update(int enemy_pos)
	{
		if (isFiring == false) return;
		int pos = getPosition();

		if (pos < enemy_pos&&direct) {
			pos = pos + 1;
		}
		else if (pos < enemy_pos&&direct) {
			pos = pos + 1;
		}
		else if (pos != 0 && direct == false) {
			pos = pos - 1;
		}
		else {
			isFiring = false;
		}

		setPosition(pos);
	}

};

int main()
{
	Screen screen{ 80 };
	Player player = { 30, "(>_<)>", &screen,10 };
	Enemy enemy{ rand()%60, "(@--@)", &screen ,5};
	Enemy enemy2{ rand()%20, "(@--@)", &screen ,5 };
	Enemy enemy3{ rand() % 40, "(@--@)", &screen ,5 };
	Enemy enemy4{ rand() % 80, "(@--@)", &screen ,5 };
	Enemy enemy5{ rand() % 100, "(@--@)", &screen ,5 };
	Bullet bullet( -1, "*", &screen,1);
	Laser laser(-1, "---", &screen,1);
	bool LaOn = false;
	while (true)
	{
		screen.clear();

		if (_kbhit())
		{
			int c = _getch();
			switch (c) {
			case 'a':
				player.Face("<(>_<)");
				player.moveLeft();
				break;
			case 'd':
				player.Face("(>_<)>");
				player.moveRight();
				break;
			case ' ':
				if (LaOn) {
					laser.fire(player.getPosition(), player.getdirect());
				}
				else
					bullet.fire(player.getPosition(), player.getdirect());
				break;
			case 'm':
				if (LaOn ) {
					LaOn = false;
				}
				else
					LaOn = true;
				break;
			}
		}
		player.draw();
		enemy.draw();
		enemy2.draw();
		enemy3.draw();
		enemy4.draw();
		enemy5.draw();

		bullet.draw();
		laser.draw();
		player.update();
		enemy.update();
		enemy2.update();
		enemy3.update();
		enemy4.update();
		enemy5.update();
		bullet.update(enemy.getPosition());
		
		screen.render();
		Sleep(66);
	}

	return 0;
}