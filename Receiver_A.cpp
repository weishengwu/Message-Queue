//
//  Receiver_A.cpp
//  Message_Queue_Final
//
//  Created by Trevor Anderson, Jonathan Ascencio, Brittney Witts, Weisheng Wu.
//  Copyright © 2018 Trevor Anderson, Jonathan Ascencio, Brittney Witts, Weisheng Wu.
//  All rights reserved.
//

/*
RECEIVER A
 
 -Accepts messages from Sender_251 and Sender_997
 -Displays value and sender's identity
 -Sends an acknowledgement notification back to 997.
 -It only terminates after both of its senders terminate
 
*/


//header files
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
using namespace std;



int main() {
    
    // create my msgQ with key value from ftok()
    int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);
    
    // declare my message buffer
    struct buf {
        long mtype; // required
        char greeting[50]; // mesg content
    };
    
    buf msg;
    int size = sizeof(msg)-sizeof(long);
    string message;
    int sender;
    bool sender997 = true;
    bool sender251 = true;
    
    
    while(sender997 || sender251) //Continues while Sender 997 and 251 are true
    {
        //cout<<"before reveice"<<endl;
        msgrcv(qid, (struct msgbuf *)&msg, size, 61, 0); // read mesg
        message = msg.greeting;
        string nMessage = message.substr(0,3);
        //cout << "test" << nMessage;
        
        try{
            sender = stoi(nMessage);
        }catch(const  invalid_argument& e){
            cout << "Invalid answer: " << nMessage << endl;
        }catch(const out_of_range& e){
            cout << "Invalid answer : " << nMessage << endl;
        }
        
        message = message.substr(5);
        
        switch(sender)
        {
            case 997:
                if(message.compare("Quit") ==0) //If Sender 997 is terminated
                {
                    msg.mtype = 41;
                    
                    cout<<"Message \""<< message <<"\" from Sender_997 Received\nSender_997 has quit"<<endl;
                    strcpy(msg.greeting, "Sender_997 quit successful");
                    //cout<<"Sender_997 quit successful"<<endl;
                    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
                    sender997 = false;
                }else
                {
                    msg.mtype = 41;
                    strcpy(msg.greeting, "Message from Sender_997 Received");
                    cout<<"Message "<<message<<" from Sender_997 Received"<<endl;
                    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
                }
                break;
            case 251:
                if(message.compare("Quit") ==0) //If Sender 251 is terminated
                {
                    sender251 = false;
                    cout<<" Sender_251 quit successfully"<<endl;
                    break;
                }
                cout<<"Message "<<message<<" from Sender_251 Received"<<endl;
                break;
        }
        
    }
    
    msgctl (qid, IPC_RMID, NULL);
    
    exit(0);
}
