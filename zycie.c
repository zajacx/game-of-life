/**
* "Game of life" - trzecie zadanie zaliczeniowe ze Wstępu do programowania.
*
* Program symuluje "Grę w życie" w następujący sposób:
* 1. Czyta początkową generację żywych komórek.
* 2. W pętli:
*	a) Wypisuje aktualny fragment planszy (okno), o WIERSZE wierszach i KOLUMNY
*	   kolumnach. Współrzędne lewego górnego rogu okna są początkowo ustawione na
*	   wiersz 1. i kolumnę 1.
*	b) Czyta i wykonuje polecenie użytkownika:
*	   . - kończy pracę programu;
*	   \n (enter) - oblicza następną generację;
*	   N (liczba naturalna dodatnia) - oblicza N-tą następną generację;
*	   0 - wypisuje stan aktualnej generacji w takim samym formacie,
*	   w jakim był wczytany przez program opis generacji początkowej;
*	   W K (dwie liczby całkowite rozdzielone spacją) - ustawia współrzędne
*	   lewego górnego rogu okna w wierszu o numerze W i w kolumnie o numerze K.
*
* Ograniczenia:
* Program działa tylko dla poprawnych danych:
* - numery wszystkich wierszy i żywych komórek muszą zawierać się w przedziale
*   [INT_MIN, INT_MAX],
* - wprowadzana liczba N oznaczająca obliczenie N-tej kolejnej generacji musi być
*   liczbą naturalną dodatnią,
* - wprowadzane współrzędne okna muszą być liczbami całkowitymi oraz wszystkie
*   współrzędne komórek w oknie muszą zawierać się w przedziale [INT_MIN, INT_MAX].
*
*
* Uwagi:
* - Jeśli w opisie funkcji mowa o "sąsiadach" komórki, mamy na myśli 8 komórek
*   otaczających aktualnie rozważaną. Dodatkowo - boczni sąsiedzi to dwie komórki
*   sąsiadujące z rozważaną komórką w tym samym wierszu; górni i dolni sąsiedzi to
*   grupy trzech sąsiadujących ze sobą komórek w wierszach, odpowiednio, w - 1 i w + 1,
*   oraz w kolumnach k - 1, k, k + 1, gdzie w i k to współrzędne komórki, której
*   sąsiadów rozważamy.
*
* autor: Tomasz Zając <tz448580@students.mimuw.edu.pl>
* data: 10 stycznia 2023
*/

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
* Liczba wierszy okna.
*/
#ifndef WIERSZE
#define WIERSZE 22
#endif

/**
* Liczba kolumn okna.
*/
#ifndef KOLUMNY
#define KOLUMNY 80
#endif

/**
* Definicja struktury struct komorka - reprezentacji pojedynczej komorki.
*/
struct komorka {
    int w;                  //numer wiersza
    int k;                  //numer kolumny
    bool stanAkt;           //true, jeśli komórka jest żywa,
    bool stanNast;          //false, jeśli jest martwa
    struct komorka *prawy;  //wskaźnik na prawego sąsiada
    struct komorka *lewy;   //wskaźnik na lewego sąsiada
};

/**
* Definicja struktury struct wezel - reprezentacji pojedynczego węzła.
*/
struct wezel {
    int w;                        //numer wiersza, który zaczyna się od węzła
    struct komorka *zawartosc;    //wskaźnik na pierwszą żywą komórkę w wierszu
    struct wezel *gorny;          //wskaźnik na węzeł powyżej
    struct wezel *dolny;          //wskaźnik na węzeł poniżej
};

typedef struct komorka Tkomorka;
typedef struct wezel Twezel;

/**
* Tworzy nowy węzeł i zwraca wskaźnik na niego.
*/
Twezel *stworzNowyWezel(int w, Tkomorka *zawartosc, Twezel *gorny, Twezel *dolny) {
    Twezel *wynik = (Twezel*)malloc(sizeof(*wynik));
    assert(wynik != NULL);
    wynik -> w = w;
    wynik -> zawartosc = zawartosc;
    wynik -> gorny = gorny;
    wynik -> dolny = dolny;
    return wynik;
}

