/* 

This is a simple illustration of the use of:
	ftok, msgget, msgsnd, msgrcv

Program B creates a message queue to be shared with Program A.
Then, they will pass messages back and forth.

Program A sends the first message and reads the reply. Program A
also sends a "fake" message to the msgQ that will never be read
by Program B.

Both child processes use message type mtype = 113 and 114.

*/

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
	string temp;
	int sender;
	bool sender997 = true;
	bool sender251 = true;
	
	
	while(sender997 || sender251)
	{
		msgrcv(qid, (struct msgbuf *)&msg, size, 61, 0); // read mesg
		message = msg.greeting;
		temp = message.substr(0,3);
		cout<<"temp = "<<temp<<endl;
		sender = stoi(temp);
		message = message.substr(4);
		
		switch(sender)
		{
			case 997:
				if(message.compare("Quit") ==0)
				{
					msg.mtype = 41;
					strcpy(msg.greeting, "Sender_997 quit successful");
					cout<<"Sender_997 quit successful"<<endl;
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
				if(message.compare("Quit") ==0)
				{
					sender251 = false;
				}
				cout<<"Message "<<message<<" from Sender_251 Received"<<endl;
				break;
		}
		
	}

	
	
	// now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
}


