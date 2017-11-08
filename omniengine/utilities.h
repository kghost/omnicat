#pragma once

#include <codecvt>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/code_converter.hpp>
#include <boost/ref.hpp>

#include "shared.h"

namespace Omni {
#ifdef USE_WIDECHAR_API
	class codecvt_utf16 : public std::codecvt<char, wchar_t, std::mbstate_t> {
		// facet for converting between UTF-8 char sequences to UTF-16 wchar_t sequences
	public:
		typedef codecvt<char, wchar_t, std::mbstate_t> _Mybase;
		typedef typename _Mybase::result result;
		typedef char _Elem;
		typedef wchar_t _Byte;
		typedef _Elem intern_type;
		typedef _Byte extern_type;
		typedef std::mbstate_t state_type;

		explicit codecvt_utf16(size_t _Refs = 0)
			: _Mybase(_Refs) {	// construct with ref count
		}

		virtual ~codecvt_utf16() noexcept {	// destroy the object
		}

	protected:
		virtual result do_in(std::mbstate_t& _State,
			const _Byte *_First1, const _Byte *_Last1, const _Byte *& _Mid1,
			_Elem *_First2, _Elem *_Last2, _Elem *& _Mid2) const {
			// convert bytes [_First1, _Last1) to [_First2, _Last)
			_Mid1 = _First1;
			_Mid2 = _First2;

			for (; 0 < _Last1 - _Mid1 && _Mid2 != _Last2; ) {
				// convert a multibyte sequence
				const _Byte *_Ptr = _Mid1;
				unsigned long _Ch;
				unsigned short _Ch0, _Ch1;

				_Ch0 = _Ptr[0];
				std::size_t _Consumed = 0;
				if (_Ch0 < 0xd800 || 0xdc00 <= _Ch0) {	// one word, consume bytes
					_Consumed = 1;
					_Ch = _Ch0;
				} else if (_Last1 - _Mid1 < 2)
					break;
				else {	// get second word
					_Ch1 = _Ptr[1];
					if (_Ch1 < 0xdc00 || 0xe000 <= _Ch1)
						return (_Mybase::error);

					_Consumed = 2;
					_Ch = (unsigned long)(_Ch0 - 0xd800 + 0x0040) << 10 | (_Ch1 - 0xdc00);
				}

				if (_Ch < 0x80) {
					*_Mid2++ = (_Elem)_Ch;
					_Mid1 += _Consumed;
				} else if (_Ch < 0x800) {
					if (_Mid2 + 2 > _Last2) break;
					*_Mid2++ = (_Elem)((_Ch >> 6) & 0x1f | 0xc0);
					*_Mid2++ = (_Elem)(_Ch & 0x3f | 0x80);
					_Mid1 += _Consumed;
				} else if (_Ch < 0x10000) {
					if (_Mid2 + 3 > _Last2) break;
					*_Mid2++ = (_Elem)((_Ch >> 12) & 0xf | 0xe0);
					*_Mid2++ = (_Elem)((_Ch >> 6) & 0x3f | 0x80);
					*_Mid2++ = (_Elem)(_Ch & 0x3f | 0x80);
					_Mid1 += _Consumed;
				} else if (_Ch < 0x110000) {
					if (_Mid2 + 4 > _Last2) break;
					*_Mid2++ = (_Elem)((_Ch >> 18) & 0x7 | 0xf0);
					*_Mid2++ = (_Elem)((_Ch >> 12) & 0x3f | 0x80);
					*_Mid2++ = (_Elem)((_Ch >> 6) & 0x3f | 0x80);
					*_Mid2++ = (_Elem)(_Ch & 0x3f | 0x80);
					_Mid1 += _Consumed;
				} else {
					return (_Mybase::error);	// code too large
				}
			}

			return (_First1 == _Mid1 ? _Mybase::partial : _Mybase::ok);
		}

