#ifndef MENU_DATA_H
#define MENU_DATA_H

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Each of the Menu*Data structures is all the data a menu needs to track.  It is not possible to just put this data
// in a global because there can be multiple instances of each menu (for example you could have 2 sound modules
// connected and each of those would have a sound menu with seperate data.  So in the g_ctx an array of the
// of all the Menu*Data is stored.  The g_ctx stores data for each module connected so to know how much memory
// to have to store the Menu*Data for each module a union of all the Menu*Data is done to fine the worst case size.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Sound Menu
extern void MenuSoundInit(uint8 modId);
extern void MenuSoundSendPackets(uint8 modId);
extern void MenuSoundReceivePackets(uint8 modId, uint8 *packet);
extern void MenuSoundActiveInit(uint8 modId);
extern uint8 MenuSoundTriggerCheck(uint8 modId);

// Add your new function declarations above 
//  Note - These declarations are needed because these go in the proc table and the Arduino IDE doesn't do it's 
//         declaration magic on header files.

typedef struct {
  uint32 nextSendUpdate;
  hwPortPin ppSound;
  uint16 triggerVal;
} MenuSoundData;

// Add new Menu*Data structs above this and add that struct to the union below

typedef union
{
  MenuSoundData menuSoundData;
} ModStore;

#endif // MENU_DATA_H