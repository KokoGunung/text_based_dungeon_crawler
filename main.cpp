#include <iostream>
#include <conio.h>//buat getche
#include <cstdlib>
#include <ctime>
using namespace std;

struct hambali{
    stat stat_p;
    // index 0 sampai 2 untuk armor (helm , baju , legging) , index 3 untuk senjata
    item equipment[4];
    int mana;
    int exp;
};

struct monster{
    stat stat_m;
};


struct stat{
    int hp, physical_atk, magical_atk, physical_def, magical_def, lvl;
};

struct item{
    string nama;
    stat stat_i;
};

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
    cout<<"1. List save file disini\n";
    cout<<"2. Kembali\n";
    cout<<": ";
    cin>>menu;
    if(menu == 2){
        system("cls");
        return true;
    }

    //temp
    if(menu == 1){
        system("cls");
        cout<<"Blm ada apa apa njir\n";
        cout<<"tekan enter untuk lanjut\n";
        getche();
        system("cls");
        return true;
    }
    return true;
}

bool game(){
    if(!main_menu()){
        return false;
    }
    return true;
}


bool lantai(hambali player, int lantai){
    int aksi;
    //temp
    monster musuh;
    musuh.stat_m.lvl = lantai * 2;
    int lvl_musuh = musuh.stat_m.lvl;
    musuh.stat_m.hp = lvl_musuh * 10;
    musuh.stat_m.physical_atk = lvl_musuh * 5;
    musuh.stat_m.physical_def = lvl_musuh * 10;
    musuh.stat_m.magical_atk = 0;
    musuh.stat_m.magical_def = 0;

    //aksi player
    cout<<"Anda menemui musuh di depan anda!\n";
    cout<<"=======Pilih aksi anda=======";
    cout<<"1. Menyerang\n";
    cout<<"2. Berlindung\n";
    cout<<"3. Cek stat musuh\n";
    cout<<"4. Cek stat anda\n";
    cout<<": ";
    cin>>aksi;
    switch(aksi){
        case 1:
            int tipe;
            int damage;
            cout<<"1. Physical attack\n";
            cout<<"2. Magical attack\n";
            cin>>tipe;
            switch(tipe){
                case 1:
                    //temp
                    damage = serang(player.stat_p, tipe) - musuh.stat_m.physical_def;
                    break;
                case 2:
                    damage = serang(player.stat_p, tipe) - musuh.stat_m.magical_def;
                    break;
            }
            if(damage <= 0){
                damage = 1;
            }
            musuh.stat_m.hp -= damage;
            break;
        case 2:
            break;
        case 3:
            cout<<"stat musuh\n";
            cek_stat(musuh.stat_m);
            break;
        case 4:
            cout<<"stat anda\n";
            cek_stat(player.stat_p);
            //temp
            cout<<"Mana : "<<player.mana<<'\n';
            break;
    }

    if(player.stat_p.hp <= 0){
        return false;
    }
    if(musuh.stat_m.hp <= 0){
        return false;
    }
    return true;
}

int serang(stat stat, int tipe){
    srand(time(0));
    int variasi = rand() % 11;
    if(rand()%2 == 1){
        variasi *= -1;
    }

    if(tipe == 1){
        return stat.physical_atk + variasi;
    }
    return stat.magical_atk + variasi;
}

void cek_stat(stat stat){
    cout<<"Level :" <<stat.lvl<<'\n';
    cout<<"Hp : " <<stat.hp<<'\n';
    cout<<"Physical attack : "<<stat.physical_atk<<'\n';
    cout<<"Physical defense : "<<stat.physical_def<<'\n';
    cout<<"Magical attack : "<<stat.magical_atk<<'\n';
    cout<<"Magical defense : "<<stat.magical_def<<'\n';
}

int main(){
    //temp
    int lvl_lantai = 0;
    hambali player;
    player.stat_p.hp = 100;
    player.stat_p.physical_def = 10;
    player.stat_p.lvl = 1;
    player.stat_p.physical_atk = 100;
    player.mana = 0;
    player.stat_p.magical_atk = 0;
    player.stat_p.magical_def = 0;
    player.exp = 0;
    //game loop
    while(game()){
        while(lantai(player, lvl_lantai)){

        }
    }
}