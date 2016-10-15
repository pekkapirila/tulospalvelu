// Pekka Pirila's sports timekeeping program (Finnish: tulospalveluohjelma)
// Copyright (C) 2015 Pekka Pirila 

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//---------------------------------------------------------------------------

#ifndef UnitHtmlEditH
#define UnitHtmlEditH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
//#include "SHDocVw_OCX.h"
#include <Vcl.OleCtrls.hpp>
#include <SHDocVw.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormHtmlEdit : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabText;
	TTabSheet *TabHtml;
	TMemo *MemoText;
	TMainMenu *MainMenu1;
	TMenuItem *Tiedosto1;
	TMenuItem *Tallenna1;
	TMenuItem *allennanimell1;
	TMenuItem *Sulje1;
	TRadioGroup *RGLuonne;
	TMenuItem *Merkist1;
	TMenuItem *UTF81;
	TMenuItem *ISO885911;
	TWebBrowser *WebBrowser1;
	TOpenDialog *OpenDialog1;
	TMenuItem *Luetiedosto1;
	TSaveDialog *SaveDialog1;
	TButton *Button1;
	TButton *Button2;
	void __fastcall TabHtmlShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Luetiedosto1Click(TObject *Sender);
	void __fastcall allennanimell1Click(TObject *Sender);
	void __fastcall Tallenna1Click(TObject *Sender);
	void __fastcall UTF81Click(TObject *Sender);
	void __fastcall ISO885911Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormHtmlEdit(TComponent* Owner);
	void __fastcall WriteHtmlFile(wchar_t *flnm, bool full, bool ansi);
	void __fastcall ReadHtmlFile(wchar_t *flnm);
	bool __fastcall TallennaNimella(void);
	UnicodeString FileName;
	UnicodeString CurrentDir;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormHtmlEdit *FormHtmlEdit;
//---------------------------------------------------------------------------
#endif
