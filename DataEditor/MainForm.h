#pragma once

#include <TableAll.h>
#include "ListData.h"

TableContainer* container;
TableFileNameList fileNameList;
CNtlBitFlagManager flagManager;

namespace DataEditor {

	using namespace System;
	using namespace System::Text;
	using namespace System::Text::RegularExpressions;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TabControl^  mainTabs;
	private: System::Windows::Forms::TabPage^  worldTblTab;
	private: System::Windows::Forms::TabPage^  tabTextAll;
	protected:

	protected:


	private: System::Windows::Forms::MenuStrip^  mainMenu;

	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;
	private: System::Windows::Forms::ListBox^  worldTblList;
	private: System::Windows::Forms::ToolStripMenuItem^  loadGameDataToolStripMenuItem;
	private: System::Windows::Forms::Panel^  bodyPanel;
	private: System::Windows::Forms::Button^  worldTblSave;

	private: System::Windows::Forms::Button^  worldTblLoad;
	private: System::Windows::Forms::RichTextBox^  log;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::TextBox^  fldWorldTblName;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::CheckBox^  cbWorldTblDynamic;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  fldWorldTblCreateCount;
	private: System::Windows::Forms::TextBox^  fldWorldTblDoorType;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  fldWorldTblNpcSp;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::TextBox^  fldWorldTblMobSp;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Button^  btnTextAllSave;

