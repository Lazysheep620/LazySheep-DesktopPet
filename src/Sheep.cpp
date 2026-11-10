#include "Sheep.h"

#include <QSettings>

#include <algorithm>

namespace {
int clampStat(int value)
{
    return std::clamp(value, 0, 100);
}

int loadStat(const QSettings &settings, const QString &key, int fallback)
{
    bool valid = false;
    const int value = settings.value(key, fallback).toInt(&valid);
    return valid ? clampStat(value) : fallback;
}
}

void Sheep::feed()
{
    hunger_ = clampStat(hunger_ + 15);
}

bool Sheep::canPlay() const
{
    return energy_ >= PlayCost;
}

bool Sheep::play()
{
    if (!canPlay())
        return false;

    mood_ = clampStat(mood_ + 15);
    energy_ = clampStat(energy_ - PlayCost);
    return true;
}

void Sheep::sleep()
{
    energy_ = clampStat(energy_ + 50);
}

void Sheep::load(const QSettings &settings)
{
    hunger_ = loadStat(settings, QStringLiteral("sheep/hunger"), DefaultHunger);
    mood_ = loadStat(settings, QStringLiteral("sheep/mood"), DefaultMood);
    energy_ = loadStat(settings, QStringLiteral("sheep/energy"), DefaultEnergy);
}

void Sheep::save(QSettings &settings) const
{
    settings.setValue(QStringLiteral("sheep/hunger"), hunger_);
    settings.setValue(QStringLiteral("sheep/mood"), mood_);
    settings.setValue(QStringLiteral("sheep/energy"), energy_);
}
