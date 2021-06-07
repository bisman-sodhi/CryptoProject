#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#define N 7
#include <openssl/sha.h>
#include "openssl/crypto.h"

using namespace std;



/* I got this sha256 function from the link posted in one of the piazza posts https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c/10632725*/
string sha256(const string str){
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update (&sha256, str.c_str(),  str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++){
        ss << hex << setw(2) << setfill ('0') << (int)hash[i];
    }
    return ss.str();
}

struct block {
    int amount;
    string sender;
    string receiver;
    string nonce;
    string hash;
    block * prevHash = NULL;
    block * head = NULL;

    //constructor
    block() : amount(0), sender(""), receiver(""), nonce(""), hash("NULL"), prevHash(NULL), head(NULL) {}
    block(int inAmt, string inSend, string inRec) : amount(inAmt), sender(inSend), 
                receiver(inRec), nonce(""), hash("NULL"), prevHash(NULL), head(NULL){};

    
    string forNonce(){
        //srand(time(0)); 
        string temp = "";
        static const char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

        temp.reserve(7);
        for (int i = 0; i < 7; ++i){
            temp += alpha[rand() % (sizeof(alpha) - 1)];
        }
        return temp;
    }
    string forResize(string a){
        int size = a.size();
        a.resize(size - 3);
        a+forNonce();
        
        return a; 
    }


    void add(int amount, string sender, string receiver){
        if (typeid(amount) != typeid(int)) {;
            cout << "Enter integer amount " << endl;
            return;
        }

        int firstAmount = 50;
        string nonceHolder = "";
        string n = "";
        string input = "";
        if(head == NULL){
            
            head = new block(firstAmount, sender, receiver);   
            nonce = to_string(firstAmount) + sender + receiver + forNonce(); 
            hash = "NULL";
            
            nonceHolder = sha256(nonce);
            while(nonceHolder.back() != '0'){
                int size = nonceHolder.size();
                nonceHolder.resize(size - 7);
                n = forNonce();
                nonceHolder = nonceHolder+n;
                nonceHolder = sha256(nonceHolder);
                nonce = n;
                
            }
            
            
            
            prevHash = NULL;
        }

        else {
            block * anotherBlock = new block(amount, sender, receiver);
            
            anotherBlock->prevHash = head;
           
            anotherBlock->head = head;
            
            nonce = to_string(amount) + sender + receiver  + forNonce();
            
            string nonceHolder = sha256(nonce);
          
            while(nonceHolder.back() != '0'){
                
                int size = nonceHolder.size();
                nonceHolder.resize(size - 7);
                n = forNonce();
                nonceHolder = nonceHolder+n;
                nonceHolder = sha256(nonceHolder);
                nonce = n;
            }
            
            hash = sha256(to_string(anotherBlock->prevHash->amount) + anotherBlock->prevHash->sender + anotherBlock->prevHash->receiver + anotherBlock->prevHash->hash + anotherBlock->prevHash->nonce);
            
        }
        
    };

    void getBalance(string person){
        int currentBalance = 0;

        block* temp = NULL;
        temp = this->head;
        while(temp != NULL){
            if(person == temp->sender ){
                currentBalance -= temp->amount;
            }
            
            if(person == temp->receiver){
                currentBalance += temp->amount;
            }
            temp = temp->prevHash;
        }
        cout << "curr " << currentBalance << endl;
    };
        

    void helperPrint(block * head){
        if(head == NULL)
            return;
        cout << "Amount: " << head->amount << endl; 
        cout << "Sender: " << head->sender << endl;
        cout << "Receiver: " << head->receiver << endl;
        cout << "Nonce: " << head->nonce << endl;
        cout << "Hash: " << head->hash << endl;

        helperPrint(head->prevHash);
    };
    void print() {
        helperPrint(head);
    }

};



int main(){
    
    int x;
    block b1;
    //do{  
    cout << "Welcome to the transaction-chian application....\n";
    cout << "1) Add a transaction to the chain.\n";
    cout << "2) Find Balance of a person.\n";
    cout << "3) Print the chain.\n";
    cout << "4) Exit.\n";
    cout << "Which operation do you want to make? (1,2,3,4): \n";
    
    string a, b, c;
    int y;
    cin >> x;
    
        switch (x){
            case 1: 
                cout << "Integer amount of money: \n";
                cin >> y;
                cout << "Sender name \n";
                cin >> a;
                cout << "Receiver name \n";
                cin >> b;
                b1.add(y, a, b); 
                break;
            case 2: 
                cout << "Person name\n";
                cin >> c;
                b1.getBalance(c);
                break;
            case 3: 
                b1.print();
                break;
            case 4: 
                exit(0);
                break;
        }
    //} while ((x > 0) && (x < 5));
    return 0;
}

