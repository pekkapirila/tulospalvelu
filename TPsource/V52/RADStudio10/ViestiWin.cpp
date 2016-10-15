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
#include <tchar.h>
#pragma hdrstop
#include "UnitMain.h"
//---------------------------------------------------------------------------
USEFORM("..\ViestiWin\UnitHenkHaku.cpp", FormHenkHaku);
USEFORM("..\ViestiWin\UnitHylkRap.cpp", FormHylkRap);
USEFORM("..\ViestiWin\UnitImport.cpp", FormImport);
USEFORM("..\ViestiWin\UnitGAZ.cpp", FormGAZ);
USEFORM("..\ViestiWin\UnitEmitMuutokset.cpp", FormEmitMuutokset);
USEFORM("..\ViestiWin\UnitEmit_VA.cpp", FormEmit_VA);
USEFORM("..\ViestiWin\UnitExport.cpp", FormExport);
USEFORM("..\ViestiWin\UnitJjVaihdot.cpp", FormJjVaihdot);
USEFORM("..\ViestiWin\UnitLahestyjat.cpp", FormLahestyjat);
USEFORM("..\ViestiWin\UnitLisatiedot.cpp", FormLisatiedot);
USEFORM("..\ViestiWin\UnitLuenta.cpp", FormLuenta);
USEFORM("..\ViestiWin\UnitKilpSeurat.cpp", FormKilpSeurat);
USEFORM("..\ViestiWin\UnitJoukkueet.cpp", FormJoukkueet);
USEFORM("..\ViestiWin\UnitJoukkuetiedot.cpp", FormJoukkuetiedot);
USEFORM("..\ViestiWin\UnitKilpMaaritys.cpp", FormKilpMaaritys);
USEFORM("..\ViestiWin\TulosUnit.cpp", TilanneForm);
USEFORM("..\ViestiWin\UnitAikavert.cpp", FormAikavert);
USEFORM("..\ViestiWin\UnitAjanotto.cpp", FormAjanotto);
USEFORM("..\ViestiWin\TulostusUnit2.cpp", TulosteForm);
USEFORM("..\ViestiWin\MuotoiluUnit.cpp", FormMuotoilu);
USEFORM("..\ViestiWin\OsuusTiedot.cpp", FormOsuus);
USEFORM("..\ViestiWin\TapUnit2.cpp", TapForm);
USEFORM("..\ViestiWin\UnitArvonta.cpp", FormArvonta);
USEFORM("..\ViestiWin\UnitComfile.cpp", FormComfile);
USEFORM("..\ViestiWin\UnitEmit.cpp", FormEmit);
USEFORM("..\ViestiWin\UnitEmithaku.cpp", FormEmithaku);
USEFORM("..\ViestiWin\UnitCfg.cpp", FormCfg);
USEFORM("..\ViestiWin\UnitASakot.cpp", FormASakot);
USEFORM("..\ViestiWin\UnitAutoFile.cpp", FormAutoFile);
USEFORM("..\ViestiWin\UnitAvausMaar.cpp", FormAvausMaar);
USEFORM("..\ViestiWin\UnitMain.cpp", FormMain);
USEFORM("..\ViestiWin\UnitYLahdot.cpp", FormYLahdot);
USEFORM("..\ViestiWin\UnitYv.cpp", FormYhtveto);
USEFORM("..\ViestiWin\VertAikaUnit.cpp", VertAikaFrm);
USEFORM("..\ViestiWin\UnitVaTuloste.cpp", FormVaTuloste);
USEFORM("..\ViestiWin\UnitSuodatus.cpp", FormSuodatus);
USEFORM("..\ViestiWin\UnitVaGr.cpp", FormVaGr);
USEFORM("..\ViestiWin\UnitValSarjat.cpp", FormValSarjat);
USEFORM("..\ViestiWin\UnitOsuuskopiointi.cpp", FormOsuuskopiointi);
USEFORM("..\ViestiWin\UnitPartio.cpp", FormPartio);
USEFORM("..\ViestiWin\UnitPiirival.cpp", FormPiiriVal);
USEFORM("..\ViestiWin\UnitOhjtiedot.cpp", FormOhjtiedot);
USEFORM("..\ViestiWin\UnitMerkKoodeja.cpp", FormMerkKoodeja);
USEFORM("..\ViestiWin\UnitMySQL.cpp", FormMySQL);
USEFORM("..\ViestiWin\UnitNollaus.cpp", FormNollaus);
USEFORM("..\ViestiWin\UnitSakkoKierr.cpp", FormSakkoKierr);
USEFORM("..\ViestiWin\UnitSeurGr.cpp", SeurantaForm);
USEFORM("..\ViestiWin\UnitSHLTul.cpp", FormSHLtul);
USEFORM("..\ViestiWin\UnitStatus.cpp", FormStatus);
USEFORM("..\ViestiWin\UnitSeuraval.cpp", FormSeuraVal);
USEFORM("..\ViestiWin\UnitSarja.cpp", FormSarja);
USEFORM("..\ViestiWin\UnitSarjatiedot.cpp", FormSarjatiedot);
USEFORM("..\ViestiWin\UnitSeurat.cpp", FormSeurat);
USEFORM("..\cbTp\UnitMsgs.cpp", FormMsgs);
USEFORM("..\cbTp\UnitMTR.cpp", FormMTR);
USEFORM("..\cbTp\UnitNappain.cpp", FormNappain);
USEFORM("..\cbTp\UnitMessages.cpp", FormMessages);
USEFORM("..\cbTp\UnitKohdistus.cpp", FormKohdistus);
USEFORM("..\cbTp\UnitLisaTekstit.cpp", FormLisaTekstit);
USEFORM("..\cbTp\UnitMaalikello.cpp", FormMaalikello);
USEFORM("..\cbTp\UnitNaytaTulos.cpp", FormNaytaTulos);
USEFORM("..\cbTp\UnitSelect2.cpp", FormValitse2);
USEFORM("..\cbTp\UnitSelect3.cpp", FormSelect3);
USEFORM("..\cbTp\UnitTulkinta.cpp", FormTulkinta);
USEFORM("..\cbTp\UnitSelect.cpp", Valitse);
USEFORM("..\cbTp\UnitPiikit.cpp", FormPiikit);
USEFORM("..\cbTp\UnitRadat.cpp", FormRadat);
USEFORM("..\cbTp\UnitSarjaVal.cpp", FormSarjaVal);
USEFORM("..\cbTp\UnitAnnaTeksti.cpp", FormAnnaTeksti);
USEFORM("..\cbTp\UnitBrowser.cpp", FormBrowser);
USEFORM("..\cbTp\UnitEditDialog.cpp", EditDialog);
USEFORM("..\cbTp\UnitAnnaArvo.cpp", FormAnnaArvo);
USEFORM("..\cbTp\UnitAikakorjaus.cpp", FormAikakorjaus);
USEFORM("..\cbTp\UnitAnnaAika.cpp", FormAnnaAika);
USEFORM("..\cbTp\UnitEmiTag.cpp", FormEmiTag);
USEFORM("..\cbTp\UnitJakelu.cpp", FormJakelu);
USEFORM("..\cbTp\UnitKartta.cpp", FormKartta);
USEFORM("..\cbTp\UnitKirjoitinVal.cpp", FormKirjVal);
USEFORM("..\cbTp\UnitHtmlEdit.cpp", FormHtmlEdit);
USEFORM("..\cbTp\UnitEmitFile.cpp", FormEmitFile);
USEFORM("..\cbTp\UnitEmitSQL.cpp", FormEmitSQL);
USEFORM("..\cbTp\UnitFIS.cpp", FormFIS);
USEFORM("..\cbTp\UnitUusinta.cpp", FormUusinta);
USEFORM("..\cbTp\UnitYhteysStatus.cpp", FormYhteysStatus);
USEFORM("..\cbTp\UnitIlmoitus.cpp", FormIlmoitus);
//---------------------------------------------------------------------------
WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
   try
   {
	   Application->Initialize();
	   Application->Title = L"ViestiWin";
	   Application->Title = "ViestiWin";
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->CreateForm(__classid(TFormStatus), &FormStatus);
		Application->CreateForm(__classid(TFormSarjatiedot), &FormSarjatiedot);
		Application->CreateForm(__classid(TFormMessages), &FormMessages);
		Application->CreateForm(__classid(TFormBrowser), &FormBrowser);
		Application->CreateForm(__classid(TFormIlmoitus), &FormIlmoitus);
		FormMain->Aloita();
		Application->Run();
   }
   catch (Exception &exception)
   {
	   Application->ShowException(&exception);
   }
   catch (...)
   {
	   try
	   {
		  throw Exception("");
	   }
	   catch (Exception &exception)
	   {
		  Application->ShowException(&exception);
	   }
   }
   return 0;
}
//---------------------------------------------------------------------------
/*
		Application->CreateForm(__classid(TFormHylkRap), &FormHylkRap);
		Application->CreateForm(__classid(TFormFIS), &FormFIS);
		Application->CreateForm(__classid(TFormMaalikello), &FormMaalikello);
		Application->CreateForm(__classid(TFormLahestyjat), &FormLahestyjat);
		Application->CreateForm(__classid(TFormCfg), &FormCfg);
		Application->CreateForm(__classid(TFormAnnaAika), &FormAnnaAika);
		Application->CreateForm(__classid(TFormAnnaArvo), &FormAnnaArvo);
		Application->CreateForm(__classid(TFormAnnaTeksti), &FormAnnaTeksti);
		Application->CreateForm(__classid(TEditDialog), &EditDialog);
		Application->CreateForm(__classid(TFormHtmlEdit), &FormHtmlEdit);
		Application->CreateForm(__classid(TFormMsgs), &FormMsgs);
		Application->CreateForm(__classid(TFormSarja), &FormSarja);
		Application->CreateForm(__classid(TFormExport), &FormExport);
		Application->CreateForm(__classid(TFormKilpSeurat), &FormKilpSeurat);
		Application->CreateForm(__classid(TFormLisatiedot), &FormLisatiedot);
		Application->CreateForm(__classid(TFormRadat), &FormRadat);
		Application->CreateForm(__classid(TFormLuenta), &FormLuenta);
		Application->CreateForm(__classid(TFormAjanotto), &FormAjanotto);
		Application->CreateForm(__classid(TFormVaTuloste), &FormVaTuloste);
		Application->CreateForm(__classid(TFormSeurat), &FormSeurat);
		Application->CreateForm(__classid(TFormPartio), &FormPartio);
		Application->CreateForm(__classid(TFormLisaTekstit), &FormLisaTekstit);
		Application->CreateForm(__classid(TFormASakot), &FormASakot);
		Application->CreateForm(__classid(TFormHenkHaku), &FormHenkHaku);
		Application->CreateForm(__classid(TFormPiikit), &FormPiikit);
		Application->CreateForm(__classid(TFormSHLtul), &FormSHLtul);
		Application->CreateForm(__classid(TFormSarjaVal), &FormSarjaVal);
		Application->CreateForm(__classid(TFormEmitSQL), &FormEmitSQL);
		Application->CreateForm(__classid(TFormEmithaku), &FormEmithaku);
		Application->CreateForm(__classid(TFormMySQL), &FormMySQL);
		Application->CreateForm(__classid(TFormEmitFile), &FormEmitFile);
		Application->CreateForm(__classid(TFormEmit_VA), &FormEmit_VA);
		Application->CreateForm(__classid(TFormAikakorjaus), &FormAikakorjaus);
		Application->CreateForm(__classid(TFormAikavert), &FormAikavert);
		Application->CreateForm(__classid(TFormNaytaTulos), &FormNaytaTulos);
		Application->CreateForm(__classid(TFormSakkoKierr), &FormSakkoKierr);
		Application->CreateForm(__classid(TFormKohdistus), &FormKohdistus);
		Application->CreateForm(__classid(TFormYhteysStatus), &FormYhteysStatus);
		Application->CreateForm(__classid(TFormJakelu), &FormJakelu);
*/


