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

#include <vcl.h>
#include <Clipbrd.hpp>
#pragma hdrstop

#include "UnitJoukkuetiedot.h"
#include "UnitMain.h"
#include "UnitHenkHaku.h"
#include "UnitEmithaku.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormJoukkuetiedot *FormJoukkuetiedot;

ColTp OsGridCols0[] = {
	{L"Osuus", 50, 0, 1, 0, true},
	{L"Etunimi", 90, 0, 1, 0, true},
	{L"Sukunimi", 120, 0, 1, 0, true},
	{L"Seura", 80, 0, 1, 0, false},
	{L"Lisenssi", 65, 0, 1, 0, false},
	{L"Badge", 65, 0, 1, 0, true},
	{L"L", 15, 0, 1, 0, true},
	{L"Tark", 45, 0, 1, 0, true},
	{L"Lähtö", 75, 0, 1, 0, true},
	{L"Lht-lj", 45, 0, 1, 0, true},
	{L"Maali", 75, 0, 1, 0, true},
	{L"Tulos", 75, 0, 1, 0, true},
	{L"Sija", 35, 0, 1, 0, true},
	{L"Amp.sakot", 60, 0, 1, 0, false},
	{L"Sakko", 45, 0, 1, 0, false},
	{L"Rata", 60, 0, 1, 0, true},
	{L"Os.tls.", 75, 0, 1, 0, false},
	{L"Os.sj", 35, 0, 1, 0, false}
};

static wchar_t *LLaji[4] = {L"Auto", L"Auto", L"Yksil", L"Kiint"};

