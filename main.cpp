#include <iostream>
#include <conio.h>//buat getche
#include <cstdlib>
#include <ctime>
#include <fstream>//untuk file handling
#include <filesystem>//untuk folder(directory)
#include <cmath>

//namespaces
using namespace std;
using namespace filesystem;

//structs
struct stats{
    int hp, physical_atk, magical_atk, physical_def, magical_def, lvl;
};

struct item{
    //0,1,2 berarti armor, 3 berarti senjata
    int tipe;
    string nama;
    stats stat_i;
};

//equipment slot
struct e_slot
{
    int idx;
    item equipment;
};

struct hambali{
    int exp;
    stats stat_p;
    //idx 0,1,2 berarti helm, armor, legging  3 berarti senjata
    e_slot slot[4];
    string nama;
    path save_file;//untuk save file
    item inventory[50];
    int jumlahitem = 0;//Jumlah item sekarang
    //buat save system
    int lantai;
    int hp_musuh;
};

struct monster{
    int tipe;
    stats stat_m;
};

hambali player;

//armor & weapon untuk item
item helm[5]={
    {0,"helm copper", {0,0,0,5,0,1}},
    {0,"helm silver", {0,0,0,10,0,2}},
    {0,"helm gold", {0,0,0,15,0,3}},
    {0,"helm platinum", {0,0,0,20,0,4}},
    {0,"helm mythril", {0,0,0,25,0,5}}
};

item armor[5]={
    {1,"armor copper", {0,0,0,10,0,1}},
    {1,"armor silver", {0,0,0,20,0,2}},
    {1,"armor gold", {0,0,0,30,0,3}},
    {1,"armor platinum", {0,0,0,40,0,4}},
    {1,"armor mythril", {0,0,0,50,0,5}}
};

item legging[5]={
    {2,"legging copper", {0,0,0,5,0,1}},
    {2,"legging silver", {0,0,0,10,0,2}},
    {2,"legging gold", {0,0,0,15,0,3}},
    {2,"legging platinum", {0,0,0,20,0,4}},
    {2,"legging mythril", {0,0,0,25,0,5}}
};

item senjata[2]={
    {3,"pedang copper", {0,10,0,0,0,1}},
    {3,"pedang silver", {0,20,0,0,0,2}},
};

//TEMP functions
void cek(string param){//function buat ngeprint suatu string
    cout<<param<<'\n';
}

void cek(int param){//function buat ngeprint suatu int
    cout<<param<<'\n';
}


//functions
void pause(){//buat ngepause
    cout<<"Tekan tombol apapun untuk lanjut\n";
    getche();
}

int serang(stats stat, int tipe){
    srand(time(0));
    int variasi = rand() % 11;
    if(rand()%2 == 1){
        variasi *= -1;
    }

    if(tipe == 1){
        return stat.physical_atk + variasi;
    }else if(tipe == 2){
        return stat.magical_atk + variasi;
    }
    return 0;
}

int berlindung(stats stat, int tipe){
    //sudah ditambah dengan base stat
    srand(time(0));
    if(tipe == 1){
        return (rand()%stat.physical_def) + stat.physical_def;
    }else if(tipe == 2){
        return (rand()%stat.magical_def) + stat.magical_def;
    }
    return 0;
}

void cek_stat(stats stat){
    cout<<"Level : " <<stat.lvl<<'\n';
    cout<<"Hp : " <<stat.hp<<'\n';
    cout<<"Physical attack : "<<stat.physical_atk<<'\n';
    cout<<"Physical defense : "<<stat.physical_def<<'\n';
    cout<<"Magical attack : "<<stat.magical_atk<<'\n';
    cout<<"Magical defense : "<<stat.magical_def<<'\n';
    cout<<"-------------------------------------------------\n";
    pause();
    system("cls");
}