		virtual result do_out(std::mbstate_t& _State,
			const _Elem *_First1, const _Elem *_Last1, const _Elem *& _Mid1,
			_Byte *_First2, _Byte *_Last2, _Byte *& _Mid2) const {	// convert [_First1, _Last1) to bytes [_First2, _Last)
			_Mid1 = _First1;
			_Mid2 = _First2;

			for (; _Mid1 != _Last1 && 1 <= _Last2 - _Mid2; ) {	// convert and put a wide char
				unsigned long _Ch;
				std::size_t _Remain = 0;
				unsigned char _Ch0 = *_Mid1;
				if ((_Ch0 & 0x80) == 0) {
					_Ch = (unsigned long)_Ch0;
					_Remain = 0;
				} else if ((_Ch0 & 0xe0) == 0xc0) {
					_Ch = (unsigned long)_Ch0 & 0x1f;
					_Remain = 1;
				} else if ((_Ch0 & 0xf0) == 0xe0) {
					_Ch = (unsigned long)_Ch0 & 0xf;
					_Remain = 2;
				} else if ((_Ch0 & 0xf8) == 0xf0) {
					_Ch = (unsigned long)_Ch0 & 0x7;
					_Remain = 3;
				} else {
					return (_Mybase::error);
				}

				if (_Mid1 + _Remain + 1 > _Last1) break;
				for (int i = 1; i <= _Remain; ++i) {
					_Ch0 = _Mid1[i];
					if ((_Ch0 & 0xc0) != 0x80) return (_Mybase::error);
					_Ch <<= 6; _Ch |= (_Ch0 & 0x3f);
				}

				bool _Extra = false;
				if (0x10ffff < _Ch)
					return (_Mybase::error);	// value too large
				else if (_Ch <= 0xffff) {	// one word, can't be code for first of two
					if (0xd800 <= _Ch && _Ch < 0xdc00)
						return (_Mybase::error);
					_Mid1 += _Remain + 1;
					*_Mid2++ = (_Byte)_Ch;
				} else if (_Last2 - _Mid2 < 2) {	// not enough room for two-word output, back up
					return (_Mybase::partial);
				} else {
					_Mid1 += _Remain + 1;
					*_Mid2++ = (_Byte)(0xd800 | (unsigned short)(_Ch >> 10) - 0x0040);
					*_Mid2++ = (_Byte)(0xdc00 | ((unsigned short)_Ch & 0x03ff));
				}
			}

			return (_First1 == _Mid1 ? _Mybase::partial : _Mybase::ok);
		}

		virtual result do_unshift(std::mbstate_t&, _Byte *_First2, _Byte *, _Byte *& _Mid2) const {
			// generate bytes to return to default shift state
			_Mid2 = _First2;
			return (_Mybase::ok);
		}

		virtual int do_length(std::mbstate_t& _State, const _Byte *_First1, const _Byte *_Last1, size_t _Count) const noexcept {
			// return min(_Count, converted length of bytes [_First1, _Last1))
			size_t _Wchars = 0;
			std::mbstate_t _Mystate = _State;

			for (; _Wchars < _Count && _First1 != _Last1; ) {	// convert another wide char
				const _Byte *_Mid1;
				_Elem *_Mid2;
				_Elem _Ch[6];

				switch (do_in(_Mystate, _First1, _Last1, _Mid1,
					_Ch, _Ch + 6, _Mid2)) {	// test result of single wide-char conversion
					case _Mybase::noconv:
						return ((int)(_Wchars + (_Last1 - _First1)));

					case _Mybase::ok:
						_Wchars += _Mid2 - _Ch;
						_First1 = _Mid1;
						break;

					default:
						return ((int)_Wchars);	// error or partial
				}
			}

			return ((int)_Wchars);
		}

		virtual bool do_always_noconv() const noexcept {
			// return true if conversions never change input
			return (false);
		}

		virtual int do_max_length() const noexcept {
			// return maximum length required for a conversion
			return (4);
		}

		virtual int do_encoding() const noexcept {
			// return length of code sequence (from codecvt)
			return (0); // varying length
		}
	};

	template<
		typename Device,
		typename Codecvt = detail::default_codecvt,
		typename Alloc = std::allocator<char>
	> class code_converter_flushable : public boost::iostreams::code_converter<Device, Codecvt, Alloc> {
	public:
		typedef boost::iostreams::code_converter<Device, Codecvt, Alloc> parent;

		template<typename ... Args>
		code_converter_flushable(Args && ... args) : parent(std::forward<Args>(args)...) {}

		struct category : parent::category, boost::iostreams::flushable_tag {};

		bool flush() { return pimpl_->buf_.second().flush(pimpl_->dev()); }
	};

	extern SHARED boost::iostreams::stream<
		code_converter_flushable<std::wistream, codecvt_utf16>
	> utf8cin;

	extern SHARED boost::iostreams::stream<
		code_converter_flushable<std::wostream, codecvt_utf16>
	> utf8cout, utf8cerr;
#endif
}
