//--------------------------------------------------------------------------------
//
// Copyright (c) 1999 @COMPANY Solutions
//
// Programming by Rich Schonthal
//
//--------------------------------------------------------------------------------

#ifndef _WAITLEN_H_
#define _WAITLEN_H_

// these defines are used for WaitForSingleObject calls (or CSingleLock or whatever the case is)
// to define the max wait len for an event to take place
// all times are in miliseconds

//--------------------------------------------------------------------------------
// NT Service stuff

// time to wait for stop event to signal
#define WAITLEN_STOP 100
#define WAITLEN_PAUSE 100
#define WAITLEN_CONTINUE 50


//--------------------------------------------------------------------------------
// time allowed for the input daemon to wait for its thread to start
// also used for thread to init
#define WAITLEN_INPUT_THREAD_INIT 8000

// wait for a new message to become available
#define WAITLEN_NEXT_MSG 50


//--------------------------------------------------------------------------------
// input daemon thread

// the amount of time to wait for the server to come online
// when first starting up
#define WAITLEN_INITIAL_SERVER_WAIT 1000

// wait for the next msg to be processed before exiting SendHL7Message
#define WAITLEN_PROCESS_NXT_MSG 60000

// sleep interval for the monitor to wait for the server 
#define WAITLEN_MONITOR_WAITS_FOR_SERVER 100
#define WAITLEN_MONITOR_WAITS_FOR_SERVER_ITER 100

// time to wait for input thread's blocking when trying to pause
#define WAITLEN_BLOCK_TO_PAUSE	INFINITE

//--------------------------------------------------------------------------------
// HL7 Monitor

// when waiting for the server to process a msg, the monitor sleeps then checks for data avail
// from the server
// WAITLEN_SERVER_WAIT_SLEEP is the sleep interval
// WAITLEN_SERVER_WAIT_ITER is the # of times to check (and sleep) before assuming the server is down
#define WAITLEN_SERVER_WAIT_SLEEP 50
#define WAITLEN_SERVER_WAIT_ITER 500


// the # of times and sleep time for monitor to attempt to open a server connection
// see CHL7Monitor::ConnectToServer
// try for two minutes to connect
#define WAITLEN_SERVER_CONNECT_SLEEP 1000
#define WAITLEN_SERVER_CONNECT_ITER 120

#endif // _WAITLEN_H_
