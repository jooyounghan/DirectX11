#define MAX_MS_COUNT    4

#define Gaussian3x3Count 9
#define Gaussian5x5Count 25
#define Gaussian7x7Count 49

float2 GetClampedTextureCoord(uint uiWidth, uint uiHeight, int x, int y)
{
    float xClamped1 = clamp(x, 0.0, uiWidth) / float(uiWidth);
    float yClamped1 = clamp(y, 0.0, uiHeight) / float(uiHeight);

    return float2(xClamped1, yClamped1);
}


static const float Gaussian3x3Kernel[Gaussian3x3Count] =
{
    0.01134374, 0.08381951, 0.01134374,
    0.08381951, 0.61934703, 0.08381951,
    0.01134374, 0.08381951, 0.01134374
};

static const float2 Offsets3x3[Gaussian3x3Count] =
{
    { -1, 1 }, { 0, 1 }, { 1, 1 },
    { -1, 0 }, { 0, 0 }, { 1, 0 },
    { -1, -1 }, { 0, -1 }, { 1, -1 },
};


static const float Gaussian5x5Kernel[Gaussian5x5Count] =
{
   0.00048091, 0.00501119, 0.01094545, 0.00501119, 0.00048091,
   0.00501119, 0.0522178, 0.11405416, 0.0522178, 0.00501119,
   0.01094545, 0.11405416, 0.2491172, 0.11405416, 0.01094545,
   0.00501119, 0.0522178, 0.11405416, 0.0522178, 0.00501119,
   0.00048091, 0.00501119, 0.01094545, 0.00501119, 0.00048091
};

static const float2 Offsets5x5[Gaussian5x5Count] =
{
    { -2, -2 }, { -2, -1 }, { -2, 0 }, { -2, 1 }, { -2, 2 },
    { -1, -2 }, { -1, -1 }, { -1, 0 }, { -1, 1 }, {-1, 2 },
    { 0, -2 }, { 0, -1 }, { 0, 0 }, { 0, 1 }, { 0, 2 },
    { 1, -2 }, { 1, -1 }, { 1, 0 }, { 1, 1 }, { 1, 2 },
    { 2, -2 }, { 2, -1 }, { 2, 0 }, { 2, 1 }, { 2, 2 }
};



static const float GaussianKernel7x7[Gaussian7x7Count] =
{
    1.9651916e-05, 2.3940935e-04, 1.0729582e-03, 1.7690092e-03, 1.0729582e-03, 2.3940935e-04, 1.9651916e-05,
    2.3940935e-04, 2.9166031e-03, 1.3071308e-02, 2.1550942e-02, 1.3071308e-02, 2.9166031e-03, 2.3940935e-04,
    1.0729582e-03, 1.3071308e-02, 5.8581538e-02, 9.6584626e-02, 5.8581538e-02, 1.3071308e-02, 1.0729582e-03,
    1.7690092e-03, 2.1550942e-02, 9.6584626e-02, 1.5924112e-01, 9.6584626e-02, 2.1550942e-02, 1.7690092e-03,
    1.0729582e-03, 1.3071308e-02, 5.8581538e-02, 9.6584626e-02, 5.8581538e-02, 1.3071308e-02, 1.0729582e-03,
    2.3940935e-04, 2.9166031e-03, 1.3071308e-02, 2.1550942e-02, 1.3071308e-02, 2.9166031e-03, 2.3940935e-04,
    1.9651916e-05, 2.3940935e-04, 1.0729582e-03, 1.7690092e-03, 1.0729582e-03, 2.3940935e-04, 1.9651916e-05
};

static const float2 Offsets7x7[Gaussian7x7Count] =
{
    {-3, -3}, {-2, -3}, {-1, -3}, {0, -3}, {1, -3}, {2, -3}, {3, -3},
    {-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2}, {3, -2},
    {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1},
    {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0},
    {-3, 1}, {-2, 1}, {-1, 1}, {0, 1}, {1, 1}, {2, 1}, {3, 1},
    {-3, 2}, {-2, 2}, {-1, 2}, {0, 2}, {1, 2}, {2, 2}, {3, 2},
    {-3, 3}, {-2, 3}, {-1, 3}, {0, 3}, {1, 3}, {2, 3}, {3, 3}
};
