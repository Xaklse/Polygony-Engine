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


#include <System.h>


/**
 * The entry point for any Windows program.
 * @param hInstance      integer which identifies each application from the others
 * @param hPrevInstance  obsolete
 * @param lpCmdLine      pointer to a string that contains the command line
 * @param nShowCmd       indicates how the window is to appear when created (minimized...)
 * @return               exit value of the program; 0 means no errors
 */
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
    System* system = new System();

    int result = system->Run();

    delete system;

    return result;
}
