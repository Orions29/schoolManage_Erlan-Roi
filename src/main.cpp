#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

struct TemplateAcc
{
    string NISN;
    int NIS;
    string password;
    string namaLengkap;
    string tanggalLahir;
    string namaAyah;
    string namaIbu;
    string kelas;
    int tahunMasuk;
};
struct TemplatePermohonan
{
    int NIS;
    string dataLama, dataBaru;
    string alasan;
    string status;
};
TemplateAcc dataAcc[100];
TemplatePermohonan dataPermohonan[500];
// Global Declare
/**
 * @brief 0 untuk path data Acc, 1 untuk data permohonan
 *
 */
string pathDbDefault[2] = {"../db/dataAcc.txt", "db/dataPermohonan.txt"};
int jumlahAcc = 0, jumlahPermohonan = 0;

// Function Asli
// Overloading Funct untuk Error Handling Pengguna
/**
 * @brief fungsi overloading untuk menangani input streing
 * @param question pertanyaan yang ditampilkan
 * @param var variabel untuk meyimpan input string
 * @param lineOr 1 untuk tanpa spasi, 2 untuk input dengan spasi
 */
void inputHandling(string question, string &var, short lineOr)
{
    bool statLoop = false;
    do
    {
        cout << question;
        // Jika 1 maka Spasi dianggap kesalahan
        if (lineOr == 1)
        {
            cin >> var;
        }
        // Jika 2 maka spasi akan dibaca
        else if (lineOr == 2)
        {
            cin.ignore(30, '\n'); // Mengabaikan Input
            getline(cin, var);
        }
        if (cin.fail())
        {
            cin.clear();          // Menghapus Semua Fail Flag
            cin.ignore(30, '\n'); // Mengabaikan Input
            cout << "\n[ERROR CIN,not str] -" << "Input Tidak Sesuai\n";
            statLoop = true;
        }
        else
        {
            statLoop = false;
        }
    } while (statLoop == true);
};

/**
 * @brief fungsi overloading untuk menangani input integer
 * @param question pertanyaan yang ditampilkan
 * @param var variabel untuk meyimpan input integer
 */
void inputHandling(string question, int &var)
{
    bool statLoop = false;
    do
    {
        cout << question;
        cin >> var;
        if (cin.fail())
        {
            cin.clear();          // Menghapus Semua Fail Flag
            cin.ignore(30, '\n'); // Mengabaikan Input
            cout << "\n[ERROR CIN,not int] -" << "Input Tidak Sesuai\n";
            statLoop = true;
        }
        else
        {
            statLoop = false;
        }
    } while (statLoop == true);
};

/**
 * @brief fungsi overloading untuk menangani input float
 * @param question pertanyaan yang ditampilkan
 * @param var variabel untuk meyimpan input float
 */
void inputHandling(string question, float &var)
{
    cout << question;
    bool statLoop = false;
    do
    {
        cin >> var;
        if (cin.fail())
        {
            cin.clear();          // Menghapus Semua Fail Flag
            cin.ignore(30, '\n'); // Mengabaikan Input
            cout << "\n[ERROR CIN,not float] -" << "Input Tidak Sesuai\n";
            statLoop = true;
        }
        else
        {
            statLoop = false;
        }
    } while (statLoop == true);
};

// Untuk Short Short
/**
 * @brief fungsi overloading untuk menangani input short
 * @param question pertanyaan yang ditampilkan
 * @param var variabel untuk meyimpan input short
 */
void inputHandling(string question, short &var)
{
    cout << question;
    bool statLoop = false;
    do
    {
        cin >> var;
        if (cin.fail())
        {
            cin.clear();          // Menghapus Semua Fail Flag
            cin.ignore(30, '\n'); // Mengabaikan Input
            cout << "\n[ERROR CIN,not shortInt] -" << "Input Tidak Sesuai\n";
            statLoop = true;
        }
        else
        {
            statLoop = false;
        }
    } while (statLoop == true);
};

// Sorting and Searching Data
/**
 * @brief
 *
 * @param dataAccPart
 * @param low
 * @param high
 * @return int
 */