/**
* Tworzy nową komórkę i zwraca wskaźnik na nią.
*/
Tkomorka *stworzNowaKomorke(int k, bool stanAkt, bool stanNast, Tkomorka *lewy, Tkomorka *prawy) {
    Tkomorka *wynik = (Tkomorka*)malloc(sizeof(*wynik));
    assert(wynik != NULL);
    wynik -> k = k;
    wynik -> stanAkt = stanAkt;
    wynik -> stanNast = stanNast;
    wynik -> lewy = lewy;
    wynik -> prawy = prawy;
    return wynik;
}

/**
* Tworzy listę żywych komórek w wierszu i zwraca wskaźnik na pierwszą z nich.
*/
Tkomorka *stworzListeKomorek(void) {
    Tkomorka *atrapa = stworzNowaKomorke(0, false, false, NULL, NULL);
    Tkomorka *poprzednia = atrapa;
    int znak;
    while((znak = getchar()) != '\n') {
        ungetc(znak, stdin);
        int numerKolumny;
        scanf("%d", &numerKolumny);
        Tkomorka *aktualna = stworzNowaKomorke(numerKolumny, true, false, poprzednia, NULL);
        poprzednia -> prawy = aktualna;
        poprzednia = aktualna;
    }
    Tkomorka *poczKomorka = atrapa -> prawy;
    free(atrapa);
    return poczKomorka;
}

/**
* Czyta dane do struktury i zwraca wskaźnik na pierwszy węzeł.
*/
Twezel *czytaj(void) {
    Twezel *atrapa = stworzNowyWezel(0, NULL, NULL, NULL);
    Twezel *poprzedni = atrapa;
    bool czytajTabele = true;
    while(czytajTabele) {
        int slash = getchar();
        assert(slash == '/');
        int znak = getchar();
        if(znak == '\n') {
            czytajTabele = false;
        } else {
            ungetc(znak, stdin);
            int numerWiersza;
            scanf("%d", &numerWiersza);
            Twezel *aktualny = stworzNowyWezel(numerWiersza, NULL, poprzedni, NULL);
            poprzedni -> dolny = aktualny;
            Tkomorka *komorki = stworzListeKomorek();
            aktualny -> zawartosc = komorki;
            poprzedni = aktualny;
        }
    }
    if(poprzedni == atrapa) {
        return NULL;
    } else {
        Twezel *generacja = atrapa -> dolny;
        generacja -> gorny = NULL;
        free(atrapa);
        return generacja;
    }
}

/**
* Wstawia pusty węzeł nad aktualnym, jeśli numer powyższego jest różny od numeru aktualnego
* pomniejszonego o 1 (przypadek wstawiania węzła powyżej pierwszego w strukturze jest
* obsłużony w funkcji dopiszWezly(Twezel **generacja)).
*/
void wstawPustyWezelPowyzej(Twezel **aktualny) {

    if((*aktualny) -> gorny -> w != ((*aktualny) -> w) - 1) {
        Twezel *pierwotnieGorny = (*aktualny) -> gorny;
        Twezel *gorny = stworzNowyWezel((*aktualny) -> w - 1, NULL, pierwotnieGorny, (*aktualny));
        pierwotnieGorny -> dolny = gorny;
        (*aktualny) -> gorny = gorny;
    }
}

/**
* Wstawia pusty węzeł pod aktualnym, jesli taki nie istnieje lub numer poniższego jest różny
* od numeru aktualnego powiększonego o 1.
*/
void wstawPustyWezelPonizej(Twezel **aktualny) {

    if((*aktualny) -> dolny == NULL) {
        Twezel *dolny = stworzNowyWezel((*aktualny) -> w + 1, NULL, *aktualny, NULL);
        (*aktualny) -> dolny = dolny;
    } else if((*aktualny) -> dolny -> w != ((*aktualny) -> w) + 1) {
        Twezel *pierwotnieDolny = (*aktualny) -> dolny;
        Twezel *dolny = stworzNowyWezel((*aktualny) -> w + 1, NULL, *aktualny, pierwotnieDolny);
        pierwotnieDolny -> gorny = dolny;
        (*aktualny) -> dolny = dolny;
    }
}

/**
* Uzupelnia zapis generacji o puste węzły nad i pod każdym węzłem, w którym znajdują się
* listy żywych komórek.
*/
void dopiszWezly(Twezel **generacja) {

    Twezel *aktualny = *generacja;
    aktualny -> gorny = stworzNowyWezel(aktualny -> w - 1, NULL, NULL, aktualny);
    while(aktualny != NULL) {
        Twezel *ponizszy = aktualny -> dolny;
        wstawPustyWezelPowyzej(&aktualny);
        wstawPustyWezelPonizej(&aktualny);
        aktualny = ponizszy;
    }
}

