#include <iostream>

using namespace std;

// Struktur node untuk menyimpan cucian yang masih dalam antrean.
struct nodeQueue {
    int nota;
    string nama;
    nodeQueue* next;
};

// Struktur node untuk menyimpan riwayat cucian yang sudah selesai.
struct nodeStack {
    int nota;
    string nama;
    nodeStack* next;
};

// Struktur node untuk menyimpan cucian pada rak menggunakan BST.
struct nodeTree {
    int nota;
    string nama;
    nodeTree* kiri;
    nodeTree* kanan;
};

// Pointer utama untuk masing-masing penyimpanan data.
nodeQueue* front = NULL;
nodeQueue* last = NULL;
nodeStack* top = NULL;
nodeTree* root = NULL;

// Menambahkan data cucian ke BST berdasarkan nomor nota.
// Nota lebih kecil ditempatkan di kiri, sedangkan nota lebih besar di kanan.
nodeTree* tambah_tree(nodeTree* akar, int nota, string nama) {
    if (akar == NULL) {
        return new nodeTree{nota, nama, NULL, NULL};
    }

    if (nota < akar->nota) {
        akar->kiri = tambah_tree(akar->kiri, nota, nama);
    } else {
        akar->kanan = tambah_tree(akar->kanan, nota, nama);
    }

    return akar;
}

// Mencari data cucian di BST menggunakan nomor nota.
nodeTree* cari_tree(nodeTree* akar, int nota) {
    if (akar == NULL || akar->nota == nota) {
        return akar;
    }

    if (nota < akar->nota) {
        return cari_tree(akar->kiri, nota);
    }

    return cari_tree(akar->kanan, nota);
}

// Menghapus data dari BST dengan menangani tiga kondisi:
// node tanpa anak, node dengan satu anak, dan node dengan dua anak.
nodeTree* hapus_tree(nodeTree* akar, int nota) {
    if (akar == NULL) {
        return NULL;
    }

    if (nota < akar->nota) {
        akar->kiri = hapus_tree(akar->kiri, nota);
    } else if (nota > akar->nota) {
        akar->kanan = hapus_tree(akar->kanan, nota);
    } else if (akar->kiri == NULL) {
        nodeTree* pengganti = akar->kanan;
        delete akar;
        return pengganti;
    } else if (akar->kanan == NULL) {
        nodeTree* pengganti = akar->kiri;
        delete akar;
        return pengganti;
    } else {
        // Mengambil node paling kiri dari subtree kanan sebagai pengganti.
        nodeTree* pengganti = akar->kanan;

        while (pengganti->kiri != NULL) {
            pengganti = pengganti->kiri;
        }

        akar->nota = pengganti->nota;
        akar->nama = pengganti->nama;
        akar->kanan = hapus_tree(akar->kanan, pengganti->nota);
    }

    return akar;
}

// Menampilkan data BST dengan urutan node, kiri, kemudian kanan.
void preorder(nodeTree* akar, int& no) {
    if (akar == NULL) {
        return;
    }

    cout << no << ". [" << akar->nota << "] " << akar->nama << endl;
    no++;

    preorder(akar->kiri, no);
    preorder(akar->kanan, no);
}

// Menampilkan data BST dengan urutan kiri, node, kemudian kanan.
void inorder(nodeTree* akar, int& no) {
    if (akar == NULL) {
        return;
    }

    inorder(akar->kiri, no);

    cout << no << ". [" << akar->nota << "] " << akar->nama << endl;
    no++;

    inorder(akar->kanan, no);
}

// Menampilkan data BST dengan urutan kiri, kanan, kemudian node.
void postorder(nodeTree* akar, int& no) {
    if (akar == NULL) {
        return;
    }

    postorder(akar->kiri, no);
    postorder(akar->kanan, no);

    cout << no << ". [" << akar->nota << "] " << akar->nama << endl;
    no++;
}

// Memasukkan cucian baru ke bagian belakang antrean.
// Memeriksa nomor nota dan nama cucian sebelum data disimpan.
void tambah_cucian() {
    int nota;
    string nama;

    cout << "Masukkan nomor nota: ";
    cin >> nota;

    if (cin.fail() || nota <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor nota harus berupa angka lebih dari 0!" << endl << endl;
        return;
    }

    // Memeriksa apakah nomor nota sudah digunakan di antrean.
    nodeQueue* cekQueue = front;

    while (cekQueue != NULL) {
        if (cekQueue->nota == nota) {
            cout << "Nomor nota sudah digunakan!" << endl << endl;
            return;
        }
        cekQueue = cekQueue->next;
    }

    // Memeriksa apakah nomor nota sudah digunakan di riwayat.
    nodeStack* cekStack = top;

    while (cekStack != NULL) {
        if (cekStack->nota == nota) {
            cout << "Nomor nota sudah digunakan!" << endl << endl;
            return;
        }
        cekStack = cekStack->next;
    }

    cout << "Masukkan nama cucian: ";
    cin.ignore();
    getline(cin, nama);

    if (nama.empty()) {
        cout << "Nama cucian tidak boleh kosong!" << endl << endl;
        return;
    }

    // Membuat node baru dan memasukkannya ke belakang Queue.
    nodeQueue* dataBaru = new nodeQueue{nota, nama, NULL};

    if (front == NULL) {
        front = last = dataBaru;
    } else {
        last->next = dataBaru;
        last = dataBaru;
    }

    cout << "Berhasil memasukkan cucian!" << endl << endl;
}

// Menampilkan seluruh data cucian yang masih berada dalam antrean.
void lihat_antrean() {
    if (front == NULL) {
        cout << "Antrean cucian masih kosong!" << endl << endl;
        return;
    }

    cout << "=== Cucian Dalam Antrean ===" << endl << endl;

    nodeQueue* data = front;
    int nomor = 1;

    while (data != NULL) {
        cout << nomor << ". [" << data->nota << "] " << data->nama << endl;
        data = data->next;
        nomor++;
    }

    cout << endl;
}

