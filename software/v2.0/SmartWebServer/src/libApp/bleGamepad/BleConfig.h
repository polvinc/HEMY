// bleGamepad configuration file
#pragma once

  //***********************************************************************************************************
  // Configuration settings
  //***********************************************************************************************************

  // ON/OFF unless noted otherwise

  // Direction button swaps
  #define NS_SWAP              OFF      // ON reverses direction buttons from OnStep standard
  #define EW_SWAP              OFF      // East/West swap

  #define LED_ENABLE            ON      // LED on/off behavior, OFF disables LED BLE connected/disconnected
                                        // indication. ON = LED off when connected, on when disconnected
                                        // REV = LED on when connected, off when disconnected

  #define FOCUS_INIT            ON      // Focuser moves to half-position on M button "Start tracking" press,
                                        // moves to "0" position on "M" button "Park". OFF disables feature

  #define SOUND                 ON      // ON, Enables beeps and alerts

  #define M_BUTTON              ON      // ON, Add addtional functions to M button.  
                                        // For supported GamePads only.

  #define INIT_ALIGN            ON      // ON, Enables M button align from "At Home" (startup) with 3 
                                        // star alignment (see ALIGNSTARS to change number of stars) 

  #define MODE_D               OFF      // OFF, uses Mode B, ON use Mode D for GamePad if it supports it.   
                                        // V2.0 code for backward compatabilty.
  
  #define M_SINGLE_CLICK       OFF      // OFF, ON, M button will activate on a single click as in  
                                        // V2.0 code, for backward compatabilty.                                                                                    

  //***********************************************************************************************************