/**
* Z lewej strony żywej komórki wstawia martwą, jeśli lewy sąsiad komórki nie istnieje lub
* gdy jego numer jest różny od numeru aktualnej komórki pomniejszonego o 1.
*/
void wstawKomorkePoLewej(Tkomorka **komorka) {

    if((*komorka) -> lewy == NULL) {
        (*komorka) -> lewy = stworzNowaKomorke((*komorka) -> k - 1, false, false, NULL, (*komorka));
    } else if((*komorka) -> lewy -> k != (*komorka) -> k - 1) {
        Tkomorka *pierwotnieLewy = (*komorka) -> lewy;
        Tkomorka *lewy = stworzNowaKomorke((*komorka) -> k - 1, false, false, pierwotnieLewy, (*komorka));
        pierwotnieLewy -> prawy = lewy;
        (*komorka) -> lewy = lewy;
    }
}

/**
* Z prawej strony żywej komórki wstawia martwą, jeśli prawy sąsiad komórki nie istnieje lub
* gdy jego numer jest różny od numeru aktualnej komórki powiększonego o 1.
*/
void wstawKomorkePoPrawej(Tkomorka **komorka) {

    if((*komorka) -> prawy == NULL) {
        (*komorka) -> prawy = stworzNowaKomorke((*komorka) -> k + 1, false, false, (*komorka), NULL);
    } else if((*komorka) -> prawy -> k != (*komorka) -> k + 1) {
        Tkomorka *pierwotniePrawy = (*komorka) -> prawy;
        Tkomorka *prawy = stworzNowaKomorke((*komorka) -> k + 1, false, false, (*komorka), pierwotniePrawy);
        pierwotniePrawy -> lewy = prawy;
        (*komorka) -> prawy = prawy;
    }
}

/**
* Dla wszystkich komórek w wierszu wykonuje uzupełnianie lewych i prawych sąsiadów
* z użyciem funkcji wstawKomorkePoLewej(Tkomorka **komorka) oraz
* wstawKomorkePoPrawej(Tkomorka ** komorka).
*/
void wypelnijWiersz(Twezel **wezel) {

    Twezel *aktualny = *wezel;
    Tkomorka *aktualna = aktualny -> zawartosc;
    aktualna -> lewy = stworzNowaKomorke((aktualna -> k) - 1, false, false, NULL, aktualna);
    aktualny -> zawartosc = aktualna -> lewy;
    Tkomorka *pierwotniePrawa = aktualna -> prawy;
    wstawKomorkePoPrawej(&aktualna);
    aktualna = pierwotniePrawa;
    while(aktualna != NULL) {
        Tkomorka *nastepna = aktualna -> prawy;
        wstawKomorkePoLewej(&aktualna);
        wstawKomorkePoPrawej(&aktualna);
        aktualna = nastepna;
    }
}

/**
* W niepustych wierszach wykonuje uzupełnianie lewych i prawych sąsiadów z użyciem
* void WypelnijWiersz(Twezel **wezel).
*/
void wypelnijNiepusteWiersze(Twezel **generacja) {

    Twezel *aktualny = *generacja;

    while(aktualny != NULL) {
        if(aktualny -> zawartosc != NULL) {
            wypelnijWiersz(&aktualny);
        }
        aktualny = aktualny -> dolny;
    }
}

/**
* Określa, czy w wierszu jest jakaś żywa komórka.
*/
bool istniejeJakasZywa(Twezel *wezel) {
    if(wezel -> zawartosc == NULL) {
        return false;
    } else {
        Tkomorka *aktualna = wezel -> zawartosc;
        bool istnieje = false;
        while(!istnieje && aktualna != NULL) {
            if(aktualna -> stanAkt == true) {
                istnieje = true;
            }
            aktualna = aktualna -> prawy;
        }
        return istnieje;
    }
}

