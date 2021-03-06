#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>

#include <meave/lib/math/funcs_approx.hpp>
#include <meave/lib/gettime.hpp>
#include <meave/lib/utils.hpp>
#include <meave/lib/raii/mmap_pointer.hpp>
#include <meave/lib/hash/hash.hpp>
#include <meave/lib/hash/fnv.hpp>

#define ARRAY_LEN (20000000)

namespace {

void test(const ::size_t step) {
	meave::raii::MMapPointer<::uint8_t> src{{ARRAY_LEN + 32}};

	for (uns i = 0; i < ARRAY_LEN; ++i) {
		src[i] = int(::rand() % 128);
	}

	if (0 == step % 4) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::fnv::naive1(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive/fnv1: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive/fnv1:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::fnv::naive1a(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive/fnv1a: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive/fnv1a:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		meave::vec::AVX hash{ .f8_ = _mm256_setzero_ps() };
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash.i8_ = _mm256_xor_si256( hash.i8_, meave::fnv::naive1_vec(&src[i], step).i8_ );
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive/1_vec: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash.qw_[0] << ":" << hash.qw_[1] << ":" << hash.qw_[2] << ":" << hash.qw_[3] << "; " << $::dec << $::setfill(' ') << e - b << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive/1_vec: " << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		meave::vec::AVX hash{ .f8_ = _mm256_setzero_ps() };
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash.i8_ = _mm256_xor_si256( hash.i8_, meave::fnv::naive1a_vec(&src[i], step).i8_ );
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive/1a_vec: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash.qw_[0] << ":" << hash.qw_[1] << ":" << hash.qw_[2] << ":" << hash.qw_[3] << "; " << $::dec << $::setfill(' ') << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive/1a_vec: " << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		::uint32_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::naive<13>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		::uint32_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::sse_1_1<13>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_1_1: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_1_1:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		::uint32_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::basic<13>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " basic: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " basic:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::basic_duo<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " basic_duo: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " basic_duo:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::basic_both<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " basic_both: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " basic_both:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::naive2<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive2: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive2:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::naive3<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive3: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive3:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 4) {
		auto hash = meave::rolhash::naive4<13, 17>(nullptr, 0);
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			const auto x = meave::rolhash::naive4<13, 17>(&src[i], step);
			hash._[0] ^= x._[0];
			hash._[1] ^= x._[1];
			hash._[2] ^= x._[2];
			hash._[3] ^= x._[3];
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive4: " << "0x" << $::hex << $::setfill('0') << hash._[0] << ":" << hash._[1] << ":" << hash._[2] << ":" << hash._[3] << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " naive4:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 16) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::sse_2<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2: " << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 16) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::sse_2_1<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_1: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_1: " << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 16) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::sse_2_2<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_2: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_2: " << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 16) {
		meave::vec::SSE hash{ .f4_ = _mm_setzero_ps() };
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash.i4_ = _mm_xor_si128( hash.i4_, meave::rolhash::sse_2_3<13, 17>(&src[i], step).i4_ );
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_3: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash.qw_[0] << ":" << hash.qw_[1] << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_3: " << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 16) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::sse_2_4<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_4: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " sse_2_4: " << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 32) {
		::uint32_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::asm_avx2(&src[i], step, 13);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " asm_avx2: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " asm_avx2:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 32) {
		::uint32_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::avx2<13>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " avx2:" << $::setw(16) << "NA" << $::endl;
	}
	if (0 == step % 32) {
		::uint64_t hash = 0;
		const double b = meave::getrealtime();
		for (::size_t i = 0; i + step <= ARRAY_LEN; i += step) {
			hash ^= meave::rolhash::avx2_2<13, 17>(&src[i], step);
		}
		const double e = meave::getrealtime();
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " avx2_2: " << "0x" << $::setw(16) << $::hex << $::setfill('0') << hash << $::dec << $::setfill(' ') << "; " << (e - b) << "seconds" << $::endl;
	} else {
		$::cerr << "step: " << $::setw(8) << $::right << step << ";" << $::setw(12) << $::right << " avx2_2:" << $::setw(16) << "NA" << $::endl;
	}

	$::cerr << $::endl;
}

} /* anonymous namespace */

int
main(int argc, char *argv[]) {
	test(atoi(argv[1]));
	return 0;
}