int partitionDataAcc(TemplateAcc dataAccPart[], int low, int high)
{
    int pivot = dataAccPart[high].NIS, i = low - 1;
    for (int j = low; j < high; j++)
    {
        if (dataAccPart[j].NIS < pivot)
            swap(dataAccPart[++i], dataAccPart[j]);
    }
    swap(dataAccPart[i + 1], dataAccPart[high]);
    return i + 1;
}
/**
 * @brief Ngequick Sort cukup panggil ini aja tanpa partition data Acc
 *
 * @param dataAccSort dataAcc yang mau di sort mana
 * @param low batas Bawah
 * @param high jumlah array yang ada di dataAcc berapa?
 */
void quickSortDataAcc(TemplateAcc dataAccSort[], int low, int high)
{
    if (low < high)
    {
        int pi = partitionDataAcc(dataAccSort, low, high);
        quickSortDataAcc(dataAccSort, low, pi - 1);
        quickSortDataAcc(dataAccSort, pi + 1, high);
    }
}
/**
 * @brief Fungsi pencarian biner iteratif pada array dataAcc berdasarkan NIS
 *
 * @param arr array dataAcc yang sudah diurutkan
 * @param left indeks awal pencarian
 * @param right indeks akhir pencarian
 * @param key NIS yang dicari
 * @return int indeks data yang ditemukan, -1 jika tidak ditemukan
 */
int binarySearchRecursive(TemplateAcc arr[], int left, int right, int key)
{
    if (left > right)
        return -1; // Basis kasus

    int mid = left + (right - left) / 2;

    if (arr[mid].NIS == key)
        return mid;
    else if (arr[mid].NIS < key)
        return binarySearchRecursive(arr, mid + 1, right, key);
    else
        return binarySearchRecursive(arr, left, mid - 1, key);
}

// Multifile Section
/**
 * @brief Pemisah data dari satu line baris yang diambil
 *
 * @param barisData baris data yang ingin di slice
 * @param barisDataOut baris data yang akan diubah nantinya dalam bentuk array
 * @param kolomData
 */
void commaSlicer(string barisData, string *barisDataOut, int kolomData)
{
    size_t posisiKoma[kolomData + 1];
    posisiKoma[0] = 0;
    // Mencari Posisi Koma lalu dimasukkan ke dalam arrayPosisiKoma
    int idxComa = 0;
    int idx = 1;
    while (barisData[idxComa] != ';')
    {
        if (barisData[idxComa] == ',')
        {
            posisiKoma[idx++] = idxComa;
        }
        idxComa++;
        if (idx > kolomData)
        {
            cout << "Gaada ';' disini tolong diperiksa db nya " << endl;
            return;
        }
    }
    posisiKoma[idx] = idxComa; // agar tau posisi akhir ; nya

    // for (int i = 0; i < kolomData; i++)
    // {
    //     cout << posisiKoma[i] << ',';
    // }
    // cout << endl;

    // Memotong string per koma
    for (int i = 0; i < idx; i++)
    {
        if (i == 0)
        {
            barisDataOut[i] = barisData.substr(posisiKoma[i], posisiKoma[i + 1] - posisiKoma[i]);
        }
        else
            barisDataOut[i] = barisData.substr(posisiKoma[i] + 1, posisiKoma[i + 1] - (posisiKoma[i] + 1));
    }
}

/**
 * @brief Fungsi untuk ngimport data dari file.txt ke dalam struct
 *
 * @param pathTxt path file txt yang mau dimasukkan
 * @param opt 1 ke dataAcc , 2 ke dataPermohonanAcc
 */
