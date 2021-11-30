#pragma once
#using <system.drawing.dll>
#include <time.h>

namespace tetrisGame {
	
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class GameForm : public System::Windows::Forms::Form
	{
	public:
		GameForm(void);
	protected:
		~GameForm();
	private: System::Windows::Forms::Timer^  GameTimer;
	private: System::ComponentModel::IContainer^  components;
	private:
#pragma region Windows Form Designer generated code

		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->GameTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->SuspendLayout();
			// 
			// GameTimer
			// 
			this->GameTimer->Enabled = true;
			this->GameTimer->Interval = 200;
			this->GameTimer->Tick += gcnew System::EventHandler(this, &GameForm::GameForm_Update);
			// 
			// GameForm
			// 
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::None;
			this->ClientSize = System::Drawing::Size(600, 900);
			this->DoubleBuffered = true;
			this->KeyPreview = true;
			this->MaximumSize = System::Drawing::Size(610, 930);
			this->MinimumSize = System::Drawing::Size(610, 930);
			this->Name = L"GameForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"This aint my first rodeo";
			this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &GameForm::GameForm_KeyDown);
			this->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &GameForm::GameForm_KeyPress);
			this->KeyUp += gcnew System::Windows::Forms::KeyEventHandler(this, &GameForm::GameForm_KeyUp);
			this->ResumeLayout(false);

		}
#pragma endregion

	private: System::Void GameForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void GameForm_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e);
	private: System::Void GameForm_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);

	private: void GameForm_Update(Object^ object, EventArgs^ e);
	private: float updateInterval = 20;
	};

	ref class UserControlledPiece;
	ref class GameField
		{
		public:

			GameField();

			array<int*>^ board = gcnew array<int*>(15);
			UserControlledPiece^ p;
			int acceleration;
			bool play, die;
			UserControlledPiece^ CreateRandom();
			bool checkCollision();

			void Rotate();
			void NewGame();
			void UpdateGame();
			void Draw(System::Windows::Forms::PaintEventArgs^ e);
			void Clear(System::Windows::Forms::PaintEventArgs^ e);
	
	};

Image^ GetImage(int num);
}
