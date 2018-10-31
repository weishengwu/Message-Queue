/* 

This is a simple illustration of the use of:
	ftok, msgget, msgsnd, msgrcv

Program A will use a message queue created by Program B. 
Then, they will pass messages back and forth.

Program A sends the first message and reads the reply. Program A
also sends a "fake" message to the msgQ that will never be read
by anyone.

Both child processes use message type mtype = 113 and 114.

*/

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
	bool receiverB = false;
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
			
			msg.mtype = 61;//send to receiver a
			message = "997, Quit"; //to_string("Quit");
			strcpy(msg.greeting, message.c_str());
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			
			msgrcv(qid, (struct msgbuf *)&msg, size, 41, 0); // read ack
			cout<<msg.greeting<<endl;//print message
			
			cont = false;
			
			
		}else
		{
			msg.mtype = 61;//send to receiver a
			message = "997, " + to_string(random);
			strcpy(msg.greeting, message.c_str());
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			
			msgrcv(qid, (struct msgbuf *)&msg, size, 41, 0); // read ack
			cout<<msg.greeting<<endl;//print message
			
			//keeps sending to receiverB until receiverB terminates
			if(receiverB)
			{
				msg.mtype = 62;//send to receiver b
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