void importDataTxt(string *pathTxt, short opt)
{
    int indexNewLine;
    switch (opt)
    {
    case 1:
        // Untuk Import Data Acc
        {
            string barisData, barisDataIn[9];
            ifstream openFileTxt(pathTxt[0], ios::in);
            if (openFileTxt.is_open())
            {
                for (int i = 0; i < 100; ++i)
                {
                    dataAcc[jumlahAcc].NISN = "";
                    dataAcc[jumlahAcc].NIS = 0;
                    dataAcc[jumlahAcc].password = "";
                    dataAcc[jumlahAcc].namaLengkap = "";
                    dataAcc[jumlahAcc].tanggalLahir = "";
                    dataAcc[jumlahAcc].namaAyah = "";
                    dataAcc[jumlahAcc].namaIbu = "";
                    dataAcc[jumlahAcc].kelas = "";
                    dataAcc[jumlahAcc].tahunMasuk = 0;
                }
                jumlahAcc = 0;
                while (!openFileTxt.eof())
                {
                    getline(openFileTxt, barisData);
                    if (barisData == "")
                    {
                        cout << "Ada Newline di [" << pathTxt[0] << "] Di Baris-" << ++indexNewLine << endl;
                        system("pause");
                        continue;
                    }
                    commaSlicer(barisData, barisDataIn, 9);
                    // Memasukkan Data
                    dataAcc[jumlahAcc].NISN = barisDataIn[0];
                    dataAcc[jumlahAcc].NIS = stoi(barisDataIn[1]);
                    dataAcc[jumlahAcc].password = barisDataIn[2];
                    dataAcc[jumlahAcc].namaLengkap = barisDataIn[3];
                    dataAcc[jumlahAcc].tanggalLahir = barisDataIn[4];
                    dataAcc[jumlahAcc].namaAyah = barisDataIn[5];
                    dataAcc[jumlahAcc].namaIbu = barisDataIn[6];
                    dataAcc[jumlahAcc].kelas = barisDataIn[7];
                    dataAcc[jumlahAcc].tahunMasuk = stoi(barisDataIn[8]);
                    jumlahAcc++;
                    indexNewLine = jumlahAcc;
                }
            }
            else
            {
                cout << "[Err] - Terjadi Kesalahan Saat Mengimpor File";
                system("pause");
            }
            openFileTxt.close();
            break;
        }
    case 2:
        // Untuk Import Permohonan pergantian Data
        {
        }
        /* Silahkan Bangun codemu disini erlan */
        break;

    default:
        cout << "Not And Option You dick";
        break;
    }
}
/**
 * @brief Pengupdate data ke txt
 * @details Meng sort dulu sebelum mengexport sebuat data
 * @param pathDb array path database yang mau diUpdate
 * @param opt 1 untuk dataAcc, 2 untuk data permohonan
 */
void exportData(string *pathDb, short opt)
{
    switch (opt)
    {
    case 1:
    {
        ofstream exportDataAcc(pathDb[0], ios::out | ios::trunc);
        string barisDataOut;
        if (exportDataAcc.is_open())
        {
            int low = 0, high = jumlahAcc - 1;
            // Di Sort dulu biar bisa di binary Search
            quickSortDataAcc(dataAcc, low, high);
            for (int i = 0; i < jumlahAcc; i++)
            {
                // Error Handling untuk yang data kosong habis dihapus
                if (dataAcc[i].NIS == 0)
                {
                    continue;
                }
                else
                {
                    barisDataOut = dataAcc[i].NISN;
                    exportDataAcc << barisDataOut;
                    barisDataOut = to_string(dataAcc[i].NIS);
                    exportDataAcc << "," << barisDataOut;
                    barisDataOut = dataAcc[i].password;
                    exportDataAcc << "," << barisDataOut;
                    barisDataOut = dataAcc[i].namaLengkap;
                    exportDataAcc << "," << barisDataOut;
                    barisDataOut = dataAcc[i].tanggalLahir;
                    exportDataAcc << "," << barisDataOut;
                    barisDataOut = dataAcc[i].namaAyah;
                    exportDataAcc << "," << barisDataOut;
                    barisDataOut = dataAcc[i].namaIbu;
                    exportDataAcc << "," << barisDataOut;
                    barisDataOut = dataAcc[i].kelas;
                    exportDataAcc << "," << barisDataOut;
                    barisDataOut = to_string(dataAcc[i].tahunMasuk);
                    // Menghapuskan endline paling akhir di baris data
                    if (i != jumlahAcc - 1)
                    {
                        exportDataAcc << "," << barisDataOut << ";" << endl;
                    }
                    else
                    {
                        exportDataAcc << "," << barisDataOut << ";";
                    }
                }
            }
            cout << "Sudah Berhasil Mengupdate" << endl;
        }
        else
        {
            cout << "[Err] - Gagal Mengexport Data" << endl;
            system("pause");
        }
        break;
    }
    /* code */
    case 2:
    {
        break;
    }

    default:
        break;
    }
}

