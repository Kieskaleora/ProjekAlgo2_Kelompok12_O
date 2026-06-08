#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

#define MAX 100

struct Laundry {
    string id;
    string nama;
    float  berat;
    string jenisLayanan;
    int    hargaTotal;
    string status;
};

const string NAMAFILE = "data_laundry.txt";

Laundry dataLaundry[MAX];
int jumlahData = 0;

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
        if (dataLaundry[i].id == id) return true;
    return false;
}

string generateID() {
    return "LDR" + to_string(jumlahData + 1);
}

int hitungHarga(const string& jenis, float berat) {
    int hargaPerKg = 0;
    string j = toUpperStr(jenis);
    if      (j == "REGULER") hargaPerKg = 5000;
    else if (j == "EXPRESS") hargaPerKg = 10000;
    else if (j == "KILAT")   hargaPerKg = 15000;
    else                     hargaPerKg = 5000;  
    return (int)(berat * hargaPerKg);
}

void tambahTransaksi() {
    if (jumlahData >= MAX) {
        cout << "Data penuh! Kapasitas maksimal " << MAX << " transaksi.\n";
        return;
    }

    Laundry baru;
    baru.id     = generateID();
    baru.status = "Menunggu";

    cout << "\nID otomatis: " << baru.id << endl;
    cout << "    Nama Pelanggan : "; cin.ignore(); getline(cin, baru.nama);
    cout << "    Berat (kg)     : "; cin >> baru.berat;
    cout << "    Jenis Layanan  \n";
    cout << "    (1) Reguler Rp5.000/kg\n";
    cout << "    (2) Express Rp10.000/kg\n";
    cout << "    (3) Kilat   Rp15.000/kg\n";
    cout << "    Pilihan        : ";
    int pil; cin >> pil;
    switch (pil) {
        case 1: baru.jenisLayanan = "Reguler"; break;
        case 2: baru.jenisLayanan = "Express"; break;
        case 3: baru.jenisLayanan = "Kilat";   break;
        default: baru.jenisLayanan = "Reguler";
    }

    baru.hargaTotal = hitungHarga(baru.jenisLayanan, baru.berat);

    dataLaundry[jumlahData++] = baru;

    cout << "\nTransaksi berhasil ditambahkan!\n";
    cout << "     Total Harga : Rp" << baru.hargaTotal << "\n";
}

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

void tampilkanData() {
    if (jumlahData == 0) {
        cout << "Belum ada dataLaundry transaksi.\n"; return;
    }
    cout << "\n=== DAFTAR SELURUH TRANSAKSI (" << jumlahData << " dataLaundry) ===\n";
    headerTabel();
    for (int i = 0; i < jumlahData; i++)
        cetakBaris(dataLaundry[i]);
    garsPemisah('=');
}

//PAKAI BUBBLE SORT
void urutkanInternal(int jenisKunci) {
    for (int i = 0; i < jumlahData - 1; i++) {
        for (int j = 0; j < jumlahData - i - 1; j++) {
            bool tukar = false;
            if (jenisKunci == 1)      tukar = toUpperStr(dataLaundry[j].id) > toUpperStr(dataLaundry[j+1].id);
            else if (jenisKunci == 2) tukar = toUpperStr(dataLaundry[j].nama) > toUpperStr(dataLaundry[j+1].nama);
            else if (jenisKunci == 3) tukar = toUpperStr(dataLaundry[j].status) > toUpperStr(dataLaundry[j+1].status);

            if (tukar) {
                Laundry temp = dataLaundry[j];
                dataLaundry[j] = dataLaundry[j+1];
                dataLaundry[j+1] = temp;
            }
        }
    }
}

