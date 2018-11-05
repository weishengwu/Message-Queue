# Message-Queue-Final




-------------READ ME----------------


OVERVIEW:.........................................................................................
This program consists of a total of 5 executable programs that interact with each other. Each of the three senders sends an “event” to all receivers.

Sender 997:
requires an acknowledgement message for every message sent to a receiver. It terminates whenever it generates a random number less than 100.

Sender 251:
does accept any acknowledgement message. It only reports its events to one receiver and terminates on a kill command from a separate terminal.

Sender 257:
only notifies one receiver as well. It terminates when its receiver stops receiving its event notifications. Both receivers are expected to receive a message and display the value and the sender’s identity.

Receiver A:
accepts messages from senders 251 and 997 and sends an acknowledgement notification back to 997. It only terminates after both of its senders terminate.

Receiver B:
accepts messages from 257 and 997 and sends an acknowledgement notification back to 997. It terminates after receiving 5000 messages.


ADDITIONS:...........................................................................................

A minimally intrusive software patch must be used for signal handling on Sender 251 to send a message queue to the correct receiver when we execute the command “kill -10<pid>” in a separate terminal. The patch runs in the background while the loop executes and executes when Sender 251 gets a kill signal. Then it sends the receiver the very last message before it exists. This software patch also does not remove the message queue, this has to be done manually from the user.

We used “g++ Sender_251.cpp patch64-18.o” to compile the 2 files, which link and fused together to produce a single executable.



DIFFICULTIES:........................................................................................

We had to view and clear the message queues every-time before compiling and running the programs.
To view the queue we used “ipcs -q” in the command terminal
To clear the clear we used: “ipcrm -Q (KEY#)”

Compiling and executing 5 programs in 5 separate terminals was somewhat extraneous, it would've been easier to run having 2 programs one Receivers program and one Senders program or a main program that handles all the Senders and Receivers.

CPU is running too fast and Sender 251 is filling up the queue too quickly with a divisor of 251.







