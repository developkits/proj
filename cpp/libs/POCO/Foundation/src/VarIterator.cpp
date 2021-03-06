//
// VarIterator.cpp
//
// $Id: //poco/Main/Data/src/VarIterator.cpp#1 $
//
// Library: Foundation
// Package: Dynamic
// Module:  VarIterator
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/Dynamic/VarIterator.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Dynamic/Struct.h"
#undef min
#undef max
#include <limits>


namespace Poco {
namespace Dynamic {


const int VarIterator::POSITION_END = std::numeric_limits<std::size_t>::max();


VarIterator::VarIterator(Var* pVar, bool positionEnd): 
	_pVar(pVar),
	_position(positionEnd ? POSITION_END : 0)
{
}


VarIterator::VarIterator(const VarIterator& other):
	_pVar(other._pVar),
	_position(other._position)
{
}


VarIterator::~VarIterator()
{
}


VarIterator& VarIterator::operator = (const VarIterator& other)
{
	VarIterator tmp(other);
	swap(tmp);
	return *this;
}


void VarIterator::swap(VarIterator& other)
{
	using std::swap;
	
	swap(_pVar, other._pVar);
	swap(_position, other._position);
}


void VarIterator::increment() const
{
	if (POSITION_END == _position)
		throw RangeException("End of iterator reached.");

	if (_position < _pVar->size() - 1)
		++_position;
	else
		_position = POSITION_END;
}


void VarIterator::decrement() const
{
	if (0 == _position)
		throw RangeException("Beginning of iterator reached.");
	else if (POSITION_END == _position)
		_position = _pVar->size() - 1;
	else
		--_position;
}


void VarIterator::setPosition(std::size_t pos) const
{
	if (_position == pos) return;

	if (pos < _pVar->size())
		_position = pos;
	else if (pos == _pVar->size())
		_position = POSITION_END;
	else
		throw RangeException("Invalid position argument.");
}


Var& VarIterator::operator * () const
{
	if (POSITION_END == _position)
		throw InvalidAccessException("End of iterator reached.");

	return _pVar->operator[](_position);
}


Var* VarIterator::operator -> () const
{
	if (POSITION_END == _position)
		throw InvalidAccessException("End of iterator reached.");

	return &_pVar->operator[](_position);
}


const VarIterator& VarIterator::operator ++ () const
{
	increment();
	return *this;
}


VarIterator VarIterator::operator ++ (int) const
{
	VarIterator old(*this);
	increment();
	return old;
}


const VarIterator& VarIterator::operator -- () const
{
	decrement();
	return *this;
}


VarIterator VarIterator::operator -- (int) const
{
	VarIterator old(*this);
	decrement();
	return old;
}


VarIterator VarIterator::operator + (std::size_t diff) const
{
	VarIterator ri(*this);
	ri.setPosition(_position + diff);
	return ri;
}


VarIterator VarIterator::operator - (std::size_t diff) const
{
	if (diff > _position) throw RangeException("Invalid position argument.");
	VarIterator ri(*this);
	ri.setPosition(_position - diff);
	return ri;
}


} } // namespace Poco::Dynamic