void isi_inventory(){
    if(player.jumlahitem == 0){
        cout<<"Inventory 0\n";
        return;
    }
    for (int i = 0; i < player.jumlahitem; i++){
        cout<<i+1 << "." << player.inventory[i].nama << " (Lvl "<< player.inventory[i].stat_i.lvl << ")\n";
    }
    
}

bool buang_item(int n){
    if(n < 0 || n >= player.jumlahitem){
        cout<<"Tidak ada item pada no tersebut\n";
        return false;
    }
    for(int i = n; i < player.jumlahitem - 1; i++){
        player.inventory[i] = player.inventory[i+1];
    }
    player.jumlahitem--;
    return true;
}

bool tambah_item(item baru){
    
    if (player.jumlahitem >= 50){
        cout<<"Inventory Penuh\n";
        return false;
    }
    player.inventory[player.jumlahitem] = baru;
    player.jumlahitem++;
    cout<<baru.nama << " telah di tambahkan\n";
    return true;
}

void menu_ambil_buang(item barang){
    int pilih;
    cout<<"Anda mendapatkan item " << barang.nama <<"!\n";
    cout<<"-------------------------------------------------\n";
    cout<<"Stat Item :\n";
    cout<<"Hp : " << barang.stat_i.hp << "\n";
    cout<<"Physical Attack : " << barang.stat_i.physical_atk << "\n";
    cout<<"Magical Attack : " << barang.stat_i.magical_atk << "\n";
    cout<<"Physical Defense : " << barang.stat_i.physical_def << "\n";
    cout<<"Magical Defense : " << barang.stat_i.magical_def << "\n";
    cout<<"Level : " << barang.stat_i.lvl << "\n";
    cout<<"-------------------------------------------------\n";
    cout<<"Pilih aksi anda\n";
    cout<<"1. Simpan Item\n";
    cout<<"2. Buang Item\n";
    cout<<": "; cin >> pilih;
    switch (pilih){
        case 1 :
            tambah_item(barang);
            break;
        case 2 :
            cout<<"Item Telah Dibuang\n";
            break;
    default:
            cout<<"Pilihan Tidak Valid\n";
            break;
    }
}

void swap_item(item &a, item &b){
    item temp = a;
    a = b;
    b = a;
}

//QUICKSORT----
int partition(item arr[], int low, int high, int mode, int urut){
    item pivot = arr[high];
    int i = low -1;

    for(int j = low; j < high; j++){
        bool kondisi = false;
        if(mode == 1){
            if(urut)
                kondisi = arr[j].nama < pivot.nama;
            else
                kondisi = arr[j].nama > pivot.nama;
        }
        else if(mode == 2){
            if(urut)
                kondisi = arr[j].stat_i.lvl < pivot.stat_i.lvl;
            else
                kondisi = arr[j].stat_i.lvl > pivot.stat_i.lvl;
        }
        if(kondisi){
            i++;
            swap_item(arr[i], arr[j]);
        }
        swap_item(arr[i+1], arr[high]);
        return(i+1);
    }
}

void quick_sort(item arr[], int low, int high, int mode, int urut){
    if(low < high){
        int pi = partition(arr, low, high, mode, urut);

        quick_sort(arr, low, pi-1, mode, urut);
        quick_sort(arr, pi+1, high, mode, urut);
    }
}

//----QUICKSORT

void menu_sort(){
    if(player.jumlahitem <= 0){
        cout<<"Inventory Kosong\n";
        return;
    }
    int tipe;
    int urutan;
    cout<<"Sorting Inventory\n";
    cout<<"1. Berdasarkan nama\n";
    cout<<"2. Berdasarkan level\n";
    cout<<": ";cin >> tipe;

    cout<<"1. Terbesar ke Terkecil\n";
    cout<<"2. Terkecil ke Terbesar\n";
    cout<<": ";cin >> urutan;

    bool urut = (urutan == 1);

    quick_sort(player.inventory, 0, player.jumlahitem - 1 , tipe, urut);
    cout<<"Inventory telah diurutkan\n";
}

