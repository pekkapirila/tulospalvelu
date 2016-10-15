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

#ifndef UnitSakkoKierrH
#define UnitSakkoKierrH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include "VDeclare.h"

typedef int intA_OSLUKU[MAXOSUUSLUKU];
//---------------------------------------------------------------------------
class TFormSakkoKierr : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TEdit *EdtKirjaus;
	TButton *BtnPeruuta;
	TLabel *Label3;
	TStringGrid *SGAvoimet;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Poistakaikkimerkinnt1;
	TMenuItem *Vaihdatiedosto1;
	TMenuItem *Suljekaavake1;
	TListBox *LBKirjatut;
	TEdit *EdtOsuus;
	TLabel *Label4;
	void __fastcall Poistakaikkimerkinnt1Click(TObject *Sender);
	void __fastcall Vaihdatiedosto1Click(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall EdtOsuusKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall BtnPeruutaClick(TObject *Sender);
	void __fastcall EdtKirjausKeyPress(TObject *Sender, System::WideChar &Key);
private:	// User declarations
	MESSAGE void __fastcall HaeAvoimetHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYHAEAVOIMET, TMyMessage, HaeAvoimetHandler);
END_MESSAGE_MAP(TComponent)
	void LaskeKierrokset(void);
	void LaskeAvoimet(void);
	void PaivitaAvoimet(void);
	TextFl *KierrFile;
	wchar_t KierrFlName[200];
	intA_OSLUKU *Kierretyt;
	intA_OSLUKU *EdAvoimet;
	intA_OSLUKU *Avoimet;
public:		// User declarations
	__fastcall TFormSakkoKierr(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSakkoKierr *FormSakkoKierr;
//---------------------------------------------------------------------------
#endif
