// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <string>

TBitField::TBitField(int len)
{
	if (len < 0) throw "Incorrect input.";
	BitLen = len;
	MemLen = (BitLen - 1) / (8 * sizeof(TELEM)) + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0;i < MemLen;i++)
	{
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0;i < MemLen;i++)
	{
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n < 0) || (n >= BitLen)) throw "Incorrect input";
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n < 0) || (n >= BitLen)) throw "Incorrect input";
	TELEM i = 1;
	return i << (n & (sizeof(TELEM) * 8 - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	int i = GetMemIndex(n);
	if ((n < 0) || (n >= BitLen)) throw "Incorrect input";
	pMem[i] = pMem[i] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen)) throw "Incorrect input";
	int i = GetMemIndex(n);
	pMem[i] = pMem[i] & ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen)) throw "Incorrect input";
	int i = GetMemIndex(n);
	if ((pMem[i] & GetMemMask(n)) == GetMemMask(n))
		return 1;
	else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (&bf != this) 
	{
		if (BitLen != bf.BitLen)
		{
			BitLen = bf.BitLen;
			MemLen = bf.MemLen;
			pMem = new TELEM[MemLen];
			for (int i = 0; i < MemLen;i++)
			{
				pMem[i] = bf.pMem[i];
			}
		}
		else
		{
			for (int i = 0; i < MemLen;i++)
			{
				pMem[i] = bf.pMem[i];
			}
		}
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	int flag = 1;
	if (BitLen != bf.BitLen)
		flag = 0;
	else 
	{
		for (int i = 0; i < MemLen; i++)
	    {
			if (pMem[i] != bf.pMem[i])
		    {
				flag = 0;
			    break;
		    }
	    }
    }
	return flag;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  int flag = 0;
	if (BitLen != bf.BitLen)
		flag = 1;
	else
	{
		for (int i = 0; i < MemLen; i++)
		{
			if (pMem[i] != bf.pMem[i])
			{
				flag = 1;
				break;
			}
		}
	}
	return flag;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int i, len = BitLen;
	if (bf.BitLen > len)
		len = bf.BitLen;
	TBitField temp(len);
	for (i = 0; i < MemLen; i++)
	{
		temp.pMem[i] = pMem[i];
	}
	for (i = 0; i < bf.MemLen; i++)
	{
		temp.pMem[i] |= bf.pMem[i];
    }
	return temp;

}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int i, blen = BitLen, mlen = MemLen;
	if (bf.BitLen > blen)
	{
		blen = bf.BitLen;
		mlen - bf.MemLen;
	}
	TBitField temp(blen);
	for (i = 0; i < mlen; i++)
	{
		temp.pMem[i] = pMem[i] & bf.pMem[i];
    }
	return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField temp(BitLen);
	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i) == 0)
			temp.SetBit(i);
		else temp.ClrBit(i);
	}
	return temp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string temp;
	istr >> temp;
	bf = TBitField(temp.size());
	for (int i = 0; i < bf.BitLen; i++)
	{
		if (temp[i] == '1')
			bf.SetBit(i);
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; ++i)
		ostr << bf.GetBit(i);
	return ostr;
}
