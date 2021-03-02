#include <QCoreApplication>
#include <iostream>
#include <vector>
#include "sniffer.h"
#include <QDebug>

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Sniffer *s = new Sniffer();
    vector<pair<char*, char*>> d;
    if(s->getDevs(d))
        for(int i = 0; i < d.size(); i++){
            string a(d[i].first);
            string b;
            if(d[i].second != NULL)
                b = d[i].second;
            else
                b = "No description available";
            cout << i + 1 << ": " << a << "\t" << b << "\n";
        }

    return a.exec();
}
