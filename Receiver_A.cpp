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
				if(message.compare("Quit") ==0)
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
