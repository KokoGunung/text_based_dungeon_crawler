#include <iostream>
#include <conio.h>//buat getche
#include <cstdlib>
#include <ctime>
#include <fstream>//untuk file handling
#include <filesystem>//untuk folder(directory)

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
    bool kosong;
    item equipment;
};

struct hambali{
    stats stat_p;
    //idx 0,1,2 berarti helm, armor, legging  3 berarti senjata
    e_slot slot[4];
    int exp;
    string nama;
    path save_file;//untuk save file
    item inventory[50];
    int jumlahitem = 0;//Jumlah item sekarang
};

struct monster{
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

item senjata[5]={
    {3,"pedang copper", {0,10,0,0,0,1}},
    {3,"pedang silver", {0,20,0,0,0,2}},
};


//functions
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

void berlindung(stats stat, int* def){
    //sudah ditambah dengan base stat
    //index 0 physical, index 1 magical
    srand(time(0));
    int m_def = (rand() % stat.magical_def) + stat.magical_def;
    int p_def = (rand() % stat.physical_def) + stat.physical_def;
    def[0] = p_def;
    def[1] = m_def;
}

void cek_stat(stats stat){
    cout<<"Level :" <<stat.lvl<<'\n';
    cout<<"Hp : " <<stat.hp<<'\n';
    cout<<"Physical attack : "<<stat.physical_atk<<'\n';
    cout<<"Physical defense : "<<stat.physical_def<<'\n';
    cout<<"Magical attack : "<<stat.magical_atk<<'\n';
    cout<<"Magical defense : "<<stat.magical_def<<'\n';
}

bool try_drop_item(item &drop){
    int chance = rand() % 100 + 1; // 10% chance
    if(chance > 10){
        return false;
    }

    int totalItems = 5 + 5 + 5 + 2;
    int index = rand() % totalItems;
    if(index < 5){
        drop = helm[index];
    } else if(index < 10){
        drop = armor[index - 5];
    } else if(index < 15){
        drop = legging[index - 10];
    } else {
        drop = senjata[index - 15];
    }
    return true;
}

void Isi_Inventory(hambali player){
    if(player.jumlahitem == 0){
        cout << "Inventory 0\n";
        return;
    }
    for (int i = 0; i < player.jumlahitem; i++){
        cout << i+1 << "." << player.inventory[i].nama << " (Lvl "<< player.inventory[i].stat_i.lvl << ")\n";
    }
    
}

bool buangitem(hambali &player, int n){
    if(n < 0 || n >= player.jumlahitem){
        cout << "Tidak ada item pada no tersebut\n";
        return false;
    }
    for(int i = n; i < player.jumlahitem - 1; i++){
        player.inventory[i] = player.inventory[i+1];
    }
    player.jumlahitem--;
    return true;
}

bool tambahitem(hambali &player, item baru){
    if (player.jumlahitem >= 50){
        cout << "Inventory Penuh\n";
        return false;
    }
    player.inventory[player.jumlahitem] = baru;
    player.jumlahitem++;
    cout << baru.nama << "Telah di tambahkan\n";
    return true;
}

void menu_ambil_buang(item barang){
    int pilih;
    cout << "Anda mendapatkan item" << barang.nama <<"\n";
    cout << "Stat Item :\n";
    cout << "Hp : " << barang.stat_i.hp << "\n";
    cout << "Physical Attack : " << barang.stat_i.physical_atk << "\n";
    cout << "Magical Attack : " << barang.stat_i.magical_atk << "\n";
    cout << "Physical Defense : " << barang.stat_i.physical_def << "\n";
    cout << "Magical Defense : " << barang.stat_i.magical_def << "\n";
    cout << "Level : " << barang.stat_i.lvl << "\n";
    cout << "Pilih aksi anda\n";
    cout << "1. Simpan Item\n";
    cout << "2. Buang Item\n";
    cout << "Pilihan Anda : "; cin >> pilih;
    switch (pilih){
        case 1 :
        tambahitem(player, barang);
        break;
        case 2 :
        cout << "Item Telah Dibuang\n";
        break;
    default:
        cout << "Pilihan Tidak Valid\n";
        break;
    }
}

void menu_Inventory (hambali &player){
    int pilih;
    int no;
    while(pilih != 3){
        cout << "Menu - Inventory\n";
        cout << "1. Lihat Inventory\n";
        cout << "2. Buang Item\n";
        cout << "3. Kembali\n";
        cout << ":";cin >> pilih;
        switch (pilih){
            case 1:
                Isi_Inventory(player);
                break;
            case 2:
                Isi_Inventory(player);
                cout << "Pilih item yang mau dibuang:";cin >> no;
                buangitem(player, no - 1);
                break;
            case 3:
                break;
        default:
            break;
        }

    }
}

bool lantai(hambali player, monster musuh, int lantai){
    int aksi_player;
    int aksi_musuh;
    int dmg_musuh;
    int dmg_player;
    int tipe_dmg_musuh;
    int tipe_dmg_player;
    int def_musuh[2];
    int def_player[2];
    int kerusakan_musuh;
    int kerusakan_player;

    //aksi player
    cout<<"Anda menemui musuh di depan anda!\n";
    //aksi monster
    srand(time(0));
    aksi_musuh = (rand()%2)+1;
    if(aksi_musuh == 1){
        cout << "Musuh akan menyerang\n";
        srand(time(0));
        tipe_dmg_musuh = (rand()%2)+1;
        if(tipe_dmg_musuh == 0){
            dmg_musuh = serang(musuh.stat_m, tipe_dmg_musuh);
        } else{
            dmg_musuh = serang(musuh.stat_m, tipe_dmg_musuh);
        }
    }else if(aksi_musuh == 2){
        cout << "Musuh akan berlindung\n";
        berlindung(musuh.stat_m, def_musuh);
        cout << "Musuh berlindung sebesar " << def_musuh[0] <<" physical defense\n";
        cout << "Musuh berlindung sebesar " << def_musuh[1] <<" magical defense\n";

    }
    cout<<"=======Pilih aksi anda=======\n";
    cout<<"1. Menyerang\n";
    cout<<"2. Berlindung\n";
    cout<<"3. Cek stat musuh\n";
    cout<<"4. Cek stat anda\n";
    cout<<"5. Buka Inventory\n";
    cout<<": ";
    cin>>aksi_player;
    switch(aksi_player){
        case 1: {
            cout<<"1. Physical attack\n";
            cout<<"2. Magical attack\n";
            cin>>tipe_dmg_player;
            switch(tipe_dmg_player){
                case 1: {
                    dmg_player = serang(player.stat_p, tipe_dmg_player);
                    break;
                }
                case 2: {
                    dmg_player = serang(player.stat_p, tipe_dmg_player);
                    break;
                }
            }
            break;
        }
        case 2: {
            berlindung(player.stat_p, def_player);
            cout << "Anda berlindung sebesar " << def_player[0] <<" physical defense\n";
            cout << "Anda berlindung sebesar " << def_player[1] <<" magical defense\n";
            break;
        }
        case 3: {
            cout<<"stat musuh\n";
            cek_stat(musuh.stat_m);
            break;
        }
        case 4:{
            cout<<"stat anda\n";
            cek_stat(player.stat_p);
            break;
        }
        case 5:{
            menu_Inventory(player);
            break;
        }
    }

    //damage player ke musuh
    if(tipe_dmg_player == 1){
        kerusakan_musuh = dmg_player - def_musuh[0];
        musuh.stat_m.hp -= kerusakan_musuh;
        cout<<"Kamu memberikan damage sebesar " << kerusakan_musuh << " physical damage\n";
    }else if(tipe_dmg_player == 2){
        kerusakan_musuh = dmg_player - def_musuh[1];
        musuh.stat_m.hp -= kerusakan_musuh;
        cout<<"Kamu memberikan damage sebesar " << kerusakan_musuh << " magical damage\n";
    }

    //damage musuh ke player
    if(aksi_musuh == 1){
        if(tipe_dmg_musuh == 1){
            kerusakan_player = dmg_musuh - def_player[0];
            player.stat_p.hp -= kerusakan_player;
            cout<<"Musuh memberikan damage sebesar "<<kerusakan_player<<" mhysical damage\n";
        }else if(tipe_dmg_musuh = 2){
            kerusakan_player = dmg_musuh - def_player[1];
            player.stat_p.hp -= kerusakan_player;
            cout<<"Musuh memberikan damage sebesar "<<kerusakan_player<<" magical damage\n";
        }
    }
    
    if(musuh.stat_m.hp <= 0){
        item drop;
        if(try_drop_item(drop)){
            cout << "Musuh berhasil dikalahkan dan menjatuhkan item: " << drop.nama << "\n";
            menu_ambil_buang(drop);
        } else {
            cout << "Musuh berhasil dikalahkan, tetapi tidak ada item yang dijatuhkan.\n";
        }
        return false;
    }
    if(player.stat_p.hp <= 0){
        return false;
    }
    return true;
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
            cout<<"Tekan tombol apapun untuk lanjut\n";
            getche();
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
    cout<<"Tekan tombol apapun untuk lanjut\n";
    getche();
    system("cls");
}


/*
    struktur save file
    1. nama player -> string
    2. hp physical_atk magical_atk physical_def magical_def lvl -> integer
    3. helm baju legging senjata -> integer(-1 kalo g ada equipment)
    4. exp -> integer
*/

void save(path sf_path){
    ofstream save_file(sf_path);
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
            player.slot[i].kosong = true;
        }else{
            player.slot[i].kosong = false;
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
    player.save_file = sf_path;
    save_file.close();
    cout<<"Save file berhasil dimuat\n";
    cout<<"Tekan tombol apapun untuk lanjut\n";
    getche();
    system("cls");
}

int aksi_sf(path sf_path){
    //return 1 berarti lanjut ke loop lantai(sf berhasil dimuat)
    //return 2 berarti balik ke loop list sf
    //return 3 berarti lanjut loop aksi sf
    int menu;
    char hapus;
    cout<<"--------------------------Pilih aksi anda--------------------------\n";
    cout<<"1. Muat save file\n";
    cout<<"2. Hapus save file\n";
    cout<<": ";
    cin>>menu;
    switch(menu)
    {
        case 1:
            muat_sf(sf_path);
            return 1;
            break;
        case 2:
            cout<<"-------------------------------------------------------------------\n";
            cout<<"Apakah anda yakin ingin menghapus save file ini? (y/n) : ";
            cin>>hapus;
            switch(hapus){
                case 'y':
                    remove(sf_path);
                    cout<<"Save file berhasil dihapus\n";
                    cout<<"Tekan tombol apapun untuk lanjut\n";
                    getche();
                    system("cls");
                    return 2;
                    break;
                case 'n':
                    system("cls");
                    return 3;
                    break;
            }
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

bool game(){
    int lvl_lantai = 0;
    monster musuh;
    musuh.stat_m.lvl = lvl_lantai * 2;
    int lvl_musuh = musuh.stat_m.lvl;
    musuh.stat_m.hp = lvl_musuh * 10;
    musuh.stat_m.physical_atk = lvl_musuh * 5;
    musuh.stat_m.physical_def = lvl_musuh * 10;
    musuh.stat_m.magical_atk = 0;
    musuh.stat_m.magical_def = 0;
    //loop main menu
    while(true){
        int rv_main_menu = main_menu();
        if(rv_main_menu == 1){
            break;
        }else if(rv_main_menu == 2){
            return false;
        }else if(rv_main_menu == 3){
            continue;
        }
    }
    //loop lantai
    while(true){
        //kalau kalah atau menang
        if(!lantai(player, musuh, lvl_lantai)){
            return true;
        }
    }
    return true;
}

int main(){
    //game loop
    while(game()){
        
        
    }
}