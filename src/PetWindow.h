#pragma once

#include "Sheep.h"

#include <QPixmap>
#include <QPoint>
#include <QSettings>
#include <QTimer>
#include <QWidget>

#include <array>

class PetWindow : public QWidget
{
public:
    PetWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    enum class State { Idle, Walk, Sleep, Eat, Happy, Count };
    static constexpr int FrameCount = 4;
    using Frames = std::array<QPixmap, FrameCount>;

    void setState(State state, int durationMs = 0);
    void wake();
    void save();
    QPoint fitOnScreen(const QPoint &position) const;

    Sheep sheep_;
    QSettings settings_;
    std::array<Frames, static_cast<int>(State::Count)> frames_;
    State state_ = State::Idle;
    int frame_ = 0;
    QTimer animation_;
    QTimer stateTimeout_;

    bool sleeping_ = false;
    bool leftPressed_ = false;
    bool dragging_ = false;
    QPoint pressGlobal_;
    QPoint pressPosition_;
};