	private: System::Windows::Forms::Button^  btnTextAllTblLoad;

	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::ListBox^  textAllTblList;
	private: System::Windows::Forms::RichTextBox^  strTextAllTbl;

















	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->mainTabs = (gcnew System::Windows::Forms::TabControl());
			this->worldTblTab = (gcnew System::Windows::Forms::TabPage());
			this->worldTblSave = (gcnew System::Windows::Forms::Button());
			this->worldTblLoad = (gcnew System::Windows::Forms::Button());
			this->bodyPanel = (gcnew System::Windows::Forms::Panel());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->fldWorldTblNpcSp = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->fldWorldTblMobSp = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->fldWorldTblDoorType = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->fldWorldTblCreateCount = (gcnew System::Windows::Forms::TextBox());
			this->cbWorldTblDynamic = (gcnew System::Windows::Forms::CheckBox());
			this->fldWorldTblName = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->worldTblList = (gcnew System::Windows::Forms::ListBox());
			this->tabTextAll = (gcnew System::Windows::Forms::TabPage());
			this->btnTextAllSave = (gcnew System::Windows::Forms::Button());
			this->btnTextAllTblLoad = (gcnew System::Windows::Forms::Button());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->textAllTblList = (gcnew System::Windows::Forms::ListBox());
			this->log = (gcnew System::Windows::Forms::RichTextBox());
			this->mainMenu = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->loadGameDataToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->strTextAllTbl = (gcnew System::Windows::Forms::RichTextBox());
			this->mainTabs->SuspendLayout();
			this->worldTblTab->SuspendLayout();
			this->bodyPanel->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->tabTextAll->SuspendLayout();
			this->panel1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->mainMenu->SuspendLayout();
			this->SuspendLayout();
			// 
			// mainTabs
			// 
			this->mainTabs->Controls->Add(this->worldTblTab);
			this->mainTabs->Controls->Add(this->tabTextAll);
			this->mainTabs->Dock = System::Windows::Forms::DockStyle::Fill;
			this->mainTabs->Location = System::Drawing::Point(0, 24);
			this->mainTabs->Name = L"mainTabs";
			this->mainTabs->SelectedIndex = 0;
			this->mainTabs->Size = System::Drawing::Size(746, 452);
			this->mainTabs->TabIndex = 0;
			// 
			// worldTblTab
			// 
			this->worldTblTab->Controls->Add(this->worldTblSave);
			this->worldTblTab->Controls->Add(this->worldTblLoad);
			this->worldTblTab->Controls->Add(this->bodyPanel);
			this->worldTblTab->Controls->Add(this->worldTblList);
			this->worldTblTab->Location = System::Drawing::Point(4, 22);
			this->worldTblTab->Name = L"worldTblTab";
			this->worldTblTab->Padding = System::Windows::Forms::Padding(3);
			this->worldTblTab->Size = System::Drawing::Size(738, 426);
			this->worldTblTab->TabIndex = 0;
			this->worldTblTab->Text = L"World Table";
			this->worldTblTab->UseVisualStyleBackColor = true;
			// 
			// worldTblSave
			// 
			this->worldTblSave->Enabled = false;
			this->worldTblSave->Location = System::Drawing::Point(169, 392);
			this->worldTblSave->Name = L"worldTblSave";
			this->worldTblSave->Size = System::Drawing::Size(75, 28);
			this->worldTblSave->TabIndex = 3;
			this->worldTblSave->Text = L"Save";
			this->worldTblSave->UseVisualStyleBackColor = true;
			this->worldTblSave->Click += gcnew System::EventHandler(this, &MainForm::worldTblSave_Click);
			// 
			// worldTblLoad
			// 
			this->worldTblLoad->Enabled = false;
			this->worldTblLoad->Location = System::Drawing::Point(8, 392);
			this->worldTblLoad->Name = L"worldTblLoad";
			this->worldTblLoad->Size = System::Drawing::Size(75, 28);
			this->worldTblLoad->TabIndex = 2;
			this->worldTblLoad->Text = L"Load";
			this->worldTblLoad->UseVisualStyleBackColor = true;
			this->worldTblLoad->Click += gcnew System::EventHandler(this, &MainForm::worldTblLoad_Click);
			// 
			// bodyPanel
			// 
			this->bodyPanel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->bodyPanel->Controls->Add(this->groupBox1);
			this->bodyPanel->Location = System::Drawing::Point(250, 6);
			this->bodyPanel->Name = L"bodyPanel";
			this->bodyPanel->Size = System::Drawing::Size(480, 417);
			this->bodyPanel->TabIndex = 1;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->fldWorldTblNpcSp);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->fldWorldTblMobSp);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->fldWorldTblDoorType);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->fldWorldTblCreateCount);
			this->groupBox1->Controls->Add(this->cbWorldTblDynamic);
			this->groupBox1->Controls->Add(this->fldWorldTblName);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->groupBox1->Location = System::Drawing::Point(0, 0);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(480, 417);
			this->groupBox1->TabIndex = 5;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"World Data";
			// 
			// fldWorldTblNpcSp
			// 
			this->fldWorldTblNpcSp->Location = System::Drawing::Point(106, 95);
			this->fldWorldTblNpcSp->Name = L"fldWorldTblNpcSp";
			this->fldWorldTblNpcSp->Size = System::Drawing::Size(364, 20);
			this->fldWorldTblNpcSp->TabIndex = 11;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(7, 98);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(93, 13);
			this->label5->TabIndex = 10;
			this->label5->Text = L"Npc Spawn Table";
			// 
			// fldWorldTblMobSp
			// 
			this->fldWorldTblMobSp->Location = System::Drawing::Point(106, 69);
			this->fldWorldTblMobSp->Name = L"fldWorldTblMobSp";
			this->fldWorldTblMobSp->Size = System::Drawing::Size(364, 20);
			this->fldWorldTblMobSp->TabIndex = 9;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(8, 72);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(94, 13);
			this->label4->TabIndex = 8;
			this->label4->Text = L"Mob Spawn Table";
			// 
			// fldWorldTblDoorType
			// 
			this->fldWorldTblDoorType->Location = System::Drawing::Point(230, 43);
			this->fldWorldTblDoorType->Name = L"fldWorldTblDoorType";
			this->fldWorldTblDoorType->Size = System::Drawing::Size(100, 20);
			this->fldWorldTblDoorType->TabIndex = 7;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(167, 46);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(57, 13);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Door Type";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(8, 46);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(69, 13);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Create Count";
			// 
			// fldWorldTblCreateCount
			// 
			this->fldWorldTblCreateCount->Location = System::Drawing::Point(83, 43);
			this->fldWorldTblCreateCount->Name = L"fldWorldTblCreateCount";
			this->fldWorldTblCreateCount->Size = System::Drawing::Size(78, 20);
			this->fldWorldTblCreateCount->TabIndex = 4;
			// 
			// cbWorldTblDynamic
			// 
			this->cbWorldTblDynamic->AutoSize = true;
			this->cbWorldTblDynamic->Location = System::Drawing::Point(362, 19);
			this->cbWorldTblDynamic->Name = L"cbWorldTblDynamic";
			this->cbWorldTblDynamic->Size = System::Drawing::Size(67, 17);
			this->cbWorldTblDynamic->TabIndex = 3;
			this->cbWorldTblDynamic->Text = L"Dynamic";
			this->cbWorldTblDynamic->UseVisualStyleBackColor = true;
			// 
			// fldWorldTblName
			// 
			this->fldWorldTblName->Location = System::Drawing::Point(48, 17);
			this->fldWorldTblName->Name = L"fldWorldTblName";
			this->fldWorldTblName->Size = System::Drawing::Size(308, 20);
			this->fldWorldTblName->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(7, 20);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(35, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Name";
			// 
			// worldTblList
			// 
			this->worldTblList->FormattingEnabled = true;
			this->worldTblList->Location = System::Drawing::Point(8, 6);
			this->worldTblList->Name = L"worldTblList";
			this->worldTblList->Size = System::Drawing::Size(236, 381);
			this->worldTblList->TabIndex = 0;
			this->worldTblList->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::worldTblList_SelectedIndexChanged);
			// 
			// tabTextAll
			// 
			this->tabTextAll->Controls->Add(this->btnTextAllSave);
			this->tabTextAll->Controls->Add(this->btnTextAllTblLoad);
			this->tabTextAll->Controls->Add(this->panel1);
			this->tabTextAll->Controls->Add(this->textAllTblList);
			this->tabTextAll->Location = System::Drawing::Point(4, 22);
			this->tabTextAll->Name = L"tabTextAll";
			this->tabTextAll->Padding = System::Windows::Forms::Padding(3);
			this->tabTextAll->Size = System::Drawing::Size(738, 426);
			this->tabTextAll->TabIndex = 1;
			this->tabTextAll->Text = L"Text All Table";
			this->tabTextAll->UseVisualStyleBackColor = true;
			// 
			// btnTextAllSave
			// 
			this->btnTextAllSave->Enabled = false;
			this->btnTextAllSave->Location = System::Drawing::Point(169, 391);
			this->btnTextAllSave->Name = L"btnTextAllSave";
			this->btnTextAllSave->Size = System::Drawing::Size(75, 28);
			this->btnTextAllSave->TabIndex = 7;
			this->btnTextAllSave->Text = L"Save";
			this->btnTextAllSave->UseVisualStyleBackColor = true;
			// 
			// btnTextAllTblLoad
			// 
			this->btnTextAllTblLoad->Enabled = false;
			this->btnTextAllTblLoad->Location = System::Drawing::Point(8, 391);
			this->btnTextAllTblLoad->Name = L"btnTextAllTblLoad";
			this->btnTextAllTblLoad->Size = System::Drawing::Size(75, 28);
			this->btnTextAllTblLoad->TabIndex = 6;
			this->btnTextAllTblLoad->Text = L"Load";
			this->btnTextAllTblLoad->UseVisualStyleBackColor = true;
			this->btnTextAllTblLoad->Click += gcnew System::EventHandler(this, &MainForm::btnTextAllTblLoad_Click);
			// 
			// panel1
			// 
			this->panel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->panel1->Controls->Add(this->groupBox2);
			this->panel1->Location = System::Drawing::Point(250, 5);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(480, 417);
			this->panel1->TabIndex = 5;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->strTextAllTbl);
			this->groupBox2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->groupBox2->Location = System::Drawing::Point(0, 0);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(480, 417);
			this->groupBox2->TabIndex = 5;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Text All Data";
			// 
			// textAllTblList
			// 
			this->textAllTblList->FormattingEnabled = true;
			this->textAllTblList->Location = System::Drawing::Point(8, 5);
			this->textAllTblList->Name = L"textAllTblList";
			this->textAllTblList->Size = System::Drawing::Size(236, 381);
			this->textAllTblList->TabIndex = 4;
			this->textAllTblList->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::textAllTblList_SelectedIndexChanged);
			// 
			// log
			// 
			this->log->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->log->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->log->Location = System::Drawing::Point(0, 476);
			this->log->Name = L"log";
			this->log->ReadOnly = true;
			this->log->Size = System::Drawing::Size(746, 70);
			this->log->TabIndex = 4;
			this->log->Text = L"";
			// 
			// mainMenu
			// 
			this->mainMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
			this->mainMenu->Location = System::Drawing::Point(0, 0);
			this->mainMenu->Name = L"mainMenu";
			this->mainMenu->Size = System::Drawing::Size(746, 24);
			this->mainMenu->TabIndex = 1;
			this->mainMenu->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->loadGameDataToolStripMenuItem,
					this->exitToolStripMenuItem
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// loadGameDataToolStripMenuItem
			// 
			this->loadGameDataToolStripMenuItem->Name = L"loadGameDataToolStripMenuItem";
			this->loadGameDataToolStripMenuItem->Size = System::Drawing::Size(161, 22);
			this->loadGameDataToolStripMenuItem->Text = L"Load Game Data";
			this->loadGameDataToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::loadGameDataToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(161, 22);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::exitToolStripMenuItem_Click);
			// 
			// strTextAllTbl
			// 
			this->strTextAllTbl->Dock = System::Windows::Forms::DockStyle::Fill;
			this->strTextAllTbl->Location = System::Drawing::Point(3, 16);
			this->strTextAllTbl->Name = L"strTextAllTbl";
			this->strTextAllTbl->Size = System::Drawing::Size(474, 398);
			this->strTextAllTbl->TabIndex = 0;
			this->strTextAllTbl->Text = L"";
			// 
			// MainForm
			// 
			this->AllowDrop = true;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(746, 546);
			this->Controls->Add(this->mainTabs);
			this->Controls->Add(this->log);
			this->Controls->Add(this->mainMenu);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->Text = L"DBO Game Data Table Editor";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->mainTabs->ResumeLayout(false);
			this->worldTblTab->ResumeLayout(false);
			this->bodyPanel->ResumeLayout(false);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->tabTextAll->ResumeLayout(false);
			this->panel1->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->mainMenu->ResumeLayout(false);
			this->mainMenu->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		Application::Exit();
	}
