#include <iostream>
#include <regex>

int main()
{
    int returnCode = 0;

    try
    {
        std::string const filePathPng("/home/user/Desktop/source/Screenshot from 2022-04-01 13-49-54.png");
        std::string const filePathPdf("/home/user/Desktop/source/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf");
        std::string const filePathIno("/home/user/Desktop/source/subdirectory/arduino_sleep_tutorial_sketch1v1_0_0(1).ino");

        {
            std::regex const regexExtension(".*\\.png$");

            if (true != std::regex_search(filePathPng, regexExtension))
            {
                returnCode = 1;
            }
            if (false != std::regex_search(filePathPdf, regexExtension))
            {
                returnCode = 1;
            }

            if (false != std::regex_search(filePathIno, regexExtension))
            {
                returnCode = 1;
            }
        }

        {
            std::regex const regexFolder(".*/subdirectory/.*");

            if (false != std::regex_search(filePathPng, regexFolder))
            {
                returnCode = 1;
            }
            if (false != std::regex_search(filePathPdf, regexFolder))
            {
                returnCode = 1;
            }

            if (true != std::regex_search(filePathIno, regexFolder))
            {
                returnCode = 1;
            }
        }

        {
            std::regex const regexFile(".*/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf$");

            if (false != std::regex_search(filePathPng, regexFile))
            {
                returnCode = 1;
            }
            if (true != std::regex_search(filePathPdf, regexFile))
            {
                returnCode = 1;
            }

            if (false != std::regex_search(filePathIno, regexFile))
            {
                returnCode = 1;
            }
        }

    }
    catch (...)
    {
        returnCode = 1;
    }

    return returnCode;
}
