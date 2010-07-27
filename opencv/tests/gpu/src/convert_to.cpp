#include "gputest.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <limits>
#include <numeric>

using namespace cv;
using namespace std;
using namespace gpu;

class CV_GpuMatOpConvertTo : public CvTest
{
    public:
        CV_GpuMatOpConvertTo();
        ~CV_GpuMatOpConvertTo();
    protected:
        void run(int);
};

CV_GpuMatOpConvertTo::CV_GpuMatOpConvertTo(): CvTest( "GPU-MatOperatorConvertTo", "convertTo" ) {}
CV_GpuMatOpConvertTo::~CV_GpuMatOpConvertTo() {}

void CV_GpuMatOpConvertTo::run( int /* start_from */)
{
    const Size img_size(67, 35);

    const int types[] = {CV_8U, CV_8S, CV_16U, CV_16S, CV_32S, CV_32F, CV_64F};
    const int types_num = sizeof(types) / sizeof(int);

    const char* types_str[] = {"CV_8U", "CV_8S", "CV_16U", "CV_16S", "CV_32S", "CV_32F", "CV_64F"};

    bool passed = true;
    try
    {
        for (int i = 0; i < types_num && passed; ++i)
        {
            for (int j = 0; j < types_num && passed; ++j)
            {
                for (int c = 1; c < 2 && passed; ++c)
                {
                    const int src_type = CV_MAKETYPE(types[i], c);
                    const int dst_type = types[j];
                    const double alpha = (double)rand() / RAND_MAX * 2.0;
                    const double beta = (double)rand() / RAND_MAX * 150.0 - 75;

                    cv::RNG rng(*ts->get_rng());

                    Mat cpumatsrc(img_size, src_type);

                    rng.fill(cpumatsrc, RNG::UNIFORM, Scalar::all(0), Scalar::all(300));

                    GpuMat gpumatsrc(cpumatsrc);
                    Mat cpumatdst;
                    GpuMat gpumatdst;

                    cpumatsrc.convertTo(cpumatdst, dst_type, alpha, beta);
                    gpumatsrc.convertTo(gpumatdst, dst_type, alpha, beta);
                    
                    double r = norm(cpumatdst, gpumatdst, NORM_INF);
                    if (r > 1)
                    {
                        cout << "FAILED: " << "SRC_TYPE=" << types_str[i] << "C" << c << " DST_TYPE=" << types_str[j] << " NORM = " << r << endl;

                        passed = false;
                    }
                }
            }
        }
    }
    catch(cv::Exception& e)
    {
        cout << "ERROR: " << e.err << endl;
    }
    ts->set_failed_test_info(passed ? CvTS::OK : CvTS::FAIL_GENERIC);
}

CV_GpuMatOpConvertTo CV_GpuMatOpConvertTo_test;
