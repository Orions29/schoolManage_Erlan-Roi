#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
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
string pathDbDefault[2] = {"../db/dataAcc.txt", "../db/dataPermohonan.txt"};
int jumlahAcc = 0, jumlahPermohonan = 0;
// Function Asli
// Overloading Funct untuk Error Handling Pengguna
/**
 * @brief fungsi overloading untuk menangani input streing
 * @param question pertanyaan yang ditampilkan
 * @param var variabel untuk meyimpan input string
 * @param lineOr 1 untuk tanpa spasi, 2 untuk input dengan spasi
 */

void inputHandling(const string &question, string &var, short lineOr)
{
    bool statLoop;
    do
    {
        statLoop = false;
        cout << question;
        if (lineOr == 1)
        {
            cin >> var;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bersihkan newline
        }
        else if (lineOr == 2)
        {
            // Bersihkan sisa newline dari input sebelumnya
            if (cin.peek() == '\n')
                cin.ignore(); // <-- ini penting!
            getline(cin, var);
        }
        if (cin.fail())
        {
            cin.clear();                                         // Reset error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Buang input error
            cout << "\n[ERROR CIN, not str] - Input Tidak Sesuai\n";
            statLoop = true;
        }
    } while (statLoop);
}

/**
 * @brief fungsi overloading untuk menangani input integer
 * @param question pertanyaan yang ditampilkan
 * @param var variabel untuk meyimpan input integer
 */
void inputHandling(const string &question, int &var)
{
    bool statLoop = false;
    do
    {
        cout << question;
        cin >> var;
        if (cin.fail())
        {
            cin.clear();                                         // Reset error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Buang sisa input
            cout << "\n[ERROR CIN, not int] - Input Tidak Sesuai\n";
            statLoop = true;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Buang sisa input setelah angka
            statLoop = false;
        }
    } while (statLoop);
}

/**
 * @brief fungsi overloading untuk menangani input float
 * @param question pertanyaan yang ditampilkan
 * @param var variabel untuk meyimpan input float
 */
void inputHandling(const string &question, float &var)
{
    bool statLoop = false;
    do
    {
        cout << question;
        cin >> var;
        if (cin.fail())
        {
            cin.clear();                                         // Reset error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Buang sisa input
            cout << "\n[ERROR CIN, not float] - Input Tidak Sesuai\n";
            statLoop = true;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bersihkan sisa karakter
            statLoop = false;
        }

    } while (statLoop);
}

// Untuk Short Short
/**
 * @brief fungsi overloading untuk menangani input short
 * @param question pertanyaan yang ditampilkan
 * @param var variabel untuk meyimpan input short
 */
void inputHandling(const string &question, short &var)
{
    bool statLoop = false;
    do
    {
        cout << question;
        cin >> var;
        if (cin.fail())
        {
            cin.clear();                                         // Reset error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bersihkan sisa karakter
            cout << "\n[ERROR CIN, not shortInt] - Input Tidak Sesuai\n";
            statLoop = true;
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bersihkan sisa setelah input valid
            statLoop = false;
        }
    } while (statLoop);
}

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
 * @brief pengurutan array TemplateAcc menggunakan shell sort
 *
 * @param arr Array dari struct TemplateAcc yang ingin diurutkan
 * @param n Jumlah elemen dalam array
 * @param mode Mode Pengurutan:
 *                          - 1. berdasarkan NIS (ascend)
 *                          - 2. berdasarkan tahun masuk (ascend)
 */
