#ifndef LESSON_H
#define LESSON_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>

class Lesson {
public:
    QString subject, teacher, auditorium, num, type;
    QStringList expectations;

    Lesson(const QString& s, const QString& t, const QString& a, const QString& n, const QString& tp, const QStringList& ex)
        : subject(s), teacher(t), auditorium(a), num(n), type(tp), expectations(ex) {}

    bool saveToFile() const {
        QFile file("result.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            QString expStr = expectations.isEmpty() ? "Нет" : expectations.join(", ");
            out << "Предмет: " << subject << " | Препод: " << teacher 
                << " | Ауд: " << auditorium << " | Пара: " << num 
                << " | Тип: " << type << " | Ожидание: " << expStr << "\n";
            file.close();
            return true;
        }
        return false;
    }
};

#endif // LESSON_H