// Memproses cucian paling depan dari Queue.
// Data kemudian dipindahkan ke Stack dan BST.
void proses_cucian() {
    if (front == NULL) {
        cout << "Antrean cucian masih kosong!" << endl << endl;
        return;
    }

    nodeQueue* dataHapus = front;

    // Memasukkan data ke posisi paling atas Stack.
    nodeStack* dataBaru = new nodeStack{
        dataHapus->nota, dataHapus->nama, top
    };

    top = dataBaru;

    // Menyimpan data cucian ke dalam rak BST.
    root = tambah_tree(root, dataHapus->nota, dataHapus->nama);

    cout << "Berhasil memproses cucian " << dataHapus->nama << "!" << endl << endl;

    // Menghapus data dari bagian depan Queue.
    front = dataHapus->next;

    if (front == NULL) {
        last = NULL;
    }

    delete dataHapus;
}

// Menampilkan riwayat cucian yang telah selesai diproses.
void lihat_riwayat() {
    if (top == NULL) {
        cout << "Belum ada cucian yang selesai!" << endl << endl;
        return;
    }

    cout << "=== Cucian Yang Sudah Selesai ===" << endl << endl;

    nodeStack* data = top;
    int nomor = 1;

    while (data != NULL) {
        cout << nomor << ". [" << data->nota << "] " << data->nama << endl;
        data = data->next;
        nomor++;
    }

    cout << endl;
}

// Mencari cucian yang berada di rak berdasarkan nomor nota.
void cari_cucian() {
    if (root == NULL) {
        cout << "Rak masih kosong!" << endl << endl;
        return;
    }

    int nota;

    cout << "Masukkan nomor nota: ";
    cin >> nota;

    if (cin.fail() || nota <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor nota harus berupa angka lebih dari 0!" << endl << endl;
        return;
    }

    nodeTree* dataHasil = cari_tree(root, nota);

    if (dataHasil == NULL) {
        cout << "Cucian dengan nota " << nota << " tidak ada di rak!" << endl << endl;
        return;
    }

    cout << "Ditemukan: [" << dataHasil->nota << "] "
         << dataHasil->nama << endl << endl;
}

// Menampilkan isi rak berdasarkan metode traversal BST yang dipilih.
void lihat_rak() {
    if (root == NULL) {
        cout << "Rak masih kosong!" << endl << endl;
        return;
    }

    char pilihan;
    int nomor = 1;

    cout << "=== Isi Rak Cucian ===" << endl << endl;
    cout << "1. Pre-order" << endl;
    cout << "2. In-order" << endl;
    cout << "3. Post-order" << endl << endl;

    cout << "Masukkan pilihan anda: ";
    cin >> pilihan;
    cout << endl;

    switch (pilihan) {
        case '1':
            preorder(root, nomor);
            break;

        case '2':
            inorder(root, nomor);
            break;

        case '3':
            postorder(root, nomor);
            break;

        default:
            cout << "Pilihan yang anda masukkan salah!" << endl;
            break;
    }

    cout << endl;
}

// Mengambil cucian dari rak dan menghapus datanya dari BST.
// Riwayat pada Stack tetap disimpan.
void ambil_cucian() {
    if (root == NULL) {
        cout << "Rak masih kosong!" << endl << endl;
        return;
    }

    int nota;

    cout << "Masukkan nomor nota: ";
    cin >> nota;

    if (cin.fail() || nota <= 0) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Nomor nota harus berupa angka lebih dari 0!" << endl << endl;
        return;
    }

    nodeTree* dataHasil = cari_tree(root, nota);

    if (dataHasil == NULL) {
        cout << "Cucian dengan nota " << nota << " tidak ada di rak!" << endl << endl;
        return;
    }

    cout << "Cucian " << dataHasil->nama
         << " telah diambil pelanggan!" << endl << endl;

    root = hapus_tree(root, nota);
}

// Program utama untuk menjalankan seluruh menu manajemen laundry.
int main() {
    char pilihan = '0';

    while (pilihan != '8') {
        cout << "=== Manajemen Laundry Rani ===" << endl << endl;
        cout << "1. Tambah Cucian" << endl;
        cout << "2. Lihat Cucian Dalam Antrean" << endl;
        cout << "3. Proses Cucian" << endl;
        cout << "4. Lihat Cucian Yang Sudah Selesai" << endl;
        cout << "5. Cari Cucian Di Rak" << endl;
        cout << "6. Lihat Isi Rak" << endl;
        cout << "7. Ambil Cucian" << endl;
        cout << "8. Keluar" << endl << endl;

        cout << "Masukkan pilihan anda: ";
        cin >> pilihan;

        switch (pilihan) {
            case '1': {
                char tambahLagi = 'y';

                while (tambahLagi == 'y' || tambahLagi == 'Y') {
                    tambah_cucian();

                    cout << "Apakah anda ingin menambahkan cucian lagi? (y/n): ";
                    cin >> tambahLagi;
                }
                break;
            }

            case '2':
                lihat_antrean();
                break;

            case '3':
                proses_cucian();
                break;

            case '4':
                lihat_riwayat();
                break;

            case '5':
                cari_cucian();
                break;

            case '6':
                lihat_rak();
                break;

            case '7':
                ambil_cucian();
                break;

            case '8':
                cout << "Program akan dikeluarkan.";
                exit(0);

            default:
                cout << "Pilihan yang anda masukkan salah!" << endl;
                cout << "Silahkan coba lagi." << endl << endl;
        }
    }

    return 0;
}
