#include <complex>
#include <memory>
#include <vector>

namespace
{
    using cd = std::complex<double>;
    
}

#define PI  3.14159265358979323846

int reverse(int num, int lg_n) {
    int res = 0;
    for (int i = 0; i < lg_n; i++) {
        if (num & (1 << i))
            res |= 1 << (lg_n - 1 - i);
    }
    return res;
}

std::vector<cd> fft(const std::vector<cd> &num, bool invert) {
    std::vector<cd> fft_image{num};
    int n = num.size();
    int lg_n = 0;
    while ((1 << lg_n) < n)
        lg_n++;

    for (int i = 0; i < n; i++)
    {
        if (i < reverse(i, lg_n))
            std::swap(fft_image[i], fft_image[reverse(i, lg_n)]);
    }

    for (int len = 2; len <= n; len <<= 1)
    {
        double ang = 2 * PI / len * (invert ? 1 : -1);
        cd wlen = std::polar(1.0, ang);
        for (int i = 0; i < n; i += len)
        {
            cd w(1);
            for (int j = 0; j < len / 2; j++)
            {
                cd u = fft_image[i+j], v = fft_image[i+j+len/2] * w;
                fft_image[i+j] = u + v;
                fft_image[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert)
    {
        for (cd & x : fft_image)
            x /= n;
    }

    return fft_image;
}
