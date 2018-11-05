//
//  Receiver_B.cpp
//  Message_Queue_Final
//
//  Created by Trevor Anderson, Jonathan Ascencio, Brittney Witts, Weisheng Wu.
//  Copyright © 2018 Trevor Anderson, Jonathan Ascencio, Brittney Witts, Weisheng Wu.
//  All rights reserved.
//

/*
RECEIVER B
 
 -Accepts messages from Sender_257 and Sender_997
 -Displays value and sender's identity
 -Sends an acknowledgement notification back to 997.
 -Receiver B is terminated after receiving 5000 messages
 
 
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
    int qid = msgget(ftok(".",'u'), 0);
    
    // declare my message buffer
    struct buf {
        long mtype; // required
        char greeting[50]; // mesg content
    };
    
    buf msg;
    int size = sizeof(msg)-sizeof(long);
    string message;
    string sender;
    bool sender997 = true;
    bool sender251 = true;
    
    
    for(int i = 0; i<5000; i++) //Receiving messages up to 5000 total
    {
        //gets message
        msgrcv(qid, (struct msgbuf *)&msg, size, 62, 0); // read mesg
        message = msg.greeting;
        sender = message.substr(0,3);
        message = message.substr(4);
        
        if(sender.compare("997") == 0)
        {
            //sends ack to sender_997
            msg.mtype = 42;
            
            strcpy(msg.greeting, "Message from Sender_997 Received");
            cout<<"Message "<<message<<" from Sender_997 Received"<<endl;
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
        }else
        {
            //sends flag to 257
            cout<<"Message "<<message<<" from Sender_257 Received"<<endl;
            strcpy(msg.greeting, "Still Alive");
            msg.mtype = 43;
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
        }
        
    }
    
    //sends quit to sender_997
    msg.mtype = 42;
    strcpy(msg.greeting, "Quit");
    cout<<"Receiver_B has terminated"<<endl;
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    
    //sends flag to sender_257
    strcpy(msg.greeting, "Dead");
    msg.mtype = 43;
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    
    exit(0);
}