void shellSortAcc(TemplateAcc arr[], int n, short mode)
{

    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        for (int i = gap; i < n; i++)
        {
            TemplateAcc temp = arr[i];
            int j = i;

            while (j >= gap && ((mode == 1 && arr[j - gap].NIS > temp.NIS) || (mode == 2 && arr[j - gap].tahunMasuk > temp.tahunMasuk)))
            {
                arr[j] = arr[j - gap];
                j -= gap;
            }

            arr[j] = temp;
        }
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
            string barisData, barisDataIn[5];
            ifstream openFileTxt(pathTxt[1], ios ::in);
            if (openFileTxt.is_open())
            {
                jumlahPermohonan = 0;
                while (!openFileTxt.eof())
                {
                    getline(openFileTxt, barisData);
                    if (barisData == "")
                    {
                        continue;
                    }
                    commaSlicer(barisData, barisDataIn, 5);
                    dataPermohonan[jumlahPermohonan].NIS = stoi(barisDataIn[0]);
                    dataPermohonan[jumlahPermohonan].dataLama = barisDataIn[1];
                    dataPermohonan[jumlahPermohonan].dataBaru = barisDataIn[2];
                    dataPermohonan[jumlahPermohonan].alasan = barisDataIn[3];
                    dataPermohonan[jumlahPermohonan].status = barisDataIn[4];
                    jumlahPermohonan++;
                }
                // ini ngebug gasi apa perasaan gw doang
            }
            else
            {
                cout << "[Err] - Gagal membuka file permohonan\n";
                system("pause");
            }
            openFileTxt.close();
            break;
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
        ofstream exportPermohonan(pathDbDefault[1], ios::out | ios::trunc);
        string barisDataOut;
        if (exportPermohonan.is_open())
        {
            for (int i = 0; i < jumlahPermohonan; i++)
            {
                if (dataPermohonan[i].NIS == 0)
                {
                    continue; // skip data yang kosong
                }

                barisDataOut = to_string(dataPermohonan[i].NIS);
                exportPermohonan << barisDataOut;
                exportPermohonan << "," << dataPermohonan[i].dataLama;
                exportPermohonan << "," << dataPermohonan[i].dataBaru;
                exportPermohonan << "," << dataPermohonan[i].alasan;
                if (i != jumlahPermohonan - 1)
                {
                    exportPermohonan << "," << dataPermohonan[i].status << ";" << endl;
                }
                else
                {
                    exportPermohonan << "," << dataPermohonan[i].status << ";";
                }
            }
        }
        else
        {
            cout << "[Err] - Gagal membuka file export permohonan\n";
            system("pause");
        }
        break;
    }

    default:
        break;
    }
}
/**
 * @brief Penampil Data ACC
 *
 * @param dataAcc Struct data Acc yang mau ditampilkan
 * @param jumlahAcc Jumlh Struct Data Acc
 * @param opt 1 Display All Data Peserta Didik (include sorting), 2. Display Spesifik Peserta didik
 * @param NISSearch True untuk nge bypass Search Input Peserta Didik, false untuk Ngeinput Manual
 * @param NIS2Search NIS yang perlu Dicari
 */
void displayData(TemplateAcc dataAcc[], int jumlahAcc, int opt, bool NISSearch = false, int NIS2Search = 0)
{
    switch (opt)
    {
    case 1:
    {
        // system("cls");
        // short sortOpt;
        // inputHandling("Tampilkan dengan urutan berdasarkan:\n1. NIS\n2. Tahun Masuk\nPilihan (1/2) : ", sortOpt);

        TemplateAcc tempData[100]; // copy ke array local
        for (int i = 0; i < jumlahAcc; i++)
        {
            tempData[i] = dataAcc[i];
        }

        short sortOpt;
        inputHandling("Tampilkan dengan urutan berdasarkan:\n1. NIS\n2. Tahun Masuk\nPilihan (1/2) : ", sortOpt);

        shellSortAcc(tempData, jumlahAcc, sortOpt);

        // system("cls");
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
                 << setw(12) << tempData[i].NISN
                 << setw(8) << tempData[i].NIS
                 << setw(15) << tempData[i].password
                 << setw(20) << tempData[i].namaLengkap
                 << setw(14) << tempData[i].tanggalLahir
                 << setw(20) << tempData[i].namaAyah
                 << setw(20) << tempData[i].namaIbu
                 << setw(8) << tempData[i].kelas
                 << setw(12) << tempData[i].tahunMasuk
                 << endl;
        }

        // Garis bawah
        cout << setfill('=') << setw(139) << "=" << setfill(' ') << endl;
        cout << ">>Data Berhasil Ditampilkan" << endl;
        system("pause");
        break;
    }
    case 2:
    {
        if (NISSearch == false)
        {
            inputHandling("Siapa Yang Mau Kamu Cari?: ", NIS2Search);
        }
        int idx = binarySearchRecursive(dataAcc, 0, jumlahAcc - 1, NIS2Search);
        if (idx == -1)
        {
            cout << ">> Data siswa tidak ditemukan\n";
            return;
        }
        cout << "\n=== ==========BIODATA SISWA ================\n";
        cout << "NISN          : " << dataAcc[idx].NISN << endl;
        cout << "NIS           : " << dataAcc[idx].NIS << endl;
        cout << "Nama Lengkap  : " << dataAcc[idx].namaLengkap << endl;
        cout << "Tanggal Lahir : " << dataAcc[idx].tanggalLahir << endl;
        cout << "Nama Ayah     : " << dataAcc[idx].namaAyah << endl;
        cout << "Nama Ibu      : " << dataAcc[idx].namaIbu << endl;
        cout << "Kelas         : " << dataAcc[idx].kelas << endl;
        cout << "Tahun Masuk   : " << dataAcc[idx].tahunMasuk << endl;
        cout << "==============================================\n";
        system("pause");
        break;
    }
    case 0:
        // Keluar dari fungsi
        break;
    default:
        break;
    }
}