/**
* Okresla, czy w kolejnej generacji w wierszu będzie jakaś żywa komórka.
*/
bool bedzieJakasZywa(Twezel *wezel) {
    if(wezel -> zawartosc == NULL) {
        return false;
    } else {
        Tkomorka *aktualna = wezel -> zawartosc;
        bool istnieje = false;
        while(!istnieje && aktualna != NULL) {
            if(aktualna -> stanNast == true) {
                istnieje = true;
            }
            aktualna = aktualna -> prawy;
        }
        return istnieje;
    }
}

/**
* Tworzy martwą komórkę za komórką wskazaną przez (*komorka).
*/
void wstawZa(Tkomorka **komorka, int k) {

    assert((*komorka) != NULL);
    Tkomorka *pom = stworzNowaKomorke(k, false, false, (*komorka), NULL);
    if((*komorka) -> prawy != NULL) {
        Tkomorka *pierwotniePrawy = (*komorka) -> prawy;
        pom -> prawy = pierwotniePrawy;
        pierwotniePrawy -> lewy = pom;
    }
    (*komorka) -> prawy = pom;
}

/**
* Tworzy w posortowanym wierszu i w kolumnie int k martwą komórkę
* i wstawia ją w odpowiednie miejsce.
*/
void wstawDoPosortowanej(Tkomorka **akt, int k) {

    Tkomorka *aktualna = *akt;
    Tkomorka *nastepna = aktualna -> prawy;

    if(nastepna == NULL) {
        wstawZa(&aktualna, k);
    } else {
        while(nastepna -> k < k && nastepna -> prawy != NULL) {
            aktualna = nastepna;
            nastepna = nastepna -> prawy;
        }
        if(nastepna -> k < k) {
            wstawZa(&nastepna, k);
        } else {
            if(aktualna -> k != k && nastepna -> k != k) {
                wstawZa(&aktualna, k);
            }
        }
    }
}

/**
* Tworzy górnych lub dolnych sąsiadów wszystkich komórek w wierszu.
* Twezel **sasiad to wskaźnik na wskaźnik na wiersz, który chcemy aktualizować -
* górny lub dolny.
*/
void dodajSasiadow(Twezel *akt, Twezel **sasiad) {

    Twezel *aktualny = akt;
    Twezel *sasiedni = *sasiad;
    Tkomorka *aktualna = aktualny -> zawartosc;
    if(sasiedni -> zawartosc == NULL) {
        sasiedni -> zawartosc = stworzNowaKomorke(INT_MIN, false, false, NULL, NULL); //atrapa
    }
    Tkomorka *sasiednia = sasiedni -> zawartosc;

    while(aktualna != NULL) {
        if(aktualna -> stanAkt == true) {
            wstawDoPosortowanej(&sasiednia, aktualna -> k - 1);
            wstawDoPosortowanej(&sasiednia, aktualna -> k);
            wstawDoPosortowanej(&sasiednia, aktualna -> k + 1);
        }
        aktualna = aktualna -> prawy;
    }
    if(sasiedni -> zawartosc -> k == INT_MIN) {
        Tkomorka *atrapa = sasiedni -> zawartosc;
        sasiedni -> zawartosc = atrapa -> prawy;
        sasiedni -> zawartosc -> lewy = NULL;
        free(atrapa);
    }
}

/**
* Tworzy górnych i dolnych sąsiadów każdej żywej komórki na planszy z użyciem
* void dodajSasiadow(Twezel *akt, Twezel **sasiad).
*/
void otoczGoraDol(Twezel **generacja) {

    Twezel *gorny = (*generacja);
    Twezel *aktualny = (*generacja) -> dolny;
    Twezel *dolny = (*generacja) -> dolny -> dolny;

    while(dolny != NULL) {
        if(istniejeJakasZywa(aktualny)) {
            dodajSasiadow(aktualny, &gorny);
            dodajSasiadow(aktualny, &dolny);
        }
        dolny = dolny -> dolny;
        aktualny = aktualny -> dolny;
        gorny = gorny -> dolny;
    }
}

/**
* Usuwa ewentualne źle wygenerowane martwe komórki z początków wierszy.
*/
void popraw(Twezel **generacja) {
    Twezel *aktualny = *generacja;
    while(aktualny != NULL) {
        Tkomorka *komorka = aktualny -> zawartosc;
        if(komorka -> k >= komorka -> prawy -> k) {
            aktualny -> zawartosc = komorka -> prawy;
            aktualny -> zawartosc -> lewy = NULL;
            free(komorka);
        }
        aktualny = aktualny -> dolny;
    }
}

