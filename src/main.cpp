#include "PetWindow.h"

#include <QApplication>
#include <QCoreApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("LazySheep"));
    QCoreApplication::setApplicationName(QStringLiteral("LazySheep-DesktopPet"));
    QApplication::setApplicationDisplayName(QStringLiteral("LazySheep"));
    QApplication::setWindowIcon(QIcon(QStringLiteral(":/sprites/idle_0.png")));

    PetWindow pet;
    pet.show();
    return app.exec();
}
