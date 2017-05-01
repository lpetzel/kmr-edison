// NOT USED ANYMORE:
// The edison is now stateless
#pragma once

// Common States for all machines
// ==============================
// These states are flags
// usually only one of then should be active at a time (currently)
//
// The actual states might reuse these states.
// Example: STATE_MY_STATE := STATE_IDLE | 3
// will create a state based on the idle state (for lights)
// and will give it the sub number 3 (for distinguishing)
// All common states are multiples of 512

// Some state transission did not work
#define STATE_ILLEGAL            0x200

// Machine is down (all lights off)
// Can be started via protobuf message
// Machine waits on signal to wake up
#define STATE_DOWN               0x400 
// Machine is out of order
// Can be started via protobuf message
// Machine waits on signal to wake up
#define STATE_OUT_OF_ORDER       0x800         
// State is broken -> eg product was
// fed without proper setup
// Red any Yellow light are blinking with 2Hz
// NOTE: currently the same as STATE_ILLEGAL
#define STATE_BROKEN            STATE_ILLEGAL
// Idle states:
// In this states one can interact with the machines.
// Green lights are on
#define STATE_IDLE              0x2000 
// Buisy state:
// The machine is processing something. Yellow light is on
#define STATE_BUISY             0x4000  
// The machine has accepted the setup command
// (green led is blinking up to 3 sec)
#define STATE_SETUP_ACCEPT     0x8000          
                
                
// States for all stations with a band
// ===================================
#define STATE_READY_FOR_PICK_UP STATE_IDLE | 1             
#define STATE_MOVE_OUT          STATE_BUISY | 1                 

// Not setup state, for all machines, which have to be
// configured
// ===================================================
#define STATE_WAIT_FOR_SETUP          STATE_IDLE | 2                 

// Wait for product state for all machines, which need input
// =========================================================
#define STATE_WAIT_FOR_PRODUCT  STATE_IDLE | 3                         

// States for the base station
// ===========================
// NOTE: All fetched by band states already
// #define STATE_DELIVER                
// #define STATE_READY_FOR_PICK_UP  STATE_IDLE              
                
// States for the Delivery station
// ===============================
#define STATE_CONSUMING_PRODUCT STATE_BUISY | 2                          

// States for the Ring station
// ===========================
#define STATE_PROCESS_BASE      STATE_BUISY | 3                     
#define STATE_MOUNT_RING        STATE_BUISY | 4                   

// States for the Cap station
// ==========================
#define STATE_DISMOUNTING_CAP   STATE_BUISY | 5                        
#define STATE_MOUNTING_CAP      STATE_BUISY | 6                     



inline const char* state2string( const unsigned int state) {
  if (state & STATE_ILLEGAL) 
    return "ILLEGAL TRANSISION";
  switch (state) {
    case STATE_DOWN:
      return "DOWN";
    case STATE_OUT_OF_ORDER:
      return "OUT OF ORDER";
    case STATE_BROKEN:
      return "BROKEN";
    case STATE_IDLE:
      return "IDLE";
    case STATE_BUISY:
      return "BUISY";
    case STATE_SIGNAL_ACCEPT:
      return "SIGNAL ACCEPT";
    case STATE_READY_FOR_PICK_UP:
      return "READY FOR PICK UP";
    case STATE_MOVE_OUT:
      return "MOVE OUT";
    case STATE_WAIT_FOR_SETUP:
      return "WAIT FOR SETUP";
    case STATE_WAIT_FOR_PRODUCT:
      return "WAIT FOR PRODUCT";
    case STATE_CONSUMING_PRODUCT:
      return "CONSUMING PRODUCT";
    case STATE_PROCESS_BASE:
      return "PROCESS BASE";
    case STATE_MOUNT_RING:
      return "MOUNT RING";
    case STATE_DISMOUNTING_CAP:
      return "DISMOUNTING CAP";
    case STATE_MOUNTING_CAP:
      return "MOUNTING CAP";
    default:
      // TODO: maybe it's better to throw an exception here.
      return "UNKNOWN STATE"; }
}