/**
* Ustawia stan komórki w następnej generacji na true.
* Domyślnie stanNast = false, wiec analogiczna funkcja do
* uśmiercania komórki nie jest potrzebna.
*/
void ozywKomorke(Tkomorka **komorka) {
    (*komorka) -> stanNast = true;
}

/**
* Zlicza żywych sąsiadów bocznych wskazanej komórki.
*/
int policzBocznychSasiadow(Tkomorka *komorka) {
    int sasiedzi = 0;
    if(komorka -> lewy != NULL) {
        if(komorka -> lewy -> k == (komorka -> k) - 1 && komorka -> lewy -> stanAkt == true) {
            sasiedzi++;
        }
    }
    if(komorka -> prawy != NULL) {
        if(komorka -> prawy -> k == (komorka -> k) + 1 && komorka -> prawy -> stanAkt == true) {
            sasiedzi++;
        }
    }
    return sasiedzi;
}

/**
* Zlicza sąsiadów komórki Tkomorka *akt z wiersza powyżej/poniżej
* (komórka oznaczona przez dol może oznaczać również górną).
*/
int policzSasiadow(Tkomorka *akt, Tkomorka **dol) {

    Tkomorka *aktualna = akt;
    Tkomorka *dolna = *dol;
    int sasiedzi = 0;
    while(dolna -> k < aktualna -> k && dolna -> prawy != NULL) {
        dolna = dolna -> prawy;
    }
    if(dolna -> k == aktualna -> k) {
        if(dolna -> stanAkt == true) {
            sasiedzi++;
        }
        if(dolna -> lewy != NULL ) {
            if(dolna -> lewy -> k == (dolna -> k) - 1 && dolna -> lewy -> stanAkt == true) {
                sasiedzi++;
            }
        }
        if(dolna -> prawy != NULL) {
            if(dolna -> prawy -> k == (dolna -> k) + 1 && dolna -> prawy -> stanAkt == true) {
                sasiedzi++;
            }
        }
        dolna = dolna -> lewy;

    } else if(dolna -> k > aktualna -> k) {
        if(dolna -> k == (aktualna -> k) + 1 && dolna -> stanAkt == true) {
            sasiedzi++;
        }
        if(dolna -> lewy != NULL) {
            dolna = dolna -> lewy;
            if(dolna -> k == (aktualna -> k) - 1 && dolna -> stanAkt == true) {
                sasiedzi++;
            }
        }
    } else if(dolna -> k < aktualna -> k) {
        if(dolna -> k == (aktualna -> k) - 1 && dolna -> stanAkt == true) {
            sasiedzi++;
        }
    }
    return sasiedzi;
}

/**
* Oblicza stany wszystkich komórek w pierwszym wierszu w następnej generacji.
*/
void obliczPierwszyWiersz(Twezel **pierwszyWezel) {

    Twezel *aktualny = *pierwszyWezel;
    Twezel *dolny = (*pierwszyWezel) -> dolny;

    Tkomorka *aktualna = aktualny -> zawartosc;
    Tkomorka *dolna = dolny -> zawartosc;

    while(aktualna != NULL) {
        int sasiedzi = 0;
        sasiedzi += policzBocznychSasiadow(aktualna);
        sasiedzi += policzSasiadow(aktualna, &dolna);
        if((aktualna -> stanAkt == true && sasiedzi == 2) ||
        (aktualna -> stanAkt == true && sasiedzi == 3) ||
        (aktualna -> stanAkt == false && sasiedzi == 3)) {
            ozywKomorke(&aktualna);
        }
        aktualna = aktualna -> prawy;
    }
}

/**
* Oblicza stany wszystkich komórek w ostatnim wierszu w następnej generacji.
*/
void obliczOstatniWiersz(Twezel **ostatniWezel) {

    Twezel *aktualny = *ostatniWezel;
    Twezel *gorny = (*ostatniWezel) -> gorny;

    Tkomorka *aktualna = aktualny -> zawartosc;
    Tkomorka *gorna = gorny -> zawartosc;

    while(aktualna != NULL) {
        int sasiedzi = 0;
        sasiedzi += policzBocznychSasiadow(aktualna);
        sasiedzi += policzSasiadow(aktualna, &gorna);
        if((aktualna -> stanAkt == true && sasiedzi == 2) ||
        (aktualna -> stanAkt == true && sasiedzi == 3) ||
        (aktualna -> stanAkt == false && sasiedzi == 3)) {
            ozywKomorke(&aktualna);
        }
        aktualna = aktualna -> prawy;
    }
}

