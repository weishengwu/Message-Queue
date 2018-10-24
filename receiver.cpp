//
//  receiver.cpp
//  Message-Queue
//
//  Created by Trevor Anderson on 10/24/18.
//  Copyright © 2018 Trevor Anderson. All rights reserved.
//

#include "receiver.hpp"

#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>


#define SENDER_TYPE1 251
#define SENDER_TYPE2 997
#define SENDER_TYPE3 257
//Receiver types
#define RECEIVER_TYPE1 1


#define RECEIVER_TYPE2 2


#define MQ_KEY 1234
#define MSG_TYPE_ACK 1
#define MSG_TYPE_DATA 2
#define MSG_TYPE_TERM 1
using namespace std;
/*
 * Declare the message structure.
 */


struct buf {
    int    mtype;
    int data;
} ;


typedef struct buf message_buf;




int receiver_type = 0;


int msqid;
int num_rec_term = 0;
int sender_id1,sender_id2;


void recieve(int type);
int main(int argc, char *argv[])
{
    //Get Receiver type from Command line
    //IF Type 1 , Receiver from Sender whose marker value is 251 or 977
    //IF Type 2 , Receiver from Sender whose marker value is 257 or 977
    if(argc < 1){
        cout << "Enter Valid Sender Type" << endl;
        return 0;
    }
    
    
    receiver_type = atoi(argv[1]);
    
    
    if((receiver_type != RECEIVER_TYPE1) && (receiver_type != RECEIVER_TYPE2)){
        cout << "Enter Valid Sender Type" << endl;
        return 0;
    }
    
    
    recieve(receiver_type);
    
    
    
    
    
    
    return 0;
}


void recieve(int type){
    int mq_id1,mq_id2;
    int mqflg =  0666;
    int num_sender_term = 0;
    int numREc =0;
    //Assign Message Queue Ids based on Sender types
    if(type == RECEIVER_TYPE1){
        sender_id1 = SENDER_TYPE1;
        sender_id2 = SENDER_TYPE2;
        
        
    }
    else{
        sender_id1 = SENDER_TYPE2;
        sender_id2 = SENDER_TYPE3;
    }
    //Open Message Queues created by senders to receive messages
    if ((mq_id1 = msgget(sender_id1, mqflg )) < 0) {
        perror("msgget");
        exit(1);
    }
    else {
        cout << "Mesage Queue created" << endl;
    }
    
    
    if ((mq_id2 = msgget(sender_id2, mqflg )) < 0) {
        perror("msgget");
        exit(1);
    }
    else {
        cout << "Mesage Queue created" << endl;
    }
    //Read Messages infinetely
    while(true)
    {
        message_buf rec,ack;
        /*
         * Receive Message from Sender -1.
         */
        if (msgrcv(mq_id1, &rec, sizeof(message_buf), 1, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }
        //Print Event received
        cout<<"Msg Recieved in Receiver :: "<<type <<"  Sender :: "<<sender_id1<<"  Msg Type :: "<<rec.mtype<<"  Data :: "<<rec.data ;
        //If sender send terminate message the Terminate if all Senders terminated(If receiver type -1)
        if(receiver_type == RECEIVER_TYPE1 && ack.mtype == MSG_TYPE_TERM){
            num_sender_term++;
            if(num_sender_term == 2){
                cout<<"Both Senders Terminated"<<endl;
                exit(1);
            }
        }
        ack.data = 0;
        //If reciver type -2 and 5000 messages are received then terminate
        if(receiver_type == RECEIVER_TYPE2){
            numREc++;
            if(numREc == 5000){
                ack.mtype = MSG_TYPE_TERM;
            }
            else{
                ack.mtype = MSG_TYPE_ACK;
            }
        }
        else{
            ack.mtype = MSG_TYPE_ACK;
        }
        
        
        /*
         * Send Acnowledgement.
         */
        if (msgsnd(mq_id1, &ack, sizeof(message_buf), IPC_NOWAIT) < 0) {
            perror("msgsnd");
            exit(1);
        }
        if(ack.mtype == MSG_TYPE_TERM)
        {
            cout<<"Reached 500 Messages Quit"<<endl;
            exit(1);
        }
        /*
         * Receive Message from Sender -2 and Do same procedure as Sender-1 , Duplicate code.
         */
        if (msgrcv(mq_id2, &rec, sizeof(message_buf), 1, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }
        cout<<"Msg Recieved in Receiver :: "<<type <<"  Sender :: "<<sender_id2<<"  Msg Type :: "<<rec.mtype<<"  Data :: "<<rec.data ;
        if(receiver_type == RECEIVER_TYPE1 && ack.mtype == MSG_TYPE_TERM){
            num_sender_term++;
            if(num_sender_term == 2){
                cout<<"Both Senders Terminated"<<endl;
                exit(1);
            }
        }
        ack.data = 0;
        if(receiver_type == RECEIVER_TYPE2){
            numREc++;
            if(numREc == 5000){
                ack.mtype = MSG_TYPE_TERM;
            }
            else{
                ack.mtype = MSG_TYPE_ACK;
            }
        }
        else{
            ack.mtype = MSG_TYPE_ACK;
        }
        
        
        
        
        /*
         * Send Acnowledgement.
         */
        if (msgsnd(mq_id1, &ack, sizeof(message_buf), IPC_NOWAIT) < 0) {
            perror("msgsnd");
            exit(1);
        }
        if(ack.mtype == MSG_TYPE_TERM)
        {
            cout<<"Reached 500 Messages Quit"<<endl;
        }
        
        
    }
    
    
}

