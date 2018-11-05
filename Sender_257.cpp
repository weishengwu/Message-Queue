//
//  Sender_257.cpp
//  Message_Queue_Final
//
//  Created by Trevor Anderson, Jonathan Ascencio, Brittney Witts, Weisheng Wu.
//  Copyright © 2018 Trevor Anderson, Jonathan Ascencio, Brittney Witts, Weisheng Wu.
//  All rights reserved.
//

/*
SENDER 257
 
 -Generates random numbers pre-assigned with divisor integer of 257
 -Doesnt accept any acknoledgement messages
 -Only notifies Receiver B.
 -It terminates when Receiver B stops receiving its event notifications.

 
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
#include "get_info.h"
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
    
    int random = -1;
    string message;
    while(cont)
    {
        while(random % 257 != 0)
        {
            random = rand() % INT_MAX;
        }
        msg.mtype = 62; //send to Receiver_B
        message = "257, " + to_string(random);
        strcpy(msg.greeting, message.c_str());
        msgsnd(qid, (struct msgbuf *)&msg, size, 0); //sends message to Receiver_B
        msgrcv(qid, (struct msgbuf *)&msg, size, 43, 0); //receives flag from Receiver_B
        
        message = msg.greeting;
        
        if(message.compare("Dead") == 0) //terminates when Receiver B stops receiving messages
        {
            cont = false;
        }
        
        random = -1;
        
    }
    
}
