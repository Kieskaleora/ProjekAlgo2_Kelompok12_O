/*
 * ============================================================
 *   SISTEM LAUNDRY SEDERHANA
 *   Proyek Akhir Algoritma Pemrograman Lanjut
 * ============================================================
 *   Materi yang diimplementasikan:
 *   - Array        : Menyimpan seluruh data transaksi
 *   - Struct       : Data pelanggan & laundry
 *   - Searching    : Cari berdasarkan ID, Nama, atau Status
 *   - Sorting      : Urutkan berdasarkan Nama, Berat, atau Harga
 *   - Pointer      : Beri diskon / ubah data
 *   - Recursive    : Hitung total pendapatan
 *   - File I/O     : Simpan & muat data dari file
 * ============================================================
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

// ============================================================
//  STRUCT & KONSTANTA
// ============================================================

struct Laundry {
    string id;
    string nama;
    float  berat;
    string jenisLayanan;
    int    hargaTotal;
    string status;
};

const int MAX_DATA   = 100;
const string NAMAFILE = "data_laundry.txt";

Laundry data[MAX_DATA];
int jumlahData = 0;

// ============================================================
//  UTILITAS
// ============================================================

void garsPemisah(char c = '-', int n = 60) {
    for (int i = 0; i < n; i++) cout << c;
    cout << endl;
}

string toUpperStr(string s) {
    for (char& c : s) c = toupper(c);
    return s;
}

bool idAda(const string& id) {
    for (int i = 0; i < jumlahData; i++)
        if (data[i].id == id) return true;
    return false;
}

string generateID() {
    return "LDR" + to_string(jumlahData + 1);
}

// ============================================================
//  HITUNG HARGA
// ============================================================

int hitungHarga(const string& jenis, float berat) {
    int hargaPerKg = 0;
    string j = toUpperStr(jenis);
    if      (j == "REGULER") hargaPerKg = 5000;
    else if (j == "EXPRESS") hargaPerKg = 10000;
    else if (j == "KILAT")   hargaPerKg = 15000;
    else                     hargaPerKg = 5000;   // default
    return (int)(berat * hargaPerKg);
}

// ============================================================
//  1. TAMBAH TRANSAKSI
// ============================================================

void tambahTransaksi() {
    if (jumlahData >= MAX_DATA) {
        cout << "[!] Data penuh! Kapasitas maksimal " << MAX_DATA << " transaksi.\n";
        return;
    }

    Laundry baru;
    baru.id     = generateID();
    baru.status = "Menunggu";

    cout << "\n[+] ID otomatis: " << baru.id << endl;
    cout << "    Nama Pelanggan : "; cin.ignore(); getline(cin, baru.nama);
    cout << "    Berat (kg)     : "; cin >> baru.berat;
    cout << "    Jenis Layanan  \n";
    cout << "    (1) Reguler Rp5.000/kg\n";
    cout << "    (2) Express Rp10.000/kg\n";
    cout << "    (3) Kilat  Rp15.000/kg\n";
    cout << "    Pilihan        : ";
    int pil; cin >> pil;
    switch (pil) {
        case 1: baru.jenisLayanan = "Reguler"; break;
        case 2: baru.jenisLayanan = "Express"; break;
        case 3: baru.jenisLayanan = "Kilat";   break;
        default: baru.jenisLayanan = "Reguler";
    }

    baru.hargaTotal = hitungHarga(baru.jenisLayanan, baru.berat);

    data[jumlahData++] = baru;

    cout << "\n[OK] Transaksi berhasil ditambahkan!\n";
    cout << "     Total Harga : Rp" << baru.hargaTotal << "\n";
}

// ============================================================
//  CETAK SATU BARIS DATA (TABEL)
// ============================================================

void headerTabel() {
    cout << left
         << setw(8)  << "ID"
         << setw(20) << "Nama"
         << setw(8)  << "Berat"
         << setw(10) << "Layanan"
         << setw(12) << "Harga"
         << setw(14) << "Status"
         << endl;
    garsPemisah('-');
}

void cetakBaris(const Laundry& d) {
    cout << left
         << setw(8)  << d.id
         << setw(20) << d.nama
         << setw(8)  << fixed << setprecision(1) << d.berat
         << setw(10) << d.jenisLayanan
         << setw(12) << ("Rp" + to_string(d.hargaTotal))
         << setw(14) << d.status
         << endl;
}

// ============================================================
//  2. TAMPILKAN SELURUH DATA
// ============================================================

void tampilkanData() {
    if (jumlahData == 0) {
        cout << "[!] Belum ada data transaksi.\n"; return;
    }
    cout << "\n=== DAFTAR SELURUH TRANSAKSI (" << jumlahData << " data) ===\n";
    headerTabel();
    for (int i = 0; i < jumlahData; i++)
        cetakBaris(data[i]);
    garsPemisah('=');
}

// ============================================================
//  3. CARI DATA (SEARCHING)
// ============================================================

void cariData() {
    cout << "\n=== CARI DATA ===\n";
    cout << "Cari berdasarkan:\n";
    cout << "(1) ID\n(2) Nama\n(3) Status\n";
    cout << "Pilihan : "; int pil; cin >> pil;
    cout << "Kata kunci : "; string kunci; cin.ignore(); getline(cin, kunci);

    bool ditemukan = false;
    headerTabel();
    for (int i = 0; i < jumlahData; i++) {
        bool cocok = false;
        string k = toUpperStr(kunci);
        if (pil == 1 && toUpperStr(data[i].id)     == k) cocok = true;
        if (pil == 2 && toUpperStr(data[i].nama).find(k) != string::npos) cocok = true;
        if (pil == 3 && toUpperStr(data[i].status) == k) cocok = true;

        if (cocok) { cetakBaris(data[i]); ditemukan = true; }
    }
    if (!ditemukan) cout << "[!] Data tidak ditemukan.\n";
}

// ============================================================
//  4. CEK STATUS LAUNDRY
// ============================================================

void cekStatus() {
    cout << "\nMasukkan ID pelanggan : "; string id; cin >> id;
    for (int i = 0; i < jumlahData; i++) {
        if (data[i].id == id) {
            cout << "\n--- Info Transaksi ---\n";
            cetakBaris(data[i]);
            return;
        }
    }
    cout << "[!] ID tidak ditemukan.\n";
}

// ============================================================
//  5. EDIT DATA TRANSAKSI
// ============================================================

void editData() {
    cout << "\nMasukkan ID yang ingin diedit : "; string id; cin >> id;
    for (int i = 0; i < jumlahData; i++) {
        if (data[i].id == id) {
            cout << "\nData saat ini:\n"; cetakBaris(data[i]);
            cout << "\nNama baru (kosongkan = tidak berubah) : ";
            cin.ignore(); string tmp; getline(cin, tmp);
            if (!tmp.empty()) data[i].nama = tmp;

            cout << "Berat baru (0 = tidak berubah)        : ";
            float b; cin >> b;
            if (b > 0) data[i].berat = b;

            cout << "Jenis Layanan baru (kosong = tetap) [Reguler/Express/Kilat] : ";
            cin.ignore(); getline(cin, tmp);
            if (!tmp.empty()) data[i].jenisLayanan = tmp;

            // Hitung ulang harga
            data[i].hargaTotal = hitungHarga(data[i].jenisLayanan, data[i].berat);

            cout << "[OK] Data berhasil diperbarui. Harga baru: Rp" << data[i].hargaTotal << "\n";
            return;
        }
    }
    cout << "[!] ID tidak ditemukan.\n";
}

// ============================================================
//  6. HAPUS DATA TRANSAKSI
// ============================================================

void hapusData() {
    cout << "\nMasukkan ID yang ingin dihapus : "; string id; cin >> id;
    for (int i = 0; i < jumlahData; i++) {
        if (data[i].id == id) {
            cout << "Konfirmasi hapus " << data[i].nama << "? (y/n) : ";
            char k; cin >> k;
            if (k == 'y' || k == 'Y') {
                for (int j = i; j < jumlahData - 1; j++)
                    data[j] = data[j + 1];
                jumlahData--;
                cout << "[OK] Data berhasil dihapus.\n";
            } else cout << "[!] Penghapusan dibatalkan.\n";
            return;
        }
    }
    cout << "[!] ID tidak ditemukan.\n";
}

// ============================================================
//  7. UPDATE STATUS LAUNDRY
// ============================================================

void updateStatus() {
    cout << "\nMasukkan ID : "; string id; cin >> id;
    for (int i = 0; i < jumlahData; i++) {
        if (data[i].id == id) {
            cout << "Status saat ini : " << data[i].status << endl;
            cout << "Pilih status baru:\n";
            cout << "(1) Menunggu\n(2) Dicuci\n(3) Disetrika\n(4) Selesai\n(5) Sudah Diambil\n";
            cout << "Pilihan : "; int p; cin >> p;
            switch (p) {
                case 1: data[i].status = "Menunggu";      break;
                case 2: data[i].status = "Dicuci";        break;
                case 3: data[i].status = "Disetrika";     break;
                case 4: data[i].status = "Selesai";       break;
                case 5: data[i].status = "Sudah Diambil"; break;
                default: cout << "[!] Pilihan tidak valid.\n"; return;
            }
            cout << "[OK] Status diperbarui menjadi: " << data[i].status << "\n";
            return;
        }
    }
    cout << "[!] ID tidak ditemukan.\n";
}

// ============================================================
//  8. SORTING
// ============================================================

void urutkanData() {
    if (jumlahData == 0) { cout << "[!] Belum ada data.\n"; return; }

    cout << "\nUrutkan berdasarkan:\n";
    cout << "(1) Nama\n(2) Berat\n(3) Harga\n";
    cout << "Pilihan : "; int pil; cin >> pil;
    cout << "Urutan  : (1) Ascending  (2) Descending : "; int asc; cin >> asc;

    // Bubble sort
    for (int i = 0; i < jumlahData - 1; i++) {
        for (int j = 0; j < jumlahData - i - 1; j++) {
            bool swap_cond = false;
            if (pil == 1) swap_cond = (asc == 1)
                ? data[j].nama > data[j+1].nama
                : data[j].nama < data[j+1].nama;
            else if (pil == 2) swap_cond = (asc == 1)
                ? data[j].berat > data[j+1].berat
                : data[j].berat < data[j+1].berat;
            else if (pil == 3) swap_cond = (asc == 1)
                ? data[j].hargaTotal > data[j+1].hargaTotal
                : data[j].hargaTotal < data[j+1].hargaTotal;

            if (swap_cond) swap(data[j], data[j+1]);
        }
    }
    cout << "[OK] Data berhasil diurutkan.\n";
    tampilkanData();
}

// ============================================================
//  9. DISKON MENGGUNAKAN POINTER
// ============================================================

void beriDiskon(Laundry* d, float persen) {
    d->hargaTotal = (int)(d->hargaTotal * (1.0f - persen / 100.0f));
}

void menuDiskon() {
    cout << "\nMasukkan ID untuk diberi diskon : "; string id; cin >> id;
    for (int i = 0; i < jumlahData; i++) {
        if (data[i].id == id) {
            cout << "Harga sebelum diskon : Rp" << data[i].hargaTotal << "\n";
            cout << "Besar diskon (%)     : "; float p; cin >> p;

            beriDiskon(&data[i], p);   // <-- penggunaan POINTER

            cout << "[OK] Harga setelah diskon " << p << "% : Rp" << data[i].hargaTotal << "\n";
            return;
        }
    }
    cout << "[!] ID tidak ditemukan.\n";
}

// ============================================================
//  10. TOTAL PENDAPATAN (RECURSIVE)
// ============================================================

long long totalPendapatanRekursif(int index) {
    if (index < 0) return 0;
    return data[index].hargaTotal + totalPendapatanRekursif(index - 1);
}

void hitungTotalPendapatan() {
    if (jumlahData == 0) { cout << "[!] Belum ada data.\n"; return; }
    long long total = totalPendapatanRekursif(jumlahData - 1);
    cout << "\n[=] Total Pendapatan (rekursif) : Rp" << total << "\n";
}

// ============================================================
//  11. SIMPAN DATA KE FILE
// ============================================================

void simpanKeFile() {
    ofstream file(NAMAFILE);
    if (!file.is_open()) {
        cout << "[!] Gagal membuka file untuk penulisan.\n"; return;
    }
    file << jumlahData << "\n";
    for (int i = 0; i < jumlahData; i++) {
        file << data[i].id          << "\n"
             << data[i].nama        << "\n"
             << data[i].berat       << "\n"
             << data[i].jenisLayanan<< "\n"
             << data[i].hargaTotal  << "\n"
             << data[i].status      << "\n";
    }
    file.close();
    cout << "[OK] Data berhasil disimpan ke \"" << NAMAFILE << "\" (" << jumlahData << " transaksi).\n";
}

// ============================================================
//  12. MUAT DATA DARI FILE
// ============================================================

void muatDariFile() {
    ifstream file(NAMAFILE);
    if (!file.is_open()) {
        cout << "[!] File \"" << NAMAFILE << "\" tidak ditemukan.\n"; return;
    }
    file >> jumlahData; file.ignore();
    for (int i = 0; i < jumlahData; i++) {
        getline(file, data[i].id);
        getline(file, data[i].nama);
        file >> data[i].berat; file.ignore();
        getline(file, data[i].jenisLayanan);
        file >> data[i].hargaTotal; file.ignore();
        getline(file, data[i].status);
    }
    file.close();
    cout << "[OK] Data berhasil dimuat dari \"" << NAMAFILE << "\" (" << jumlahData << " transaksi).\n";
}

// ============================================================
//  MENU UTAMA
// ============================================================

void tampilkanMenu() {
    garsPemisah('=');
    cout << "       SISTEM LAUNDRY SEDERHANA\n";
    garsPemisah('=');
    cout << " [1]  Tambah Transaksi\n";
    cout << " [2]  Tampilkan Seluruh Data\n";
    cout << " [3]  Cari Data (ID / Nama / Status)\n";
    cout << " [4]  Cek Status Laundry\n";
    cout << " [5]  Edit Data Transaksi\n";
    cout << " [6]  Hapus Data Transaksi\n";
    cout << " [7]  Update Status Laundry\n";
    cout << " [8]  Urutkan Data\n";
    cout << " [9]  Beri Diskon (Pointer)\n";
    cout << " [10] Hitung Total Pendapatan (Rekursif)\n";
    cout << " [11] Simpan Data ke File\n";
    cout << " [12] Muat Data dari File\n";
    cout << " [0]  Keluar\n";
    garsPemisah('=');
    cout << " Pilihan : ";
}

// ============================================================
//  MAIN
// ============================================================

int main() {
    // Coba muat otomatis saat program dibuka
    ifstream cek(NAMAFILE);
    if (cek.is_open()) {
        cek.close();
        muatDariFile();
        cout << "[INFO] Data sebelumnya telah dimuat secara otomatis.\n";
    }

    int pilihan;
    do {
        tampilkanMenu();
        cin >> pilihan;
        cout << endl;
        switch (pilihan) {
            case 1:  tambahTransaksi();      break;
            case 2:  tampilkanData();        break;
            case 3:  cariData();             break;
            case 4:  cekStatus();            break;
            case 5:  editData();             break;
            case 6:  hapusData();            break;
            case 7:  updateStatus();         break;
            case 8:  urutkanData();          break;
            case 9:  menuDiskon();           break;
            case 10: hitungTotalPendapatan();break;
            case 11: simpanKeFile();         break;
            case 12: muatDariFile();         break;
            case 0:
                cout << "Menyimpan data sebelum keluar...\n";
                simpanKeFile();
                cout << "Terima kasih! Program selesai.\n";
                break;
            default:
                cout << "[!] Pilihan tidak valid.\n";
        }
        if (pilihan != 0) {
            cout << "\nTekan Enter untuk lanjut...";
            cin.ignore(); cin.get();
        }
    } while (pilihan != 0);

    return 0;
}