// pengubah huruf kecil
string huruf_kecil(string kata){
    for(int i = 0; i < kata.length(); i++){
        kata[i] = tolower(kata[i]);
    }
    return kata;
}

void search_nama(string target, hambali &p){
    bool ketemu = false;
    string target_low = huruf_kecil(target);

    cout<<"Hasil pencarian untuk \"" << target << "\":\n";
    for(int i = 0; i < p.jumlahitem; i++){
        string nama_item = huruf_kecil(p.inventory[i].nama);
        if(nama_item.find(target_low) != string::npos){
            cout<<"Slot " << i+1 << "." << p.inventory[i].nama << " (Lvl "<< p.inventory[i].stat_i.lvl << ")\n";
        ketemu = true; }
    }
    if(!ketemu) cout<<"Item tidak ditemukan.\n";
    cout<<endl;
    cout<<"-------------------------------------------------\n";
    
}

void search_level(int target_lvl, hambali &p){
    bool ketemu = false;
    cout<<"Hasil pencarian untuk level: \"" << target_lvl << "\":\n";
    for(int i = 0; i < p.jumlahitem; i++){
        if(p.inventory[i].stat_i.lvl == target_lvl){
            cout<<"Slot " << i+1 << ". " << p.inventory[i].nama << " (Lvl "<< p.inventory[i].stat_i.lvl << ")\n";
            ketemu = true;
        }
    }
    if(!ketemu) cout<<"Item tidak ditemukan.\n";
    cout<<endl;
    cout<<"-------------------------------------------------\n";
    
}

void menu_Inventory(){
    int pilih;
    int no;
    int target_lvl;
    string nama_cari;
    while(pilih != 3){
        cout<<"Menu - Inventory\n";
        cout<<"1. Lihat Inventory\n";
        cout<<"2. Buang Item\n";
        cout<<"3. Sorting Inventory\n";
        cout<<"4. Searching Inventory (By Name)\n";
        cout<<"5. Searching Inventory (By Level)\n";
        cout<<"6. Kembali\n";
        cout<<":";cin >> pilih;
        switch (pilih){
            case 1:
                isi_inventory();
                break;
            case 2:
                isi_inventory();
                cout<<"Pilih item yang mau dibuang:";cin >> no;
                buang_item(no - 1);
                break;
            case 3:
                menu_sort();
                break;
            case 4:
                cout<<"Masukkan nama item yang ingin dicari: "; 
                cin.ignore();
                getline(cin, nama_cari);
                search_nama(nama_cari, player);
                break;
            case 5:
                cout<<"Masukkan level item yang ingin dicari: ";
                cin >> target_lvl;
                search_level(target_lvl, player);
                break;
            case 6:
                system("cls");
                break;
        default:
            break;
        }

    }
}

item jatuh_barang(){
    int jml_helm = sizeof(helm)/sizeof(helm[0]);
    int jml_armor = sizeof(armor)/sizeof(armor[0]);
    int jml_legging = sizeof(legging)/sizeof(legging[0]);
    int jml_senjata = sizeof(senjata)/sizeof(senjata[0]);
    int rarity;
    srand(time(0));
    int piece = rand() % 4;//0 helm, 1 armor, 2 legging, 3 senjata
    item barang;
    srand(time(0));
    switch(piece){
        case 0:
            rarity = rand() % jml_helm;
            barang = helm[rarity];
            break;
        case 1:
            rarity = rand() % jml_armor;
            barang = armor[rarity];
            break;
        case 2:
            rarity = rand() % jml_legging;
            barang = legging[rarity];
            break;
        case 3:
            rarity = rand() % jml_senjata;
            barang = senjata[rarity];
            break;
    }
    return barang;
}