/**
* Oblicza stany wszystkich komórek w następnej generacji w wierszu,
* dla którego istnieje wiersz poprzedni i nastepny.
*/
void obliczSrodkowyWiersz(Twezel **srodkowyWezel) {

    Twezel *aktualny = *srodkowyWezel;
    Twezel *gorny = (*srodkowyWezel) -> gorny;
    Twezel *dolny = (*srodkowyWezel) -> dolny;

    Tkomorka *aktualna = aktualny -> zawartosc;
    Tkomorka *gorna = gorny -> zawartosc;
    Tkomorka *dolna = dolny -> zawartosc;

    while(aktualna != NULL) {
        int sasiedzi = 0;
        sasiedzi += policzBocznychSasiadow(aktualna);
        sasiedzi += policzSasiadow(aktualna, &gorna);
        sasiedzi += policzSasiadow(aktualna, &dolna);
        if((aktualna -> stanAkt == true && sasiedzi == 2) ||
        (aktualna -> stanAkt == true && sasiedzi == 3) ||
        (aktualna -> stanAkt == false && sasiedzi == 3)) {
            ozywKomorke(&aktualna);
        }
        aktualna = aktualna -> prawy;
    }
}

/**
* Oblicza stan wszystkich komórek na planszy w następnej generacji.
*/
void obliczNastepneStany(Twezel **generacja) {

    Twezel *aktualny = *generacja;

    obliczPierwszyWiersz(&aktualny);
    aktualny = aktualny -> dolny;
    while(aktualny -> dolny != NULL) {
        obliczSrodkowyWiersz(&aktualny);
        aktualny = aktualny -> dolny;
    }
    obliczOstatniWiersz(&aktualny);
}

/**
* Usuwa z wiersza wszystkie komórki, które w następnej generacji będą martwe.
*/
void usunMartweWWierszu(Twezel **wezel) {
    Twezel *aktualny = *wezel;
    Tkomorka *aktualna = (*wezel) -> zawartosc;
    while(aktualna != NULL) {
        if(aktualna -> stanNast == false) {
            if((aktualna -> lewy == NULL) && (aktualna -> prawy != NULL)) {
                Tkomorka *prawa = aktualna -> prawy;
                prawa -> lewy = NULL;
                aktualny -> zawartosc = prawa;
                free(aktualna);
                aktualna = prawa;
            } else if((aktualna -> lewy != NULL) && (aktualna -> prawy == NULL)) {
                Tkomorka *lewa = aktualna -> lewy;
                lewa -> prawy = NULL;
                free(aktualna);
                aktualna = NULL;
            } else if((aktualna -> lewy != NULL) && (aktualna -> prawy != NULL)) {
                Tkomorka *lewa = aktualna -> lewy;
                Tkomorka *prawa = aktualna -> prawy;
                prawa -> lewy = aktualna -> lewy;
                lewa -> prawy = aktualna -> prawy;
                free(aktualna);
                aktualna = prawa;
            } else {
                aktualny -> zawartosc = NULL;
                free(aktualna);
                aktualna = aktualny -> zawartosc;
            }
        } else {
            aktualna = aktualna -> prawy;
        }
    }
}

/**
* Usuwa wszystkie komórki z planszy, które będą martwe w następnej generacji,
* z użyciem void usunMartweWWierszu(Twezel **wezel).
*/
void usunMartwe(Twezel **generacja) {

    Twezel *aktualny = *generacja;
    while(aktualny != NULL) {
        usunMartweWWierszu(&aktualny);
        aktualny = aktualny -> dolny;
    }
}

/**
* Po usuwaniu martwych komórek z całej planszy zmienia aktualny stan wszystkich
* żywych komórek na true, a ich stan następny - domyślnie - na false.
*/
void aktualizujStan(Twezel **generacja) {
    Twezel *aktualny = *generacja;
    while(aktualny != NULL) {
        if(aktualny -> zawartosc != NULL) {
            Tkomorka *aktualna = aktualny -> zawartosc;
            while(aktualna != NULL) {
                aktualna -> stanAkt = true;
                aktualna -> stanNast = false;
                aktualna = aktualna -> prawy;
            }
        }
        aktualny = aktualny -> dolny;
    }
}

