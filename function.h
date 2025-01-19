#ifndef function_h
#define function_h
#include <iostream>
#include <queue>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<fstream>
#include<cstring>
#include<string>
#include <windows.h>
#include<chrono>
#include <iomanip>
#include<cmath>
#include<cctype>
#include<conio.h>
using namespace std;
struct user{
    string username;
    string password;
    string nameInGame;
};
struct hashTable
{
// Variables ( Attributes )
    struct hashNodeUser
    {
        string key ;
        string value ;
        hashNodeUser* next = nullptr;
    };

    int capacity ;
    vector < hashNodeUser * > table ;
    // Functions ( Methods )
    void init ( unsigned int hashSize ) {
        capacity = hashSize;
        table = vector < hashNodeUser * >(capacity, nullptr);
    }
    void release () {
        for(int i = 0; i < capacity; i++){
            if(table[i] == nullptr){
                delete table[i];
                continue;
            }
            while(table[i]->next != nullptr){
                hashNodeUser* tmp = table[i]->next;
                table[i] = nullptr;
                delete table[i];
                table[i] = tmp;
            }
            table[i] = nullptr;
            delete table[i];
        }
    }
    unsigned int hashFunction (string key ) {
        unsigned int index = 0;
        long long sum = 0;
        for(int i = 0; i < key.size(); i++){
            sum += (int)key[i]*pow(31,i);
        }
        long long m = pow(10,9) + 9;
        index = sum % m;
        return index%capacity;
    }
    void add ( string key , string value ) {
        unsigned int index = hashFunction(key);
        if(table[index] == nullptr){
            table[index] = new hashNodeUser;
            table[index]->value = value;
            table[index]->key = key;
            return;
        }
        if(table[index]->key == key){
            table[index]->value = value;
            return;
        }
        hashNodeUser* tmp = table[index];
        while(tmp->next != nullptr){
            tmp = tmp->next;
        }
        tmp->next = new hashNodeUser;
        tmp->next->key = key;
        tmp->next->value = value;
    }
    string searchValue ( string key ) {
        unsigned int index = hashFunction(key);
        if(table[index] == nullptr){
            return "-1";
        }
        while(table[index] != nullptr){
            if(table[index] != nullptr && table[index]->key == key){
                return table[index]->value;
            }
            if(table[index]->next == nullptr){
                return "";
            }
            table[index] = table[index]->next;
        }
        return "";
    }
    void removeKey ( string key ) {
        unsigned int index = hashFunction(key);
        while(table[index] != nullptr){
            if( table[index]->key == key){
                //tmp->key = "";
                table[index] = nullptr;
                return;
            }
            if(table[index]->next == nullptr){
                break;
            }
            table[index] = table[index]->next;
        }
        return;
    }
};



void clearScreen();
user login(hashTable& h);
void playGame();
#endif