int lantai(monster &musuh, int lantai){
    //return 0 lanjut loop lantai
    //return 1 player menang
    //return 2 player kalah
    //return 3 player ingin keluar ke main menu
    int aksi_player;
    int aksi_musuh;
    int dmg_musuh;
    int dmg_player;
    int tipe_dmg_musuh = musuh.tipe;
    int tipe_dmg_player;
    int def_musuh;
    int def_player;
    int tipe_def_musuh = musuh.tipe;
    int tipe_def_player;
    int kerusakan_musuh;
    int kerusakan_player;

    //aksi monster
    srand(time(0));
    aksi_musuh = (rand()%2)+1;
    if(aksi_musuh == 1){
        dmg_musuh = serang(musuh.stat_m, tipe_dmg_musuh);
        def_musuh = 0;
        tipe_def_musuh = 0;
    }else if(aksi_musuh == 2){
        def_musuh = berlindung(musuh.stat_m, tipe_def_musuh);
        dmg_musuh = 0;
        tipe_dmg_musuh = 0;
    }

    while(true){
        //aksi player
        cout<<"========================== Lantai "<<lantai<<" ==========================\n";
        cout<<"Anda menemui musuh di depan anda!\n";
        cout<<"-------------------------------------------------\n";
        if(aksi_musuh == 1 && tipe_dmg_musuh == 1){
            cout<<"Musuh akan menyerang dengan tipe serangan physical\n";
        }else if(aksi_musuh == 1 && tipe_dmg_musuh == 2){
            cout<<"Musuh akan menyerang dengan tipe serangan magical\n";
        }
        if(aksi_musuh == 2 && tipe_def_musuh == 1){
            cout<<"Musuh berlindung sebesar "<<def_musuh<<" physical defense\n";
        }else if(aksi_musuh == 2 && tipe_def_musuh == 2){
            cout<<"Musuh berlindung sebesar "<<def_musuh<<" magical defense\n";
        }

        cout<<"-----------------Pilih aksi anda-----------------\n";
        cout<<"1. Menyerang\n";
        cout<<"2. Berlindung\n";
        cout<<"3. Cek stat musuh\n";
        cout<<"4. Cek stat anda\n";
        cout<<"5. Buka Inventory\n";
        cout<<"6. Keluar\n";
        cout<<": ";
        cin>>aksi_player;
        cout<<"-------------------------------------------------\n";
        switch(aksi_player){
            case 1:
                cout<<"Anda akan menyerang\n";
                cout<<"Pilih tipe serangan anda\n";
                cout<<"1. Physical\n";
                cout<<"2. Magical\n";
                cout<<"3. Kembali\n";
                cout<<": ";
                cin>>tipe_dmg_player;
                switch(tipe_dmg_player){
                    case 1: 
                        dmg_player = serang(player.stat_p, tipe_dmg_player);
                        break;
                    case 2:
                        dmg_player = serang(player.stat_p, tipe_dmg_player);
                        break;
                    case 3:
                        system("cls");
                        continue;
                        break;
                }
                def_player = 0;
                tipe_def_player = 0;
                break;
            case 2:
                cout<<"Anda akan berlindung\n";
                cout<<"Pilih tipe perlindungan anda\n";
                cout<<"1. Physical\n";
                cout<<"2. Magical\n";
                cout<<"3. Kembali\n";
                cout<<": ";
                cin>>tipe_def_player;
                switch(tipe_def_player){
                    case 1:
                        def_player = berlindung(player.stat_p, tipe_def_player);
                        cout<<"Anda berlindung sebesar "<<def_player<<" physical defense\n";
                        break;
                    case 2:
                        def_player = berlindung(player.stat_p, tipe_def_player);
                        cout<<"Anda berlindung sebesar "<<def_player<<" magical defense\n";
                        break;
                    case 3:
                        system("cls");
                        continue;
                        break;
                }
                dmg_player = 0;
                tipe_dmg_player = 0;
                break;
            case 3:
                cout<<"Stat musuh :\n";
                cek_stat(musuh.stat_m);
                continue;
                break;
            case 4:
                cout<<"Stat anda :\n";
                cek_stat(player.stat_p);
                continue;
                break;
            
            case 5:
                menu_Inventory();
                continue;
                break;
            case 6:
                system("cls");
                return 3;
                break;
        }
        break;
    }
    
    //damage player ke musuh
    if(aksi_player == 1){
        if(tipe_dmg_player == tipe_def_musuh){//cek tipe
            kerusakan_musuh = dmg_player - def_musuh;
        }else{
            kerusakan_musuh = dmg_player;
        }
        if(kerusakan_musuh <= 0){//kerusakan minimalnya 1
            kerusakan_musuh = 1;
        }
        musuh.stat_m.hp -= kerusakan_musuh;
        if(tipe_dmg_player == 1){
            cout<<"Kamu memberikan damage sebesar " << kerusakan_musuh << " physical damage\n";
        }else if(tipe_dmg_player == 2){
            cout<<"Kamu memberikan damage sebesar " << kerusakan_musuh << " magical damage\n";
        }
    }

    //damage musuh ke player
    if(aksi_musuh == 1){
        if(tipe_dmg_musuh == tipe_def_player){//cek tipe
            kerusakan_player = dmg_musuh - def_player;
        }else{
            kerusakan_player = dmg_musuh;
        }
        if(kerusakan_player <= 0){//kerusakan minimalnya 1
            kerusakan_player = 1;
        }
        player.stat_p.hp -= kerusakan_player;
        if(tipe_dmg_musuh == 1){
            cout<<"Musuh memberikan damage sebesar "<<kerusakan_player<<" physical damage\n";
        }else if(tipe_dmg_musuh = 2){
            cout<<"Musuh memberikan damage sebesar "<<kerusakan_player<<" magical damage\n";
        }
    }
    
    if(musuh.stat_m.hp <= 0){
        cout<<"-------------------------------------------------\n";
        pause();
        system("cls");
        return 1;
    }
    if(player.stat_p.hp <= 0){
        cout<<"-------------------------------------------------\n";
        pause();
        system("cls");
        return 2;
    }
    cout<<"-------------------------------------------------\n";
    pause();
    system("cls");
    return 0;
}


