#include <iostream>
#include <fstream>
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
                while (!openFileTxt.eof())
                {
                    getline(openFileTxt, barisData);
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
 *
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
            for (int i = 0; i < jumlahAcc; i++)
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
                exportDataAcc << "," << barisDataOut << ";\n";
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

void login()
{
    string usernameIn;
    inputHandling("Masukkan Username : ", usernameIn, 1);
    if (usernameIn == "exit")
    {
        return;
    }
    else
    {
    }
}

int main(int argc, char const *argv[])
{
    // string coba = "240xxxxx,16870,YuhuJokowi,Joko Wi Dek Tok,11/11/2005,Mulyadi,Ismayarti,IF-D,2018;";
    // string coba2 = "16870,aku pingin tanggal 2018,2018 jadi hehehe,ya gitu deh males aku,ACC;";
    // string cobaArr[9];
    importDataTxt(pathDbDefault, 1);
    // Membuat dummy data pada struct dataAcc
    // dataAcc[0] = {"1234567890", 10001, "password1", "Budi Santoso", "01/01/2005", "Pak Budi", "Bu Budi", "X-A", 2020};
    // dataAcc[1] = {"0987654321", 10002, "password2", "Siti Aminah", "02/02/2006", "Pak Siti", "Bu Siti", "XI-B", 2021};
    // jumlahAcc = 2;
    exportData(pathDbDefault, 1);

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
    return 0;
}
