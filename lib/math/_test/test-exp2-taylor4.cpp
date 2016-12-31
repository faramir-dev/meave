#include <cmath>
#include <iostream>

#include <meave/lib/math/funcs_approx.hpp>
#include <meave/lib/gettime.hpp>
#include <meave/lib/utils.hpp>
#include <meave/lib/raii/mmap_pointer.hpp>

#define ARRAY_LEN (20000*1024 + 24)

namespace {

void test() {
	meave::raii::MMapPointer<float> src{{ARRAY_LEN}};
	meave::raii::MMapPointer<float> dst_exp2f{{ARRAY_LEN}};
	meave::raii::MMapPointer<float> dst_exp2_taylor{{ARRAY_LEN}};
	meave::raii::MMapPointer<float> dst_exp2_taylor_one{{ARRAY_LEN}};
	meave::raii::MMapPointer<float> dst_exp256{{ARRAY_LEN}};

	for (uns i = 0; i < ARRAY_LEN; ++i) {
		src[i] = dst_exp2f[i] = dst_exp2_taylor[i] = dst_exp2_taylor_one[i] = dst_exp256[i] = (int(i) - ARRAY_LEN/2) / 1000000.f;
	}

	{
		const double b1 = meave::getrealtime();
		for (uns i = 0; i < ARRAY_LEN; ++i) {
			dst_exp2f[i] = ::exp2f(dst_exp2f[i]);
		}
		const double e1 = meave::getrealtime();
		$::cerr << "libc: " << (e1 - b1) << $::endl;
	}

	{
		const double b2 = meave::getrealtime();
		::exp2_taylor(*dst_exp2_taylor, *src, ARRAY_LEN);
		const double e2 = meave::getrealtime();
		$::cerr << "taylor4: " << (e2 - b2) << $::endl;
	}

	{
		const double b = meave::getrealtime();
		for (uns i = 0; i < ARRAY_LEN; i += 8) {
			*(v8sf*)&dst_exp2_taylor_one[i] = ::exp2_taylor_one(*(v8sf*)&src[i]);
		}
		const double e = meave::getrealtime();
		$::cerr << "taylor4_one: " << (e - b) << $::endl;
	}
	{
		// This is calculation of exp(x), not exp2(x), it is here to compare speed!
		const double b = meave::getrealtime();
		for (uns i = 0; i < ARRAY_LEN; i += 8) {
			*(v8sf*)&dst_exp256[i] = ::exp256_ps(*(v8sf*)&src[i]);
		}
		const double e = meave::getrealtime();
		$::cerr << "exp256: " << (e - b) << $::endl;
	}

	float max_abs_err = 0.f;
	float max_rel_err = 0.f;
	float max_abs_err_taylor_one = 0.f;
	float max_rel_err_taylor_one = 0.f;
	$::cout << "Value" << '|' << "Exp2" << '|' << "Exp2-qd" << '|' << "Exp2-one" << $::endl;
	for (uns i = 0; i < ARRAY_LEN; ++i) {
		$::cout << src[i] << '|' << dst_exp2f[i] << '|' << dst_exp2_taylor[i] << '|' << dst_exp2_taylor_one[i] << $::endl;
		{
			const float abs_err = ::fabs(dst_exp2f[i] - dst_exp2_taylor[i]);
			const float rel_err = abs_err / dst_exp2f[i];
			max_abs_err = $::max(abs_err, max_abs_err);
			max_rel_err = $::max(rel_err, max_rel_err);
		}
		{
			const float abs_err = ::fabs(dst_exp2f[i] - dst_exp2_taylor_one[i]);
			const float rel_err = abs_err / dst_exp2f[i];
			max_abs_err_taylor_one = $::max(abs_err, max_abs_err_taylor_one);
			max_rel_err_taylor_one = $::max(rel_err, max_rel_err_taylor_one);
		}

	}
	$::cerr << "taylor:     abs-err:" << max_abs_err            << "; rel-err:" << max_rel_err            << $::endl;
	$::cerr << "taylor-one: abs-err:" << max_abs_err_taylor_one << "; rel-err:" << max_rel_err_taylor_one << $::endl;
}

} /* anonymous namespace */

int
main(void) {
	test();

	return 0;
}