/**
* Usuwa puste wiersze z planszy.
*/
void usunPusteWezly(Twezel **generacja) {
    Twezel *aktualny = *generacja;
    while(aktualny != NULL) {
        if(aktualny -> zawartosc == NULL) {
            if(aktualny -> dolny == NULL && aktualny -> gorny == NULL) {
                *generacja = NULL;
                free(aktualny);
            } else if(aktualny -> gorny == NULL && aktualny -> dolny != NULL) {
                Twezel *dolny = aktualny -> dolny;
                dolny -> gorny = NULL;
                *generacja = dolny;
                free(aktualny);
                aktualny = dolny;
            } else if(aktualny -> gorny != NULL && aktualny -> dolny == NULL) {
                Twezel *gorny = aktualny -> gorny;
                gorny -> dolny = NULL;
                free(aktualny);
                aktualny = NULL;
            } else { /* aktualny -> gorny != NULL && aktualny -> dolny != NULL */
                Twezel *gorny = aktualny -> gorny;
                Twezel *dolny = aktualny -> dolny;
                gorny -> dolny = dolny;
                dolny -> gorny = gorny;
                free(aktualny);
                aktualny = dolny;
            }
        } else {
            aktualny = aktualny -> dolny;
        }
    }
}

/**
* Wypisuje aktualną generację żywych komórek w takim samym formacie, w jakim
* została wczytana generacja początkowa.
*/
void piszGeneracje(Twezel *generacja) {
    Twezel *w = generacja;
    while(w != NULL) {
        printf("/%d", w -> w);
        Tkomorka *k = w -> zawartosc;
        while(k != NULL) {
            printf(" %d", k -> k);
            k = k -> prawy;
        }
        printf("\n");
        w = w -> dolny;
    }
    printf("/\n");
}


/**
* Wypisuje wiersz złożony z samych kropek.
*/
void piszWierszKropek(void) {
    for(int i = 0; i < KOLUMNY; i++) {
        printf(".");
    }
    printf("\n");
}

/**
* Wypisuje wiersz końcowy, złożony ze znaków równości.
*/
void piszWierszKoncowy(void) {
    for(int i = 0; i < KOLUMNY; i++) {
        printf("=");
    }
    printf("\n");
}

/**
* Wypisuje okno złożone z samych kropek.
*/
void piszPusteOkno(void) {
    for(int i = 0; i < WIERSZE; i++) {
        piszWierszKropek();
    }
    piszWierszKoncowy();
}

/**
* Pisze wiersz, w ktorym są jakieś żywe komórki, zaczynąjac od kolumny k.
*/
void piszWiersz(Twezel *wezel, int k) {

    Twezel *aktualny = wezel;
    Tkomorka *aktualna = aktualny -> zawartosc;
    int kGen = aktualna -> k;
    if(kGen < k) {
        while(aktualna -> k < k && aktualna -> prawy != NULL) {
            aktualna = aktualna -> prawy;
        }
        kGen = aktualna -> k;
    }
    bool saKolumny = true;
    for(int i = k; i < k + KOLUMNY; i++) {
        if(saKolumny && i == kGen) {
            printf("0");
            if(aktualna -> prawy != NULL) {
                aktualna = aktualna -> prawy;
                kGen = aktualna -> k;
            } else {
                saKolumny = false;
            }
        } else {
            printf(".");
        }
    }
    printf("\n");
}

/**
* Pisze okno - komórki, ktore maja numer wiersza od w do w + WIERSZE - 1
* i numer kolumny od k do k + KOLUMNY - 1.
*/
void piszOkno(Twezel *generacja, int w, int k) {
    Twezel *aktualny = generacja;
    if(aktualny == NULL) {
        piszPusteOkno();
    } else {
        int wGen = aktualny -> w;
        if(wGen < w) {
            while(aktualny -> w < w && aktualny -> dolny != NULL) {
                aktualny = aktualny -> dolny;
            }
            wGen = aktualny -> w;
        }
        bool saWiersze = true;
        for(int i = w; i < w + WIERSZE; i++) {
            if(saWiersze && i == wGen) {
                piszWiersz(aktualny, k);
                if(aktualny -> dolny != NULL) {
                    aktualny = aktualny -> dolny;
                    wGen = aktualny -> w;
                } else {
                    saWiersze = false;
                }
            } else {
                piszWierszKropek();
            }
        }
        piszWierszKoncowy();
    }
}