//---------------------------------------------------------------------------
__fastcall TFormJoukkuetiedot::TFormJoukkuetiedot(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	Sarja = -1;
	laika = kuuluttaja ? kilpparam.laika : kilpparam.laika2;

}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdtKilpnoChanged(TObject *Sender)
{
	int KilpNo, d;

	KilpNo = _wtoi(EdtKilpno->Text.c_str());
	if (KilpNo == Kilp.kilpno)
		return;
	if (!sallimuokkaus) {
	   if ((d = getpos(KilpNo)) > 0) {
		  dKilp = d;
		  Kilp.getrec(d);
		  }
	   else {
			dKilp = 0;
			Kilp.nollaa();
			}
	   Kilp1 = Kilp;
	   Sarja = Kilp.sarja;
	   nimihaku = false;
	   GridDims();
	   naytaTiedot();
	   salliNumeromuutos = false;
	   return;
	   }
	else {
		if (!salliNumeromuutos) {
			int vast;
			vast = select3(3, L"Yritys muuttaa joukkueen numeroa muokkaustilassa. Valitse toimintatapa",
				L"Valitse", L"Säilytä aiempi numero ja tallenna muut muutokset",
				L"Säilytä aiempi numero ja jatka muiden tietojen muokkaamista",
				L"Salli joukkueen numeron muuttaminen", this);
			switch (vast) {
				case 1:
					EdtKilpno->Text = Kilp.kilpno;
					BtnTallennaClick(Sender);
					return;
				case 2:
					EdtKilpno->Text = Kilp.kilpno;
					return;
				case 3:
					salliNumeromuutos = true;
					break;
				}
			}
		Kilp.kilpno = haeSeurVapaaKno(KilpNo);
		naytaTiedot();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormJoukkuetiedot::GridDims(void)
{
	int n;

	OsGrid->RowCount = Sarjat[Sarja].osuusluku + 1;
//	OsGrid->Height = (OsGrid->DefaultRowHeight+3) * (OsGrid->RowCount+1);
	OsGridCols[4].Visible = Lisenssinumero1->Checked;
	OsGridCols[5].Visible = Badge1->Checked;
	OsGridCols[13].Visible = Sarjat[Kilp.sarja].maxpaikat > 0;
	OsGridCols[14].Visible = Sarjat[Kilp.sarja].taslaji/10 > 0;
	n = 0;
	for (int i = 0; i < sizeof(OsGridCols0)/sizeof(ColTp); i++) {
		if (OsGridCols[i].Visible) {
			n++;
			}
		}
	OsGrid->ColCount = n;
	n = 0;
	for (int i = 0; i < sizeof(OsGridCols0)/sizeof(ColTp); i++) {
		if (OsGridCols[i].Visible) {
			OsGrid->Cells[n][0] = UnicodeString(OsGridCols[i].Caption);
			OsGrid->ColWidths[n] = OsGridCols[i].Width * Screen->PixelsPerInch / 96;
			OsGridCols[i].Order = n;
			n++;
			}
		}
	TlsGrid->ColCount = Sarjat[Sarja].osuusluku + 1;
	TlsGrid->RowCount = Sarjat[Kilp.sarja].Vaikaluku() + 2;
	for (int col = 1; col < TlsGrid->ColCount; col++) {
		TlsGrid->Cells[col][0] = UnicodeString("Osuus ") + wosuuskoodi(Sarja, col-1, 0, 0);
		}
	for (int row = 1; row < TlsGrid->RowCount-1; row++) {
		TlsGrid->Cells[0][row] = UnicodeString(row) + L". va.";
		}
	TlsGrid->Cells[0][TlsGrid->RowCount - 1] = L"Vaihto";
	FormResize(this);
}

void __fastcall TFormJoukkuetiedot::tyhjennaKaavake(void)
{
	int n;

	   salliNumeromuutos = false;
		EdtJoukkId->Text = L"";
		EdtMaa->Text = L"";
		EdtSeura->Text = L"";
		EdtJoukkue->Text = L"";
		EdtPiiri->Text = L"";
		EdPisteet0->Text = L"";
		EdPisteet1->Text = L"";
		EdPisteet2->Text = L"";
		for (int r = 1; r < OsGrid->RowCount; r++) {
			for (int c = 1; c < OsGrid->ColCount; c++) {
				OsGrid->Cells[c][r] = L"";
				}
			}
		for (int r = 1; r < TlsGrid->RowCount; r++) {
			for (int c = 1; c < TlsGrid->ColCount; c++) {
				TlsGrid->Cells[c][r] = L"";
				}
			}
}
//---------------------------------------------------------------------------
void __fastcall TFormJoukkuetiedot::naytaKilpailija(int d)
{
	if (ToimintaTila == 1) {
		RGSelaus->ItemIndex = 2;
		RGSelaus->Enabled = false;
		}
   if (d > 0) {
	  dKilp = d;
	  Kilp.getrec(d);
	  Kilp1 = Kilp;
	  Sarja = Kilp.sarja;
	  nimihaku = false;
//	  GridDims();
	  naytaTiedot();
	  }
}//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::naytaTiedot(void)
	{
	wchar_t ch, cs, ch2, ct, line[80], st[100], trk;
	int i, n, d, ep, rvali, sj;
	int tl, tlsum, toprow, leftcol, tlstoprow, tlsleftcol;

	toprow = OsGrid->TopRow;
	leftcol = OsGrid->LeftCol;
	tlstoprow = TlsGrid->TopRow;
	tlsleftcol = TlsGrid->LeftCol;
	if (SrjVal->Items->Count != sarjaluku) {
		SrjVal->Clear();
		for (int srj = 0; srj < sarjaluku; srj++)
			SrjVal->Items->Add(Sarjat[srj].sarjanimi);
		SrjVal->ItemIndex = 0;
		}
	BtnSalli->Visible = sallimuokkausvalinta;
	BtnPeruuta->Visible = sallimuokkaus;
	BtnTallenna->Visible = sallimuokkaus;
	BtnJjVaihto->Visible = sallimuokkaus;
	BtnPaivita->Visible = !sallimuokkaus;
	memset(line, 0, sizeof(line));
//	if (Sarja != Kilp.sarja)
		{
		Sarja = Kilp.sarja;
//		FormResize(this);
		}
	if (Lisays || Kilp.kilpno || (ToimintaTila == 1 && dKilp)) {
		if (SrjVal->ItemIndex != Sarja) {
			for (int i = 0; i < SrjVal->Items->Count; i++) {
				if (SrjVal->Items->Strings[i] == UnicodeString(Sarjat[Sarja].sarjanimi)) {
					SrjVal->ItemIndex = i;
					break;
					}
				}
			}
//		if (OsGrid->RowCount != Sarjat[Sarja].osuusluku + 1 ||
//			TlsGrid->ColCount != Sarjat[Sarja].osuusluku + 1 ||
//			TlsGrid->RowCount != Sarjat[Sarja].Vaikaluku() + 2)
			GridDims();

		Kilp.Joukkue(Kohde, sizeof(Kohde)/2-1);
		FormMain->SetFormKohde(this, Kohde);
//		sijat(Sarja);
		vOs = Sarjat[Kilp.sarja].osuusluku-1;
		EdtKilpno->Text = UnicodeString(Kilp.kilpno);
		EdtJoukkId->Text = UnicodeString(Kilp.joukkid);
		EdtMaa->Text = Kilp.Maa(st);
		EdtSeura->Text = Kilp.Seura(st, kilpparam.lseura);
		EdtJoukkue->Text = UnicodeString(Kilp.joukkue);
		EdtPiiri->Text = UnicodeString(Kilp.piiri);
		EdPisteet0->Text = UnicodeString(Kilp.pisteet[0]);
		EdPisteet1->Text = UnicodeString(Kilp.pisteet[1]);
		EdPisteet2->Text = UnicodeString(Kilp.pisteet[2]);

		for (int os = 0; os <= vOs; os++) {
			int yos;
			yos = Sarjat[Kilp.sarja].yosuus[os];
			for (int i = 0; i < sizeof(OsGridCols0)/sizeof(ColTp); i++) {
				if (OsGridCols[i].Visible) {
					int col = OsGridCols[i].Order;
					ColIx[col] = i;
					switch (i) {
						case 0:
							OsGrid->Cells[OsGridCols[i].Order][os+1] =
								UnicodeString(L"Os. ") + wosuuskoodi(Kilp.sarja, os, 0, 0);
							break;
						case 1:
							OsGrid->Cells[OsGridCols[i].Order][os+1] =
								Kilp.EtuNimi(st, kilpparam.lnimi, os); ;
							break;
						case 2:
							OsGrid->Cells[OsGridCols[i].Order][os+1] =
								Kilp.SukuNimi(st, kilpparam.lnimi, os); ;
							break;
						case 3:
							MbsToWcs(st, Kilp.ostiet[os].osseura, 40);
							OsGrid->Cells[OsGridCols[i].Order][os+1] = st;
							break;
						case 4:
							OsGrid->Cells[OsGridCols[i].Order][os+1] =
								UnicodeString(Kilp.ostiet[os].lisno);
							break;
						case 5:
							OsGrid->Cells[OsGridCols[i].Order][os+1] =
								UnicodeString(Kilp.ostiet[os].badge[0]);
							break;
						case 6:
							OsGrid->Cells[OsGridCols[i].Order][os+1] =
								UnicodeString(Kilp.ostiet[os].laina[0]);
							break;
						case 7:
							if (kilpparam.partio && os != Sarjat[Kilp.sarja].osuusluku - 1) {
								line[0] = 0;
								}
							else {
								switch (Kilp.wTark(os, 0)) {
									case L'K' :
										wcscpy(line, L"Kesk");
										break;
									case L'H' :
										wcscpy(line, L"Hyl");
										break;
									case L'E' :
										wcscpy(line, L"Eiläht.");
										break;
									case L'T' :
										wcscpy(line, L"Tark.");
										break;
									case L'I' :
										wcscpy(line, L"esItys");
										break;
									case L'P' :
										wcscpy(line, L"Poissa");
										break;
									default :
										wcscpy(line, L"Avoin");
										break;
									}
								}
							OsGrid->Cells[OsGridCols[i].Order][os+1] =
								UnicodeString(line);
							break;
						case 8:
							tl = Kilp.ostiet[os].ylahto;
							if (tl != TMAALI0) {
								AIKATOWSTRS(line, tl, t0);
								line[laika] = 0;
								}
							else
								line[0] = 0;
								OsGrid->Cells[OsGridCols[i].Order][os+1] =
								UnicodeString(line);
							break;
						case 9:
							OsGrid->Cells[OsGridCols[i].Order][os+1] =
								LLaji[Kilp.ostiet[os].lahtolaji+1];
							break;
						case 10:
							line[0] = 0;
							tl = Kilp.Maali(os, 0);
							if (tl != TMAALI0) {
								AIKATOWSTRS(line, tl, t0);
								line[laika] = 0;
								}
							OsGrid->Cells[OsGridCols[i].Order][os+1] =
								UnicodeString(line);
							break;
						case 11:
							if (kilpparam.partio && os != Sarjat[Kilp.sarja].osuusluku - 1) {
								line[0] = 0;
								}
							else {
								tl = Kilp.tTulos(yos, 0);
								if (tl != 0) {
									AIKATOWSTRS(line, tl, 0);
									line[laika] = 0;
									}
								else
									wcscpy(line , L"00.00.00");
								}
							OsGrid->Cells[OsGridCols[i].Order][os+1] =
								UnicodeString(line);
							break;
						case 12:
							if ((!kilpparam.partio || os == Sarjat[Kilp.sarja].osuusluku - 1) &&
								Kilp.Tulos(os, 0))
								OsGrid->Cells[OsGridCols[i].Order][os+1] =
									UnicodeString(Kilp.Sija(os, 0));
							else
								OsGrid->Cells[OsGridCols[i].Order][os+1] = L"";
							break;
						case 13:
							OsGrid->Cells[OsGridCols[i].Order][os+1] = Kilp.Ampsakot(os);
							break;
						case 14:
							tl = Kilp.ostiet[os].sakko;
							if (Sarjat[Kilp.sarja].tsak[os] != 0)
								tl /= Sarjat[Kilp.sarja].tsak[os]*SEK;
							OsGrid->Cells[OsGridCols[i].Order][os+1] = tl;
							break;
						case 15:
							OsGrid->Cells[OsGridCols[i].Order][os+1] =
								UnicodeString(Kilp.ostiet[os].hajonta);
							break;
						case 16:
							tl = Kilp.osTulos(os, 0);
							if (tl != 0) {
								AIKATOWSTRS(line, tl, 0);
								line[laika] = 0;
								}
							else
								wcscpy(line , L"00.00.00");
							OsGrid->Cells[OsGridCols[i].Order][os+1] =
								UnicodeString(line);
							break;
						case 17:
							if (Kilp.osTulos(os, 0))
								OsGrid->Cells[OsGridCols[i].Order][os+1] =
									UnicodeString(Kilp.Sija(os, 0));
							else
								OsGrid->Cells[OsGridCols[i].Order][os+1] =
									L"";
							break;
						}
					}
				}
			}
		for (int col = 1; col < TlsGrid->ColCount; col++) {
			int os = col-1;
			int yos;
			yos = Sarjat[Kilp.sarja].yosuus[os];
			for (int row = 1; row < TlsGrid->RowCount; row++) {
				wchar_t ln[40] = L"";
				int va = 0;
				if (row < TlsGrid->RowCount - 1)
					va = row;
				if (os < Sarjat[Kilp.sarja].osuusluku && va < Sarjat[Kilp.sarja].valuku[Sarjat[Kilp.sarja].yosuus[os]] + 1) {
					switch (RGVaTieto->ItemIndex) {
						case 0:
							aikatowstr_cols_n(ln, Kilp.Maali(os, va), t0, 0, laika);
							break;
						case 1:
							aikatowstr_cols_n(ln, Kilp.tTulos(yos, va), 0, 0, laika);
							break;
						case 2:
							aikatowstr_cols_n(ln, Kilp.osTulos(os, va), 0, 0, laika);
							break;
						case 3:
							if (Kilp.tTulos(yos, va) > 5*SEK)
								_itow(Kilp.Sija(os, va), ln, 10);
							break;
						case 4:
							if (va)
								MbsToWcs(ln, Sarjat[Kilp.sarja].va_matka[yos][va-1], 20);
							else
								MbsToWcs(ln, Sarjat[Kilp.sarja].matka[yos], 20);
							break;
						}
					}
				TlsGrid->Cells[col][row] = ln;
				}
			}
		if (toprow >= OsGrid->RowCount)
			toprow = OsGrid->RowCount - 1;
		if (leftcol >= OsGrid->ColCount)
			leftcol = OsGrid->ColCount - 1;
		if (tlstoprow >= TlsGrid->RowCount)
			tlstoprow = TlsGrid->RowCount - 1;
		if (tlsleftcol >= TlsGrid->ColCount)
			tlsleftcol = TlsGrid->ColCount - 1;
		OsGrid->TopRow = toprow;
		OsGrid->LeftCol = leftcol;
		TlsGrid->TopRow = tlstoprow;
		TlsGrid->LeftCol = tlsleftcol;
		}
	else
		tyhjennaKaavake();
	}

//---------------------------------------------------------------------------

int __fastcall TFormJoukkuetiedot::tallennaTiedot(void)
   {
   kilptietue Kilp2;

   if (Lisays) {
//	   Kilp.setId(EdtKilpno->Text.ToInt());
//	   haeTiedot(&Kilp);
	   if ((ToimintaTila == 2 && !Kilp.kilpno) || getpos(Kilp.kilpno) > 0) {
			Application->MessageBox(L"Kilpailijanumero ei ole käytettävissä, ei tallenneta", L"Virhe",
				MB_OK);
			return(1);
			}
	   addtall(&Kilp, &dKilp, 0);
	   Kilp.getrec(dKilp);
	   Kilp1 = Kilp;
	   if (oistuvienlisystentila1->Checked) {
			OsGrid->Col = 1;
			OsGrid->Row = 1;
			Liskilpailija1Click(this);
			FocusControl(EdtKilpno);
			return(1);
			}
	   return(0);
	   }
   if (Kilp1 == Kilp) {
	  Application->MessageBox(L"Kilpailijan tiedot eivät ole muuttuneet", L"Ei tallenneta",
		  MB_OK);
	  return(1);
	  }

	if (Kilp1.kilpno != Kilp.kilpno && getpos(Kilp.kilpno) > 0) {
	  Application->MessageBox(L"Kilpailijanumero ei ole käytettävissä, ei tallenneta", L"Virhe",
		  MB_OK);
	  return(1);
	  }

	if (ToimintaTila == 2 && Kilp.kilpno == 0) {
	  Application->MessageBox(L"Joukkueelle on annettava numero", L"Virhe",
		  MB_OK);
	  return(1);
	  }

   EnterCriticalSection(&tall_CriticalSection);
   Kilp2.getrec(dKilp);
   if (!(Kilp2 == Kilp1)) {
	  LeaveCriticalSection(&tall_CriticalSection);
	  Application->MessageBox(L"Kilpailijan tietoja muutettu toisaalla, tietoja ei tallenneta", L"Este",
		  MB_OK);
	  return(1);
	  }

   Kilp.Tallenna(dKilp, 0, 0, 0, &Kilp1);
   LeaveCriticalSection(&tall_CriticalSection);
   Kilp.getrec(dKilp);
   Kilp1 = Kilp;
   if (!oistuvienlisystentila1->Checked)
	   salliNumeromuutos = false;
   return(0);
   }
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdtSeuraExit(TObject *Sender)
{
	wchar_t st[100];

	if (sallimuokkaus) {
		memset(Kilp.seura, 0, sizeof(Kilp.seura));
		Kilp.setSeura(EdtSeura->Text.c_str());
		}
	EdtSeura->Text = Kilp.Seura(st, 80);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdtJoukkNimiExit(TObject *Sender)
{
	wchar_t st[100];

	if (sallimuokkaus) {
		memset(Kilp.joukknimi, 0, sizeof(Kilp.joukknimi));
		WcsToMbs(Kilp.joukknimi, EdtJoukkNimi->Text.c_str(), sizeof(Kilp.joukknimi)-1);
		}
	MbsToWcs(st, Kilp.joukknimi, 60);
	EdtJoukkNimi->Text = st;
}
//---------------------------------------------------------------------------


void __fastcall TFormJoukkuetiedot::EdtJoukkueExit(TObject *Sender)
{
	if (sallimuokkaus) {
		Kilp.joukkue = _wtoi(EdtJoukkue->Text.c_str());
		}
	EdtJoukkue->Text = Kilp.joukkue;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdtMaaExit(TObject *Sender)
{
	wchar_t st[10];

	if (sallimuokkaus) {
		memset(Kilp.maa, 0, sizeof(Kilp.maa));
		Kilp.setMaa(EdtMaa->Text.c_str());
		}
	EdtMaa->Text = Kilp.Maa(st);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::SrjValExit(TObject *Sender)
{
	if (sallimuokkaus) {
		Kilp.sarja = SrjVal->ItemIndex;
		GridDims();
		naytaTiedot();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdtJoukkIdExit(TObject *Sender)
{
	if (sallimuokkaus)
		Kilp.joukkid = _wtoi(EdtJoukkId->Text.c_str());
	EdtJoukkId->Text = Kilp.joukkid;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdPisteet0Exit(TObject *Sender)
{
	if (sallimuokkaus)
		Kilp.pisteet[0] = _wtoi(EdPisteet0->Text.c_str());
	EdPisteet0->Text = Kilp.pisteet[0];
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdPisteet1Exit(TObject *Sender)
{
	if (sallimuokkaus)
		Kilp.pisteet[1] = _wtoi(EdPisteet1->Text.c_str());
	EdPisteet1->Text = Kilp.pisteet[1];
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdPisteet2Exit(TObject *Sender)
{
	if (sallimuokkaus)
		Kilp.pisteet[2] = _wtoi(EdPisteet2->Text.c_str());
	EdPisteet2->Text = Kilp.pisteet[2];
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdtPiiriExit(TObject *Sender)
{
	if (sallimuokkaus)
		Kilp.piiri = _wtoi(EdtPiiri->Text.c_str());
	EdtPiiri->Text = Kilp.piiri;
}
//---------------------------------------------------------------------------

int __fastcall TFormJoukkuetiedot::paivitaMuutos(int col, int row)
{
	int i, k, srj, ipv, upv;
	int tl, tle;
	wchar_t line[120], kh;

	k = row;
	ipv = row - 1;
	switch (ColIx[col]) {
		case 1:
			Kilp.setNimi(OsGrid->Cells[col][k].c_str(), OsGrid->Cells[col+1][k].c_str(), ipv);
//			OsGrid->Cells[col][k] = Kilp.EtuNimi(line, 80, ipv);
			break;
		case 2:
			Kilp.setNimi(OsGrid->Cells[col-1][k].c_str(), OsGrid->Cells[col][k].c_str(), ipv);
//			OsGrid->Cells[col][k] = Kilp.SukuNimi(line, 80, ipv);
			break;
		case 3:
			WcsToMbs(Kilp.ostiet[ipv].osseura, OsGrid->Cells[col][k].c_str(), sizeof(Kilp.ostiet[ipv].osseura)-1);
			MbsToWcs(line, Kilp.ostiet[ipv].osseura, 80);
//			OsGrid->Cells[col][k] = st;
			break;
		case 4:
			Kilp.ostiet[ipv].lisno = _wtoi(OsGrid->Cells[col][k].c_str());
//			OsGrid->Cells[col][k] = Kilp.ostiet[ipv].lisno;
			break;
		case 5:
			Kilp.ostiet[ipv].badge[0] = _wtoi(OsGrid->Cells[col][k].c_str());
//			OsGrid->Cells[col][k] = Kilp.ostiet[ipv].badge[0];
			break;
		case 6:
			Kilp.ostiet[ipv].laina[0] = wchartoansi(OsGrid->Cells[col][k].c_str()[0]);
//			OsGrid->Cells[col][k] = ansitowchar(Kilp.pv[ipv].laina[0]);
			break;
		case 7:
			if (OsGrid->Cells[col][k].Length() == 0)
				return(0);
			kh = towupper(OsGrid->Cells[col][k].c_str()[0]);
			switch (kh) {
				case L'E' :
					if (OsGrid->Cells[col][k].Length() == 1 || towupper(OsGrid->Cells[col][k].c_str()[2]) != L'S')
						kh = L'E';
					else
						kh = L'I';
					break;
				case L'A' :
				case L'L' :
					kh = L'-';
					break;
				}
			Kilp.SetTark(ipv, kh);
			break;
		case 8:
			wcsncpy(line,OsGrid->Cells[col][k].c_str(), laika);
			line[laika] = 0;
			elimwbl(line);
			if (line[0] == 0 || line[0] == L':') {
				tl = TMAALI0;
				}
			else {
				int np = 0;
				for (wchar_t *p = line; *p; p++) {
					if (*p == L'.' || *p == L':' || *p == L'/') {
						*p = L':';
						np++;
						}
					if (*p == L',') {
						np++;
						}
					}
				if (np == 1)
					wcscat(line, L":00");
				tl = wstrtoaika_vap(line, t0);
				}
//			OsGrid->Cells[col][k] = UnicodeString(AIKATOWSTRS(line, tl, t0)).SubString(1, laika);
			Kilp.ostiet[ipv].ylahto = tl;
			if (tl != TMAALI0) {
				if (Kilp.ostiet[ipv].lahtolaji == 0)
					Kilp.ostiet[ipv].lahtolaji = 1;
				}
			break;
		case 9:
			Kilp.ostiet[ipv].lahtolaji = wcswcind(towupper(OsGrid->Cells[col][k].c_str()[0]), L"AYK");
			if (Kilp.ostiet[ipv].lahtolaji > 2)
				Kilp.ostiet[ipv].lahtolaji = 0;
			break;
		case 10:
			Kilp.setMaali(ipv, 0, wstrtoaika_vap(OsGrid->Cells[col][k].c_str(), t0));
			break;
		case 13:
			if (Sarjat[Kilp.sarja].paikat[ipv] > 0) {
				wcsncpy(line, OsGrid->Cells[col][k].c_str(), Sarjat[Kilp.sarja].paikat[ipv]+1);
				line[Sarjat[Kilp.sarja].paikat[ipv]] = 0;
				for (int is = 0; is < Sarjat[Kilp.sarja].paikat[ipv]; is++) {
					if (line[is] < L'0' || line[is] > L'9')
						Kilp.setAmpsakot(ipv, is, -1);
					else
						Kilp.setAmpsakot(ipv, is, line[is]-L'0');
					}
				}
			break;
		case 14:
			tl = _wtoi(OsGrid->Cells[col][k].c_str());
			if (Sarjat[Kilp.sarja].tsak[ipv])
				tl *= Sarjat[Kilp.sarja].tsak[ipv] * SEK;
			Kilp.ostiet[ipv].sakko = tl;
			break;
		case 15:
			memset(Kilp.ostiet[ipv].hajonta, 0, sizeof(Kilp.ostiet[ipv].hajonta));
			Kilp.setHajonta(ipv, OsGrid->Cells[col][k].UpperCase().c_str());
			break;
		}
	naytaTiedot();
	return(0);
}
//---------------------------------------------------------------------------

int __fastcall TFormJoukkuetiedot::paivitaVaMuutos(int col, int row)
{
	int lj, os, va;
	int tl, tle;

	if (col < 1 || row < 1)
		return(1);
	switch (RGVaTieto->ItemIndex) {
		case 0:
			lj = 0;
			break;
		case 1:
			lj = 1;
			break;
		default:
			return(1);
		}
	os = col - 1;
	if (row == TlsGrid->RowCount-1)
		va = 0;
	else
		va = row;
	if (lj == 0) {
		Kilp.setMaali(os, va, wstrtoaika_vap(TlsGrid->Cells[col][row].c_str(), t0));
		}
	else {
		tl = wstrtoaika_vap(TlsGrid->Cells[col][row].c_str(), 0);
		if (tl == 0)
			Kilp.setMaali(os, va, TMAALI0);
		else {
			if ((tle = Kilp.Tulos(os, va)) > 0) {
				Kilp.setMaali(os, va, Kilp.Maali(os, va) + tl - tle);
				}
			else {
				tle = 0;
				if (osuuslahto(&Kilp, os) != TMAALI0 && (os == 0 || (tle = Kilp.Tulos(os-1, 0)) > 0)) {
					Kilp.setMaali(os, va, osuuslahto(&Kilp, os) + tl - tle);
					}
				}
			}
		}
	naytaTiedot();
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::SrjKarkiBtn(TObject *Sender)
{
	int p = -1, lj = 0;

	SelOsuus = CBSelOsuus->ItemIndex;
	salliNumeromuutos = false;
	if (nilm[Sarja] > 0) {
		if (ToimintaTila == 2) {
			EnterCriticalSection(&tall_CriticalSection);

			for (lj = 0; lj < 5; lj++)
				if ((p = jalku[Sarja][SelOsuus][lj]) != 0)
					break;
			if (p > 0) {
				dKilp = p;
				Kilp.getrec(p);
				Kilp1 = Kilp;
				Sarja = Kilp.sarja;
				nimihaku = false;
				GridDims();
				naytaTiedot();
				}
			LeaveCriticalSection(&tall_CriticalSection);
			}
		else if (Sarja != Kilp.sarja) {
			kilptietue kilp;
			for (p = 1; p < datf2.numrec; p++) {
				kilp.getrec(p);
				if (kilp.kilpstatus == 0 && kilp.sarja == Sarja) {
					dKilp = p;
					Kilp.getrec(p);
					Kilp1 = Kilp;
					Sarja = Kilp.sarja;
					GridDims();
					naytaTiedot();
					}
				}
			}
		}
	OsGrid->Refresh();
	TlsGrid->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::SeurBtnClick(TObject *Sender)
{
   int sj;
	int p = -1;

   if (dKilp <= 0)
	  return;
   salliNumeromuutos = false;
	EnterCriticalSection(&tall_CriticalSection);
	switch (RGSelaus->ItemIndex) {
		case 0:
		   p = JarjSeur(0, SelOsuus, 0, dKilp);
		   nimihaku = false;
		   break;
		case 1:
			for (int k = Kilp.kilpno+1; k <= maxkilpno; k++) {
				if ((p = getpos(k)) > 0) {
					nimihaku = false;
					break;
					}
				}
			break;
		case 2:
			wchar_t st[61];
			char keyn[MAXINDL+1];

			if (!nimihaku) {
				 memmove(keyn, (char *) keyname(&Kilp),
					nameindex.keyl);
				searchix(&nameindex, keyn, &p);
				}
			nimihaku = true;
			if (nextix(&nameindex, keyn, &p)) {
				nextix(&nameindex, keyn, &p);
				previx(&nameindex, keyn, &p);
				}
			break;
		}
	if (p >= 0) {
		Kilp.getrec(p);
		Kilp1 = Kilp;
		dKilp = p;
		Sarja = Kilp.sarja;
		naytaTiedot();
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdBtnClick(TObject *Sender)
{
	int sj;
	int p = -1, p1;

	if (dKilp <= 0)
		return;
   salliNumeromuutos = false;
	EnterCriticalSection(&tall_CriticalSection);
	switch (RGSelaus->ItemIndex) {
		case 0:
			sj = Kilp.Sija(SelOsuus, 0);
			p = -1;
			if (sj > 1 && sj <= ntulos[Sarja][SelOsuus][0]) {
				p = jalku[Sarja][SelOsuus][0];
				while ((p1 = JarjSeur(0, SelOsuus, 0, p)) >= 0 && p1 != dKilp)
					p = p1;
				if (p1 != dKilp)
					p = -1;
				}
			nimihaku = false;
			break;
		case 1:
			for (int k = Kilp.kilpno-1; k >= minkilpno; k--) {
				if ((p = getpos(k)) > 0) {
					nimihaku = false;
					break;
					}
				}
			break;
		case 2:
			wchar_t st[61];
			char keyn[MAXINDL+1];

			if (!nimihaku) {
				 memmove(keyn, (char *) keyname(&Kilp),
					nameindex.keyl);
				searchix(&nameindex, keyn, &p);
				}
			nimihaku = true;
			if (previx(&nameindex, keyn, &p)) {
				previx(&nameindex, keyn, &p);
				nextix(&nameindex, keyn, &p);
				}
			break;
		}
	if (p >= 0) {
		Kilp.getrec(p);
		Kilp1 = Kilp;
		dKilp = p;
		Sarja = Kilp.sarja;
		naytaTiedot();
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdtKilpnoKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	if (Key == VK_RETURN)
		EdtKilpnoChanged(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::SrjValChange(TObject *Sender)
{
	int Os;

	if (!sallimuokkaus) {
		if (nilm[SrjVal->ItemIndex] > 0) {
			Sarja = SrjVal->ItemIndex;
			SrjKarkiBtn(Sender);
			}
		else {
			SrjVal->ItemIndex = Sarja;
			return;
			}
		}
	if (SrjVal->ItemIndex < 0)
		OsGrid->RowCount = kilpparam.osuusluku+1;
	else
		OsGrid->RowCount =
			Sarjat[SrjVal->ItemIndex].osuusluku + 1;
	CBSelOsuus->Clear();
	for (int os = 0; os < kilpparam.ntosuus; os++)
		CBSelOsuus->Items->Add(UnicodeString(os+1) + L" Os.");
	if (SelOsuus >= kilpparam.ntosuus)
		SelOsuus = kilpparam.ntosuus;
	CBSelOsuus->ItemIndex = SelOsuus;
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::FormDestroy(TObject *Sender)
{
   delete[] OsGridCols;
}
//---------------------------------------------------------------------------


void __fastcall TFormJoukkuetiedot::BtnSalliClick(TObject *Sender)
{
	if (oistuvienlisystentila1->Checked) {
		sallimuokkaus = true;
		salliNumeromuutos = true;
		}
	else {
		salliNumeromuutos = false;
		sallimuokkaus = !sallimuokkaus;
		}
	BtnPeruuta->Visible = sallimuokkaus;
	BtnTallenna->Visible = sallimuokkaus;
	BtnJjVaihto->Visible = sallimuokkaus;
	BtnPaivita->Visible = !sallimuokkaus;
	EdtSeura->ReadOnly = !sallimuokkaus;
	EdtJoukkue->ReadOnly = !sallimuokkaus;
	EdtMaa->ReadOnly = !sallimuokkaus;
	EdtPiiri->ReadOnly = !sallimuokkaus;
	EdtJoukkId->ReadOnly = !sallimuokkaus;
	EdtJoukkNimi->ReadOnly = !sallimuokkaus;
	EdtJoukkId->ReadOnly = !sallimuokkaus;
	EdPisteet0->ReadOnly = !sallimuokkaus;
	EdPisteet1->ReadOnly = !sallimuokkaus;
	EdPisteet2->ReadOnly = !sallimuokkaus;
	EdtNimihaku->Enabled = !sallimuokkaus;
	SrjKarki->Enabled = !sallimuokkaus;
	SeurBtn->Enabled = !sallimuokkaus;
	EdBtn->Enabled = !sallimuokkaus;
	if (sallimuokkaus) {
		OsGrid->Options << goEditing;
		if (RGVaTieto->ItemIndex < 2)
			TlsGrid->Options << goEditing;
		if (Lisays)
			Tila->Caption = UnicodeString(L"Uuden tietueen muokkaus");
		else
			Tila->Caption = UnicodeString(L"Muokkaustila");
		BtnSalli->Caption = L"Hakuun ja katseluun";
		}
	else {
		OsGrid->Options >> goEditing;
		TlsGrid->Options >> goEditing;
		Tila->Caption = UnicodeString(L"Haku- ja katselutila");
		BtnSalli->Caption = L"Salli muokkaus";
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::BtnPeruutaClick(TObject *Sender)
{
	if (!oistuvienlisystentila1->Checked)
		salliNumeromuutos = false;
	if (Lisays) {
		Lisays = false;
		BtnSalliClick(Sender);
		EdBtnClick(Sender);
		}
	Kilp = Kilp1;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::BtnTallennaClick(TObject *Sender)
{
   if (!oistuvienlisystentila1->Checked)
		salliNumeromuutos = false;
	if (tallennaTiedot() == 0) {
		BtnPeruutaClick(Sender);
		}
	BtnSalliClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::OsGridExit(TObject *Sender)
{
	if (sallimuokkaus && aktcol > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}

	aktrow = 0;
	aktcol = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdtNimihakuChange(TObject *Sender)
{
	salliNumeromuutos = false;
	if (EdtSeura->ReadOnly == true) {
		wchar_t txt[62];
		char key[MAXINDL+1] = "";
		int d;

		wcsncpy(txt, EdtNimihaku->Text.c_str(),60);
		txt[60] = 0;
		keyfromU16name(key, txt, NIMIINDL-3, 0);
		searchix(&nameindex, key, &d);
		nimihaku = true;
		dKilp = d;
		Kilp.getrec(d);
		Kilp1 = Kilp;
		Sarja = Kilp.sarja;
		RGSelaus->ItemIndex = 2;
		naytaTiedot();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (sallimuokkaus) {
		if (!(Kilp1 == Kilp) && Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallennus",
			MB_YESNO) == IDYES) {
			tallennaTiedot();
			}
		sallimuokkaus = false;
		}
	oistuvienlisystentila1->Checked = false;
	salliNumeromuutos = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::BtnSuljeClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------



void __fastcall TFormJoukkuetiedot::OsGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_F9 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (sallimuokkaus) {
			BtnTallennaClick(Sender);
			}
		else {
			BtnSalliClick(Sender);
			}
		}
	if (Key == VK_RIGHT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (OsGrid->Col < OsGrid->ColCount-1)
			OsGrid->Col++;
		}
	if (Key == VK_LEFT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (OsGrid->Col > 0)
			OsGrid->Col--;
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormJoukkuetiedot::FormResize(TObject *Sender)
{
	static bool init = true;
	int ho, ht, wo, wt;

	OsGrid->Width = ClientWidth - 12 * Screen->PixelsPerInch / 96;
	ho = (OsGrid->DefaultRowHeight+3) * (OsGrid->RowCount+1);
	ht = (TlsGrid->DefaultRowHeight+3) * (TlsGrid->RowCount+1);
	if (init) {
		if (ClientHeight > ho + ht + OsGrid->Top + 18 * Screen->PixelsPerInch / 96)
			ClientHeight = ho + ht + OsGrid->Top + 18 * Screen->PixelsPerInch / 96;
		}
	init = false;
	ht = (ClientHeight - OsGrid->Top - 18 * Screen->PixelsPerInch / 96) * ht / (ht + ho);
	if (ht < (3 + (Sarjat[Sarja].Vaikaluku() > 0)) * (TlsGrid->DefaultRowHeight+3))
		ht = (3 + (Sarjat[Sarja].Vaikaluku() > 0)) * (TlsGrid->DefaultRowHeight+3);

	OsGrid->Height = ClientHeight - OsGrid->Top - 18 * Screen->PixelsPerInch / 96 - ht;
	TlsGrid->Top = OsGrid->Top + OsGrid->Height + 6 * Screen->PixelsPerInch / 96;
	TlsGrid->Height = ht;
//	if (ClientHeight < TlsGrid->Top + 12 * Screen->PixelsPerInch / 96)
//		ClientHeight = TlsGrid->Top + 12 * Screen->PixelsPerInch / 96;
//	TlsGrid->Height = ClientHeight - TlsGrid->Top - 6 * Screen->PixelsPerInch / 96;
	TlsGrid->Width = OsGrid->Width;
	Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::OsGridSelectCell(TObject *Sender, int ACol,
		  int ARow, bool &CanSelect)
{
	if (aktrow == ARow && aktcol == ACol)
		return;
	if (sallimuokkaus && aktcol > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}
	aktrow = ARow;
	aktcol = ACol;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::FormShow(TObject *Sender)
{
	SrjVal->Clear();
	for (int srj = 0; srj < sarjaluku; srj++)
		SrjVal->Items->Add(Sarjat[srj].sarjanimi);
	SrjVal->ItemIndex = Sarja;
	CBSelOsuus->Clear();
	for (int os = 0; os < kilpparam.ntosuus; os++)
		CBSelOsuus->Items->Add(UnicodeString(os+1) + L" Os.");
	CBSelOsuus->ItemIndex = SelOsuus;
	OsGrid->DefaultRowHeight = 20 * Screen->PixelsPerInch / 96;
	OsGrid->RowCount = kilpparam.osuusluku + 1;
	TlsGrid->DefaultRowHeight = 20 * Screen->PixelsPerInch / 96;
	TlsGrid->ColCount = kilpparam.osuusluku + 1;
	TlsGrid->RowCount = kilpparam.maxvaluku + 2;
	TlsGrid->DefaultColWidth = 75 * Screen->PixelsPerInch / 96;
	TlsGrid->ColWidths[0] = 45 * Screen->PixelsPerInch / 96;
	FormResize(Sender);
	salliNumeromuutos = false;
	if (dKilp)
		naytaKilpailija(dKilp);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::FormCreate(TObject *Sender)
{
	Sarja = 0;
	dKilp = -1;
	nimihaku = false;
	sallimuokkaus = false;
	sallimuokkausvalinta = false;
	Kilp.nollaa();
	OsGridCols = new ColTp[sizeof(OsGridCols0)/sizeof(ColTp)];
	memcpy(OsGridCols, OsGridCols0, sizeof(OsGridCols0));
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ilmoittautumiset");
}
//---------------------------------------------------------------------------


void __fastcall TFormJoukkuetiedot::EdtKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift)
{
	if (Key == VK_F9 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (sallimuokkaus) {
			BtnTallennaClick(Sender);
			}
		else {
			BtnSalliClick(Sender);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdtKilpnoKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		EdtKilpnoChanged(Sender);
		Key = 0;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::SiirraSeuraTiedot(int haku)
{
}
//---------------------------------------------------------------------------


void __fastcall TFormJoukkuetiedot::EdtSeuraChange(TObject *Sender)
{
	if (sallimuokkaus) {
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdtSeuraEnter(TObject *Sender)
{
	if (sallimuokkaus) {
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::EdtJoukkNimiEnter(TObject *Sender)
{
	if (sallimuokkaus) {
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::Emitkoodihaku1Click(TObject *Sender)
{
	int kno, bdg = 0, toinen = -1;
	wchar_t wch;

	salliNumeromuutos = false;
	FormEmithaku = new TFormEmithaku(this);
	FormEmithaku->ShowModal();
	if (FormEmithaku->ModalResult == mrOk) {
		kno = FormEmithaku->Kilpno;
		if (kno > 0) {
			naytaKilpailija(getpos(kno));
			}
		}
	delete FormEmithaku;
}
//---------------------------------------------------------------------------


void __fastcall TFormJoukkuetiedot::Perusnumerohaku1Click(TObject *Sender)
{
	int kno = 0;
	wchar_t wch;

	salliNumeromuutos = false;
	inputint_prompt(&kno, L"Anna haettava numero", &wch, this);
	if (wch == L'\r') {
		int d;
		kilptietue kilp;

		for (d = 1; d < datf2.numrec; d++) {
			kilp.getrec(d);
			if (kilp.kilpstatus == 0 && kilp.kilpno == kno)
				break;
			}
		if (d < datf2.numrec) {
			naytaKilpailija(d);
			}
		else {
			Application->MessageBoxW(L"Numero ei ole käytössä", L"Ei löydy", MB_OK);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::Liskilpailija1Click(TObject *Sender)
{
	dKilp = 0;
	Kilp.nollaa();
	Lisays = true;
	sallimuokkaus = false;
	salliNumeromuutos = true;
	BtnSalliClick(Sender);
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::Poistakilpailija1Click(TObject *Sender)
{
	if (Application->MessageBoxW(L"Poista näytetty kilpailija", L"Vahvista poisto", MB_YESNO) == IDYES)	{
		deltall(dKilp, 0);
		EdBtnClick(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::OsGridClick(TObject *Sender)
{
	bool sel;

	OsGridSelectCell(Sender, OsGrid->Col, OsGrid->Row, sel);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::OsGridKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (sallimuokkaus && Key == L'\r' && OsGrid->Col > 0 && OsGrid->Row > 0) {
		Key = 0;
		paivitaMuutos(OsGrid->Col, OsGrid->Row);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::RGVaTietoClick(TObject *Sender)
{
	if (sallimuokkaus && RGVaTieto->ItemIndex < 2)
		TlsGrid->Options << goEditing;
	else
		TlsGrid->Options >> goEditing;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::BtnJjVaihtoClick(TObject *Sender)
{
	char st[MAXOSUUSLUKU][OSNIMIL+1];
	wchar_t wst[OSNIMIL+1] = L"", *p;
	FormJjVaihdot = new TFormJjVaihdot(this);
	int lisno[MAXOSUUSLUKU];
	int ekdi[MAXOSUUSLUKU];
	char lna[MAXOSUUSLUKU];
	static int jjFormHeight;
	char haj[MAXOSUUSLUKU][HAJONTA+1];

	if (jjFormHeight == 0)
		jjFormHeight = FormJjVaihdot->Height;
	else
		FormJjVaihdot->Height = jjFormHeight;
	FormJjVaihdot->SG1->RowCount = Sarjat[Kilp.sarja].osuusluku + 1;
	memset(st, 0, sizeof(st));
	for (int os = 0; os < Sarjat[Kilp.sarja].osuusluku; os++) {
		FormJjVaihdot->SG1->Cells[0][os+1] = Kilp.Nimi(wst, OSNIMIL, os);
		FormJjVaihdot->SG1->Cells[1][os+1] = wosuuskoodi(Kilp.sarja, os, 0, 0);
		strncpy(st[os], Kilp.ostiet[os].nimi, OSNIMIL);
		lisno[os] = Kilp.ostiet[os].lisno;
		ekdi[os] = Kilp.ostiet[os].badge[0];
		lna[os] = Kilp.ostiet[os].laina[0];
		strncpy(haj[os], Kilp.ostiet[os].hajonta, HAJONTA);
		}
	FormJjVaihdot->ShowModal();
	switch (FormJjVaihdot->ModalResult) {
		case 101:
			for (int os = 0; os < Sarjat[Kilp.sarja].osuusluku; os++) {
				int u = tulkOsuuskoodi(Kilp.sarja, FormJjVaihdot->SG1->Cells[1][os+1].c_str());
				if (u >= 0 && u < Sarjat[Kilp.sarja].osuusluku) {
					memcpy(Kilp.ostiet[u].nimi, st[os], OSNIMIL+1);
					if (FormJjVaihdot->CBLisenssi->Checked) {
						Kilp.ostiet[u].lisno = lisno[os];
						}
					if (FormJjVaihdot->CBEmitkoodit->Checked) {
						Kilp.ostiet[u].badge[0] = ekdi[os];
						Kilp.ostiet[u].laina[0] = lna[os];
						}
					if (FormJjVaihdot->CBHajonnat->Checked) {
						strncpy(Kilp.ostiet[u].hajonta, haj[os], HAJONTA);
						}
					}
				}
			break;
		case 102:
			for (int os = 0; os < Sarjat[Kilp.sarja].osuusluku; os++) {
				Kilp.setNimi(OsGrid->Cells[2][os+1].c_str(), OsGrid->Cells[1][os+1].c_str(), os);
				}
			break;
		case 103:
			for (int os = 0; os < Sarjat[Kilp.sarja].osuusluku; os++) {
				wchar_t wst[OSNIMIL+1], ste[OSNIMIL+1], sts[OSNIMIL+1];
				MbsToWcs(wst, st[os], OSNIMIL);
				if (wst[0] == 0)
					continue;
				if (wst[0] == L'|')
					wmemmove(wst, wst+1, OSNIMIL);
				if (wst[wcslen(wst-1)] == L'|')
					wst[wcslen(wst-1)] = 0;
				for (wchar_t *p = wst; *p; p++)
					if (*p == L'|')
						*p = L' ';
				wesnimet(wst, ste, sts);
				Kilp.setNimi(ste, sts, os);
				}
			break;
		case 104:
			for (int os = 0; os < Sarjat[Kilp.sarja].osuusluku; os++) {
				int ens = 1;
				wcsncpy(wst, OsGrid->Cells[2][os+1].c_str(), OSNIMIL);
				for (p = wst; *p; p++) {
					if (ens)
						*p = towupper2(*p);
					else
						*p = towlower2(*p);
					ens = *p == L' ';
					}
				OsGrid->Cells[2][os+1] = wst;
				ens = 1;
				wcsncpy(wst, OsGrid->Cells[1][os+1].c_str(), OSNIMIL);
				for (p = wst; *p; p++) {
					if (ens)
						*p = towupper2(*p);
					else
						*p = towlower2(*p);
					ens = *p == L' ';
					}
				OsGrid->Cells[1][os+1] = wst;
				Kilp.setNimi(OsGrid->Cells[1][os+1].c_str(), OsGrid->Cells[2][os+1].c_str(), os);
				}
			break;
		}
	jjFormHeight = FormJjVaihdot->Height;
	delete FormJjVaihdot;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::TlsGridSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
	if (aktvarow == ARow && aktvacol == ACol)
		return;
	if (sallimuokkaus && RGVaTieto->ItemIndex < 2 && aktvacol > 0 && aktvarow > 0) {
		paivitaVaMuutos(aktvacol, aktvarow);
	}
	aktvarow = ARow;
	aktvacol = ACol;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::TlsGridExit(TObject *Sender)
{
	if (sallimuokkaus && RGVaTieto->ItemIndex < 2 && aktvacol > 0 && aktvarow > 0) {
		paivitaVaMuutos(aktvacol, aktvarow);
	}

	aktvarow = 0;
	aktvacol = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::TlsGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_F9 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (sallimuokkaus) {
			BtnTallennaClick(Sender);
			}
		else {
			BtnSalliClick(Sender);
			}
		}
	if (Key == VK_RIGHT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (TlsGrid->Col < TlsGrid->ColCount-1)
			TlsGrid->Col++;
		}
	if (Key == VK_LEFT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (TlsGrid->Col > 0)
			TlsGrid->Col--;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::TlsGridKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (sallimuokkaus && RGVaTieto->ItemIndex < 2 && Key == L'\r' && TlsGrid->Col > 0 && TlsGrid->Row > 0) {
		Key = 0;
		paivitaVaMuutos(TlsGrid->Col, TlsGrid->Row);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::TlsGridClick(TObject *Sender)
{
	bool sel;

	TlsGridSelectCell(Sender, TlsGrid->Col, TlsGrid->Row, sel);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::OsGridDblClick(TObject *Sender)
{
	if (OsGrid->Row == 0)
		return;
	if (!OsuusTiedot)
		OsuusTiedot = new TFormOsuus(this);

	OsuusTiedot->naytaOsuus(Kilp.kilpno, OsGrid->Row);
	OsuusTiedot->Show();
	if (OsuusTiedot->WindowState == wsMinimized)
		OsuusTiedot->WindowState = wsNormal;
	OsuusTiedot->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall TFormJoukkuetiedot::oistuvienlisystentila1Click(TObject *Sender)
{
	oistuvienlisystentila1->Checked = !oistuvienlisystentila1->Checked;
	if (oistuvienlisystentila1->Checked) {
		salliNumeromuutos = true;
		Liskilpailija1Click(Sender);
		}
	else {
		salliNumeromuutos = false;
		BtnSalliClick(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::Suljekaavake1Click(TObject *Sender)
{
	BtnSuljeClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormJoukkuetiedot::Lisenssinumero1Click(TObject *Sender)
{
	Lisenssinumero1->Checked = !Lisenssinumero1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::Badge1Click(TObject *Sender)
{
	Badge1->Checked = !Badge1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::Henkilhaku1Click(TObject *Sender)
{
	if (sallimuokkaus)
		return;
	FormHenkHaku = new TFormHenkHaku(this);
	FormHenkHaku->ShowModal();
	if (FormHenkHaku->ModalResult == mrOk && FormHenkHaku->KilpNo)
		naytaKilpailija(getpos(FormHenkHaku->KilpNo));
	delete FormHenkHaku;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkuetiedot::BtnPaivitaClick(TObject *Sender)
{
	if (dKilp) {
		Kilp.getrec(dKilp);
		Kilp1 = Kilp;
		Sarja = Kilp.sarja;
		naytaTiedot();
		}
}
//---------------------------------------------------------------------------

