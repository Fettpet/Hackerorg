#ifndef BESTPATHENTRY_H
#define BESTPATHENTRY_H
#include <QString>
#include <ostream>

class BestPathEntry
{
public:
    BestPathEntry();
    BestPathEntry(const BestPathEntry& k);
    BestPathEntry(const QString& path);
    BestPathEntry(const char* path);

    QString path;
    unsigned int posX, posY;
    unsigned int count, stepCount;

    bool operator<(const BestPathEntry &t) const;
    bool operator<(BestPathEntry &t) const;
    bool operator==(const BestPathEntry &t) const;
    bool operator==(BestPathEntry &t) const;
    BestPathEntry& operator = (const BestPathEntry& src);  // Zuweisungsoperator
    bool isReady();
};

std::ostream& operator<< (std::ostream& os, BestPathEntry &val);

#endif // BESTPATHENTRY_H
