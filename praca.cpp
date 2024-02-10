#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

class Student {
private:
    string nrid;  
    string imie;
    string nazwisko;
    string klasa;
    bool maKsiazki;

public:
    Student(const string& n, const string& im, const string& naz, const string& k, bool maK)
        : nrid(n), imie(im), nazwisko(naz), klasa(k), maKsiazki(maK) {}

    string getNrid() const { return nrid; }
    string getImie() const { return imie; }
    string getNazwisko() const { return nazwisko; }
    string getKlasa() const { return klasa; }
    bool czyMaKsiazki() const { return maKsiazki; }

    void wyswietl() const {
        cout << setw(10) << nrid << setw(15) << imie << setw(15) << nazwisko << setw(10) << klasa;
        cout << setw(15) << (maKsiazki ? "Tak" : "Nie") << endl;
    }
};

class BazaDanych {
private:
    vector<Student> studenci;
    string nazwaPliku;

public:
    BazaDanych(const string& nazwaPliku) : nazwaPliku(nazwaPliku) {
        wczytajZPliku();
    }

    void wczytajZPliku();
    void wylistujStudentow() const;
    void dodajStudenta();
    void usunStudenta();
    void zapiszDoPliku() const;
    bool czyNridUnikatowy(const string& nrid) const;
};

void BazaDanych::wczytajZPliku() {
    ifstream plik(nazwaPliku);
    if (plik.is_open()) {
        string nrid, imie, nazwisko, klasa;
        bool maKsiazki;
        while (plik >> nrid >> imie >> nazwisko >> klasa >> maKsiazki) {
            Student student(nrid, imie, nazwisko, klasa, maKsiazki);
            studenci.push_back(student);
        }
        plik.close();
        cout << "Dane studentow zostaly wczytane z pliku." << endl;
    }
    else {
        cout << "Nie udalo sie otworzyc pliku. Tworzenie nowej bazy danych." << endl;
    }
}

void BazaDanych::wylistujStudentow() const {
    cout << setw(10) << "Nrid" << setw(15) << "Imie" << setw(15) << "Nazwisko";
    cout << setw(10) << "Klasa" << setw(15) << "Czy ma ksiazki" << endl;
    cout << "----------------------------------------------------------" << endl;
    for (const auto& student : studenci) {
        student.wyswietl();
    }
}

void BazaDanych::dodajStudenta() {
    string nrid, imie, nazwisko, klasa;
    bool maKsiazki;

    do {
        cout << "Podaj unikatowy numer identyfikacyjny (Nrid): ";
        cin >> nrid;
    } while (!czyNridUnikatowy(nrid));

    cout << "Podaj imie: ";
    cin >> imie;
    cout << "Podaj nazwisko: ";
    cin >> nazwisko;
    cout << "Podaj klase: ";
    cin >> klasa;
    cout << "Czy ma ksiazki (1 - Tak, 0 - Nie): ";
    cin >> maKsiazki;

    Student student(nrid, imie, nazwisko, klasa, maKsiazki);
    studenci.push_back(student);

    cout << "Dodano nowego studenta." << endl;
}

void BazaDanych::usunStudenta() {
    string nrid;
    cout << "Podaj numer identyfikacyjny (Nrid) studenta do usuniecia: ";
    cin >> nrid;

    auto it = remove_if(studenci.begin(), studenci.end(),
        [nrid](const Student& s) { return s.getNrid() == nrid; });

    if (it != studenci.end()) {
        studenci.erase(it, studenci.end());
        cout << "Usunieto studenta o numerze identyfikacyjnym NrID: " << nrid << endl;
    }
    else {
        cout << "Nie znaleziono studenta o numerze identyfikacyjnym Nrid: " << nrid << endl;
    }
}

void BazaDanych::zapiszDoPliku() const {
    ofstream plik(nazwaPliku);
    if (plik.is_open()) {
        for (const auto& student : studenci) {
            plik << student.getNrid() << " " << student.getImie() << " " << student.getNazwisko() << " " << student.getKlasa() << " " << student.czyMaKsiazki() << endl;
        }
        plik.close();
        cout << "Baza danych zostala zapisana do pliku." << endl;
    }
    else {
        cout << "Nie udalo sie otworzyc pliku do zapisu." << endl;
    }
}

bool BazaDanych::czyNridUnikatowy(const string& nrid) const {
    return find_if(studenci.begin(), studenci.end(), [nrid](const Student& s) { return s.getNrid() == nrid; }) == studenci.end();
}

int main() {
    BazaDanych baza("Baza.txt");

    char wybor;
    while (true) {
        cout << "Wybierz opcje:" << endl;
        cout << "1. Wylistuj studentow" << endl;
        cout << "2. Dodaj nowego studenta" << endl;
        cout << "3. Usun studenta" << endl;
        cout << "4. Zakoncz program i zapisz baze danych" << endl;
        cin >> wybor;

        switch (wybor) {
        case '1':
            baza.wylistujStudentow();
            break;
        case '2':
            baza.dodajStudenta();
            break;
        case '3':
            baza.usunStudenta();
            break;
        case '4':
            baza.zapiszDoPliku();
            return 0;
        default:
            cout << "Nieznana opcja. Sprobuj ponownie." << endl;
        }
    }

    return 0;
}