void displayData(TemplateAcc dataAcc[], int jumlahAcc, int opt)
{
    switch (opt)
    {
    case 1:
    {
        system("cls");
        cout << "Display Data All Peserta Didik" << endl;
        // Header Bolo
        cout << left
             << setw(12) << "NISN"
             << setw(8) << "NIS"
             << setw(15) << "Password"
             << setw(20) << "Nama Lengkap"
             << setw(14) << "Tgl Lahir"
             << setw(20) << "Nama Ayah"
             << setw(20) << "Nama Ibu"
             << setw(8) << "Kelas"
             << setw(12) << "Thn Masuk"
             << endl;

        // Garis pemisah
        cout << setfill('=') << setw(139) << "=" << setfill(' ') << endl;

        // Isi tabel
        for (int i = 0; i < jumlahAcc; ++i)
        {
            cout << left
                 << setw(12) << dataAcc[i].NISN
                 << setw(8) << dataAcc[i].NIS
                 << setw(15) << dataAcc[i].password
                 << setw(20) << dataAcc[i].namaLengkap
                 << setw(14) << dataAcc[i].tanggalLahir
                 << setw(20) << dataAcc[i].namaAyah
                 << setw(20) << dataAcc[i].namaIbu
                 << setw(8) << dataAcc[i].kelas
                 << setw(12) << dataAcc[i].tahunMasuk
                 << endl;
        }

        // Garis bawah
        cout << setfill('=') << setw(139) << "=" << setfill(' ') << endl;
        cout << ">>Data Berhasil Ditampilkan" << endl;
        system("pause");
        break;
    }
    case 2:
        // Erlan tolong yang cari data acc
        break;
    case 0:
        // Keluar dari fungsi
        break;

    default:
        break;
    }
}