path folder_save_file(){
    //(path itu class dari filesystem)
    //BUAT FOLDER(directory) UNTUK SAVE FILE (jika belum ada) & RETURN PATH UNTUK FOLDER(directory) SAVE FILE!!!!
    path p_save_file = current_path();//path folder program
    p_save_file.append("save_file");//path untuk folder baru
    if(!is_directory(p_save_file)){
        create_directory(p_save_file);//membuat folder baru dengan nama "save_file" jika belum ada
    }
    return p_save_file;
}

void buat_save_file_baru(){
    path p_save_file = folder_save_file();//buat folder dan path ke foldernya
    //save file
    cin.ignore();
    string nama_player;
    string nama_file;
    //nama save file
    while(true){ //loop untuk mengecek apakah ada save file dgn nama yang sama atau tidak
        cout<<"===========BUAT SAVE FILE BARU===========\n";
        path test_path = p_save_file;//path sementara untuk mengecek apakah ada file dgn nama yang sama atau tidak 
        cout<<"Masukkan nama save file\n";
        cout<<": ";
        getline(cin,nama_file);
        nama_file += ".txt";
        test_path.append(nama_file);//path untuk save file baru
        if(exists(test_path)){
            cout<<"Save file dengan nama yang sama sudah ada\n";
            cout<<"-----------------------------------------\n";
            pause();
            system("cls");
            continue;
        }
        p_save_file = test_path;
        break;
    }
    //nama player
    cout<<"Masukkan nama player\n";
    cout<<": ";
    getline(cin, nama_player);
    //create save file
    ofstream save_file(p_save_file);//membuat save file baru di folder "save_file" jk belum ada
    save_file<<nama_player;

    //alokasi poin stat
    int hp = 10, physical_atk = 10, magical_atk = 10, p_def = 10, m_def = 10, poin = 10, menu;
    while(poin > 0){
        system("cls");
        cout<<"===========BUAT SAVE FILE BARU===========\n";
        cout<<"Alokasikan poin stat anda\n";
        cout<<"-------------------------------------\n";
        cout<<"Anda memiliki "<<poin<<" poin stat\n";
        cout<<"1. Hp : "<<hp<<'\n';
        cout<<"2. Physical attack : "<<physical_atk<<'\n';
        cout<<"3. Magical attack : "<<magical_atk<<'\n';
        cout<<"4. Physical defense : "<<p_def<<'\n';
        cout<<"5. Magical defense : "<<m_def<<'\n';
        cout<<": ";
        cin>>menu;
        switch(menu){
            case 1:
                hp += 1;
                break;
            case 2:
                physical_atk += 1;
                break;
            case 3:
                magical_atk += 1;
                break;
            case 4:
                p_def += 1;
                break;
            case 5:
                m_def += 1;
                break;
        }
        poin -= 1;
    }

    //isi save file
    save_file<<'\n'<<hp<<' '<<physical_atk<<' '<<magical_atk<<' '<<p_def<<' '<<m_def<<' '<<1;
    save_file<<'\n'<<-1<<' '<<-1<<' '<<-1<<' '<<-1;
    save_file<<'\n'<<0;
    save_file<<'\n'<<1;
    save_file<<'\n'<<-1;
    system("cls");

    //overview
    cout<<"===========BUAT SAVE FILE BARU===========\n";
    cout<<"Nama : "<<nama_player<<'\n';
    cout<<"-------------------------------------\n";
    cout<<"Hp : "<<hp<<'\n';
    cout<<"Physical attack : "<<physical_atk<<'\n';
    cout<<"Magical attack : "<<magical_atk<<'\n';
    cout<<"Physical defense : "<<p_def<<'\n';
    cout<<"Magical defense : "<<m_def<<'\n';
    cout<<"-------------------------------------\n";
    save_file.close();
    cout<<"Save file berhasil dibuat\n";
    pause();
    system("cls");
}


