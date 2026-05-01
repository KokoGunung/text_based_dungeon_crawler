#include <iostream>
#include <conio.h>//buat getche
using namespace std;

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

int main(){
    //game loop
    while(game()){

    }
}