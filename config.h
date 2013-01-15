#ifndef CONFIG_H
#define CONFIG_H

/**
 * @file
 * @brief Defines global variables or preprocessor defines values.
 * @author BOUTTER Loïc
 * @version 1
 * @date 2012-11
 */

// Debug constant : we want some debugs cout and methods
#define DEBUG

#include <QString>

namespace Config
{
    extern unsigned int cMajorVersion; ///< The major version of Lemurien
    extern unsigned int cMinorVersion; ///< The minor version of Lemurien
    extern unsigned int cMaxMark; ///< The maximum mark you can give to a song. Can be 5, 10, 20, ... Default: 5
    /*extern bool         cFadeChaining; ///< IS there a fading between 2 songs ?
    extern int          cFadeTime; ///< Fading time in msec
    extern QString      cIntroSong; ///< Filepath of intro song (splash screen)*/
}

#endif // CONFIG_H