/*
    struktur save file
    1. nama player -> string
    2. hp physical_atk magical_atk physical_def magical_def lvl -> integer
    3. helm baju legging senjata -> integer(-1 kalo g ada equipment)
    4. exp -> integer
    5. lantai -> integer
    6. hp_musuh -> integer(-1 kalo masih save file baru)
*/

void save(path sf_path){
    ofstream save_file(sf_path, ofstream::trunc | ofstream::out);
    save_file<<player.nama;
    save_file<<'\n'<<player.stat_p.hp<<' '<<player.stat_p.physical_atk<<' '<<player.stat_p.magical_atk<<' '<<player.stat_p.physical_def<<' '<<player.stat_p.magical_def<<' '<<player.stat_p.lvl;
    save_file<<'\n'<<player.slot[0].idx<<' '<<player.slot[1].idx<<' '<<player.slot[2].idx<<' '<<player.slot[3].idx;
    save_file<<'\n'<<player.exp;
    save_file<<'\n'<<player.lantai;
    save_file<<'\n'<<player.hp_musuh;
    cout<<"Save berhasil\n";
    pause();
}

void muat_sf(path sf_path){
    //inisialisasi player
    int e_idx;
    ifstream save_file(sf_path);
    save_file>>player.nama;
    save_file>>player.stat_p.hp;
    save_file>>player.stat_p.physical_atk;
    save_file>>player.stat_p.magical_atk;
    save_file>>player.stat_p.physical_def;
    save_file>>player.stat_p.magical_def;
    save_file>>player.stat_p.lvl;
    for(int i = 0; i < 4; i++){
        save_file>>e_idx;
        if(e_idx < 0){
            player.slot[i].idx = -1;
        }else{
            player.slot[i].idx = e_idx;
            switch(i){
                case 0:
                    player.slot[i].equipment = helm[e_idx];
                    break;
                case 1:
                    player.slot[i].equipment = armor[e_idx];
                    break;
                case 2:
                    player.slot[i].equipment = legging[e_idx];
                    break;
                case 3:
                    player.slot[i].equipment = senjata[e_idx];
                    break;
            }
        }
    }
    save_file>>player.exp;
    save_file>>player.lantai;
    save_file>>player.hp_musuh;
    player.save_file = sf_path;
    save_file.close();
    cout<<"Save file berhasil dimuat\n";
    cout<<"-------------------------------------------------------------------\n";
    pause();
    system("cls");
}