/**
* Sprawdza, czy wszystkie komórki na planszy są martwe.
*/
bool czyWszystkieMartwe(Twezel *generacja) {
    Twezel *aktualny = generacja;
    if(aktualny == NULL) {
        return true;
    } else {
        while(aktualny != NULL) {
            if(aktualny -> zawartosc != NULL) {
                return false;
            } else {
                aktualny = aktualny -> dolny;
            }
        }
        return true;
    }
}

/**
* Usuwa całą generację i zwalnia pamięć.
*/
void wyczyscGeneracje(Twezel **generacja) {
    Twezel *aktualny = *generacja;
    while(aktualny != NULL) {
        Twezel *nastepny = aktualny -> dolny;
        if(aktualny -> zawartosc != NULL) {
            Tkomorka *aktualna = aktualny -> zawartosc;
            while(aktualna != NULL) {
                Tkomorka *nastepna = aktualna -> prawy;
                free(aktualna);
                aktualna = nastepna;
            }
        }
        free(aktualny);
        aktualny = nastepny;
    }
}

/**
* W pętli: wypisuje planszę i wykonuje polecenia użytkownika.
*
* Z góry przepraszam za objętość tej funkcji! Wszystkie operacje obliczające
* kolejną generację miały być zebrane w oddzielną funkcję, ale pojawiał się problem
* ze wskaźnikiem na całą strukturę, co zmusiło mnie do wstawienia wszystkich
* operacji do poniższej, ogólniejszej funkcji.
*/

void wykonuj(Twezel **wczytany) {

    Twezel *generacja = *wczytany;

    int lgrWiersz = 1;
    int lgrKolumna = 1;

    while(true) {
        piszOkno(generacja, lgrWiersz, lgrKolumna);
        int x = getchar();
        if(x == '.') {
            wyczyscGeneracje(&generacja);
            break;
        } else if(x == '\n' || x == EOF) {
            if(generacja != NULL) {
                dopiszWezly(&generacja);
                generacja = generacja -> gorny;
                wypelnijNiepusteWiersze(&generacja);
                otoczGoraDol(&generacja);
                popraw(&generacja);
                obliczNastepneStany(&generacja);
                usunMartwe(&generacja);
                if(czyWszystkieMartwe(generacja)) {
                    wyczyscGeneracje(&generacja);
                    generacja = NULL;
                } else {
                    while(generacja -> zawartosc == NULL) {
                        Twezel *aktualny = generacja;
                        generacja = generacja -> dolny;
                        free(aktualny);
                        generacja -> gorny = NULL;
                    }
                    aktualizujStan(&generacja);
                    usunPusteWezly(&generacja);
                }
            }
        } else {
            ungetc(x, stdin);
            int y;
            scanf("%d", &y);
            x = getchar();
            if(x == ' ') {
                lgrWiersz = y;
                scanf("%d", &y);
                lgrKolumna = y;
                x = getchar();
            } else {
                if(y == 0) {
                    piszGeneracje(generacja);
                } else {
                    for(int i = 0; i < y; i++) {
                        if(generacja != NULL) {
                            dopiszWezly(&generacja);
                            generacja = generacja -> gorny;
                            wypelnijNiepusteWiersze(&generacja);
                            otoczGoraDol(&generacja);
                            popraw(&generacja);
                            obliczNastepneStany(&generacja);
                            usunMartwe(&generacja);
                            if(czyWszystkieMartwe(generacja)) {
                                wyczyscGeneracje(&generacja);
                                generacja = NULL;
                            } else {
                                while(generacja -> zawartosc == NULL) {
                                    Twezel *aktualny = generacja;
                                    generacja = generacja -> dolny;
                                    free(aktualny);
                                    generacja -> gorny = NULL;
                                }
                                aktualizujStan(&generacja);
                                usunPusteWezly(&generacja);
                            }
                        }
                    }
                }
            }
        }
    }
}


int main(void) {

    Twezel *wczytany = czytaj();
    wykonuj(&wczytany);

    return 0;
}
