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
	
	
	for(int i = 0; i<5000; i++)
	{
		//gets message
		msgrcv(qid, (struct msgbuf *)&msg, size, 62, 0); // read mesg
		message = msg.greeting;		
		sender = message.substr(0,3);
		message = message.substr(4);
		
		if(sender.compare("997") == 0)
		{
			//sneds ack to sender_997
			msg.mtype = 41;
			
			strcpy(msg.greeting, "Message from Sender_997 Received");
			cout<<"Message "<<message<<" from Sender_997 Received"<<endl;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		}else
		{
			//sends flag to 257
			cout<<"Message "<<message<<" from Sender_257 Received"<<endl;
			strcpy(msg.greeting, "Still Alive");
			msg.mtype = 43
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			
		}
		
			
	}
	
	//sends quit to sender_997
	strcpy(msg.greeting, "Quit");
	cout<<"Receiver_B has terminated"<<endl;
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	
	//sends flag to sender_257
	strcpy(msg.greeting, "Dead");
	msg.mtype = 43;
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	
	


	exit(0);
}