//INI BINARY SEARCH
void cariData() {
    if (jumlahData == 0) {
        cout << "Belum ada data laundry untuk dicari.\n";
        return;
    }

    cout << "\n=== CARI DATA (BINARY SEARCH) ===\n";
    cout << "Cari berdasarkan:\n";
    cout << "(1) ID\n(2) Nama\n(3) Status\n";
    cout << "Pilihan : "; int pil; cin >> pil;
    if (pil < 1 || pil > 3) { cout << "[!] Pilihan tidak valid.\n"; return; }

    cout << "Kata kunci : "; string kunci; cin.ignore(); getline(cin, kunci);
    string k = toUpperStr(kunci);

    urutkanInternal(pil);

    int low = 0, high = jumlahData - 1;
    bool ditemukan = false;

    headerTabel();
    
    while (low <= high) {
        int mid = low + (high - low) / 2;
        string nilaiTengah = "";

        if (pil == 1)      nilaiTengah = toUpperStr(dataLaundry[mid].id);
        else if (pil == 2) nilaiTengah = toUpperStr(dataLaundry[mid].nama);
        else if (pil == 3) nilaiTengah = toUpperStr(dataLaundry[mid].status);

        if (nilaiTengah == k || (pil == 2 && nilaiTengah.find(k) != string::npos)) {
            cetakBaris(dataLaundry[mid]);
            ditemukan = true;

            int kiri = mid - 1;
            while (kiri >= 0) {
                string nilaiKiri = (pil == 1) ? toUpperStr(dataLaundry[kiri].id) : (pil == 2) ? toUpperStr(dataLaundry[kiri].nama) : toUpperStr(dataLaundry[kiri].status);
                if (nilaiKiri == k || (pil == 2 && nilaiKiri.find(k) != string::npos)) {
                    cetakBaris(dataLaundry[kiri]);
                    kiri--;
                } else break;
            }
            
            int kanan = mid + 1;
            while (kanan < jumlahData) {
                string nilaiKanan = (pil == 1) ? toUpperStr(dataLaundry[kanan].id) : (pil == 2) ? toUpperStr(dataLaundry[kanan].nama) : toUpperStr(dataLaundry[kanan].status);
                if (nilaiKanan == k || (pil == 2 && nilaiKanan.find(k) != string::npos)) {
                    cetakBaris(dataLaundry[kanan]);
                    kanan++;
                } else break;
            }
            break;
        }
        
        if (nilaiTengah < k) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (!ditemukan) cout << "Data tidak ditemukan.\n";
}

void cekStatus() {
    cout << "\nMasukkan ID pelanggan : "; string id; cin >> id;
    for (int i = 0; i < jumlahData; i++) {
        if (dataLaundry[i].id == id) {
            cout << "\n--- Info Transaksi ---\n";
            cetakBaris(dataLaundry[i]);
            return;
        }
    }
    cout << "ID tidak ditemukan.\n";
}

void editData() {
    cout << "\nMasukkan ID yang ingin diedit : "; string id; cin >> id;
    for (int i = 0; i < jumlahData; i++) {
        if (dataLaundry[i].id == id) {
            cout << "\nData saat ini:\n"; cetakBaris(dataLaundry[i]);
            cout << "\nNama baru (kosongkan = tidak berubah) : ";
            cin.ignore(); string tmp; getline(cin, tmp);
            if (!tmp.empty()) dataLaundry[i].nama = tmp;

            cout << "Berat baru (0 = tidak berubah)        : ";
            float b; cin >> b;
            if (b > 0) dataLaundry[i].berat = b;

            cout << "Jenis Layanan baru (kosong = tetap) [Reguler/Express/Kilat] : ";
            cin.ignore(); getline(cin, tmp);
            if (!tmp.empty()) dataLaundry[i].jenisLayanan = tmp;

            dataLaundry[i].hargaTotal = hitungHarga(dataLaundry[i].jenisLayanan, dataLaundry[i].berat);

            cout << "[OK] Data berhasil diperbarui. Harga baru: Rp" << dataLaundry[i].hargaTotal << "\n";
            return;
        }
    }
    cout << "[!] ID tidak ditemukan.\n";
}

void hapusData() {
    cout << "\nMasukkan ID yang ingin dihapus : "; string id; cin >> id;
    for (int i = 0; i < jumlahData; i++) {
        if (dataLaundry[i].id == id) {
            cout << "Konfirmasi hapus " << dataLaundry[i].nama << "? (y/n) : ";
            char k; cin >> k;
            if (k == 'y' || k == 'Y') {
                for (int j = i; j < jumlahData - 1; j++)
                    dataLaundry[j] = dataLaundry[j + 1];
                jumlahData--;
                cout << "Data berhasil dihapus.\n";
            } else cout << "Penghapusan dibatalkan.\n";
            return;
        }
    }
    cout << "ID tidak ditemukan.\n";
}

void updateStatus() {
    cout << "\nMasukkan ID : "; string id; cin >> id;
    for (int i = 0; i < jumlahData; i++) {
        if (dataLaundry[i].id == id) {
            cout << "Status saat ini : " << dataLaundry[i].status << endl;
            cout << "Pilih status baru:\n";
            cout << "(1) Menunggu\n(2) Dicuci\n(3) Disetrika\n(4) Selesai\n(5) Sudah Diambil\n";
            cout << "Pilihan : "; int p; cin >> p;
            switch (p) {
                case 1: dataLaundry[i].status = "Menunggu";      break;
                case 2: dataLaundry[i].status = "Dicuci";        break;
                case 3: dataLaundry[i].status = "Disetrika";     break;
                case 4: dataLaundry[i].status = "Selesai";       break;
                case 5: dataLaundry[i].status = "Sudah Diambil"; break;
                default: cout << "Pilihan tidak valid.\n"; return;
            }
            cout << "Status diperbarui menjadi: " << dataLaundry[i].status << "\n";
            return;
        }
    }
    cout << "ID tidak ditemukan.\n";
}

void urutkanData() {
    if (jumlahData == 0) { cout << "Belum ada data laundry.\n"; return; }

    cout << "\nUrutkan berdasarkan:\n";
    cout << "(1) Nama\n(2) Berat\n(3) Harga\n";
    cout << "Pilihan : "; int pil; cin >> pil;
    cout << "Urutan  : (1) Ascending  (2) Descending : "; int asc; cin >> asc;

    for (int i = 0; i < jumlahData - 1; i++) {
        for (int j = 0; j < jumlahData - i - 1; j++) {
            bool swap_cond = false;
            if (pil == 1) swap_cond = (asc == 1)
                ? dataLaundry[j].nama > dataLaundry[j+1].nama
                : dataLaundry[j].nama < dataLaundry[j+1].nama;
            else if (pil == 2) swap_cond = (asc == 1)
                ? dataLaundry[j].berat > dataLaundry[j+1].berat
                : dataLaundry[j].berat < dataLaundry[j+1].berat;
            else if (pil == 3) swap_cond = (asc == 1)
                ? dataLaundry[j].hargaTotal > dataLaundry[j+1].hargaTotal
                : dataLaundry[j].hargaTotal < dataLaundry[j+1].hargaTotal;

            if (swap_cond) {
                Laundry temp = dataLaundry[j];
                dataLaundry[j] = dataLaundry[j+1];
                dataLaundry[j+1] = temp;
            }
        }
    }
    cout << "Data berhasil diurutkan.\n";
    tampilkanData();
}

void beriDiskon(Laundry* d, float persen) {
    d->hargaTotal = (int)(d->hargaTotal * (1.0f - persen / 100.0f));
}

void menuDiskon() {
    cout << "\nMasukkan ID untuk diberi diskon : "; string id; cin >> id;
    for (int i = 0; i < jumlahData; i++) {
        if (dataLaundry[i].id == id) {
            cout << "Harga sebelum diskon : Rp" << dataLaundry[i].hargaTotal << "\n";
            cout << "Besar diskon (%)     : "; float p; cin >> p;

            beriDiskon(&dataLaundry[i], p);

            cout << "Harga setelah diskon " << p << "% : Rp" << dataLaundry[i].hargaTotal << "\n";
            return;
        }
    }
    cout << "ID tidak ditemukan.\n";
}

long long totalPendapatanRekursif(int index) {
    if (index < 0) return 0;
    return dataLaundry[index].hargaTotal + totalPendapatanRekursif(index - 1);
}

void hitungTotalPendapatan() {
    if (jumlahData == 0) { cout << "Belum ada data laundry.\n"; return; }
    long long total = totalPendapatanRekursif(jumlahData - 1);
    cout << "\nTotal Pendapatan (rekursif) : Rp" << total << "\n";
}

void simpanKeFile() {
    ofstream file(NAMAFILE);
    if (!file.is_open()) {
        cout << "Gagal membuka file untuk penulisan.\n"; return;
    }
    file << jumlahData << "\n";
    for (int i = 0; i < jumlahData; i++) {
        file << dataLaundry[i].id          << "\n"
             << dataLaundry[i].nama        << "\n"
             << dataLaundry[i].berat       << "\n"
             << dataLaundry[i].jenisLayanan<< "\n"
             << dataLaundry[i].hargaTotal  << "\n"
             << dataLaundry[i].status      << "\n";
    }
    file.close();
    cout << "Data berhasil disimpan ke \"" << NAMAFILE << "\" (" << jumlahData << " transaksi).\n";
}

void muatDariFile() {
    ifstream file(NAMAFILE);
    if (!file.is_open()) {
        cout << "File \"" << NAMAFILE << "\" tidak ditemukan.\n"; return;
    }
    file >> jumlahData; file.ignore();
    for (int i = 0; i < jumlahData; i++) {
        getline(file, dataLaundry[i].id);
        getline(file, dataLaundry[i].nama);
        file >> dataLaundry[i].berat; file.ignore();
        getline(file, dataLaundry[i].jenisLayanan);
        file >> dataLaundry[i].hargaTotal; file.ignore();
        getline(file, dataLaundry[i].status);
    }
    file.close();
    cout << "Data berhasil dimuat dari \"" << NAMAFILE << "\" (" << jumlahData << " transaksi).\n";
}

void tampilkanMenu() {
    garsPemisah('=');
    cout << "         SISTEM LAUNDRY SEDERHANA\n";
    garsPemisah('=');
    cout << "1. Tambah Transaksi\n";
    cout << "2. Tampilkan Seluruh Data\n";
    cout << "3. Cari Data (Binary Search)\n";
    cout << "4. Cek Status Laundry\n";
    cout << "5. Edit Data Transaksi\n";
    cout << "6. Hapus Data Transaksi\n";
    cout << "7. Update Status Laundry\n";
    cout << "8. Urutkan Data\n";
    cout << "9. Beri Diskon (Pointer)\n";
    cout << "10. Hitung Total Pendapatan (Rekursif)\n";
    cout << "11. Simpan Data ke File\n";
    cout << "12. Muat Data dari File\n";
    cout << "0.  Keluar\n";
    garsPemisah('=');
    cout << " Pilihan : ";
}

int main() {
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
                cout << "Menyimpan data laundry sebelum keluar...\n";
                simpanKeFile();
                cout << "Terima kasih! Program selesai.\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
        if (pilihan != 0) {
            cout << "\nTekan Enter untuk lanjut...";
            cin.ignore(); cin.get();
        }
    } while (pilihan != 0);

    return 0;
}
