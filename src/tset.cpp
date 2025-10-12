// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
    if (mp < 0) {
        throw std::invalid_argument("mp<0 is bad");
    }
}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf)
{
}

TSet::operator TBitField()
{
    return TBitField(0);
}


int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < 0) {
        throw std::invalid_argument("Elem<0 is bad");
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem<0 || Elem>MaxPower) {
        throw std::invalid_argument("Elem is bad");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem<0 || Elem>MaxPower) {
        throw std::invalid_argument("Elem is bad");
    }
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if (MaxPower == s.MaxPower && BitField == s.BitField) {
        return 1;
    }
    else {
        return 0;
    }
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    int newMaxPower = max(MaxPower, s.MaxPower);
    TSet result(newMaxPower);
    for (int i = 0; i < MaxPower; i++) {
        if (BitField.GetBit(i)) {
            result.BitField.SetBit(i);
        }
    }
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.BitField.GetBit(i)) {
            result.BitField.SetBit(i);
        }
    }
    return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::invalid_argument("Elem is bad");
    }
    TSet res(*this);
    res.BitField.SetBit(Elem);
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::invalid_argument("Elem is bad");
    }
    TSet res(*this);
    res.BitField.ClrBit(Elem);
    return res;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    TBitField res(BitField & s.BitField);
    return TSet(res);
}

TSet TSet::operator~(void) // дополнение
{
    return TSet(~BitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    string input;
    istr >> input;
    for (int i = 0; i < s.GetMaxPower(); i++) {
        s.DelElem(i);
    }
    int lengthToProcess = min((int)input.length(), s.GetMaxPower());
    for (int i = 0; i < lengthToProcess; i++) {
        if (input[i] == '1') {
            s.InsElem(i);
        }
        else if (input[i] == '0') {
            s.DelElem(i);
        }
        else {
            throw invalid_argument("bad elem");
        }
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (int i = 0; i < s.GetMaxPower(); i++) {
        ostr << s.IsMember(i);
    }
    return ostr;
}