int aksi_sf(path sf_path){
    //return 1 berarti lanjut ke loop lantai(sf berhasil dimuat)
    //return 2 berarti balik ke loop list sf
    //return 3 berarti lanjut loop aksi sf
    int menu;
    char hapus;
    cout<<"-------------------Pilih aksi anda-------------------\n";
    cout<<"1. Muat save file\n";
    cout<<"2. Hapus save file\n";
    cout<<"3. Kembali\n";
    cout<<": ";
    cin>>menu;
    switch(menu)
    {
        case 1:
            muat_sf(sf_path);
            return 1;
            break;
        case 2:
            cout<<"-----------------------------------------------------\n";
            cout<<"Apakah anda yakin ingin menghapus\nsave file ini? (y/n) : ";
            cin>>hapus;
            switch(hapus){
                case 'y':
                    remove(sf_path);
                    cout<<"Save file berhasil dihapus\n";
                    cout<<"-------------------------------------------------------------------\n";
                    pause();
                    system("cls");
                    return 2;
                    break;
                case 'n':
                    system("cls");
                    return 3;
                    break;
            }
            break;
        case 3:
            system("cls");
            return 2;
            break;
    }
    return 2;
}

int list_save_file(){
    //return 1 berarti save file berhasil di muat
    //return 2 berarti balik ke menu save file
    //return 3 berarti lanjut loop list save file
    int menu;
    int counter = 1;
    path p_folder = folder_save_file();
    cout<<"--------------Pilih Save File Anda--------------\n";
    cout<<"1. Kembali\n";
    cout<<"------------------------------------------------\n";
    if(std::filesystem::is_empty(p_folder)){//ambiguous njir jadinya pke std::filesystem::
        cout<<"Belum ada save file\n";
    }
    for(auto const& save_file : directory_iterator(p_folder)){//namanya range based for loop klo penasaran
        string nama_file = save_file.path().stem().string();
        cout<<counter + 1<<". "<<nama_file<<'\n';
        counter += 1;
    }
    path p_save_file[counter - 1];
    counter = 0;
    for(auto const& save_file : directory_iterator(p_folder)){
        p_save_file[counter] = save_file.path();
        counter += 1;
    }
    cout<<": ";
    cin>>menu;
    if(menu == 1){
        system("cls");
        return 2;
    }else{
        system("cls");
        while(true){
            int rv_aksi_sf = aksi_sf(p_save_file[menu - 2]);
            if(rv_aksi_sf == 1){
                return 1;
            }else if(rv_aksi_sf == 2){
                return 3;
            }else if(rv_aksi_sf == 3){
                continue;
            }
        }
    }
    return 3;
}

int menu_save_file(){
    //return 1 berarti lanjut ke loop lantai(save file berhasil dimuat)
    //return 2 berarti kembali ke main menu
    //return 3 berarti lanjut loop menu save file
    int menu;
    //save file
    cout<<"--------------Menu Save File--------------\n";
    cout<<"1. Buat save file baru\n";
    cout<<"2. List save file\n";
    cout<<"3. Kembali\n";
    cout<<": ";
    cin>>menu;

    switch(menu){
        case 1:
            system("cls");
            buat_save_file_baru();
            return 3;
            break;
        case 2:
            system("cls");
            while(true)
            {
                int rv_list_sf = list_save_file();
                if(rv_list_sf == 1){
                    return 1;
                }else if(rv_list_sf == 2){
                    return 3;
                }else if(rv_list_sf == 3){
                    continue;
                }
            }
            break;
        case 3:
            system("cls");
            return 2;
            break;
    }
    return 3;
}

