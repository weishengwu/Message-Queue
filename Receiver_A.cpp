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
	int sender;
	bool sender997 = true;
	bool sender251 = true;


	while(sender997 || sender251)
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
				if(message.compare("Quit") ==0)
				{
					msg.mtype = 41;

					cout<<"Message "<<message<<" from Sender_997 Received Sender_997 has quit"<<endl;
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
				if(message.compare("Quit") ==0)
				{
					sender251 = false;
				}
				cout<<"Message "<<message<<" from Sender_251 Received"<<endl;
				break;
		}

	}


	/*					// don't read "fake" mesg
	cout << getpid() << ": gets message" << endl;
	cout << "message: " << msg.greeting << endl;

	strcat(msg.greeting, " and Adios.");
	cout << getpid() << ": sends reply" << endl;
	msg.mtype = 314; // only reading mesg with type mtype = 314
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	cout << getpid() << ": now exits" << endl;

	msgrcv (qid, (struct msgbuf *)&msg, size, -112, 0);
	msgrcv (qid, (struct msgbuf *)&msg, size, 0, 0);
	msgrcv (qid, (struct msgbuf *)&msg, size, 117, 0);
	*/
	// now safe to delete message queue
	msgctl (qid, IPC_RMID, NULL);

	exit(0);
}
