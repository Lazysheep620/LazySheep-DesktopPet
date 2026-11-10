#include "PetWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>

#include <algorithm>

PetWindow::PetWindow()
    : QWidget(nullptr, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint)
{
    setWindowTitle(tr("LazySheep"));
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_QuitOnClose);
    setFixedSize(192, 160);
    setCursor(Qt::OpenHandCursor);

    const std::array<const char *, static_cast<int>(State::Count)> names{
        "idle", "walk", "sleep", "eat", "happy"
    };
    for (std::size_t state = 0; state < names.size(); ++state) {
        for (int frame = 0; frame < FrameCount; ++frame) {
            auto &pixmap = frames_[state][frame];
            const QString path = QStringLiteral(":/sprites/%1_%2.png")
                                     .arg(QString::fromLatin1(names[state])).arg(frame);
            if (!pixmap.load(path))
                qFatal("Missing or unreadable sprite: %s", qPrintable(path));
        }
    }

    connect(&animation_, &QTimer::timeout, this, [this] {
        frame_ = (frame_ + 1) % FrameCount;
        update();
    });
    stateTimeout_.setSingleShot(true);
    connect(&stateTimeout_, &QTimer::timeout, this, [this] {
        setState(sleeping_ ? State::Sleep : State::Idle);
    });

    sheep_.load(settings_);
    sleeping_ = settings_.value(QStringLiteral("sheep/sleeping"), false).toBool();
    const QRect area = QGuiApplication::primaryScreen()->availableGeometry();
    const QPoint defaultPosition = area.bottomRight() - QPoint(width() + 23, height() + 23);
    move(fitOnScreen(settings_.value(QStringLiteral("window/position"), defaultPosition).toPoint()));
    setState(sleeping_ ? State::Sleep : State::Idle);
}

void PetWindow::setState(State state, int durationMs)
{
    stateTimeout_.stop();
    state_ = state;
    frame_ = 0;
    switch (state) {
    case State::Walk:
        animation_.start(105);
        break;
    case State::Eat:
    case State::Happy:
        animation_.start(180);
        break;
    case State::Idle:
    case State::Sleep:
    case State::Count:
        animation_.start(320);
        break;
    }
    if (durationMs > 0)
        stateTimeout_.start(durationMs);
    update();
}

void PetWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, false);
    painter.drawPixmap(rect(), frames_[static_cast<int>(state_)][frame_]);
}

void PetWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        QWidget::mousePressEvent(event);
        return;
    }

    leftPressed_ = true;
    dragging_ = false;
    pressGlobal_ = event->globalPosition().toPoint();
    pressPosition_ = pos();
    event->accept();
}

void PetWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (!leftPressed_ || !(event->buttons() & Qt::LeftButton)) {
        QWidget::mouseMoveEvent(event);
        return;
    }

    const QPoint delta = event->globalPosition().toPoint() - pressGlobal_;
    if (!dragging_ && delta.manhattanLength() >= QApplication::startDragDistance()) {
        dragging_ = true;
        setCursor(Qt::ClosedHandCursor);
        setState(State::Walk);
    }
    if (dragging_)
        move(pressPosition_ + delta);
    event->accept();
}

void PetWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !leftPressed_) {
        QWidget::mouseReleaseEvent(event);
        return;
    }

    leftPressed_ = false;
    if (dragging_) {
        dragging_ = false;
        setCursor(Qt::OpenHandCursor);
        move(fitOnScreen(pos()));
        setState(sleeping_ ? State::Sleep : State::Idle);
        save();
    } else if (sleeping_) {
        wake();
    } else {
        setState(State::Happy, 900);
    }
    event->accept();
}

void PetWindow::contextMenuEvent(QContextMenuEvent *event)
{
    if (leftPressed_) {
        event->accept();
        return;
    }

    QMenu menu(this);
    QAction *stats = menu.addAction(tr("Hunger %1   Mood %2   Energy %3")
                                        .arg(sheep_.hunger()).arg(sheep_.mood()).arg(sheep_.energy()));
    stats->setEnabled(false);
    menu.addSeparator();
    QAction *feed = menu.addAction(tr("Feed"));
    QAction *play = menu.addAction(sheep_.canPlay() ? tr("Play") : tr("Play (needs 15 energy)"));
    play->setEnabled(sheep_.canPlay());
    QAction *sleep = menu.addAction(sleeping_ ? tr("Wake up") : tr("Sleep"));
    menu.addSeparator();
    QAction *quit = menu.addAction(tr("Quit"));

    const QPoint menuPosition = event->reason() == QContextMenuEvent::Keyboard
        ? mapToGlobal(rect().center()) : event->globalPos();
    QAction *chosen = menu.exec(menuPosition);
    if (chosen == feed) {
        sleeping_ = false;
        sheep_.feed();
        setState(State::Eat, 1250);
        save();
    } else if (chosen == play && sheep_.play()) {
        sleeping_ = false;
        setState(State::Happy, 1250);
        save();
    } else if (chosen == sleep) {
        if (sleeping_) {
            wake();
        } else {
            sleeping_ = true;
            sheep_.sleep();
            setState(State::Sleep);
            save();
        }
    } else if (chosen == quit) {
        close();
    }
    event->accept();
}

void PetWindow::closeEvent(QCloseEvent *event)
{
    save();
    QWidget::closeEvent(event);
}

void PetWindow::wake()
{
    sleeping_ = false;
    setState(State::Idle);
    save();
}

void PetWindow::save()
{
    sheep_.save(settings_);
    settings_.setValue(QStringLiteral("sheep/sleeping"), sleeping_);
    settings_.setValue(QStringLiteral("window/position"), pos());
    settings_.sync();
}

QPoint PetWindow::fitOnScreen(const QPoint &position) const
{
    const QPoint center = position + rect().center();
    QScreen *screen = QGuiApplication::screenAt(center);
    if (!screen) {
        const QRect window(position, size());
        int largestOverlap = 0;
        for (QScreen *candidate : QGuiApplication::screens()) {
            const QRect overlap = window.intersected(candidate->availableGeometry());
            const int overlapArea = overlap.width() * overlap.height();
            if (overlapArea > largestOverlap) {
                largestOverlap = overlapArea;
                screen = candidate;
            }
        }
    }
    if (!screen)
        screen = QGuiApplication::primaryScreen();

    const QRect area = screen->availableGeometry();
    const int maxX = std::max(area.left(), area.right() - width() + 1);
    const int maxY = std::max(area.top(), area.bottom() - height() + 1);
    return QPoint(std::clamp(position.x(), area.left(), maxX),
                  std::clamp(position.y(), area.top(), maxY));
}