void tambahHapusPesertaDidik(short optSub)
{
    TemplateAcc addSiswa;
    bool subMenuLoop = false;
    system("cls");
    switch (optSub)
    {
    case 1:
    {
        cout << ">> Menu Tambah Peserta Didik" << endl;
        cout << "-----------------" << endl;
        inputHandling("NISN             :", addSiswa.NISN, 1);
        do
        {
            inputHandling("NIS              :", addSiswa.NIS);
            if (addSiswa.NIS > 99999)
            {
                cout << "hmmm aneh kok banyak banget nih" << endl;
            }
            else
            {
                int left = 0, right = jumlahAcc - 1;
                if (binarySearchRecursive(dataAcc, left, right, addSiswa.NIS) > -1)
                {
                    cout << "Alamak NIS ini sudah ada nih" << endl;
                    addSiswa.NIS = 1000000;
                }
            }
        } while ((addSiswa.NIS > 99999));
        inputHandling("Password         :", addSiswa.password, 1);
        inputHandling("Nama Lengkap     :", addSiswa.namaLengkap, 2);
        inputHandling("Tanggal Lahir\n(dd/mm/yyyy) :", addSiswa.tanggalLahir, 1);
        inputHandling("Nama Ayah        :", addSiswa.namaAyah, 2);
        inputHandling("Nama Ibu         :", addSiswa.namaIbu, 2);
        inputHandling("Kelas            :", addSiswa.kelas, 1);
        inputHandling("Tahun Masuk      :", addSiswa.tahunMasuk);
        dataAcc[jumlahAcc] = addSiswa;
        jumlahAcc++;
        exportData(pathDbDefault, 1);
        cout << "Peserta didik berhasil ditambahkan!" << endl;
        system("pause");
        system("cls");
        break;
    }
    case 2:
    {
        bool statDelLoop = false;
        int optSubSub;
        do
        {
            int indexAcc, nisDel;
            int left = 0, right = jumlahAcc - 1;
            cout << ">> Menu Hapus Peserta Didik" << endl;
            cout << "-----------------" << endl;
            inputHandling("Berapa NIM nya? (exit = 0): ", nisDel);
            if (nisDel == 0)
            {
                optSubSub = 0;
            }
            else
            {

                indexAcc = binarySearchRecursive(dataAcc, left, right, nisDel);
                if (indexAcc > -1)
                {
                    int i = indexAcc;
                    cout << "--------------------------" << endl;
                    cout << "NISN: " << dataAcc[i].NISN << endl;
                    cout << "NIS: " << dataAcc[i].NIS << endl;
                    cout << "Password: " << dataAcc[i].password << endl;
                    cout << "Nama Lengkap: " << dataAcc[i].namaLengkap << endl;
                    cout << "Tanggal Lahir: " << dataAcc[i].tanggalLahir << endl;
                    cout << "Nama Ayah: " << dataAcc[i].namaAyah << endl;
                    cout << "Nama Ibu: " << dataAcc[i].namaIbu << endl;
                    cout << "Kelas: " << dataAcc[i].kelas << endl;
                    cout << "Tahun Masuk: " << dataAcc[i].tahunMasuk << endl;
                    cout << "--------------------------" << endl;
                    inputHandling("Apakah Yang ini ? \n(Ya = 1 | Tidak = 2 | Tidak jadi = 0): ", optSubSub);
                }
                else
                {
                    cout << "Ga Ketemu boss" << endl;
                    optSubSub = 2;
                    system("pause");
                }
                switch (optSubSub)
                {
                case 1:
                {
                    dataAcc[indexAcc].NISN = "";
                    dataAcc[indexAcc].NIS = 0;
                    dataAcc[indexAcc].password = "";
                    dataAcc[indexAcc].namaLengkap = "";
                    dataAcc[indexAcc].tanggalLahir = "";
                    dataAcc[indexAcc].namaAyah = "";
                    dataAcc[indexAcc].namaIbu = "";
                    dataAcc[indexAcc].kelas = "";
                    dataAcc[indexAcc].tahunMasuk = 0;
                    cout << "Peserta didik berhasil Dihapus!" << endl;
                    statDelLoop = false;
                    jumlahAcc--;
                    exportData(pathDbDefault, 1);
                    system("pause");
                    system("cls");
                    break;
                }
                case 2:
                {
                    statDelLoop = true;
                    break;
                }
                case 0:
                {
                    statDelLoop = false;
                    break;
                }
                default:
                    cout << "Ngomong opo toh" << endl;
                    statDelLoop = true;
                    break;
                }
            }
        } while (statDelLoop == true);
        break;
    }
    case 0:
        // Keluar dari fungsi
        break;
    default:
        break;
    }
}
void editDataPesertaDidik()
{
    bool statEditLoop = false;
    int optSubSub, optEdit = -1;
    do
    {
        system("cls");
        int indexAcc, nisEdit;
        int left = 0, right = jumlahAcc - 1;
        cout << ">> Menu Edit Peserta Didik" << endl;
        cout << "-----------------" << endl;
        inputHandling("Berapa NIM nya? (exit = 0): ", nisEdit);
        if (nisEdit == 0)
        {
            optSubSub = 0;
            return;
        }
        else
        {
            indexAcc = binarySearchRecursive(dataAcc, left, right, nisEdit);
            if (indexAcc > -1)
            {
                int i = indexAcc;
                system("cls");
                importDataTxt(pathDbDefault, 1);
                cout << "--------------------------" << endl;
                cout << "NISN: " << dataAcc[i].NISN << endl;
                cout << "NIS: " << dataAcc[i].NIS << endl;
                cout << "Password: " << dataAcc[i].password << endl;
                cout << "Nama Lengkap: " << dataAcc[i].namaLengkap << endl;
                cout << "Tanggal Lahir: " << dataAcc[i].tanggalLahir << endl;
                cout << "Nama Ayah: " << dataAcc[i].namaAyah << endl;
                cout << "Nama Ibu: " << dataAcc[i].namaIbu << endl;
                cout << "Kelas: " << dataAcc[i].kelas << endl;
                cout << "Tahun Masuk: " << dataAcc[i].tahunMasuk << endl;
                cout << "--------------------------" << endl;
                inputHandling("Apakah Yang ini ? \n(Ya = 1 | Tidak = 2 | Tidak jadi = 0): ", optSubSub);
            }
            else
            {
                cout << "Ga Ketemu boss" << endl;
                optSubSub = 2;
                system("pause");
            }
            switch (optSubSub)
            {
            case 1:
            {
                int i = indexAcc;
                while (optEdit != 0)
                {
                    system("cls");
                    cout << ">>> " << endl;
                    cout << "NISN: " << dataAcc[i].NISN << endl;
                    cout << "NIS: " << dataAcc[i].NIS << endl;
                    cout << "Password: " << dataAcc[i].password << endl;
                    cout << "Nama Lengkap: " << dataAcc[i].namaLengkap << endl;
                    cout << "Tanggal Lahir: " << dataAcc[i].tanggalLahir << endl;
                    cout << "Nama Ayah: " << dataAcc[i].namaAyah << endl;
                    cout << "Nama Ibu: " << dataAcc[i].namaIbu << endl;
                    cout << "Kelas: " << dataAcc[i].kelas << endl;
                    cout << "Tahun Masuk: " << dataAcc[i].tahunMasuk << endl;
                    cout << "--------------------------" << endl;
                    inputHandling("1. NISN \n2. NIS \n3. Nama Lengkap \n4. TTL \n5. Nama Ayah \n6. Nama Ibu \n7. Kelas \n8. Tahun Masuk \nApa Yang Mau diEdit? (exit = 0) ", optEdit);
                    switch (optEdit)
                    {
                    case 1:
                        inputHandling("NISN baru: ", dataAcc[i].NISN, 1);
                        break;
                    case 2:
                    {
                        int newNIS;
                        do
                        {
                            inputHandling("NIS baru: ", newNIS);
                            if (newNIS > 99999)
                            {
                                cout << "NIS terlalu besar!" << endl;
                            }
                            else
                            {
                                int left = 0, right = jumlahAcc - 1;
                                int idx = binarySearchRecursive(dataAcc, left, right, newNIS);
                                if (idx > -1 && idx != i)
                                {
                                    cout << "NIS sudah ada!" << endl;
                                    newNIS = 1000000;
                                }
                                else
                                {
                                    dataAcc[i].NIS = newNIS;
                                }
                            }
                        } while (newNIS > 99999);
                        break;
                    }
                    case 3:
                        inputHandling("Nama Lengkap baru: ", dataAcc[i].namaLengkap, 2);
                        break;
                    case 4:
                        inputHandling("Tanggal Lahir baru (dd/mm/yyyy): ", dataAcc[i].tanggalLahir, 1);
                        break;
                    case 5:
                        inputHandling("Nama Ayah baru: ", dataAcc[i].namaAyah, 2);
                        break;
                    case 6:
                        inputHandling("Nama Ibu baru: ", dataAcc[i].namaIbu, 2);
                        break;
                    case 7:
                        inputHandling("Kelas baru: ", dataAcc[i].kelas, 1);
                        break;
                    case 8:
                        inputHandling("Tahun Masuk baru: ", dataAcc[i].tahunMasuk);
                        break;
                    case 0:
                        break;
                    default:
                        cout << "Pilihan tidak valid!" << endl;
                        break;
                    }
                    if (optEdit != 0 && optEdit < 9 && optEdit > 0)
                    {
                        cout << "Peserta didik berhasil DiEdit!" << endl;
                        system("pause");
                    }
                    statEditLoop = false;
                }
                exportData(pathDbDefault, 1);
                break;
            }
            case 2:
            {
                statEditLoop = true;
                break;
            }
            case 0:
            {
                statEditLoop = false;
                break;
            }
            default:
                cout << "Ngomong opo toh" << endl;
                statEditLoop = true;
                break;
            }
        }

    } while (statEditLoop == true);
}
/**
 * @brief Menu yang bersifat looping
 *
 * @param menuAdminStat admin stat yang ada di login
 * @param menuLoginStat login stat yang mengatur perulangan
 */
