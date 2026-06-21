#ifndef FILEMANAGER_H
#define FILEMANAGER_H
using namespace std;
#include <string>
#include "University.h"

class FileManager
{
public:
    static void saveAll(const string &folder, const University &uni);
    static void loadAll(const string &folder, University &uni);
};

#endif
