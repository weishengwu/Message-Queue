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

void get_info(int, struct msgbuf *, int, long);

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
		
		message = "257, " + to_string(random);
		strcpy(msg.greeting, message.c_str());
		//sends message to Receiver_B
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		//receives flag from Receiver_B
		msgrcv(qid, (struct msgbuf *)&msg, size, 43, 0); 
		
		message = msg.greeting;
		
		if(message.compare("Dead") == 0)
		{
			cont = false;
		}
		
		
	}
	
}
