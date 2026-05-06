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

struct hambali{
    stats stat_p;
    // index 0 sampai 2 untuk armor (helm , baju , legging) , index 3 untuk senjata
    item equipment[4];
    int exp;
    string nama;
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
    cout<<"=======Pilih aksi anda=======";
    cout<<"1. Menyerang\n";
    cout<<"2. Berlindung\n";
    cout<<"3. Cek stat musuh\n";
    cout<<"4. Cek stat anda\n";
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
    save_file<<nama_player<<'\n';
    save_file.close();
    cout<<"Save file berhasil dibuat\n";
    cout<<"Tekan tombol apapun untuk lanjut\n";
    getche();
    system("cls");
}

bool muat_save_file(int idx, path* arr){
    cout<<"Blm diimplementasi njir saya capek\n";
    cout<<"Tekan tombol apapun untuk lanjut\n";
    getche();
    system("cls");
    return true;
}

bool list_save_file(){
    //return false berarti balik ke menu save file
    //return true berarti save file berhasil di muat
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
    path p_save_file[counter];
    counter = 0;
    for(auto const& save_file : directory_iterator(p_folder)){
        p_save_file[counter] = save_file.path();
    }
    cout<<": ";
    cin>>menu;
    if(menu == 1){
        system("cls");
        return false;
    }else{
        system("cls");
        //temp
        if(muat_save_file(menu, p_save_file) == true){
            return false;
        };
    }
    return false;
}

int menu_save_file(){
    //return 1 berarti lanjut loop menu save file
    //return 2 berarti kembali ke main menu
    int menu;
    //save file
    cout<<"--------------Menu Save File--------------\n";
    cout<<"1. Buat save file baru\n";
    cout<<"2. List semua save file\n";
    cout<<"3. Kembali\n";
    cout<<": ";
    cin>>menu;

    switch(menu){
        case 1:
            system("cls");
            buat_save_file_baru();
            return 1;
            break;
        case 2:
            system("cls");
            if(!list_save_file()){
                return 1;
            };
            break;
        case 3:
            return 2;
            break;
    }
    return 1;
}

int main_menu(){
    //return 1 berarti lanjut loop main menu
    //return 2 berarti keluar loop lanjut ke loop lantai
    //return 3 berarti keluar loop keluar program
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
                if(menu_save_file() == 2){
                    system("cls");
                    return 1;
                }
            }
            break;
        case 2:
            return 3;
    }
    return 1;
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
        if(rv_main_menu == 2){
            break;
        }else if(rv_main_menu == 3){
            return false;
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
    //TEMP
    //player
    player.stat_p.hp = 100;
    player.stat_p.physical_def = 10;
    player.stat_p.lvl = 1;
    player.stat_p.physical_atk = 100;
    player.stat_p.magical_atk = 0;
    player.stat_p.magical_def = 0;
    player.exp = 0;

    //game loop
    while(game()){
        
        
    }
}