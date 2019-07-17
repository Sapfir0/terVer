#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <kyplukList.h>
#include <kyplukSmartPtr.h>
#include <kyplukDecimal.h>
#include <QString>

using decimal = kypluk::Decimal<12>;


class abstractElement {
    public:
        virtual decimal calculate_breaks() = 0; // вывести вероятность поломки
        virtual QString getFormul() = 0;
        virtual ~abstractElement() {}
};

class realElement: public abstractElement {
    public:
        realElement(const decimal& prob) {
            this->prob = prob;
        }

        realElement(const QString& name) {
            this->name = name;
        }

        realElement(const decimal& prob, const QString& name) {
            this->prob = prob;
            this->name = name;
        }

        realElement(const realElement& other) {
            prob = other.prob;
            name = other.name;
        }

        decimal calculate_breaks() {
            return prob;
        }

        QString getFormul() {
            return name;
        }

    protected:
        decimal prob; // вероятность поломки данного элемента
        QString name;
};

class virtualElement : public abstractElement {
    public:
        enum Type { TypeParallel, TypeSerial };
        class typeElementError : public kypluk::Exception {};
        virtualElement(Type typeska) {
            type = typeska;
        }

        virtualElement(const virtualElement& other) {
            content = other.content;
            type = other.type;
        }

        ~virtualElement() {
            content.clear();
        }

        decimal calculate_breaks() {
            decimal res(1);
            if (type == TypeSerial) {
                for (auto item : content) {
                    res *= item->calculate_breaks();
                }
            }
            elif (type == TypeParallel) {
                for (auto item : content) {
                    res *= (decimal(1) - item->calculate_breaks());
                }
                res = decimal(1) - res;
            } else {
                throw typeElementError();
            }

            return res;
        }

        QString getFormul() {
            QString res;
            if (type == TypeSerial) {
                res = "(";
                for (auto it = content.begin(); it != content.end(); ++it) {
                    if (it == content.begin()) {
                        res += (*it)->getFormul();
                    } else {
                        res += " * " + (*it)->getFormul();
                    }
                }
                res += ")";
            }
            elif (type == TypeParallel) {
                res = "(1 - (";
                for (auto it = content.begin(); it != content.end(); ++it) {
                    if (it == content.begin()) {
                        res += "(1 - " + (*it)->getFormul() + ")";
                    } else {
                        res += " * (1 - " + (*it)->getFormul() + ")";
                    }
                }
                res += "))";
            } else {
                throw typeElementError();
            }

            return res;
        }

        virtualElement& setParallel() {
            type = TypeParallel;
            return *this;
        }
        virtualElement& setSerial() {
            type = TypeSerial;
            return *this;
        }

        // добавить элемент
        virtualElement& addElement(const virtualElement& child) {
            content.push_back(new virtualElement(child));
            return *this;
        }

        virtualElement& addElement(const realElement& child) {
            content.push_back(new realElement(child));
            return *this;
        }

    protected:
        kypluk::List <kypluk::SharedPtr<abstractElement>> content; // содержимое
        Type type;

};

#endif // ELEMENTS_H
