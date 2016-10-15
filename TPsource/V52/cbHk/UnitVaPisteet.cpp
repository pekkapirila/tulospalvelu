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
#pragma hdrstop

#include "UnitVaPisteet.h"
#include "UnitTuloskaava.h"
#include "UnitSarjatiedot.h"
#include "UnitVaToisto.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormVaPisteet *FormVaPisteet;

//---------------------------------------------------------------------------
__fastcall TFormVaPisteet::TFormVaPisteet(TComponent* Owner)
	: TForm(Owner)
{
	NLaji = 5;
	NNaytaLaji = NLaji;
	ValLaji = 0;
	ComboBox1->ItemIndex = 0;
	Mrmatkatratatiedoista1->Enabled = (ToimintaTila == 2);
}
//---------------------------------------------------------------------------
void __fastcall TFormVaPisteet::editKaava(int srj, int pv, int va)
{
	if (VAGrid->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset tähänastiset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			tallennaTiedot();
			}
		VAGrid->Options >> (goEditing);
		}
	if (!FormTuloskaava)
		FormTuloskaava = new TFormTuloskaava(this);
	FormTuloskaava->AsetaPiste(srj, pv, va);
	FormTuloskaava->Show();
	if (FormTuloskaava->WindowState == wsMinimized)
		FormTuloskaava->WindowState = wsNormal;
	FormTuloskaava->BringToFront();
}

//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::naytaTiedot(void)
{
	int ipv, edRows;
	wchar_t line[20];

	aktcol = 0;
	aktrow = 0;
	ipv = CBPaiva->ItemIndex;
	if (CBPaiva->Items->Count < kilpparam.n_pv_akt) {
		for (int i = CBPaiva->Items->Count; i < kilpparam.n_pv_akt; i++) {
			CBPaiva->Items->Add(UnicodeString(i+1)+UnicodeString(L". päivä"));
			}
		}
	while (CBPaiva->Items->Count > kilpparam.n_pv_akt)
		CBPaiva->Items->Delete(CBPaiva->Items->Count-1);
	VAGrid->RowCount = NNaytaLaji*sarjaluku + 1;
	VAGrid->ColCount = kilpparam.valuku + 3;
	VAGrid->FixedCols = 2;
	edRows = CellsRowCount;
	CellsRowCount = NLaji*sarjaluku + 1;
	if (CellsRowCount != edRows && Cells != NULL)
		delete[] Cells;
	Cells = new cellTpA[CellsRowCount];
	for (int row = 0; row < CellsRowCount; row++) {
		for (int col = 0; col < VAGrid->ColCount; col++) {
			Cells[row][col].text = L"";
			}
		}
	for (int col = 2; col < VAGrid->ColCount-1; col++) {
		Cells[0][col].text = UnicodeString(col-1)+UnicodeString(L". va");
		}
	Cells[0][0].text = UnicodeString(L"Sarja");
	Cells[0][1].text = UnicodeString(L"Tieto");
	Cells[0][VAGrid->ColCount-1].text = UnicodeString(L"Maali");
	for (int srj = 0; srj < sarjaluku; srj++) {
		Cells[NLaji*srj + 1][0].text = UnicodeString(Sarjat[srj].sarjanimi);
		Cells[NLaji*srj + 1][1].text = UnicodeString(L"Sijainti");
		Cells[NLaji*srj + 2][1].text = UnicodeString(L"Aikaraja");
		if (wcswcind(kilpparam.kilplaji, L"AB") >= 0)
			Cells[NLaji*srj + 3][1].text = UnicodeString(L"Sakkoja");
		else
			Cells[NLaji*srj + 3][1].text = UnicodeString(L"Rastikoodi");
		Cells[NLaji*srj + 4][1].text = UnicodeString(L"Julkisuus");
		Cells[NLaji*srj + 5][1].text = UnicodeString(L"Kaava");
		}
	for (int srj = 0; srj < sarjaluku; srj++) {
		for (int iva = 0; iva < kilpparam.valuku; iva++) {
			Cells[NLaji*srj + 1][iva + 2].text = UnicodeString(Sarjat[srj].va_matka[ipv][iva]);
			Cells[NLaji*srj + 2][iva + 2].text = UnicodeString(aikatowstr_s(line, Sarjat[srj].va_raja[ipv][iva], 0)).SubString(1, 8);
			if (wcswcind(kilpparam.kilplaji, L"AB") >= 0)
				Cells[NLaji*srj + 3][iva + 2].text = UnicodeString(Sarjat[srj].va_sakot[ipv][iva+1]);
			else
				Cells[NLaji*srj + 3][iva + 2].text = UnicodeString(Sarjat[srj].va_koodi[ipv][iva]);
			if (Sarjat[srj].va_piilota[ipv][iva] == 0)
				Cells[NLaji*srj + 4][iva + 2].text = L"Piilota";
			else if (Sarjat[srj].va_piilota[ipv][iva] >= 9)
				Cells[NLaji*srj + 4][iva + 2].text = L"Julkinen";
			else
				Cells[NLaji*srj + 4][iva + 2].text = UnicodeString(L"Taso")+Sarjat[srj].va_piilota[ipv][iva];
			if (Sarjat[srj].kaava[ipv][iva+1].on)
				Cells[NLaji*srj + 5][iva + 2].text = L"Kaava";
			else
				Cells[NLaji*srj + 5][iva + 2].text = L" ";
			}
		Cells[NLaji*srj + 2][kilpparam.valuku + 2].text = UnicodeString(aikatowstr_s(line, Sarjat[srj].tul_raja[ipv], 0)).SubString(1, 8);
		}
	VAGrid->Refresh();
}
//---------------------------------------------------------------------------

