/*
 *       ___          ___          ___   ___          ___          ___          ___       ___       
 *      /\  \        /\  \        /\__\ |\__\        /\  \        /\  \        /\__\     |\__\      
 *     /::\  \      /::\  \      /:/  / |:|  |      /::\  \      /::\  \      /::|  |    |:|  |     
 *    /:/\:\  \    /:/\:\  \    /:/  /  |:|  |     /:/\:\  \    /:/\:\  \    /:|:|  |    |:|  |     
 *   /::\-\:\  \  /:/  \:\  \  /:/  /   |:|__|__  /:/  \:\  \  /:/  \:\  \  /:/|:|  |__  |:|__|__   
 *  /:/\:\ \:\__\/:/__/ \:\__\/:/__/    /::::\__\/:/__/_\:\__\/:/__/ \:\__\/:/ |:| /\__\ /::::\__\  
 *  \/__\:\/:/  /\:\  \ /:/  /\:\  \   /:/  /    \:\  /\ \/__/\:\  \ /:/  /\/__|:|/:/  //:/  /      
 *       \::/  /  \:\  /:/  /  \:\  \ /:/  /      \:\ \:\__\   \:\  /:/  /     |:/:/  //:/  /       
 *        \/__/    \:\/:/  /    \:\  \\/__/        \:\/:/  /    \:\/:/  /      |::/  / \/__/        
 *                  \::/  /      \:\__\             \::/  /      \::/  /       /:/  /               
 *                   \/__/        \/__/              \/__/        \/__/        \/__/                
 *               ___          ___          ___                     ___          ___                 
 *              /\  \        /\__\        /\  \         ___       /\__\        /\  \                
 *             /::\  \      /::|  |      /::\  \       /\  \     /::|  |      /::\  \               
 *            /:/\:\  \    /:|:|  |     /:/\:\  \      \:\  \   /:|:|  |     /:/\:\  \              
 *           /::\-\:\  \  /:/|:|  |__  /:/  \:\  \     /::\__\ /:/|:|  |__  /::\-\:\  \             
 *          /:/\:\ \:\__\/:/ |:| /\__\/:/__/_\:\__\ __/:/\/__//:/ |:| /\__\/:/\:\ \:\__\            
 *          \:\-\:\ \/__/\/__|:|/:/  /\:\  /\ \/__//\/:/  /   \/__|:|/:/  /\:\-\:\ \/__/            
 *           \:\ \:\__\      |:/:/  /  \:\ \:\__\  \::/__/        |:/:/  /  \:\ \:\__\              
 *            \:\ \/__/      |::/  /    \:\/:/  /   \:\__\        |::/  /    \:\ \/__/              
 *             \:\__\        /:/  /      \::/  /     \/__/        /:/  /      \:\__\                
 *              \/__/        \/__/        \/__/                   \/__/        \/__/                
 *
 * Polygony Engine, since 25th Jun 2011
 * by Javier "Xaklse" Osset
 */


#include <windows.h> //Header files useful for Windows programming.


#include "System.h"


/**
 * The entry point for any program based on Windows.
 * @param instanceHandle  integer which identifies this application
 * @param hPrevInstance   obsolete
 * @param lpCommandLine   long pointer to string that contains the command line
 * @param nShowCommand    indicates how the window is to appear when created
 * @return                exit value of the program; 0 means no errors
 */
int WINAPI WinMain(HINSTANCE instanceHandle,HINSTANCE hPrevInstance,
    LPSTR lpCommandLine,int nShowCommand)
{
    Poly::System* pSystem = new Poly::System();

    int result = pSystem->Run(instanceHandle,string(lpCommandLine));

    delete pSystem;

    return result;
}
