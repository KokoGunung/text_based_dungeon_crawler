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
    int vita, physical_atk, magical_atk, physical_def, magical_def, lvl;
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
    bool kosong;
    item equipment;
};

struct hambali{
    int hp;
    int exp = 0;//exp player
    int max_xp;//batas exp untuk naik level
    stats b_stat_p;//base stat
    stats t_stat_p;//total stat
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
    int hp;
    int tipe;
    stats stat_m;
};

//Variabel global
hambali player;

//tier item buat physical
/*
    1. Copper
    2. Iron
    3. Tungsten
    4. Platinum
    5. Iridium
    6. Mythril
    7. Adamantium
    8. Uranium
*/

//tier item buat magical
/*
    1. Brass
    2. Silver
    3. Gold
    4. Demonite
    5. Hellstone
    6. Orichalcum
    7. Brimstone
    8. Luminite
*/


//Template armor & weapon untuk item
item helm[16]={
    //Physical
    {0,"Helm Copper",{2, 1, 0, 3, 3}},
    {0,"Helm Iron",{4, 2, 0, 6, 6}},
    {0,"Helm Tungsten",{8, 4, 0, 12, 12}},
    {0,"Helm Platinum",{16, 8, 0, 24, 24}},
    {0,"Helm Iridium",{32, 16, 0, 48, 48}},
    {0,"Helm Mythril",{64, 32, 0, 96, 96}},
    {0,"Helm Adamantium",{128, 64, 0, 192, 192}},
    {0,"Helm Uranium",{256, 128, 0, 384, 384}},
    //Magical
    {0,"Helm Brass",{1, 0, 4, 2, 2}},
    {0,"Helm Silver",{2, 0, 8, 4, 4}},
    {0,"Helm Gold",{4, 0, 16, 8, 8}},
    {0,"Helm Demonite",{8, 0, 32, 16, 16}},
    {0,"Helm Hellstone",{16, 0, 64, 32, 32}},
    {0,"Helm Orichalcum",{32, 0, 128, 64, 64}},
    {0,"Helm Brimstone",{64, 0, 256, 128, 128}},
    {0,"Helm Luminite",{128, 0, 512, 256, 256}}
};

item armor[16]={
    //physical
    {1,"Armor Copper",{5, 3, 0, 7, 7}},
    {1,"Armor Iron",{11, 7, 0, 15, 15}},
    {1,"Armor Tungsten",{23, 15, 0, 31, 31}},
    {1,"Armor Platinum",{47, 31, 0, 63, 63}},
    {1,"Armor Iridium",{95, 63, 0, 127, 127}},
    {1,"Armor Mythril",{191, 127, 0, 255, 255}},
    {1,"Armor Adamantium",{383, 255, 0, 511, 511}},
    {1,"Armor Uranium",{767, 511, 0, 1023, 1023}},
    //magical
    {1,"Armor Brass",{3, 0, 8, 4, 4}},
    {1,"Armor Silver",{7, 0, 17, 9, 9}},
    {1,"Armor Gold",{15, 0, 35, 19, 19}},
    {1,"Armor Demonite",{31, 0, 71, 39, 39}},
    {1,"Armor Hellstone",{63, 0, 143, 79, 79}},
    {1,"Armor Orichalcum",{127, 0, 287, 159, 159}},
    {1,"Armor Brimstone",{155, 0, 575, 319, 319}},
    {1,"Armor Luminite",{311, 0, 1151, 639, 639}}
};

item legging[16]={
    //physical
    {2,"Legging Copper",{3, 2, 0, 5, 5}},
    {2,"Legging Iron",{8, 6, 0, 12, 12}},
    {2,"Legging Tungsten",{18, 14, 0, 26, 26}},
    {2,"Legging Platinum",{38, 30, 0, 54, 54}},
    {2,"Legging Iridium",{78, 62, 0, 110, 110}},
    {2,"Legging Mythril",{158, 126, 0, 222, 222}},
    {2,"Legging Adamantium",{318, 254, 0, 446, 446}},
    {2,"Legging Uranium",{638, 510, 0, 894, 894}},
    //magical
    {2,"Legging Brass",{2, 0, 6, 3, 3}},
    {2,"Legging Silver",{6, 0, 14, 8, 8}},
    {2,"Legging Gold",{14, 0, 30, 18, 18}},
    {2,"Legging Demonite",{30, 0, 62, 38, 38}},
    {2,"Legging Hellstone",{62, 0, 126, 78, 78}},
    {2,"Legging Orichalcum",{126, 0, 254, 158, 158}},
    {2,"Legging Brimstone",{254, 0, 510, 318, 318}},
    {2,"Legging Luminite",{510, 0, 1022, 638, 638}}
};

