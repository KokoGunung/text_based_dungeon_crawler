#include <iostream>
#include <conio.h>//buat getche
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;


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

void input_data_save_file(){

    string nama_file;
    cout << "Masukkan nama save file\n";
    cout << "Nama File: "; getline(cin, nama_file);
    cin.ignore();
    cout << "Masukkan nama player\n";
    cout << "Nama: "; getline(cin, player.nama);
    nama_file += ".txt";
    fstream file(nama_file);

    file << player.nama << nama_file;
    
    file.close();
}
bool main_menu(){
    //menu
    int menu;
    cout<<"=================DUNGEON CRAWLER================\n";
    cout<<"1. Main\n";
    cout<<"2. Keluar\n";
    cout<<": ";
    cin>>menu;
    if(menu == 2){
        return false;
    }
    system("cls");
    
    //save file
    cout<<"--------------Pilih save file anda--------------\n";
    cout<<"1. Buat save file baru\n";
    cout<<"2. Kembali\n";
    cout<<"3. List save file disini\n";
    cout<<": ";
    cin>>menu;

    if(menu == 1){
        system("cls");
        input_data_save_file();
        getche();
        return true;
    }

    if(menu == 2){
        system("cls");
        return true;
    }

    //temp
    if(menu == 3){
        system("cls");
        cout<<"Blm ada apa apa njir\n";
        cout<<"tekan enter untuk lanjut\n";
        getche();
        system("cls");
        return true;
    }
    return true;
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

int* berlindung(stats stat){
    //sudah ditambah dengan base stat
    //index 0 physical, index 1 magical
    srand(time(0));
    int m_def = (rand() % stat.magical_def) + stat.magical_def;
    int p_def = (rand() % stat.physical_def) + stat.physical_def;
    int *arr = new int[2];
    arr[0] = p_def;
    arr[1] = m_def;
    return arr;
}


void cek_stat(stats stat){
    cout<<"Level :" <<stat.lvl<<'\n';
    cout<<"Hp : " <<stat.hp<<'\n';
    cout<<"Physical attack : "<<stat.physical_atk<<'\n';
    cout<<"Physical defense : "<<stat.physical_def<<'\n';
    cout<<"Magical attack : "<<stat.magical_atk<<'\n';
    cout<<"Magical defense : "<<stat.magical_def<<'\n';
}

bool game(){
    if(!main_menu()){
        return false;
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
    int *def_musuh;
    int *def_player;
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
        def_musuh = berlindung(musuh.stat_m);
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
            def_player = berlindung(player.stat_p);
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
        return false;
    }
    if(player.stat_p.hp <= 0){
        return false;
    }
    return true;
}

//temp
item pedang_besar_hitam;


int main(){
    //TEMP
    //player
    int lvl_lantai = 0;
   
    player.stat_p.hp = 100;
    player.stat_p.physical_def = 10;
    player.stat_p.lvl = 1;
    player.stat_p.physical_atk = 100;
    player.stat_p.magical_atk = 0;
    player.stat_p.magical_def = 0;
    player.exp = 0;
    //item
    pedang_besar_hitam.tipe = 3;




    //game loop
    while(game()){
        monster musuh;
        musuh.stat_m.lvl = lvl_lantai * 2;
        int lvl_musuh = musuh.stat_m.lvl;
        musuh.stat_m.hp = lvl_musuh * 10;
        musuh.stat_m.physical_atk = lvl_musuh * 5;
        musuh.stat_m.physical_def = lvl_musuh * 10;
        musuh.stat_m.magical_atk = 0;
        musuh.stat_m.magical_def = 0;
        while(lantai(player, musuh, lvl_lantai)){

        }
    }
}