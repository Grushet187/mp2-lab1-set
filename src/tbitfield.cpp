// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0) {
        throw invalid_argument("len<0");
    }
    BitLen = len;
    MemLen = (len == 0) ? 1 : (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
    pMem = new TELEM[MemLen]{ 0 };
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n<0 || n>=BitLen) {
        throw invalid_argument("n<0 or n>BitLen");
    }
    return int(n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen) {
        throw invalid_argument("n<0 or n>BitLen");
    }
    int bit_pos = n % (sizeof(TELEM) * 8);
    return TELEM(1) << bit_pos;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen) {
        throw invalid_argument("n<0 or n>BitLen");
    }
    int ind = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    pMem[ind] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen) {
        throw invalid_argument("n<0 or n>BitLen");
    }
    int ind = GetMemIndex(n);
    TELEM mask = ~GetMemMask(n);
    pMem[ind] &= mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen) {
        throw invalid_argument("n<0 or n>BitLen");
    }
    int ind = GetMemIndex(n);
    TELEM mask = GetMemMask(n);
    return int((pMem[ind] & mask) != 0);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf) return *this;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i != MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen) {
        for (int i = 0; i != MemLen; i++) {
            if (pMem[i] != bf.pMem[i]) {
                return 0;
            }
        }
        return 1;
  }
    return 0;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen == bf.BitLen) {
        for (int i = 0; i < MemLen; i++) {
            if (pMem[i] != bf.pMem[i]) {
                return 1;
            }
        }
        return 0;
  }
  return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int maxLen = max(BitLen, bf.BitLen);
    TBitField res(maxLen);
    for (int i = 0; i < MemLen; i++) {
        if (i < res.MemLen) res.pMem[i] |= pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        if (i < res.MemLen) res.pMem[i] |= bf.pMem[i];
    }
    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int maxLen = max(BitLen, bf.BitLen);
    TBitField result(maxLen);
    int minLen = min(BitLen, bf.BitLen);
    for (int i = 0; i < minLen; i++) {
        if (GetBit(i) && bf.GetBit(i)) {
            result.SetBit(i);
        }
    }
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~pMem[i];
    }
    if (BitLen > 0) {
        int extraBits = BitLen % (sizeof(TELEM) * 8);
        if (extraBits != 0) {
            TELEM mask = (TELEM(1) << extraBits) - 1;
            result.pMem[MemLen - 1] &= mask;
        }
    }

    return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int length;
    istr >> length;
    if (length <= 0) {
        istr.setstate(ios::failbit);
        return istr;
    }
    if (bf.BitLen != length) {
        TBitField temp(length);
        bf = temp;
    }
    char ch;
    for (int i = 0; i < bf.BitLen; i++) {
        istr >> ch;

        if (ch == '1') {
            bf.SetBit(i);
        }
        else if (ch == '0') {
            bf.ClrBit(i);
        }
        else {
            istr.setstate(ios::failbit);
            break;
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