void menu(bool &menuAdminStat, bool &menuLoginStat)
{
    // Menu Admin
    if (menuAdminStat == true)
    {
        short opt, optSub;
        bool subMenLoop = false;
        while (menuLoginStat == true)
        {
            importDataTxt(pathDbDefault, 1);
            system("cls");
            cout << ">> Selamat Datang Di Menu Admin" << endl;
            cout << "1. Display Peserta Didik\n2. Tambah dan Hapus Peserta Didik\n3. Edit Data Peserta Didik\n4. Jawab Permohonan Pergantian Data" << endl;
            inputHandling(" mau yang mana menunya (exit = 0): ", opt);
            switch (opt)
            {
            case 1:
            {
                do
                {
                    system("cls");
                    cout << ">>Menu Penampil Data Akun" << endl;
                    inputHandling("1. Tampilkan Seluruh Akun\n2. Cari Data Akun\n Mau Pilih yang mana nih (back = 0): ", optSub);
                    displayData(dataAcc, jumlahAcc, optSub);
                    if (optSub == 0)
                    {
                        subMenLoop = false;
                    }
                    else
                    {
                        subMenLoop = true;
                    }
                } while (subMenLoop == true);
                subMenLoop = false;
                /* code */
                break;
            }
            case 2:
            {
                do
                {
                    system("cls");
                    cout << ">>Menu Tambah dan Hapus Akun" << endl;
                    inputHandling("1. Tambah Akun\n2. Hapus Akun\n Mau Pilih yang mana nih (back = 0): ", optSub);
                    tambahHapusPesertaDidik(optSub);
                    if (optSub == 0)
                    {
                        subMenLoop = false;
                    }
                    else
                    {
                        subMenLoop = true;
                    }
                } while (subMenLoop == true);
                subMenLoop = false;
                break;
            }
            case 3:
            {
                editDataPesertaDidik();
                break;
            }
            case 0:
                menuAdminStat = false;
                menuLoginStat = false;
                break;

            default:
                break;
            }
        }
    }
    // Menu Client
    else
    {
        short opt, optSub;
        while (menuLoginStat == true)
        {
            cout << ">> Selamat Datang Di Menu Client" << endl;
            inputHandling(" mau yang mana menunya (exit = 0)", opt);
            cout << "Masuk ke menu orang hutan" << endl;
            // Code Client Dsini
        }
    }
};
/**
 * @brief Menu Login Utama untuk membedakan mana admin dan mana manusia biasa
 *
 */
