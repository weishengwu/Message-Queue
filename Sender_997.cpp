//
//  Sender_997.cpp
//  Message_Queue_Final
//
//  Created by Trevor Anderson, Jonathan Ascencio, Brittney Witts, Weisheng Wu.
//  Copyright © 2018 Trevor Anderson, Jonathan Ascencio, Brittney Witts, Weisheng Wu.
//  All rights reserved.
//

/*
SENDER 997
 
  -Generates random numbers pre-assigned with divisor integer of 257
  -Sends event to Receiver A and Receiver B.
  -Requires an acknowledgement message for every message sent to a receiver.
  -It terminates whenever it generates a random number less than 100.

 */


//header files
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <bits/stdc++.h>
using namespace std;

int main() {
    
    int qid = msgget(ftok(".",'u'), 0);
    
    // declare my message buffer
    struct buf {
        long mtype; // required
        char greeting[50]; // mesg content
    };
    
    buf msg;
    int size = sizeof(msg)-sizeof(long);
    bool cont = true;
    bool receiverB = true;
    int random = -1;
    string message;
    
    while(cont)
    {
        while(random % 997 != 0)
        {
            random = rand() % INT_MAX;
        }
        
        if(random <= 100)//if this is true, sender_997 will terminate
        {
            
            msg.mtype = 61;//send to receiver A
            message = "997, Quit"; //to_string("Quit");
            strcpy(msg.greeting, message.c_str());
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
            
            msgrcv(qid, (struct msgbuf *)&msg, size, 41, 0); // read ack
            cout<<msg.greeting<<endl;//print message
            
            cont = false;
            
        }else
        {
            msg.mtype = 61;//send to receiver A
            message = "997, " + to_string(random);
            strcpy(msg.greeting, message.c_str());
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);
            
            msgrcv(qid, (struct msgbuf *)&msg, size, 41, 0); // read ack
            cout<<msg.greeting<<endl;//print message
            
            //keeps sending to receiverB until receiverB terminates
            if(receiverB)
            {
                cout<<"inside receiverB"<<endl;
                msg.mtype = 62;//send to receiver B
                message = "997, " + to_string(random);
                strcpy(msg.greeting, message.c_str());
                msgsnd(qid, (struct msgbuf *)&msg, size, 0);
                
                msgrcv(qid, (struct msgbuf *)&msg, size, 42, 0); // read ack
                message = msg.greeting;
                if(message.compare("Quit") == 0)
                {
                    receiverB = false;
                }
                cout<<msg.greeting<<endl;//print message
            }
            
        }
        random = -1;
    }
    
    exit(0);
}