int main_menu(){
    //return 1 berarti keluar loop lanjut ke loop lantai
    //return 2 berarti keluar loop keluar program
    //return 3 berarti lanjut loop main menu
    //menu
    int menu;
    cout<<"=================DUNGEON CRAWLER================\n";
    cout<<"1. Main\n";
    cout<<"2. Keluar\n";
    cout<<": ";
    cin>>menu;
    system("cls");
    switch(menu){
        case 1:
            while(true){
                int rv_menu_sf = menu_save_file();
                if(rv_menu_sf == 1){
                    return 1;
                }else if(rv_menu_sf == 2){
                    return 3;
                }else if(rv_menu_sf == 3){
                    continue;
                }
            }
            break;
        case 2:
            return 2;
            break;
        case 3:
            
            cek(sizeof(helm));
            cek(sizeof(helm[0]));
            cek(sizeof(helm)/sizeof(helm[0]));
            getche();
            system("cls");
            return 3;
            break;
    }
    return 3;
}

int main(){
    //game loop
    while(true){
        bool init;
        int lvl_lantai;
        //loop main menu
        while(true){
            int rv_main_menu = main_menu();
            if(rv_main_menu == 1){
                lvl_lantai = player.lantai;
                init = true;
                break;
            }else if(rv_main_menu == 2){
                return 0;
            }else if(rv_main_menu == 3){
                continue;
            }
        }
        //loop main
        while(true){
            char s;
            int rv_lantai;
            //inisialisasi musuh
            monster musuh;
            musuh.stat_m.lvl = lvl_lantai * 2;
            int lvl_musuh = musuh.stat_m.lvl;
            if(init){
                if(player.hp_musuh <= 0){
                    musuh.stat_m.hp = lvl_musuh * 10;
                    init = false;
                }else{
                    musuh.stat_m.hp = player.hp_musuh;
                    init = false; 
                }
            }else{
                musuh.stat_m.hp = lvl_musuh * 10;
            }
            musuh.stat_m.physical_atk = lvl_musuh * 5;
            musuh.stat_m.physical_def = lvl_musuh * 10;
            musuh.stat_m.magical_atk = lvl_musuh * 10;
            musuh.stat_m.magical_def = lvl_musuh * 5;
            srand(time(0));
            musuh.tipe = (rand()%2)+1;
            //loop lantai
            while(true){
                rv_lantai = lantai(musuh, lvl_lantai);
                if(rv_lantai != 0){
                    break;
                }
            }
            if(rv_lantai == 1){
                //logika klo menang disini
                item barang;
                //TEMP
                cout<<"=================================================\n";
                cout<<"Anda Menang!\n";
                cout<<"-------------------------------------------------\n";
                if((10+lvl_lantai) >= (rand()%(100 + (lvl_lantai * 2)))){//rumusnya probabilitasnya adalah (10 + lvl_lantai)/(10 + lvl_lantai * 2)
                    cout<<"Musuh menjatuhkan barang!\n";
                    barang = jatuh_barang();//sukses dpt barang
                    menu_ambil_buang(barang);
                    cout<<"-------------------------------------------------\n";
                }
                pause();
                system("cls");
                lvl_lantai += 1;
                continue;
            }else if(rv_lantai == 2){
                //logika klo kalah disini
                //TEMP
                cout<<"Anda kalah!\n";
                pause();
                system("cls");
                lvl_lantai = 1;
                player.stat_p.hp = 10;
                continue;
            }else if(rv_lantai == 3){
                player.hp_musuh = musuh.stat_m.hp;
                player.lantai = lvl_lantai;
                cout<<"Apakah anda ingin save progress anda? (y/n) : ";
                cin>>s;
                if(s == 'y'){
                    save(player.save_file);
                }
                system("cls");
                break;
            }
        }
    }
}