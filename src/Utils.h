/*
 * Utils.h
 *
 *  Created on: Nov 3, 2013
 *      Author: egv
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <FIo.h>
#include <FBase.h>

using namespace Tizen::Base;
using namespace Tizen::Io;

class Utils
{
    public:
        static Utils& getInstance()
        {
            static Utils	instance; // Guaranteed to be destroyed.
            						  // Instantiated on first use.
            return instance;
        }

        String *accessToken();
        void setAccessToken(const String& acessToken);

    private:
        Registry registry;
        String ACCESS_TOKEN_KEY;
        String MAIN_SECTION_NAME;


        Utils();                   // Constructor? (the {} brackets) are needed here.

        // Dont forget to declare these two. You want to make sure they
        // are unaccessable otherwise you may accidently get copies of
        // your singleton appearing.
        Utils(Utils const&);              // Don't Implement
        void operator=(Utils const&); // Don't implement};
};
#endif /* UTILS_H_ */