int __fastcall TFormVaPisteet::tallennaTiedot(void)
{
	int ipv;

	ipv = CBPaiva->ItemIndex;
	for (int srj = 0; srj < sarjaluku; srj++) {
		Sarjat[srj].valuku[ipv] = 0;
		for (int iva = 0; iva < kilpparam.valuku; iva++) {
			wcsncpy(Sarjat[srj].va_matka[ipv][iva], Cells[NLaji*srj + 1][iva + 2].text.c_str(), sizeof(Sarjat[0].va_matka[0][0])/2-1);
			if (wcslen(Sarjat[srj].va_matka[ipv][iva]) > 0 && wcscmpU(Sarjat[srj].va_matka[ipv][iva], L"0")
				&& iva + 1 > Sarjat[srj].valuku[ipv]) {
				Sarjat[srj].valuku[ipv] = iva + 1;
				}
			Sarjat[srj].va_raja[ipv][iva] = wstrtoaika_vap(Cells[NLaji*srj + 2][iva + 2].text.c_str(), 0)/SEK;
			if (wcswcind(kilpparam.kilplaji, L"AB") >= 0)
				Sarjat[srj].va_sakot[ipv][iva+1] = _wtoi(Cells[NLaji*srj + 3][iva + 2].text.c_str());
			else
				Sarjat[srj].va_koodi[ipv][iva] = _wtoi(Cells[NLaji*srj + 3][iva + 2].text.c_str());
			switch(towupper(Cells[NLaji*srj + 4][iva + 2].text.c_str()[0])) {
				case L'P':
					Sarjat[srj].va_piilota[ipv][iva] = 0;
					break;
				case L'J':
					Sarjat[srj].va_piilota[ipv][iva] = 10;
					break;
				default:
					Sarjat[srj].va_piilota[ipv][iva] = _wtoi(Cells[NLaji*srj + 4][iva + 2].text.c_str()+4);
					break;
				}
			}
		Sarjat[srj].tul_raja[ipv] = wstrtoaika_vap(Cells[NLaji*srj + 2][kilpparam.valuku + 2].text.c_str(), 0)/SEK;
		}
	kirjEventXml(sarjaxmlfname, true, false, false);
	naytaTiedot();
//	kirjsarjat();
	return(0);
}
void __fastcall TFormVaPisteet::Button1Click(TObject *Sender)
{
	VAGrid->Options << (goEditing);
}
//---------------------------------------------------------------------------
void __fastcall TFormVaPisteet::Button3Click(TObject *Sender)
{
	naytaTiedot();
	VAGrid->Options >> (goEditing);
}
//---------------------------------------------------------------------------
void __fastcall TFormVaPisteet::Button2Click(TObject *Sender)
{
	tallennaTiedot();
	VAGrid->Options >> (goEditing);
}
//---------------------------------------------------------------------------
void __fastcall TFormVaPisteet::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (VAGrid->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			tallennaTiedot();
			}
		else
			VAGrid->Options >> (goEditing);
		}
	if (FormVaToisto)
		delete FormVaToisto;
	FormVaToisto = NULL;
}
//---------------------------------------------------------------------------
int __fastcall TFormVaPisteet::paivitaMuutos(int col, int row)
{
	wchar_t line[20], ch;
	int CRow, lj;

	CRow = CRowFromARow(row);
	lj = (row - 1) % NNaytaLaji;
	if (col > 1) {
		if (lj == 0) {
			Cells[CRow][col].text = Cells[CRow][col].text.SubString(1, sizeof(Sarjat[0].va_matka[0][0])/2-1);
			}
		else {
			if (NNaytaLaji == 2)
				lj = ValLaji;
			switch (lj) {
				case 1:
					Cells[CRow][col].text =
						UnicodeString(AIKATOWSTRS(line, wstrtoaika_vap(Cells[CRow][col].text.c_str(), 0), 0)).SubString(1, 8);
					break;
				case 2:
					Cells[CRow][col].text = _wtoi(Cells[CRow][col].text.c_str());
					break;
				case 3:
					ch = towupper(Cells[CRow][col].text.c_str()[0]);
					if (ch == L'T')
						ch = Cells[CRow][col].text.c_str()[Cells[CRow][col].text.Length()-1];
					if (ch > L'0' && ch <= L'9')
						Cells[CRow][col].text = L"Taso"+UnicodeString(ch);
					else
						Cells[CRow][col].text = ((ch == L'P') ? L"Piilota" : L"Julkinen");
					break;
				case 4:
					editKaava((row-1)/NNaytaLaji, CBPaiva->ItemIndex, col-1);
					if (Sarjat[(row-1)/NNaytaLaji].kaava[CBPaiva->ItemIndex][col-1].on)
						Cells[row][col].text = L"Kaava";
					break;
				}
			}
		}
	VAGrid->Refresh();
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormVaPisteet::Button4Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormVaPisteet::FormShow(TObject *Sender)
{
	CBPaiva->Clear();
	for (int i = 0;i < kilpparam.n_pv_akt; i++) {
	   CBPaiva->Items->Add(UnicodeString(i+1)+L". vaihe");
		}
	CBPaiva->ItemIndex = k_pv;
	if (FormSarjatiedot->CBTiedot->ItemIndex > 0)
		CBPaiva->ItemIndex = FormSarjatiedot->CBTiedot->ItemIndex - 1;
	naytaTiedot();
}
//---------------------------------------------------------------------------
void __fastcall TFormVaPisteet::FormResize(TObject *Sender)
{
	VAGrid->Width = ClientWidth - 16 * Screen->PixelsPerInch / 96;
	VAGrid->Height = ClientHeight - VAGrid->Top - 8 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaPisteet::VAGridSelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	if (VAGrid->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}

	aktcol = ACol;
	aktrow = ARow;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaPisteet::VAGridExit(TObject *Sender)
{
	if (VAGrid->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}

	aktcol = 0;
	aktrow = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaPisteet::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Sarjat");
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::Kopioi1Click(TObject *Sender)
{
	bool onkaava = false;
	for (int srj = 1; srj < sarjaluku; srj++) {
	   for (int col = 2; col < VAGrid->ColCount; col++) {
			for (int lj = 0; lj < NLaji; lj++) {
				if (lj != 4)
					Cells[NLaji*srj+lj+1][col].text = Cells[lj+1][col].text;
				else if (Cells[lj+1][col].text.c_str()[0] == L'K')
					onkaava = true;
				}
			}
		}
	if (onkaava)
		Application->MessageBoxW(L"Kaavojen kopiointi sarjasta toisiin tehdään kaavan määrittelykaavakkeella", L"Varoitus", MB_OK);
	VAGrid->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::VAGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RIGHT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (VAGrid->Col < VAGrid->ColCount-1)
			VAGrid->Col++;
		}
	if (Key == VK_LEFT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (VAGrid->Col > 2)
			VAGrid->Col--;
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormVaPisteet::VAGridDblClick(TObject *Sender)
{
	if (NNaytaLaji > 1 && VAGrid->Col > 1 && VAGrid->Col < VAGrid->ColCount - 1 &&
		CRowFromARow(VAGrid->Row) % NLaji == 0)
		editKaava((VAGrid->Row-1)/NNaytaLaji, CBPaiva->ItemIndex, VAGrid->Col-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::Mritteletuloksenlaskentakaava1Click(TObject *Sender)

{
	editKaava(aktrow > 0 ?(aktrow-1)/NNaytaLaji : 0, CBPaiva->ItemIndex, aktrow > 0 ? aktcol-1 : 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::CBPaivaChange(TObject *Sender)
{
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::Kirjoitatekstitiedostoon1Click(TObject *Sender)
{
	TextFl *OutFl;

	if (VAGrid->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			tallennaTiedot();
			}
		else
			VAGrid->Options >> (goEditing);
		}
	SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->FileName = UnicodeString(L"vapisteet.csv");
	SaveDialog1->DefaultExt = L"csv";
	SaveDialog1->Filter = L"Csv-tiedostot (*.csv)|*.CSV|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		OutFl = new TextFl(SaveDialog1->FileName.c_str(), L"wt");
		if (OutFl->IsOpen()) {
			wchar_t line[1000];
			for (int srj = 0; srj < sarjaluku; srj++) {
				swprintf(line, L"%s", Sarjat[srj].sarjanimi);
				for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
					swprintf(line+wcslen(line), L";%s;%d", Sarjat[srj].matka[ipv], Sarjat[srj].tul_raja[ipv]);
					for (int piste = 0; piste < kilpparam.valuku; piste++) {
						swprintf(line+wcslen(line), L";%s;%d;%d;%d;%d", Sarjat[srj].va_matka[ipv][piste],
						Sarjat[srj].va_koodi[ipv][piste], Sarjat[srj].va_sakot[ipv][piste+1],
						Sarjat[srj].va_piilota[ipv][piste], Sarjat[srj].va_raja[ipv][piste]);
						}
					}
				wcscat(line, L"\n");
				OutFl->WriteLine(line);
				}
			}
		delete OutFl;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::Luetekstitiedostosta1Click(TObject *Sender)
{
	TextFl *InFl;
	int srj = 0;
	wchar_t *fields[5*N_PV*(VALUKU+2)+2], erottimet[] = L";\t";
	int nfld = 0;

	OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->FileName = UnicodeString(L"vapisteet.csv");
	OpenDialog1->DefaultExt = L"csv";
	OpenDialog1->Filter = L"Csv-tiedostot (*.csv)|*.CSV|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		InFl = new TextFl(OpenDialog1->FileName.c_str(), L"rt");
		while (InFl->IsOpen() && !InFl->Feof()) {
			wchar_t line[1000];

			InFl->ReadLine(line, sizeof(line)/2-1);
			memset(fields, 0, sizeof(fields));
			nfld = getfields(line, fields, sizeof(fields)/sizeof(fields[0]), erottimet,
				L"\"", true, false);
			if (nfld < 5*kilpparam.n_pv_akt*(kilpparam.valuku+1) - 1)
				break;
			for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
				Sarjat[srj].tul_raja[ipv] = _wtoi(fields[5*ipv*(kilpparam.valuku+1)+2]);
				for (int piste = 0; piste < kilpparam.valuku; piste++) {
					int i;
					wcsncpy(Sarjat[srj].va_matka[ipv][piste], fields[5*(ipv*(kilpparam.valuku+1)+piste)+3],
						sizeof(Sarjat[srj].va_matka[ipv][piste])/2-1);
					Sarjat[srj].va_koodi[ipv][piste] = _wtoi(fields[5*(ipv*(kilpparam.valuku+1)+piste)+4]);
					Sarjat[srj].va_sakot[ipv][piste+1] = _wtoi(fields[5*(ipv*(kilpparam.valuku+1)+piste)+5]);
					Sarjat[srj].va_piilota[ipv][piste] = _wtoi(fields[5*(ipv*(kilpparam.valuku+1)+piste)+6]);
					i = 5*(ipv*(kilpparam.valuku+1)+piste)+7;
					if (i < nfld)
						Sarjat[srj].va_raja[ipv][piste] = _wtoi(fields[i]);
					}
				}
			srj++;
			}
		delete InFl;
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::VAGridMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (VAGrid->RowCount - VAGrid->FixedRows >= VAGrid->VisibleRowCount + VAGrid->TopRow)
		VAGrid->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::VAGridMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (VAGrid->TopRow > VAGrid->FixedRows)
		VAGrid->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::VAGridDrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State)
{
	int X;
	int CRow;

	CRow = CRowFromARow(ARow);
   Graphics::TBitmap *pBitmap = new Graphics::TBitmap();
	TRect Rect1;
   pBitmap->Width = Rect.Width();
	pBitmap->Height = Rect.Height();
   Rect1.Left = 0;
   Rect1.Top = 0;
   Rect1.Right = pBitmap->Width;
   Rect1.Bottom = pBitmap->Height;
   Rect1.Empty();
//   if (Lihavoitu1->Checked)
//		pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
//   else
		pBitmap->Canvas->Font->Style = TFontStyles();
   if (Cells && ARow < VAGrid->RowCount) {
	   if (ARow == 0 || ACol == 0 || ACol == 1)
		  pBitmap->Canvas->Brush->Color = clBtnFace;
	   else {
		  switch (Cells[CRow][ACol].style) {
			case 0:
				pBitmap->Canvas->Brush->Color = clWindow;
				break;
			case 1:
				pBitmap->Canvas->Brush->Color = TColor(korVari[0]);  //clLime;
//            pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
				break;
			case 2:
				pBitmap->Canvas->Brush->Color = TColor(korVari[1]);  //clYellow;
//            pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
				break;
			case 3:
				pBitmap->Canvas->Brush->Color = TColor(korVari[2]);  //clFuchsia;
				break;
			case 4:
				pBitmap->Canvas->Brush->Color = TColor(korVari[3]);  //clSkyBlue;
				break;
			case 5:
				pBitmap->Canvas->Brush->Color = TColor(korVari[4]);  //clAqua;
				break;
			case 6:
				pBitmap->Canvas->Brush->Color = TColor(korVari[5]);  //clOlive - light;
				break;
			case 7:
				pBitmap->Canvas->Brush->Color = TColor(0xffffd0);  //clOlive - light;
				break;
			 }
		  }
	   pBitmap->Canvas->FillRect(Rect1);
//	   pBitmap->Canvas->Font->Size = FontSize;
	   X = 1;
	   if (OtsCells[ACol].justify == 1) {
		   X =  Rect.Width() - pBitmap->Canvas->TextWidth(Cells[CRow][ACol].text) - 2;
		   if (X < 1)
			  X = 1;
		   }
	   pBitmap->Canvas->TextOut(X,1,Cells[CRow][ACol].text);
//   pBitmap->Canvas->TextRect(Rect, X,1,Cells[ACol][ARow].text);
		}
   VAGrid->Canvas->Draw(Rect.Left, Rect.Top, pBitmap);
   delete pBitmap;
}
//---------------------------------------------------------------------------


void __fastcall TFormVaPisteet::FormDestroy(TObject *Sender)
{
	if (Cells)
		delete[] Cells;
	Cells = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::ComboBox1Change(TObject *Sender)
{
	if (VAGrid->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			tallennaTiedot();
			}
		else
			VAGrid->Options >> (goEditing);
		}
	ValLaji = ComboBox1->ItemIndex;
	if (ValLaji == 0)
		NNaytaLaji = NLaji;
	else if (ValLaji == NLaji)
		NNaytaLaji = 1;
	else
		NNaytaLaji = 2;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::VAGridSetEditText(TObject *Sender, int ACol, int ARow,
		  const UnicodeString Value)
{
	Cells[CRowFromARow(ARow)][ACol].text = Value;
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::Mrmatkatratatiedoista1Click(TObject *Sender)
{
	int kpv = CBPaiva->ItemIndex;
	ratatp *rt;

	if (!emitfl || !rata) {
		Application->MessageBox(L"Ei ratatietoja", L"Este", MB_OK);
		return;
		}

	if (VAGrid->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset aiemmat muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			tallennaTiedot();
			}
		else
			VAGrid->Options >> (goEditing);
		}
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (ToimintaTila != 2) {
			continue;
			}
		if ((rt = haerata_sarja(srj, kpv)) == NULL)
			continue;
		for (int iva = 0; iva < kilpparam.valuku; iva++) {
			if (Sarjat[srj].va_koodi[kpv][iva] == 0)
				continue;
			for (int k = 0; k < rt->rastiluku; k++) {
				if (rt->rastikoodi[k] == Sarjat[srj].va_koodi[kpv][iva]) {
					if (rt->matka[k] >= 50) {
						swprintf(Sarjat[srj].va_matka[kpv][iva], L"%d,%d",
						   (rt->matka[k]+50)/1000, ((rt->matka[k]+50)%1000)/100);
						break;
						}
					}
				}
			}
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::Kopioivaiheentiedotmuillevaiheille1Click(TObject *Sender)

{
	int kpv = CBPaiva->ItemIndex;

	if (VAGrid->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset tehdyt muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			tallennaTiedot();
			}
		else
			VAGrid->Options >> (goEditing);
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		if (ipv == kpv)
			continue;
		for (int srj = 0; srj < sarjaluku; srj++) {
			memcpy(Sarjat[srj].va_matka[ipv], Sarjat[srj].va_matka[kpv], sizeof(Sarjat[srj].va_matka[ipv]));
			memcpy(Sarjat[srj].va_koodi[ipv], Sarjat[srj].va_koodi[kpv], sizeof(Sarjat[srj].va_koodi[ipv]));
			memcpy(Sarjat[srj].va_sakot[ipv], Sarjat[srj].va_sakot[kpv], sizeof(Sarjat[srj].va_sakot[ipv]));
			memcpy(Sarjat[srj].va_piilota[ipv], Sarjat[srj].va_piilota[kpv], sizeof(Sarjat[srj].va_piilota[ipv]));
			memcpy(Sarjat[srj].va_raja[ipv], Sarjat[srj].va_raja[kpv], sizeof(Sarjat[srj].va_raja[ipv]));
			Sarjat[srj].valuku[ipv] = Sarjat[srj].valuku[kpv];
			Sarjat[srj].tul_raja[ipv] = Sarjat[srj].tul_raja[kpv];
			memcpy(Sarjat[srj].kaava[ipv], Sarjat[srj].kaava[kpv], sizeof(Sarjat[srj].kaava[ipv]));
			}
		}
}
//---------------------------------------------------------------------------

int __fastcall TFormVaPisteet::CRowFromARow(int ARow)
{
	int CRow;

	if (NNaytaLaji == NLaji)
		CRow = ARow;
	else if (NNaytaLaji == 1)
		CRow = NLaji * (ARow - 1) + 1;
	else {
		CRow = NLaji * (((ARow+1) / 2) - 1) + 1;
		if (ARow % 2 == 0)
			CRow += ValLaji;
		}
	if (CRow < 0)
		CRow = 0;
	return(CRow);
}
//---------------------------------------------------------------------------


void __fastcall TFormVaPisteet::Kopioitmsolukaikkiinsarjoihin1Click(TObject *Sender)

{
	int CRow, lj;

	UnicodeString Val;
	if (VAGrid->Options.Contains(goEditing)) {
		CRow = CRowFromARow(VAGrid->Row);
		lj = (CRow - 1) % NLaji;
		if (CRow > 0 && VAGrid->Col > 1) {
			Val = Cells[CRow][VAGrid->Col].text;
			for (int srj = 0; srj < sarjaluku; srj++)
			   Cells[NLaji*srj+lj+1][VAGrid->Col].text = Val;
			}
		VAGrid->Refresh();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::Luotasavlisetvliajatsarjalle1Click(TObject *Sender)

{
	int ipv = CBPaiva->ItemIndex;
	int srj = (VAGrid->Row-1)/NLaji;
	if (srj < 0 || srj >= sarjaluku)
		return;
	if (!FormVaToisto)
		FormVaToisto = new TFormVaToisto(this);
	FormVaToisto->EdtSarja->Text = Sarjat[srj].sarjanimi;
	FormVaToisto->ShowModal();
	if (FormVaToisto->ModalResult == mrOk) {
		int n, vali, ensmtk, askel, ens, tapa, julk;
		wchar_t des;
		n = _wtoi(FormVaToisto->EdtLkm->Text.c_str());
		vali = _wtoi(FormVaToisto->EdtVali->Text.c_str());
		ensmtk = _wtoi(FormVaToisto->EdtMtk1->Text.c_str());
		askel = _wtoi(FormVaToisto->EdtAskel->Text.c_str());
		ens = _wtoi(FormVaToisto->EdtEnspiste->Text.c_str());
		julk = FormVaToisto->CBJulkisuus->ItemIndex;
		if (julk > 3)
			julk = 9;
		tapa = FormVaToisto->CBtapa->ItemIndex;
		des = L",."[FormVaToisto->RGdes->ItemIndex];
		for (int i = 0; i < n; i++) {
			int va = ens+i*askel-1;
			int m = ensmtk + i*vali;
			wchar_t st[30] = L"";
			if (va >= kilpparam.valuku)
				continue;
			switch (tapa) {
				case 0:
					m = (m+500) / 1000;
					_itow(m, st, 10);
					break;
				case 1:
					m = (m+50) / 100;
					swprintf(st, L"%d%c%1.1d", m/10, des, m%10);
					break;
				case 2:
					m = (m+5) / 10;
					swprintf(st, L"%d%c%2.2d", m/100, des, m%100);
					break;
				case 3:
					_itow(m, st, 10);
					break;
				}
			wcscpy(Sarjat[srj].va_matka[ipv][va], st);
			Sarjat[srj].va_piilota[ipv][va] = julk;
			}
		naytaTiedot();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::Poistasolunvliaika1Click(TObject *Sender)
{
	int CRow, lj, srj;

	if (VAGrid->Options.Contains(goEditing)) {
		CRow = CRowFromARow(VAGrid->Row) - (CRowFromARow(VAGrid->Row)-1) % NLaji;
		Cells[CRow][VAGrid->Col].text = L"";
		Cells[CRow+1][VAGrid->Col].text = L"00.00.00";
		Cells[CRow+2][VAGrid->Col].text = L"0";
		Cells[CRow+3][VAGrid->Col].text = L"Piilota";
		Cells[CRow+4][VAGrid->Col].text = L"";
		VAGrid->Refresh();
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormVaPisteet::VAGridGetEditText(TObject *Sender, int ACol, int ARow,
		  UnicodeString &Value)
{
	Value = Cells[CRowFromARow(ARow)][ACol].text;
}
//---------------------------------------------------------------------------

void __fastcall TFormVaPisteet::Kopioikaikkivalitunpisteentiedotmuihinsarjoihin1Click(TObject *Sender)

{
	int CRow, lj;

	UnicodeString Val;
	if (VAGrid->Options.Contains(goEditing)) {
		CRow = CRowFromARow(VAGrid->Row) - (CRowFromARow(VAGrid->Row)-1) % NLaji;
		if (CRow > 0 && VAGrid->Col > 1) {
			for (int srj = 0; srj < sarjaluku; srj++) {
				if (NLaji*srj+1 != CRow) {
					for (lj = 0; lj < NLaji; lj++)
						Cells[NLaji*srj+lj+1][VAGrid->Col].text = Cells[CRow+lj][VAGrid->Col].text;
					}
				}
			}
		VAGrid->Refresh();
		}
}
//---------------------------------------------------------------------------