item senjata[16]={
    //physical
    {3,"Pedang Iron",{0, 7, 0, 0, 0}},
    {3,"Pedang Copper",{0, 21, 0, 0, 0}},
    {3,"Pedang Tungsten",{0, 63, 0, 0, 0}},
    {3,"Pedang Platinum",{0, 189, 0, 0, 0}},
    {3,"Pedang Iridium",{0, 567, 0, 0, 0}},
    {3,"Pedang Mythril",{0, 1701, 0, 0, 0}},
    {3,"Pedang Adamantium",{0, 5103, 0, 0, 0}},
    {3,"Pedang Uranium",{0, 15309, 0, 0, 0}},
    //magical
    {3,"Tongkat Brass",{0, 0, 14, 0, 0}},
    {3,"Tongkat Silver",{0, 0, 42, 0, 0}},
    {3,"Tongkat Gold",{0, 0, 126, 0, 0}},
    {3,"Tongkat Demonite",{0, 0, 378, 0, 0}},
    {3,"Tongkat Hellstone",{0, 0, 1134, 0, 0}},
    {3,"Tongkat Orichalcum",{0, 0, 3402, 0, 0}},
    {3,"Tongkat Brimstone",{0, 0, 10206, 0, 0}},
    {3,"Tongkat Luminite",{0, 0, 30618, 0, 0}}
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

void cek_stat(stats stat, bool no_lvl = false, bool no_pause = false, bool no_clear = false){
    if(!no_lvl){
        cout<<"Level : " <<stat.lvl<<'\n';
    }
    cout<<"-------------------------------------------------\n";
    cout<<"Vitality : " <<stat.vita<<'\n';
    cout<<"Physical attack : "<<stat.physical_atk<<'\n';
    cout<<"Physical defense : "<<stat.physical_def<<'\n';
    cout<<"Magical attack : "<<stat.magical_atk<<'\n';
    cout<<"Magical defense : "<<stat.magical_def<<'\n';
    cout<<"-------------------------------------------------\n";
    if(!no_pause){
        pause();
    }
    if(!no_clear){
        system("cls");
    }
}

void buang_item(int n){
    for(int i = n; i < player.jumlahitem - 1; i++){
        player.inventory[i] = player.inventory[i+1];
    }
    player.jumlahitem--;
}

bool tambah_item(item baru){
    if (player.jumlahitem >= 50){
        return false;
    }
    player.inventory[player.jumlahitem] = baru;
    player.jumlahitem++;
    return true;
}

void menu_ambil_buang(item item){
    int pilih;
    cout<<"Anda mendapatkan item " << item.nama <<"!\n";
    cek_stat(item.stat_i, false, true, true);
    cout<<"Pilih aksi anda\n";
    cout<<"1. Simpan Item\n";
    cout<<"2. Buang Item\n";
    cout<<": "; cin >> pilih;
    switch (pilih){
        case 1 :
            if(tambah_item(item)){
                cout<<item.nama << " telah di tambahkan\n";
            }else{
                cout<<"Inventory Penuh\n";
            }
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
    b = temp;
}

//QUICKSORT----
int partition(item arr[], int low, int high, int mode, bool urut){
    item pivot = arr[high];
    int i = low -1;

    for(int j = low; j < high; j++){
        bool kondisi = false;
        if(mode == 1){
            if(urut)
                kondisi = arr[j].nama > pivot.nama;
            else
                kondisi = arr[j].nama < pivot.nama;
        }
        else if(mode == 2){
            if(urut)
                kondisi = arr[j].stat_i.lvl > pivot.stat_i.lvl;
            else
                kondisi = arr[j].stat_i.lvl < pivot.stat_i.lvl;
        }
        if(kondisi){
            i++;
            swap_item(arr[i], arr[j]);
        }
    }
    swap_item(arr[i+1], arr[high]);
    return(i+1);
}

void quick_sort(item arr[], int low, int high, int mode, bool urut){
    if(low < high){
        int pi = partition(arr, low, high, mode, urut);

        quick_sort(arr, low, pi-1, mode, urut);
        quick_sort(arr, pi+1, high, mode, urut);
    }
}

//----QUICKSORT

void menu_sort(bool kosong){
    if(kosong){
        cout<<"Inventory Kosong\n";
        pause();
        system("cls");
        return;
    }
    int tipe;
    int urutan;
    cout<<"Urutkan Inventory\n";
    cout<<"1. Berdasarkan nama\n";
    cout<<"2. Berdasarkan level\n";
    cout<<"3. Kembali\n";
    cout<<": ";cin >> tipe;
    if(tipe == 3){
        system("cls");
        return;
    }
    if(tipe < 1 || tipe > 3){
        cout<<"Tolong pilih antara 1 sampai 3 saja!\n";
        pause();
        system("cls");
        return;
    }
    cout<<"-------------------------------------------------\n";
    cout<<"1. Terbesar ke Terkecil\n";
    cout<<"2. Terkecil ke Terbesar\n";
    cout<<"3. Kembali\n";
    cout<<": ";cin >> urutan;
    cout<<"-------------------------------------------------\n";
    if(urutan < 1 || urutan > 2){
        cout<<"Tolong pilih antara 1 sampai 2 saja!\n";
        pause();
        system("cls");
        return;
    }
    if(urutan == 3){
        system("cls");
        return;
    }
    bool urut = (urutan == 1);
    quick_sort(player.inventory, 0, player.jumlahitem - 1 , tipe, urut);
    cout<<"Inventory telah diurutkan\n";
    pause();
    system("cls");
    return;
}

// pengubah huruf kecil
string huruf_kecil(string kata){
    for(int i = 0; i < kata.length(); i++){
        kata[i] = tolower(kata[i]);
    }
    return kata;
}

int search_nama(string target, int*& arr){
    bool ketemu = false;
    string target_low = huruf_kecil(target);
    int counter = 0;
    int temp[player.jumlahitem];
    for(int i = 0; i < player.jumlahitem; i++){
        string nama_item = huruf_kecil(player.inventory[i].nama);
        if(nama_item.find(target_low) != string::npos){
            temp[counter] = i;
            counter += 1;
        ketemu = true; }
    }
    arr = new int[counter];
    if(ketemu){
        for(int j = 0; j < counter; j++){
            arr[j] = temp[j];
        }
    }
    return counter;
}

int search_level(int target_lvl, int*& arr){
    bool ketemu = false;
    int counter = 0;
    int temp[player.jumlahitem];
    for(int i = 0; i < player.jumlahitem; i++){
        if(player.inventory[i].stat_i.lvl == target_lvl){
            temp[counter] = i;
            counter += 1;
            ketemu = true;
        }
    }
    arr = new int[counter];
    if(ketemu){
        for(int j = 0; j < counter; j++){
            arr[j] = temp[j];
        }
    }
    return counter;
}

bool aksi_item(int idx){
    //return true berarti lanjut loop aksi_item
    //return false berarti keluar ke loop list_inventory
    char buang;
    int menu;
    int piece = player.inventory[idx].tipe;
    item item = player.inventory[idx];
    while(true){
        cout<<"-----------------Pilih aksi anda-----------------\n";
        cout<<"Nama : "<<item.nama<<'\n';
        cout<<"Level : " <<item.stat_i.lvl<<'\n';
        cout<<"Vitality : " <<item.stat_i.vita<<'\n';
        cout<<"Physical attack : "<<item.stat_i.physical_atk<<'\n';
        cout<<"Physical defense : "<<item.stat_i.physical_def<<'\n';
        cout<<"Magical attack : "<<item.stat_i.magical_atk<<'\n';
        cout<<"Magical defense : "<<item.stat_i.magical_def<<'\n';
        cout<<"-------------------------------------------------\n";
        cout<<"Stat item\n";
        cout<<"1. Pakai item\n";
        cout<<"2. Buang item\n";
        cout<<"3. Kembali\n";
        cout<<": ";
        cin>>menu;
        cout<<"-------------------------------------------------\n";
        switch(menu){
            case 1:
                if(!player.slot[piece].kosong){
                    player.t_stat_p.vita -= player.slot[piece].equipment.stat_i.vita;
                    player.t_stat_p.physical_atk -= player.slot[piece].equipment.stat_i.physical_atk;
                    player.t_stat_p.magical_atk -= player.slot[piece].equipment.stat_i.magical_atk;
                    player.t_stat_p.physical_def -= player.slot[piece].equipment.stat_i.physical_def;
                    player.t_stat_p.magical_def -= player.slot[piece].equipment.stat_i.magical_def;
                    player.inventory[idx] = player.slot[piece].equipment;//pindah item dari slot equipment ke inventory
                }else{
                    player.slot[piece].kosong = false;
                    buang_item(idx);//item yg dipakai bakal dihilangin dari inventory
                }
                player.slot[piece].equipment = item;//pindah item dari inventory ke slot equipment
                player.t_stat_p.vita += item.stat_i.vita;
                player.t_stat_p.physical_atk += item.stat_i.physical_atk;
                player.t_stat_p.magical_atk += item.stat_i.magical_atk;
                player.t_stat_p.physical_def += item.stat_i.physical_def;
                player.t_stat_p.magical_def += item.stat_i.magical_def;
                cout<<"Item berhasil di pakai\n";
                pause();
                system("cls");
                return true;
                break;
            case 2:
                cout<<"Apakah anda yakin? (y/n) : ";
                cin>>buang;
                if(buang != 'y'){
                    continue;
                }
                buang_item(idx);
                system("cls");
                return false;
                break;
            case 3:
                system("cls");
                return true;
                break;
            default:
                cout<<"Tolong pilih antara 1 sampai 3 saja!\n";
                pause();
                system("cls");
                continue;
                break;
        }
    }
}

bool list_item_cari(bool ketemu, int* arr, int ukuran){
    int idx;
    cout<<"--------------------List item--------------------\n";
    cout<<"1. Kembali\n";
    cout<<"-------------------------------------------------\n";
    
    if(!ketemu){
        cout<<"Item tidak ditemukan\n";
    }else{
        for(int i = 0; i < ukuran; i++){
            idx = arr[i];
            cout<<idx + 4 << ". " << player.inventory[idx].nama << " (lvl "<< player.inventory[idx].stat_i.lvl << ")\n";
        }
    }
    cout<<": ";
    cin>>idx;
    if(!ketemu && idx != 1){
        system("cls");
        return true;
    }
    if(idx < 1 || idx > (player.jumlahitem + 3)){
        cout<<"Tolong pilih antara 1 sampai "<<player.jumlahitem + 3<<" saja!\n";
        pause();
        system("cls");
        return true;
    }
    if(idx == 1){
        system("cls");
        return false;
    }else{
        system("cls");
        if(!aksi_item(idx-4)){
            return false;
        }
        return true;
    }
    return true;
}

void menu_search(bool kosong){
    int menu;
    int level;
    int ukuran;
    int* arr;
    string nama;
    bool ketemu = false;
    if(kosong){
        cout<<"Inventory Kosong\n";
        pause();
        system("cls");
        return;
    }
    cout<<"Cari Item\n";
    cout<<"1. Cari dengan nama item\n";
    cout<<"2. Cari dengan level item\n";
    cout<<"3. Kembali\n";
    cout<<": ";
    cin>>menu;
    cout<<"-------------------------------------------------\n";
    switch(menu){
        case 1:
            cout<<"Masukkan nama item yang ingin dicari\n";
            cout<<": "; 
            cin.ignore();
            getline(cin, nama);
            ukuran = search_nama(nama, arr);
            break;
        case 2:
            cout<<"Masukkan level item yang dicari\n";
            cout<<": ";
            cin>>level;
            ukuran = search_level(level, arr);
            break;
        case 3:
            system("cls");
            return;
            break;
        default:
            cout<<"Tolong pilih antara 1 sampai 3 saja!\n";
            pause();
            system("cls");
            return;
    }
    if(ukuran > 0){
        ketemu = true;
    }
    while(true){
        system("cls");
        if(!list_item_cari(ketemu, arr, ukuran)){
            break;
        };
    }
    delete[] arr;
    arr = nullptr;
    return;
}

bool list_item(){
    //return true berarti lanjut loop list_item
    //return false berarti balik ke menu_inventory
    int menu;
    bool kosong = false;
    if(player.jumlahitem == 0){
        kosong = true;
    }
    
    cout<<"--------------------List item--------------------\n";
    cout<<"1. Kembali\n";
    cout<<"2. Urutkan item\n";
    cout<<"3. Cari item \n";
    cout<<"-------------------------------------------------\n";
    if(kosong){
        cout<<"Inventory kosong\n";
    }else{
        for (int i = 0; i < player.jumlahitem; i++){
            cout<<i+4 << ". " << player.inventory[i].nama << " (lvl "<< player.inventory[i].stat_i.lvl << ")\n";
        }
    }
    cout<<"-------------------------------------------------\n";
    cout<<": ";
    cin>>menu;
    cout<<"-------------------------------------------------\n";
    if(menu < 1 || menu > (player.jumlahitem + 3)){
        cout<<"Tolong pilih antara 1 sampai "<<player.jumlahitem + 3<<" saja!\n";
        pause();
        system("cls");
        return true;
    }
    if(menu == 1){
        system("cls");
        return false;
    }else if(menu == 2){
        menu_sort(kosong);
        return true;
    }else if(menu == 3){
        menu_search(kosong);
        return true;
    }else{
        system("cls");
        aksi_item(menu - 4);
        return true;
    }
    return false;
}

bool menu_equipment(){
    int menu;
    cout<<"--------------------Equipment--------------------\n";
    cout<<"Item yang sedang dipakai\n";
    cout<<"-------------------------------------------------\n";
    for(int i = 0; i < 4; i++){
        switch(i){
            case 0:
                cout<<"Helm : ";
                break;
            case 1:
                cout<<"Armor : ";
                break;
            case 2:
                cout<<"Legging : ";
                break;
            case 3:
                cout<<"Senjata : ";
                break;
        }
        if(player.slot[i].kosong == true){
            cout<<"tidak ada\n";
        }else{
            cout<<player.slot[i].equipment.nama<<" (lvl "<<player.slot[i].equipment.stat_i.lvl<<")\n";
        }
    }
    cout<<"-------------------------------------------------\n";
    cout<<"1. Lepas item\n";
    cout<<"2. Lihat detail item\n";
    cout<<"3. Kembali\n";
    cout<<": ";
    cin>>menu;
    cout<<"-------------------------------------------------\n";
    switch(menu){
        case 1:
            cout<<"Item mana yang ingin di lepas?\n";
            cout<<"1. Helm\n";
            cout<<"2. Armor\n";
            cout<<"3. Legging\n";
            cout<<"4. Senjata\n";
            cout<<": ";
            cin>>menu;
            menu -= 1;
            cout<<"-------------------------------------------------\n";
            if(menu < 0 || menu >= 4){
                cout<<"Tolong pilih antara 1 sampai 4 saja\n";
                pause();
                system("cls");
                return true;
            }
            if(player.slot[menu].kosong){
                cout<<"Kosong\n";
                pause();
                system("cls");
                return true;
            }else{
                tambah_item(player.slot[menu].equipment);
                player.slot[menu].kosong = true;
                player.t_stat_p.vita -= player.slot[menu].equipment.stat_i.vita;
                player.t_stat_p.physical_atk -= player.slot[menu].equipment.stat_i.physical_atk;
                player.t_stat_p.magical_atk -= player.slot[menu].equipment.stat_i.magical_atk;
                player.t_stat_p.physical_def -= player.slot[menu].equipment.stat_i.physical_def;
                player.t_stat_p.magical_def -= player.slot[menu].equipment.stat_i.magical_def;
                cout<<player.slot[menu].equipment.nama<<" telah dilepas\n";
                pause();
                system("cls");
                return true;
            }
            break;
        case 2:
            cout<<"Item mana yang ingin di cek?\n";
            cout<<"1. Helm\n";
            cout<<"2. Armor\n";
            cout<<"3. Legging\n";
            cout<<"4. Senjata\n";
            cout<<": ";
            cin>>menu;
            menu -= 1;
            cout<<"-------------------------------------------------\n";
            if(menu < 0 || menu >= 4){
                cout<<"Tolong pilih antara 1 sampai 4 saja\n";
                pause();
                system("cls");
                return true;
            }
            if(player.slot[menu].kosong){
                cout<<"Kosong\n";
                pause();
                system("cls");
                return true;
            }else{
                system("cls");
                cout<<"--------------------Cek detail--------------------\n";
                cek_stat(player.slot[menu].equipment.stat_i);
                return true;
            }
            break;
        case 3:
            system("cls");
            return false;
            break;
        default:
            system("cls");
            return false;
            break;
    }

}

bool menu_inventory(){
    //return true berarti lanjut loop menu_inventory
    //return false berarti balik ke loop rest area
    int pilih;
    int no;
    int target_lvl;
    bool kosong;
    if(player.jumlahitem == 0){
        kosong = true;
    }
    string nama_cari;
    cout<<"--------------------Inventory--------------------\n";
    cout<<"Base stat\n";
    cout<<"-------------------------------------------------\n";
    cek_stat(player.b_stat_p, false, true, true);
    cout<<"Total stat\n";
    cek_stat(player.t_stat_p, true, true, true);
    cout<<"Pilih aksi anda\n";
    cout<<"1. List item di Inventory\n";
    cout<<"2. List equipment\n";
    cout<<"3. Kembali\n";
    cout<<": ";cin >> pilih;
    switch (pilih){
        case 1:
            system("cls");
            while(true){
                if(!list_item()){
                    return true;
                }else{
                    continue;
                }
            }
            break;
        case 2:
            system("cls");
            while(true){
                if(!menu_equipment()){
                    return true;
                }else{
                    continue;
                }
            }
            break;
        case 3:
            system("cls");
            return false;
        default:
            return false;
            break;
    }
    return false;
}

item jatuh_item(int lvl_lantai){
    int jml_helm = sizeof(helm)/sizeof(helm[0]);
    int jml_armor = sizeof(armor)/sizeof(armor[0]);
    int jml_legging = sizeof(legging)/sizeof(legging[0]);
    int jml_senjata = sizeof(senjata)/sizeof(senjata[0]);
    int rarity;
    srand(time(0));
    int piece = rand() % 4;//0 helm, 1 armor, 2 legging, 3 senjata
    item item;
    srand(time(0));
    switch(piece){
        case 0:
            rarity = rand() % jml_helm;
            item = helm[rarity];
            break;
        case 1:
            rarity = rand() % jml_armor;
            item = armor[rarity];
            break;
        case 2:
            rarity = rand() % jml_legging;
            item = legging[rarity];
            break;
        case 3:
            rarity = rand() % jml_senjata;
            item = senjata[rarity];
            break;
    }
    item.stat_i.lvl = lvl_lantai;
    item.stat_i.vita = floor(float(item.stat_i.vita) * (float(lvl_lantai)/2.0));
    item.stat_i.physical_atk = floor(float(item.stat_i.physical_atk) * (float(lvl_lantai)/2.0));
    item.stat_i.magical_atk = floor(float(item.stat_i.magical_atk) * (float(lvl_lantai)/2.0));
    item.stat_i.physical_def = floor(float(item.stat_i.physical_def) * (float(lvl_lantai)/2.0));
    item.stat_i.magical_def = floor(float(item.stat_i.magical_def) * (float(lvl_lantai)/2.0));
    return item;
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
    int tipe_dmg_musuh;
    int tipe_dmg_player;
    int def_musuh;
    int def_player;
    int tipe_def_musuh;
    int tipe_def_player;
    int kerusakan_musuh;
    int kerusakan_player;

    srand(time(0));
    musuh.tipe = (rand()%2)+1;
    tipe_dmg_musuh = musuh.tipe;
    tipe_def_musuh = musuh.tipe;
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
        cout<<"Hp anda : "<<player.hp<<'\n';
        cout<<"Hp musuh : "<<musuh.hp<<'\n';
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
        cout<<"5. Keluar\n";
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
                        dmg_player = serang(player.t_stat_p, tipe_dmg_player);
                        break;
                    case 2:
                        dmg_player = serang(player.t_stat_p, tipe_dmg_player);
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
                        def_player = berlindung(player.t_stat_p, tipe_def_player);
                        cout<<"Anda berlindung sebesar "<<def_player<<" physical defense\n";
                        break;
                    case 2:
                        def_player = berlindung(player.t_stat_p, tipe_def_player);
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
                cout<<"Stat Musuh\n";
                cek_stat(musuh.stat_m);
                continue;
                break;
            case 4:
                cout<<"Stat Anda\n";
                cek_stat(player.t_stat_p);
                continue;
                break;
            
            case 5:
                return 3;
                break;
            default:
                continue;
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
        musuh.hp -= kerusakan_musuh;
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
        player.hp -= kerusakan_player;
        if(tipe_dmg_musuh == 1){
            cout<<"Musuh memberikan damage sebesar "<<kerusakan_player<<" physical damage\n";
        }else if(tipe_dmg_musuh = 2){
            cout<<"Musuh memberikan damage sebesar "<<kerusakan_player<<" magical damage\n";
        }
    }
    cout<<"-------------------------------------------------\n";
    cout<<"Sisa hp anda : "<<player.hp<<'\n';
    cout<<"Sisa hp musuh : "<<musuh.hp<<'\n';
    if(musuh.hp <= 0){
        cout<<"-------------------------------------------------\n";
        pause();
        system("cls");
        return 1;
    }
    if(player.hp <= 0){
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

bool rest_area(int lantai){
    //return 0 berarti lanjut lantai
    //return 1 berarti lanjut loop rest area
    //return 2 brearti keluar ke main menu
    int menu;
    char lanjut;
    while(true){
        cout<<"======================Rest Area======================\n";
        cout<<"Apa yang ingin anda lakukan?\n";
        cout<<"1. Buka inventory\n";
        cout<<"2. Pergi ke lantai selanjutnya ("<<lantai + 1<<")\n";
        cout<<"3. Keluar\n";
        cout<<": ";
        cin>>menu;
        cout<<"-------------------------------------------------\n";
        switch(menu){
            case 1:
                system("cls");
                while(true){
                    if(!menu_inventory()){
                        system("cls");
                        break;
                    }
                    continue;
                }
                continue;
                break;
            case 2:
                cout<<"Apakah anda ingin pergi ke lantai"<<'\n'<<"selanjutnya?(y/n): ";
                cin>>lanjut;
                if(lanjut == 'y'){
                    system("cls");
                    return true;
                }
                system("cls");
                continue;
                break;
            case 3:
                return false;
                break;
            default:
                system("cls");
                continue;;
                break;
        }
    }
    return false;
}

// ak masih bingung bagian ini karena kaya nambahin stat dasar tp bukan stat tambahan dri item ehehe XD
void level_up(){
    player.t_stat_p.vita -= player.b_stat_p.vita;
    player.t_stat_p.physical_atk -= player.b_stat_p.physical_atk;
    player.t_stat_p.magical_atk -= player.b_stat_p.magical_atk;
    player.t_stat_p.physical_def -= player.b_stat_p.physical_def;
    player.t_stat_p.magical_def -= player.b_stat_p.magical_def;

    int menu_level_up;
    int poin_level = 5;
    while(poin_level > 0){
        system("cls");
        cout<<"====================LEVEL UP=====================\n";
        cout<<"Alokasikan poin stat anda\n";
        cout<<"-------------------------------------------------\n";
        cout<<"Anda memiliki "<<poin_level<<" poin stat\n";
        cout<<"1. Vitality : "<<player.b_stat_p.vita<<'\n';
        cout<<"2. Physical attack : "<<player.b_stat_p.physical_atk<<'\n';
        cout<<"3. Magical attack : "<<player.b_stat_p.magical_atk<<'\n';
        cout<<"4. Physical defense : "<<player.b_stat_p.physical_def<<'\n';
        cout<<"5. Magical defense : "<<player.b_stat_p.magical_def<<'\n';
        cout<<": ";
        cin>>menu_level_up;
        switch(menu_level_up){
            case 1:
                player.b_stat_p.vita += 1;
                break;
            case 2:
                player.b_stat_p.physical_atk += 1;
                break;
            case 3:
                player.b_stat_p.magical_atk += 1;
                break;
            case 4:
                player.b_stat_p.physical_def += 1;
                break;
            case 5:
                player.b_stat_p.magical_def += 1;
                break;
        }
        poin_level -= 1;
    }

    player.t_stat_p.lvl = player.b_stat_p.lvl;
    player.t_stat_p.vita += player.b_stat_p.vita;
    player.t_stat_p.physical_atk += player.b_stat_p.physical_atk;
    player.t_stat_p.magical_atk += player.b_stat_p.magical_atk;
    player.t_stat_p.physical_def += player.b_stat_p.physical_def;
    player.t_stat_p.magical_def += player.b_stat_p.magical_def;
    system("cls");
    cout<<"====================LEVEL UP=====================\n";
    cek_stat(player.b_stat_p);
    system("cls");
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
    int vita = 10, physical_atk = 10, magical_atk = 10, p_def = 10, m_def = 10, poin = 10, menu;
    while(poin > 0){
        system("cls");
        cout<<"===========BUAT SAVE FILE BARU===========\n";
        cout<<"Alokasikan poin stat anda\n";
        cout<<"-------------------------------------\n";
        cout<<"Anda memiliki "<<poin<<" poin stat\n";
        cout<<"1. Vitality : "<<vita<<'\n';
        cout<<"2. Physical attack : "<<physical_atk<<'\n';
        cout<<"3. Magical attack : "<<magical_atk<<'\n';
        cout<<"4. Physical defense : "<<p_def<<'\n';
        cout<<"5. Magical defense : "<<m_def<<'\n';
        cout<<": ";
        cin>>menu;
        switch(menu){
            case 1:
                vita += 1;
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
    save_file<<'\n'<<vita<<' '<<physical_atk<<' '<<magical_atk<<' '<<p_def<<' '<<m_def<<' '<<1;
    save_file<<'\n'<<0<<' '<<100;
    save_file<<'\n'<<1;
    save_file<<'\n'<<-1;
    save_file<<'\n'<<1<<' '<<1<<' '<<1<<' '<<1;
    save_file<<'\n'<<0;
    system("cls");

    //overview
    cout<<"===========BUAT SAVE FILE BARU===========\n";
    cout<<"Nama : "<<nama_player<<'\n';
    cout<<"-------------------------------------\n";
    cout<<"Vitality : "<<vita<<'\n';
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
    2. vita physical_atk magical_atk physical_def magical_def lvl -> integer
    3. exp max_exp-> integer
    5. lantai -> integer(minus kalo di rest area)
    6. hp_musuh -> integer(-1 kalo masih save file baru)
    kosong -> integer(slot equipment, klo 0 berarti g kosong (perlu load itemnya), klo 1 berarti kosong )
    item -> (klo kosong isinya bkn 0)
    jml_item -> integer
    item

    struktur item
    1. nama item -> string
    2. vita physical_atk magical_atk physical_def magical_def lvl -> integer
    3. tipe -> integer
*/

void save(path sf_path){
    ofstream save_file(sf_path, ofstream::trunc | ofstream::out);
    save_file<<player.nama;
    save_file<<'\n'<<player.b_stat_p.vita<<' '<<player.b_stat_p.physical_atk<<' '<<player.b_stat_p.magical_atk<<' '<<player.b_stat_p.physical_def<<' '<<player.b_stat_p.magical_def<<' '<<player.b_stat_p.lvl;
    save_file<<'\n'<<player.exp<<' '<<player.max_xp;
    save_file<<'\n'<<player.lantai;
    save_file<<'\n'<<player.hp_musuh;
    for(int i = 0; i < 4; i++){
        save_file<<'\n'<<player.slot[i].kosong;
        if(!player.slot[i].kosong){
            save_file<<'\n'<<player.slot[i].equipment.nama;
            save_file<<'\n'<<player.slot[i].equipment.stat_i.vita<<' '<<player.slot[i].equipment.stat_i.physical_atk<<' '<<player.slot[i].equipment.stat_i.magical_atk<<' '<<player.slot[i].equipment.stat_i.physical_def<<' '<<player.slot[i].equipment.stat_i.magical_def<<' '<<player.slot[i].equipment.stat_i.lvl;
            save_file<<'\n'<<player.slot[i].equipment.tipe;
        }
    }
    save_file<<'\n'<<player.jumlahitem;
    for(int j = 0; j < player.jumlahitem; j++){
        save_file<<'\n'<<player.inventory[j].nama;
        save_file<<'\n'<<player.inventory[j].stat_i.vita<<' '<<player.inventory[j].stat_i.physical_atk<<' '<<player.inventory[j].stat_i.magical_atk<<' '<<player.inventory[j].stat_i.physical_def<<' '<<player.inventory[j].stat_i.magical_def<<' '<<player.inventory[j].stat_i.lvl;
        save_file<<'\n'<<player.inventory[j].tipe;
    }
    save_file.close();
    cout<<"Save berhasil\n";
    pause();
}

void muat_sf(path sf_path){
    //inisialisasi player
    string nama;
    ifstream save_file(sf_path);
    getline(save_file, nama, '\n');
    player.nama = nama;
    save_file>>player.b_stat_p.vita;
    save_file>>player.b_stat_p.physical_atk;
    save_file>>player.b_stat_p.magical_atk;
    save_file>>player.b_stat_p.physical_def;
    save_file>>player.b_stat_p.magical_def;
    save_file>>player.b_stat_p.lvl;
    save_file>>player.exp;
    save_file>>player.max_xp;
    save_file>>player.lantai;
    save_file>>player.hp_musuh;
    for(int i = 0; i < 4; i++){
        save_file>>player.slot[i].kosong;
        if(!player.slot[i].kosong){
            save_file.ignore();
            getline(save_file, nama, '\n');
            player.slot[i].equipment.nama = nama;
            save_file>>player.slot[i].equipment.stat_i.vita;
            save_file>>player.slot[i].equipment.stat_i.physical_atk;
            save_file>>player.slot[i].equipment.stat_i.magical_atk;
            save_file>>player.slot[i].equipment.stat_i.physical_def;
            save_file>>player.slot[i].equipment.stat_i.magical_def;
            save_file>>player.slot[i].equipment.stat_i.lvl;
            save_file>>player.slot[i].equipment.tipe;
        }
    }
    save_file>>player.jumlahitem;
    for(int j = 0; j < player.jumlahitem; j++){
        save_file.ignore();
        getline(save_file, nama, '\n');
        player.inventory[j].nama = nama;
        save_file>>player.inventory[j].stat_i.vita;
        save_file>>player.inventory[j].stat_i.physical_atk;
        save_file>>player.inventory[j].stat_i.magical_atk;
        save_file>>player.inventory[j].stat_i.physical_def;
        save_file>>player.inventory[j].stat_i.magical_def;
        save_file>>player.inventory[j].stat_i.lvl;
        save_file>>player.inventory[j].tipe;
    }
    player.save_file = sf_path;
    save_file.close();
    player.t_stat_p.vita = player.b_stat_p.vita;
    player.t_stat_p.physical_atk = player.b_stat_p.physical_atk;
    player.t_stat_p.magical_atk = player.b_stat_p.magical_atk;
    player.t_stat_p.physical_def = player.b_stat_p.physical_def;
    player.t_stat_p.magical_def = player.b_stat_p.magical_def;
    for(int k = 0; k < 4; k++){
        if(!player.slot[k].kosong){
            player.t_stat_p.vita += player.slot[k].equipment.stat_i.vita;
            player.t_stat_p.physical_atk += player.slot[k].equipment.stat_i.physical_atk;
            player.t_stat_p.magical_atk += player.slot[k].equipment.stat_i.magical_atk;
            player.t_stat_p.physical_def += player.slot[k].equipment.stat_i.physical_def;
            player.t_stat_p.magical_def += player.slot[k].equipment.stat_i.magical_def;
        }
    }
    player.t_stat_p.lvl = player.b_stat_p.lvl;
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
    }
    return 3;
}

int main(){
    //game loop
    while(true){
        char s;
        bool init;
        int lvl_lantai;
        bool lanjut;
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
        
        if(lvl_lantai < 0){
            lvl_lantai *= -1;
            lanjut = rest_area(lvl_lantai);
            if(!lanjut){
                player.lantai = lvl_lantai;
                cout<<"Apakah anda ingin save progress anda? (y/n) : ";
                cin>>s;
                if(s == 'y'){
                    if(player.lantai > 0){
                        player.lantai *= -1;
                    }
                    save(player.save_file);
                }
                system("cls");
                continue;
            }else{
                lvl_lantai += 1;
            }
        }
        //loop main
        while(true){
            int rv_lantai;
            //inisialisasi musuh
            monster musuh;
            musuh.stat_m.lvl = lvl_lantai * 2;
            int lvl_musuh = musuh.stat_m.lvl + player.t_stat_p.lvl;
            musuh.stat_m.vita = lvl_musuh * 5;
            if(init){
                if(player.hp_musuh <= 0){
                    musuh.hp = musuh.stat_m.vita;
                    init = false;
                }else{
                    musuh.hp = player.hp_musuh;
                    init = false; 
                }
            }else{
                musuh.hp = musuh.stat_m.vita;
            }
            musuh.stat_m.physical_atk = lvl_musuh * 5;
            musuh.stat_m.physical_def = lvl_musuh * 10;
            musuh.stat_m.magical_atk = lvl_musuh * 10;
            musuh.stat_m.magical_def = lvl_musuh * 5;
            player.hp = player.t_stat_p.vita;
            //loop lantai
            while(true){
                rv_lantai = lantai(musuh, lvl_lantai);
                if(rv_lantai != 0){
                    break;
                }
            }
            if(rv_lantai == 1){
                //logika klo menang disini
                srand(time(0));
                int exp_diterima = 100 + ((rand()%player.max_xp + 1) * musuh.stat_m.lvl)/3;
                item item;
                cout<<"=================================================\n";
                cout<<"Anda Menang!\n";
                cout<<"-------------------------------------------------\n";
                cout<<"Anda mendapatkan "<<exp_diterima<<" exp\n";
                pause();
                system("cls");
                player.exp += exp_diterima;
                if(player.exp >= player.max_xp){
                    int jml_lvl = 0;
                    while(player.exp >= player.max_xp){
                        player.exp -= player.max_xp;
                        player.max_xp += ((50 * player.t_stat_p.lvl)/2);
                        player.b_stat_p.lvl += jml_lvl;
                        level_up();
                    }
                    //rumus untuk max xp selanjutnya, bisa diubah sesuai kebutuhan
                    
                    
                }
                if((30+lvl_lantai) >= (rand()%(100 + lvl_lantai * 2))){//rumusnya probabilitasnya adalah (40 + lvl_lantai)/(100 + lvl_lantai)
                    cout<<"=================================================\n";
                    cout<<"Musuh menjatuhkan item!\n";
                    item = jatuh_item(lvl_lantai);//sukses dpt item
                    menu_ambil_buang(item);
                    cout<<"-------------------------------------------------\n";
                    pause();
                    system("cls");
                }
                lanjut = rest_area(lvl_lantai);
                if(!lanjut){
                    player.hp_musuh = musuh.hp;
                    player.lantai = lvl_lantai;
                    cout<<"Apakah anda ingin save progress anda? (y/n) : ";
                    cin>>s;
                    if(s == 'y'){
                        if(player.lantai > 0){
                            player.lantai *= -1;
                        }
                        save(player.save_file);
                    }
                    system("cls");
                    break;
                }
                lvl_lantai += 1;
                continue;
            }else if(rv_lantai == 2){
                //logika klo kalah disini
                cout<<"Anda kalah!\n";
                cout<<"-------------------------------------------------\n";
                cout<<"Game Berakhir!, anda akan kembali ke main menu\n";
                pause();
                system("cls");
                break;
            }else if(rv_lantai == 3){
                player.hp_musuh = musuh.hp;
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