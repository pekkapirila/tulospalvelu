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
#include <tchar.h>
#include "WinHk.h"
//---------------------------------------------------------------------------

USEFORM("..\cbTp\UnitMaalikello.cpp", FormMaalikello);
USEFORM("..\cbTp\UnitLisaTekstit.cpp", FormLisaTekstit);
USEFORM("..\cbTp\UnitMsgs.cpp", FormMsgs);
USEFORM("..\cbTp\UnitMessages.cpp", FormMessages);
USEFORM("..\cbTp\UnitKohdistus.cpp", FormKohdistus);
USEFORM("..\cbTp\UnitJakelu.cpp", FormJakelu);
USEFORM("..\cbTp\UnitIlmoitus.cpp", FormIlmoitus);
USEFORM("..\cbTp\UnitKirjoitinVal.cpp", FormKirjVal);
USEFORM("..\cbTp\UnitKartta.cpp", FormKartta);
USEFORM("..\cbTp\UnitSelect.cpp", Valitse);
USEFORM("..\cbTp\UnitSarjaVal.cpp", FormSarjaVal);
USEFORM("..\cbTp\UnitSelect3.cpp", FormSelect3);
USEFORM("..\cbTp\UnitSelect2.cpp", FormValitse2);
USEFORM("..\cbTp\UnitRadat.cpp", FormRadat);
USEFORM("..\cbTp\UnitNappain.cpp", FormNappain);
USEFORM("..\cbTp\UnitMTR.cpp", FormMTR);
USEFORM("..\cbTp\UnitPiikit.cpp", FormPiikit);
USEFORM("..\cbTp\UnitNaytaTulos.cpp", FormNaytaTulos);
USEFORM("..\cbTp\UnitAikakorjaus.cpp", FormAikakorjaus);
USEFORM("..\cbTp\UnitAnnaAika.cpp", FormAnnaAika);
USEFORM("..\cbTp\UnitAmpPaikat.cpp", FormAmpPaikat);
USEFORM("..\cbHk\VertAikaUnit.cpp", VertAikaFrm);
USEFORM("..\cbHk\UnitYv.cpp", FormYhtveto);
USEFORM("..\cbHk\WinHk.cpp", FormMain);
USEFORM("..\cbTp\UnitEmitSQL.cpp", FormEmitSQL);
USEFORM("..\cbTp\UnitEmitFile.cpp", FormEmitFile);
USEFORM("..\cbTp\UnitHtmlEdit.cpp", FormHtmlEdit);
USEFORM("..\cbTp\UnitFIS.cpp", FormFIS);
USEFORM("..\cbTp\UnitEmiTag.cpp", FormEmiTag);
USEFORM("..\cbTp\UnitAnnaTeksti.cpp", FormAnnaTeksti);
USEFORM("..\cbTp\UnitAnnaArvo.cpp", FormAnnaArvo);
USEFORM("..\cbTp\UnitEditDialog.cpp", EditDialog);
USEFORM("..\cbTp\UnitBrowser.cpp", FormBrowser);
USEFORM("..\cbTp\UnitYhteysStatus.cpp", FormYhteysStatus);
USEFORM("..\cbTp\UnitTietoTulk.cpp", FormTietoTulk);
USEFORM("..\cbTp\UnitSivuTuloste.cpp", FormSivuTuloste);
USEFORM("..\cbTp\UnitUusinta.cpp", FormUusinta);
USEFORM("..\cbTp\UnitTulkinta.cpp", FormTulkinta);
USEFORM("..\cbHk\UnitYhdistetty.cpp", FormYhdistetty);
USEFORM("..\cbHk\UnitEmit.cpp", FormEmit);
USEFORM("..\cbHk\UnitComfile.cpp", FormComfile);
USEFORM("..\cbHk\UnitEmitKoodit.cpp", FormEmitKoodit);
USEFORM("..\cbHk\UnitEmithaku.cpp", FormEmithaku);
USEFORM("..\cbHk\UnitCfg.cpp", FormCfg);
USEFORM("..\cbHk\UnitASuunn.cpp", FormASuunn);
USEFORM("..\cbHk\UnitASakot.cpp", FormASakot);
USEFORM("..\cbHk\UnitAvausMaar.cpp", FormAvausMaar);
USEFORM("..\cbHk\UnitAutoFile.cpp", FormAutoFile);
USEFORM("..\cbHk\UnitJoukkueTilanne.cpp", FormJoukkueTilanne);
USEFORM("..\cbHk\UnitImport.cpp", FormImport);
USEFORM("..\cbHk\UnitKilpailijaEmit.cpp", FormKilpailijaEmit);
USEFORM("..\cbHk\UnitKaavaTaul.cpp", FormKaavaTaul);
USEFORM("..\cbHk\UnitIlm2.cpp", FormIlm2);
USEFORM("..\cbHk\UnitEmit_VA.cpp", FormEmit_VA);
USEFORM("..\cbHk\UnitEmitMuutokset.cpp", FormEmitMuutokset);
USEFORM("..\cbHk\UnitHylkRap.cpp", FormHylkRap);
USEFORM("..\cbHk\UnitExport.cpp", FormExport);
USEFORM("..\cbHk\MuotoiluUnit.cpp", FormMuotoilu);
USEFORM("..\cbHk\LoppuKilpUnit.cpp", LoppuKilpForm);
USEFORM("..\cbHk\AsetusUnit.cpp", AsetusForm);
USEFORM("..\cbHk\UnitAikavert.cpp", FormAikavert);
USEFORM("..\cbHk\UnitAikaSiirto.cpp", FormAikaSiirto);
USEFORM("..\cbHk\UnitArvonta.cpp", FormArvonta);
USEFORM("..\cbHk\UnitAjanotto.cpp", FormAjanotto);
USEFORM("..\cbHk\TulosUnit.cpp", TilanneForm);
USEFORM("..\cbHk\TapUnit2.cpp", TapForm);
USEFORM("..\cbHk\TakaaAjoUnit.cpp", TakaaAjoForm);
USEFORM("..\cbHk\TulostusUnit2.cpp", TulosteForm);
USEFORM("..\cbHk\UnitTapahtSiirrot.cpp", FormTapahtSiirrot);
USEFORM("..\cbHk\UnitSuodatus.cpp", FormSuodatus);
USEFORM("..\cbHk\UnitTaulu.cpp", FormTaulu);
USEFORM("..\cbHk\UnitTarkMerk.cpp", FormTarkMerk);
USEFORM("..\cbHk\UnitStatus.cpp", FormStatus);
USEFORM("..\cbHk\UnitSHLTul.cpp", FormSHLtul);
USEFORM("..\cbHk\UnitSeuraval.cpp", FormSeuraVal);
USEFORM("..\cbHk\UnitSprintSarja.cpp", FormSprintSarja);
USEFORM("..\cbHk\UnitSprinthiihto.cpp", FormSprinthiihto);
USEFORM("..\cbHk\UnitValSarjat.cpp", FormValSarjat);
USEFORM("..\cbHk\UnitValitseKilp.cpp", FormValitseKilp);
USEFORM("..\cbHk\UnitVaToisto.cpp", FormVaToisto);
USEFORM("..\cbHk\UnitVaPisteet.cpp", FormVaPisteet);
USEFORM("..\cbHk\UnitVakLisays.cpp", FormVakLisays);
USEFORM("..\cbHk\UnitTuloskaava.cpp", FormTuloskaava);
USEFORM("..\cbHk\UnitTKanta.cpp", FormTKanta);
USEFORM("..\cbHk\UnitVaiheMaar.cpp", FormVaiheMaar);
USEFORM("..\cbHk\UnitVaGr.cpp", FormVaGr);
USEFORM("..\cbHk\UnitLisaTietue.cpp", FormLisaTietue);
USEFORM("..\cbHk\UnitLisatiedot.cpp", FormLisatiedot);
USEFORM("..\cbHk\UnitNollaus.cpp", FormNollaus);
USEFORM("..\cbHk\UnitMySQL.cpp", FormMySQL);
USEFORM("..\cbHk\UnitKisaLuett.cpp", FormKisaLuett);
USEFORM("..\cbHk\UnitKilpailijatiedot.cpp", FormKilpailijatiedot);
USEFORM("..\cbHk\UnitKilpailijaOnline.cpp", FormKilpailijaOnline);
USEFORM("..\cbHk\UnitKilpSeurat.cpp", FormKilpSeurat);
USEFORM("..\cbHk\UnitKilpMaaritys.cpp", FormKilpMaaritys);
USEFORM("..\cbHk\UnitSarjatiedot.cpp", FormSarjatiedot);
USEFORM("..\cbHk\UnitSarja.cpp", FormSarja);
USEFORM("..\cbHk\UnitSeuratoimet.cpp", FormSeuratoimet);
USEFORM("..\cbHk\UnitSeurat.cpp", FormSeurat);
USEFORM("..\cbHk\UnitSakkoKierr.cpp", FormSakkoKierr);
USEFORM("..\cbHk\UnitOsanottajat.cpp", FormOsanottajat);
USEFORM("..\cbHk\UnitOhjtiedot.cpp", FormOhjtiedot);
USEFORM("..\cbHk\UnitPistelasku.cpp", FormPistelasku);
USEFORM("..\cbHk\UnitPiirival.cpp", FormPiiriVal);
USEFORM("..\cbTp\UnitEmitnouto.cpp", FormEmitNouto);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
//		Application->MainFormOnTaskBar = true;
		Application->Title = "HkKisaWin";
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->CreateForm(__classid(TFormStatus), &FormStatus);
		Application->CreateForm(__classid(TFormBrowser), &FormBrowser);
		Application->CreateForm(__classid(TFormMessages), &FormMessages);
		Application->CreateForm(__classid(TFormTKanta), &FormTKanta);
		Application->CreateForm(__classid(TFormSeurat), &FormSeurat);
		Application->CreateForm(__classid(TFormSarjatiedot), &FormSarjatiedot);
		Application->CreateForm(__classid(TFormIlmoitus), &FormIlmoitus);
		FormMain->Aloita();
		Application->Run();