void login()
{
    bool adminStat = false, loginStat = false;
    int usernameIn;
    string passwordIn;
    while (loginStat == false)
    {
        system("cls");
        cout << "<Menu Login>" << endl;
        inputHandling("Masukkan Username (exit = 0) : ", usernameIn);
        if (usernameIn == 0)
        {
            adminStat = false;
            loginStat = false;
            return;
        }
        else if (usernameIn == 11111)
        {
            adminStat = true;
            loginStat = true;
            importDataTxt(pathDbDefault, 1);
            menu(adminStat, loginStat);
        }
        else
        {
            importDataTxt(pathDbDefault, 1);
            int left = 0, right = jumlahAcc - 1;
            int indexAcc = binarySearchRecursive(dataAcc, left, right, usernameIn);
            if (indexAcc == -1)
            {
                cout << "Data Tidak Ditemukan Bolo pindah file atau cari lagi sana" << endl;
                system("pause");
            }
            else
            {
                adminStat = false;
                loginStat = true;
                cout << dataAcc[indexAcc].namaLengkap << " [ini bukan?]" << endl;
                menu(adminStat, loginStat);
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    // string coba = "240xxxxx,16870,YuhuJokowi,Joko Wi Dek Tok,11/11/2005,Mulyadi,Ismayarti,IF-D,2018;";
    // string coba2 = "16870,aku pingin tanggal 2018,2018 jadi hehehe,ya gitu deh males aku,ACC;";
    // string cobaArr[9];

    // Membuat dummy data pada struct dataAcc
    // dataAcc[10] = {"1234567890", 10001, "password1", "Budi Santoso", "01/01/2005", "Pak Budi", "Bu Budi", "X-A", 2020};
    // dataAcc[11] = {"0987654321", 10002, "password2", "Siti Aminah", "02/02/2006", "Pak Siti", "Bu Siti", "XI-B", 2021};
    // jumlahAcc = 12;
    login();
    // for (int i = 0; i < jumlahAcc; ++i)
    // {
    //     cout << "NISN: " << dataAcc[i].NISN << endl;
    //     cout << "NIS: " << dataAcc[i].NIS << endl;
    //     cout << "Password: " << dataAcc[i].password << endl;
    //     cout << "Nama Lengkap: " << dataAcc[i].namaLengkap << endl;
    //     cout << "Tanggal Lahir: " << dataAcc[i].tanggalLahir << endl;
    //     cout << "Nama Ayah: " << dataAcc[i].namaAyah << endl;
    //     cout << "Nama Ibu: " << dataAcc[i].namaIbu << endl;
    //     cout << "Kelas: " << dataAcc[i].kelas << endl;
    //     cout << "Tahun Masuk: " << dataAcc[i].tahunMasuk << endl;
    //     cout << "--------------------------" << endl;
    // }

    // exportData(pathDbDefault, 1);

    return 0;
}
