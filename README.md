# Airlines Seat Assign Helper

//***********************************  PROGRAM DESCRIPTION  *******************************************
//*                                                                                                   *
//*   PROCESS: This program assign seats to passengers for Southern Comfort Airlines. It reads in     *
//*   passenger data then assign seats for them on various flights according to the rules.            *
//*   #Rule 1: Specific seat requests are honored first.                                              * 
//*   #Rule 2: If a requested seat is already assigned to another passenger, the person should be     *
//*    place in the same row, if possible, starting from the left side (lowest seat number).          *
//*   #Rule 3: If the requested row is full, the passenger should be seated as far forward as         *
//*    possible in the requested column but within the requested section.                             *
//*   #Rule 4: If the request using Rule 2 or Rule 3 cannot be honored, the passenger should be       * 
//*    assigned the first available seat starting from the front and left of the requested section    *
//*    going across each row until a vacant seat is found.                                            * 
//*   #Rule 5: First Class and Coach request must be honored.  If there are no seats in the requested *
//*    section, the passenger's Boarding Number must go on a waiting list for the next flight.        * 
//*                                                                                                   * 
//*****************************************************************************************************
