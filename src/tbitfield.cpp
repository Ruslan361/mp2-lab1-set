// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>

// Fake variables used as placeholders in tests
//static const int FAKE_INT = -1;
//static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0) throw invalid_argument("length of bitfield can not be < 0");
    if (len == 0) {
        pMem = nullptr;
        BitLen = 0;
        MemLen = 0;
    }
    else {
        BitLen = len;
        MemLen = (len - 1) / sizeof(TELEM) / 8 + 1;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = (TELEM)0;
        }
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (MemLen == 0) {
        pMem = nullptr;
    }
    else
    {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
}

TBitField::~TBitField()
{
    if (pMem != nullptr) {
        delete[] pMem;
    }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM mask = (TELEM)1;
    if (n >= sizeof(TELEM) * 8)
        throw invalid_argument("invalid argument n >= " + std::to_string(sizeof(TELEM) * 8));
    mask = mask << n;
    return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0) throw invalid_argument("you can not set negative bit");
    if (n >= BitLen) throw invalid_argument("you can not set to much index");
    TELEM mask = 00000001u;
    int bit_pos = n & (sizeof(TELEM) * 8 - 1); //%
    int mem_pos = GetMemIndex(n);
    mask = mask << bit_pos;
    pMem[mem_pos] = pMem[mem_pos] | mask;
    return;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0) throw invalid_argument("you can not clear negative bit");
    if (n >= BitLen) throw invalid_argument("you can not clear to much index");
    TELEM mask = 00000001u;
    int bit_pos = n & (sizeof(TELEM) * 8 - 1);
    mask = mask << bit_pos;
    mask = ~mask;
    int mem_pos = GetMemIndex(n);
    pMem[mem_pos] = pMem[mem_pos] & mask;
    return;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0) throw invalid_argument("you can not get negative bit");
    if (n >= BitLen) throw invalid_argument("you can not get to much index");
    TELEM mask = 00000001u;
    int bit_pos = n & (sizeof(TELEM) * 8 - 1);
    int mem_pos = GetMemIndex(n);
    mask = mask << bit_pos;
    return pMem[mem_pos] & mask;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (bf.pMem != pMem)
    {
        if (MemLen != bf.MemLen)
        {
            TELEM* pNew = new TELEM[bf.MemLen];
            if (pNew != nullptr)
            {
                delete[] pMem;
                pMem = pNew;
            }
            else
            {
                throw "Memory exception";
            }
        }
        MemLen = bf.MemLen;
        BitLen = bf.BitLen;
        for (int i = 0; i < MemLen; i++)
        {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen) return false;
    int current_mem_index = MemLen - 1;
    for (int i = 0; i < current_mem_index; i++)
    {
        if (pMem[i] != bf.pMem[i])
            return false;
    }

    int currentIndex = BitLen & (sizeof(TELEM) * 8 - 1);
    TELEM mask = 0;
    mask = ~mask;
    mask = mask >> (sizeof(TELEM) * 8) - currentIndex;

    TELEM this_last = pMem[current_mem_index];
    TELEM bf_last = bf.pMem[current_mem_index];
    this_last = this_last & mask;
    bf_last = bf_last & mask;
    if (this_last == bf_last)
        return true;
    return false;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return ~(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int min_length = bf.MemLen < this->MemLen ? bf.MemLen : this->MemLen;
    int min_bit_length = bf.BitLen < this->BitLen ? bf.BitLen : this->BitLen;
    int max_bit_length = bf.BitLen > this->BitLen ? bf.BitLen : this->BitLen;
    TBitField bitfield = TBitField(max_bit_length);
    for (size_t i = 0; i < min_length; i++)
    {
        bitfield.pMem[i] = this->pMem[i] | bf.pMem[i];
    }
    return bitfield;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int min_length = bf.MemLen < this->MemLen ? bf.MemLen : this->MemLen;
    int min_bit_length = bf.BitLen < this->BitLen ? bf.BitLen : this->BitLen;
    int max_bit_length = bf.BitLen > this->BitLen ? bf.BitLen : this->BitLen;
    TBitField bitfield = TBitField(max_bit_length);
    for (size_t i = 0; i < min_length; i++)
    {
        bitfield.pMem[i] = this->pMem[i] & bf.pMem[i];
    }
    return bitfield;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField current = TBitField(*this);
    for (int i = 0; i < current.MemLen; i++)
    {
        current.pMem[i] = ~current.pMem[i];
    }
    int size = BitLen & (sizeof(TELEM) * 8 - 1);
    TELEM mask = 0;
    mask = ~mask;
    mask = mask >> (sizeof(TELEM) * 8) - size;
    current.pMem[current.MemLen - 1] = current.pMem[current.MemLen - 1] & mask;
    return current;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    std::string str;
    istr >> str;
    bf = TBitField(str.size());
    for (int i = 0; i < bf.BitLen; i++)
    {
        if (str[i] == '1')
            bf.SetBit(i);
        else if (str[i] == 0)
        {
            bf.ClrBit(i);
        }
        else
        {
            throw exception("Wrong element" + str[i]);
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    int size = sizeof(TELEM) * 8;
    int k = bf.BitLen;
    for (int i = 0; i < bf.MemLen; i++)
    {
        for (int j = 0; (j < size)&&(k > 0); j++)
        {
            TELEM mask = bf.GetMemMask(j);
            TELEM current = bf.pMem[i] & mask;
            k--;
            if (current > 0)
                ostr << 1;
            else
                ostr << 0;
        }
    }
    return ostr;
}