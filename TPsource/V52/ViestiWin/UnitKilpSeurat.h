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

#ifndef UnitKilpSeuratH
#define UnitKilpSeuratH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include "VDef.h"
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
typedef struct {
	char key[LSEURA+2];
	wchar_t seura[LSEURA+1];
	int lkm;
	int pvlkm;
	int srjlkm[MAXSARJALUKU+MAXYHD];
} sralkmtp;

class TFormKilpSeurat : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *SG1;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Kirjoitaseurat;
	TMenuItem *Sisosotluku;
	TSaveDialog *SaveDialog1;
	TMenuItem *Kirjoitakaikkitaulukontiedot1;
	TLabel *Label1;
	TMenuItem *N1;
	TMenuItem *Suljekaavake1;
	void __fastcall KirjoitaseuratClick(TObject *Sender);
	void __fastcall SisosotlukuClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Kirjoitakaikkitaulukontiedot1Click(TObject *Sender);
	void __fastcall SG1MouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall SG1MouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall Suljekaavake1Click(TObject *Sender);
private:	// User declarations
	void __fastcall Nayta(void);
	void __fastcall NaytaSarjat(void);
	void __fastcall KirjoitaSeurat(int tyyli);
	sralkmtp *sra;
	int nsra;
	int maxnseura;
public:		// User declarations
	__fastcall TFormKilpSeurat(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormKilpSeurat *FormKilpSeurat;
//---------------------------------------------------------------------------
#endif
