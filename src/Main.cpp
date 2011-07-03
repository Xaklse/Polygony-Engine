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


#include <windows.h> //Includes lots of other header files useful for Windows programming.


/**
 * The entry point for any Windows program.
 * @param hInstance      integer which identifies each application from the others
 * @param hPrevInstance  obsolete
 * @param lpCmdLine      pointer to a string that contains the command line
 * @param nShowCmd       indicates how the window is to appear when created (minimized, maximized...)
 * @return               exit value of the program; 0 means no errors
 */
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
    //Create a "Hello World" message box.
    MessageBox(NULL,L"Hello World!",L"Another Hello World program!",MB_ICONEXCLAMATION | MB_OK);

    return EXIT_SUCCESS; //This macro means that the application was successful.
}
