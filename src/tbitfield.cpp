// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) throw "len < 0";
    this->BitLen = len;
    if (BitLen % (sizeof(TELEM)*8) == 0) MemLen = len / (sizeof(TELEM)*8);
    else MemLen = (len / (sizeof(TELEM)*8)) + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}



TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (size_t i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    if (pMem != nullptr) delete[] pMem;
    pMem = nullptr;
    BitLen = 0;
    MemLen = 0;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n > BitLen) throw "Error index";
    return n / (sizeof(TELEM)*8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n > BitLen) throw "Error index";
    return 1 << ((n-1) % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen) throw "Error index";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen) throw "Error index";
    pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen) throw "Error index!";
    if (!(pMem[GetMemIndex(n)] & GetMemMask(n))) return 0;
    else return 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) {
        if (pMem != nullptr) {
            delete[] pMem;  
        }
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;

}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) {
    TBitField a(std::max(BitLen, bf.BitLen));  
    for (int i = 0; i < a.MemLen; i++) {
        if (i < MemLen) a.pMem[i] |= pMem[i];  
        if (i < bf.MemLen) a.pMem[i] |= bf.pMem[i]; 
    }
    return a;
}


TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{ 
    TBitField a(std::max(BitLen, bf.BitLen)); 
    for (int i = 0; i <= GetMemIndex(min(BitLen, bf.BitLen)); i++) {
        a.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return a;
}


TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(*this);
    for (size_t i = 0; i < tmp.BitLen; i++)
    {
        if (tmp.GetBit(i)) tmp.ClrBit(i);
        else tmp.SetBit(i);
    }
    return tmp;
}


// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    
    int len = 0;
    cout << "Enter the len: ";
    istr >> len;
    if (len != bf.BitLen) throw "error len";
    for (int i = 0; i < len; i++) {
        int f = 0;
        istr >> f;
        if (f == 1) bf.SetBit(i);
        else if (f == 0) bf.ClrBit(i);
        else throw "error index";
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    ostr << "{";
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    ostr << "}" << endl;
    return ostr;
}
