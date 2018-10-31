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
	
	msg.mtype = 61;
	strcpy(msg.greeting, "251, Quit");
	
	get_info(qid, (struct msgbuf *)&msg, size, 61); //sends quit message to receiver_A after termination
	
	
	int random = -1;
	string message;
	while(true)
	{
		while(random % 251 != 0)
		{
			random = rand() % INT_MAX;
		}
		
		message = "251, " + to_string(random);
		strcpy(msg.greeting, message.c_str());
		
		msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		random = -1;
		
	}
	exit(0);
}
