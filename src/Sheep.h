#pragma once

class QSettings;

class Sheep
{
public:
    int hunger() const { return hunger_; }
    int mood() const { return mood_; }
    int energy() const { return energy_; }

    void feed();
    bool canPlay() const;
    bool play();
    void sleep();

    void load(const QSettings &settings);
    void save(QSettings &settings) const;

private:
    static constexpr int DefaultHunger = 60;
    static constexpr int DefaultMood = 70;
    static constexpr int DefaultEnergy = 50;
    static constexpr int PlayCost = 15;

    int hunger_ = DefaultHunger;
    int mood_ = DefaultMood;
    int energy_ = DefaultEnergy;
};