/**
 * @brief menampilkan data peserta didik setelah disort menggunakan shell sort
 *
 * fungsi ini menyalin data dari array global ke lokal boloo
 * terus diurutin berdasarkan nis atau tahun masuk boloo
 */

void tampilkanDataShellSort()
{
    if (jumlahAcc == 0)
    {
        cout << "Data Kosong ngab!\n";
        return;
    }

    TemplateAcc tempData[100]; // copy global array
    for (int i = 0; i < jumlahAcc; i++)
    {
        tempData[i] = dataAcc[i];
    }

    short pilihanSort;
    inputHandling("Sort berdasarkan:\n1. NIS\n2. Tahun Masuk\nPilihan: ", pilihanSort);

    shellSortAcc(tempData, jumlahAcc, pilihanSort);
    displayData(tempData, jumlahAcc, 1);
}

/**
 * @brief Menambahkan atau menghapus data peserta didik
 *
 * @param optSub pilihan submenu: 1. tambah, 2. hapus
 */

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
            inputHandling("Berapa NIS nya? (exit = 0): ", nisDel);
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

/**
 * @brief Mengedit data peserta didik berdasarkan NIS
 *
 */
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
        inputHandling("Berapa NIS nya? (exit = 0): ", nisEdit);
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
 * @brief mengajukan permohonan perubahan data peserta didik
 *
 * @param nis NIS peserta diidik yang mengajukan perubahan
 * @param indexAcc indeks dari peserta didik dalam array dataAcc
 */
void ajukanPermohonanPerubahanData(int nis, int indexAcc)
{
    TemplatePermohonan permohonan;
    importDataTxt(pathDbDefault, 2);
    system("cls");
    displayData(dataAcc, jumlahAcc, 2, true, dataAcc[indexAcc].NIS);
    permohonan.NIS = nis;
    inputHandling("Masukkan data lama yang ingin diajukan perubahan : ", permohonan.dataLama, 2);
    inputHandling("Masukkan data baru yang diinginkan : ", permohonan.dataBaru, 2);
    inputHandling("Alasan perubahan : ", permohonan.alasan, 2);
    permohonan.status = "Waiting";
    dataPermohonan[jumlahPermohonan++] = permohonan;
    exportData(pathDbDefault, 2);
    cout << ">> Permohonan perubahan berhasil diajukan, harap menunggu maksimal 300 hari kerja\n";
    system("pause");
}

/**
 * @brief memproses seluruh permohonan perubahan data yang berstatus "Waiting"
 */