/*
		Application->CreateForm(__classid(TFormEmitNouto), &FormEmitNouto);
		Application->CreateForm(__classid(TFormHylkRap), &FormHylkRap);
		Application->CreateForm(__classid(TFormMaalikello), &FormMaalikello);
		Application->CreateForm(__classid(TFormTaulu), &FormTaulu);
		Application->CreateForm(__classid(TFormTietoTulk), &FormTietoTulk);
		Application->CreateForm(__classid(TFormYhdistetty), &FormYhdistetty);
		Application->CreateForm(__classid(TFormAmpPaikat), &FormAmpPaikat);
		Application->CreateForm(__classid(TTapForm), &TapForm);
		Application->CreateForm(__classid(TAsetusForm), &AsetusForm);
		Application->CreateForm(__classid(TFormOhjtiedot), &FormOhjtiedot);
		Application->CreateForm(__classid(TFormTuloskaava), &FormTuloskaava);
		Application->CreateForm(__classid(TFormKilpailijaEmit), &FormKilpailijaEmit);
		Application->CreateForm(__classid(TFormEmitMuutokset), &FormEmitMuutokset);
		Application->CreateForm(__classid(TFormKilpailijaOnline), &FormKilpailijaOnline);
		Application->CreateForm(__classid(TFormKilpailijatiedot), &FormKilpailijatiedot);
		Application->CreateForm(__classid(TFormKilpSeurat), &FormKilpSeurat);
		Application->CreateForm(__classid(TFormSeuraVal), &FormSeuraVal);
		Application->CreateForm(__classid(TFormPiiriVal), &FormPiiriVal);
		Application->CreateForm(__classid(TFormEmit), &FormEmit);
		Application->CreateForm(__classid(TFormIlm2), &FormIlm2);
		Application->CreateForm(__classid(TVertAikaFrm), &VertAikaFrm);
		Application->CreateForm(__classid(TFormOsanottajat), &FormOsanottajat);
		Application->CreateForm(__classid(TFormSarja), &FormSarja);
		Application->CreateForm(__classid(TFormStatus), &FormStatus);
		Application->CreateForm(__classid(TTilanneForm), &TilanneForm);
		Application->CreateForm(__classid(TFormAjanotto), &FormAjanotto);
		Application->CreateForm(__classid(TFormAnnaAika), &FormAnnaAika);
		Application->CreateForm(__classid(TFormAnnaArvo), &FormAnnaArvo);
		Application->CreateForm(__classid(TFormAnnaTeksti), &FormAnnaTeksti);
		Application->CreateForm(__classid(TFormArvonta), &FormArvonta);
		Application->CreateForm(__classid(TFormComfile), &FormComfile);
		Application->CreateForm(__classid(TEditDialog), &EditDialog);
		Application->CreateForm(__classid(TFormExport), &FormExport);
		Application->CreateForm(__classid(TFormIlm2), &FormIlm2);
		Application->CreateForm(__classid(TFormLisatiedot), &FormLisatiedot);
		Application->CreateForm(__classid(TFormMySQL), &FormMySQL);
		Application->CreateForm(__classid(TFormPistelasku), &FormPistelasku);
		Application->CreateForm(__classid(TFormSeuratoimet), &FormSeuratoimet);
		Application->CreateForm(__classid(TFormSprinthiihto), &FormSprinthiihto);
		Application->CreateForm(__classid(TFormTarkMerk), &FormTarkMerk);
		Application->CreateForm(__classid(TFormVaGr), &FormVaGr);
		Application->CreateForm(__classid(TFormVaPisteet), &FormVaPisteet);
		Application->CreateForm(__classid(TFormYhtveto), &FormYhtveto);
		Application->CreateForm(__classid(TFormLahtoajat), &FormLahtoajat);
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->CreateForm(__classid(TFormRadat), &FormRadat);
		Application->CreateForm(__classid(TFormTKanta), &FormTKanta);
		Application->CreateForm(__classid(TFormSeurat), &FormSeurat);
		Application->CreateForm(__classid(TFormSuodatus), &FormSuodatus);
		Application->CreateForm(__classid(TFormAikaSiirto), &FormAikaSiirto);
		Application->CreateForm(__classid(TFormUusinta), &FormUusinta);
		Application->CreateForm(__classid(TFormASakot), &FormASakot);
		Application->CreateForm(__classid(TFormAutoFile), &FormAutoFile);
		Application->CreateForm(__classid(TFormMsgs), &FormMsgs);
		Application->CreateForm(__classid(TFormTulkinta), &FormTulkinta);
		Application->CreateForm(__classid(TFormMTR), &FormMTR);
		Application->CreateForm(__classid(TFormEmiTag), &FormEmiTag);
		Application->CreateForm(__classid(TFormTapahtSiirrot), &FormTapahtSiirrot);
		Application->CreateForm(__classid(TFormKisaLuett), &FormKisaLuett);
		Application->CreateForm(__classid(TFormValitseKilp), &FormValitseKilp);
		Application->CreateForm(__classid(TFormHtmlEdit), &FormHtmlEdit);
		Application->CreateForm(__classid(TFormNappain), &FormNappain);
		Application->CreateForm(__classid(TFormEmitKoodit), &FormEmitKoodit);
		Application->CreateForm(__classid(TFormVaiheMaar), &FormVaiheMaar);
		Application->CreateForm(__classid(TFormKartta), &FormKartta);
		Application->CreateForm(__classid(TFormLisaTekstit), &FormLisaTekstit);
		Application->CreateForm(__classid(TFormFIS), &FormFIS);
		Application->CreateForm(__classid(TFormPiikit), &FormPiikit);
		Application->CreateForm(__classid(TFormSarjaVal), &FormSarjaVal);
		Application->CreateForm(__classid(TFormSHLtul), &FormSHLtul);
		Application->CreateForm(__classid(TFormVaToisto), &FormVaToisto);
		Application->CreateForm(__classid(TFormSprintSarja), &FormSprintSarja);
		Application->CreateForm(__classid(TFormKaavaTaul), &FormKaavaTaul);
		Application->CreateForm(__classid(TFormSivuTuloste), &FormSivuTuloste);
		Application->CreateForm(__classid(TFormEmithaku), &FormEmithaku);
		Application->CreateForm(__classid(TFormSakkoKierr), &FormSakkoKierr);
		Application->CreateForm(__classid(TFormLisaTietue), &FormLisaTietue);
		Application->CreateForm(__classid(TFormEmitFile), &FormEmitFile);
		Application->CreateForm(__classid(TFormEmit_VA), &FormEmit_VA);
		Application->CreateForm(__classid(TFormAikakorjaus), &FormAikakorjaus);
		Application->CreateForm(__classid(TFormKohdistus), &FormKohdistus);
		Application->CreateForm(__classid(TFormYhteysStatus), &FormYhteysStatus);
		Application->CreateForm(__classid(TFormJakelu), &FormJakelu);
		Application->CreateForm(__classid(TFormCfg), &FormCfg);
*/
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
