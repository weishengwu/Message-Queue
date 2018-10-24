//
//  sender.cpp
//  Message-Queue
//
//  Created by Trevor Anderson on 10/24/18.
//  Copyright © 2018 Trevor Anderson. All rights reserved.
//

#include "sender.hpp"

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <signal.h>


//Sender Types, Change values according to the Programme
#define SENDER_TYPE1 251
#define SENDER_TYPE2 997
#define SENDER_TYPE3 257




//Type of Events Data event (based on Division by Marker), Acnowldge ment , Send Signal to receivers when terminated
#define MSG_TYPE_ACK 1
#define MSG_TYPE_DATA 2
#define MSG_TYPE_TERM 1


using namespace std;
/*
 * Declare the message structure. To receive Message
 */


struct buf {
    int    mtype; //Type of Message Data/Ack/Terminate mesage
    int data;
} ;


typedef struct buf message_buf;


//Signal handler to receive kill signal
void sig_handler(int);


//Current sender typr
int sender_type = 0;
//Falgs to create Message Queue
int mqflg = IPC_CREAT | 0666;


int msqid;
int num_rec_term = 0;
int main(int argc, char *argv[])
{
    //Read Sender type , With Marker 251/977/257
    if(argc < 1){
        cout << "Enter Valid Sender Type" << endl;
        return 0;
    }
    
    
    sender_type = atoi(argv[1]);
    
    
    if((sender_type != SENDER_TYPE1) && (sender_type != SENDER_TYPE2) && (sender_type != SENDER_TYPE3)){
        cout << "Enter Valid Sender Type" << endl;
        return 0;
    }
    
    
    //Create a Message Queue , with Id  251/977/257 . Receiver will receive Events
    if ((msqid = msgget(sender_type, mqflg )) < 0) {
        perror("msgget");
        exit(1);
    }
    else {
        cout << "Mesage Queue creation failed" << endl;
    }
    //Assign Signal Handler to receive Kill Signal
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    
    
    //Start sending Events
    
    
    while(true)
    {
        //Generate Random Number
        int r = (rand() % 100) + 1;
        cout<<"Random Value :: "<<r<<endl;
        message_buf sbuf;
        sbuf.data = r;
        //IF Sender Marker in 977 and Random Value is 100 then terminate and send terminate message
        if(sender_type == SENDER_TYPE2 && r < 100){
            sbuf.mtype = MSG_TYPE_TERM;
        }
        else{
            sbuf.mtype = MSG_TYPE_DATA;
        }
        int div = r % sender_type;
        if(div != 0 && sbuf.mtype != MSG_TYPE_TERM){
            continue;
        }
        /*
         * Send a message.
         */
        if (msgsnd(msqid, &sbuf, sizeof(message_buf), IPC_NOWAIT) < 0) {
            perror("msgsnd");
            exit(1);
        }
        else{
            printf("Message: \"%d\" Sent\n", sbuf.mtype);
        }
        
        
        if(sender_type == SENDER_TYPE2 && r < 100){
            exit(1);
        }
        //Receive Acknowledgement
        message_buf ack;
        /*
         * Receive an answer of message type 1.
         */
        if (msgrcv(msqid, &ack, sizeof(message_buf), 1, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }
        if(sender_type == SENDER_TYPE2 && ack.mtype == MSG_TYPE_ACK){
            continue;
        }
        //If Terminate Message Received and Receiver is terminated
        else{
            
            
            if(ack.mtype == MSG_TYPE_TERM){
                exit(1);
            }
            else{
                continue;
            }
        }
        
        
    }
    
    
    return 0;
}


void sig_handler(int sig) {
    message_buf sbuf;
    sbuf.data = 0;
    sbuf.mtype = MSG_TYPE_TERM;
    
    
    /*
     * Terminate and Send a Terminate message message.
     */
    if (msgsnd(msqid, &sbuf, sizeof(message_buf), IPC_NOWAIT) < 0) {
        perror("msgsnd");
        exit(1);
    }
    exit(1);
}