void prosesPermohonanData()
{
    importDataTxt(pathDbDefault, 2);
    for (int i = 0; i < jumlahPermohonan; i++)
    {
        if (dataPermohonan[i].status == "Waiting")
        {
            system("cls");
            cout << ">>Permohonan -" << i + 1 << "\n";
            cout << "NIS        : " << dataPermohonan[i].NIS << endl;
            cout << "Data Lama  : " << dataPermohonan[i].dataLama << endl;
            cout << "Data Baru  : " << dataPermohonan[i].dataBaru << endl;
            cout << "Alsan      : " << dataPermohonan[i].alasan << endl;

            short decision;
            inputHandling("1. Acc\n2. Tolak\nPilihan : ", decision);

            if (decision == 1)
            {
                int idx = binarySearchRecursive(dataAcc, 0, jumlahAcc - 1, dataPermohonan[i].NIS);
                if (idx != -1)
                {
                    bool cocok = false;
                    if (dataAcc[idx].namaLengkap == dataPermohonan[i].dataLama)
                    {
                        dataAcc[idx].namaLengkap = dataPermohonan[i].dataBaru;
                        cocok = true;
                    }
                    else if (dataAcc[idx].tanggalLahir == dataPermohonan[i].dataLama)
                    {
                        dataAcc[idx].tanggalLahir = dataPermohonan[i].dataBaru;
                        cocok = true;
                    }
                    else if (dataAcc[idx].namaAyah == dataPermohonan[i].dataLama)
                    {
                        dataAcc[idx].namaAyah = dataPermohonan[i].dataBaru;
                        cocok = true;
                    }
                    else if (dataAcc[idx].namaIbu == dataPermohonan[i].dataLama)
                    {
                        dataAcc[idx].namaIbu = dataPermohonan[i].dataBaru;
                        cocok = true;
                    }
                    else if (dataAcc[idx].kelas == dataPermohonan[i].dataLama)
                    {
                        dataAcc[idx].kelas = dataPermohonan[i].dataBaru;
                        cocok = true;
                    }
                    if (cocok)
                    {
                        dataPermohonan[i].status = "Acc";
                        exportData(pathDbDefault, 1);
                    }
                    else
                    {
                        cout << "Data lama tidak cocok, perunahan tidak dapat dilakukan\n";
                        system("pause");
                    }
                }
                else
                {
                    cout << ">> NIS tidak ditemukan, permohonan diskip\n";
                    system("pause");
                }
            }
            else if (decision == 2)
            {
                dataPermohonan[i].status = "Rejected";
            }
        }
        exportData(pathDbDefault, 2);
        importDataTxt(pathDbDefault, 2);
    }

    cout << ">> Semua permohonan telah diproses\n";
    system("pause");
}

/**
 * @brief Menu yang bersifat looping
 *
 * @param menuAdminStat admin stat yang ada di login
 * @param menuLoginStat login stat yang mengatur perulangan
 */
void menu(bool &menuAdminStat, bool &menuLoginStat, int usernameIn, int indexAcc = -1)
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
            case 4:
            {
                prosesPermohonanData();
                break;
            }
            case 0:
            {
                menuAdminStat = false;
                menuLoginStat = false;
                break;
            }

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
            system("cls");
            cout << ">> Selamat Datang Di Menu Client" << endl;
            cout << dataAcc[indexAcc].namaLengkap << " (" << dataAcc[indexAcc].NIS << ")" << endl;
            // inputHandling(" mau yang mana menunya (exit = 0)", opt);
            inputHandling("1. Lihat Biodata Siswa\n2. Ajukan Perubahan Data\nPilih menu (exit = 0): ", opt);
            cout << "Masuk ke menu orang hutan" << endl;
            switch (opt)
            {
            case 1:
                displayData(dataAcc, jumlahAcc, 2, true, dataAcc[indexAcc].NIS);
                break;
            case 2:
                ajukanPermohonanPerubahanData(dataAcc[indexAcc].NIS, indexAcc);
                break;
            case 0:
                menuLoginStat = false;
                break;

            default:
                cout << "Pilihan tidak valid\n";
                break;
            }
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
            menu(adminStat, loginStat, usernameIn);
        }
        else
        {
            importDataTxt(pathDbDefault, 1);
            int left = 0, right = jumlahAcc - 1;
            int indexAcc = binarySearchRecursive(dataAcc, left, right, usernameIn);
            if (indexAcc == -1)
            {
                cout << "Data Tidak Ditemukan Bolo pindah file atau cari lagi sana" << endl;
                loginStat = false;
                system("pause");
            }
            else
            {
                bool statPassLoop = false;
                do
                {
                    system("cls");
                    cout << "Hai " << dataAcc[indexAcc].NIS << " !!!" << endl;
                    inputHandling("Apa Passwordmu? : ", passwordIn, 1);
                    if (passwordIn != dataAcc[indexAcc].password)
                    {
                        statPassLoop = true;
                    }
                    else
                    {
                        statPassLoop = false;
                        loginStat = true;
                        menu(adminStat, loginStat, usernameIn, indexAcc);
                    }
                } while (statPassLoop == true);
            }
        }
    }
}

/**
 * @brief fungsi utama program. Menampilkan menu login dan memulai program
 *
 * @param argc Jumlah argumen command-line
 * @param argv Array argumen command-line
 * @return int status out program
 */
int main(int argc, char const *argv[])
{
    cout << "Program Manajemen Sekolah" << endl;
    cout << "By Erlan and Roi" << endl;
    system("pause");
    login();
    return 0;
}
