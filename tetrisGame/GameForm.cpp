#include "GameForm.h"
#include <Windows.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace tetrisGame;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew GameForm);

	
	return 0;
}

ref class Glob {
public:
	static tetrisGame::GameField^ game;
	static Graphics^ g;
	static System::Drawing::Rectangle gf;

	static Image^ imageB = Image::FromFile("test.png");
	static Image^ imageP = Image::FromFile("test2.png");
	static Image^ imageC = Image::FromFile("test3.png");

	static int fallTime = 0;
};

tetrisGame::GameForm::GameForm(void)
{
	InitializeComponent();
	
	Glob::g = this->CreateGraphics();
	Glob::game = gcnew GameField;
	srand(time(0));
	this->DoubleBuffered = true;
	
	GameTimer->Interval = updateInterval;
	GameTimer->Start();
	
	Glob::game->NewGame();
	System::Drawing::Rectangle gf(0, 0, 800, 900);
	Glob::game->acceleration = 0;
	

}
tetrisGame::GameForm::~GameForm()
{
	{
		if (components)
		{
			delete components;
		}
	}
}



ref class coords {
public:
	int x;
	int y;

	coords% operator = (const coords %copy);
};

coords %coords::operator = (const coords %copy)
{
	if (this == %copy)
		return *this;

	x = copy.x;
	y = copy.y;

	return *this;
}



ref class tetrisGame::UserControlledPiece
{
public:	
	array<coords^ >^ cur;
	array<coords^ >^ buf;
	int type;
	int ColourNum;
	UserControlledPiece()
	{
		cur = gcnew array<coords^ >(4);
		buf = gcnew array<coords^ >(4);
		for (int i = 0; i < 4; i++)
		{
			cur[i] = gcnew coords;
		}
		for (int i = 0; i < 4; i++)
		{
			buf[i] = gcnew coords;
		}
	}

	

};
int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,7, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};







GameField::GameField() {
	for (int i = 0; i < 15; i++)
	{
		board[i] = new int[10];
		
		for (int m = 0; m < 10; m++)
			board[i][m] = 0;
	}
}


void GameField::NewGame()
{
	p = CreateRandom();
	play = 1;
	die = 0;

	
	Image^ image = Image::FromFile("test.png");
	Image^ image2 = Image::FromFile("test2.png");
	

	

}

UserControlledPiece^ GameField::CreateRandom()
{
	UserControlledPiece a; 
	

	for (int i = 0; i < 4; i++) board[a.buf[i]->y][a.buf[i]->x] = a.type;
	a.type = 1 + rand() % 7;
	int n = rand() % 7;
	a.ColourNum = 1 + rand() % 3;
	for (int i = 0; i < 4; i++)
	{
		a.cur[i]->x = figures[n][i] % 2;
		a.cur[i]->y = figures[n][i] / 2;
	}

	return %a;
}
bool GameField::checkCollision()
{
	for (int i = 0; i < 4; i++)
		if (p->cur[i]->x < 0 || p->cur[i]->x >= 10 || p->cur[i]->y >= 15) return 0;
		else if (board[p->cur[i]->y][p->cur[i]->x]) return 0;
	return 1;
}

void GameField::UpdateGame()
{

	if (Glob::fallTime % 3 == 0)
	for (int i = 0; i < 4; i++) { p->buf[i]->x = p->cur[i]->x; p->cur[i]->x += acceleration; }

	if (!checkCollision()) 
		for (int i = 0; i < 4; i++) 
			p->cur[i]->x = p->buf[i]->x;
Glob::fallTime += 1;

	if (Glob::fallTime >= 20)
	{
		for (int i = 0; i < 4; i++) {
			p->buf[i]->y = p->cur[i]->y;
			p->cur[i]->y += 1;
		}
		Glob::fallTime = 0;
	}

	if (!checkCollision())
	{

		int f = 1 + rand() % 7;
		for (int i = 0; i < 4; i++) 
		{ 
			if (this->p->buf[i]->y == 1)
				die = 1;
			this->board[p->buf[i]->y][p->cur[i]->x] = p->ColourNum; }

		p->ColourNum = 1 + rand() % 3;
		int n = rand() % 7;
		for (int i = 0; i < 4; i++)
		{
			p->cur[i]->x = (figures[n][i] % 2) +f;
			p->cur[i]->y = (figures[n][i] / 2);
		}
	}


	int k = 15 - 1;
	for (int i = 15 - 1; i > 0; i--)
	{
		int count = 0;
		for (int j = 0; j < 10; j++)
		{
			if (board[i][j]) count++;
			board[k][j] = board[i][j];
		}
		if (count < 10) k--;
	}
}

void GameField::Rotate()
{
	coords^ c = this->p->cur[1]; //center of rotation
	for (int i = 0; i < 4; i++)
	{
		int x = this->p->cur[i]->y - c->y;
		int y = this->p->cur[i]->x - c->x;
		this->p->cur[i]->x = c->x - x;
		this->p->cur[i]->y = c->y + y;
	}
	if (!this->checkCollision()) 
		for (int i = 0; i < 4; i++)
		{
			this->p->cur[i]->x = this->p->buf[i]->x;
			this->p->cur[i]->y = this->p->buf[i]->y;
		}
}

void GameField::Draw(System::Windows::Forms::PaintEventArgs^ e)
{
	Graphics^ g = e->Graphics;

	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 10; j++)
		{
			if (this->board[i][j] == 0) continue;
			g->DrawImage(GetImage(Glob::game->board[i][j]) , j * 60, i * 60, GetImage(Glob::game->board[i][j])->Size.Width, GetImage(this->p->ColourNum)->Size.Height);
		}
	for (int i = 0; i < 4; i++)
	{
		g->DrawImage(GetImage(this->p->ColourNum), this->p->cur[i]->x * 60, this->p->cur[i]->y * 60, GetImage(this->p->ColourNum)->Size.Width, GetImage(this->p->ColourNum)->Size.Height);
	}
}


void GameField::Clear(System::Windows::Forms::PaintEventArgs^ e)
{
	Graphics^ g = e->Graphics;
	g->Clear(Color::White);
}



System::Void tetrisGame::GameForm::GameForm_KeyDown(System::Object ^ sender, System::Windows::Forms::KeyEventArgs ^ e)
{
	if (e->KeyCode == Keys::Left)
		Glob::game->acceleration = -1;
	if (e->KeyCode == Keys::Right)
		Glob::game->acceleration = 1;
	if (e->KeyCode == Keys::Down)
		Glob::fallTime += 10;
	if (e->KeyCode == Keys::Space)
		Glob::game->Rotate();
}

System::Void tetrisGame::GameForm::GameForm_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
{
	Glob::game->acceleration = 0;
}

System::Void tetrisGame::GameForm::GameForm_KeyUp(System::Object ^ sender, System::Windows::Forms::KeyEventArgs ^ e)
{
	Glob::game->acceleration = 0;
}

void tetrisGame::GameForm::GameForm_Update(Object ^ object, EventArgs ^ e)
{

	if (Glob::game->play && !Glob::game->die)
	{
		Glob::game->UpdateGame();
		this->Refresh();
		Glob::game->Draw(%PaintEventArgs(Glob::g, Glob::gf));
	}
	if (Glob::game->die)
		abort;
}

Image ^ tetrisGame::GetImage(int num)
{


	if (num == 1)
		return Glob::imageB;
	else if (num == 2)
		return Glob::imageP;
	else if (num == 3)
		return Glob::imageC;
}