private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) {
	container = new TableContainer();
	flagManager.Create(TableContainer::TABLE_COUNT);
	fileNameList.Create();

	flagManager.Set(TableContainer::TABLE_WORLD);
	flagManager.Set(TableContainer::TABLE_PC);
	flagManager.Set(TableContainer::TABLE_MOB);
	flagManager.Set(TableContainer::TABLE_NPC);
	flagManager.Set(TableContainer::TABLE_ITEM);
	flagManager.Set(TableContainer::TABLE_ITEM_OPTION);
	flagManager.Set(TableContainer::TABLE_SKILL);
	flagManager.Set(TableContainer::TABLE_SYSTEM_EFFECT);
	flagManager.Set(TableContainer::TABLE_NEWBIE);
	flagManager.Set(TableContainer::TABLE_MERCHANT);
	flagManager.Set(TableContainer::TABLE_HTB_SET);
	flagManager.Set(TableContainer::TABLE_USE_ITEM);
	flagManager.Set(TableContainer::TABLE_SET_ITEM);
	flagManager.Set(TableContainer::TABLE_CHARM);
	flagManager.Set(TableContainer::TABLE_ACTION);
	flagManager.Set(TableContainer::TABLE_CHAT_COMMAND);
	flagManager.Set(TableContainer::TABLE_QUEST_ITEM);
	flagManager.Set(TableContainer::TABLE_QUEST_TEXT_DATA);
	flagManager.Set(TableContainer::TABLE_TEXT_ALL);
	flagManager.Set(TableContainer::TABLE_OBJECT);
	flagManager.Set(TableContainer::TABLE_WORLD_MAP);
	flagManager.Set(TableContainer::TABLE_LAND_MARK);
	flagManager.Set(TableContainer::TABLE_HELP);
	flagManager.Set(TableContainer::TABLE_GUIDE_HINT);
	flagManager.Set(TableContainer::TABLE_DRAGONBALL);
	flagManager.Set(TableContainer::TABLE_DRAGONBALL_REWARD);
	flagManager.Set(TableContainer::TABLE_TIMEQUEST);
	flagManager.Set(TableContainer::TABLE_BUDOKAI);
	flagManager.Set(TableContainer::TABLE_RANKBATTLE);
	flagManager.Set(TableContainer::TABLE_DIRECTION_LINK);
	flagManager.Set(TableContainer::TABLE_CHATTING_FILTER);
	flagManager.Set(TableContainer::TABLE_PORTAL);
	flagManager.Set(TableContainer::TABLE_SPEECH);
	flagManager.Set(TableContainer::TABLE_SCRIPT_LINK);
	flagManager.Set(TableContainer::TABLE_QUEST_NARRATION);
	flagManager.Set(TableContainer::TABLE_VEHICLE);
	flagManager.Set(TableContainer::TABLE_DUNGEON);
	flagManager.Set(TableContainer::TABLE_MOB_MOVE_PATTERN);
	flagManager.Set(TableContainer::TABLE_DYNAMIC_OBJECT);
	flagManager.Set(TableContainer::TABLE_ITEM_RECIPE);
	flagManager.Set(TableContainer::TABLE_ITEM_UPGRADE);
	flagManager.Set(TableContainer::TABLE_MIX_MACHINE);
	flagManager.Set(TableContainer::TABLE_DOJO);
	flagManager.Set(TableContainer::TABLE_QUEST_REWARD);
	flagManager.Set(TableContainer::TABLE_WORLD_ZONE);
	flagManager.Set(TableContainer::TABLE_NPC_SPAWN);
	flagManager.Set(TableContainer::TABLE_MOB_SPAWN);
	flagManager.Set(TableContainer::TABLE_FORMULA);
	flagManager.Set(TableContainer::TABLE_GAME_MANIA_TIME);
	flagManager.Set(TableContainer::TABLE_BASIC_DROP);
	flagManager.Set(TableContainer::TABLE_LEGENDARY_DROP);
	flagManager.Set(TableContainer::TABLE_NORMAL_DROP);
	flagManager.Set(TableContainer::TABLE_SUPERIOR_DROP);
	flagManager.Set(TableContainer::TABLE_EACH_DROP);
	flagManager.Set(TableContainer::TABLE_TYPE_DROP);
	flagManager.Set(TableContainer::TABLE_EXCELLENT_DROP);
	flagManager.Set(TableContainer::TABLE_EXP);

	fileNameList.SetFileName(TableContainer::TABLE_WORLD, "Table_World_Data");
	fileNameList.SetFileName(TableContainer::TABLE_PC, "Table_PC_Data");
	fileNameList.SetFileName(TableContainer::TABLE_MOB, "Table_MOB_Data");
	fileNameList.SetFileName(TableContainer::TABLE_NPC, "Table_NPC_Data");
	fileNameList.SetFileName(TableContainer::TABLE_ITEM, "Table_Item_Data");
	fileNameList.SetFileName(TableContainer::TABLE_ITEM_OPTION, "Table_Item_Option_Data");
	fileNameList.SetFileName(TableContainer::TABLE_SKILL, "Table_Skill_Data");
	fileNameList.SetFileName(TableContainer::TABLE_SYSTEM_EFFECT, "Table_System_Effect_Data");
	fileNameList.SetFileName(TableContainer::TABLE_NEWBIE, "Table_Newbie_Data");
	fileNameList.SetFileName(TableContainer::TABLE_MERCHANT, "Table_Merchant_Data");
	fileNameList.SetFileName(TableContainer::TABLE_HTB_SET, "Table_HTB_Set_Data");
	fileNameList.SetFileName(TableContainer::TABLE_USE_ITEM, "Table_Use_Item_Data");
	fileNameList.SetFileName(TableContainer::TABLE_SET_ITEM, "Table_Set_Item_Data");
	fileNameList.SetFileName(TableContainer::TABLE_CHARM, "Table_Charm_Data");
	fileNameList.SetFileName(TableContainer::TABLE_ACTION, "Table_Action_Data");
	fileNameList.SetFileName(TableContainer::TABLE_CHAT_COMMAND, "Table_Chat_Command_Data");
	fileNameList.SetFileName(TableContainer::TABLE_QUEST_ITEM, "Table_Quest_Item_Data");
	fileNameList.SetFileName(TableContainer::TABLE_QUEST_TEXT_DATA, "Table_Quest_Text_Data");
	fileNameList.SetFileName(TableContainer::TABLE_TEXT_ALL, "Table_Text_All_Data");
	fileNameList.SetFileName(TableContainer::TABLE_OBJECT, "Table_Object");
	fileNameList.SetFileName(TableContainer::TABLE_WORLD_MAP, "Table_Worldmap_Data");
	fileNameList.SetFileName(TableContainer::TABLE_LAND_MARK, "Table_Landmark_Data");
	fileNameList.SetFileName(TableContainer::TABLE_HELP, "Table_Help_Data");
	fileNameList.SetFileName(TableContainer::TABLE_GUIDE_HINT, "Table_Guide_Hint_Data");
	fileNameList.SetFileName(TableContainer::TABLE_DRAGONBALL, "Table_Dragon_Ball_Data");
	fileNameList.SetFileName(TableContainer::TABLE_DRAGONBALL_REWARD, "Table_DB_Reward_Data");
	fileNameList.SetFileName(TableContainer::TABLE_TIMEQUEST, "Table_TMQ_Data");
	fileNameList.SetFileName(TableContainer::TABLE_BUDOKAI, "Table_Tenkaichibudokai_Data");
	fileNameList.SetFileName(TableContainer::TABLE_RANKBATTLE, "Table_RankBattle_Data");
	fileNameList.SetFileName(TableContainer::TABLE_DIRECTION_LINK, "Table_Direction_Link_Data");
	fileNameList.SetFileName(TableContainer::TABLE_CHATTING_FILTER, "Table_Chatting_Filter_Data");
	fileNameList.SetFileName(TableContainer::TABLE_PORTAL, "Table_Portal_Data");
	fileNameList.SetFileName(TableContainer::TABLE_SPEECH, "Table_NPC_Speech_Data");
	fileNameList.SetFileName(TableContainer::TABLE_SCRIPT_LINK, "Table_Script_Link_Data");
	fileNameList.SetFileName(TableContainer::TABLE_QUEST_NARRATION, "Table_Quest_Narration_Data");
	fileNameList.SetFileName(TableContainer::TABLE_VEHICLE, "Table_Vehicle_Data");
	fileNameList.SetFileName(TableContainer::TABLE_DUNGEON, "Table_Dungeon_Data");
	fileNameList.SetFileName(TableContainer::TABLE_MOB_MOVE_PATTERN, "Table_Mob_Move_Pattern_Data");
	fileNameList.SetFileName(TableContainer::TABLE_DYNAMIC_OBJECT, "Table_Dynamic_Object_Data");
	fileNameList.SetFileName(TableContainer::TABLE_ITEM_RECIPE, "Table_Item_Recipe_Data");
	fileNameList.SetFileName(TableContainer::TABLE_ITEM_UPGRADE, "Table_Item_Upgrade_Data");
	fileNameList.SetFileName(TableContainer::TABLE_MIX_MACHINE, "Table_Item_Mix_Machine_Data");
	fileNameList.SetFileName(TableContainer::TABLE_DOJO, "Table_Dojo_Data");
	fileNameList.SetFileName(TableContainer::TABLE_QUEST_REWARD, "Table_Quest_Reward_Data");
	fileNameList.SetFileName(TableContainer::TABLE_WORLD_ZONE, "Table_World_Zone_Data");
	fileNameList.SetFileName(TableContainer::TABLE_FORMULA, "TD_Formula");
	fileNameList.SetFileName(TableContainer::TABLE_GAME_MANIA_TIME, "Table_GameManiaTime_Data");
	fileNameList.SetFileName(TableContainer::TABLE_EXP, "table_exp_data");
	fileNameList.SetFileName(TableContainer::TABLE_BASIC_DROP, "table_basic_drop_data");
	fileNameList.SetFileName(TableContainer::TABLE_LEGENDARY_DROP, "table_legendary_drop_data");
	fileNameList.SetFileName(TableContainer::TABLE_SYSTEM_EFFECT, "table_system_effect_data");

	this->log->AppendText("---------------------------------------------------------\nTo start go File > Load Game Data\n---------------------------------------------------------\n\n");
}
private: System::Void loadGameDataToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	this->log->AppendText("Loading game data ...\n");
	if (container->Create(flagManager, "..\\Tables", &fileNameList, eLOADING_METHOD::LOADING_METHOD_SECURED_BINARY, GetACP(), NULL))
	{
		this->worldTblLoad->Enabled = true;
		//this->worldTblSave->Enabled = true;
		this->btnTextAllTblLoad->Enabled = true;
		this->log->AppendText("Game Data Loaded!\n");
	}
	else
		this->log->AppendText("Failed to load Game Data!\n");
}
private: System::Void worldTblLoad_Click(System::Object^  sender, System::EventArgs^  e) {
	List<ListData^>^ data = gcnew List<ListData^>();
	
	for (Table::TABLEIT iter = container->GetWorldTable()->Begin(); container->GetWorldTable()->End() != iter; iter++)
	{
		sWORLD_TBLDAT* pWorldTableData = (sWORLD_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", gcnew String(pWorldTableData->szName), pWorldTableData->tblidx);
		it->Value = pWorldTableData->tblidx;

		data->Add(it);
	}

	this->worldTblList->DisplayMember = "Text";
	this->worldTblList->ValueMember = "Value";
	this->worldTblList->DataSource = data;
}
private: System::Void worldTblList_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	int value = ((ListData^)this->worldTblList->SelectedItem)->Value;

	sWORLD_TBLDAT data = *(sWORLD_TBLDAT*)container->GetWorldTable()->FindData(value);

	this->fldWorldTblName->Text = gcnew String(data.szName);
	this->cbWorldTblDynamic->Checked = data.bDynamic;
	this->fldWorldTblCreateCount->Text = String::Format("{0}", data.nCreateCount);
	this->fldWorldTblDoorType->Text = String::Format("{0}", data.byDoorType);
	this->fldWorldTblMobSp->Text = gcnew String(data.wszMobSpawn_Table_Name);
	this->fldWorldTblNpcSp->Text = gcnew String(data.wszNpcSpawn_Table_Name);
}
private: System::Void worldTblSave_Click(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void btnTextAllTblLoad_Click(System::Object^  sender, System::EventArgs^  e) {
	List<ListData^>^ data = gcnew List<ListData^>();

	TextTable* pTextTbl = container->GetTextAllTable()->GetActionTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "Action", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetChatCommandTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "ChatCommand", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetCSTextTble();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "CSText", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetDBRewardTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "DBReward", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetDragonTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "Dragon", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetETCTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "ETC", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetFilteringTable();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "Filtering", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetGMToolTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "GMTool", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetHelpPopoTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "HelpPopo", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetHelpTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "Help", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetHTBSetTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "HTBSet", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetItemTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "NormalItem", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetMapNameTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "MapName", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetMerchantTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "Merchant", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetMilePostTable();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "MilePost", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetMobTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "Mob", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetNpcDialogTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "NpcDialog", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetNPCTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "NPC", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetObjectTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "Object", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetQuestItemTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "QuestItem", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetSkillTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "Skill", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetSystemEffectTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "SystemEffect", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetTMQTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "TMQ", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetUseItemTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "UseItem", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}
	pTextTbl = container->GetTextAllTable()->GetDBOTipTbl();
	for (Table::TABLEIT iter = pTextTbl->Begin(); pTextTbl->End() != iter; iter++)
	{
		sTEXT_TBLDAT* pData = (sTEXT_TBLDAT*)(iter->second);
		ListData^ it = gcnew ListData();

		it->Text = String::Format("{0} [{1}]", "DBOTip", pData->tblidx);
		it->Value = pData->tblidx;

		data->Add(it);
	}

	this->textAllTblList->DisplayMember = "Text";
	this->textAllTblList->ValueMember = "Value";
	this->textAllTblList->DataSource = data;
}
private: System::Void textAllTblList_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	int value = ((ListData^)this->textAllTblList->SelectedItem)->Value;
	String^ text = ((ListData^)this->textAllTblList->SelectedItem)->Text;

	TextTable* pTextTbl= NULL;

	if (Regex::IsMatch(text, "Action"))
		pTextTbl = container->GetTextAllTable()->GetActionTbl();
	if (Regex::IsMatch(text, "ChatCommand"))
		pTextTbl = container->GetTextAllTable()->GetChatCommandTbl();
	if (Regex::IsMatch(text, "CSText"))
		pTextTbl = container->GetTextAllTable()->GetCSTextTble();
	if (Regex::IsMatch(text, "DBReward"))
		pTextTbl = container->GetTextAllTable()->GetDBRewardTbl();
	if (Regex::IsMatch(text, "Dragon"))
		pTextTbl = container->GetTextAllTable()->GetDragonTbl();
	if (Regex::IsMatch(text, "ETC"))
		pTextTbl = container->GetTextAllTable()->GetETCTbl();
	if (Regex::IsMatch(text, "Filtering"))
		pTextTbl = container->GetTextAllTable()->GetFilteringTable();
	if (Regex::IsMatch(text, "GMTool"))
		pTextTbl = container->GetTextAllTable()->GetGMToolTbl();
	if (Regex::IsMatch(text, "HelpPopo"))
		pTextTbl = container->GetTextAllTable()->GetHelpPopoTbl();
	if (Regex::IsMatch(text, "Help "))
		pTextTbl = container->GetTextAllTable()->GetHelpTbl();
	if (Regex::IsMatch(text, "HTBSet"))
		pTextTbl = container->GetTextAllTable()->GetHTBSetTbl();
	if (Regex::IsMatch(text, "NormalItem"))
		pTextTbl = container->GetTextAllTable()->GetItemTbl();
	if (Regex::IsMatch(text, "MapName"))
		pTextTbl = container->GetTextAllTable()->GetMapNameTbl();
	if (Regex::IsMatch(text, "Merchant"))
		pTextTbl = container->GetTextAllTable()->GetMerchantTbl();
	if (Regex::IsMatch(text, "MilePost"))
		pTextTbl = container->GetTextAllTable()->GetMilePostTable();
	if (Regex::IsMatch(text, "Mob"))
		pTextTbl = container->GetTextAllTable()->GetMobTbl();
	if (Regex::IsMatch(text, "NpcDialog"))
		pTextTbl = container->GetTextAllTable()->GetNpcDialogTbl();
	if (Regex::IsMatch(text, "NPC "))
		pTextTbl = container->GetTextAllTable()->GetNPCTbl();
	if (Regex::IsMatch(text, "Object"))
		pTextTbl = container->GetTextAllTable()->GetObjectTbl();
	if (Regex::IsMatch(text, "QuestItem"))
		pTextTbl = container->GetTextAllTable()->GetQuestItemTbl();
	if (Regex::IsMatch(text, "Skill"))
		pTextTbl = container->GetTextAllTable()->GetSkillTbl();
	if (Regex::IsMatch(text, "SystemEffect"))
		pTextTbl = container->GetTextAllTable()->GetSystemEffectTbl();
	if (Regex::IsMatch(text, "TMQ"))
		pTextTbl = container->GetTextAllTable()->GetTMQTbl();
	if (Regex::IsMatch(text, "UseItem"))
		pTextTbl = container->GetTextAllTable()->GetUseItemTbl();
	if (Regex::IsMatch(text, "DBOTip"))
		pTextTbl = container->GetTextAllTable()->GetDBOTipTbl();

	sTEXT_TBLDAT data = *(sTEXT_TBLDAT*)pTextTbl->FindData(value);
	this->strTextAllTbl->Clear();
	this->strTextAllTbl->AppendText(gcnew String(data.wstrText.c_str()));
}
};
}
